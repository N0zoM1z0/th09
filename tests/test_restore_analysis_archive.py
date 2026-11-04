"""Selective analysis evidence restore is safe and never overwrites local work."""
from pathlib import Path
import hashlib
import importlib.util
import json
import shutil
import subprocess
import tempfile
import unittest

MODULE = Path(__file__).resolve().parents[1]/'scripts/restore-analysis-archive.py'
spec = importlib.util.spec_from_file_location('restore_analysis_archive', MODULE)
restore_module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(restore_module)


def digest(raw: bytes) -> str:
    return hashlib.sha256(raw).hexdigest()


class AnalysisArchiveRestoration(unittest.TestCase):
    def test_path_must_be_json_inside_analyses_without_symlink(self):
        with tempfile.TemporaryDirectory() as temporary:
            root=Path(temporary)
            self.assertEqual(restore_module.safe_path(root,'.analysis/a/b.json'),
                             root/'.analysis/a/b.json')
            for bad in ('/tmp/out.json','../escape.json','.analysis/../../bad.json',
                        'docs/other.json','.analysis/thing.exe'):
                with self.subTest(path=bad), self.assertRaises(ValueError):
                    restore_module.safe_path(root,bad)
            (root/'.analysis').mkdir()
            (root/'outside').mkdir()
            (root/'.analysis'/'linked').symlink_to(root/'outside', target_is_directory=True)
            with self.assertRaises(ValueError):
                restore_module.safe_path(root,'.analysis/linked/b.json')

    @unittest.skipUnless(shutil.which('tar') and shutil.which('zstd'),
                         'tar/zstd are optional local-archive tools')
    def test_exact_receipt_and_member_are_required_before_exclusive_restore(self):
        with tempfile.TemporaryDirectory() as temporary:
            root=Path(temporary)
            member='.analysis/probes/large-output.json'
            target=root/member
            target.parent.mkdir(parents=True)
            payload=b'{"source":"unmodified PE","result":true}\n' * 13
            target.write_bytes(payload)
            folder=root/restore_module.BASE
            folder.mkdir(parents=True)
            archive=root/restore_module.ARCHIVE
            manifest=root/restore_module.MANIFEST
            receipt=root/restore_module.RECEIPT
            subprocess.run(['tar','--create','--file',str(archive),
                            '--use-compress-program=zstd -3',member],
                           cwd=root,check=True,capture_output=True)
            row={'path':member,'bytes':len(payload),'sha256':digest(payload)}
            manifest.write_text(json.dumps({'count':1,'original_bytes':len(payload),
                                            'files':[row]})+'\n')
            receipt.write_text(json.dumps({'count_archived_and_removed':1,
                                           'original_bytes':len(payload),
                                           'archive_sha256':restore_module.file_sha256(archive),
                                           'manifest_sha256':restore_module.file_sha256(manifest)})+'\n')
            target.unlink()
            preview=restore_module.restore(root,[member])
            self.assertEqual((preview['absent_files'],preview['bytes_to_restore']),
                             (1,len(payload)))
            with self.assertRaisesRegex(ValueError,'specify one or more'):
                restore_module.restore(root,[],apply=True)
            with self.assertRaisesRegex(ValueError,'not present'):
                restore_module.restore(root,['.analysis/probes/missing.json'],apply=True)
            restored=restore_module.restore(root,[member],apply=True)
            self.assertEqual(restored['absent_files'],1)
            self.assertEqual(target.read_bytes(),payload)
            already=restore_module.restore(root,[member],apply=True)
            self.assertEqual(already['already_present'],1)
            target.write_bytes(b'operator-edited content')
            with self.assertRaisesRegex(ValueError,'refusing overwrite'):
                restore_module.restore(root,[member],apply=True)
            self.assertEqual(target.read_bytes(),b'operator-edited content')
            # A modified archive is blocked even on preview.
            with archive.open('ab') as stream:stream.write(b'broken archive suffix')
            with self.assertRaisesRegex(ValueError,'do not match'):
                restore_module.restore(root,[])


if __name__=='__main__': unittest.main()

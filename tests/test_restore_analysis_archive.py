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
    def test_bundle_paths_stay_within_generated_evidence_scopes(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            for good in ('build/analysis/probe/candidate.obj', '.analysis/probe/private.toml',
                         '.analysis/probe/result.json'):
                self.assertEqual(restore_module.safe_path(root, good, bundle=True), root/good)
            for bad in ('src/game.obj', 'build/game.exe', '.analysis/project.gbf',
                        '.analysis/a/../result.json', './build/a.obj', 'build/a.obj/'):
                with self.subTest(path=bad), self.assertRaises(ValueError):
                    restore_module.safe_path(root, bad, bundle=True)

    @unittest.skipUnless(shutil.which('tar') and shutil.which('zstd'),
                         'tar/zstd are optional local-archive tools')
    def test_bundle_prefix_restore_preflights_every_selected_member(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            bundle = Path('.analysis/handoff-archive-20261106')
            folder = root/bundle
            folder.mkdir(parents=True)
            payloads = {'build/analysis/probe/a.obj': b'object bytes',
                        '.analysis/probe/private.toml': b'[private]\nvalue = 1\n',
                        '.analysis/probe/result.json': b'{"result": true}\n'}
            rows = []
            for member, payload in payloads.items():
                target = root/member
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(payload)
                rows.append({'path': member, 'bytes': len(payload), 'sha256': digest(payload)})
            archive = folder/'generated-evidence.tar.zst'
            subprocess.run(['tar', '--create', '--file', str(archive),
                            '--use-compress-program=zstd -3', *payloads],
                           cwd=root, check=True, capture_output=True)
            manifest = folder/'manifest.json'
            receipt = folder/'receipt.json'
            def seal():
                total = sum(row['bytes'] for row in rows)
                manifest.write_text(json.dumps({'count': len(rows), 'original_bytes': total,
                                                'files': rows}))
                receipt.write_text(json.dumps({'bundle_kind': 'generated-evidence-v1',
                    'count_archived_and_removed': len(rows), 'original_bytes': total,
                    'archive_sha256': restore_module.file_sha256(archive),
                    'manifest_sha256': restore_module.file_sha256(manifest)}))
            seal()
            for member in payloads:
                (root/member).unlink()
            preview = restore_module.restore(root, [], bundle=bundle,
                                             prefixes=['.analysis/probe/'])
            self.assertEqual(preview['absent_files'], 2)
            self.assertFalse((root/'.analysis/probe/result.json').exists())
            # Even a self-consistent receipt cannot authorize corrupt member content.
            rows[1]['sha256'] = '0'*64
            seal()
            with self.assertRaisesRegex(ValueError, 'member'):
                restore_module.restore(root, [], bundle=bundle,
                                       prefixes=['.analysis/probe'], apply=True)
            self.assertFalse((root/'.analysis/probe/result.json').exists())
            rows[1]['sha256'] = digest(payloads[rows[1]['path']])
            seal()
            restored = restore_module.restore(root, [], bundle=bundle,
                prefixes=['.analysis/probe', 'build/analysis/probe'], apply=True)
            self.assertEqual(restored['absent_files'], 3)
            for member, payload in payloads.items():
                self.assertEqual((root/member).read_bytes(), payload)
            (root/rows[0]['path']).write_bytes(b'local edited object')
            with self.assertRaisesRegex(ValueError, 'refusing overwrite'):
                restore_module.restore(root, [], bundle=bundle,
                                       prefixes=['build/analysis/probe'], apply=True)
            with self.assertRaises(ValueError):
                restore_module.restore(root, [], bundle=Path('build/other'))
            with self.assertRaisesRegex(ValueError, 'no manifest-listed'):
                restore_module.restore(root, [], bundle=bundle, prefixes=['build/missing'])

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

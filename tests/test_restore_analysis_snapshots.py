"""Git/EOL recovery without overwriting candidate work or escaping analysis."""
import hashlib
import importlib.util
from pathlib import Path
import subprocess
import tempfile
import unittest

spec=importlib.util.spec_from_file_location('restore_snapshots',Path(__file__).resolve().parents[1]/'scripts/restore-analysis-snapshots.py')
restore=importlib.util.module_from_spec(spec);spec.loader.exec_module(restore)


class RestoreAnalysisSnapshots(unittest.TestCase):
    def fixture(self, root):
        subprocess.run(['git','init','-q',str(root)],check=True)
        source=b'first\nsecond\n'
        oid=subprocess.check_output(['git','hash-object','-w','--stdin'],input=source,cwd=root,text=False).decode().strip()
        original=b'first\r\nsecond\n'
        row=dict(path='.analysis/old/src/a.cpp',git_blob=oid,crlf_lines=[0],bytes=len(original),sha256=hashlib.sha256(original).hexdigest())
        return row,original

    def test_preview_and_apply_recover_original_bytes(self):
        with tempfile.TemporaryDirectory() as folder:
            root=Path(folder);row,expected=self.fixture(root);doc=dict(copies=[row]);path=root/row['path']
            self.assertEqual(restore.restore(root,doc)['absent_files'],1)
            self.assertFalse(path.exists())
            restore.restore(root,doc,apply=True)
            self.assertEqual(path.read_bytes(),expected)
            self.assertEqual(restore.restore(root,doc,apply=True)['already_present'],1)

    def test_bad_hash_or_later_conflict_cannot_partially_restore(self):
        with tempfile.TemporaryDirectory() as folder:
            root=Path(folder);row,_=self.fixture(root);second=dict(row,path='.analysis/old/src/b.cpp',sha256='0'*64)
            with self.assertRaises(ValueError):restore.restore(root,dict(copies=[row,second]),apply=True)
            self.assertFalse((root/row['path']).exists())
            existing=root/second['path'];existing.parent.mkdir(parents=True);existing.write_bytes(b'NEW CANDIDATE')
            second['sha256']=row['sha256']
            with self.assertRaisesRegex(ValueError,'refusing overwrite'):
                restore.restore(root,dict(copies=[row,second]),apply=True)
            self.assertEqual(existing.read_bytes(),b'NEW CANDIDATE')
            self.assertFalse((root/row['path']).exists())

    def test_reject_escape_symlink_and_invalid_line_map(self):
        with tempfile.TemporaryDirectory() as folder, tempfile.TemporaryDirectory() as outside:
            root=Path(folder);row,_=self.fixture(root)
            for changed in [dict(row,path='../outside.cpp'),dict(row,path='src/a.cpp'),
                            dict(row,crlf_lines=[2]),dict(row,crlf_lines=[0,0])]:
                with self.subTest(changed=changed),self.assertRaises(ValueError):
                    restore.restore(root,dict(copies=[changed]),apply=True)
            (root/'.analysis').symlink_to(Path(outside),target_is_directory=True)
            with self.assertRaisesRegex(ValueError,'symlink'):
                restore.restore(root,dict(copies=[row]),apply=True)
            self.assertFalse((Path(outside)/'old').exists())

    def test_prefix_limits_recovery_and_duplicate_paths_fail(self):
        with tempfile.TemporaryDirectory() as folder:
            root=Path(folder);row,_=self.fixture(root);other=dict(row,path='.analysis/other/a.cpp')
            result=restore.restore(root,dict(copies=[row,other]),prefix='.analysis/old',apply=True)
            self.assertEqual(result['absent_files'],1)
            self.assertFalse((root/other['path']).exists())
            with self.assertRaisesRegex(ValueError,'duplicate'):
                restore.restore(root,dict(copies=[row,row]))

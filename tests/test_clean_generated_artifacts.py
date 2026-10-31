"""Safety gates for TH09's optional ignored-build handoff cleanup."""

import importlib.util
import hashlib
import json
from pathlib import Path
import tempfile
import unittest

spec = importlib.util.spec_from_file_location(
    'clean_generated_artifacts',
    Path(__file__).resolve().parents[1] / 'scripts' / 'clean-generated-artifacts.py')
CLEAN = importlib.util.module_from_spec(spec)
import sys
sys.modules[spec.name] = CLEAN
spec.loader.exec_module(CLEAN)


class GeneratedArtifactCleanupTests(unittest.TestCase):
    def test_dry_run_and_apply_preserve_source_evidence_and_original_target(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            for relative, content in [
                ('build/matching/candidate.obj', b'BUILD'),
                ('build/handwritten_probe.cpp', b'candidate under generated tree'),
                ('.analysis/probe/result.obj', b'OBJ'),
                ('.analysis/probe/result.pdb', b'PDB'),
                ('.analysis/probe/probe.py', b'print(1)'),
                ('.analysis/probe/candidate.cpp', b'int candidate;'),
                ('.analysis/probe/result.json', b'{"exact": false}'),
                ('.analysis/probe/original.idb', b'IDA DATABASE'),
                ('.analysis/probe/original.i64', b'IDA DATABASE 64'),
                ('resources/th09.exe', b'ORIGINAL GAME'),
            ]:
                path = root / relative
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(content)
            plan = CLEAN.inspect(root)
            self.assertEqual(plan.summary()['build_files'], 2)
            self.assertEqual(plan.summary()['analysis_compiler_files'], 1)
            self.assertTrue((root / 'build/matching/candidate.obj').exists())
            summary = CLEAN.apply(plan)
            self.assertEqual(summary['total_files'], 3)
            self.assertFalse((root / 'build').exists())
            self.assertEqual((root / '.analysis/probe/result.obj').read_bytes(), b'OBJ')
            self.assertFalse((root / '.analysis/probe/result.pdb').exists())
            self.assertEqual((root / '.analysis/probe/probe.py').read_bytes(), b'print(1)')
            self.assertEqual((root / '.analysis/probe/result.json').read_bytes(), b'{"exact": false}')
            self.assertEqual((root / '.analysis/probe/original.idb').read_bytes(), b'IDA DATABASE')
            self.assertEqual((root / '.analysis/probe/original.i64').read_bytes(), b'IDA DATABASE 64')
            self.assertEqual((root / 'resources/th09.exe').read_bytes(), b'ORIGINAL GAME')
            self.assertTrue((root / CLEAN.MANIFEST).is_file())
            import json
            audit = root / CLEAN.MANIFEST
            before = audit.read_bytes()
            self.assertEqual(json.loads(before)['summary']['total_files'], 3)
            CLEAN.apply(CLEAN.inspect(root))
            self.assertEqual(audit.read_bytes(), before)  # No-op must not erase audit.
            regenerated = root / 'build/again.obj'
            regenerated.parent.mkdir(parents=True)
            regenerated.write_bytes(b'NEXT')
            CLEAN.apply(CLEAN.inspect(root))
            after = json.loads(audit.read_text())
            self.assertEqual(after['summary']['total_files'], 4)
            self.assertEqual(len(after['runs']), 2)
            self.assertFalse(regenerated.exists())

    def test_reject_symlink_roots_and_outside_manifest(self):
        with tempfile.TemporaryDirectory() as temporary, tempfile.TemporaryDirectory() as outside:
            root = Path(temporary)
            (root / '.analysis').symlink_to(Path(outside), target_is_directory=True)
            with self.assertRaisesRegex(ValueError, 'symlink cache root'):
                CLEAN.inspect(root)
            (root / '.analysis').unlink()
            plan = CLEAN.inspect(root)
            with self.assertRaisesRegex(ValueError, 'ignored .analysis'):
                CLEAN.apply(plan, Path('../outside.json'))
            self.assertFalse((Path(outside) / 'generated-cache-manifest.json').exists())

    def test_safely_ignores_symlink_compiler_file(self):
        with tempfile.TemporaryDirectory() as temporary, tempfile.TemporaryDirectory() as outside:
            root = Path(temporary)
            d = root / '.analysis/candidate'
            d.mkdir(parents=True)
            item = Path(outside) / 'protected.obj'
            item.write_bytes(b'KEEP')
            (d / 'linked.obj').symlink_to(item)
            plan = CLEAN.inspect(root)
            self.assertEqual(plan.summary()['analysis_compiler_files'], 0)
            CLEAN.apply(plan)
            self.assertEqual(item.read_bytes(), b'KEEP')

    def test_only_hash_reviewed_objects_are_deleted(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            folder = root / '.analysis/probe'
            folder.mkdir(parents=True)
            approved, pending = folder / 'completed.obj', folder / 'candidate.obj'
            approved.write_bytes(b'DONE')
            pending.write_bytes(b'PENDING')
            manifest = folder / 'review.json'
            manifest.write_text(json.dumps({'objects': [dict(path='.analysis/probe/completed.obj',
                sha256=hashlib.sha256(b'DONE').hexdigest())]}))
            plan = CLEAN.inspect(root, object_review=manifest)
            self.assertEqual(plan.analysis_files, (approved,))
            CLEAN.apply(plan)
            self.assertFalse(approved.exists())
            self.assertEqual(pending.read_bytes(), b'PENDING')

    def test_invalid_review_cannot_select_outside_wrong_hash_or_duplicate(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            folder = root / '.analysis/probe'
            folder.mkdir(parents=True)
            item = folder / 'candidate.obj'
            item.write_bytes(b'KEEP')
            manifest = folder / 'review.json'
            valid = dict(path='.analysis/probe/candidate.obj', sha256=hashlib.sha256(b'KEEP').hexdigest())
            for rows in ([dict(valid, path='../outside.obj')], [dict(valid, path='src/a.obj')],
                         [dict(valid, sha256='0'*64)], [valid, valid],
                         [dict(valid, path='.analysis/probe/original.idb')]):
                manifest.write_text(json.dumps({'objects': rows}))
                with self.subTest(rows=rows), self.assertRaises(ValueError):
                    CLEAN.inspect(root, object_review=manifest)
                self.assertEqual(item.read_bytes(), b'KEEP')

    def test_changed_reviewed_file_aborts_before_any_delete(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            build = root / 'build/first.obj'
            build.parent.mkdir()
            build.write_bytes(b'BUILD')
            item = root / '.analysis/probe/result.pdb'
            item.parent.mkdir(parents=True)
            item.write_bytes(b'OLD')
            plan = CLEAN.inspect(root)
            item.write_bytes(b'NEW')
            with self.assertRaisesRegex(ValueError, 'changed after inspection'):
                CLEAN.apply(plan)
            self.assertEqual(build.read_bytes(), b'BUILD')
            self.assertEqual(item.read_bytes(), b'NEW')

    def test_parent_symlink_swap_cannot_remove_external_file(self):
        with tempfile.TemporaryDirectory() as temporary, tempfile.TemporaryDirectory() as outside:
            root = Path(temporary)
            folder = root / '.analysis/probe'
            folder.mkdir(parents=True)
            (folder / 'result.pdb').write_bytes(b'KEEP')
            plan = CLEAN.inspect(root)
            folder.rename(folder.with_name('original'))
            external = Path(outside) / 'result.pdb'
            external.write_bytes(b'KEEP')
            folder.symlink_to(Path(outside), target_is_directory=True)
            with self.assertRaisesRegex(ValueError, 'symlink cache parent'):
                CLEAN.apply(plan)
            self.assertEqual(external.read_bytes(), b'KEEP')

    def test_new_build_file_or_invalid_audit_aborts_before_deletion(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            item = root / 'build/result.obj'
            item.parent.mkdir()
            item.write_bytes(b'KEEP')
            plan = CLEAN.inspect(root)
            added = root / 'build/new.obj'
            added.write_bytes(b'NEW')
            with self.assertRaisesRegex(ValueError, 'build tree changed'):
                CLEAN.apply(plan)
            added.unlink()
            audit = root / CLEAN.MANIFEST
            audit.parent.mkdir(parents=True)
            audit.write_text('{"runs": ["invalid"]}')
            with self.assertRaisesRegex(ValueError, 'invalid earlier'):
                CLEAN.apply(plan)
            self.assertEqual(item.read_bytes(), b'KEEP')


if __name__ == '__main__':
    unittest.main()

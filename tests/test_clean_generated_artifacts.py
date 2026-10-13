"""Safety gates for TH09's optional ignored-build handoff cleanup."""

import importlib.util
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
                ('resources/th09.exe', b'ORIGINAL GAME'),
            ]:
                path = root / relative
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(content)
            plan = CLEAN.inspect(root)
            self.assertEqual(plan.summary()['build_files'], 2)
            self.assertEqual(plan.summary()['analysis_compiler_files'], 2)
            self.assertTrue((root / 'build/matching/candidate.obj').exists())
            summary = CLEAN.apply(plan)
            self.assertEqual(summary['total_files'], 4)
            self.assertFalse((root / 'build').exists())
            self.assertFalse((root / '.analysis/probe/result.obj').exists())
            self.assertFalse((root / '.analysis/probe/result.pdb').exists())
            self.assertEqual((root / '.analysis/probe/probe.py').read_bytes(), b'print(1)')
            self.assertEqual((root / '.analysis/probe/result.json').read_bytes(), b'{"exact": false}')
            self.assertEqual((root / 'resources/th09.exe').read_bytes(), b'ORIGINAL GAME')
            self.assertTrue((root / CLEAN.MANIFEST).is_file())
            import json
            audit = root / CLEAN.MANIFEST
            before = audit.read_bytes()
            self.assertEqual(json.loads(before)['summary']['total_files'], 4)
            CLEAN.apply(CLEAN.inspect(root))
            self.assertEqual(audit.read_bytes(), before)  # No-op must not erase audit.
            regenerated = root / 'build/again.obj'
            regenerated.parent.mkdir(parents=True)
            regenerated.write_bytes(b'NEXT')
            CLEAN.apply(CLEAN.inspect(root))
            after = json.loads(audit.read_text())
            self.assertEqual(after['summary']['total_files'], 5)
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


if __name__ == '__main__':
    unittest.main()

"""Cold-build failures must never inherit an older successful probe receipt."""
import copy
import importlib.util
import json
from pathlib import Path
import subprocess
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import Mock, patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'screen_context_probe_test', ROOT/'scripts/probe-screen-shake-context.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


class ScreenContextColdReceiptTests(unittest.TestCase):
    def setUp(self):
        (ROOT/'.analysis').mkdir(exist_ok=True)
        self.temporary = tempfile.TemporaryDirectory(dir=ROOT/'.analysis')
        self.addCleanup(self.temporary.cleanup)
        self.dest = Path(self.temporary.name)/'trial'
        self.dest.mkdir()
        (self.dest/'ScreenEffect.obj').write_bytes(b'old object')
        (self.dest/'result.json').write_text('{"diagnostic": {"complete_bytes_agree": true}}')
        self.target = SimpleNamespace(verified_target=Mock(return_value=b'attested target'))

    def test_wrong_target_leaves_outputs_and_never_invokes_compiler(self):
        self.target.verified_target.side_effect = ValueError('wrong target')
        with patch.object(probe, 'module', return_value=self.target), \
                patch.object(probe.subprocess, 'run') as compile_, \
                self.assertRaisesRegex(ValueError, 'wrong target'):
            probe.run_variant(self.dest, 'new source', list(probe.PROFILE))
        compile_.assert_not_called()
        self.assertEqual((self.dest/'ScreenEffect.obj').read_bytes(), b'old object')

    def test_failed_compile_removes_stale_object_and_success_receipt(self):
        def failure(*args, **kwargs):
            self.assertFalse((self.dest/'ScreenEffect.obj').exists())
            self.assertFalse((self.dest/'result.json').exists())
            return subprocess.CompletedProcess(args[0], 9, '', 'compiler failed')
        with patch.object(probe, 'module', return_value=self.target), \
                patch.object(probe.subprocess, 'run', side_effect=failure):
            row = probe.run_variant(self.dest, 'new source', list(probe.PROFILE))
        self.assertEqual(row['compile_exit'], 9)
        self.assertIn('error', row)
        self.assertNotIn('diagnostic', row)
        self.assertEqual(json.loads((self.dest/'result.json').read_text()), row)

    @staticmethod
    def compile_object(command, **kwargs):
        Path(command[2]).write_bytes(b'new object')
        return subprocess.CompletedProcess(command, 0, '', '')

    def test_field_identity_rejection_cannot_restore_an_old_success(self):
        audit = SimpleNamespace(diagnose=Mock(side_effect=ValueError('changed field order')))
        with patch.object(probe, 'module', side_effect=[self.target, audit]), \
                patch.object(probe.subprocess, 'run', side_effect=self.compile_object):
            row = probe.run_variant(self.dest, 'new source', list(probe.PROFILE))
        self.assertEqual(row['error'], 'changed field order')
        self.assertNotIn('diagnostic', row)
        self.assertEqual(json.loads((self.dest/'result.json').read_text()), row)

    def test_zero_differences_remain_noncrediting(self):
        audit = SimpleNamespace(diagnose=Mock(return_value={
            'complete_bytes_agree': True, 'overlapping_differences': 0}))
        with patch.object(probe, 'module', side_effect=[self.target, audit]), \
                patch.object(probe.subprocess, 'run', side_effect=self.compile_object):
            row = probe.run_variant(self.dest, 'new source', list(probe.PROFILE))
        self.assertTrue(row['diagnostic_only'])
        self.assertEqual(row['exactness_credit'], 'none')

    def test_changed_canonical_reference_fails_before_build(self):
        unit = {'source': 'src/ScreenEffect.cpp', 'symbol': '?CalcShake@ScreenEffect@@SIHPAU1@@Z',
                'target_address': 0x422660, 'size': 316, 'profile': list(probe.PROFILE)}
        for key, bad in [('source', 'src/Other.cpp'), ('symbol', '?Different@@YAHXZ'),
                         ('target_address', 0x422661), ('size', 315), ('profile', ['/Ob0'])]:
            changed = copy.deepcopy(unit)
            changed[key] = bad
            with self.subTest(key=key), patch.object(probe.tomllib, 'load', return_value={
                    'units': {'screen-effect-calc-shake': changed}}), self.assertRaises(ValueError):
                probe.profile()


if __name__ == '__main__':
    unittest.main()

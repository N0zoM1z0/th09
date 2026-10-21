"""Failed/changed inputs must not leave a successful Timer context proof."""
import copy
import hashlib
import importlib.util
from pathlib import Path
import tempfile
import tomllib
from types import SimpleNamespace
import unittest
from unittest.mock import Mock, patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'runecl_setter_context', ROOT/'scripts/probe-runecl-setter-context.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


class RunEclSetterContextTests(unittest.TestCase):
    def setUp(self):
        self.manifest = tomllib.loads((ROOT/'config/match-units.toml').read_text())
        self.profile = self.manifest['units']['ecl-pop-context']['profile']

    def test_native_callees_keep_real_source_abi_profile_extent_and_fields(self):
        probe.require_native_contract(self.manifest['units'], self.profile)
        for name in probe.CALLEES:
            for change in ({'source': 'src/EclManager.cpp'},
                           {'object': 'build/unknown.obj'}, {'symbol': '?Alias@@QAEXH@Z'},
                           {'target_address': 0x400000}, {'size': 24},
                           {'relocations': [{'offset': 0, 'type': 'DIR32'}]},
                           {'profile': ['/O1']}):
                units = copy.deepcopy(self.manifest['units'])
                units[name].update(change)
                with self.subTest(name=name, change=change), self.assertRaises(ValueError):
                    probe.require_native_contract(units, self.profile)

    def test_wrong_target_preserves_old_evidence_without_output_or_compiler_work(self):
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory)/'old.json'
            output.write_text('previous evidence')
            wrong = SimpleNamespace(verified_target=Mock(side_effect=ValueError('wrong target')))
            with (patch.object(probe, 'load', return_value=wrong),
                  patch.object(probe, 'capture_inputs') as capture,
                  patch.object(probe.subprocess, 'run') as compiler,
                  patch.object(probe.tempfile, 'TemporaryDirectory') as temporary):
                with self.assertRaisesRegex(ValueError, 'wrong target'):
                    probe.run('pair-before', output)
            self.assertEqual(output.read_text(), 'previous evidence')
            capture.assert_not_called()
            compiler.assert_not_called()
            temporary.assert_not_called()

    def run_failed_input(self, *, changed=False, staged=False):
        # Tests need no private game input or installed compiler/site packages.
        fake_image = b'target-independent-input'
        manifest = {**self.manifest, 'target_sha256': hashlib.sha256(fake_image).hexdigest()}
        fake_coff = SimpleNamespace(verified_target=Mock(return_value=fake_image),
                                    object_function=Mock(side_effect=AssertionError('stale COFF used')))
        original_load = probe.load
        sources, pins = probe.capture_inputs()
        if changed:
            pins['src/EclManager.cpp'] = '0'*64
        def compile_result(args, **kwargs):
            if staged:
                Path(args[4]).write_text('changed staged source')
            return SimpleNamespace(returncode=0 if changed or staged else 1,
                                   stdout='', stderr='compiler failed')
        with tempfile.TemporaryDirectory(prefix='setter-test-', dir=ROOT/'.analysis') as directory:
            output = Path(directory)/'old.json'
            output.write_text('previous successful output')
            with (patch.object(probe, 'load', side_effect=lambda name:
                               fake_coff if name == 'compare-coff-function.py' else original_load(name)),
                  patch.object(probe.tomllib, 'loads', return_value=manifest),
                  patch.object(probe, 'capture_inputs', return_value=(sources, pins)),
                  patch.object(probe.subprocess, 'run', side_effect=compile_result) as compiler):
                message = 'staged.*changed' if staged else 'inputs changed' if changed else 'compiler failed'
                with self.assertRaisesRegex(ValueError, message):
                    probe.run('baseline', output)
            compiler.assert_called_once()
            fake_coff.object_function.assert_not_called()
            self.assertFalse(output.exists())
            self.assertEqual(list(Path(directory).iterdir()), [])

    def test_failed_compiler_clears_stale_receipt_and_self_cleans_carrier(self):
        self.run_failed_input()

    def test_changed_source_fingerprint_prevents_stale_coff_analysis(self):
        self.run_failed_input(changed=True)

    def test_changed_actual_staged_source_prevents_coff_analysis(self):
        self.run_failed_input(staged=True)


if __name__ == '__main__':
    unittest.main()

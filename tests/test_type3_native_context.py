"""Fail-closed source/build provenance for the Type3 visibility experiment."""
import importlib.util
from pathlib import Path
import tempfile
import tomllib
from types import SimpleNamespace
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'native_type3_producer', ROOT / 'scripts/probe-type3-native-context.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


class Type3NativeContextTests(unittest.TestCase):
    def setUp(self):
        unit = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units'][
            'ex-attack-initialize-type3']
        self.profile = unit['profile']
        self.inputs = {probe.SOURCE: b'fixture\n'}
        self.diagnostic = {'candidate_bytes': 780, 'bound_fields': 38,
                           'overlap_differences': 15, 'missing_target_bytes': 0}

    def modules(self, wrong_target=False):
        def target():
            if wrong_target:
                raise ValueError('wrong target fixture')
            return b'attested fixture'
        modules = {
            'compare-coff-function.py': SimpleNamespace(
                verified_target=target, object_function=lambda *a, **k: (bytes(780), [])),
            'inspect-exattack-type3-schedule.py': SimpleNamespace(
                SYMBOL='fixture', context_report=lambda _: self.diagnostic,
                analyze=lambda _: self.diagnostic),
            'check-boss-frontiers.py': SimpleNamespace(
                guard_exattack_type3_update=lambda _: None),
        }
        return lambda _, filename: modules[filename]

    def test_failed_compiler_removes_stale_result_and_both_temporary_carriers(self):
        binaries = []
        def failure(command, **kwargs):
            binaries.append(Path(command[5]).parent)
            return SimpleNamespace(returncode=1, stdout='', stderr='fixture failure')
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            report = output / 'type3-context-baseline.json.gz'
            report.write_bytes(b'stale success')
            with (patch.object(probe, 'module', side_effect=self.modules()),
                  patch.object(probe, 'inputs_for', return_value=(self.inputs, self.profile)),
                  patch.object(probe.subprocess, 'run', side_effect=failure)):
                with self.assertRaisesRegex(RuntimeError, 'compiler failed'):
                    probe.run(output, 'baseline')
            self.assertEqual(list(output.iterdir()), [])
            self.assertTrue(binaries)
            self.assertTrue(all(not p.exists() for p in binaries))

    def test_one_selected_case_binds_actual_compile_and_cleans_outputs(self):
        commands = []
        def compile_one(command, **kwargs):
            commands.append(command)
            Path(command[5]).write_bytes(b'fixture object')
            return SimpleNamespace(returncode=0, stdout='', stderr='')
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            with (patch.object(probe, 'module', side_effect=self.modules()),
                  patch.object(probe, 'inputs_for', return_value=(self.inputs, self.profile)),
                  patch.object(probe.subprocess, 'run', side_effect=compile_one),
                  patch.object(probe.subprocess, 'check_output', return_value='fixture\n')):
                report = probe.run(output, 'collision')
            self.assertEqual(len(commands), 1)
            self.assertEqual(commands[0][:3], ['nice', '-n', '10'])
            self.assertEqual(commands[0][6:], self.profile)
            self.assertEqual(report['case'], 'collision')
            self.assertEqual(report['source_inputs'].keys(), self.inputs.keys())
            self.assertEqual(report['exactness_credit'], 'none')
            self.assertFalse(report['compiler_artifacts_retained'])
            self.assertEqual(list(output.iterdir()), [])
            self.assertFalse(Path(commands[0][5]).parent.exists())

    def test_wrong_target_does_not_invalidate_existing_evidence(self):
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            path = output / 'type3-context-baseline.json.gz'
            path.write_bytes(b'earlier evidence')
            with (patch.object(probe, 'module', side_effect=self.modules(wrong_target=True)),
                  patch.object(probe.subprocess, 'run') as compiler):
                with self.assertRaisesRegex(ValueError, 'wrong target'):
                    probe.run(output, 'baseline')
            compiler.assert_not_called()
            self.assertEqual(path.read_bytes(), b'earlier evidence')

    def test_source_change_during_compile_cannot_leave_a_successful_report(self):
        def compile_one(command, **kwargs):
            Path(command[5]).write_bytes(b'fixture object')
            return SimpleNamespace(returncode=0, stdout='', stderr='')
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            with (patch.object(probe, 'module', side_effect=self.modules()),
                  patch.object(probe, 'inputs_for', side_effect=[
                      (self.inputs, self.profile), ({probe.SOURCE: b'changed'}, self.profile)]),
                  patch.object(probe.subprocess, 'run', side_effect=compile_one),
                  patch.object(probe.subprocess, 'check_output', return_value='fixture\n')):
                with self.assertRaisesRegex(ValueError, 'inputs changed'):
                    probe.run(output, 'baseline')
            self.assertEqual(list(output.iterdir()), [])


if __name__ == '__main__':
    unittest.main()

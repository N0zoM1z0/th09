"""Fail-closed source/build provenance for the Type3 visibility experiment."""
import copy
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
    def test_pointer_context_requires_both_real_alias_units_without_extent_drift(self):
        inputs, profile = probe.inputs_for('pointer-views')
        units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        probe.require_pointer_view_units(units, profile)
        source = probe.render_source(inputs, 'pointer-views', 'baseline')
        self.assertTrue(source.startswith(b'#include "VectorPointerViews.cpp"\n'))
        self.assertEqual(source.split(b'\n', 1)[1], inputs[probe.SOURCE])
        for name, _ in probe.POINTER_VIEW_UNITS:
            for changes in ({'source': 'src/unrelated.cpp'}, {'size': 6},
                            {'target_address': 0x4343D1}, {'profile': ['/Ob2']},
                            {'symbol': '?FakeIdentity@@QAEPAMXZ'},
                            {'relocations': [{'offset': 0, 'type': 'DIR32'}]}):
                bad = copy.deepcopy(units)
                bad[name].update(changes)
                with self.subTest(name=name, changes=changes):
                    with self.assertRaisesRegex(ValueError, 'pointer-view alias'):
                        probe.require_pointer_view_units(bad, profile)

    def test_bad_emitted_pointer_body_cannot_leave_a_report(self):
        factory = self.modules()

        def modules(name, filename):
            result = factory(name, filename)
            if filename == 'compare-coff-function.py':
                result.pe_bytes_at = lambda *args: bytes.fromhex('8bc1c3')
                result.object_function = lambda obj, symbol, **kw: (
                    (bytes.fromhex('33c0c3'), []) if symbol.startswith('??B')
                    else (bytes(780), []))
            return result

        def compile_one(command, **kwargs):
            Path(command[5]).write_bytes(b'fixture object')
            return SimpleNamespace(returncode=0, stdout='', stderr='')

        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            stale = output / probe.report_name('pointer-views', 'baseline')
            stale.write_bytes(b'stale successful result')
            with (patch.object(probe, 'module', side_effect=modules),
                  patch.object(probe, 'inputs_for', return_value=(self.inputs, self.profile)),
                  patch.object(probe, 'render_source', return_value=b'fixture caller'),
                  patch.object(probe.subprocess, 'run', side_effect=compile_one)):
                with self.assertRaisesRegex(ValueError, 'pointer view lost'):
                    probe.run(output, 'pointer-views')
            self.assertEqual(list(output.iterdir()), [])

    def test_joint_model_keeps_real_caller_and_only_the_native_interrupt_body(self):
        inputs, profile = probe.inputs_for('interrupt')
        before = copy.deepcopy(inputs)
        maintained = inputs[probe.SOURCE]
        self.assertEqual(probe.render_source(inputs, 'baseline', 'baseline'), maintained)
        joint = probe.render_source(inputs, 'interrupt', 'forwarded-collision')
        self.assertIn(b'ReadType3TailWord', joint)
        self.assertIn(b'CheckType3Position', joint)
        self.assertIn(b'typedef short i16;', joint)
        self.assertEqual(joint.count(b'void AnmVm::SetInterrupt(i16 interrupt)'), 1)
        self.assertNotIn(b'#include "AnmManagerRuntime.cpp"', joint)
        self.assertNotIn(b'struct AnmVm\n', joint)
        self.assertEqual(inputs, before)
        changed = copy.deepcopy(inputs)
        changed['AnmManagerRuntime.cpp'] = changed['AnmManagerRuntime.cpp'].replace(
            b'pendingInterrupt = interrupt;', b'pendingInterrupt = 0;')
        with self.assertRaisesRegex(ValueError, 'interrupt source changed'):
            probe.render_source(changed, 'interrupt', 'forwarded-collision')
        unit = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units'][
            'anm-vm-set-interrupt']
        probe.require_interrupt_unit(unit, profile)
        for changes in ({'source': 'src/unrelated.cpp'}, {'size': 17},
                        {'target_address': 0x406791}, {'profile': ['/Ob2']},
                        {'symbol': '?SetInterrupt@Unrelated@@QAEXF@Z'}):
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                probe.require_interrupt_unit({**unit, **changes}, profile)

    def test_rejected_joint_model_keeps_its_guard_failure_and_separate_report(self):
        modules = self.modules()
        def imports(name, filename):
            result = modules(name, filename)
            if filename == 'check-boss-frontiers.py':
                result.guard_exattack_type3_update = lambda _: (_ for _ in ()).throw(
                    RuntimeError('damaged formerly matching bytes'))
            return result
        def compile_one(command, **kwargs):
            Path(command[5]).write_bytes(b'fixture object')
            return SimpleNamespace(returncode=0, stdout='', stderr='')
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            baseline = output/probe.report_name('baseline', 'baseline')
            baseline.write_bytes(b'earlier maintained proof')
            with (patch.object(probe, 'module', side_effect=imports),
                  patch.object(probe, 'inputs_for', return_value=(self.inputs, self.profile)),
                  patch.object(probe, 'render_source', return_value=b'joint fixture'),
                  patch.object(probe.subprocess, 'run', side_effect=compile_one),
                  patch.object(probe.subprocess, 'check_output', return_value='fixture\n')):
                report = probe.run(output, 'baseline', 'forwarded-collision')
            self.assertEqual(report['exactness_credit'], 'none')
            self.assertFalse(report['production_frontier']['passed'])
            self.assertIn('damaged formerly matching', report['production_frontier']['reason'])
            self.assertEqual(report['source_variant'], 'forwarded-collision')
            self.assertEqual(baseline.read_bytes(), b'earlier maintained proof')
            self.assertNotEqual(probe.report_name('baseline', 'baseline'),
                                probe.report_name('baseline', 'forwarded-collision'))

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

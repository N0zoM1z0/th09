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
    def test_coordinate_helpers_preserve_sample_indices_and_lazy_queries(self):
        inputs, _ = probe.inputs_for('baseline')
        source = inputs[probe.SOURCE].decode()
        for case, count in (('coordinate-indexed', 3), ('coordinate-middle', 1)):
            result = probe.coordinate_samples(source, case)
            self.assertEqual(result.count('    ConvertType3Sample('), count)
            self.assertIn('ConvertType3Sample(middle, extra->history10, 12);', result)
            self.assertEqual(result.count('CheckBulletCollision('), 3)
            self.assertEqual(result.count(' == 2 ||'), 2)
            self.assertNotIn('__forceinline', result)
            self.assertNotIn('volatile', result)
            self.assertIn('out.z = 0.0f;', result)
            if count == 1:
                self.assertIn('InverseTransformPopupX(extra->history10[24].x)', result)
            with self.assertRaisesRegex(ValueError, 'unqualified baseline'):
                probe.render_source(inputs, case, 'forwarded-collision')

    def test_coordinate_source_drift_fails_before_compilation(self):
        inputs, _ = probe.inputs_for('baseline')
        source = inputs[probe.SOURCE].decode()
        for bad in (source+source, source.replace('history10[12].y', 'history10[13].y'),
                    source.replace('middle.z = 0.0f;', 'middle.z = 1.0f;')):
            with self.assertRaises(ValueError):
                probe.coordinate_samples(bad, 'coordinate-indexed')

    def test_history_samples_reject_changed_indices_and_preserve_native_queries(self):
        inputs, _ = probe.inputs_for('sample-cursor')
        source = inputs[probe.SOURCE].decode()
        for case in probe.HISTORY_CASES:
            result = probe.history_access(source, case)
            self.assertEqual(result.count('->CheckBulletCollision('), 3)
            self.assertEqual(result.count('InverseTransformPopupX('), 3)
            self.assertEqual(result.count('InverseTransformPopupY('), 3)
            self.assertNotIn('volatile', result)
            self.assertNotIn('__forceinline', result)
        for case in ('sample-cursor', 'sample-references'):
            with self.subTest(case=case), self.assertRaises(ValueError):
                probe.history_access(source.replace('history10[24].x', 'history10[25].x'), case)
        with self.assertRaises(ValueError):
            probe.history_access(source, 'unknown-history')

    def test_sample_containers_preserve_native_calls_and_reject_source_drift(self):
        inputs, profile = probe.inputs_for('sample-array')
        source = inputs[probe.SOURCE].decode()
        for case in ('sample-array', 'sample-query'):
            with self.subTest(case=case):
                staged = probe.compiled_inputs(inputs, case, 'baseline')
                self.assertEqual(set(staged), set(inputs))
                self.assertEqual({name for name in staged if staged[name] != inputs[name]},
                                 {probe.SOURCE})
                transformed = staged[probe.SOURCE].decode()
                self.assertEqual(transformed.count('->CheckBulletCollision('), 3)
                self.assertEqual(transformed.count('InverseTransformPopupX('), 3)
                self.assertEqual(transformed.count('InverseTransformPopupY('), 3)
                self.assertNotIn('__forceinline', transformed)
                self.assertNotIn('volatile', transformed)
                for mutation in ('    PlayerPositionView current;',
                                 '    PlayerPositionView *collision ='):
                    with self.assertRaisesRegex(ValueError, 'actual Type3'):
                        probe.sample_container(source.replace(mutation, 'changed source'), case)
        with self.assertRaises(ValueError):
            probe.sample_container(source, 'invented-container')

    def test_sample_guard_failure_cannot_become_exactness_credit(self):
        inputs, profile = probe.inputs_for('sample-array')
        factory = self.modules()
        def imports(name, filename):
            result = factory(name, filename)
            if filename == 'inspect-exattack-type3-schedule.py':
                def reject(_):
                    raise ValueError('fixed-home census changed')
                result.analyze = reject
            return result
        def compiler(command, **kwargs):
            Path(command[5]).write_bytes(b'fixture object')
            return SimpleNamespace(returncode=0, stdout='', stderr='')
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            earlier = output/probe.report_name('baseline', 'baseline')
            earlier.write_bytes(b'earlier maintained evidence')
            with (patch.object(probe, 'module', side_effect=imports),
                  patch.object(probe, 'inputs_for', return_value=(inputs, profile)),
                  patch.object(probe.subprocess, 'run', side_effect=compiler),
                  patch.object(probe.subprocess, 'check_output', return_value='fixture\n')):
                report = probe.run(output, 'sample-array')
            self.assertFalse(report['production_frontier']['passed'])
            self.assertEqual(report['exactness_credit'], 'none')
            self.assertIn('fixed-home census', report['production_frontier']['reason'])
            self.assertFalse(report['compiler_artifacts_retained'])
            self.assertEqual(list(output.iterdir()), [earlier])

    def test_timer_reset_context_keeps_real_bodies_and_manifest_profiles(self):
        inputs, profile = probe.inputs_for('timer-all')
        units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        for case, count in zip(probe.TIMER_BODY_CASES, (1, 2, 17)):
            with self.subTest(case=case):
                selected = probe.timer_body_units(case, units, profile)
                self.assertEqual(len(selected), count)
                source = probe.render_source(inputs, case, 'baseline')
                self.assertTrue(source.endswith(inputs[probe.SOURCE]))
                self.assertEqual(source.count(probe.TIMER_ASSIGNMENT.encode()), 1)
                self.assertEqual(source.count(probe.TIMER_SETTER.encode()), int(count > 1))
                self.assertNotIn(b'__forceinline', source)
                self.assertNotIn(b'volatile', source)
                changed = dict(inputs)
                changed['ZunTimer.cpp'] = changed['ZunTimer.cpp'].replace(
                    b'this->SetCurrent(value);', b'this->SetCurrent(0);')
                with self.assertRaisesRegex(ValueError, 'reset definitions changed'):
                    probe.render_source(changed, case, 'baseline')
                bad = copy.deepcopy(units)
                bad['zun-timer-assign-int']['profile'] = ['/Ob2']
                with self.assertRaisesRegex(ValueError, 'source/profile changed'):
                    probe.timer_body_units(case, bad, profile)

    def test_native_timer_proof_rejects_code_field_and_literal_corruption(self):
        # A real read-only scalar section, with a separate four-byte code body.
        import struct
        data = bytearray(64)
        struct.pack_into('<HH', data, 0, 0x14c, 1)
        struct.pack_into('<II', data, 36, 4, 60)
        struct.pack_into('<I', data, 56, 0x40000040)
        struct.pack_into('<I', data, 60, 0x3f800000)
        unit = dict(symbol='fixture', target_address=0x401000, size=4,
                    relocations=[dict(offset=0, type='DIR32',
                                      symbol='__real@3f800000', target=0x402000)])
        row = dict(offset=0, type='DIR32', symbol='__real@3f800000', addend=0,
                   symbol_section=1, symbol_value=0)
        original = (0x402000).to_bytes(4, 'little')
        coff = SimpleNamespace(object_function=lambda *a, **k: (bytes(4), [row]),
                               pe_bytes_at=lambda _, address, size:
                               original if address == 0x401000 else bytes(data[60:64]))
        with tempfile.TemporaryDirectory() as temporary:
            obj = Path(temporary)/'fixture.obj'
            obj.write_bytes(data)
            self.assertTrue(probe.prove_timer_bodies(obj, {'fixture': unit}, b'', coff)[0][
                'complete_original_equal'])
            for raw, rows in ((bytes(3), [row]), (bytes(5), [row]), (bytes(4), []),
                              (bytes(4), [{**row, 'addend': 1}]),
                              (bytes(4), [{**row, 'symbol': 'fake'}]),
                              (bytes(4), [{**row, 'offset': 1}]),
                              (bytes(4), [{**row, 'type': 'REL32'}])):
                with self.subTest(raw=raw, rows=rows):
                    coff.object_function = lambda *a, raw=raw, rows=rows, **k: (raw, rows)
                    with self.assertRaises(ValueError):
                        probe.prove_timer_bodies(obj, {'fixture': unit}, b'', coff)
            coff.object_function = lambda *a, **k: (bytes(4), [row])
            coff.pe_bytes_at = lambda _, address, size: bytes(4)
            with self.assertRaisesRegex(ValueError, 'complete original equality'):
                probe.prove_timer_bodies(obj, {'fixture': unit}, b'', coff)
            coff.pe_bytes_at = lambda _, address, size: (
                original if address == 0x401000 else struct.pack('<I', 0x3f800000))
            data[60] ^= 1
            obj.write_bytes(data)
            with self.assertRaisesRegex(ValueError, 'literal definition differs'):
                probe.prove_timer_bodies(obj, {'fixture': unit}, b'', coff)

    def test_plain_timer_context_pins_native_owner_and_actual_header(self):
        inputs, profile = probe.inputs_for('timer-plain')
        unit = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units'][
            'zun-timer-greater-equal']
        probe.require_timer_compare_unit(unit, profile)
        staged = probe.compiled_inputs(inputs, 'timer-plain', 'baseline')
        self.assertEqual(staged[probe.SOURCE], inputs[probe.SOURCE])
        self.assertEqual({name for name in staged if staged[name] != inputs[name]},
                         {'ZunTimerGreaterEqual.inl'})
        self.assertNotIn(b'__declspec', staged['ZunTimerGreaterEqual.inl'])
        self.assertNotIn(b'inline', staged['ZunTimerGreaterEqual.inl'])
        for change in ({'source': 'src/unrelated.cpp'}, {'object': 'build/fake.obj'},
                       {'profile': profile}, {'symbol': '?FakeCompare@@QAEIH@Z'},
                       {'size': 19}, {'target_address': 0x401521},
                       {'relocations': [{'offset': 0, 'type': 'DIR32'}]}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                probe.require_timer_compare_unit({**unit, **change}, profile)

    def test_wrong_plain_timer_body_preserves_other_reports_and_cleans(self):
        inputs, profile = probe.inputs_for('timer-plain')
        factory = self.modules()
        def modules(name, filename):
            value = factory(name, filename)
            if filename == 'compare-coff-function.py':
                value.pe_bytes_at = lambda *args: bytes(20)
                value.object_function = lambda obj, symbol, **kw: (
                    (bytes([1])*20, []) if symbol == probe.TIMER_COMPARE_SYMBOL
                    else (bytes(780), []))
            return value
        def compiler(command, **kwargs):
            Path(command[5]).write_bytes(b'fixture object')
            return SimpleNamespace(returncode=0, stdout='', stderr='')
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            other = output/probe.report_name('baseline', 'baseline')
            other.write_bytes(b'other evidence')
            stale = output/probe.report_name('timer-plain', 'baseline')
            stale.write_bytes(b'stale success')
            with (patch.object(probe, 'module', side_effect=modules),
                  patch.object(probe, 'inputs_for', return_value=(inputs, profile)),
                  patch.object(probe.subprocess, 'run', side_effect=compiler)):
                with self.assertRaisesRegex(ValueError, 'Timer comparison lost'):
                    probe.run(output, 'timer-plain')
            self.assertEqual(other.read_bytes(), b'other evidence')
            self.assertEqual(list(output.iterdir()), [other])

    def test_changed_actual_staged_header_cannot_leave_a_success(self):
        inputs, profile = probe.inputs_for('timer-plain')
        def compiler(command, **kwargs):
            Path(command[5]).write_bytes(b'fixture object')
            (Path(command[4]).parent/'ZunTimerGreaterEqual.inl').write_bytes(b'changed header')
            return SimpleNamespace(returncode=0, stdout='', stderr='')
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            stale = output/probe.report_name('timer-plain', 'baseline')
            stale.write_bytes(b'stale success')
            with (patch.object(probe, 'module', side_effect=self.modules()),
                  patch.object(probe, 'inputs_for', return_value=(inputs, profile)),
                  patch.object(probe.subprocess, 'run', side_effect=compiler)):
                with self.assertRaisesRegex(ValueError, 'actual compiled.*inputs changed'):
                    probe.run(output, 'timer-plain')
            self.assertEqual(list(output.iterdir()), [])

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

"""Source drift and failed compiles must not become successful diagnostic receipts."""
import contextlib
import importlib.util
import io
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'runecl_raw_context_test', ROOT/'scripts/probe-runecl-raw-context.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


def sources():
    control = ('namespace Th09EclRunControl\n{\n'+probe.RAW_INT+
               '    case TH09_ECL_OPCODE_JUMP:\n'
               '    {\n        int value;\n'
               '        memcpy(&value, &Th09EclRunControl::RawInt(instruction, 0), sizeof(value));\n        '+probe.TIMER+'\n'
               '        timer = value;\n    }\n'
               '        instruction = reinterpret_cast<Th09EclRawInstructionHeaderView *>(\n'
               '            reinterpret_cast<unsigned char *>(instruction) +\n'
               '            Th09EclRunControl::RawInt(instruction, 1));\n'
               '    case TH09_ECL_OPCODE_SET_INT:\n')
    state = probe.RAW_BYTE+'auto b = Th09EclRunState::RawByte(instruction, 0);\n'
    consumer = ('auto a = Th09EclRunControl::RawInt(cursor(), index());\n'
                'auto b = Th09EclRunState::RawByte(cursor(), offset());\n'
                'Other::RawInt(x, y); object.RawInt(x, y); pointer->RawByte(x, y);\n')
    return {'EclRunControl.inl':control.encode(), 'EclRunState.inl':state.encode(),
            'consumer.cpp':consumer.encode()}


class RunEclRawContextTests(unittest.TestCase):
    def test_macro_recipes_are_selective_and_do_not_mutate_live_inputs(self):
        inputs = sources()
        before = dict(inputs)
        for case in ('raw-int-macro', 'raw-byte-macro', 'raw-family-macros'):
            changed = probe.render(case, inputs)
            combined = {**inputs, **changed}
            consumer = combined['consumer.cpp'].decode()
            self.assertIn('Other::RawInt(x, y); object.RawInt(x, y); pointer->RawByte(x, y);',
                          consumer)
            self.assertEqual(consumer.count('cursor()'), 2)
            self.assertEqual(consumer.count('index()'), 1)
            self.assertEqual(inputs, before)
        self.assertEqual(probe.render('baseline', inputs), {})
        with self.assertRaises(ValueError):
            probe.render('unknown', inputs)

    def test_changed_accessor_width_address_or_definition_count_is_rejected(self):
        for filename, old, new, case in (
                ('EclRunControl.inl', 'int *', 'short *', 'raw-int-macro'),
                ('EclRunState.inl', '0x0C', '0x10', 'raw-byte-macro')):
            inputs = sources()
            inputs[filename] = inputs[filename].replace(old.encode(), new.encode())
            with self.subTest(case=case), self.assertRaises(ValueError):
                probe.render(case, inputs)
        inputs = sources()
        inputs['EclRunControl.inl'] += probe.RAW_INT.encode()
        with self.assertRaises(ValueError):
            probe.render('raw-int-macro', inputs)

    def test_typed_jump_keeps_scalar_copy_and_native_timer(self):
        inputs = sources()
        text = probe.render('jump-typed-wire', inputs)['EclRunControl.inl'].decode()
        self.assertNotIn('volatile', text)
        self.assertIn('memcpy(&value, &wire.time0C, sizeof(value));', text)
        self.assertIn(probe.DIRECT_TIMER, text)
        self.assertNotIn(probe.TIMER, text)
        inputs['EclRunControl.inl'] += probe.TIMER.encode()
        with self.assertRaises(ValueError):
            probe.render('jump-native-time', inputs)

    def test_scalar_return_contexts_preserve_width_and_unselected_copy(self):
        inputs = probe.collect_inputs()
        before = dict(inputs)
        for case in ('wire-template-jump', 'wire-template-byte',
                     'wire-template-both', 'wire-fixed-both', 'wire-reference-both'):
            changed = probe.render(case, inputs)
            with self.subTest(case=case):
                self.assertEqual(inputs, before)
                self.assertEqual(set(changed), {'EclRunControl.inl'} if
                                 case == 'wire-template-jump' else
                                 {'EclRunControl.inl', 'EclRunState.inl'})
                self.assertIn(probe.TIMER, changed['EclRunControl.inl'].decode())
                self.assertIn('memcpy(&value, ', changed['EclRunControl.inl'].decode())
                self.assertIn('sizeof(value)', changed['EclRunControl.inl'].decode())
        byte_only = probe.render('wire-template-byte', inputs)
        self.assertIn('memcpy(&value, &Th09EclRunControl::RawInt(instruction, 0), sizeof(value));',
                      byte_only['EclRunControl.inl'].decode())
        self.assertIn('ReadWireScalar<unsigned char>(&Th09EclRunState::RawByte(instruction, 0))',
                      byte_only['EclRunState.inl'].decode())

    def test_scalar_return_context_rejects_changed_width_size_and_duplicate_copy(self):
        for filename, old, new in (
                ('EclRunControl.inl', 'reinterpret_cast<int *>', 'reinterpret_cast<short *>'),
                ('EclRunState.inl', '0x0C + byteOffset', '0x10 + byteOffset'),
                ('EclRunState.inl', 'sizeof(value));', 'sizeof(int));')):
            inputs = probe.collect_inputs()
            self.assertIn(old.encode(), inputs[filename])
            inputs[filename] = inputs[filename].replace(old.encode(), new.encode())
            with self.subTest(filename=filename, old=old), self.assertRaises(ValueError):
                probe.render('wire-template-both', inputs)
        inputs = probe.collect_inputs()
        inputs['EclRunControl.inl'] += (b'int value;\n        memcpy(&value, '
            b'&Th09EclRunControl::RawInt(instruction, 0), sizeof(value));')
        with self.assertRaises(ValueError):
            probe.render('wire-template-jump', inputs)

    def test_project_include_escape_is_rejected(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            (root/'src').mkdir()
            (root/'src/EclManager.cpp').write_text('#include "../outside.hpp"\n')
            with patch.object(probe, 'ROOT', root), self.assertRaisesRegex(ValueError, 'escapes'):
                probe.collect_inputs()

    def test_target_failure_invalidates_old_success_and_preserves_new_failure_evidence(self):
        with tempfile.TemporaryDirectory(dir=ROOT/'.analysis') as td:
            out = Path(td)
            path = out/'baseline.json'
            argv = ['probe', '--output', str(out)]
            path.write_text(json.dumps(dict(state='complete-diagnostic', stale=True)))
            with patch.object(probe.sys, 'argv', argv), \
                    patch.object(probe, 'run', side_effect=ValueError('wrong target')), \
                    contextlib.redirect_stderr(io.StringIO()):
                self.assertEqual(probe.main(), 2)
            result = json.loads(path.read_text())
            self.assertEqual(result['state'], 'rejected')
            self.assertNotIn('stale', result)

            def failed_compile(*args):
                path.write_text(json.dumps(dict(state='rejected', compile_exit=1,
                                                compile_log='captured failure')))
                raise ValueError('compiler rejected source')

            with patch.object(probe.sys, 'argv', argv), \
                    patch.object(probe, 'run', side_effect=failed_compile), \
                    contextlib.redirect_stderr(io.StringIO()):
                self.assertEqual(probe.main(), 2)
            self.assertEqual(json.loads(path.read_text())['compile_log'], 'captured failure')

    def test_symlinked_report_is_rejected_before_any_target_access(self):
        with tempfile.TemporaryDirectory(dir=ROOT/'.analysis') as td:
            out = Path(td)
            protected = out/'protected'
            protected.write_text('preserve')
            (out/'baseline.json').symlink_to(protected)
            with patch.object(probe.sys, 'argv', ['probe', '--output', str(out)]), \
                    patch.object(probe, 'run') as run, \
                    contextlib.redirect_stderr(io.StringIO()), self.assertRaises(SystemExit):
                probe.main()
            run.assert_not_called()
            self.assertEqual(protected.read_text(), 'preserve')

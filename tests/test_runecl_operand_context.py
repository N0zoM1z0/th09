"""Guard diagnostic expression expansion against incomplete source rewrites."""
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
def load(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module

probe = load('operand_context', 'probe-runecl-operand-context.py')
provider = load('operand_provider', 'probe-runecl-raw-context.py')

class OperandContextTests(unittest.TestCase):
    def test_native_assignment_retains_the_same_reviewed_storage_owner(self):
        for owner in ('enemy->', 'state.'):
            source = (owner+'primaryFlags337C =\n '
                      'Th09EclRunState::AssignTimeoutSpellBit(\n '+
                      owner+'primaryFlags337C, value);')
            self.assertEqual(probe.native_timeout_assignment(source),
                             owner+'primaryFlagBits337C.timeoutSpell24 = value;')

    def test_native_assignment_rejects_missing_duplicate_or_mixed_storage(self):
        source = ('enemy->primaryFlags337C = '
                  'Th09EclRunState::AssignTimeoutSpellBit(enemy->primaryFlags337C, value);')
        for bad in ('', source+source, source.replace('enemy->', 'other->'),
                    source.replace('(enemy->', '(state.'),
                    'other_'+source, 'globals::'+source):
            with self.subTest(source=bad), self.assertRaisesRegex(ValueError, 'timeout flag assignment'):
                probe.native_timeout_assignment(bad)

    def test_both_native_cases_render_current_inputs_before_the_compiler(self):
        inputs = provider.collect_inputs()
        for case in ('operand-macros-native-timeout', 'operand-macros-native-plain'):
            with self.subTest(case=case):
                result = probe.render(case, inputs)
                state = result['EclRunState.inl'].decode()
                start = state.index('case TH09_ECL_OPCODE_SET_TIMEOUT_SPELL:')
                block = state[start:state.index('break;', start)]
                self.assertIn('enemy->primaryFlagBits337C.timeoutSpell24 = value;', block)
                self.assertNotIn('AssignTimeoutSpellBit(', block)
                self.assertIn('memcpy(&value,', block)

    def test_current_calls_are_fully_expanded(self):
        result = probe.render('operand-macros', provider.collect_inputs())
        self.assertIn(b'TH09_ECL_READ_FLOAT(enemy, instruction, 1)', result['EclManager.cpp'])
        self.assertNotIn(b'ReadFloat(enemy, instruction,', result['EclManager.cpp'])

    def test_side_effecting_extra_call_is_rejected(self):
        inputs = provider.collect_inputs()
        inputs['EclManager.cpp'] += b'\nReadInt(enemy++, instruction, 0);\n'
        with self.assertRaisesRegex(ValueError, 'unreviewed operand call'):
            probe.render('operand-macros', inputs)

    def test_changed_wrapper_semantics_are_rejected(self):
        inputs = provider.collect_inputs()
        inputs['EclRunControl.inl'] = inputs['EclRunControl.inl'].replace(
            b'1U << operandIndex', b'2U << operandIndex')
        with self.assertRaisesRegex(ValueError, 'wrapper bodies changed'):
            probe.render('operand-macros', inputs)

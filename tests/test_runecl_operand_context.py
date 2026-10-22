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

"""Fail-closed, target-independent x86 reviewed switch-CFG tests."""
import importlib.util
from pathlib import Path
from types import SimpleNamespace as NS
import unittest

SOURCE = Path(__file__).resolve().parents[1] / 'scripts/diagnostic_switch_graph.py'
spec = importlib.util.spec_from_file_location('reviewed_switch_graph', SOURCE)
graph = importlib.util.module_from_spec(spec)
spec.loader.exec_module(graph)

IMM, MEM = 2, 3


def ins(address, mnemonic, *, destination=None, operand='', return_bytes=None):
    if destination is not None:
        ops = [NS(type=IMM, imm=destination)]
    elif return_bytes is not None:
        ops = [NS(type=IMM, imm=return_bytes)]
    elif operand:
        ops = [NS(type=MEM)]
    else:
        ops = []
    return NS(address=address, mnemonic=mnemonic, operands=ops, op_str=operand)


class SwitchGraphTests(unittest.TestCase):
    def setUp(self):
        self.instructions = [
            ins(0x1000, 'jmp', operand='dword ptr [ecx*4 + 0x2000]'),
            ins(0x1007, 'call', destination=0x405760),
            ins(0x100C, 'ret', return_bytes=4),
            ins(0x100F, 'ret'),
        ]

    def build(self, table=(0x1007, 0x100F)):
        return graph.switch_graph(self.instructions, IMM, 0x1000,
                                  'dword ptr [ecx*4 + 0x2000]', table)

    def test_all_table_destinations_and_physical_order(self):
        value = self.build()
        self.assertEqual(len(value), 3)
        self.assertEqual(value[0][:2], ('jmp', (1, 2)))
        self.assertEqual(value[1][2], (0x405760,))
        self.assertEqual(value[1][3], (4,))
        self.assertEqual(value[2][3], ())

    def test_duplicate_table_destination_retained_not_silently_deduped(self):
        value = self.build((0x1007, 0x1007, 0x100F, 0x100F))
        self.assertEqual(value[0][1], (1, 1, 2, 2))

    def test_table_target_must_be_instruction_boundary(self):
        with self.assertRaisesRegex(ValueError, 'table points outside'):
            self.build((0x1008, 0x100F))

    def test_alternate_dispatch_operand_is_not_a_valid_switch(self):
        self.instructions[0] = ins(0x1000, 'jmp', operand='dword ptr [edx*4 + 0x2000]')
        with self.assertRaisesRegex(ValueError, 'unexpected indirect jump'):
            self.build()

    def test_other_indirect_call_fails_closed(self):
        self.instructions[1] = ins(0x1007, 'call', operand='eax')
        with self.assertRaisesRegex(ValueError, 'indirect call'):
            self.build()

    def test_missing_dispatch_fails_closed(self):
        with self.assertRaisesRegex(ValueError, 'missing or duplicated'):
            graph.switch_graph([ins(0x1000, 'ret')], IMM, 0x1000,
                               'dword ptr [ecx*4 + 0x2000]', (0x1000,))

    def test_changed_branch_target_fails_closed(self):
        self.instructions.insert(2, ins(0x100A, 'jne', destination=0x5000))
        with self.assertRaisesRegex(ValueError, 'direct branch outside'):
            self.build()

    def test_unreviewed_exit_fails_closed(self):
        self.instructions[2] = ins(0x100C, 'retf')
        with self.assertRaisesRegex(ValueError, 'unreviewed control transfer'):
            self.build()


if __name__ == '__main__':
    unittest.main()

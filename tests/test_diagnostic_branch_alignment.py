"""Target-independent tests: similarity must not hide a wrong jump exit."""
from difflib import Match
import importlib.util
from pathlib import Path
from types import SimpleNamespace as Record
import unittest


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    'branch_diagnostic', ROOT / 'scripts/diagnostic_branch_alignment.py'
)
DIAGNOSTIC = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(DIAGNOSTIC)


def instruction(address, mnemonic, destination=0, operand_type=1):
    return Record(address=address, mnemonic=mnemonic,
                  operands=[Record(type=operand_type, imm=destination)])


class BranchAlignmentTests(unittest.TestCase):
    def test_shifted_addresses_keep_same_logical_destination(self):
        target = [instruction(100, 'jmp', 120), instruction(110, 'inc'), instruction(120, 'ret')]
        candidate = [instruction(200, 'jmp', 218), instruction(208, 'inc'), instruction(218, 'ret')]
        self.assertEqual(DIAGNOSTIC.aligned_branch_conflicts(
            target, candidate, [Match(0, 0, 3)], 1), ([], 0))

    def test_wrong_exit_is_not_hidden_by_identical_mnemonics(self):
        target = [instruction(100, 'jmp', 120), instruction(110, 'inc'), instruction(120, 'ret')]
        candidate = [instruction(200, 'jmp', 208), instruction(208, 'inc'), instruction(218, 'ret')]
        self.assertEqual(DIAGNOSTIC.aligned_branch_conflicts(
            target, candidate, [Match(0, 0, 3)], 1), ([(100, 120, 110)], 0))

    def test_unpaired_destination_is_explicitly_incomplete(self):
        target = [instruction(100, 'je', 120)]
        candidate = [instruction(200, 'je', 218)]
        self.assertEqual(DIAGNOSTIC.aligned_branch_conflicts(
            target, candidate, [Match(0, 0, 1)], 1), ([], 1))

    def test_indirect_branch_is_not_claimed_as_direct(self):
        target = [instruction(100, 'jmp', operand_type=2)]
        candidate = [instruction(200, 'jmp', operand_type=2)]
        self.assertEqual(DIAGNOSTIC.aligned_branch_conflicts(
            target, candidate, [Match(0, 0, 1)], 1), ([], 0))


class DirectControlFlowTests(unittest.TestCase):
    def graph(self, instructions):
        return DIAGNOSTIC.direct_control_flow(instructions, 1)

    def test_extra_noncontrol_instruction_does_not_drop_edges(self):
        target = [instruction(100, 'je', 130), instruction(110, 'call', 900),
                  instruction(120, 'jmp', 140), instruction(130, 'inc'), instruction(140, 'ret')]
        candidate = [instruction(200, 'xor'), instruction(210, 'je', 250),
                     instruction(220, 'call', 900), instruction(230, 'mov'),
                     instruction(240, 'jmp', 260), instruction(250, 'inc'), instruction(260, 'ret')]
        self.assertEqual(self.graph(target), self.graph(candidate))

    def test_identical_branch_mnemonics_do_not_hide_wrong_exit(self):
        target = [instruction(100, 'je', 130), instruction(110, 'call', 900),
                  instruction(120, 'jmp', 140), instruction(130, 'inc'), instruction(140, 'ret')]
        wrong = [instruction(100, 'je', 130), instruction(110, 'call', 900),
                 instruction(120, 'jmp', 130), instruction(130, 'inc'), instruction(140, 'ret')]
        self.assertNotEqual(self.graph(target), self.graph(wrong))

    def test_same_whole_call_order_does_not_hide_moved_call(self):
        target = [instruction(100, 'je', 130), instruction(110, 'call', 900),
                  instruction(120, 'jmp', 140), instruction(130, 'inc'), instruction(140, 'ret')]
        wrong = [instruction(100, 'call', 900), instruction(110, 'je', 130),
                 instruction(120, 'jmp', 140), instruction(130, 'inc'), instruction(140, 'ret')]
        self.assertNotEqual(self.graph(target), self.graph(wrong))

    def test_indirect_jump_fails_closed(self):
        with self.assertRaisesRegex(ValueError, 'non-direct'):
            self.graph([instruction(100, 'jmp', 110, operand_type=2), instruction(110, 'ret')])

    def test_indirect_call_fails_closed(self):
        with self.assertRaisesRegex(ValueError, 'non-direct'):
            self.graph([instruction(100, 'call', 900, operand_type=2), instruction(110, 'ret')])

    def test_destination_outside_decoded_stream_fails_closed(self):
        with self.assertRaisesRegex(ValueError, 'outside'):
            self.graph([instruction(100, 'jmp', 999), instruction(110, 'ret')])

    def test_missing_conditional_fallthrough_fails_closed(self):
        with self.assertRaisesRegex(ValueError, 'fallthrough'):
            self.graph([instruction(100, 'jne', 100)])

    def test_return_stack_cleanup_is_not_ignored(self):
        self.assertNotEqual(self.graph([instruction(100, 'ret', 4)]),
                            self.graph([instruction(200, 'ret', 8)]))


if __name__ == '__main__':
    unittest.main()

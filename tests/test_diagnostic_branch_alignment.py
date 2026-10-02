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


if __name__ == '__main__':
    unittest.main()

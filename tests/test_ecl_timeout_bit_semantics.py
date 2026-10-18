"""Target-independent counterexamples for RunEcl opcode-155 machine semantics.

The live PE/COFF/Capstone and source cold build remain mandatory in the real
opcode audit. These pure tests exercise its whitelist and two-register flows
without treating mathematical equivalence as byte-exact codegen.
"""

import copy
import importlib.util
from pathlib import Path
from types import SimpleNamespace
import unittest

ROOT = Path(__file__).resolve().parents[1]
BOSS_SPEC = importlib.util.spec_from_file_location(
    'run_ecl_semantic_guard', ROOT/'scripts/check-boss-frontiers.py')
BOSS = importlib.util.module_from_spec(BOSS_SPEC)
BOSS_SPEC.loader.exec_module(BOSS)
SPEC = importlib.util.spec_from_file_location(
    'ecl_timeout_bit_semantics', ROOT/'scripts/audit-ecl-timeout-bit.py')
AUDIT = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(AUDIT)


def instructions(*lines):
    return [SimpleNamespace(mnemonic=operation, op_str=operands)
            for operation, operands in lines]


ORIGINAL = instructions(
    ('movzx', 'ecx, byte ptr [ebx + 0xc]'),
    ('mov', 'esi, dword ptr [edi + 0x337c]'),
    ('shl', 'ecx, 0x18'),
    ('xor', 'ecx, esi'),
    ('and', 'ecx, 0x1000000'),
    ('mov', 'eax, esi'),
    ('xor', 'eax, ecx'),
    ('mov', 'dword ptr [edi + 0x337c], eax'),
)
CANDIDATE = instructions(
    ('mov', 'cl, byte ptr [ebx + 0xc]'),
    ('mov', 'eax, dword ptr [edi + 0x337c]'),
    ('movzx', 'ecx, cl'),
    ('shl', 'ecx, 0x18'),
    ('xor', 'ecx, eax'),
    ('and', 'ecx, 0x1000000'),
    ('xor', 'ecx, eax'),
    ('mov', 'dword ptr [edi + 0x337c], ecx'),
)


class Opcode155TargetSemantics(unittest.TestCase):
    def test_both_reviewed_machine_sequences_preserve_all_other_flag_bits(self):
        self.assertEqual(AUDIT.validate_transition(AUDIT.prefix(ORIGINAL),
                                                   AUDIT.prefix(CANDIDATE)), 3072)

    def test_wire_odd_even_sets_only_timeout_flag_bit_24(self):
        opcode = AUDIT.prefix(ORIGINAL)
        previous = 0xFEDCBA98
        for wire in (0, 1, 2, 3, 0xFE, 0xFF):
            with self.subTest(wire=wire):
                actual = AUDIT.emulate(opcode, previous, wire)
                expected = (previous & ~AUDIT.FLAG_MASK) | ((wire & 1) << 24)
                self.assertEqual(actual, expected)

    def test_unencodable_byte_to_dword_mov_is_rejected(self):
        # MOV ECX, CL is not an encodable i386 instruction. The bit-24
        # masking *would* make its hypothetical low bit semantically equal;
        # reject the impossible machine instruction, not the algebra.
        bad = copy.deepcopy(CANDIDATE)
        bad[2].mnemonic = 'mov'
        with self.assertRaisesRegex(ValueError, 'operand widths'):
            AUDIT.prefix(bad)

    def test_mask_change_to_another_bit_is_rejected(self):
        bad = copy.deepcopy(CANDIDATE)
        bad[5].op_str = 'ecx, 0x2000000'
        with self.assertRaisesRegex(ValueError, 'flag behavior differs'):
            AUDIT.validate_transition(AUDIT.prefix(ORIGINAL), AUDIT.prefix(bad))

    def test_xor_replacement_is_rejected_fail_closed(self):
        bad = copy.deepcopy(CANDIDATE)
        bad[6].mnemonic = 'or'
        with self.assertRaisesRegex(ValueError, 'unknown opcode'):
            AUDIT.prefix(bad)

    def test_extra_write_and_wrong_member_fail_closed(self):
        for source in ('dword ptr [edi + 0x3380]',
                       'byte ptr [edi + 0x337c]'):
            with self.subTest(source=source):
                bad = copy.deepcopy(ORIGINAL)
                bad[-1].op_str = source + ', eax'
                with self.assertRaisesRegex(ValueError, 'unreviewed'):
                    AUDIT.prefix(bad)

    def test_unreviewed_instruction_is_not_silently_skipped(self):
        bad = copy.deepcopy(ORIGINAL)
        bad[0].mnemonic = 'bswap'
        with self.assertRaisesRegex(ValueError, 'unknown opcode'):
            AUDIT.prefix(bad)

    def test_incomplete_prefix_fails_closed(self):
        with self.assertRaisesRegex(ValueError, 'flag store'):
            AUDIT.prefix(ORIGINAL[:-1])


class Opcode155CompleteOwnerGate(unittest.TestCase):
    @staticmethod
    def evidence():
        return dict(diagnostic_only=True, exactness_credit='none',
                    full_owner_original_sha256=BOSS.RUNECL_TARGET_OWNER_SHA,
                    full_owner_ordinary_differences=30,
                    flag_transition_equal=True, behavior_cases=3072,
                    wire_values_tested=256, flags_tested=12,
                    original_opcode_va='0x40b770', opcode_bytes=45,
                    original_instructions=12, candidate_instructions=12,
                    original_flag_prefix_instructions=8,
                    candidate_flag_prefix_instructions=8)

    def test_valid_full_owner_gate_is_noncrediting(self):
        report = BOSS.guard_runecl_timeout_semantics(self.evidence(), ordinary_differences=30)
        self.assertIs(report['flag_transition_equal'], True)
        self.assertEqual(report['behavior_cases'], 3072)
        self.assertEqual(report['exactness_credit'], 'none')

    def test_corrupt_original_count_bytes_instruction_or_behavior_fails(self):
        original = self.evidence()
        for key, value in (
            ('diagnostic_only', False), ('exactness_credit', 'accepted'),
            ('full_owner_original_sha256', '0' * 64),
            ('full_owner_ordinary_differences', 29),
            ('flag_transition_equal', False), ('behavior_cases', 3071),
            ('wire_values_tested', 255), ('flags_tested', 11),
            ('original_opcode_va', '0x40b771'), ('opcode_bytes', 44),
            ('original_instructions', 11), ('candidate_instructions', 11),
            ('original_flag_prefix_instructions', 7),
            ('candidate_flag_prefix_instructions', 7)):
            with self.subTest(key=key), self.assertRaises(RuntimeError):
                BOSS.guard_runecl_timeout_semantics({**original, key: value},
                                                    ordinary_differences=30)
        with self.assertRaises(RuntimeError):
            BOSS.guard_runecl_timeout_semantics({}, ordinary_differences=30)

    def test_strictly_binds_semantics_to_current_full_owner_residual(self):
        report = self.evidence()
        report['full_owner_ordinary_differences'] = 29
        self.assertEqual(BOSS.guard_runecl_timeout_semantics(
            report, ordinary_differences=29)['exactness_credit'], 'none')
        with self.assertRaises(RuntimeError):
            BOSS.guard_runecl_timeout_semantics(report, ordinary_differences=30)


if __name__ == '__main__':
    unittest.main()

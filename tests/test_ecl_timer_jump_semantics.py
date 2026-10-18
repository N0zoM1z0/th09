"""Pure-input safety checks for real TH09 RunEcl opcode-4 jump timer.

Original and compiler candidate differ by seven ordinary machine bytes. The
full 29-byte handler must call the same genuine timer assignment and advance
the same instruction pointer. Neither this nor arithmetic equivalence credits
byte-exact reconstruction.
"""

import copy
import importlib.util
from pathlib import Path
from types import SimpleNamespace
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    'opcode4_source_cold', ROOT/'scripts/audit-ecl-timer-jump.py')
AUDIT = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(AUDIT)
SPEC = importlib.util.spec_from_file_location(
    'opcode4_full_owner_guard', ROOT/'scripts/check-boss-frontiers.py')
BOSS = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(BOSS)


def assembly(*items):
    return [SimpleNamespace(mnemonic=mnemonic, op_str=operands)
            for mnemonic, operands in items]


ORIGINAL = assembly(
    ('mov', 'ecx, dword ptr [ebx + 0xc]'),
    ('push', 'ecx'),
    ('mov', 'ecx, dword ptr [edi + 0x2ce0]'),
    ('add', 'ecx, 8'),
    ('call', '0x401500'),
    ('add', 'ebx, dword ptr [ebx + 0x10]'),
    ('mov', 'dword ptr [ebp - 8], ebx'),
    ('jmp', '0x408750'),
)
CANDIDATE = assembly(
    ('mov', 'eax, dword ptr [ebx + 0xc]'),
    ('mov', 'ecx, dword ptr [edi + 0x2ce0]'),
    ('push', 'eax'),
    ('add', 'ecx, 8'),
    ('call', '0x401500'),
    ('add', 'ebx, dword ptr [ebx + 0x10]'),
    ('mov', 'dword ptr [ebp - 8], ebx'),
    ('jmp', '0x408750'),
)


class Opcode4MachineSemantics(unittest.TestCase):
    def test_target_and_candidate_preserve_timer_and_jump_across_all_cases(self):
        self.assertEqual(AUDIT.validate_transitions(
            AUDIT.parsed_instructions(ORIGINAL),
            AUDIT.parsed_instructions(CANDIDATE)), 90)

    def test_raw_unsigned_argument_and_signed_relative_instruction(self):
        source = AUDIT.parsed_instructions(ORIGINAL)
        for time, delta in ((0x80000000, -12), (-1, -12), (1, 0x7000)):
            with self.subTest(time=time, delta=delta):
                result = AUDIT.execute(source, time=time,
                                       instruction_offset=delta,
                                       context_address=0x40001000)
                self.assertEqual(result, {
                    'call': (0x401500, 0x40001008, time & 0xffffffff),
                    'next_instruction': (0x51001000 + delta) & 0xffffffff,
                    'branch': 0x408750,
                })

    def test_wrong_timer_or_redispatch_fail_closed(self):
        for index, address in ((4, '0x4014d0'), (7, '0x408751')):
            broken = copy.deepcopy(CANDIDATE)
            broken[index].op_str = address
            with self.subTest(index=index), self.assertRaisesRegex(ValueError, 'incorrect|redispatch'):
                AUDIT.parsed_instructions(broken)

    def test_wrong_timer_receiver_or_operand_fails(self):
        for index, operand in ((1, 'ecx, dword ptr [edi + 0x2ce4]'),
                               (3, 'ecx, 4'),
                               (0, 'eax, dword ptr [ebx + 0x10]')):
            broken = copy.deepcopy(CANDIDATE)
            broken[index].op_str = operand
            with self.subTest(index=index):
                if index == 1:
                    with self.assertRaisesRegex(ValueError, 'unreviewed|argument count'):
                        AUDIT.parsed_instructions(broken)
                else:
                    with self.assertRaises(ValueError):
                        AUDIT.validate_transitions(
                            AUDIT.parsed_instructions(ORIGINAL),
                            AUDIT.parsed_instructions(broken))

    def test_wrong_raw_time_input_or_stack_argument_fails(self):
        for index, operand in ((0, 'eax, dword ptr [ebx + 0x10]'),
                               (2, 'ecx'), (2, 'ebx')):
            broken = copy.deepcopy(CANDIDATE)
            broken[index].op_str = operand
            with self.subTest(index=index), self.assertRaises(ValueError):
                AUDIT.validate_transitions(AUDIT.parsed_instructions(ORIGINAL),
                                           AUDIT.parsed_instructions(broken))

    def test_post_call_register_residue_cannot_replace_true_ebx_cursor(self):
        # EAX is volatile across the original exact ZunTimer thiscall; its
        # leftover value cannot replace EBX as the published ECL cursor.
        bad = copy.deepcopy(CANDIDATE)
        bad[6].op_str = 'dword ptr [ebp - 8], eax'
        with self.assertRaisesRegex(ValueError, 'different next instruction'):
            AUDIT.validate_transitions(AUDIT.parsed_instructions(ORIGINAL),
                                       AUDIT.parsed_instructions(bad))

    def test_wrong_next_pointer_and_spurious_memory_write_fail(self):
        for index, operand in ((5, 'ebx, dword ptr [ebx + 0xc]'),
                               (6, 'dword ptr [ebx + 0x10], ebx')):
            broken = copy.deepcopy(CANDIDATE)
            broken[index].op_str = operand
            with self.subTest(index=index), self.assertRaises(ValueError):
                AUDIT.validate_transitions(AUDIT.parsed_instructions(ORIGINAL),
                                           AUDIT.parsed_instructions(broken))

    def test_unreviewed_instruction_and_missing_terminator_fail(self):
        broken = copy.deepcopy(CANDIDATE)
        broken[4].mnemonic = 'callf'
        with self.assertRaisesRegex(ValueError, 'unreviewed'):
            AUDIT.parsed_instructions(broken)
        with self.assertRaisesRegex(ValueError, 'eight reviewed'):
            AUDIT.parsed_instructions(CANDIDATE[:-1])


class Opcode4CompleteOwnerGate(unittest.TestCase):
    @staticmethod
    def report():
        return dict(diagnostic_only=True, exactness_credit='none',
                    full_owner_original_sha256=BOSS.RUNECL_TARGET_OWNER_SHA,
                    full_owner_ordinary_differences=30,
                    original_opcode_va='0x408937', opcode_bytes=29,
                    original_instructions=8, candidate_instructions=8,
                    timer_call_va='0x401500', redispatch_va='0x408750',
                    time_values_tested=6, offsets_tested=5,
                    contexts_tested=3, behavior_cases=90,
                    timer_and_advance_equal=True)

    def test_full_owner_semantics_pass_but_never_pretend_exactness(self):
        approved = BOSS.guard_runecl_timer_jump(self.report(), ordinary_differences=30)
        self.assertEqual(approved['behavior_cases'], 90)
        self.assertEqual(approved['exactness_credit'], 'none')

    def test_invalid_census_identity_or_credit_fails_closed(self):
        evidence = self.report()
        for field, value in (
            ('diagnostic_only', False), ('exactness_credit', 'accepted'),
            ('full_owner_original_sha256', '0'*64),
            ('full_owner_ordinary_differences', 29),
            ('original_opcode_va', '0x408938'), ('opcode_bytes', 28),
            ('original_instructions', 7), ('candidate_instructions', 7),
            ('timer_call_va', '0x4014d0'), ('redispatch_va', '0x408751'),
            ('time_values_tested', 5), ('offsets_tested', 4),
            ('contexts_tested', 2), ('behavior_cases', 89),
            ('timer_and_advance_equal', False)):
            with self.subTest(field=field), self.assertRaises(RuntimeError):
                BOSS.guard_runecl_timer_jump({**evidence, field: value},
                                             ordinary_differences=30)
        with self.assertRaises(RuntimeError):
            BOSS.guard_runecl_timer_jump({}, ordinary_differences=30)

    def test_both_real_callee_leaves_use_one_pinned_exact_carrier(self):
        import tomllib
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        assign = units['zun-timer-assign-int']
        setter = units['zun-timer-set-current']
        self.assertEqual((assign['source'], assign['object'], assign['size'],
                          assign['target_address']),
                         ('src/ZunTimer.cpp', 'build/matching/ZunTimer.obj',
                          9, 0x00401500))
        self.assertEqual((setter['source'], setter['object'], setter['size'],
                          setter['target_address']),
                         ('src/ZunTimer.cpp', 'build/matching/ZunTimer.obj',
                          25, 0x004014D0))
        self.assertEqual(assign['profile'], setter['profile'])
        self.assertEqual(assign['relocations'], [
            {'offset': 5, 'type': 'REL32',
             'symbol': '?SetCurrent@ZunTimer@@QAEXH@Z',
             'target': 0x004014D0}])
        self.assertEqual(setter['relocations'], [])

    def test_new_verified_smaller_residual_remains_noncrediting(self):
        report = self.report()
        report['full_owner_ordinary_differences'] = 29
        self.assertEqual(BOSS.guard_runecl_timer_jump(
            report, ordinary_differences=29)['exactness_credit'], 'none')
        with self.assertRaises(RuntimeError):
            BOSS.guard_runecl_timer_jump(report, ordinary_differences=30)


if __name__ == '__main__':
    unittest.main()

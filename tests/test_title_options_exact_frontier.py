"""No-target corruption and reviewed-field guards for Title OnUpdateOptions."""

import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest
from unittest.mock import patch

try:
    import capstone
except ImportError:  # Public CI uses python -S without site packages.
    capstone = None

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'title_options_original_frontier', ROOT / 'scripts/inspect-title-options.py')
check = importlib.util.module_from_spec(spec)
spec.loader.exec_module(check)

spec = importlib.util.spec_from_file_location(
    'title_boss_frontier', ROOT / 'scripts/check-boss-frontiers.py')
boss = importlib.util.module_from_spec(spec)
spec.loader.exec_module(boss)


class OptionsOriginalFields(unittest.TestCase):
    @staticmethod
    def synthetic():
        symbol = '?g_TitleMusicVolume@@3CA'
        address = check.DESTINATIONS[symbol]
        original = [(8 + 8 * i, 'DIR32', address) for i in range(check.ORIGINAL_FIELDS)]
        rows = [{'offset': 12 + 8 * i, 'type': 'DIR32',
                 'symbol': symbol, 'addend': 0}
                for i in range(check.ORIGINAL_FIELDS)]
        return rows, original

    def test_reviewed_extent_hash_and_census_are_independent(self):
        target = bytes(check.SIZE)
        digest = hashlib.sha256(target).hexdigest()
        check.require_original_identity(target, digest)
        self.assertEqual(check.ORIGINAL_FIELDS, 135)
        self.assertEqual(check.ORIGINAL_CALLS, 51)
        self.assertEqual(check.ORIGINAL_BLOCKS, 121)
        with self.assertRaises(ValueError):
            check.require_original_identity(target, '0' * 64)
        with self.assertRaises(ValueError):
            check.require_original_identity(target[:-1], hashlib.sha256(target[:-1]).hexdigest())
        changed = bytearray(target)
        changed[10] = 1
        with self.assertRaises(ValueError):
            check.require_original_identity(bytes(changed), digest)

    def test_all_135_candidate_fields_link_at_actual_coff_offsets(self):
        rows, original = self.synthetic()
        result = check.bind_candidate(bytes(2048), rows, original)
        self.assertEqual(len(result), 2048)
        for row, (_, kind, dest) in zip(rows, original):
            self.assertEqual(struct.unpack_from('<I', result, row['offset'])[0], dest)
        self.assertEqual(len(check.bind_candidate(bytes(2045), rows, original)), 2045)

    def test_missing_reordered_and_fabricated_fields_are_rejected(self):
        rows, original = self.synthetic()
        tests = [rows[:-1], list(reversed(rows)),
                 [dict(rows[0], symbol='?bogus@@3HA')] + rows[1:],
                 [dict(rows[0], addend=4)] + rows[1:],
                 [dict(rows[0], type='REL32')] + rows[1:],
                 rows[:1] + [dict(rows[1], offset=rows[0]['offset'] + 1)] + rows[2:]]
        for broken in tests:
            with self.subTest(broken=broken[:2]), self.assertRaises(ValueError):
                check.bind_candidate(bytes(2048), broken, original)
        with self.assertRaises(ValueError):
            check.bind_candidate(bytes(2048), rows, original[:-1])
        with self.assertRaises(ValueError):
            check.bind_candidate(bytes(1900), rows, original)

    def test_reject_original_field_order_or_dest_change(self):
        rows, original = self.synthetic()
        broken = original[:1] + [(original[1][0], 'DIR32', 0x4A1EC0)] + original[2:]
        with self.assertRaises(ValueError):
            check.bind_candidate(bytes(2048), rows, broken)

    @unittest.skipIf(capstone is None, 'Capstone unavailable under python -S')
    def test_branch_displacement_is_not_an_external_operand(self):
        # MOV ECX,[actual global]; CALL forward; JNZ forward; RET.
        raw = bytes.fromhex('8b0d50c54d00e8000000000f8500000000c3')
        ins = check.full_instructions(raw)
        ops, calls = check.absolute_operands(ins)
        self.assertEqual(ops, [(2, 'DIR32', 0x4DC550),
                               (7, 'REL32', check.BASE + 11)])
        self.assertEqual(calls, [check.BASE + 11])

    @unittest.skipIf(capstone is None, 'Capstone unavailable under python -S')
    def test_wrong_ret_abi_and_indirect_call_rejected(self):
        for raw in (bytes.fromhex('ffd0c3'), bytes.fromhex('c21000')):
            with self.subTest(raw=raw.hex()), self.assertRaises(ValueError):
                ins = check.full_instructions(raw)
                check.absolute_operands(ins)


class OptionsBossFrontierWiring(unittest.TestCase):
    @staticmethod
    def options_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 2045, 'candidate_bytes': 2048,
            'target_instructions': 551, 'candidate_instructions': 554,
            'literal_definitions_checked': 1,
            'original_fields': 135, 'bound_fields': 135,
            'ordered_calls': 51, 'ordered_calls_agree': True,
            'original_blocks': 121, 'candidate_blocks': 121,
            'direct_graphs_agree': True,
            'aligned_instructions': 540, 'paired_branch_conflicts': [],
            'unpaired_branch_destinations': 5,
            'overlap_differences': 610,
            'target_sha256': check.TARGET_SHA256,
            'difference_offsets': sorted(boss.TITLE_OPTIONS_RESIDUAL_OFFSETS),
            'extra_candidate_bytes': 3, 'missing_target_bytes': 0,
            'complete_bytes_equal': False,
        }

    @staticmethod
    def result_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 485, 'candidate_bytes': 484,
            'target_instructions': 147, 'candidate_instructions': 147,
            'original_fields': 34, 'bound_fields': 34,
            'ordered_calls': 9, 'calls_agree': True,
            'original_blocks': 40, 'candidate_blocks': 40,
            'direct_graphs_agree': True,
            'target_sha256': '526beca1d1f7fe3e410002739257bcf486d0c93e6246fae21f6f7902945e1d48',
            'difference_offsets': [
                0x00E, 0x015, 0x0CB, 0x0FE, 0x0FF, 0x100, 0x101, 0x102, 0x103, 0x104,
                0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D, 0x110,
                0x111, 0x112, 0x113, 0x115, 0x116, 0x117, 0x118, 0x119, 0x11A, 0x11B,
                0x11C, 0x11D, 0x11E, 0x11F, 0x120, 0x121, 0x122, 0x125, 0x126, 0x127,
                0x128, 0x129, 0x12A, 0x12B, 0x12C, 0x12D, 0x12F, 0x130, 0x131, 0x132,
                0x133, 0x134, 0x135, 0x136, 0x137, 0x138, 0x139, 0x13B, 0x13C, 0x13D,
                0x13E, 0x141, 0x142, 0x143, 0x146, 0x147, 0x148, 0x149, 0x14A, 0x14B,
                0x14C, 0x14D, 0x14E, 0x14F, 0x150, 0x151, 0x152, 0x153, 0x154, 0x155,
                0x156, 0x157, 0x158, 0x159, 0x15A, 0x15B, 0x15C, 0x15D, 0x15E, 0x15F,
                0x160, 0x161, 0x162, 0x163, 0x164, 0x165, 0x166, 0x167, 0x168, 0x169,
                0x16A, 0x16B, 0x16C, 0x16D, 0x16E, 0x16F, 0x170, 0x171, 0x172, 0x173,
                0x174, 0x175, 0x176, 0x177, 0x178, 0x179, 0x17A, 0x17B, 0x17C, 0x17D,
                0x17E, 0x17F, 0x180, 0x181, 0x182, 0x183, 0x184, 0x185, 0x186, 0x187,
                0x188, 0x189, 0x18A, 0x18B, 0x18C, 0x18D, 0x18E, 0x18F, 0x190, 0x191,
                0x192, 0x193, 0x194, 0x195, 0x196, 0x197, 0x198, 0x199, 0x19A, 0x19B,
                0x19C, 0x19D, 0x19E, 0x19F, 0x1A0, 0x1A1, 0x1A2, 0x1A3, 0x1A4, 0x1A5,
                0x1A6, 0x1A7, 0x1A8, 0x1A9, 0x1AA, 0x1AB, 0x1AC, 0x1AD, 0x1AF, 0x1B0,
                0x1B1, 0x1B2, 0x1B3, 0x1B4, 0x1B5, 0x1B6, 0x1B7, 0x1B8, 0x1B9, 0x1BA,
                0x1BB, 0x1BC, 0x1BD, 0x1BE, 0x1BF, 0x1C0, 0x1C1, 0x1C2, 0x1C3, 0x1C4,
                0x1C5, 0x1C6, 0x1C7, 0x1C8, 0x1C9, 0x1CA, 0x1CB, 0x1CE, 0x1CF, 0x1D0,
                0x1D1, 0x1D2, 0x1D3, 0x1D4, 0x1D5, 0x1D7, 0x1D8, 0x1D9, 0x1DA, 0x1DB,
                0x1DC, 0x1DD, 0x1DE, 0x1DF, 0x1E0, 0x1E1, 0x1E2, 0x1E3,
            ],
            'overlap_differences': 218, 'missing_target_bytes': 1,
            'extra_candidate_bytes': 0, 'complete_bytes_equal': False,
        }

    def test_title_options_complete_frontier_rejects_bytes_and_graph_regressions(self):
        baseline = self.options_evidence()
        boss.guard_title_options(baseline)
        for changed in (
                {'candidate_bytes': 2049}, {'candidate_bytes': 2044},
                {'literal_definitions_checked': None}, {'literal_definitions_checked': 0},
                {'literal_definitions_checked': True}, {'literal_definitions_checked': 2},
                {'target_instructions': 550}, {'candidate_instructions': 555},
                {'original_fields': 134}, {'bound_fields': 134},
                {'ordered_calls': 50}, {'ordered_calls_agree': False},
                {'original_blocks': 120}, {'candidate_blocks': 120},
                {'direct_graphs_agree': False}, {'aligned_instructions': 539},
                {'paired_branch_conflicts': [(7, 9)]},
                {'unpaired_branch_destinations': 6},
                {'overlap_differences': 611}, {'overlap_differences': -1},
                {'extra_candidate_bytes': 4}, {'missing_target_bytes': 1},
                {'complete_bytes_equal': True}, {'exactness_credit': 'accepted'}):
            with self.subTest(changed=changed), self.assertRaises(RuntimeError):
                boss.guard_title_options({**baseline, **changed})
        # Convergence is permitted at the diagnostic gate but not promoted.
        exact_diagnostic = {**baseline, 'candidate_bytes': 2045,
                            'candidate_instructions': 551,
                            'aligned_instructions': 551,
                            'unpaired_branch_destinations': 0,
                            'overlap_differences': 0,
                            'difference_offsets': [],
                            'extra_candidate_bytes': 0,
                            'complete_bytes_equal': True}
        boss.guard_title_options(exact_diagnostic)
        self.assertEqual(exact_diagnostic['exactness_credit'], 'none')

    def test_native_input_identity_does_not_accept_a_shorter_original_owner(self):
        rows = [dict(original_offset=offset, candidate_offset=offset,
                     original_target=destination, type='DIR32',
                     symbol=check.NATIVE_INPUT_SYMBOL, root=check.NATIVE_INPUT_ROOT,
                     addend=destination - check.NATIVE_INPUT_ROOT)
                for offset, destination in check.ORIGINAL_INPUT_FIELDS]
        check.require_native_input_references(rows, 2044)
        shorter = {**self.options_evidence(), 'candidate_bytes': 2044,
                   'missing_target_bytes': 1, 'extra_candidate_bytes': 0,
                   'native_input_array_references_verified': True}
        with self.assertRaisesRegex(RuntimeError, 'frontier drifted'):
            boss.guard_title_options(shorter)

    def test_every_equal_options_position_is_protected_at_the_same_difference_count(self):
        baseline = self.options_evidence()
        known = baseline['difference_offsets']
        equal = set(range(2045)) - set(known)
        self.assertEqual(len(known), 610)
        self.assertEqual(len(equal), 1435)
        for offset in equal:
            changed = sorted([*known[1:], offset])
            self.assertEqual(len(changed), 610)
            with self.subTest(offset=offset), self.assertRaises(RuntimeError):
                boss.guard_title_options({**baseline, 'difference_offsets': changed})

    def test_options_can_narrow_the_residual_set_without_exact_credit(self):
        baseline = self.options_evidence()
        closer = {**baseline, 'difference_offsets': baseline['difference_offsets'][1:],
                  'overlap_differences': 609}
        boss.guard_title_options(closer)
        self.assertEqual(closer['exactness_credit'], 'none')

    def test_options_rejects_missing_malformed_offsets_and_wrong_original_hash(self):
        baseline = self.options_evidence()
        known = baseline['difference_offsets']
        missing = dict(baseline)
        missing.pop('difference_offsets')
        with self.assertRaises(RuntimeError):
            boss.guard_title_options(missing)
        for offsets in (None, tuple(known), [True], [1.0], [[]], [-1], [2045],
                        list(reversed(known)), [*known, known[-1]], known[:-1]):
            with self.subTest(offsets=offsets), self.assertRaises(RuntimeError):
                boss.guard_title_options({**baseline, 'difference_offsets': offsets})
        with self.assertRaises(RuntimeError):
            boss.guard_title_options({**baseline, 'target_sha256': '0' * 64})

    def test_title_result_update_complete_frontier_rejects_byte_and_shape_regressions(self):
        baseline = self.result_evidence()
        boss.guard_title_update_result(baseline)
        for changed in (
                {'candidate_bytes': 483}, {'candidate_bytes': 487},
                {'target_instructions': 146}, {'candidate_instructions': 146},
                {'original_fields': 33}, {'bound_fields': 33},
                {'ordered_calls': 8}, {'calls_agree': False},
                {'original_blocks': 39}, {'candidate_blocks': 39},
                {'direct_graphs_agree': False}, {'overlap_differences': 219},
                {'overlap_differences': -1}, {'missing_target_bytes': 2},
                {'extra_candidate_bytes': 1}, {'complete_bytes_equal': True},
                {'diagnostic_only': False}):
            with self.subTest(changed=changed), self.assertRaises(RuntimeError):
                boss.guard_title_update_result({**baseline, **changed})
        exact_diagnostic = {**baseline, 'candidate_bytes': 485,
                            'candidate_instructions': 147,
                            'overlap_differences': 0, 'missing_target_bytes': 0,
                            'difference_offsets': [],
                            'complete_bytes_equal': True}
        boss.guard_title_update_result(exact_diagnostic)
        self.assertEqual(exact_diagnostic['exactness_credit'], 'none')

    def test_result_update_preserves_each_equal_byte_at_the_same_difference_count(self):
        baseline = self.result_evidence()
        observed = baseline['difference_offsets']
        self.assertEqual(len(observed), 218)
        for offset in set(range(484)) - set(observed):
            exchanged = sorted([*observed[1:], offset])
            with self.subTest(offset=offset), self.assertRaises(RuntimeError):
                boss.guard_title_update_result({**baseline, 'difference_offsets': exchanged})
        for bad in (None, [True], [[0]], list(reversed(observed)), observed[:-1]):
            with self.subTest(bad=str(bad)[:40]), self.assertRaises(RuntimeError):
                boss.guard_title_update_result({**baseline, 'difference_offsets': bad})
        with self.assertRaises(RuntimeError):
            boss.guard_title_update_result({**baseline, 'target_sha256': '0' * 64})

    def test_options_identity_gate_fails_closed(self):
        evidence = self.options_evidence()
        for changed in ({'bound_fields': 134}, {'original_fields': 134},
                        {'target_instructions': 550}, {'ordered_calls': 50},
                        {'ordered_calls_agree': False}, {'original_blocks': 120},
                        {'direct_graphs_agree': False}):
            with self.subTest(changed=changed), patch.object(
                    boss, 'build') as compile_options, patch.object(
                    boss, 'parsed', return_value={**evidence, **changed}), self.assertRaises(RuntimeError):
                boss.run_title_options(protect_exact=True)
            compile_options.assert_called_once_with('title-screen-options-update')



if __name__ == '__main__':
    unittest.main()

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
    def test_options_identity_gate_fails_closed(self):
        evidence = {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 2045, 'candidate_bytes': 2048,
            'target_instructions': 551, 'candidate_instructions': 554,
            'original_fields': 135, 'bound_fields': 135,
            'ordered_calls': 51, 'ordered_calls_agree': True,
            'original_blocks': 121, 'direct_graphs_agree': True,
            'extra_candidate_bytes': 3, 'missing_target_bytes': 0,
        }
        # Selective test: let the four cursor/result audits provide valid
        # lightweight evidence before the Options audit; reject corruption
        # without invoking the remaining renderer calls.
        cursors = [{
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': size, 'candidate_bytes': size,
            'original_fields': fields, 'bound_fields': fields,
            'ordered_calls': calls, 'ordered_calls_agree': True,
            'overlap_differences': 0, 'extra_candidate_bytes': 0,
            'direct_graphs_agree': True,
        } for size,fields,calls in ((211,6,4),(175,2,2),(175,2,2))]
        update = {'diagnostic_only': True, 'exactness_credit': 'none',
                  'target_bytes': 485, 'candidate_bytes': 483,
                  'original_fields': 34, 'bound_fields': 34,
                  'ordered_calls': 9, 'calls_agree': True,
                  'original_blocks': 40, 'direct_graphs_agree': True,
                  'overlap_differences': 0, 'missing_target_bytes': 2}
        for changed in ({'bound_fields': 134}, {'original_fields': 134},
                        {'target_instructions': 550}, {'ordered_calls': 50},
                        {'ordered_calls_agree': False}, {'original_blocks': 120},
                        {'direct_graphs_agree': False}):
            with self.subTest(changed=changed), patch.object(boss, 'build'), patch.object(
                    boss, 'build_nonexact') as compile_options, patch.object(
                    boss, 'parsed', side_effect=[*cursors, update, {**evidence, **changed}]), self.assertRaises(RuntimeError):
                boss.run_title_open()
            compile_options.assert_called_once()


if __name__ == '__main__':
    unittest.main()

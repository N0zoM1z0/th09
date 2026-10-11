"""Target-independent checks for the TitleDrawCallback non-crediting Oracle."""
import importlib.util
from pathlib import Path
import sys
from types import ModuleType
from unittest import TestCase, main
from unittest.mock import patch

SOURCE = Path(__file__).resolve().parents[1] / 'scripts/inspect-title-draw-callback.py'
with patch.dict(sys.modules, {'capstone': ModuleType('capstone')}), \
        patch.object(sys, 'path', [str(SOURCE.parent), *sys.path]):
    spec = importlib.util.spec_from_file_location('title_draw_callback_inspector', SOURCE)
    oracle = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(oracle)


class TitleDrawCallbackFrontierTests(TestCase):
    def test_candidate_shift_is_exactly_one_byte_after_missing_dec(self):
        self.assertEqual(oracle.expected_candidate_offset(0x100, True), 0x100)
        self.assertEqual(oracle.expected_candidate_offset(0x107, True), 0x106)
        self.assertEqual(oracle.expected_candidate_offset(0x13E, True), 0x13D)
        self.assertEqual(oracle.expected_candidate_offset(0x13E, False), 0x13E)

    def test_missing_dec_is_not_treated_as_a_relocation_field(self):
        with self.assertRaisesRegex(ValueError, 'missing DEC'):
            oracle.expected_candidate_offset(oracle.GAP_OFFSET, True)
        with self.assertRaisesRegex(ValueError, 'outside'):
            oracle.expected_candidate_offset(oracle.SIZE, False)

    def test_relocations_have_reviewed_extent_and_nonoverlapping_offsets(self):
        self.assertEqual(len(oracle.FIELDS), 20)
        for missing in (False, True):
            ranges = []
            for offset, kind, symbol, destination in oracle.FIELDS:
                mapped = oracle.expected_candidate_offset(offset, missing)
                self.assertTrue(kind in ('REL32', 'DIR32'))
                self.assertTrue(symbol and destination > 0x400000)
                self.assertLessEqual(mapped + 4, oracle.SIZE - int(missing))
                ranges.extend(range(mapped, mapped + 4))
            self.assertEqual(len(ranges), len(set(ranges)))

    def test_target_draw_call_order_is_explicitly_reviewed(self):
        names = [(row[1], row[2]) for row in oracle.FIELDS
                 if 'Draw2D@' in row[2] or 'DrawNoRotation@' in row[2]]
        self.assertEqual(names, [
            ('REL32', '?Draw2D@AnmManager@@QAEHPAUAnmVm@@@Z'),
            ('REL32', '?DrawNoRotation@AnmManager@@QAEHPAUAnmVm@@@Z'),
            ('REL32', '?DrawNoRotation@AnmManager@@QAEHPAUAnmVm@@@Z'),
        ])

    def test_all_relocation_field_offsets_are_strictly_ordered(self):
        offsets = [row[0] for row in oracle.FIELDS]
        self.assertEqual(offsets, sorted(offsets))
        self.assertEqual(sum(kind == 'REL32' for _, kind, _, _ in oracle.FIELDS), 12)


if __name__ == '__main__':
    main()

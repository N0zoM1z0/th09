"""Original input operands must retain the real array root and member addends."""
import copy
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'options_native_input', ROOT / 'scripts/inspect-title-options.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


def native_references():
    return [dict(original_offset=offset, candidate_offset=offset,
                 original_target=destination, type='DIR32',
                 symbol=audit.NATIVE_INPUT_SYMBOL, root=audit.NATIVE_INPUT_ROOT,
                 addend=destination - audit.NATIVE_INPUT_ROOT)
            for offset, destination in audit.ORIGINAL_INPUT_FIELDS]


class NativeInputArray(unittest.TestCase):
    def test_original_receivers_and_front_word_use_third_native_slot(self):
        rows = native_references()
        self.assertEqual([row['addend'] for row in rows].count(2 * 0x8E), 2)
        self.assertEqual([row['addend'] for row in rows].count(2 * 0x8E + 6), 6)
        audit.require_native_input_references(rows, 2048)
        # Source positions may converge to the original full extent later.
        audit.require_native_input_references(rows, 2045)

    def test_native_fields_are_checked_against_the_actual_candidate_extent(self):
        # The genuine visible ChangeCurrentScreen context emits 2,044 bytes.
        # Its last two native-input operands move three bytes forward, and
        # all eight fields still belong to the same array and member slots.
        rows = native_references()
        for row in rows[-2:]:
            row['candidate_offset'] += 3
        audit.require_native_input_references(rows, 2044)
        last_field_end = rows[-1]['candidate_offset'] + 4
        audit.require_native_input_references(rows, last_field_end)
        with self.assertRaises(ValueError):
            audit.require_native_input_references(rows, last_field_end - 1)

    def test_same_numeric_address_does_not_validate_old_subobject_aliases(self):
        for index, row in enumerate(native_references()):
            rows = native_references()
            old = ('?g_TitleInput@@3UInputView@@A' if row['addend'] == 0x11C
                   else '?g_TitleInputFlags@@3IA')
            rows[index].update(symbol=old, root=row['original_target'], addend=0)
            self.assertEqual(rows[index]['root'] + rows[index]['addend'],
                             row['original_target'])
            with self.subTest(index=index), self.assertRaises(ValueError):
                audit.require_native_input_references(rows, 2048)

    def test_all_eight_original_and_actual_source_identities_are_required(self):
        for index in range(8):
            for key, value in (('original_offset', -1), ('original_target', 0),
                               ('symbol', '?fake@@3IA'), ('type', 'REL32'),
                               ('root', 0x4ACF34), ('addend', 0),
                               ('original_offset', float(audit.ORIGINAL_INPUT_FIELDS[index][0]))):
                rows = native_references()
                rows[index][key] = value
                with self.subTest(index=index, key=key), self.assertRaises(ValueError):
                    audit.require_native_input_references(rows, 2048)
        for rows in ([], native_references()[:-1], native_references() + [{}],
                     [None] * 8):
            with self.subTest(rows=rows), self.assertRaises(ValueError):
                audit.require_native_input_references(rows, 2048)

    def test_actual_field_coordinates_cannot_overlap_or_leave_the_owner(self):
        for index, offset in ((0, -1), (0, False), (1, 733), (7, 2045)):
            rows = native_references()
            rows[index]['candidate_offset'] = offset
            with self.subTest(index=index, offset=offset), self.assertRaises(ValueError):
                audit.require_native_input_references(rows, 2048)
        for size in (None, True, 0, -1, 2044.0):
            with self.subTest(size=size), self.assertRaises(ValueError):
                audit.require_native_input_references(copy.deepcopy(native_references()), size)


if __name__ == '__main__':
    unittest.main()

"""Pure CI checks: aligned address fields are evidence, never exact-byte credit."""
import importlib.util
from pathlib import Path
import unittest

spec = importlib.util.spec_from_file_location(
    'field_alignment',
    Path(__file__).resolve().parents[1] / 'scripts' / 'diagnostic_field_alignment.py')
FIELD = importlib.util.module_from_spec(spec)
spec.loader.exec_module(FIELD)


class FieldAlignmentTests(unittest.TestCase):
    def test_equal_identities_at_shifted_offsets_do_not_report_false_drift(self):
        target = [(2, 'DIR32', 0x4A7D90), (12, 'REL32', 0x41AA07)]
        candidate = [(3, 'DIR32', 0x4A7D90), (25, 'REL32', 0x41AA07)]
        result = FIELD.align_fields(target, candidate)
        self.assertEqual(result['paired_field_identities'], 2)
        self.assertEqual(result['changes'], [])

    def test_multiplicity_and_reordering_stay_visible(self):
        target = [(1, 'DIR32', 0x4A7D90), (8, 'DIR32', 0x4A7DAC),
                  (17, 'DIR32', 0x4A7D90)]
        candidate = [(1, 'DIR32', 0x4A7D90), (6, 'DIR32', 0x4A7D90),
                     (17, 'DIR32', 0x4A7DAC)]
        result = FIELD.align_fields(target, candidate)
        self.assertEqual(result['target_fields'], 3)
        self.assertEqual(result['candidate_fields'], 3)
        self.assertEqual(result['paired_field_identities'], 2)
        self.assertEqual([x['kind'] for x in result['changes']], ['insert', 'delete'])
        self.assertTrue(any((17, 'DIR32', 0x4A7D90) in x['target']
                            for x in result['changes']))

    def test_missing_and_extra_destinations_are_never_paired(self):
        target = [(1, 'DIR32', 0x4A7D90), (8, 'DIR32', 0x4A7D94)]
        candidate = [(1, 'DIR32', 0x4A7D90), (8, 'REL32', 0x4A7D94),
                     (15, 'DIR32', 0x4A7D98)]
        result = FIELD.align_fields(target, candidate)
        self.assertEqual(result['paired_field_identities'], 1)
        self.assertEqual(result['changes'][0]['kind'], 'replace')
        self.assertEqual(result['changes'][0]['target'], [(8, 'DIR32', 0x4A7D94)])
        self.assertEqual(result['changes'][0]['candidate'][0],
                         (8, 'REL32', 0x4A7D94))

    def test_bad_orders_and_unknown_kinds_fail_closed(self):
        for rows in (
            [(1, 'DIR32', 0x4A7D90), (1, 'DIR32', 0x4A7D91)],
            [(1, 'DIR32', 0x4A7D90), (0, 'DIR32', 0x4A7D91)],
            [(2, 'DIR16', 0x4A7D90)],
            [(2, 'DIR32', -1)],
            [(2, 'DIR32')],
        ):
            with self.subTest(rows=rows), self.assertRaises(ValueError):
                FIELD.align_fields([], rows)


if __name__ == '__main__':
    unittest.main()

"""Original indexed GameManager references must remain independently complete."""
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('fan', ROOT/'scripts/inspect-effect-player-offset-fan.py')
fan = importlib.util.module_from_spec(spec)
spec.loader.exec_module(fan)


class IndexedFanField(unittest.TestCase):
    def report(self, destination=0x4A7D94):
        return {'diagnostic_only': True, 'exactness_credit': 'none',
                'original_fields': [{'offset': 10, 'type': 'DIR32', 'destination': 0x491620}],
                'bindings': [{'offset': 10, 'type': 'DIR32', 'effective_target': 0x491620},
                             {'offset': 0x36, 'type': 'DIR32', 'effective_target': destination}]}

    def original(self):
        return [{'offset': 0x36, 'type': 'DIR32', 'destination': 0x4A7D94}]

    def test_original_inventory_covers_the_indexed_field_without_granting_credit(self):
        out = fan.complete_indexed_update_fields(self.report(), self.original())
        self.assertTrue(out['all_original_fields_agree'])
        self.assertTrue(out['field_referent_order_agrees'])
        self.assertEqual(len(out['original_fields']), 2)
        self.assertEqual(out['exactness_credit'], 'none')

    def test_wrong_or_missing_original_indexed_reference_is_rejected(self):
        for rows in [[], self.original()*2,
                     [dict(self.original()[0], offset=0x35)],
                     [dict(self.original()[0], type='REL32')],
                     [dict(self.original()[0], destination=0x4A7D90)]]:
            with self.subTest(rows=rows), self.assertRaises(ValueError):
                fan.complete_indexed_update_fields(self.report(), rows)

    def test_candidate_cannot_redirect_the_original_global(self):
        out = fan.complete_indexed_update_fields(self.report(0x4A7D95), self.original())
        self.assertFalse(out['all_original_fields_agree'])
        self.assertEqual(out['original_fields'][-1]['destination'], 0x4A7D94)

    def test_duplicate_original_field_is_rejected(self):
        report = self.report()
        report['original_fields'] += self.original()
        with self.assertRaises(ValueError):
            fan.complete_indexed_update_fields(report, self.original())


if __name__ == '__main__':
    unittest.main()

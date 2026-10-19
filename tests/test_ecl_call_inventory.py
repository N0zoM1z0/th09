"""Target-independent failures for the non-crediting RunEcl call census."""
from pathlib import Path
import importlib.util
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'ecl_call_census', ROOT/'scripts/inspect-ecl-call-inventory.py')
census = importlib.util.module_from_spec(spec)
spec.loader.exec_module(census)


class CallInventoryTests(unittest.TestCase):
    def test_equal_call_counts_do_not_hide_a_changed_known_destination(self):
        result = census.compare_calls(['a', 'a'], [0x401000, 0x402000],
                                      {'a': {0x401000}})
        self.assertTrue(result['same_direct_call_count'])
        self.assertFalse(result['same_direct_call_multiset'])
        self.assertEqual(result['missing_target_calls'],
                         [{'destination': '0x402000', 'count': 1}])
        self.assertEqual(result['extra_known_calls'],
                         [{'destination': '0x401000', 'count': 1}])

    def test_source_only_wrapper_cannot_borrow_the_missing_resolver_identity(self):
        result = census.compare_calls(['a', 'source_wrapper'], [1, 2], {'a': {1}})
        self.assertTrue(result['same_direct_call_count'])
        self.assertFalse(result['same_direct_call_multiset'])
        self.assertEqual(result['unresolved_symbols'],
                         [{'symbol': 'source_wrapper', 'count': 1,
                           'possible_destinations': []}])
        self.assertEqual(result['missing_target_calls'],
                         [{'destination': '0x2', 'count': 1}])

    def test_ambiguous_canonical_and_alias_addresses_are_not_fit_to_target(self):
        known = census.symbol_destinations(
            {'unit': {'symbol': 'ambiguous', 'target_address': 1}}, {'ambiguous': 2})
        result = census.compare_calls(['ambiguous'], [1], known)
        self.assertFalse(result['same_direct_call_multiset'])
        self.assertEqual(result['unresolved_symbols'][0]['possible_destinations'], [1, 2])

    def test_equal_multiset_explicitly_does_not_prove_order_cfg_or_exactness(self):
        result = census.compare_calls(['b', 'a'], [1, 2], {'a': {1}, 'b': {2}})
        self.assertTrue(result['same_direct_call_multiset'])
        self.assertEqual(result['exactness_credit'], 'none')
        self.assertEqual(result['ordering_or_cfg_identity'], 'not established')

    def test_actual_coff_table_labels_are_retained_independently_of_target(self):
        start = 228
        rows = [dict(offset=start+4*i, type='DIR32', addend=0,
                     symbol_section=1, owner_section=1,
                     symbol_value=8, owner_value=0)
                for i in range(census.TABLE_ENTRIES)]
        actual_start, labels = census.table_destinations(1000, rows)
        self.assertEqual(actual_start, start)
        self.assertEqual(labels, [8]*193)

    def test_missing_foreign_or_out_of_code_table_field_is_rejected(self):
        rows = [dict(offset=228+4*i, type='DIR32', addend=0,
                     symbol_section=1, owner_section=1,
                     symbol_value=8, owner_value=0)
                for i in range(census.TABLE_ENTRIES)]
        for broken in (rows[:-1], rows+[dict(rows[0])],
                       [dict(rows[0], symbol_section=2)]+rows[1:],
                       [dict(rows[0], addend=4)]+rows[1:],
                       [dict(rows[0], symbol_value=228)]+rows[1:]):
            with self.subTest(first=broken[0]), self.assertRaises(ValueError):
                census.table_destinations(1000, broken)


if __name__ == '__main__':
    unittest.main()

"""Alias byte evidence must not assign the folded target an authored owner."""
import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('vector_pointer_policy',
                                             ROOT / 'scripts/check-vector-pointer-views.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


class FoldedVectorPointerPolicy(unittest.TestCase):
    def setUp(self):
        self.units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
        self.function = next(row for row in audit.read_rows('functions.csv')
                             if int(row['address'], 0) == audit.ADDRESS)
        self.origin = next(row for row in audit.read_rows('function-origins.csv')
                           if int(row['address'], 0) == audit.ADDRESS)
        self.matches = audit.read_rows('matches.csv')

    def validate(self, units=None, function=None, origin=None, matches=None):
        audit.validate_contract(self.units if units is None else units,
                                self.function if function is None else function,
                                self.origin if origin is None else origin,
                                self.matches if matches is None else matches)

    def test_both_native_interfaces_share_one_unknown_physical_owner(self):
        self.validate()
        owners = {(self.units[name]['target_address'], self.units[name]['size'])
                  for name in audit.ALIASES}
        self.assertEqual(owners, {(0x4343D0, 3)})
        self.assertNotIn('0x004343D0', {row['address'] for row in self.matches})

    def test_alias_cannot_change_actual_source_profile_symbol_or_extent(self):
        for name in audit.ALIASES:
            for change in ({'target_address': 0x4343D1}, {'size': 2},
                           {'source': 'src/Float3.cpp'}, {'symbol': 'fake'},
                           {'profile': ['/O1']},
                           {'relocations': [{'offset': 0, 'target': 0x4343D0}]}):
                units = copy.deepcopy(self.units)
                units[name].update(change)
                with self.subTest(name=name, change=change), self.assertRaises(ValueError):
                    self.validate(units=units)

    def test_byte_equality_cannot_promote_origin_or_unique_class(self):
        for change in ({'proposed_name': 'Float3::operator float*'}, {'module': 'Float3'},
                       {'owner': 'authored'}, {'status': 'matching'},
                       {'match_percent': '100.00'}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                self.validate(function={**self.function, **change})
        for change in ({'origin': 'authored_game'}, {'origin': 'compiler_generated'},
                       {'disposition': 'authored'}, {'disposition': 'exclude'}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                self.validate(origin={**self.origin, **change})

    def test_duplicate_alias_credit_for_the_same_address_is_rejected(self):
        for unit in audit.ALIASES:
            with self.subTest(unit=unit), self.assertRaises(ValueError):
                self.validate(matches=[*self.matches, {'address': '0x004343D0', 'unit': unit}])


if __name__ == '__main__':
    unittest.main()

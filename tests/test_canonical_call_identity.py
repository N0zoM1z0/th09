"""A byte replay must not redirect a maintained callee to a different method.

Only names that already have canonical function owners are constrained here.
Private unimplemented views still require independent identity review; this
check does not infer identity from a method's short name or validate addends.
Complete object/relocation replay remains necessary alongside this guard.
"""
import csv
from collections import defaultdict
from pathlib import Path
import tomllib
import unittest


class CanonicalCallIdentityTests(unittest.TestCase):
    def test_calls_keep_their_canonical_symbol_destinations(self):
        root = Path(__file__).resolve().parents[1]
        units = tomllib.loads((root / 'config/match-units.toml').read_text())['units']
        owners = defaultdict(set)
        for unit in units.values():
            owners[unit['symbol']].add(unit['target_address'])
        checked = 0
        for name, unit in units.items():
            for field in unit.get('relocations', []):
                if field['type'] == 'REL32' and field['symbol'] in owners:
                    with self.subTest(unit=name, offset=field['offset'], symbol=field['symbol']):
                        self.assertIn(field['target'], owners[field['symbol']])
                    checked += 1
        self.assertGreater(checked, 0)

    def test_claimed_bytes_are_separate_from_physical_comparison_extent(self):
        root = Path(__file__).resolve().parents[1]
        units = tomllib.loads((root / 'config/match-units.toml').read_text())['units']
        with (root / 'config/matches.csv').open(newline='') as stream:
            claims = list(csv.DictReader(stream))
        for claim in claims:
            unit = units[claim['unit']]
            with self.subTest(unit=claim['unit']):
                self.assertEqual(unit['target_address'], int(claim['address'], 0))
                self.assertEqual(unit['size'], int(claim['size'], 0))
                self.assertGreaterEqual(unit.get('compare_size', unit['size']), unit['size'])

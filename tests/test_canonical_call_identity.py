"""A byte replay must not redirect a maintained callee to a different method.

Only names that already have canonical function owners are constrained here.
Private unimplemented views still require independent identity review; this
check does not infer identity from a method's short name or validate addends.
Complete object/relocation replay remains necessary alongside this guard.
"""
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

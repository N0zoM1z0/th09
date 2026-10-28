"""Manifest/ledger identities; affected PE/COFF replay establishes exactness."""
import csv
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
SYMBOL = '?InitializeSubroutine@EnemyEclManagerView@@QAEHPAUTh09EclContextView@@F@Z'
NAME = 'EnemyEclManagerView::InitializeSubroutine'
LEGACY = ('?InitializeSubroutine@ManagerStateView@Th09EclRunState@@',
          '?InitializeSubroutine@EnemySpawnEclManagerBoundary@@',
          '?InitializeSubroutine@EnemySpawnRecordEclManagerBoundary@@')


class SelectedEclManagerOwner(unittest.TestCase):
    def test_canonical_initializer_and_every_original_call_use_one_identity(self):
        units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        owner = units['ecl-initialize-subroutine']
        self.assertEqual((owner['symbol'], owner['functions'], owner['source']),
                         (SYMBOL, [NAME], 'src/EclInitializeSubroutine.cpp'))
        self.assertEqual((owner['target_address'], owner['size']), (0x406850, 0x49))
        self.assertEqual([(r['offset'], r['target']) for r in owner['relocations']],
                         [(0x26, 0x401500), (0x33, 0x401500)])
        calls = []
        for name, unit in units.items():
            for row in unit.get('relocations', []):
                if row['type'] == 'REL32' and row['target'] == 0x406850:
                    self.assertEqual(row['symbol'], SYMBOL, name)
                    calls.append(name)
            for old in LEGACY:
                self.assertNotIn(old, str(unit))
        self.assertGreaterEqual(len(calls), 6)

    def test_all_identity_ledgers_remove_legacy_owner(self):
        for file, field in (('functions.csv', 'proposed_name'),
                            ('matches.csv', 'name'), ('reccmp-functions.csv', 'name')):
            with (ROOT/'config'/file).open(newline='') as stream:
                rows = [r for r in csv.DictReader(stream) if int(r['address'], 0) == 0x406850]
            self.assertEqual(len(rows), 1, file)
            self.assertEqual(rows[0][field], NAME, file)
        implemented = (ROOT/'config/implemented.csv').read_text().splitlines()
        self.assertIn(NAME, implemented)
        self.assertNotIn('Th09EclRunState::ManagerStateView::InitializeSubroutine', implemented)


if __name__ == '__main__':
    unittest.main()

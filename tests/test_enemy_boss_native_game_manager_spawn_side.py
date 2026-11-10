"""Nonexact 3900-byte Enemy boss borrows the actual native gameplay side.

The original-bound complete 97-field PE/COFF oracle is external; this
protects the exact hot-branch object identity and call semantics without
falsely marking this boss as byte-exact.
"""
from pathlib import Path
import csv
import tomllib
import unittest
ROOT=Path(__file__).resolve().parents[1]

class EnemyBossNativeGameManagerSpawnSide(unittest.TestCase):

    def test_complete_original_boss_remains_nonexact_with_real_exact_siblings(self):
        with (ROOT/'config/functions.csv').open() as fd:
            rows={row['address']:row for row in csv.DictReader(fd)}
        boss=rows['0x00410730']
        self.assertEqual(boss['size'],'3883')  # code; original physical owner adds 17-byte table/tail
        self.assertEqual(boss['source_file'],'src/EclManager.cpp')
        self.assertNotEqual(boss['status'],'matching')
        with (ROOT/'config/match-units.toml').open('rb') as fd:
            units=tomllib.load(fd)['units']
        self.assertTrue(any(u['source']=='src/EclManager.cpp' and u['size']>200
                            for u in units.values()))
        self.assertEqual(units['enemy-attached-effect-update']['size'],151)
        inspector=(ROOT/'scripts/inspect-enemy-manager-update.py').read_text()
        self.assertIn('OWNER_SIZE = 3900',inspector)
        self.assertIn('CODE_SIZE = 3883',inspector)
        self.assertIn("if len(relocations) != 97",inspector)
        self.assertIn("'complete_differences'",inspector)
        self.assertIn("'reviewed_control_flow_agrees'",inspector)

if __name__=='__main__':unittest.main()

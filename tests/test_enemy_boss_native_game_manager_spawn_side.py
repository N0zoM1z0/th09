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
    def test_actual_typed_0x38_game_manager_side_in_enemy_boss(self):
        src=(ROOT/'src/EnemyManagerCore.inl').read_text()
        original_side=(ROOT/'src/EnemyManager.hpp').read_text()
        native=(ROOT/'src/GameManagerSideState.hpp').read_text()
        self.assertIn('#include "AsciiGameManagerView.hpp"',src)
        self.assertIn('EnemyCoreRealGameManagerSpawnSide',src)
        self.assertIn('AsciiGameManagerSideView &spawnSide =',src)
        self.assertIn('reinterpret_cast<AsciiGameManagerSideView *>(',src)
        self.assertIn('manager->sideState320);',src)
        self.assertNotIn('EnemySideStateView &spawnSide = *manager->sideState320;',src)
        self.assertIn('spawnSide.character20 == 13 ? 1 : 3',src)
        self.assertIn('spawnSide.etama08->SpawnBulletPatternPrimary(',src)
        self.assertIn('BulletSpawnDescriptorCtorView storage;',src)
        self.assertIn('descriptor.position =',src)
        self.assertIn('AsciiGameManagerSideView',native)
        self.assertIn('EnemySideStateView',original_side)
        for phrase in ('sizeof(AsciiGameManagerSideView) == 0x38',
                       'sizeof(EnemySideStateView) == 0x38',
                       'offsetof(AsciiGameManagerSideView, etama08) == 0x08',
                       'offsetof(AsciiGameManagerSideView, character20) == 0x20',
                       'offsetof(EnemySideStateView, etama08) == 0x08',
                       'offsetof(EnemySideStateView, characterIndex20) == 0x20',
                       'offsetof(EnemyManagerView, sideState320) == 0x320'):
            self.assertIn(phrase,src)

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

    def test_source_local_scalar_probe_respects_real_native_side(self):
        src=(ROOT/'scripts/probe-enemy-native-spawn-context.py').read_text()
        self.assertIn('AsciiGameManagerSideView &spawnSide =',src)
        self.assertIn('AsciiGameManagerSideView *spawnSidePtr;',src)
        self.assertIn('spawnSide.character20',src)
        self.assertNotIn('EnemySideStateView &spawnSide = *manager->sideState320;',src)

if __name__=='__main__':unittest.main()

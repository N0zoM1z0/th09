"""Native Player owns the 190-byte shot producer used by the Charge boss.

Exactness remains decided by pinned VC7.1 full-unit byte comparisons; this
covers the source's single physical Player receiver and reviewed caller fields.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
NATIVE_SHOTS='?SpawnShots@PlayerLifecycleView@@QAIXHH@Z'

class PlayerNativeSpawnShotsOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            cls.units=tomllib.load(stream)['units']

    def test_native_player_method_and_array_bounds(self):
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source=(ROOT/'src/PlayerSpawnShots.cpp').read_text()
        self.assertIn('void __fastcall SpawnShots(int value, int powerLevel);',native)
        self.assertIn('void __fastcall PlayerLifecycleView::SpawnShots(',source)
        self.assertIn('PlayerLifecycleView *player,',source)
        self.assertIn('PlayerShotCtorView shots[128]',native)
        self.assertIn('sizeof(shots) / sizeof(shots[0])',source)
        self.assertIn('PlayerShotCtorSizeIs484',header)
        self.assertNotIn('PlayerSpawnShotsView::SpawnShots',source)
        self.assertIn('void __fastcall SpawnShots(',header)
        # Legacy state-effect and charge interfaces can remain zero-storage
        # derived facades, without owning the native shot method.
        self.assertFalse((ROOT/'src/PlayerSpawnShotsView.hpp').exists())

    def test_original_bound_producer_and_two_consumers(self):
        owner=self.units['player-spawn-shots']
        self.assertEqual(owner['symbol'],NATIVE_SHOTS)
        self.assertEqual(owner['target_address'],0x0041F4C0)
        self.assertEqual(owner['size'],190)
        self.assertEqual(len(owner['relocations']),1)
        caller=self.units['effect-update-type35']
        calls=[r for r in caller['relocations'] if r['symbol']==NATIVE_SHOTS]
        self.assertEqual(len(calls),1)
        self.assertEqual(calls[0]['offset'],0x1B)
        self.assertEqual(calls[0]['target'],0x0041F4C0)
        self.assertEqual(calls[0]['type'],'REL32')
        self.assertEqual(caller['size'],38)
        charge=(ROOT/'src/PlayerChargeAttackUpdate.cpp').read_text()
        self.assertEqual(charge.count('SpawnShots(static_cast<int>('),2)
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('int UpdateChargeAttack();',native)
        self.assertIn('int PlayerLifecycleView::UpdateChargeAttack()',charge)

    def test_tracking_names_one_native_method(self):
        for file in ('implemented.csv','reccmp-functions.csv',
                     'matches.csv','functions.csv'):
            with self.subTest(file=file):
                text=(ROOT/'config'/file).read_text()
                self.assertIn('PlayerLifecycleView::SpawnShots',text)
                self.assertNotIn('PlayerSpawnShotsView::SpawnShots',text)

if __name__=='__main__':
    unittest.main()

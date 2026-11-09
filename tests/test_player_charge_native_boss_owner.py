"""TH09's 1,210-byte charge boss and 522-byte PlayerRuntime caller share Player.

Only strict original-bound PE/COFF checks establish exactness; these guard
natural C++ ownership, unchanged native data and target-linked call identity.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
CHARGE_SYMBOL='?UpdateChargeAttack@PlayerLifecycleView@@QAEHXZ'

class PlayerNativeChargeBoss(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_native_player_source_and_callee_ownership(self):
        source=(ROOT/'src/PlayerChargeAttackUpdate.cpp').read_text()
        runtime=(ROOT/'src/PlayerRuntime.cpp').read_text()
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('int UpdateChargeAttack();',native)
        self.assertIn('int PlayerLifecycleView::UpdateChargeAttack()',source)
        self.assertIn('short side = static_cast<short>(sideIndex);',source)
        self.assertIn('GameplaySetupNativeSides::PlayableCount == 2',source)
        setup=(ROOT/'src/GameManagerSetup.cpp').read_text()
        registration=(ROOT/'src/PlayerLifecycle.cpp').read_text()
        self.assertEqual(setup.count('PlayerRegisterChain('),1)
        self.assertIn('for (sideIndex = 0; sideIndex < GameplaySetupNativeSides::PlayableCount; sideIndex++)',setup)
        self.assertIn('PlayerRegisterChain(0, sideIndex,',setup)
        self.assertIn('player->sideIndex = sideIndex;',registration)
        self.assertIn('player->UpdateChargeAttack();',runtime)
        self.assertNotIn('reinterpret_cast<PlayerChargeAttackView',runtime)
        self.assertNotIn('PlayerChargeAttackView',source)
        self.assertFalse((ROOT/'src/PlayerChargeAttackView.hpp').exists())
        self.assertIn('void __fastcall SpawnShots(int value, int powerLevel);',native)
        for name in ('SubtractResourceClamped','ConfigureState3Effect13',
                     'ConfigureState3Effect14','ConfigureState3Effect15'):
            self.assertIn(name+'(',native)
            self.assertIn(name+'(',source)
        self.assertEqual(source.count('SpawnShots(static_cast<int>('),2)

    def test_real_original_caller_address_and_manifest(self):
        runtime=self.units['player-on-update']
        self.assertEqual(runtime['size'],522)
        hits=[r for r in runtime['relocations'] if r['symbol']==CHARGE_SYMBOL]
        self.assertEqual(len(hits),1)
        self.assertEqual(hits[0]['offset'],0x1BE)
        self.assertEqual(hits[0]['type'],'REL32')
        self.assertEqual(hits[0]['target'],0x0041F810)
        self.assertEqual(hits[0].get('addend',0),0)
        self.assertIn(CHARGE_SYMBOL,(ROOT/'scripts/inspect-player-charge-update.py').read_text())
        self.assertIn('BASE, SIZE = 0x0041F810, 1210',
                      (ROOT/'scripts/inspect-player-charge-update.py').read_text())
        self.assertIn(CHARGE_SYMBOL,(ROOT/'scripts/inspect-player-charge-text.py').read_text())
        for filename in ('functions.csv','implemented.csv','reccmp-functions.csv'):
            s=(ROOT/'config'/filename).read_text()
            self.assertIn('PlayerLifecycleView::UpdateChargeAttack',s)
            self.assertNotIn('PlayerChargeAttackView::UpdateChargeAttack',s)

if __name__=='__main__':
    unittest.main()

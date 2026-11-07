"""The still-nonexact charge boss calls its exact state-3 effects as base methods."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerChargeNativeState3Chain(unittest.TestCase):
    def test_zero_extra_storage_method_chain(self):
        charge=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source=(ROOT/'src/PlayerChargeAttackUpdate.cpp').read_text()
        self.assertIn('int UpdateChargeAttack();',charge)
        self.assertIn('int PlayerLifecycleView::UpdateChargeAttack()',source)
        self.assertIn('void SubtractResourceClamped(float amount);',charge)
        self.assertIn('void __fastcall SpawnShots(int value, int powerLevel);',charge)
        self.assertFalse((ROOT/'src/PlayerStateEffectView.hpp').exists())
        self.assertIn('PlayerLifecycleSizeIs30F70',charge)
        self.assertNotIn('reinterpret_cast<PlayerStateEffectView *>(this)',source)
        for effect_number in (13,14,15):
            self.assertIn(f'ConfigureState3Effect{effect_number}();',source)

    def test_original_method_referents_still_independent(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        for effect_number,size in ((13,91),(14,91),(15,100)):
            name=f'player-state3-effect{effect_number}'
            self.assertEqual(u[name]['size'],size)
            self.assertEqual(u[name]['source'],'src/PlayerState3EffectSetup.cpp')
        self.assertEqual(u['player-subtract-resource-clamped']['size'],51)

if __name__=='__main__':unittest.main()

"""Charge boss resources and three State3 effects belong to the native Player.

Original-byte identity is enforced by the source-cold strict PE/COFF replay;
these checks make method/receiver ownership and complete callees explicit.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativePlayerState3AndResource(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            cls.units=tomllib.load(stream)['units']

    def test_single_player_native_methods_not_duplicated_in_derived_facades(self):
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        src=(ROOT/'src/PlayerState3EffectSetup.cpp').read_text()
        clamp=(ROOT/'src/PlayerResourceSubtract.cpp').read_text()
        self.assertIn('void SubtractResourceClamped(float amount);',native)
        self.assertIn('PlayerLifecycleNativeChargeResourceAt30388',native)
        self.assertIn('void PlayerLifecycleView::SubtractResourceClamped(',clamp)
        self.assertNotIn('struct PlayerCollisionRegionCreateView\n{',src)
        for n in (13,14,15):
            self.assertIn(f'Effect *ConfigureState3Effect{n}();',native)
            self.assertFalse((ROOT/'src/PlayerStateEffectView.hpp').exists())
            self.assertIn(f'Effect *PlayerLifecycleView::ConfigureState3Effect{n}()',src)
        self.assertFalse((ROOT/'src/PlayerResourceSubtractView.hpp').exists())
        self.assertIn('PlayerLifecycleSizeIs30F70',native)

    def test_four_original_bound_methods_and_big_boss_call_family(self):
        goals=(('player-subtract-resource-clamped',51,0x0041F310),
               ('player-state3-effect13',91,0x00403C30),
               ('player-state3-effect14',91,0x00403C90),
               ('player-state3-effect15',100,0x00403CF0))
        for unit,size,target in goals:
            with self.subTest(unit=unit):
                obj=self.units[unit]
                self.assertEqual(obj['size'],size)
                self.assertEqual(obj['target_address'],target)
                self.assertIn('@PlayerLifecycleView@@',obj['symbol'])
        boss=(ROOT/'src/PlayerChargeAttackUpdate.cpp').read_text()
        self.assertEqual(boss.count('SubtractResourceClamped('),3)
        for n in (13,14,15):
            self.assertEqual(boss.count(f'ConfigureState3Effect{n}();'),1)
        self.assertEqual(boss.count('SpawnShots(static_cast<int>('),2)

    def test_tracker_and_original_charge_referents(self):
        inspector=(ROOT/'scripts/inspect-player-charge-update.py').read_text()
        for text in ('?SubtractResourceClamped@PlayerLifecycleView@@QAEXM@Z',
                     '?ConfigureState3Effect13@PlayerLifecycleView@@QAEPAUEffect@@XZ',
                     '?ConfigureState3Effect14@PlayerLifecycleView@@QAEPAUEffect@@XZ',
                     '?ConfigureState3Effect15@PlayerLifecycleView@@QAEPAUEffect@@XZ'):
            self.assertIn(text,inspector)
        for file in ('implemented.csv','reccmp-functions.csv','matches.csv','functions.csv'):
            tracker=(ROOT/'config'/file).read_text()
            self.assertIn('PlayerLifecycleView::SubtractResourceClamped',tracker)
            self.assertIn('PlayerLifecycleView::ConfigureState3Effect13',tracker)

if __name__=='__main__':unittest.main()

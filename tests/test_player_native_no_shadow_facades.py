"""All five exact ChargeAttack helper leaves have one native Player C++ owner.

This source topology check supplements, not replaces, full original-bound
pinned VC7.1 PE/COFF verification of leaves and the open 1,210-byte boss.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerNativeWithoutShadows(unittest.TestCase):
    def test_no_duplicate_empty_facade_types_or_includes(self):
        for name in ('PlayerSpawnShotsView.hpp','PlayerResourceSubtractView.hpp',
                     'PlayerStateEffectView.hpp','PlayerChargeAttackView.hpp'):
            with self.subTest(file=name):
                self.assertFalse((ROOT/'src'/name).exists())
        for name in ('PlayerSpawnShots.cpp','PlayerResourceSubtract.cpp',
                     'PlayerState3EffectSetup.cpp','PlayerChargeAttackUpdate.cpp'):
            with self.subTest(source=name):
                src=(ROOT/'src'/name).read_text()
                self.assertIn('#include "PlayerLifecycleView.hpp"',src)
                self.assertNotIn('PlayerSpawnShotsView',src)
                self.assertNotIn('PlayerStateEffectView',src)
                self.assertNotIn('PlayerResourceSubtractView',src)
        source=(ROOT/'src/PlayerSpawnShots.cpp').read_text()
        for s in ('typedef int (__fastcall *PlayerSpawnCallback)',
                  'PlayerLifecycleView *player,',
                  'PlayerShotCtorView *shot,',
                  'PlayerShtDescriptorView *entry)',
                  'PlayerShotCtorView *shot = shots;',
                  'PlayerShtDescriptorView *entry = primaryShtFile->shotPowerLevels[powerLevel].descriptors;'):
            self.assertIn(s,source)
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('PlayerLifecycleNativeChargeResourceAt30388',header)
        self.assertIn('PlayerLifecyclePositionAt1B88',header)
        self.assertIn('PlayerLifecycleTimer303C8At303C8',header)

    def test_exact_leaves_and_unmatched_boss_remain_separately_owned(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        expected=(('player-spawn-shots',190,0x0041F4C0),
                  ('player-subtract-resource-clamped',51,0x0041F310),
                  ('player-state3-effect13',91,0x00403C30),
                  ('player-state3-effect14',91,0x00403C90),
                  ('player-state3-effect15',100,0x00403CF0))
        for name,size,addr in expected:
            with self.subTest(unit=name):
                u=units[name]
                self.assertEqual(u['size'],size)
                self.assertEqual(u['target_address'],addr)
                self.assertIn('@PlayerLifecycleView@@',u['symbol'])
        self.assertNotIn('player-charge-update',units)
        charge=(ROOT/'src/PlayerChargeAttackUpdate.cpp').read_text()
        self.assertIn('int PlayerLifecycleView::UpdateChargeAttack()',charge)
        self.assertEqual(charge.count('SpawnShots(static_cast<int>('),2)
        self.assertEqual(charge.count('SubtractResourceClamped('),3)

if __name__=='__main__':
    unittest.main()

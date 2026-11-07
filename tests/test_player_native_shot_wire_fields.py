"""Source-facing TH09 SpawnShots uses real native SHT/Player shot storage.

Strict complete original PE/COFF comparisons provide exactness, not these
source checks. Both Player callbacks and VM flags are verified as member data.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeShotWireFields(unittest.TestCase):
    def test_real_sht_descriptor_and_player_shot_fields(self):
        src=(ROOT/'src/PlayerSpawnShots.cpp').read_text()
        hdr=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('PlayerShtDescriptorView *entry = primaryShtFile->shotPowerLevels[powerLevel].descriptors;',src)
        self.assertIn('PlayerShotCtorView *shot = shots;',src)
        self.assertIn('reinterpret_cast<PlayerSpawnCallback>(entry->spawnCallback)',src)
        self.assertIn('entry->fireInterval < 0',src)
        self.assertIn('shot->vm.flagsWord |= 0x2000u;',src)
        self.assertIn('shot->descriptor480 = entry;',src)
        self.assertIn('shot->collisionCallback47C = entry->collisionCallback;',src)
        self.assertIn('shot->updateCallback474 = reinterpret_cast<PlayerShotUpdateCallback>(entry->updateCallback);',src)
        self.assertIn('shot->drawCallback478 = reinterpret_cast<PlayerShotDrawCallback>(entry->drawCallback);',src)
        self.assertNotIn('reinterpret_cast<PlayerSpawnShotsShotView *>(shots)',src)
        self.assertNotIn('reinterpret_cast<PlayerSpawnShotsShtFileView *>(primaryShtFile)',src)
        for entry in ('void *collisionCallback47C;', 'PlayerShtDescriptorView *descriptor480;',
                      'offsetof(PlayerShotCtorView, collisionCallback47C) == 0x47C',
                      'offsetof(PlayerShotCtorView, descriptor480) == 0x480',
                      'PlayerShotCtorView shots[128]'):
            self.assertIn(entry,hdr)

    def test_original_bound_callee_and_boss_ownership(self):
        with (ROOT/'config/match-units.toml').open('rb') as fp:
            u=tomllib.load(fp)['units']
        owner=u['player-spawn-shots']
        self.assertEqual(owner['size'],190)
        self.assertEqual(owner['target_address'],0x0041F4C0)
        self.assertEqual(owner['symbol'],'?SpawnShots@PlayerLifecycleView@@QAIXHH@Z')
        self.assertEqual(len(owner['relocations']),1)
        self.assertEqual(u['effect-update-type35']['size'],38)
        self.assertEqual(u['player-on-update']['size'],522)
        # The ChargeAttack owner is separately nonexact; a typed source
        # change cannot promote it based on exact leaf results.
        self.assertIn('int PlayerLifecycleView::UpdateChargeAttack()',
                      (ROOT/'src/PlayerChargeAttackUpdate.cpp').read_text())

if __name__=='__main__':
    unittest.main()

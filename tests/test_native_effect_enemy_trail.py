"""An exact Effect trail callback owns native Enemy and GameManager fields."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeEffectEnemyTrail(unittest.TestCase):
    def test_native_enemy_fields_replace_private_padded_projection(self):
        source = (ROOT/'src/EffectUpdateEnemyTrail.cpp').read_text()
        flags = (ROOT/'src/EclEnemyPrimaryFlags.hpp').read_text()
        enemy = (ROOT/'src/EnemyManager.hpp').read_text()
        self.assertNotIn('struct EffectTrailEnemyView', source)
        self.assertNotIn('struct EffectTrailEnemyFlags', source)
        self.assertIn('EffectTrailNativeEnemyStorage', source)
        self.assertIn('EnemyView *enemy = reinterpret_cast<EnemyView *>(', source)
        self.assertIn('enemy->acceleration2DF8 += state->speedA0;', source)
        self.assertIn('enemy->secondaryFlagBits3380.effectCategory06_08 = state->colorA6;', source)
        self.assertIn('enemy->movementAngle2DE0 =', source)
        self.assertIn('offsetof(EnemyView, acceleration2DF8) == 0x2DF8', source)
        self.assertIn('offsetof(EnemyView, secondaryFlags3380) == 0x3380', source)
        self.assertIn('unsigned int effectCategory06_08 : 3;', flags)
        self.assertIn('unsigned int unknown04_05 : 2;', flags)
        self.assertIn('unsigned int unknown09_31 : 23;', flags)
        self.assertIn('Th09EclEnemySecondaryFlagBits secondaryFlagBits3380;', enemy)

    def test_effect_source_reads_native_opposing_side_and_real_spawn_receiver(self):
        source = (ROOT/'src/EffectUpdateEnemyTrail.cpp').read_text()
        self.assertIn('static inline AsciiGameManagerSideView *EffectTrailOpponent(',source)
        self.assertIn('return &g_GameManager.sides[1 - state->sideIndexA8];',source)
        self.assertIn('static_cast<EnemyManagerCardAttackSpawnView *>(',source)
        self.assertNotIn('reinterpret_cast<EnemySideStateView *>(',source)
        self.assertIn('EffectTrailOpponent(state)->character20 != 7',source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        owner = units['effect-update-enemy-trail-type3-type4']
        self.assertEqual((owner['target_address'],owner['size'],len(owner['relocations'])),
                         (0x0040D9E0,464,20))
        self.assertEqual(sum('g_GameManager' in r['symbol'] for r in owner['relocations']),3)
        spawn = units['enemy-manager-spawn-record']
        self.assertTrue(any(r['symbol']==spawn['symbol'] and
                            r['target']==spawn['target_address']==0x0040F1D0
                            for r in owner['relocations']))


if __name__=='__main__':
    unittest.main()

"""Native Player owns the exact damage callback and Enemy caller method."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
SYMBOL='?CalcDamageToEnemy@PlayerLifecycleView@@QAEHPAUPlayerDamageFloat3@@0PAH11@Z'

class PlayerDamageNativeOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.body=(ROOT/'src/PlayerDamage.cpp').read_text()
        cls.player=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        cls.enemy=(ROOT/'src/EnemyManagerCore.inl').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_real_receiver_and_real_timer_resource_effect_subobjects(self):
        body=self.body
        self.assertIn('int PlayerLifecycleView::CalcDamageToEnemy(',body)
        self.assertIn('CalcDamageToEnemy(PlayerDamageFloat3 *position',self.player)
        self.assertNotIn('int PlayerDamagePlayerView::CalcDamageToEnemy(',body)
        self.assertIn('timer303C8.HasTicked()',body)
        self.assertIn('header24.anmFile98->SetAndExecuteScriptIdx(',body)
        self.assertIn('sideState->effectManager0C->SpawnEffectInFixedSlot(',body)
        self.assertIn('PlayerShotCtorView *shot;',body)
        self.assertIn('shot = shots;',body)
        self.assertIn('shot->vm.rotation.z',body)
        self.assertIn('&shot->vm, shot->animationIndex46C + 6',body)
        self.assertIn('PlayerPositionView *effectPosition = &shots[0].position2A4;',body)
        self.assertNotIn('shot->hitboxSize430',body)
        self.assertIn('reinterpret_cast<PlayerDamageRegionView **>(activeB100)',body)
        self.assertEqual(self.enemy.count('playerLifecycle04->CalcDamageToEnemy('),2)
        self.assertNotIn('PlayerDamagePlayerView *>(\n',self.enemy)

    def test_complete_original_owner_and_method_callees(self):
        unit=self.units['player-calc-damage-to-enemy']
        self.assertEqual((unit['target_address'],unit['size'],len(unit['relocations'])),
                         (0x0041FCD0,996,17))
        self.assertEqual(unit['symbol'],SYMBOL)
        rows={r['offset']:r for r in unit['relocations']}
        for off,target,stem in ((0x17,0x0040F810,'?HasTicked@ZunTimer'),
                                (0x15A,0x00403E00,'?SetAndExecuteScriptIdx@AnmLoaded'),
                                (0x1BF,0x0040CD70,'?SpawnEffectInFixedSlot@EffectManager')):
            with self.subTest(offset=off):
                self.assertEqual((rows[off]['type'],rows[off]['target']),('REL32',target))
                self.assertTrue(rows[off]['symbol'].startswith(stem))
        for off,target,stem in ((0xEB,0x0040F830,'??LZunTimer'),
                                (0x16B,0x004343D0,'??BPlayerPositionView')):
            with self.subTest(offset=off):
                self.assertEqual((rows[off]['type'],rows[off]['target']),('REL32',target))
                self.assertTrue(rows[off]['symbol'].startswith(stem))
        inspector=(ROOT/'scripts/inspect-enemy-manager-update.py').read_text()
        self.assertIn('PLAYER_DAMAGE_SYMBOL = '+repr(SYMBOL),inspector)
        self.assertIn('PLAYER_DAMAGE_FIELDS = (0x8BF, 0x8FF)',inspector)

if __name__=='__main__':unittest.main()

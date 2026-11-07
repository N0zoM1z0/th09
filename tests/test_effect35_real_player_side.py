"""TH09 exact Effect Type35 callbacks borrow real GameManager side/Player.

Complete PE/COFF comparison remains the exactness gate; these source tests
ensure the same native owner used by EffectManager::Create and SpawnShots.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class Effect35RealSidePlayer(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_both_callbacks_use_one_native_side_and_player(self):
        code=(ROOT/'src/EffectType35.cpp').read_text()
        native=(ROOT/'src/GameManagerSideState.hpp').read_text()
        player=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('#include "PlayerLifecycleView.hpp"',code)
        self.assertIn('EffectType35NativeSidePlayer',code)
        self.assertIn('sizeof(PlayerSideStateView) == 0x38',code)
        self.assertIn('offsetof(PlayerSideStateView, player04) == 0x04',code)
        self.assertEqual(code.count('reinterpret_cast<PlayerSideStateView *>(effect->manager->sideState)'),2)
        self.assertNotIn('struct EffectShotSideView',code)
        self.assertNotIn('PlayerSpawnShotsView *player04',code)
        self.assertIn('PlayerLifecycleView *player04;',native)
        self.assertIn('AnmLoaded *anmFile98;',player)
        self.assertIn('->header24.anmFile98->SetAndExecuteScriptIdx',code)
        self.assertIn('->SpawnShots(effect->timer, 2);',code)

    def test_original_exact_callee_and_effect_lifecycles(self):
        init=self.units['effect-initialize-type35']
        update=self.units['effect-update-type35']
        shot=self.units['player-spawn-shots']
        creator=self.units['effect-manager-create']
        self.assertEqual((init['size'],update['size'],shot['size'],creator['size']),
                         (72,38,190,327))
        self.assertEqual(shot['target_address'],0x0041F4C0)
        self.assertEqual(shot['symbol'],'?SpawnShots@PlayerLifecycleView@@QAIXHH@Z')
        refs=[r for r in update['relocations'] if r['symbol']==shot['symbol']]
        self.assertEqual(len(refs),1)
        self.assertEqual((refs[0]['offset'],refs[0]['target'],refs[0]['type']),
                         (0x1B,0x0041F4C0,'REL32'))
        self.assertEqual(creator['target_address'],0x0040D320)

if __name__=='__main__':unittest.main()

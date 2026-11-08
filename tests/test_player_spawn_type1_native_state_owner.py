"""Exact Type1 shot-spawn callback shares native Player state+0x3031C.

Complete original PE/COFF replay, rather than source shape, proves bytes.
"""
from pathlib import Path
import tomllib
import unittest
ROOT=Path(__file__).resolve().parents[1]

class Type1SpawnNativeState(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_native_shot_state_and_actual_0x3031c_alignment(self):
        src=(ROOT/'src/PlayerShotSpawnCallback1.cpp').read_text()
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('#include "PlayerLifecycleView.hpp"',src)
        self.assertNotIn('struct PlayerShotSpawn1StateView',src)
        self.assertNotIn('struct PlayerShotSpawn1PlayerView',src)
        self.assertIn('PlayerLifecycleView *player,',src)
        self.assertIn('PlayerShotCtorView *shot,',src)
        self.assertNotIn('PlayerShotSpawn1ShotView',src)
        self.assertIn('PlayerState3031CView state3031C;',native)
        self.assertNotIn('PlayerShotSpawn1PlayerView',src)
        self.assertIn('PlayerLifecycleView *player,',src)
        self.assertIn('PlayerShotCtorView *shot,',src)
        self.assertNotIn('PlayerShotSpawn1ShotView',src)
        self.assertIn('PlayerShotSpawn1RealNativeState',src)
        self.assertIn('offsetof(PlayerLifecycleView, state3031C) == 0x3031C',src)
        self.assertIn('offsetof(PlayerState3031CView, shot0C) == 0x0C',src)
        self.assertIn('player->state3031C.timer00 = 30;',src)
        self.assertIn('player->state3031C.shot0C = shot;',src)
        self.assertIn('PlayerShotCtorView *shot0C;',native)
        self.assertIn('PlayerState3031CView state3031C;',native)

    def test_original_complete_callback_and_exact_constructed_state(self):
        shot=self.units['player-shot-spawn-callback-type1']
        ctor=self.units['player-state3031c-constructor']
        self.assertEqual((shot['target_address'],shot['size'],len(shot['relocations'])),
                         (0x00441EF0,151,2))
        self.assertEqual((ctor['target_address'],ctor['size'],len(ctor['relocations'])),
                         (0x0041EE20,12,1))
        self.assertEqual(ctor['relocations'][0]['target'],0x00403390)
        self.assertEqual(self.units['player-constructor']['size'],243)

if __name__=='__main__':unittest.main()

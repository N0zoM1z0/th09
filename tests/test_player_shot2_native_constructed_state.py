"""Type-2 shot update uses actual constructed Player and Shot storage."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativePlayerShotType2(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source=(ROOT/'src/PlayerShotUpdateCallback2.cpp').read_text()
        cls.player=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_constructed_player_and_shot_without_duplicate_storage(self):
        s=self.source
        self.assertNotIn('struct PlayerShotUpdate2PlayerView\n{',s)
        self.assertNotIn('struct PlayerShotUpdate2ShotView\n{',s)
        self.assertIn('PlayerLifecycleView *lifecycle = reinterpret_cast<PlayerLifecycleView *>(player);',s)
        self.assertIn('PlayerShotCtorView *nativeShot = reinterpret_cast<PlayerShotCtorView *>(shot);',s)
        for item in ('state3031C.timer00','state3031C.shot0C','trailVectors2B0',
                     'hitbox430','velocity43C','field46A','vm.scale.y'):
            self.assertIn(item,s)
        self.assertIn('offsetof(PlayerShotCtorView, trailVectors2B0) == 0x2B0',s)
        self.assertIn('sizeof(PlayerShotCtorView) == 0x484',s)
        self.assertIn('PlayerState3031CView state3031C;',self.player)

    def test_real_collision_producer_and_original_call(self):
        s=self.source
        self.assertIn('lifecycle->CreateRectType4(',s)
        self.assertNotIn('player->CreateRectType4(',s)
        u=self.units['player-shot-update-callback-type2']
        self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),
                         (0x00441F90,643,16))
        call=[r for r in u['relocations'] if r['target']==0x0041D090]
        self.assertEqual(len(call),1)
        self.assertIn('CreateRectType4@PlayerLifecycleView',call[0]['symbol'])

if __name__=='__main__': unittest.main()

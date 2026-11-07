"""Three exact state-3 effects share the native Player Side/position/timer."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerState3EffectNativeStorage(unittest.TestCase):
    def test_one_real_player_owner_and_method_abi(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source=(ROOT/'src/PlayerState3EffectSetup.cpp').read_text()
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('struct PlayerLifecycleView',header)
        self.assertIn('PlayerLifecycleSizeIs30F70',header)
        self.assertIn('PlayerSideStateSizeIs38',header)
        self.assertIn('PlayerLifecyclePositionAt1B88',header)
        self.assertIn('PlayerLifecycleTimer303C8At303C8',header)
        self.assertNotIn('struct PlayerStateEffectSideView',header)
        self.assertNotIn('struct PlayerStateEffectPosition',header)
        self.assertNotIn('unknown00010[0x1B88',header)
        self.assertEqual(source.count('sideState->effectManager0C->SpawnEffect('),3)
        self.assertIn('PlayerSideStateView *sideState;',native)
        self.assertIn('#include "PlayerLifecycleView.hpp"',source)
        self.assertFalse((ROOT/'src/PlayerStateEffectView.hpp').exists())

    def test_three_original_bound_function_identifiers_stable(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        for suffix,size in ((13,91),(14,91),(15,100)):
            name=f'player-state3-effect{suffix}'
            with self.subTest(unit=name):
                self.assertEqual(u[name]['size'],size)
                self.assertEqual(len(u[name]['relocations']),5)
                self.assertEqual(u[name]['source'],'src/PlayerState3EffectSetup.cpp')

if __name__=='__main__':unittest.main()

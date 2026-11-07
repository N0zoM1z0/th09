"""Player State3 effects consume one real collision region source owner.

Physical addresses and byte identity are checked by the strict canonical
compiler/PE tools; this only protects the recovered C++ type relationship.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerState3NativeRegionOwner(unittest.TestCase):
    def test_true_region_header_not_redeclared_in_state_effects(self):
        source=(ROOT/'src/PlayerState3EffectSetup.cpp').read_text()
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        region=(ROOT/'src/PlayerCollisionRegionCreate.hpp').read_text()
        self.assertIn('PlayerState3NativeRegionProducer',source)
        self.assertIn('offsetof(PlayerLifecycleView, collisionRegionB0BC)',source)
        self.assertIn('offsetof(PlayerCollisionRegionCreateView, embeddedB0BC)',source)
        self.assertIn('offsetof(PlayerLifecycleView, activeB100)',source)
        self.assertIn('offsetof(PlayerCollisionRegionCreateView, activeB100)',source)
        self.assertIn('#include "PlayerCollisionRegionCreate.hpp"',native)
        self.assertIn('struct PlayerCollisionRegionCreateView',region)
        self.assertNotIn('struct PlayerCollisionRegionCreateView\n{',source)
        self.assertNotIn('struct PlayerRegionPointView\n{',source)
        self.assertEqual(source.count('->CreateCircleType1('),3)
        self.assertEqual(source.count('->CreateCircleType4('),3)

    def test_three_full_original_bound_state_effects(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        for num,size in ((13,91),(14,91),(15,100)):
            name=f'player-state3-effect{num}'
            with self.subTest(unit=name):
                unit=u[name]
                self.assertEqual(unit['source'],'src/PlayerState3EffectSetup.cpp')
                self.assertEqual(unit['size'],size)
                callee_targets=[r['target'] for r in unit['relocations']
                                if 'CreateCircleType' in r['symbol']]
                self.assertEqual(callee_targets,[0x0041CFE0,0x0041D0D0])
                self.assertTrue(any(r['target']==0x0040CC00
                                    for r in unit['relocations']))

if __name__=='__main__':unittest.main()

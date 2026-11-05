"""Native ANM sprite height shared by Etama initialization and the Enemy boss.

Source assertions complement original-bound complete 832-byte Etama
AddedCallback and the independent exact AnmLoaded::SetSprite carrier.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class EtamaNativeLoadedSprite(unittest.TestCase):
    def test_etama_uses_original_native_sprite_not_private_height_overlay(self):
        code=(ROOT/'src/EtamaControllerDraw.cpp').read_text()
        anm=(ROOT/'src/AnmManager.hpp').read_text()
        enemy=(ROOT/'src/EnemyManagerCore.inl').read_text()
        self.assertNotIn('struct EtamaLoadedSpriteView',code)
        self.assertIn('typedef char EtamaNativeSpriteHeightAt30',code)
        self.assertIn('offsetof(AnmLoadedSprite, heightPx) == 0x30',code)
        self.assertIn('AnmLoadedSprite *loadedSprite;', (ROOT/'src/AnmVmLifecycle.hpp').read_text())
        self.assertIn('->heightPx',code)
        self.assertNotIn('->height30',code)
        self.assertIn('float heightPx;',anm)
        self.assertIn('EnemyCoreLoadedSpriteNativeDimensions',enemy)

    def test_full_added_callback_original_extent_and_native_leaf_identity(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        added=units['etama-controller-added-callback']
        sprite=units['anm-loaded-set-sprite']
        self.assertEqual((added['target_address'],added['size'],
                          added['compare_size']),(0x00413E40,598,832))
        self.assertEqual((sprite['target_address'],sprite['size'],
                          len(sprite['relocations'])),(0x00436AC0,189,4))


if __name__=='__main__':
    unittest.main()

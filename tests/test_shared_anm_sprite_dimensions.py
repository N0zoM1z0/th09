"""Original-verified ANM sprite dimensions shared by Enemy boss and exact leaf.

Source-side checks enforce one typed 0x44-byte sprite object; separate focused
PE/COFF replays verify actual native VC7.1 owner bytes and all call fields.
"""

from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class SharedAnmSpriteDimensions(unittest.TestCase):
    def test_sprite_owner_recovers_original_dimension_and_uv_fields(self):
        owner = (ROOT/'src/AnmManager.hpp').read_text()
        self.assertIn('class AnmLoadedSprite', owner)
        for field in ('textureHeight18', 'textureWidth1C',
                      'uvStartX', 'uvStartY', 'uvEndX', 'uvEndY',
                      'heightPx', 'widthPx', 'scaleX38', 'scaleY3C'):
            with self.subTest(field=field):
                self.assertIn('float '+field+';', owner)
        self.assertIn('AnmLoadedSpriteSizeIs44', owner)
        self.assertNotIn('unknown030[0x04]', owner)
        self.assertNotIn('unknown038[0x0C]', owner)

    def test_native_set_sprite_leaf_uses_exact_loaded_sprite_owner(self):
        source = (ROOT/'src/AnmLoadedSetSprite.cpp').read_text()
        self.assertNotIn('struct AnmSetSpriteLoadedSpriteView', source)
        self.assertIn('AnmLoadedSprite *sprites0C;', (ROOT/'src/AnmManager.hpp').read_text())
        self.assertIn('AnmSetSpriteNativeLoadedFields', source)
        self.assertIn('AnmVm *target = vm;', source)
        self.assertIn('target->loadedSprite = &self->sprites0C[spriteIndex];', source)
        self.assertIn('offsetof(AnmLoadedSprite, textureHeight18) == 0x18', source)
        self.assertIn('offsetof(AnmLoadedSprite, textureWidth1C) == 0x1C', source)
        self.assertIn('offsetof(AnmLoadedSprite, heightPx) == 0x30', source)
        self.assertIn('offsetof(AnmLoadedSprite, widthPx) == 0x34', source)
        self.assertIn('target->loadedSprite->heightPx;', source)
        self.assertIn('target->loadedSprite->widthPx;', source)

        with (ROOT/'config/match-units.toml').open('rb') as stream:
            unit=tomllib.load(stream)['units']['anm-loaded-set-sprite']
        self.assertEqual((unit['target_address'],unit['size'],len(unit['relocations'])),
                         (0x00436AC0,189,4))

    def test_enemy_boss_consumes_native_sprite_not_fake_parallel_view(self):
        source = (ROOT/'src/EnemyManagerCore.inl').read_text()
        self.assertNotIn('struct EnemyCoreLoadedSpriteView', source)
        self.assertIn('AnmLoadedSprite *loadedSprite =', source)
        self.assertIn('EnemyCoreLoadedSpriteNativeDimensions', source)
        self.assertIn('offsetof(AnmLoadedSprite, heightPx) == 0x30', source)
        self.assertIn('offsetof(AnmLoadedSprite, widthPx) == 0x34', source)
        self.assertIn('enemy->primaryAnmVm0008.loadedSprite->heightPx', source)
        self.assertIn('enemy->primaryAnmVm0008.loadedSprite->widthPx', source)
        self.assertIn('g_GameManager.IsWithinPlayfield(', source)


    def test_player_bullet_uses_native_sprite_width_and_height(self):
        player=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        update=(ROOT/'src/PlayerRuntime.cpp').read_text()
        self.assertNotIn('struct PlayerLoadedSpriteView',player)
        self.assertIn('AnmLoadedSprite *loadedSprite224;',player)
        self.assertIn('PlayerRuntimeNativeBulletSpriteDimensions',update)
        self.assertIn('offsetof(AnmLoadedSprite, heightPx) == 0x30',update)
        self.assertIn('offsetof(AnmLoadedSprite, widthPx) == 0x34',update)
        self.assertIn('shot->vm.loadedSprite224->widthPx,',update)
        self.assertIn('shot->vm.loadedSprite224->heightPx))',update)
        self.assertNotIn('shot->vm.loadedSprite224->extent34',update)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            unit=tomllib.load(f)['units']['player-update-shots']
        self.assertEqual(unit['size'],251)

    def test_etama_native_sprite_dimensions_preserve_original_physical_reads(self):
        source = (ROOT/'src/BulletManager.cpp').read_text()
        self.assertIn('BulletNativeAnmLoadedSpriteDimensions', source)
        self.assertNotIn('struct BulletLoadedSpriteView', source)
        self.assertIn('offsetof(AnmLoadedSprite, heightPx) == 0x30', source)
        self.assertIn('offsetof(AnmLoadedSprite, widthPx) == 0x34', source)
        self.assertIn('sizeSource->loadedSprite;', source)
        self.assertIn('sprite->heightPx <= 16.0f', source)
        self.assertIn('sprite->heightPx <= 32.0f', source)
        self.assertIn('position[0], position[1], sprite->widthPx, sprite->heightPx)', source)
        self.assertIn('laser->width / laserSprite->widthPx;', source)
        self.assertIn('currentLength / laserSprite->heightPx;', source)


if __name__ == '__main__':
    unittest.main()

"""ANM native 0x44-byte sprite layout across the resource loader and SetSprite.

The source assertion only prevents parallel false storage classes; all
original-byte equality is independently checked by pinned VC7.1 units.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeLoadedSpriteResourceLayout(unittest.TestCase):
    def test_real_sprite_data_is_shared_without_private_loader_projection(self):
        loader=(ROOT/'src/AnmManagerLoad.cpp').read_text()
        header=(ROOT/'src/AnmManager.hpp').read_text()
        self.assertNotIn('struct AnmLoadedSpriteLoadView',loader)
        self.assertNotIn('struct Float2LoadView',loader)
        self.assertIn('AnmLoadedSpriteLoadNativeOffsets',loader)
        self.assertIn('sizeof(AnmLoadedSprite) == 0x44',loader)
        for field,offset in (
            ('startPixelX08','0x08'),('endPixelY14','0x14'),
            ('textureWidth1C','0x1C'),('heightPx','0x30'),
            ('widthPx','0x34'),('scaleX38','0x38')):
            with self.subTest(field=field):
                self.assertIn('offsetof(AnmLoadedSprite, '+field+') == '+offset,loader)
                self.assertIn(field,header)
        self.assertIn('AnmLoadedSprite loadedSprite;',loader)
        self.assertIn('AnmLoadedSprite *source = loadedSprite;',loader)
        self.assertIn('anm->sprites[spriteIndex] = *source;',loader)
        self.assertIn('anmLoadedObject->LoadSprite(currentSpriteNumber, &loadedSprite);',loader)
        self.assertNotIn('textureHeader->textureHeight18',loader)

    def test_uv_and_physical_extents_use_native_fields(self):
        loader=(ROOT/'src/AnmManagerLoad.cpp').read_text()
        for statement in (
            'anm->sprites[spriteIndex].uvStartX =',
            'anm->sprites[spriteIndex].uvEndX =',
            'anm->sprites[spriteIndex].uvStartY =',
            'anm->sprites[spriteIndex].uvEndY =',
            'anm->sprites[spriteIndex].textureWidth1C;',
            'anm->sprites[spriteIndex].textureHeight18;',
            'loadedSprite.startPixelX08 =',
            'loadedSprite.startPixelY0C =',
            'loadedSprite.endPixelX10 =',
            'loadedSprite.endPixelY14 =',
        ):
            with self.subTest(statement=statement):
                self.assertIn(statement,loader)

    def test_original_native_loader_and_callers_frozen(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for name,address,size,field_count in (
            ('anm-loaded-load-sprite',0x0043BCB0,138,0),
            ('anm-manager-load-texture-data',0x0043C800,558,16),
            ('anm-manager-create-texture-from-anm',0x0043C4C0,330,8),
            ('anm-loaded-set-sprite',0x00436AC0,189,4),
        ):
            with self.subTest(unit=name):
                u=units[name]
                self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),
                                 (address,size,field_count))

if __name__=='__main__':
    unittest.main()

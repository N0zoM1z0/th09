"""Two exact ANM text-rendering owners consume the real VM and sprite types.

The frozen physical functions are source-cold compared separately; this test
guards the shared native representation and the real texture/call interfaces.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class AnmNativeTextRuntime(unittest.TestCase):
    def test_typed_vm_and_sprite_replace_both_fake_text_views(self):
        code=(ROOT/'src/AnmManagerUtility.cpp').read_text()
        vm=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        sprite=(ROOT/'src/AnmManager.hpp').read_text()
        self.assertIn('#include "AnmVmLifecycle.hpp"',code)
        self.assertNotIn('struct AnmTextVmView',code)
        self.assertNotIn('struct AnmLoadedSpriteUtilityView',code)
        self.assertIn('AnmNativeTextSpriteAndVmOffsets',code)
        self.assertEqual(code.count('AnmVm *vm = vmObject;'),2)
        self.assertEqual(code.count('AnmLoadedSprite *sprite = vm->loadedSprite;'),2)
        self.assertEqual(code.count('static_cast<IDirect3DTexture8 *>(sprite->texture)'),2)
        for field,off in (
            ('flagsWord','0x1F8'),('loadedSprite','0x224'),
            ('fontWidth','0x298'),('fontHeight','0x299'),
        ):
            with self.subTest(field=field):
                self.assertIn('offsetof(AnmVm, '+field+') == '+off,code)
                self.assertIn(field,vm)
        for field,off in (
            ('startPixelX08','0x08'),('textureWidth1C','0x1C'),
            ('widthPx','0x34'),('scaleX38','0x38'),
        ):
            with self.subTest(field=field):
                self.assertIn('offsetof(AnmLoadedSprite, '+field+') == '+off,code)
                self.assertIn(field,sprite)

    def test_text_draw_owner_offsets_and_original_callees_frozen(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for name,target,size,fields in (
            ('anm-manager-draw-text-inner',0x0043BD40,127,4),
            ('anm-manager-draw-text-left',0x0043BDC0,139,6),
            ('anm-manager-draw-text-centered',0x0043BE50,217,9),
            ('anm-loaded-set-sprite',0x00436AC0,189,4),
        ):
            with self.subTest(name=name):
                u=units[name]
                self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),
                                 (target,size,fields))
        code=(ROOT/'src/AnmManagerUtility.cpp').read_text()
        self.assertIn('this->DrawTextInner(',code)
        self.assertEqual(code.count('vm->flagsWord |= 1;'),2)


if __name__=='__main__':
    unittest.main()

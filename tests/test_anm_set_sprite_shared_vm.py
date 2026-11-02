"""The exact 189-byte AnmLoaded::SetSprite operates on the shared native VM.

Source/layout assertions are supplemented by the pinned source-cold original
PE/COFF comparisons and intact 0x00401300 matrix callee identity.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeSetSpriteStorage(unittest.TestCase):
    def test_one_shared_vm_and_target_native_offsets(self):
        code=(ROOT/'src/AnmLoadedSetSprite.cpp').read_text()
        header=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('#include "AnmVmLifecycle.hpp"',code)
        self.assertNotIn('struct AnmSetSpriteVmView',code)
        self.assertIn('AnmVm *target = vm;',code)
        self.assertIn('typedef char AnmSetSpriteNativeVmOffsets',code)
        for member,offset in (
            ('spriteSize','0x28'), ('matrix1','0x130'), ('matrix2','0x170'),
            ('matrix3','0x1B0'), ('anmFile','0x204'),
            ('activeSpriteIndex','0x214'), ('loadedSprite','0x224')):
            with self.subTest(member=member):
                self.assertIn('offsetof(AnmVm, '+member+') == '+offset,code)
                self.assertIn(member,header)
        self.assertIn('AnmVmLifecycleSizeIs2A4',header)

    def test_original_matrix_method_and_loaded_sprite_are_still_true_callees(self):
        code=(ROOT/'src/AnmLoadedSetSprite.cpp').read_text()
        self.assertIn('reinterpret_cast<AnmMatrixRuntime *>(&target->matrix1)->SetIdentity();',code)
        self.assertIn('reinterpret_cast<AnmMatrixRuntime *>(&target->matrix3)->SetIdentity();',code)
        self.assertIn('target->loadedSprite = &self->sprites0C[spriteIndex];',code)
        self.assertIn('target->spriteSize.x = target->loadedSprite->widthPx;',code)
        self.assertIn('target->spriteSize.y = target->loadedSprite->heightPx;',code)
        self.assertIn('target->matrix2 = target->matrix1;',code)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        set_sprite=units['anm-loaded-set-sprite']
        self.assertEqual((set_sprite['target_address'],set_sprite['size'],
                          len(set_sprite['relocations'])),(0x00436AC0,189,4))
        self.assertEqual([x['target'] for x in set_sprite['relocations'][:2]],[0x00401300,0x00401300])
        script=units['anm-manager-execute-script']
        self.assertEqual((script['target_address'],script['size'],
                          script['compare_size'],len(script['relocations'])),
                         (0x00436F30,9358,9776,368))


if __name__=='__main__':
    unittest.main()

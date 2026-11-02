"""Original VM+0x216/+0x218 sprite indices shared with Etama initialization.

This tests the single native 0x228-byte VM prefix; source-cold Etama and VM
constructor original-byte comparisons run separately.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeAnmVmSpriteIndices(unittest.TestCase):
    def test_vm_prefix_reconstructs_native_file_and_base_sprite_indices(self):
        header=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        etama=(ROOT/'src/EtamaControllerDraw.cpp').read_text()
        runtime=(ROOT/'src/AnmManagerRuntime.cpp').read_text()
        self.assertIn('short anmFileIndex;',header)
        self.assertIn('short baseSpriteIndex;',header)
        self.assertIn('AnmVmNativeFileIndexAt216',header)
        self.assertIn('AnmVmNativeBaseSpriteAt218',header)
        self.assertIn('offsetof(AnmVmBase, anmFileIndex) == 0x216',header)
        self.assertIn('offsetof(AnmVmBase, baseSpriteIndex) == 0x218',header)
        self.assertNotIn('unknown_0216[4]',header)
        self.assertIn('sprites->bulletVm.baseSpriteIndex = sprites->bulletVm.activeSpriteIndex;',etama)
        self.assertNotIn('inline short &BaseSpriteIndex(',etama)
        self.assertNotIn('reinterpret_cast<unsigned char *>(vm) + 0x218',etama)
        self.assertIn('vm->baseSpriteIndex = vm->activeSpriteIndex;',runtime)

    def test_original_vm_constructor_and_etama_callee_fields_pinned(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        for name,target,length,fields in (
            ('anm-vm-constructor',0x00403A10,45,2),
            ('etama-controller-added-callback',0x00413E40,598,30),
        ):
            with self.subTest(name=name):
                u=units[name]
                self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),
                                 (target,length,fields))


if __name__=='__main__':
    unittest.main()

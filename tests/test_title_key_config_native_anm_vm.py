"""The original 2803-byte Title key-config update uses native 0x2A4 ANM VMs."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleKeyConfigNativeAnmVm(unittest.TestCase):
    def test_real_vm_storage_and_script_color_are_shared(self):
        source=(ROOT/'src/TitleScreenKeyConfig.cpp').read_text()
        native=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('#include "AnmVmLifecycle.hpp"',source)
        self.assertIn('struct AnmVmView : AnmVm {};',source)
        self.assertIn('sizeof(AnmVmView) == sizeof(AnmVm)',source)
        self.assertNotIn('union TitleKeyColor',source)
        self.assertNotIn('unknown000[0x1F0]',source)
        self.assertNotIn('reinterpret_cast<AnmVm *>(vms)',source)
        self.assertIn('AnmVmView helpTextVms[57];',source)
        for field,offset in (('color1Script','0x1F0'),
                             ('pendingInterrupt','0x1FE'),
                             ('baseSpriteIndex','0x218')):
            self.assertIn(f'offsetof(AnmVmView, {field}) == {offset}',source)
        self.assertEqual(source.count('.color1Script.d3dColor'),20)
        self.assertIn('AnmVmScriptColor color1Script;',native)

    def test_original_key_config_and_swap_leaf_remain_distinct(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        self.assertEqual((u['title-screen-update-key-config']['size'],
                          len(u['title-screen-update-key-config']['relocations'])),(2803,118))
        self.assertEqual((u['title-screen-swap-duplicate-character-index']['size'],
                          len(u['title-screen-swap-duplicate-character-index']['relocations'])),(106,0))

if __name__=='__main__':unittest.main()

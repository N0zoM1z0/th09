"""The exact FrontSide HUD uses one native ANM VM/color record."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]

class FrontSideNativeAnmVm(unittest.TestCase):
    def test_native_vm_fields_replace_second_2a4_byte_view(self):
        src = (ROOT/'src/FrontSide.cpp').read_text()
        header = (ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('FrontSideNativeVmMemberPositions', src)
        self.assertNotIn('struct FrontSideVmView', src)
        self.assertNotIn('struct FrontSideFloat3View', src)
        self.assertNotIn('struct FrontSideFloat2View', src)
        self.assertNotIn('union FrontSideColorView', src)
        self.assertNotIn('FrontSideVm(', src)
        self.assertIn('offsetof(AnmVm, scale) == 0x18', src)
        self.assertIn('offsetof(AnmVm, flagsWord) == 0x1F8', src)
        self.assertIn('offsetof(AnmVm, pos) == 0x208', src)
        self.assertIn('offsetof(AnmVm, pos2) == 0x288', src)
        self.assertIn('color1Script',src)
        self.assertIn('struct AnmVmScriptColor',header)
        self.assertIn('union { unsigned long color1; AnmVmScriptColor color1Script; unsigned char color1Bytes[4]; }',header)

    def test_original_front_side_exact_owner_sizes_unchanged(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertEqual(units['front-side-on-update']['size'],2381)
        self.assertEqual(units['front-side-added-callback']['size'],460)
        self.assertEqual(units['front-side-create']['size'],283)

if __name__=='__main__':
    unittest.main()

"""Inner render bosses use actual AnmVm fields, never a duplicated VM layout."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class AnmNativeInnerVm(unittest.TestCase):
    def test_native_vm_inner_fields_and_layout(self):
        source=(ROOT/'src/AnmManagerRender.cpp').read_text()
        vm=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('AnmManagerRenderNativeInnerVmWords',source)
        for member,offset in [('color1','0x1F0'),('color2','0x1F4'),
                              ('flagsWord','0x1F8'),('loadedSprite','0x224')]:
            with self.subTest(member=member):
                self.assertIn('offsetof(AnmVmBase, '+member+') == '+offset,source)
        self.assertIn('vm->color1',source)
        self.assertIn('vm->color2',source)
        self.assertIn('vm->loadedSprite',source)
        self.assertNotIn('AnmVmDrawInnerLayout',source)
        self.assertNotIn('drawVm->',source)
        self.assertIn('unsigned long color1',vm)
        self.assertIn('unsigned long color2',vm)

    def test_original_source_owner_bounds(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        render=[u for u in units.values() if u.get('source')=='src/AnmManagerRender.cpp']
        self.assertEqual(len(render),16)
        self.assertEqual(sum(u.get('compare_size',u['size']) for u in render),5814)
        self.assertEqual(sum(len(u.get('relocations',[])) for u in render),395)
        for name in ('anm-manager-draw-inner','anm-manager-draw-inner-flipped-x'):
            with self.subTest(name=name):
                self.assertEqual(units[name].get('compare_size',units[name]['size']),1067)


if __name__=='__main__':
    unittest.main()

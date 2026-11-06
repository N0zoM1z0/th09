"""Guard genuine AnmVm use in primitive drawing and its exact target owners."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeAnmPrimitiveVm(unittest.TestCase):
    def test_no_duplicate_vm_layout_or_pointer_reinterpret(self):
        src = (ROOT/'src/AnmManagerPrimitiveDraw.cpp').read_text()
        header = (ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('PrimitiveNativeVmDrawFields',src)
        self.assertIn('offsetof(AnmVmBase, flagsWord) == 0x1F8',src)
        self.assertIn('offsetof(AnmVmBase, loadedSprite) == 0x224',src)
        self.assertIn('offsetof(AnmVmScriptColor, a) == 0x1F3',src)
        self.assertNotIn('struct AnmVmPrimitiveView',src)
        self.assertNotIn('reinterpret_cast<AnmVmPrimitiveView',src)
        self.assertGreaterEqual(src.count('vmObject->flagsWord'),2)
        self.assertGreaterEqual(src.count('vmObject->color1Script.a'),2)
        self.assertGreaterEqual(src.count('vmObject->loadedSprite'),3)
        self.assertIn('AnmLoadedSprite *loadedSprite;',header)

    def test_original_complete_primitive_carrier_sizes(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        group=[u for u in units.values()
               if u.get('source')=='src/AnmManagerPrimitiveDraw.cpp']
        self.assertEqual(len(group),12)
        self.assertEqual(sum(u.get('compare_size',u['size']) for u in group),1763)
        self.assertEqual(sum(len(u.get('relocations',[])) for u in group),90)
        self.assertTrue(all(u['object']=='build/matching/AnmManagerPrimitiveDraw.obj'
                            for u in group))


if __name__=='__main__':
    unittest.main()

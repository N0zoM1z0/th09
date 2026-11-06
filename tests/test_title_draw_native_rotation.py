"""Require Title Draw to use the native animation VM rotation member."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class TitleDrawNativeRotation(unittest.TestCase):
    def test_native_vm_rotation_z_not_reinterpreted_byte_offset(self):
        title = (ROOT / 'src/TitleDrawCallback.cpp').read_text()
        native = (ROOT / 'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('vm->rotation.z != 0.0f', title)
        self.assertIn('TitleDrawNativeVmRotationZAt08', title)
        self.assertIn('offsetof(AnmVmBase, rotation) + offsetof(Float3, z) == 0x08', title)
        self.assertIn('Float3 rotation;', native)
        self.assertNotIn('reinterpret_cast<unsigned char *>(vm) + 0x08', title)

    def test_actual_canonical_function_span_and_referents(self):
        with (ROOT / 'config/match-units.toml').open('rb') as f:
            owner = tomllib.load(f)['units']['title-draw-callback']
        self.assertEqual(owner['source'], 'src/TitleDrawCallback.cpp')
        self.assertEqual(owner['target_address'], 0x00424898)
        self.assertEqual(owner['size'], 329)
        self.assertEqual(len(owner['relocations']), 20)


if __name__ == '__main__':
    unittest.main()

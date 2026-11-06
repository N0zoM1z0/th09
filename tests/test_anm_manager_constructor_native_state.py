"""Keep native ANM constructor storage typed, with original offsets pinned."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class AnmManagerConstructorState(unittest.TestCase):
    def test_known_device_states_have_native_layout_and_initializers(self):
        header = (ROOT/'src/AnmManager.hpp').read_text()
        source = (ROOT/'src/AnmManagerLoad.cpp').read_text()
        names = {
            'currentTextureFactor1287C': '0x1287C',
            'currentTexture12880': '0x12880',
            'currentBlendMode12884': '0x12884',
            'unknown12885': '0x12885',
            'currentVertexShader12886': '0x12886',
            'disableZWrite12887': '0x12887',
            'cameraMode12888': '0x12888',
            'quadVertexBuffer12890': '0x12890',
            'field2B28F0': '0x2B28F0',
        }
        self.assertIn('NativeCtorRenderStateOffsets', source)
        for name, offset in names.items():
            with self.subTest(member=name):
                self.assertIn(name, header)
                self.assertIn('offsetof(AnmManager, '+name+') == '+offset, source)
        for statement in ('quadVertexBuffer12890 = NULL;',
                          'currentTexture12880 = NULL;',
                          'currentTextureFactor1287C = 1;',
                          'field2B28F0 = -1;',
                          'cameraMode12888 = 0xFF;'):
            self.assertIn(statement, source)

    def test_raw_offset_writes_are_not_reintroduced_for_typed_members(self):
        source = (ROOT/'src/AnmManagerLoad.cpp').read_text()
        constructor = source.split('AnmManager::AnmManager()', 1)[1].split(
            '\nstruct D3DXIMAGE_INFO', 1)[0]
        for offset in ('0x1287C', '0x12880', '0x12884', '0x12885',
                       '0x12886', '0x12887', '0x12888', '0x12890', '0x2B28F0'):
            with self.subTest(offset=offset):
                self.assertNotIn('self + '+offset, constructor)
                self.assertNotIn('self['+offset+']', constructor)


if __name__ == '__main__':
    unittest.main()

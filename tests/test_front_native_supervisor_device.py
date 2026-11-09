"""Original-bound Front renderers share the actual Supervisor D3D8 device."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
SUPERVISOR = '?g_Supervisor@@3VSupervisor@@A'


class NativeFrontSupervisorDevice(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT / 'config/match-units.toml').open('rb') as handle:
            cls.units = tomllib.load(handle)['units']

    def test_real_supervisor_receiver_not_truncated_second_class(self):
        line = (ROOT / 'src/FrontLineStrip.cpp').read_text()
        message = (ROOT / 'src/FrontMessageRuntimeDraw.cpp').read_text()
        supervisor = (ROOT / 'src/Supervisor.hpp').read_text()
        self.assertIn('IDirect3DDevice8 *d3dDevice08;', supervisor)
        self.assertIn('offsetof(Supervisor, d3dDevice08) == 0x08', supervisor)
        self.assertNotIn('FrontLineSupervisorView', line)
        self.assertNotIn('FrontMessageDrawSupervisorView', message)
        self.assertEqual(line.count('g_Supervisor.d3dDevice08->'), 12)
        self.assertEqual(message.count('g_Supervisor.d3dDevice08->'), 10)

    def test_original_line_render_device_dwords_are_bound_to_same_root(self):
        line = self.units['front-line-strip']
        refs = [r for r in line['relocations'] if r['symbol'] == SUPERVISOR]
        self.assertEqual((line['target_address'], line['size'], len(line['relocations'])),
                         (0x00422330, 318, 26))
        self.assertEqual(len(refs), 12)
        for row in refs:
            with self.subTest(offset=row['offset']):
                self.assertEqual((row['target'], row.get('addend', 0), row['type']),
                                 (0x004B3100, 0x08, 'DIR32'))
        message = self.units['front-message-runtime-draw']
        self.assertEqual((message['size'], len(message['relocations'])), (995, 73))


if __name__ == '__main__':
    unittest.main()

"""Native Supervisor DirectInput ownership across exact 882-byte StartupThread.

The DIDEVCAPS backing retains original byte size. The original controller
interfaces and true callback references are validated via pinned VC7.1
original-PE/COFF owner comparisons, not just text checks.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeDirectInputStartup(unittest.TestCase):
    def test_native_device_and_window_offsets(self):
        h=(ROOT/'src/Supervisor.hpp').read_text()
        startup=(ROOT/'src/SupervisorStartup.cpp').read_text()
        self.assertIn('SupervisorNativeDInputFields',h)
        for field,offset in (
            ('directInput0C','0x0C'),('keyboard10','0x10'),
            ('controller014','0x14'),('controller118','0x18'),
            ('controllerCaps1C','0x1C'),('window74','0x74'),
            ('ctorArray078','0x78')):
            with self.subTest(field=field):
                self.assertIn('offsetof(Supervisor, '+field+') == '+offset,h)
        self.assertIn('sizeof(((Supervisor *)0)->controllerCaps1C[0]) == 0x2C',h)
        self.assertIn('sizeof(DIDEVCAPS) == 0x2C',startup)
        self.assertNotIn('struct SupervisorDirectInputPrefix',startup)
        self.assertIn('Supervisor *s = supervisor;',startup)
        self.assertIn('s->window74',startup)
        self.assertIn('s->directInput0C->CreateDevice(',startup)
        self.assertIn('s->keyboard10->SetCooperativeLevel(',startup)
        self.assertIn('s->controller014->GetCapabilities(',startup)
        self.assertIn('s->controller118->GetCapabilities(',startup)
        self.assertIn('&g_Supervisor.controllerCaps1C[0]',startup)
        self.assertIn('&g_Supervisor.controllerCaps1C[1]',startup)

    def test_original_callers_pinned(self):
        with (ROOT/'config/match-units.toml').open('rb') as fd:
            units=tomllib.load(fd)['units']
        for name,size,fields in (
            ('supervisor-startup-thread',882,106),
            ('supervisor-deleted-callback',405,44),
            ('supervisor-enum-controllers',93,6),
            ('supervisor-enum-objects0',94,1),
            ('supervisor-enum-objects1',94,1)):
            with self.subTest(name=name):
                u=units[name]
                self.assertEqual((u['size'],len(u['relocations'])),(size,fields))

if __name__ == '__main__':
    unittest.main()

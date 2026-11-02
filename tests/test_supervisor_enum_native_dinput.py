"""Original controller enumeration callbacks use the native Supervisor DInput.

The three separately compiled callback functions are verified by actual
target PE/VC7.1 COFF comparisons; this source test protects ownership/ABI.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeSupervisorEnumeration(unittest.TestCase):
    def test_enum_callbacks_access_original_native_device_slots(self):
        source=(ROOT/'src/SupervisorEnumControllers.cpp').read_text()
        header=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertNotIn('struct SupervisorEnumControllerLayout',source)
        self.assertIn('SupervisorEnumController0At14',source)
        self.assertIn('SupervisorEnumController1At18',source)
        self.assertEqual(source.count('Supervisor *supervisor = &g_Supervisor;'),3)
        for name in ('controller014','controller118','directInput0C'):
            self.assertIn('supervisor->'+name,source)
            self.assertIn(name,header)
        self.assertNotIn('reinterpret_cast<SupervisorEnumControllerLayout *>',source)
        self.assertIn('BOOL CALLBACK StartupEnumControllers(',source)
        self.assertIn('BOOL CALLBACK StartupEnumObjects0(',source)
        self.assertIn('BOOL CALLBACK StartupEnumObjects1(',source)
        self.assertIn('SetProperty(DIPROP_RANGE, &range.diph)',source)

    def test_canonical_three_callback_extents_are_frozen(self):
        with (ROOT/'config/match-units.toml').open('rb') as file:
            u=tomllib.load(file)['units']
        for name,size,fields in (
            ('supervisor-enum-controllers',93,6),
            ('supervisor-enum-objects0',94,1),
            ('supervisor-enum-objects1',94,1)):
            with self.subTest(name=name):
                unit=u[name]
                self.assertEqual(unit['source'],'src/SupervisorEnumControllers.cpp')
                self.assertEqual((unit['size'],len(unit['relocations'])),(size,fields))

if __name__=='__main__':
    unittest.main()

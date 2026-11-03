"""Physical TH09 Supervisor DInput array shared with Controller polling."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class ControllerNativeSupervisorDevices(unittest.TestCase):
    def test_same_original_device_window_is_used_by_polling_and_startup(self):
        source = (ROOT/'src/ControllerInput.cpp').read_text()
        header = (ROOT/'src/Supervisor.hpp').read_text()
        self.assertNotIn('struct ControllerSupervisorDevicesView', source)
        self.assertNotIn('ControllerDevices().controllers14', source)
        self.assertIn('ControllerNativeDeviceSlotsAt14', source)
        self.assertEqual(source.count('g_Supervisor.controllers14['), 6)
        for native_slot in ('controller014', 'controller118'):
            self.assertIn('IDirectInputDevice8A *'+native_slot+';', header)
        self.assertIn('IDirectInputDevice8A *controllers14[2];', header)
        self.assertIn('offsetof(Supervisor, controllers14) == 0x14', header)
        self.assertIn('offsetof(Supervisor, controller014) == 0x14', header)
        self.assertIn('offsetof(Supervisor, controller118) == 0x18', header)
        self.assertIn('sizeof(((Supervisor *)0)->controllers14) == 8', header)
        # The exact setup callee reads both old individual member spellings;
        # those coexist with the same array used by the non-exact poll boss.
        startup = (ROOT/'src/SupervisorStartup.cpp').read_text()
        self.assertIn('s->controller014->GetCapabilities(', startup)
        self.assertIn('SupervisorNativeDInputFields', header)


if __name__ == '__main__':
    unittest.main()

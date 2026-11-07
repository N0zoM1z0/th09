"""The exact GameManager capture method receives the one shared 3-float position."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SupervisorSharedLoadingPosition(unittest.TestCase):
    def test_position_is_single_native_record(self):
        header=(ROOT/'src/SupervisorSetupLayoutView.hpp').read_text()
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        self.assertIn('struct SetupLoadingPosition',header)
        self.assertIn('sizeof(SetupLoadingPosition) == sizeof(Float3)',header)
        for field in ('x','y','z'):
            self.assertIn(f'offsetof(SetupLoadingPosition, {field}) == offsetof(Float3, {field})',header)
        self.assertNotIn('struct SetupLoadingPosition\n{',source)
        self.assertIn('sizeof(SetupLoadingPosition) == 0x0C',source)
        self.assertIn('void SetupLoadingVmsAndInitCapture(SetupLoadingPosition *position)',header)
        self.assertIn('SupervisorSetupLayout::SetupLoadingVmsAndInitCapture(',source)

    def test_callee_keeps_independent_original_extent(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        unit=units['supervisor-setup-loading-vms-and-capture']
        self.assertEqual((unit['target_address'],unit['size']), (0x004303C0,214))

if __name__=='__main__':unittest.main()

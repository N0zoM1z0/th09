"""The 214-byte native capture installer and gameplay share one 12-byte source record."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeSupervisorCapturePosition(unittest.TestCase):
    def test_native_loading_position_is_shared_and_offset_backed(self):
        shared=(ROOT/'src/SupervisorSetupLayoutView.hpp').read_text()
        game=(ROOT/'src/GameManagerSetup.cpp').read_text()
        self.assertIn('struct SetupLoadingPosition\n{',shared)
        self.assertIn('sizeof(SetupLoadingPosition) == sizeof(Float3)',shared)
        for member in ('x','y','z'):
            self.assertIn('offsetof(SetupLoadingPosition, '+member+') == offsetof(Float3, '+member+')',shared)
        self.assertNotIn('struct SetupLoadingPosition\n{',game)
        self.assertIn('#include "SupervisorSetupLayoutView.hpp"',game)
        self.assertIn('sizeof(SetupLoadingPosition) == 0x0C',game)
        self.assertIn('void SupervisorSetupLayout::SetupLoadingVmsAndInitCapture(',game)

    def test_original_bound_callee_and_parent_update(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:units=tomllib.load(stream)['units']
        callee=units['supervisor-setup-loading-vms-and-capture']
        self.assertEqual((callee['target_address'],callee['size'],len(callee['relocations'])),(0x004303C0,214,17))
        self.assertEqual((units['game-manager-on-update']['size'],units['game-manager-added-callback']['size']),(1230,187))

if __name__=='__main__':unittest.main()

"""A real 0x7B0-byte Supervisor underlies GameManager and Title loading APIs."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class GameManagerSharedSupervisorSetupView(unittest.TestCase):
    def test_shared_native_object_and_capture_method_abi(self):
        game=(ROOT/'src/GameManagerSetup.cpp').read_text()
        shared=(ROOT/'src/SupervisorSetupLayoutView.hpp').read_text()
        loading=(ROOT/'src/SupervisorLoadingSetupView.hpp').read_text()
        self.assertIn('#include "SupervisorSetupLayoutView.hpp"',game)
        self.assertNotIn('struct SupervisorSetupLayout :',game)
        self.assertIn('struct SupervisorSetupLayout : SupervisorLoadingSetupView',shared)
        self.assertIn('struct SupervisorLoadingSetupView : SupervisorThreadStartView',loading)
        self.assertIn('sizeof(SupervisorSetupLayout) == sizeof(Supervisor)',shared)
        self.assertIn('offsetof(SupervisorSetupLayout, loadingAnm5D0) == 0x5D0',shared)
        self.assertIn('offsetof(SupervisorSetupLayout, loadingVmsHaveBeenSetup) == 0x740',shared)
        self.assertIn('void SetupLoadingVmsAndInitCapture(SetupLoadingPosition *position);',shared)
        self.assertIn('SupervisorSetupLayout::SetupLoadingVmsAndInitCapture(',game)

    def test_native_exact_callee_and_big_caller(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        capture=units['supervisor-setup-loading-vms-and-capture']
        self.assertEqual((capture['size'],len(capture['relocations'])),(214,17))
        for name,size in (('game-manager-on-update',1230),('game-manager-added-callback',187)):
            self.assertEqual(units[name]['size'],size)
            self.assertEqual(units[name]['source'],'src/GameManagerSetup.cpp')

if __name__=='__main__':unittest.main()

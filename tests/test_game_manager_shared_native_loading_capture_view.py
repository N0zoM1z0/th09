"""GameManager's exact 214-byte capture installer uses the shared Supervisor class."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SharedSupervisorLoadingCapture(unittest.TestCase):
    def test_one_supervisor_allocation_and_original_method_identity(self):
        impl=(ROOT/'src/GameManagerSetup.cpp').read_text()
        head=(ROOT/'src/SupervisorSetupLayoutView.hpp').read_text()
        native=(ROOT/'src/SupervisorLoadingSetupView.hpp').read_text()
        self.assertIn('#include "SupervisorSetupLayoutView.hpp"',impl)
        self.assertNotIn('struct SupervisorSetupLayout :',impl)
        self.assertIn('struct SupervisorSetupLayout : SupervisorLoadingSetupView',head)
        self.assertIn('struct SupervisorLoadingSetupView : SupervisorThreadStartView',native)
        self.assertIn('sizeof(SupervisorSetupLayout) == sizeof(Supervisor)',head)
        self.assertIn('offsetof(SupervisorSetupLayout, loadingAnm5D0) == 0x5D0',head)
        self.assertIn('offsetof(SupervisorSetupLayout, loadingVmsHaveBeenSetup) == 0x740',head)
        self.assertIn('SupervisorSetupLayout::SetupLoadingVmsAndInitCapture(',impl)

    def test_true_exact_callee_and_boss_extents(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        item=u['supervisor-setup-loading-vms-and-capture']
        self.assertEqual((item['target_address'],item['size']),(0x004303C0,214))
        self.assertIn('SupervisorSetupLayout',item['symbol'])
        self.assertEqual(u['game-manager-on-update']['size'],1230)
        self.assertEqual(u['game-manager-added-callback']['size'],187)

if __name__=='__main__':unittest.main()

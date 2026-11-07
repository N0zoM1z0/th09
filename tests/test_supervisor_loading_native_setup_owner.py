"""The exact loading-VM initializer uses native Supervisor ANM and flags."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SupervisorLoadingNativeSetupOwner(unittest.TestCase):
    def test_173_byte_method_reuses_supervisor_allocation(self):
        source=(ROOT/'src/SupervisorLoadingVms.cpp').read_text()
        shared=(ROOT/'src/SupervisorLoadingSetupView.hpp').read_text()
        self.assertIn('#include "SupervisorLoadingSetupView.hpp"',source)
        self.assertIn('struct SupervisorLoadingSetupView : SupervisorThreadStartView',shared)
        self.assertIn('sizeof(SupervisorLoadingSetupView) == sizeof(Supervisor)',shared)
        self.assertIn('offsetof(SupervisorLoadingSetupView, loadingAnm5D0) == 0x5D0',shared)
        self.assertIn('offsetof(SupervisorLoadingSetupView, loadingVmsHaveBeenSetup) == 0x740',shared)
        self.assertNotIn('unsigned char unknown000[0x5D0]',source)
        self.assertEqual(source.count('this->loadingAnm5D0->ExecuteAnmIdx'),3)
        self.assertIn('void SupervisorLoadingSetupView::SetupLoadingVms(Float3 *position)',source)

    def test_true_original_leaf_identity(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        v=units['supervisor-setup-loading-vms']
        self.assertEqual((v['target_address'],v['size']),(0x004302B0,173))
        self.assertEqual(v['symbol'],'?SetupLoadingVms@SupervisorLoadingSetupView@@QAEXPAUFloat3@@@Z')

if __name__=='__main__':unittest.main()

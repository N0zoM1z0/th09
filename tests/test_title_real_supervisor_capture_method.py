"""Original Title AddedCallback binds the independent exact capture installer."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleRealSupervisorCaptureMethod(unittest.TestCase):
    def test_one_shared_native_supervisor_capture_method(self):
        title=(ROOT/'src/TitleScreen.cpp').read_text()
        titleview=(ROOT/'src/TitleSupervisorRuntimeView.hpp').read_text()
        native=(ROOT/'src/SupervisorSetupLayoutView.hpp').read_text()
        self.assertIn('struct TitleSupervisorView : SupervisorSetupLayout',titleview)
        self.assertIn('struct SupervisorSetupLayout : SupervisorLoadingSetupView',native)
        self.assertNotIn('void SetupLoadingVmsAndInitCapture(TitleFloat3View *position);',titleview)
        self.assertIn('sizeof(TitleFloat3View) == sizeof(SetupLoadingPosition)',title)
        for coordinate in ('x','y','z'):
            self.assertIn('offsetof(TitleFloat3View, '+coordinate+') == offsetof(SetupLoadingPosition, '+coordinate+')',title)
        self.assertIn('g_TitleSupervisor.SetupLoadingVmsAndInitCapture(reinterpret_cast<SetupLoadingPosition *>(&loadingPosition));',title)

    def test_true_original_call_target_and_field_unchanged(self):
        with (ROOT/'config/match-units.toml').open('rb') as inp:units=tomllib.load(inp)['units']
        owner=units['title-screen-actual-added-callback']
        callee=units['supervisor-setup-loading-vms-and-capture']
        self.assertEqual((owner['size'],len(owner['relocations'])),(335,27))
        self.assertEqual((callee['size'],callee['target_address']),(214,0x004303C0))
        site=next(x for x in owner['relocations'] if x['offset']==0x10D)
        self.assertEqual((site['symbol'],site['target'],site['type']),
                         (callee['symbol'],callee['target_address'],'REL32'))

if __name__=='__main__':unittest.main()

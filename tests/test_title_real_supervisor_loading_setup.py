"""The original Title AddedCallback uses the actual 173-byte Supervisor VM loader."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleNativeSupervisorLoadingSetup(unittest.TestCase):
    def test_source_faces_one_real_0x7b0_supervisor(self):
        title=(ROOT/'src/TitleScreen.cpp').read_text()
        owner=(ROOT/'src/TitleSupervisorRuntimeView.hpp').read_text()
        native=(ROOT/'src/SupervisorLoadingSetupView.hpp').read_text()
        self.assertIn('struct TitleSupervisorView : SupervisorLoadingSetupView',owner)
        self.assertIn('struct SupervisorLoadingSetupView : SupervisorThreadStartView',native)
        self.assertNotIn('void SetupLoadingVms(TitleFloat3View *position);',owner)
        self.assertIn('sizeof(TitleFloat3View) == sizeof(Float3)',title)
        for field in ('x','y','z'):
            self.assertIn('offsetof(TitleFloat3View, '+field+') == offsetof(Float3, '+field+')',title)
        self.assertIn('g_TitleSupervisor.SetupLoadingVms(reinterpret_cast<Float3 *>(&loadingPosition));',title)

    def test_true_original_callee_target_is_preserved(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        leaf=units['supervisor-setup-loading-vms']
        caller=units['title-screen-actual-added-callback']
        self.assertEqual((leaf['target_address'],leaf['size']),(0x004302B0,173))
        self.assertEqual((caller['size'],len(caller['relocations'])),(335,27))
        site=next(r for r in caller['relocations'] if r['offset']==0x126)
        self.assertEqual((site['type'],site['target'],site['symbol']),
                         ('REL32',leaf['target_address'],leaf['symbol']))

if __name__=='__main__':unittest.main()

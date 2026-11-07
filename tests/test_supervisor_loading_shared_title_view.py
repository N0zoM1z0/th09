"""TitleScreen and loading transition exact leaves share the same native Supervisor."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeSupervisorLoadingSharedTitleView(unittest.TestCase):
    def test_single_native_title_supervisor_for_two_exact_source_owners(self):
        title=(ROOT/'src/TitleScreen.cpp').read_text()
        loading=(ROOT/'src/SupervisorLoadingTransitions.cpp').read_text()
        shared=(ROOT/'src/TitleSupervisorRuntimeView.hpp').read_text()
        self.assertIn('#include "TitleSupervisorRuntimeView.hpp"',title)
        self.assertIn('#include "TitleSupervisorRuntimeView.hpp"',loading)
        self.assertIn('struct TitleSupervisorView : SupervisorLoadingSetupView',shared)
        self.assertIn('sizeof(TitleSupervisorView) == sizeof(Supervisor)',shared)
        self.assertNotIn('struct TitleSupervisorView',loading)
        self.assertNotIn('struct SupervisorLoadingFinishView',loading)
        self.assertNotIn('unknown000[0x740]',loading)
        self.assertIn('TitleSupervisorView::HideLoadingVms()',loading)
        self.assertIn('if (loadingVmsHaveBeenSetup == 1)',loading)
        self.assertIn('if (supervisor->loadingVmsHaveBeenSetup == 1)',loading)
        self.assertIn('supervisor->loadingVmsHaveBeenSetup = 2;',loading)

    def test_both_original_bound_loading_methods_unchanged(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        for n,size in (('supervisor-hide-loading-vms',85),('supervisor-finish-loading',85)):
            with self.subTest(n=n):
                self.assertEqual(units[n]['size'],size)
                self.assertEqual(units[n]['source'],'src/SupervisorLoadingTransitions.cpp')
        self.assertEqual(units['title-screen-start-menu']['size'],1560)
        self.assertEqual(units['title-screen-actual-added-callback']['size'],335)

if __name__=='__main__': unittest.main()

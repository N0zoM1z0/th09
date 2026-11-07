"""Exact Supervisor effect-start leaf borrows the true Supervisor allocation."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SupervisorStartEffectNativeView(unittest.TestCase):
    def test_native_zero_extra_storage_method(self):
        header=(ROOT/'src/SupervisorStartEffectView.hpp').read_text()
        impl=(ROOT/'src/SupervisorEffectLeaf.cpp').read_text()
        self.assertIn('struct SupervisorStartEffectView : ScoreSupervisorView',header)
        self.assertIn('sizeof(SupervisorStartEffectView) == sizeof(Supervisor)',header)
        self.assertIn('offsetof(SupervisorStartEffectView, flags5D4) == 0x5D4',header)
        self.assertIn('void StartEffect(int index);',header)
        self.assertIn('#include "SupervisorStartEffectView.hpp"',impl)
        self.assertNotIn('struct SupervisorStartEffectView',impl)
        self.assertIn('SupervisorStartEffectView::StartEffect(int index)',impl)

    def test_true_leaf_identity(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        leaf=units['supervisor-start-effect-leaf']
        self.assertEqual((leaf['target_address'],leaf['size']),(0x00430500,47))
        self.assertIn('SupervisorStartEffectView',leaf['symbol'])

if __name__=='__main__':unittest.main()

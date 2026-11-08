"""Original exact 29-byte state reset is a genuine PlayerOwnerState method.

These structural checks supplement the strict 116-function PE/COFF source-
cold replay; they do not substitute for complete physical-byte comparison.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
CALLEE='?PrepareReset@PlayerOwnerStateView@@QAEXXZ'

class NativePlayerOwnerPrepareReset(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_one_native_owner_without_another_state_class(self):
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        reset=(ROOT/'src/PlayerOwnerStateReset.cpp').read_text()
        added=(ROOT/'src/PlayerAddedState.cpp').read_text()
        self.assertIn('void PrepareReset();',native)
        self.assertIn('PlayerOwnerStateView ownerState30410;',native)
        self.assertIn('PlayerOwnerStateNativeFullMembers',native)
        self.assertIn('void PlayerOwnerStateView::PrepareReset()',reset)
        self.assertIn('PlayerOwnerPrepareResetNativeLayout',reset)
        self.assertIn('this->timer20 = 0;',reset)
        self.assertIn('this->state04 = 0;',reset)
        self.assertIn('this->state38 = 0;',reset)
        self.assertNotIn('PlayerOwnerStateResetView',reset)
        self.assertIn('void PlayerOwnerStateView::Reset()', added)
        self.assertIn('    PrepareReset();',added)
        self.assertIn('playerOwner00->sideState->AddScore(maxValue10);',added)
        self.assertIn('    maxValue10 = 0;',added)
        self.assertIn('    value0C = 0;',added)
        self.assertIn('this->ownerState30410.Reset();',added)
        self.assertNotIn('PlayerAddedOwnerStateView',added)
        self.assertNotIn('PlayerAddedHandleManagerView',added)
        self.assertNotIn('PlayerAddedOwnerPrepareReset()',added)
        self.assertNotIn('extern void PlayerAddedOwnerPrepareReset();',added)
        self.assertIn('this->ownerState30410.Reset();',added)
        self.assertIn('int maxState08;',native)

    def test_existing_exact_29_byte_leaf_and_41_byte_caller_original_va(self):
        provider=self.units['player-owner-state-prepare-reset']
        caller=self.units['player-added-owner-reset']
        self.assertEqual((provider['target_address'],provider['size'],len(provider['relocations'])),
                         (0x0040F8C0,29,1))
        self.assertEqual(provider['symbol'],CALLEE)
        self.assertEqual((caller['target_address'],caller['size'],len(caller['relocations'])),
                         (0x0041D7E0,41,3))
        calls=[r for r in caller['relocations'] if r['symbol']==CALLEE]
        self.assertEqual([(r['offset'],r['type'],r['target'],r.get('addend',0)) for r in calls],
                         [(0x4,'REL32',0x0040F8C0,0)])
        self.assertEqual(self.units['player-initialize-added-state']['size'],552)
        self.assertEqual(self.units['player-initialize-added-state']['compare_size'],572)
        self.assertEqual(self.units['player-owner-apply-reward']['compare_size'],1540)

    def test_tracking_preserves_original_exact_provider(self):
        for name in ('functions.csv','implemented.csv','matches.csv','reccmp-functions.csv'):
            with self.subTest(name=name):
                data=(ROOT/'config'/name).read_text()
                self.assertIn('PlayerOwnerStateView::PrepareReset',data)
                self.assertNotIn('PlayerOwnerStateResetView::PrepareReset',data)

if __name__=='__main__':unittest.main()

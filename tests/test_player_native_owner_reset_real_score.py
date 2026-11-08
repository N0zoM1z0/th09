"""Player owner reset at 0x41D7E0 calls real AddScore, not ReleaseHandle.

Strict complete PE/COFF comparison establishes exact 41/572 byte owners.
This guards the actual physical receiver and immutable original call VAs.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
PREPARE='?PrepareReset@PlayerOwnerStateView@@QAEXXZ'
RESET='?Reset@PlayerOwnerStateView@@QAEXXZ'
SCORE='?AddScore@AsciiGameManagerSideView@@QAEXH@Z'

class PlayerNativeOwnerResetRealScore(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_actual_owner_score_reset_without_fake_handle(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source=(ROOT/'src/PlayerAddedState.cpp').read_text()
        self.assertIn('void Reset();',header)
        self.assertIn('void PlayerOwnerStateView::Reset()',source)
        self.assertIn('PlayerAddedRealNativeOwnerAt30410',source)
        self.assertIn('PlayerOwnerStateView ownerState30410;',source)
        self.assertIn('this->ownerState30410.Reset();',source)
        self.assertIn('PrepareReset();',source)
        self.assertIn('playerOwner00->sideState->AddScore(maxValue10);',source)
        self.assertIn('maxValue10 = 0;',source)
        self.assertIn('value0C = 0;',source)
        self.assertIn('timer14 = 0;',source)
        self.assertNotIn('PlayerAddedOwnerStateView',source)
        self.assertNotIn('PlayerAddedHandleManagerView',source)
        self.assertNotIn('PlayerAddedOwnerContextView',source)
        self.assertNotIn('ReleaseHandle(',source)
        self.assertIn('int maxValue10;',header)
        self.assertIn('PlayerLifecycleView *playerOwner00;',header)
        self.assertIn('void AddScore(int score);',(ROOT/'src/GameManagerSideState.hpp').read_text())

    def test_complete_41_byte_reset_three_original_call_fields(self):
        u=self.units['player-added-owner-reset']
        self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),(0x0041D7E0,41,3))
        self.assertEqual(u['symbol'],RESET)
        self.assertEqual([(r['offset'],r['symbol'],r['target'],r['type']) for r in u['relocations'][:2]],
                         [(0x4,PREPARE,0x0040F8C0,'REL32'),
                          (0x12,SCORE,0x0040F860,'REL32')])
        self.assertEqual(self.units['player-owner-state-prepare-reset']['size'],29)
        self.assertEqual(self.units['game-manager-side-add-score']['size'],33)
        self.assertEqual(self.units['player-owner-apply-reward']['compare_size'],1540)

    def test_complete_572_byte_initializer_uses_real_reset_at_original_va(self):
        unit=self.units['player-initialize-added-state']
        self.assertEqual((unit['target_address'],unit['size'],unit['compare_size'],len(unit['relocations'])),
                         (0x0041EBC0,552,572,25))
        actual=[r for r in unit['relocations'] if r['symbol']==RESET]
        self.assertEqual([(r['offset'],r['target'],r['type']) for r in actual],
                         [(0x16A,0x0041D7E0,'REL32')])
        self.assertEqual(len([r for r in unit['relocations'] if r['symbol'].startswith('$L')]),6)
        self.assertNotIn('PlayerAddedOwnerStateView::Reset',(ROOT/'config/functions.csv').read_text())
        self.assertIn('PlayerOwnerStateView::Reset',(ROOT/'config/functions.csv').read_text())

if __name__=='__main__':unittest.main()

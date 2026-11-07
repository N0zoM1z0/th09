"""Selector uses true native Player state methods, not its shadow methods."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSelectorNativeUpdateStateMethods(unittest.TestCase):
    def test_native_lifecycle_getter_and_four_byte_state_setter(self):
        src=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        state=(ROOT/'src/PlayerState4OpsView.hpp').read_text()
        self.assertNotIn('#include "PlayerState4OpsView.hpp"',src)
        self.assertNotIn('void EnterGameplayMode(int mode);',src)
        self.assertNotIn('int GetUpdateState();',src)
        self.assertEqual(src.count('->GetUpdateState()'),2)
        self.assertIn('phasePlayer->SetUpdateState(3);',src)
        self.assertIn('int GetUpdateState();',native)
        self.assertIn('void SetUpdateState(int state);',native)
        self.assertNotIn('void SetUpdateState(int state);',state)
        self.assertIn('offsetof(PlayerState4OpsView, updateState00) == 0',state)
        # The separately authored selector resolver keeps its original method identity.
        self.assertIn('float *PlayerGameplayMethods::ResolvePatternOffset(',src)

    def test_all_three_original_bound_native_callees(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        boss=u['player-update-selector-state']
        self.assertEqual((boss['size'],len(boss['relocations'])),(3238,136))
        rows={r['offset']:r for r in boss['relocations']}
        for offset,leafname,size in ((0x11B,'player-get-update-state-alias',3),
                                     (0x129,'player-set-update-state',12),
                                     (0xC10,'player-get-update-state-alias',3)):
            with self.subTest(offset=offset):
                row=rows[offset];leaf=u[leafname]
                self.assertEqual(leaf['size'],size)
                self.assertEqual((row['symbol'],row['type'],row['target'],row.get('addend',0)),
                                 (leaf['symbol'],'REL32',leaf['target_address'],0))
        resolver=u['player-resolve-pattern-offset']
        self.assertEqual((resolver['size'],resolver.get('compare_size'),len(resolver['relocations'])),(346,412,18))

if __name__=='__main__':unittest.main()

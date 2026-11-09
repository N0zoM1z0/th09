"""Exact 1230-byte GameManager update consumes a genuine shared runtime leaf."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class GameManagerNativePlayerSharedBlockGate(unittest.TestCase):
    def test_native_receiver_replaces_fake_subsystem_method(self):
        s=(ROOT/'src/GameManagerSetup.cpp').read_text()
        layout=(ROOT/'src/GameManagerSetupView.hpp').read_text()
        real=(ROOT/'src/PlayerSharedRuntimeView.hpp').read_text()
        self.assertIn('#include "PlayerSharedRuntimeView.hpp"',s)
        self.assertNotIn('struct GameSubsystemSetupView',s)
        self.assertNotIn('->IsActive()',s)
        self.assertIn('!globalManager.gameSubsystem->IsBlocked()',s)
        self.assertIn('PlayerSharedRuntimeView *playerSharedRuntimeA8;',layout)
        self.assertIn('offsetof(GameManagerSetupLayout, playerSharedRuntimeA8) == 0xA8',layout)
        self.assertNotIn('int IsBlocked();',real)
        self.assertIn('int IsBlocked();',
                      (ROOT/'src/FrontFactoryConstructView.hpp').read_text())
        self.assertIn('int blockStateE94C;',real)

    def test_original_bound_call_and_exact_callee(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        caller=units['game-manager-on-update']
        callee=units['player-shared-is-blocked']
        self.assertEqual((caller['size'],len(caller['relocations'])),(1230,87))
        self.assertEqual((callee['target_address'],callee['size']),(0x00404980,24))
        field=next(x for x in caller['relocations'] if x['offset']==0x1F6)
        self.assertEqual((field['symbol'],field['target'],field['type']),
                         (callee['symbol'],callee['target_address'],'REL32'))

if __name__=='__main__':unittest.main()

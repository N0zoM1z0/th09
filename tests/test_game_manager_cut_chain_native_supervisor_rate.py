"""Exact 98-byte GameManager cut-chain publishes genuine Supervisor frame rate.

Compiler-original PE/COFF replay independently establishes full byte identity;
this tests that source no longer invents a second 0x5B8-byte Supervisor.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class GameManagerCutChainNativeSupervisorRate(unittest.TestCase):
    def test_native_supervisor_field_not_private_prefix(self):
        src=(ROOT/'src/GameManagerSetup.cpp').read_text()
        sup=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertNotIn('GameManagerCutChainSupervisorView',src)
        self.assertNotIn('reinterpret_cast<GameManagerCutChainSupervisorView *>(',src)
        self.assertIn('GameManagerCutChainNativeSupervisorRateAt5B8',src)
        self.assertIn('offsetof(Supervisor, framerateMultiplier5B8) == 0x5B8',src)
        self.assertIn('sizeof(Supervisor) == 0x7B0',src)
        self.assertIn('g_Supervisor.framerateMultiplier5B8 = 1.0f;',src)
        self.assertIn('union { float framerateMultiplier5B8; float gameSpeed; };',sup)
        self.assertIn('SupervisorSizeIs7B0',sup)
        self.assertIn('g_Chain.Cut(&g_GameManagerCalcChain);',src)
        self.assertIn('g_Chain.Cut(&g_GameManagerDrawChain);',src)

    def test_exact_leaf_and_nonexact_setup_remain_separate(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        leaf=units['game-manager-cut-chain']
        self.assertEqual((leaf['target_address'],leaf['size'],len(leaf['relocations'])),
                         (0x41A827,98,10))
        self.assertEqual(leaf['source'],'src/GameManagerSetup.cpp')
        self.assertNotIn('gameplay-setup-thread',units)
        self.assertEqual(units['game-manager-on-update']['size'],1230)

if __name__=='__main__':unittest.main()

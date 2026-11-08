"""GameManager_CutChain writes the genuine Supervisor frame multiplier.

Strict PE-bound source-cold checks, not this source-level test, prove complete
98-byte original machine code and its ten original relocation fields.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class GameManagerCutChainNativeSupervisor(unittest.TestCase):
    def test_no_duplicate_cut_chain_supervisor_owner(self):
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        supervisor=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('void __fastcall GameManager_CutChain()',source)
        self.assertIn('g_Supervisor.framerateMultiplier5B8 = 1.0f;',source)
        self.assertIn('GameManagerCutChainNativeSupervisorRateAt5B8',source)
        self.assertIn('offsetof(Supervisor, framerateMultiplier5B8) == 0x5B8',source)
        self.assertIn('sizeof(Supervisor) == 0x7B0',source)
        self.assertNotIn('GameManagerCutChainSupervisorView',source)
        self.assertIn('float framerateMultiplier5B8;',supervisor)
        self.assertIn('SupervisorSizeIs7B0',supervisor)
        self.assertIn('g_Chain.Cut(&g_GameManagerCalcChain);',source)
        self.assertIn('g_Chain.Cut(&g_GameManagerDrawChain);',source)
        self.assertIn('g_GameManager.SelectOpponentConfiguration();',source)
        self.assertEqual(source.count('g_GameManager.SelectOpponentConfiguration();'),4)

    def test_same_original_supervisor_field_operand_and_exact_cut_chain_owner(self):
        with (ROOT/'config/match-units.toml').open('rb') as fd:
            units=tomllib.load(fd)['units']
        u=units['game-manager-cut-chain']
        self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),
                         (0x0041A827,98,10))
        fields=[r for r in u['relocations'] if r['symbol']=='?g_Supervisor@@3VSupervisor@@A']
        self.assertEqual([(r['offset'],r['type'],r['target'],r.get('addend',0)) for r in fields],
                         [(0x3F,'DIR32',0x004B3100,0x5B8)])
        self.assertEqual(len([r for r in u['relocations'] if r['symbol']=='?Cut@Chain@@QAEXPAVChainElem@@@Z']),2)
        self.assertEqual(units['game-manager-on-update']['size'],1230)
        self.assertEqual(units['game-manager-cleanup-gameplay-state']['size'],425)
        self.assertNotIn('gameplay-setup-thread',units)

if __name__=='__main__':unittest.main()

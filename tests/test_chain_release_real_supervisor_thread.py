"""Chain::Release calls the genuine thread shutdown method on native Supervisor."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class ChainReleaseNativeSupervisorThread(unittest.TestCase):
    def test_chain_release_calls_true_thread_close(self):
        s=(ROOT/'src/ChainRelease.cpp').read_text()
        self.assertIn('#include "SupervisorThreadStartView.hpp"',s)
        self.assertNotIn('struct SupervisorThreadStartView',s)
        self.assertNotIn('extern SupervisorThreadStartView g_Supervisor;',s)
        self.assertIn('static_cast<SupervisorThreadStartView *>(&g_Supervisor)->ThreadClose();',s)

    def test_original_field_identity_and_callee(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        caller=units['chain-release'];callee=units['supervisor-thread-join-close']
        fields={r['offset']:r for r in caller['relocations']}
        self.assertEqual((caller['size'],len(fields)),(34,4))
        self.assertEqual((fields[0x4]['symbol'],fields[0x4]['target']),
                         ('?g_Supervisor@@3VSupervisor@@A',0x004B3100))
        self.assertEqual((fields[0x9]['symbol'],fields[0x9]['target']),
                         (callee['symbol'],callee['target_address']))
        self.assertEqual((callee['target_address'],callee['size']),(0x00430230,108))

if __name__=='__main__': unittest.main()

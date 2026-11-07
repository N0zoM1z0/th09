"""Supervisor AddedCallback calls the exact native GameManager reset leaf."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SupervisorAddedNativeGameManagerReset(unittest.TestCase):
    def test_real_reset_receiver_and_source_spelling(self):
        source=(ROOT/'src/SupervisorAddedCallback.cpp').read_text()
        shared=(ROOT/'src/GameManagerResetView.hpp').read_text()
        self.assertIn('#include "GameManagerResetView.hpp"',source)
        self.assertNotIn('struct SupervisorAddedGameManagerView',source)
        self.assertNotIn('extern SupervisorAddedGameManagerView g_GameManager',source)
        self.assertIn('static_cast<GameManagerResetView *>(&g_GameManager)->ResetForSupervisorAdded();',source)
        self.assertIn('struct GameManagerResetView : AsciiGameManagerView',shared)
        self.assertIn('sizeof(GameManagerResetView) == 0x390',shared)

    def test_original_referents_and_exact_leaf(self):
        with (ROOT/'config/match-units.toml').open('rb') as inp:units=tomllib.load(inp)['units']
        owner=units['supervisor-added-callback']
        real=units['game-manager-reset-for-supervisor-added']
        self.assertEqual((owner['size'],len(owner['relocations'])),(238,27))
        self.assertEqual((real['size'],real['target_address']),(47,0x0041BA4F))
        fields={r['offset']:r for r in owner['relocations']}
        self.assertEqual((fields[0x0A]['symbol'],fields[0x0A]['target']),
                         ('?g_GameManager@@3UAsciiGameManagerView@@A',0x004A7D90))
        self.assertEqual((fields[0x19]['symbol'],fields[0x19]['type'],fields[0x19]['target']),
                         (real['symbol'],'REL32',real['target_address']))

if __name__=='__main__':unittest.main()

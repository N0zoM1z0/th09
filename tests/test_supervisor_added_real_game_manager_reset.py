"""The 238-byte Supervisor AddedCallback calls the exact native GameManager reset.

The global and callee are both bound to real TH09 original addresses.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SupervisorAddedRealGameManagerReset(unittest.TestCase):
    def test_one_game_manager_native_allocation_and_method(self):
        source=(ROOT/'src/SupervisorAddedCallback.cpp').read_text()
        view=(ROOT/'src/GameManagerResetView.hpp').read_text()
        self.assertIn('#include "GameManagerResetView.hpp"',source)
        self.assertNotIn('struct SupervisorAddedGameManagerView',source)
        self.assertNotIn('extern SupervisorAddedGameManagerView g_GameManager;',source)
        self.assertIn('static_cast<GameManagerResetView *>(&g_GameManager)->ResetForSupervisorAdded();',source)
        self.assertIn('struct GameManagerResetView : AsciiGameManagerView',view)
        self.assertIn('sizeof(GameManagerResetView) == sizeof(AsciiGameManagerView)',view)

    def test_original_byte_bound_caller_global_and_callee(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        caller=units['supervisor-added-callback']
        reset=units['game-manager-reset-for-supervisor-added']
        by_offset={r['offset']:r for r in caller['relocations']}
        self.assertEqual((caller['size'],len(by_offset)),(238,27))
        self.assertEqual((reset['size'],reset['target_address']),(47,0x0041BA4F))
        self.assertEqual((by_offset[0x0A]['type'],by_offset[0x0A]['target'],by_offset[0x0A]['symbol']),
                         ('DIR32',0x004A7D90,'?g_GameManager@@3UAsciiGameManagerView@@A'))
        self.assertEqual((by_offset[0x19]['type'],by_offset[0x19]['target'],by_offset[0x19]['symbol']),
                         ('REL32',reset['target_address'],reset['symbol']))

if __name__=='__main__':unittest.main()

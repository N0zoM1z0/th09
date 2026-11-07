"""The exact CardAttack draw caller uses native Supervisor viewport identity.

The source assertions complement full original-bound COFF owner comparisons.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]

class CardAttackNativeViewport(unittest.TestCase):
    def test_no_duplicate_fake_supervisor_receiver(self):
        source = (ROOT / 'src/CardAttack.cpp').read_text()
        self.assertIn('#include "Supervisor.hpp"', source)
        self.assertNotIn('struct CardAttackSupervisorView', source)
        self.assertNotIn('extern CardAttackSupervisorView g_Supervisor', source)
        self.assertIn('g_Supervisor.ConfigureScreenViewport(cardAttack->sideIndex00);', source)
        self.assertNotIn('PreparePlayerDraw', source)

    def test_native_referents_are_original_target_bound(self):
        with (ROOT / 'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        callee = units['supervisor-configure-screen-viewport']
        caller = units['card-attack-on-draw']
        fields = {r['offset']:r for r in caller['relocations']}
        self.assertEqual((caller['size'],len(fields)),(334,17))
        self.assertEqual((fields[8]['symbol'],fields[8]['target']),
                         ('?g_Supervisor@@3VSupervisor@@A',0x004B3100))
        self.assertEqual((fields[13]['symbol'],fields[13]['target']),
                         (callee['symbol'],callee['target_address']))
        self.assertEqual(callee['target_address'],0x00403D90)

if __name__ == '__main__':
    unittest.main()

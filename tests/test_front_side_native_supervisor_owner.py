"""FrontSide OnUpdate calls the independently exact native Supervisor method."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class FrontSideNativeSupervisorOwner(unittest.TestCase):
    def test_update_calls_the_real_supervisor_not_a_fake_type(self):
        source = (ROOT/'src/FrontSide.cpp').read_text()
        self.assertIn('#include "Supervisor.hpp"', source)
        self.assertNotIn('struct FrontSideSupervisorView', source)
        self.assertNotIn('extern FrontSideSupervisorView g_Supervisor', source)
        self.assertIn('g_Supervisor.SelectSide(frontSide->sideIndexA66C)', source)

    def test_original_referents_are_from_independent_exact_method(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u = tomllib.load(f)['units']
        self.assertEqual(u['supervisor-select-side']['target_address'], 0x00401440)
        self.assertEqual(u['supervisor-select-side']['symbol'], '?SelectSide@Supervisor@@QAEXH@Z')
        update = u['front-side-on-update']
        self.assertEqual(update['size'],2381)
        refs = {r['offset']:r for r in update['relocations']}
        self.assertEqual(refs[0x11]['symbol'],'?g_Supervisor@@3VSupervisor@@A')
        self.assertEqual(refs[0x11]['target'],0x004B3100)
        self.assertEqual(refs[0x16]['symbol'],u['supervisor-select-side']['symbol'])
        self.assertEqual(refs[0x16]['target'],u['supervisor-select-side']['target_address'])


if __name__ == '__main__':
    unittest.main()

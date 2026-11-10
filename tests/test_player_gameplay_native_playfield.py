"""Target-bound GameManager bounds shared by Player selector and movement."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerGameplayNativePlayfield(unittest.TestCase):
    def test_3238_byte_exact_selector_uses_native_game_manager_bounds(self):
        gameplay=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        movement=(ROOT/'src/PlayerMovement.cpp').read_text()
        header=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        self.assertNotIn('#include "GameManagerResetView.hpp"',gameplay)
        self.assertIn('#include "GameManagerRuntimeView.hpp"',gameplay)
        self.assertIn('PlayerSelectorNativePlayfieldBounds',gameplay)
        for legacy,member,offset in (
            ('g_PlayerPlayfieldMinX','coordinate360','0x360'),
            ('g_PlayerPlayfieldMinY','coordinate364','0x364'),
            ('g_PlayerPlayfieldWidth','coordinate368','0x368'),
            ('g_PlayerPlayfieldHeight','coordinate36C','0x36C')):
            with self.subTest(field=member):
                self.assertNotIn(legacy,gameplay)
                self.assertIn('g_GameManager.'+member,gameplay)
                self.assertNotIn('reinterpret_cast<GameManagerResetView *>(&g_GameManager)',gameplay)
                self.assertIn('g_GameManager.'+member,movement)
                self.assertIn('float '+member+';',header)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        selector=units['player-update-selector-state']
        resolver=units['player-resolve-pattern-offset']
        self.assertEqual((selector['target_address'],selector['size']), (0x004049A0,3238))
        self.assertEqual(len(selector['relocations']),136)
        self.assertEqual((resolver['target_address'],resolver['size'],resolver['compare_size']),
                         (0x00404710,346,412))
        self.assertEqual(len(resolver['relocations']),18)
        # Keep base-owner references separate from the native Front pointer
        # operand, whose GameManager addend is +0xA8.
        bound=[r for r in selector['relocations']
               if r['symbol']=='?g_GameManager@@3UAsciiGameManagerView@@A'
               and r.get('addend', 0) == 0]
        self.assertEqual(len(bound),38)
        self.assertTrue(all(r['target']==0x004A7D90 for r in bound))

if __name__=='__main__':unittest.main()

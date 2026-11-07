"""Exact Player selector's three FrontSide calls use their true PE callees."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSelectorNativeFrontSide(unittest.TestCase):
    def test_source_replaces_wrong_timing_method_name(self):
        src=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        header=(ROOT/'src/FrontSide.hpp').read_text()
        self.assertIn('#include "FrontSide.hpp"',src)
        self.assertNotIn('struct PlayerGameplayFrontSideView',src)
        self.assertNotIn('reinterpret_cast<PlayerGameplayFrontSideView *>',src)
        self.assertEqual(src.count('->SetTimingDigits('),2)
        self.assertEqual(src.count('->ResetPatternTiming();'),1)
        self.assertIn('int SetTimingDigits(int frames);',header)
        self.assertIn('int SetPatternTiming(int frames);',header)

    def test_three_original_calls_select_actual_exact_origins(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        caller=units['player-update-selector-state']
        self.assertEqual((caller['target_address'],caller['size'],len(caller['relocations'])),
                         (0x004049A0,3238,136))
        self.assertEqual(units['front-side-set-timing-digits']['target_address'],0x00418950)
        self.assertEqual(units['front-side-set-pattern-timing']['target_address'],0x0041A3E0)
        for offset,name in ((0xAB,'front-side-set-timing-digits'),
                            (0x186,'front-side-reset-pattern-timing'),
                            (0x196,'front-side-set-timing-digits')):
            with self.subTest(offset=offset):
                row=next(r for r in caller['relocations'] if r['offset']==offset)
                leaf=units[name]
                self.assertEqual((row['symbol'],row['target'],row['type'],row.get('addend',0)),
                                 (leaf['symbol'],leaf['target_address'],'REL32',0))
        resolver=units['player-resolve-pattern-offset']
        self.assertEqual((resolver['size'],resolver.get('compare_size')), (346,412))
        self.assertEqual(len(resolver['relocations']),18)

if __name__=='__main__':unittest.main()

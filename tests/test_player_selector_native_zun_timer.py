"""The exact 3238-byte selector reads real ZunTimer conversion operators."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSelectorNativeZunTimer(unittest.TestCase):
    def test_native_timer_fields_not_a_private_method_view(self):
        source=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        header=(ROOT/'src/ZunTimer.hpp').read_text()
        self.assertNotIn('struct PlayerGameplayTimerCurrentView',source)
        self.assertNotIn('GetCurrent()',source)
        self.assertEqual(source.count('static_cast<int>(header->openingTimer5C)'),2)
        self.assertEqual(source.count('static_cast<int>(header->secondaryTimer68)'),1)
        self.assertIn('operator int();',header)
        self.assertIn('sizeof(ZunTimer) == 0x0C',header)

    def test_three_exact_original_callees_and_independent_resolver(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        selector=units['player-update-selector-state']
        leaf=units['zun-timer-current-value-alias']
        self.assertEqual((selector['size'],len(selector['relocations'])),(3238,136))
        self.assertEqual((leaf['target_address'],leaf['size']),(0x00435F00,4))
        fields={r['offset']:r for r in selector['relocations']}
        for offset in (0x97,0x141,0x1CE):
            with self.subTest(offset=offset):
                r=fields[offset]
                self.assertEqual((r['type'],r['symbol'],r['target'],r.get('addend',0)),
                                 ('REL32',leaf['symbol'],leaf['target_address'],0))
        resolver=units['player-resolve-pattern-offset']
        self.assertEqual((resolver['size'],resolver.get('compare_size'),len(resolver['relocations'])),(346,412,18))

if __name__=='__main__':unittest.main()

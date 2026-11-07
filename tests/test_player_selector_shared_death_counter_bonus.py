"""Original Player selector/death methods publish/read the same 0x4A7E4C word."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSelectorSharedDeathCounterBonus(unittest.TestCase):
    def test_selector_publishes_native_death_bonus_without_private_alias(self):
        selector=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        death=(ROOT/'src/PlayerDeath.cpp').read_text()
        self.assertIn('extern int g_PlayerDeathCounterBonus;',selector)
        self.assertIn('g_PlayerDeathCounterBonus = config->protocolValue18;',selector)
        self.assertNotIn('g_PlayerPatternProtocolValue',selector)
        self.assertIn('extern int g_PlayerDeathCounterBonus;',death)
        self.assertIn('counterStep += g_PlayerDeathCounterBonus;',death)

    def test_two_independent_original_bound_same_global_cells(self):
        with (ROOT/'config/match-units.toml').open('rb') as source:
            units=tomllib.load(source)['units']
        for name,offset,size in (('player-update-selector-state',0x1F4,3238),
                                 ('player-enter-death-state',0x46,753)):
            with self.subTest(name=name):
                unit=units[name]
                row=next(x for x in unit['relocations'] if x['offset']==offset)
                self.assertEqual(unit['size'],size)
                self.assertEqual((row['type'],row['symbol'],row['target'],row.get('addend',0)),
                                 ('DIR32','?g_PlayerDeathCounterBonus@@3HA',0x004A7E4C,0))
        self.assertEqual((units['player-resolve-pattern-offset']['size'],
                          units['player-resolve-pattern-offset'].get('compare_size')),(346,412))

if __name__=='__main__':unittest.main()

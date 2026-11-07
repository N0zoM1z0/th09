"""Exact selector writes the same bonus word the exact PlayerDeath owner reads."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSelectorNativeDeathBonus(unittest.TestCase):
    def test_native_original_global_is_shared_by_selector_and_death(self):
        selector=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        death=(ROOT/'src/PlayerDeath.cpp').read_text()
        self.assertNotIn('g_PlayerPatternProtocolValue',selector)
        self.assertIn('extern int g_PlayerDeathCounterBonus;',selector)
        self.assertIn('extern int g_PlayerDeathCounterBonus;',death)
        self.assertIn('g_PlayerDeathCounterBonus = config->protocolValue18;',selector)
        self.assertIn('counterStep += g_PlayerDeathCounterBonus;',death)

    def test_original_bound_both_owners_target_same_physical_address(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        boss=u['player-update-selector-state']
        death=u['player-enter-death-state']
        selector_field=next(x for x in boss['relocations'] if x['offset']==0x1F4)
        death_fields=[x for x in death['relocations'] if x['target']==0x004A7E4C]
        self.assertEqual(len(death_fields),1)
        for row in (selector_field,death_fields[0]):
            self.assertEqual((row['symbol'],row['type'],row['target'],row.get('addend',0)),
                             ('?g_PlayerDeathCounterBonus@@3HA','DIR32',0x004A7E4C,0))
        self.assertEqual((boss['size'],len(boss['relocations'])),(3238,136))
        resolver=u['player-resolve-pattern-offset']
        self.assertEqual((resolver['size'],resolver.get('compare_size')),(346,412))

if __name__=='__main__':unittest.main()

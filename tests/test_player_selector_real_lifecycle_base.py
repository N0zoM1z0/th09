"""Exact Player selector and its 346-byte resolver share native Player storage."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSelectorRealLifecycleBase(unittest.TestCase):
    def test_source_facing_resolver_is_zero_extra_storage_derived_type(self):
        source=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        real=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('struct PlayerGameplayMethods : PlayerLifecycleView',source)
        self.assertIn('sizeof(PlayerGameplayMethods) == sizeof(PlayerLifecycleView)',source)
        self.assertIn('offsetof(PlayerGameplayMethods, primaryShtFile) == 0x30338',source)
        self.assertIn('offsetof(PlayerGameplayMethods, position1B88) == 0x1B88',source)
        self.assertIn('static_cast<PlayerLifecycleView *>(this)',source)
        self.assertNotIn('reinterpret_cast<PlayerLifecycleView *>(this)',source)
        self.assertEqual(source.count('reinterpret_cast<PlayerGameplayMethods *>(candidatePlayer)->ResolvePatternOffset('),5)
        self.assertIn('PlayerGameplayMethods::ResolvePatternOffset(',source)
        self.assertIn('PlayerPositionView position1B88;',real)

    def test_independent_original_bound_resolver_and_large_selector(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        child=units['player-resolve-pattern-offset']
        owner=units['player-update-selector-state']
        self.assertEqual((child['target_address'],child['size'],child.get('compare_size'),len(child['relocations'])),
                         (0x00404710,346,412,18))
        self.assertEqual((owner['target_address'],owner['size'],len(owner['relocations'])),
                         (0x004049A0,3238,136))
        sites=[r for r in owner['relocations'] if r['symbol']==child['symbol']]
        self.assertEqual(len(sites),5)
        self.assertTrue(all(r['type']=='REL32' and r['target']==child['target_address']
                            for r in sites))

if __name__=='__main__':unittest.main()

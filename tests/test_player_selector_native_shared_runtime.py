"""The exact Player selector shares the native 0x10964 runtime and blocked leaf."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSelectorNativeSharedRuntime(unittest.TestCase):
    def test_one_shared_native_runtime_allocation(self):
        src=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        real=(ROOT/'src/PlayerSharedRuntimeView.hpp').read_text()
        self.assertIn('#include "FrontFactoryConstructView.hpp"',src)
        self.assertNotIn('struct PlayerSharedRuntimeView\n{',src)
        self.assertNotIn('extern PlayerSharedRuntimeView *g_PlayerSharedRuntime;',src)
        self.assertIn('g_GameManager.frontFactoryA8->transitionVmCount1095C',src)
        self.assertIn('g_GameManager.frontFactoryA8->IsBlocked()',src)
        self.assertIn('sizeof(PlayerSharedRuntimeView) == 0x10964',real)
        self.assertIn('offsetof(PlayerSharedRuntimeView, blockStateE94C) == 0xE94C',real)
        self.assertIn('offsetof(PlayerSharedRuntimeView, updateBlock1095C) == 0x1095C',real)
        self.assertIn('offsetof(PlayerSharedRuntimeView, drawCounter10960) == 0x10960',real)

    def test_both_original_bound_referents_are_native(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        big=units['player-update-selector-state'];leaf=units['player-shared-is-blocked']
        self.assertEqual((big['size'],len(big['relocations'])),(3238,136))
        self.assertEqual((leaf['size'],leaf['target_address']),(24,0x00404980))
        r={x['offset']:x for x in big['relocations']}
        self.assertEqual((r[0x42]['symbol'],r[0x42]['target'],r[0x42]['type'],r[0x42].get('addend',0)),
                         ('?g_GameManager@@3UAsciiGameManagerView@@A',0x004A7D90,'DIR32',0xA8))
        self.assertEqual((r[0x67]['symbol'],r[0x67]['target'],r[0x67]['type']),
                         (leaf['symbol'],leaf['target_address'],'REL32'))
        resolver=units['player-resolve-pattern-offset']
        self.assertEqual((resolver['size'],resolver.get('compare_size')),(346,412))

if __name__=='__main__':unittest.main()

"""The exact 3238-byte Player selector uses the existing native replay RNG."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSelectorNativeRng(unittest.TestCase):
    def test_source_uses_true_eight_byte_runtime_and_shared_root(self):
        source=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        native=(ROOT/'src/RngRuntimeLeaves.hpp').read_text()
        self.assertIn('#include "RngRuntimeLeaves.hpp"',source)
        self.assertNotIn('struct PlayerGameplayRngView',source)
        self.assertNotIn('g_PlayerGameplayRng',source)
        self.assertIn('extern RngRuntimeView g_ReplayRng;',source)
        for method in ('GetRandomU32InRange(4)',
                       'GetRandomU16InRange(2)',
                       'GetRandomF32InRange(448.0f)'):
            self.assertIn('g_ReplayRng.'+method,source)
        self.assertIn('sizeof(RngRuntimeView) == 8',native)

    def test_six_original_bound_rng_fields_and_346byte_resolver(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        big=u['player-update-selector-state']
        self.assertEqual((big['size'],len(big['relocations'])),(3238,136))
        offsets={r['offset']:r for r in big['relocations']}
        for off in (0x5E9,0x6A1,0x8D9):
            with self.subTest(global_offset=off):
                row=offsets[off]
                self.assertEqual((row['symbol'],row['target'],row['type'],row.get('addend',0)),
                                 ('?g_ReplayRng@@3URngRuntimeView@@A',0x004ACE0C,'DIR32',0))
        for off,leaf in ((0x5F5,'rng-runtime-u32-range'),
                         (0x6A6,'rng-runtime-u16-range'),
                         (0x8E1,'rng-runtime-f32-range')):
            with self.subTest(offset=off):
                row=offsets[off];provider=u[leaf]
                self.assertEqual((row['symbol'],row['target'],row['type']),
                                 (provider['symbol'],provider['target_address'],'REL32'))
        resolver=u['player-resolve-pattern-offset']
        self.assertEqual((resolver['size'],resolver.get('compare_size'),len(resolver['relocations'])),(346,412,18))

if __name__=='__main__':unittest.main()

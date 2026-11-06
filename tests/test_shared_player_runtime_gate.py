"""Guard native signed IsBlocked state without conflating anonymous C++ types."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeSharedPlayerRuntime(unittest.TestCase):
    def test_global_gate_is_native_int_in_one_header(self):
        header = (ROOT/'src/PlayerSharedRuntimeView.hpp').read_text()
        leaf = (ROOT/'src/PlayerSharedRuntimeLeaves.cpp').read_text()
        for field,off in [('blockStateE94C','0xE94C'),
                          ('updateBlock1095C','0x1095C'),
                          ('drawCounter10960','0x10960')]:
            self.assertIn('offsetof(PlayerSharedRuntimeView, '+field+') == '+off, header)
        self.assertIn('sizeof(PlayerSharedRuntimeView) == 0x10964', header)
        self.assertIn('int value = blockStateE94C;', leaf)
        self.assertNotIn('reinterpret_cast<unsigned char *>(this) + 0xE94C', leaf)
        for name in ('PlayerRuntime.cpp','ReplayPlaybackFrameAdvance.cpp'):
            source = (ROOT/'src'/name).read_text()
            self.assertIn('#include "PlayerSharedRuntimeView.hpp"', source)
            self.assertNotIn('struct PlayerSharedRuntimeView\n{', source)

    def test_original_anonymous_selector_type_is_not_silently_unified(self):
        selector = (ROOT/'src/PlayerGameplayState.cpp').read_text()
        self.assertIn('struct PlayerSharedRuntimeView\n{', selector)
        self.assertNotIn('#include "PlayerSharedRuntimeView.hpp"', selector)
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        self.assertEqual(units['player-shared-is-blocked']['size'],24)
        self.assertEqual(units['replay-control-frame-advance']['size'],59)
        self.assertEqual(units['player-on-update']['size'],522)
        refs = [x['symbol'] for x in units['player-update-selector-state']['relocations']]
        self.assertTrue(any('PlayerSharedRuntimeView@?A0x' in name for name in refs))


if __name__ == '__main__':
    unittest.main()

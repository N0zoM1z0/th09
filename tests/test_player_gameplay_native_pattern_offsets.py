"""The exact gameplay pattern resolver reads Player's real movement/SHT fields.

This is a source/layout identity test; exact original/COFF equivalence of
the two independently compiled gameplay owners is checked separately.
"""
import tomllib
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class PlayerGameplayNativePatternOffsets(unittest.TestCase):
    def test_gameplay_resolver_uses_real_player_lifecycle_storage(self):
        source = (ROOT / 'src/PlayerGameplayState.cpp').read_text()
        header = (ROOT / 'src/PlayerLifecycleView.hpp').read_text()
        self.assertNotIn('struct PlayerPatternOffsetView', source)
        self.assertIn('PlayerLifecycleView *player =', source)
        self.assertIn('static_cast<PlayerLifecycleView *>(this);', source)
        self.assertIn('struct PlayerGameplayMethods : PlayerLifecycleView',source)
        self.assertIn('PlayerPatternNativeMovementAndShtOffsets', source)
        self.assertIn('offsetof(PlayerLifecycleView, primaryShtFile) == 0x30338', source)
        self.assertIn('offsetof(PlayerLifecycleView, movementFields1CCC)', source)
        self.assertIn('PlayerMovementFields movementFields1CCC;', header)
        self.assertIn('PlayerShtFileView *primaryShtFile;', header)
        for name in ('speedMultiplier1CDC', 'speedMultiplier1CE0',
                     'speedMultiplier1CE4', 'speedMultiplier1CE8'):
            self.assertIn('player->movementFields1CCC.' + name, source)
        self.assertNotIn('player->primaryShtFile30338', source)

    def test_original_exact_gameplay_referents_remain_frozen(self):
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        for name, address, size, fields in (
            ('player-resolve-pattern-offset', 0x00404710, 346, 18),
            ('player-update-selector-state', 0x004049A0, 3238, 136),
        ):
            with self.subTest(name=name):
                u = units[name]
                self.assertEqual(u['target_address'], address)
                self.assertEqual(u['size'], size)
                self.assertEqual(len(u['relocations']), fields)


if __name__ == '__main__':
    unittest.main()

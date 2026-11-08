"""Two original-exact Player shot callbacks share native movement scalars.

Their externally consumed callback and rectangle method symbol spellings stay
unchanged. Exactness is independently proven by the focused original/COFF
match-unit replays, not by this source-side test.
"""

from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class PlayerShotCallbacksNativeMovement(unittest.TestCase):
    def test_spawn_type3_uses_native_player_scalar_without_fake_storage(self):
        s = (ROOT/'src/PlayerShotSpawnCallback3.cpp').read_text()
        self.assertIn('struct PlayerShotSpawn3PlayerView;', s)
        self.assertNotIn('unsigned char unknown0000[0x1CDC]', s)
        self.assertIn('reinterpret_cast<PlayerLifecycleView *>(player);', s)
        self.assertIn('lifecycle->movementFields1CCC.speedMultiplier1CE0 = 0.5f;', s)
        self.assertIn('lifecycle->movementFields1CCC.speedMultiplier1CDC = 0.5f;', s)
        self.assertIn('reinterpret_cast<PlayerLifecycleView *>(player)->InitializeShot(', s)

    def test_update_type4_uses_native_player_position_and_scalars(self):
        s = (ROOT/'src/PlayerShotUpdateCallback4.cpp').read_text()
        self.assertIn('#include "PlayerLifecycleView.hpp"', s)
        self.assertIn('struct PlayerShotUpdate4PlayerView', s)
        self.assertNotIn('unsigned char unknown0000[0x1B88]', s)
        self.assertIn('&lifecycle->position1B88', s)
        self.assertIn('lifecycle->movementFields1CCC.speedMultiplier1CE0 = 1.0f;', s)
        self.assertIn('lifecycle->movementFields1CCC.speedMultiplier1CDC = 1.0f;', s)
        self.assertIn('player->CreateRectType0(', s)
        self.assertIn('player->CreateRectType1(', s)

    def test_original_callback_and_callee_owners_remain_pinned(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        for name, address, size, fields in (
            ('player-shot-spawn-callback-type3', 0x004432A0, 86, 1),
            ('player-shot-update-callback-type4', 0x00443930, 249, 9),
            ('player-initialize-shot', 0x0041F350, 309, 7),
        ):
            with self.subTest(name=name):
                u = units[name]
                self.assertEqual((u['target_address'], u['size'], len(u['relocations'])),
                                 (address, size, fields))


if __name__ == '__main__':
    unittest.main()

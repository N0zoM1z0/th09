"""Native PlayerShotCtorView consumed by exact 249-byte shot callback.

The original callback/method ABI is preserved while its private Shot storage
overlay is eliminated; the actual original bytes are independently compared.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class ExactShotUpdate4NativeShot(unittest.TestCase):
    def test_real_shot_record_has_original_player_state_offsets(self):
        header = (ROOT / 'src/PlayerLifecycleView.hpp').read_text()
        source = (ROOT / 'src/PlayerShotUpdateCallback4.cpp').read_text()
        self.assertIn('struct PlayerShotUpdate4ShotView;', source)
        self.assertNotIn('struct PlayerShotUpdate4ShotView\n{', source)
        self.assertIn('PlayerShotCtorView *nativeShot =', source)
        self.assertIn('reinterpret_cast<PlayerShotCtorView *>(shot);', source)
        self.assertIn('PlayerShotUpdate4NativeShotOffsets', source)
        for field, offset in (('position2A4', '0x2A4'), ('velocity43C', '0x43C'),
                              ('timer454', '0x454'), ('state462', '0x462')):
            self.assertIn('offsetof(PlayerShotCtorView, ' + field + ') == ' + offset,
                          source)
            self.assertIn(field, header)
        self.assertIn('reinterpret_cast<ZunTimer *>(&nativeShot->timer454)', source)
        self.assertIn('nativeShot->state462 == 1', source)
        self.assertIn('lifecycle->CreateRectType0(', source)
        self.assertIn('lifecycle->CreateRectType1(', source)
        self.assertNotIn('struct PlayerShotUpdate4PlayerView\n{', source)
        self.assertIn('PlayerCollisionRegionCreateRegionView *CreateRectType1(', header)
        self.assertIn('PlayerCollisionRegionCreateRegionView *CreateRectType0(', header)

    def test_real_original_x87_operand_order_preserved_by_snapshot(self):
        source = (ROOT / 'src/PlayerShotUpdateCallback4.cpp').read_text()
        self.assertIn('float velocityY = nativeShot->velocity43C.y;', source)
        self.assertIn('nativeShot->position2A4.y += velocityY;', source)
        self.assertIn('nativeShot->velocity43C.y -= 1.0f;', source)
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            unit = tomllib.load(stream)['units']['player-shot-update-callback-type4']
        self.assertEqual((unit['target_address'], unit['size'],
                          len(unit['relocations'])), (0x00443930, 249, 9))


if __name__ == '__main__':
    unittest.main()

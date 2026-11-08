"""The 86-byte original Type3 shot-spawn callback consumes real Shot velocity.

The externally consumed callback parameter alias stays frozen, whereas its
physical velocity fields come from PlayerShotCtorView at +0x43C.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class ExactShotSpawn3NativeShot(unittest.TestCase):
    def test_exact_spawn_receiver_uses_shared_native_shot_velocity(self):
        source = (ROOT/'src/PlayerShotSpawnCallback3.cpp').read_text()
        player = (ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('struct PlayerShotSpawn3ShotView;', source)
        self.assertNotIn('struct PlayerShotSpawn3ShotView\n{', source)
        self.assertIn('PlayerShotSpawn3NativeVelocityAt43C', source)
        self.assertIn('PlayerShotCtorView *nativeShot =', source)
        self.assertIn('reinterpret_cast<PlayerShotCtorView *>(shot);', source)
        self.assertIn('nativeShot->velocity43C.x = 0.0f;', source)
        self.assertIn('nativeShot->velocity43C.y = -24.0f;', source)
        self.assertIn('PlayerPositionView velocity43C;', player)
        self.assertIn('PlayerShotCtorView', player)
        self.assertIn('reinterpret_cast<PlayerLifecycleView *>(player)->InitializeShot(', source)

    def test_original_callback_callee_and_peer_are_pinned(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        for name, addr, size, fields in (
            ('player-shot-spawn-callback-type3',0x004432A0,86,1),
            ('player-shot-update-callback-type4',0x00443930,249,9),
            ('player-initialize-shot',0x0041F350,309,7),
        ):
            with self.subTest(name=name):
                u=units[name]
                self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),
                                 (addr,size,fields))


if __name__=='__main__':
    unittest.main()

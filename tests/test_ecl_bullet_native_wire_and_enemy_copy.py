"""Keep ECL/Enemy wire copies attached to one native original storage layout."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeEclWireAndEnemyDescriptor(unittest.TestCase):
    def test_laser_and_shot_use_native_wire_payload(self):
        wire = (ROOT/'src/EclOpcodes.hpp').read_text()
        bullet = (ROOT/'src/EclRunBullet.inl').read_text()
        post = (ROOT/'src/EclPostRuntime.inl').read_text()
        self.assertIn('offsetof(Th09EclRawInstructionHeaderView, arguments) == 0x0C', wire)
        self.assertIn('instruction->arguments', bullet)
        self.assertIn('instruction->arguments', post)
        self.assertNotIn('reinterpret_cast<unsigned char *>(instruction) + 0x0C', bullet)
        self.assertNotIn('reinterpret_cast<unsigned char *>(instruction) + 0x0C', post)
        self.assertNotIn('reinterpret_cast<unsigned char *>(instruction) + 0x0E', bullet)
        self.assertIn('Th09EclRunBullet::ReadLaserFloat', bullet)
        self.assertIn('Th09EclRunBullet::ReadLaserInt', bullet)

    def test_enemy_bullet_descriptor_copies_native_pod_xyz(self):
        source = (ROOT/'src/EnemyManagerCore.inl').read_text()
        self.assertIn('BulletSpawnDescriptorCtorView storage;', source)
        self.assertIn('descriptor.position =', source)
        self.assertIn('reinterpret_cast<const Float3 &>(enemy->position2D74);', source)
        self.assertNotIn('memcpy(&descriptor.position, &enemy->position2D74, sizeof(descriptor.position));', source)
        native=(ROOT/'src/Float3.hpp').read_text()
        self.assertIn('sizeof(Float3)',native) if 'sizeof(Float3)' in native else self.assertIn('struct Float3',native)


if __name__ == '__main__':
    unittest.main()

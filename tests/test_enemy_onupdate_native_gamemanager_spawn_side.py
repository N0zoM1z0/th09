"""The 3,900-byte Enemy boss uses real 0x38-byte GameManager side for Etama.

Complete pinned original PE/COFF source-cold and boss checks independently
prove bytes/fields/calls. These assertions protect the native C++ identity.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class EnemyOnUpdateNativeGameManagerSpawnSide(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_existing_exact_etama_descriptor_and_uncredited_boss(self):
        src=(ROOT/'src/EnemyManagerCore.inl').read_text()
        self.assertIn('BulletSpawnDescriptorCtorView storage;',src)
        self.assertIn('BulletSpawnDescriptor &descriptor = storage.descriptor;',src)
        self.assertIn('descriptor.position =',src)
        self.assertIn('sizeof(BulletSpawnDescriptor) == 0x214',src)
        self.assertEqual(self.units['enemy-attached-effect-update']['size'],151)
        self.assertEqual(self.units['enemy-collision-append-box']['size'],82)
        self.assertEqual(self.units['bullet-pattern-primary']['target_address'],0x004130F0)
        # Original 3,900-byte OnUpdate still has 13 unlike bytes and is
        # deliberately NOT added as a canonical byte-exact unit.
        self.assertFalse(any(u['target_address']==0x00410730
                             for u in self.units.values()))
        self.assertIn('int __fastcall EnemyManagerView::OnUpdate(',src)

if __name__=='__main__':unittest.main()

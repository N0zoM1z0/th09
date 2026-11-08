"""Original-exact Type1 uses the real loader-owned 0x38-byte SHT descriptor.

Strict target PE/VC7.1 source-cold replay separately validates the bytes.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSpawnType1NativeShtDescriptor(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as fd:
            cls.units=tomllib.load(fd)['units']

    def test_one_true_descriptor_allocation_across_sht_loader_and_spawn(self):
        callback=(ROOT/'src/PlayerShotSpawnCallback1.cpp').read_text()
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        runtime=(ROOT/'src/PlayerRuntime.cpp').read_text()
        self.assertIn('PlayerShtDescriptorView *entry',callback)
        self.assertNotIn('struct PlayerShotSpawn1DescriptorView',callback)
        self.assertIn('PlayerShtNativeType1SpawnFields',header)
        self.assertIn('PlayerShtDescriptorSizeIs38',header)
        self.assertIn('PlayerShtDescriptorView *descriptors;',header)
        self.assertIn('PlayerShtDescriptorView *descriptor;',runtime)
        self.assertIn('void *spawnCallback;',header)
        for name,off in (('fireInterval','0x00'),('positionOffsetX04','0x04'),
                         ('positionOffsetY08','0x08'),('sourceOptionIndex20','0x20'),
                         ('spawnCallback','0x28')):
            with self.subTest(member=name):
                self.assertIn('offsetof(PlayerShtDescriptorView, '+name+') == '+off,header)
        for stmt in ('value != entry->fireInterval',
                     'entry->sourceOptionIndex20',
                     'entry->positionOffsetX04',
                     'entry->positionOffsetY08'):
            self.assertIn(stmt,callback)
        self.assertIn('PlayerLifecycleView *player,',callback)
        self.assertIn('PlayerShotCtorView *shot,',callback)
        self.assertIn('PlayerShotSpawn1RealNativeState',callback)

    def test_all_original_source_bytes_and_two_callee_positions(self):
        callback=self.units['player-shot-spawn-callback-type1']
        self.assertEqual((callback['target_address'],callback['size'],len(callback['relocations'])),
                         (0x00441EF0,151,2))
        self.assertEqual(callback['symbol'],'?PlayerShotSpawnCallbackType1@@YIHPAUPlayerLifecycleView@@PAUPlayerShotCtorView@@HPAUPlayerShtDescriptorView@@@Z')
        self.assertEqual([(r['offset'],r['type'],r['target']) for r in callback['relocations']],
                         [(0x27,'REL32',0x00401500),(0x5D,'REL32',0x0041F350)])
        self.assertEqual(self.units['player-constructor']['size'],243)
        self.assertEqual(self.units['player-state3031c-constructor']['size'],12)

if __name__=='__main__':unittest.main()

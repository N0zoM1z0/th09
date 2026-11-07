"""Three original-exact TH09 SHT callbacks share real Player Shot/SHT fields.

These guard source and original referent contracts. The whole-function pinned
VC7.1 PE/COFF comparison, not source assertions, establishes byte exactness.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativePlayerShotCallbackOwners(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            cls.units=tomllib.load(stream)['units']

    def test_three_callbacks_use_same_native_record_and_no_private_timer(self):
        for file in ('PlayerShotUpdateCallback1.cpp',
                     'PlayerShotUpdateCallback3.cpp',
                     'PlayerShotSpawnCallback7.cpp'):
            with self.subTest(source=file):
                src=(ROOT/'src'/file).read_text()
                self.assertIn('PlayerShotCtorView *shot',src)
                self.assertIn('player->tracking30364',src)
                self.assertNotIn('struct PlayerShotUpdate',src)
                self.assertNotIn('struct PlayerShotSpawn7',src)
                self.assertNotIn('TimerView',src)
                self.assertIn('shot->velocity43C.',src)
        for file in ('PlayerShotUpdateCallback1.cpp','PlayerShotUpdateCallback3.cpp'):
            src=(ROOT/'src'/file).read_text()
            self.assertIn('static_cast<int>(shot->timer454)',src)
            self.assertIn('Th09EclRunControl::VectorAngle(',src)
        spawn=(ROOT/'src/PlayerShotSpawnCallback7.cpp').read_text()
        self.assertIn('PlayerShtDescriptorView *entry',spawn)
        self.assertIn('entry->fireInterval',spawn)
        self.assertIn('entry->angle14',spawn)
        self.assertIn('entry->speed18',spawn)
        self.assertNotIn('PlayerShotSpawn7DescriptorView',spawn)

    def test_sht_native_float_offset_and_callback_destination(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('float angle14;',header)
        self.assertIn('float speed18;',header)
        self.assertIn('PlayerShtNativeAngleAndSpeed',header)
        self.assertIn('offsetof(PlayerShtDescriptorView, angle14) == 0x14',header)
        self.assertIn('offsetof(PlayerShtDescriptorView, speed18) == 0x18',header)
        self.assertIn('offsetof(PlayerShtDescriptorView, spawnCallback) == 0x28',header)
        self.assertIn('PlayerShotCtorSizeIs484',header)

    def test_three_complete_original_function_and_native_callee_fields(self):
        goals=(('player-shot-update-callback-type1',471),
               ('player-shot-update-callback-type3',248),
               ('player-shot-spawn-callback-type7',139))
        for unit,size in goals:
            with self.subTest(unit=unit):
                spec=self.units[unit]
                self.assertEqual(spec['size'],size)
                self.assertIn('PAUPlayerShotCtorView@@',spec['symbol'])
        a=self.units['player-shot-update-callback-type1']['relocations']
        b=self.units['player-shot-update-callback-type3']['relocations']
        self.assertTrue(any(r['symbol']=='??BZunTimer@@QAEHXZ' and
                            r['target']==0x00435F00 and r['offset']==0x24 for r in a))
        self.assertTrue(any(r['symbol']=='??BZunTimer@@QAEHXZ' and
                            r['target']==0x00435F00 and r['offset']==0x21 for r in b))
        self.assertTrue(any(r['symbol']=='??BPlayerPositionView@@QAEPAMXZ' and
                            r['target']==0x004343D0 and r['offset']==0x5E for r in a))
        self.assertEqual(len(a),19)
        self.assertEqual(len(b),9)
        self.assertEqual(len(self.units['player-shot-spawn-callback-type7']['relocations']),5)
        tracked=(ROOT/'config/functions.csv').read_text()
        self.assertNotIn('PlayerShotUpdate1ShotView',tracked)
        self.assertNotIn('PlayerShotUpdate3ShotView',tracked)
        self.assertNotIn('PlayerShotSpawn7DescriptorView',tracked)

if __name__=='__main__':
    unittest.main()

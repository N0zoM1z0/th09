"""Native 0x8E ReplayInputState owns both network and ordinary input windows."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeNetworkReplayInputWindows(unittest.TestCase):
    def test_native_supervisor_owns_keyboard_and_controller_mapping_bits(self):
        source=(ROOT/'src/SampleNetworkInput.cpp').read_text()
        self.assertIn('#include "Supervisor.hpp"',source)
        self.assertNotIn('struct SampleNetworkInputSupervisorView',source)
        self.assertNotIn('extern SampleNetworkInputSupervisorView g_Supervisor;',source)
        self.assertIn('SampleNetworkNativeSupervisorOwner',source)
        self.assertIn('offsetof(Supervisor, keyboard10) == 0x10',source)
        self.assertIn('offsetof(Supervisor, config388) == 0x388',source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        u=units['sample-network-input']
        fields=[r for r in u['relocations']
                if r['symbol']=='?g_Supervisor@@3VSupervisor@@A']
        self.assertEqual(len(fields),8)
        self.assertEqual([(r['offset'],r['target']) for r in fields],
                         [(0x2C,0x4B3100),(0x57,0x4B3100),
                          (0x2A7,0x4B3100),(0x2BB,0x4B3100),
                          (0x2DC,0x4B3100),(0x519,0x4B3100),
                          (0x537,0x4B3100),(0x54C,0x4B3100)])

    def test_original_network_apply_global_address_is_true_replay_global(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        apply=units['apply-network-input']
        self.assertEqual((apply['target_address'],apply['size']),(0x42ACE0,131))
        self.assertEqual(apply['relocations'],[dict(
            offset=0x0C,type='DIR32',symbol='?g_ReplayInputStates@@3PAUReplayInputState@@A',
            target=0x4ACE18)])
        self.assertTrue(any(r['symbol']==apply['relocations'][0]['symbol'] and
                            r['target']==0x4ACE18
                            for r in units['replay-manager-playback-input']['relocations']))
        self.assertEqual(units['replay-input-update']['size'],111)

if __name__=='__main__':unittest.main()

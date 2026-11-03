"""Native 0x8E ReplayInputState owns both network and ordinary input windows."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeNetworkReplayInputWindows(unittest.TestCase):
    def test_network_repeat_window_is_distinct_from_replay_held_window(self):
        source=(ROOT/'src/ApplyNetworkInput.cpp').read_text()
        native=(ROOT/'src/ReplayInputState.hpp').read_text()
        self.assertNotIn('struct ReplayInputFrontState',source)
        self.assertIn('#include "ReplayInputState.hpp"',source)
        self.assertIn('ReplayInputState *state = &g_ReplayInputStates[stateIndex];',source)
        self.assertIn('u16 *held = state->networkHeldFrames0A;',source)
        for member,offset in (('networkPrevious02','0x02'),('networkRepeat04','0x04'),
                              ('networkPressed06','0x06'),('networkReleased08','0x08'),
                              ('networkHeldFrames0A','0x0A'),('heldFrames','0x38')):
            with self.subTest(member=member):
                self.assertIn('offsetof(ReplayInputState, '+member+') == '+offset,native)
        self.assertIn('sizeof(ReplayInputState) == 0x8E',native)
        self.assertIn('unsigned char unknown0A[0x20];',native)
        self.assertIn('unsigned short networkHeldFrames0A[16];',native)
        self.assertIn('unsigned short heldFrames[16];',native)

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

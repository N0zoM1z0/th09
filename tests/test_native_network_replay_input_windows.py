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

    def test_exact_1507_byte_sample_owner_reads_same_native_network_words(self):
        source=(ROOT/'src/SampleNetworkInput.cpp').read_text()
        self.assertIn('#include "ReplayInputState.hpp"',source)
        self.assertNotIn('struct SampleNetworkInputState',source)
        self.assertNotIn('extern SampleNetworkInputState g_ReplayInputStates[3];',source)
        self.assertIn('ReplayInputState *inputState = &g_ReplayInputStates[side];',source)
        self.assertIn('SampleNetworkNativeReplayInput',source)
        for name in ('currentInput','networkPrevious02','networkRepeat04',
                     'networkPressed06','networkReleased08','networkHeldFrames0A'):
            with self.subTest(field=name):
                self.assertIn('inputState->'+name,source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']['sample-network-input']
        self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),
                         (0x42B850,1507,13))
        r=u['relocations'][1]
        self.assertEqual((r['offset'],r['symbol'],r['target']),
                         (0x1F,'?g_ReplayInputStates@@3PAUReplayInputState@@A',0x4ACE18))

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

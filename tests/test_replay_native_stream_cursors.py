"""Cross-stream ReplayManager cursor layout and real writer/reader consumers.

Source guards complement, but do not substitute for, pinned original PE/COFF
comparisons of complete VC7.1 outputs for Replay, Title and GameManager.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class ReplayNativeStreamCursors(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header = (ROOT / 'src/ReplayManagerRuntimeView.hpp').read_text()
        cls.source = (ROOT / 'src/ReplayInput.cpp').read_text()
        cls.gates = (ROOT / 'src/ReplayNativeInputGateCapacity.hpp').read_text()
        cls.units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']

    def test_cursor_storage_is_three_adjacent_native_words(self):
        h = self.header
        self.assertIn('u16 *inputCursors[ReplayRecordingCapacity::InputStreamCount];', h)
        self.assertIn('ReplayManagerInputCursorsAt0C', h)
        for assertion in (
            'offsetof(ReplayManagerView, inputCursors) == 0x0C',
            'sizeof(((ReplayManagerView *)0)->inputCursors) == 0x0C',
            'ReplayRecordingCapacity::InputStreamCount == 3',
            'offsetof(ReplayManagerView, fpsCursor) ==',
            'sizeof(((ReplayManagerView *)0)->inputCursors))',
            'ReplayManagerSizeIs164',
            'u16 *input[3];',
        ):
            with self.subTest(assertion=assertion):
                self.assertIn(assertion, h)
        for stale in ('u16 *inputCursor0;', 'u16 *inputCursor1;', 'u16 *inputCursor2;'):
            self.assertNotIn(stale, h)

    def test_record_and_playback_use_the_same_three_stream_coordinates(self):
        s = self.source
        for index in range(3):
            with self.subTest(stream=index):
                cursor = f'replayManager->inputCursors[{index}]'
                self.assertGreaterEqual(s.count(cursor), 8)
                self.assertIn(f'replayManager->currentBuffer->input[{index}] = {cursor};',s)
                self.assertIn(f'memset({cursor}, 0, ReplayRecordingCapacity::InputStreamBytes);',s)
        for stale in ('replayManager->inputCursor0', 'replayManager->inputCursor1',
                      'replayManager->inputCursor2'):
            self.assertNotIn(stale, s)
        self.assertIn('ReplayRecordingCapacity::InputStreamCount == ReplayNativeInputGateCapacity::StateCount',self.gates)

    def test_native_array_is_shared_by_exact_record_playback_and_open_setup(self):
        names = ('replay-manager-record-input','replay-manager-playback-input',
                 'replay-manager-begin-recording-stage')
        for name in names:
            with self.subTest(owner=name):
                self.assertEqual(self.units[name]['source'], 'src/ReplayInput.cpp')
                self.assertGreater(self.units[name]['size'], 250)
        self.assertNotIn('replay-manager-begin-playback-stage',self.units)
        self.assertNotIn('replay-manager-save-replay',self.units)


if __name__ == '__main__':
    unittest.main()

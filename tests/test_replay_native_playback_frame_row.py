"""Original playback checks the native replay frame table-row address.

The observed (potentially surprising) original address check must not be
'safely fixed' into a different stored frame-pointer test. Strict 540-byte
PE/COFF diagnostics remain the byte oracle, these are type/ABI guardrails.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class ReplayNativePlaybackFrameRow(unittest.TestCase):
    def test_playback_native_row_address_not_frame_data_pointer(self):
        source=(ROOT/'src/ReplayInput.cpp').read_text()
        header=(ROOT/'src/ReplayDataOriginalView.hpp').read_text()
        s=source.split('int ReplayManagerView::BeginPlaybackStage(',1)[1].split(
            '\nint ReplayManagerView::CaptureFrameSyncState(',1)[0]
        self.assertIn('if (replayManager->replayData->frameStart[stage] == NULL)',s)
        self.assertNotIn('frameStart[0][stage] == NULL',s)
        self.assertNotIn('stage * sizeof(replayManager->replayData->frameStart[0])',s)
        self.assertNotIn('offsetof(ReplayDataView, frameStart) +',s)
        self.assertIn('ReplayNativePlaybackFrameRowAddress',source)
        self.assertIn('sizeof(((ReplayDataView *)0)->frameStart[0]) == 0x28',source)
        self.assertIn('ReplayRecordingCapacity::StageCount * sizeof(ReplayFrameDataStartView *)',source)
        self.assertIn('ReplayRecordingCapacity::InputStreamCount * 0x28',source)
        self.assertIn('ReplayFrameDataStartView *frameStart[3][10]',header)
        self.assertIn('ReplayDataFrame0At20',header)

    def test_unmatched_boss_and_exact_same_source_are_separate(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertEqual(units['replay-manager-begin-recording-stage']['source'],'src/ReplayInput.cpp')
        self.assertEqual(units['replay-manager-begin-recording-stage']['target_address'],0x00421140)
        self.assertEqual(units['replay-manager-begin-recording-stage']['size'],1323)
        self.assertNotIn('replay-manager-begin-playback-stage',units)

if __name__=='__main__':
    unittest.main()

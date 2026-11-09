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

    def test_side0_snapshot_and_runtime_owner_follow_native_load_order(self):
        source = (ROOT/'src/ReplayInput.cpp').read_text()
        body = source.split('int ReplayManagerView::BeginPlaybackStage(',1)[1].split(
            '\nint ReplayManagerView::CaptureFrameSyncState(',1)[0]
        sites = [
            'g_GameManager.value37C = replayManager->replayData->value1E7;',
            'const u8 side0Value1E8 = replayManager->replayData->value1E8;',
            'AsciiGameSideRuntimeView *volatile &side0RuntimeSlot =',
            'AsciiGameSideRuntimeView *firstSide0 = side0RuntimeSlot;',
            'g_GameManager.sides[0].value30 = side0Value1E8;',
        ]
        offsets = [body.index(site) for site in sites]
        self.assertEqual(offsets, sorted(offsets))
        self.assertEqual(body.count('g_GameManager.sides[0].value30 ='),1)
        self.assertIn('g_GameManager.sides[0].runtime;', body)
        self.assertNotIn('ReplaySideRuntimeSlotView', source)
        self.assertNotIn('reinterpret_cast<ReplaySideRuntimeSlotView', body)
        # The target's separate side owner accesses must not be merged into
        # a single potentially stale runtime pointer through the whole body.
        self.assertIn('g_GameManager.sides[0].runtime->value04 = frame0->value00;',body)

    def test_unmatched_boss_and_exact_same_source_are_separate(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertEqual(units['replay-manager-begin-recording-stage']['source'],'src/ReplayInput.cpp')
        self.assertEqual(units['replay-manager-begin-recording-stage']['target_address'],0x00421140)
        self.assertEqual(units['replay-manager-begin-recording-stage']['size'],1323)
        self.assertNotIn('replay-manager-begin-playback-stage',units)

if __name__=='__main__':
    unittest.main()

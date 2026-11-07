"""TH09 native Replay link geometry, FPS sample interval and source owners.

These are structural and arithmetic regression checks. Original byte evidence
comes from the pinned PE/COFF affected-owner replay, not source tests.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class ReplayNativeFrameCapacity(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header=(ROOT/'src/ReplayManagerRuntimeView.hpp').read_text()
        cls.source=(ROOT/'src/ReplayInput.cpp').read_text()

    def test_true_recording_link_has_three_unsigned_short_input_streams(self):
        self.assertIn('struct ReplayBufferLink',self.header)
        self.assertIn('u16 *input[3];',self.header)
        self.assertIn('u8 *fps;',self.header)
        self.assertIn('i32 frameCount;',self.header)
        self.assertIn('ReplayBufferLinkSizeIs18',self.header)
        self.assertIn('ReplayBufferLinkNextAt14',self.header)
        self.assertIn('InputFrameBytes = sizeof(((ReplayBufferLink *)0)->input[0][0])',self.header)
        self.assertIn('InputFrameCapacity = InputStreamBytes / InputFrameBytes',self.header)
        self.assertIn('FpsStreamBytes = InputFrameCapacity / FramesPerFpsSample + 1',self.header)
        self.assertIn('LinkRolloverFrames = InputFrameCapacity - 2',self.header)
        self.assertIn('ReplayRecordingCapacityPhysicalSizes',self.header)

    def test_native_frame_bounds_and_lookahead_are_consistent(self):
        stream_bytes=0x1C20
        frame_bytes=2
        sample_frames=30
        frame_capacity=stream_bytes//frame_bytes
        rollover=frame_capacity-2
        fps_capacity=frame_capacity//sample_frames+1
        self.assertEqual((frame_capacity,rollover,fps_capacity),(3600,3598,121))
        self.assertEqual(fps_capacity,0x79)
        self.assertLess(rollover,frame_capacity)
        self.assertIn('replayManager->fpsCursor[1]',self.source)
        self.assertIn('LinkRolloverFrames == 3598',self.header)
        self.assertIn('FpsStreamBytes == 0x79',self.header)

    def test_record_playback_save_and_wire_field_share_one_interval(self):
        source=self.source
        self.assertEqual(source.count('frameCounter % ReplayRecordingCapacity::FramesPerFpsSample'),2)
        self.assertIn('currentBuffer->frameCount >= ReplayRecordingCapacity::LinkRolloverFrames',source)
        self.assertIn('ReplayRecordingCapacity::InputFrameBytes * link->frameCount',source)
        self.assertIn('link->frameCount / ReplayRecordingCapacity::FramesPerFpsSample',source)
        self.assertIn('replayCopy.value1D0 = ReplayRecordingCapacity::FramesPerFpsSample;',source)
        self.assertIn('FramesPerFpsSample = 30',self.header)
        self.assertNotIn('currentBuffer->frameCount >= 3598',source)
        self.assertNotIn('int fpsSize = link->frameCount / 30;',source)

    def test_source_present_exact_and_open_owners_remain_separately_tracked(self):
        units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        names=[n for n,u in units.items() if u['source']=='src/ReplayInput.cpp']
        self.assertEqual(len(names),12)
        self.assertIn('replay-manager-record-input',names)
        self.assertIn('replay-manager-playback-input',names)
        self.assertIn('replay-manager-begin-recording-stage',names)
        self.assertNotIn('replay-manager-save-replay',units)
        self.assertNotIn('replay-manager-begin-playback-stage',units)
        self.assertEqual(units['replay-manager-begin-recording-stage']['size'],1323)
        self.assertEqual(units['replay-manager-record-input']['size'],460)
        self.assertEqual(units['replay-manager-playback-input']['size'],299)


if __name__=='__main__':
    unittest.main()

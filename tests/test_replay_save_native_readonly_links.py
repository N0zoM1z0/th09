"""The 1238-byte SaveReplay boss serializes immutable stage buffer links."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class ReplaySaveNativeReadOnlyLinks(unittest.TestCase):
    def test_recorded_stage_chain_is_readonly_during_serialization(self):
        source=(ROOT/'src/ReplayInput.cpp').read_text()
        start=source.index('int ReplayManagerView::SaveReplay(')
        end=source.index('\n\nReplayInputState::ReplayInputState()',start)
        save=source[start:end]
        self.assertIn('const ReplayBufferLink *stageRoot = &replayManager->stageBuffers[0];',save)
        self.assertIn('for (const ReplayBufferLink *link = stageRoot; link != NULL; link = link->next)',save)
        self.assertEqual(save.count('const ReplayBufferLink *link = &replayManager->stageBuffers[loopIndex];'),2)
        self.assertEqual(save.count('link = link->next'),3)
        self.assertIn(
            'memcpy(tempBuffer + currentOffset - sizeof(ReplayHeaderView), frameHeader, sizeof(*frameHeader));',
            save)
        # The size is taken from the native wire record's declared type,
        # whose complete 0x20-byte extent is independently layout-guarded.
        self.assertIn('ReplayFrameDataStartSizeIs20',
                      (ROOT/'src/ReplayDataOriginalView.hpp').read_text())
        self.assertNotIn('memcpy(tempBuffer + currentOffset - sizeof(ReplayHeaderView), frameHeader, 32)',save)
        self.assertIn('memcpy(tempBuffer + currentOffset - sizeof(ReplayHeaderView), link->input[stream], streamSize);',save)
        self.assertIn('memcpy(tempBuffer + currentOffset - sizeof(ReplayHeaderView), link->fps, fpsSize);',save)
        self.assertEqual(save.count('replayCopy.frameOffset[stream][loopIndex] = currentOffset;'),1)
        self.assertNotIn('link->frameCount =',save)

    def test_exact_siblings_stay_original_bound_and_nonexact_stays_open(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        same=[(n,v) for n,v in u.items() if v['source']=='src/ReplayInput.cpp']
        self.assertEqual(len(same),12)
        self.assertEqual(sum(v.get('compare_size',v['size']) for n,v in same),4093)
        self.assertEqual(u['replay-manager-begin-recording-stage']['target_address'],0x00421140)

if __name__=='__main__':unittest.main()

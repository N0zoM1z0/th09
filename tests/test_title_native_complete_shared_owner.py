"""One native 0x1B388-byte Title object owns core, replay menu and draw-save.

This guards C++ storage/receiver identity; original byte equality is
independently established by pinned VC7.1 scoped PE/COFF complete comparisons.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeTitleCompleteSharedOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_single_real_full_title_screen_definition(self):
        header=(ROOT/'src/TitleScreenReplayView.hpp').read_text()
        core=(ROOT/'src/TitleScreen.cpp').read_text()
        self.assertEqual(header.count('struct TitleScreenView {'),1)
        self.assertNotIn('struct TitleScreenView {',core)
        self.assertIn('#include "TitleScreenReplayView.hpp"',core)
        self.assertIn('sizeof(TitleScreenView) == 0x1B388',header)
        self.assertIn('TitleReplayNativeFullOwnerExtent',header)
        self.assertIn('TitleScreenSizeIs1B388',core)
        self.assertIn('struct AnmVmView : AnmVm',header)
        self.assertNotIn('struct AnmVmView : AnmVm',core)
        self.assertIn('struct TitleConfigSnapshotView {',header)
        self.assertNotIn('struct TitleConfigSnapshotView {',core)
        for f in ('TitleScreenReplayMenuDraw.cpp','TitleScreenReplaySaveDraw.cpp'):
            with self.subTest(source=f):
                src=(ROOT/'src'/f).read_text()
                self.assertIn('#include "TitleScreenReplayView.hpp"',src)
                self.assertNotIn('struct TitleScreenView',src)

    def test_replay_prefix_is_real_title_allocation_not_opaque_shadows(self):
        native=(ROOT/'src/TitleScreenReplayView.hpp').read_text()
        shared=(ROOT/'src/TitleReplayNativeSlots.hpp').read_text()
        expected=(('replayAllocations','0x681C'),('replays','0x68E4'),
                  ('currentReplay','0xC8FC'),('selectedReplay','0xC908'),
                  ('embeddedVms','0x11BA0'),('stateTimer2','0x1B230'),
                  ('configSnapshot','0x1B2BC'))
        for member,offset in expected:
            with self.subTest(member=member):
                self.assertIn('offsetof(TitleScreenView, '+member+') == '+offset,
                              native+shared)
        self.assertIn('ReplayDataView *replayAllocations[50];',native)
        self.assertIn('ReplayDataView replays[50];',native)
        self.assertIn('ReplayDataView *currentReplay;',native)
        self.assertIn('AnmVmView embeddedVms[57];',native)
        self.assertIn('TitleConfigSnapshotView configSnapshot;',native)
        self.assertNotIn('unknown0C8FC',native+shared)
        self.assertIn('sizeof(((TitleScreenView *)0)->replays) == 0x6018',shared)

    def test_complete_core_and_exact_draw_save_distinct_from_open_menu(self):
        core=[u for u in self.units.values() if u['source']=='src/TitleScreen.cpp' and
              u['object']=='build/matching/TitleScreen.obj']
        self.assertEqual(len(core),40)
        self.assertEqual(sum(int(u.get('compare_size',u['size'])) for u in core),18756)
        separate=self.units['score-record-insert']
        self.assertEqual(separate['source'],'src/TitleScreen.cpp')
        self.assertEqual((separate['size'],len(separate['relocations'])),(216,5))
        save=self.units['title-screen-draw-replay-save']
        self.assertEqual((save['target_address'],save['size'],len(save['relocations'])),
                         (0x004239F6,800,41))
        self.assertEqual(save['symbol'],'?DrawReplaySave@TitleScreenView@@QAEHXZ')
        self.assertNotIn('title-draw-replay-menu',self.units)

if __name__=='__main__':unittest.main()

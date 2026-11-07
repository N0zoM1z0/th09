"""TH09 full Title options boss shares native Title, Replay and POD config.

The physical original byte oracle is a separate VC7.1 source-cold PE/COFF
comparison. This asserts the single layout and the no-constructor boundary.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleOptionsNativeCompleteOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_one_title_object_for_options_core_and_two_draws(self):
        shared=(ROOT/'src/TitleScreenReplayView.hpp').read_text()
        opts=(ROOT/'src/TitleScreenOptions.cpp').read_text()
        core=(ROOT/'src/TitleScreen.cpp').read_text()
        self.assertEqual(shared.count('struct TitleScreenView {'),1)
        self.assertNotIn('struct TitleScreenView {',opts)
        self.assertNotIn('struct TitleScreenView {',core)
        self.assertIn('#include "TitleScreenReplayView.hpp"',opts)
        self.assertIn('#include "TitleScreenReplayView.hpp"',core)
        self.assertIn('TitleReplayNativeFullOwnerExtent',shared)
        self.assertIn('sizeof(TitleScreenView) == 0x1B388',shared)
        self.assertIn('struct TitleAnmView : AnmLoaded {};',shared)
        self.assertNotIn('struct TitleAnmView : AnmLoaded {};',core)
        self.assertIn('TitleAnmView *titleAnm;',shared)
        self.assertIn('AnmVmView embeddedVms[57];',shared)
        self.assertIn('int OnUpdateOptions();',shared)
        self.assertIn('int TitleScreenView::OnUpdateOptions()',opts)
        self.assertIn('int TitleScreenView::PlayMenuSound(',opts)

    def test_config_snapshot_is_real_serialized_layout_without_ctor(self):
        header=(ROOT/'src/TitleScreenReplayView.hpp').read_text()
        config=(ROOT/'src/GameConfiguration.hpp').read_text()
        start=header.index('struct TitleConfigSnapshotView {')
        end=header.index('\n};',start)
        snap=header[start:end]
        self.assertIn('u32 words[0x33];',snap)
        self.assertIn('u8 unknown00[0xAD];',snap)
        self.assertIn('u8 valueAD;',snap)
        self.assertIn('u8 windowed;',snap)
        self.assertIn('u8 unknownB2[0xCC-0xB2];',snap)
        self.assertNotIn('GameConfiguration ',snap)
        self.assertNotIn('TitleConfigSnapshotView(',snap)
        self.assertIn('GameConfiguration();',config)
        self.assertIn('TitleNativeOptionsSnapshotFields',header)
        for field,off in (('valueAD','0xAD'),('windowed','0xB1')):
            self.assertIn('offsetof(TitleConfigSnapshotView, '+field+') == '+off,header)
        self.assertIn('optionsFrameCounter) == 0xC910',header)
        self.assertIn('optionsUiReady) == 0xC918',header)
        self.assertIn('TitleConfigSnapshotView configSnapshot;',header)
        self.assertNotIn('typedef GameConfiguration TitleConfigSnapshotView;',
                         (ROOT/'src/TitleScreenOptions.cpp').read_text())

    def test_full_original_bound_options_and_sound_leaf_are_exact_owners(self):
        boss=self.units['title-screen-options-update']
        sound=self.units['title-screen-play-menu-sound']
        self.assertEqual((boss['size'],len(boss['relocations'])),(2045,135))
        self.assertEqual((sound['size'],len(sound['relocations'])),(34,3))
        self.assertEqual(boss['source'],sound['source'])
        self.assertIn('@TitleScreenView@@',boss['symbol'])
        self.assertIn('@TitleScreenView@@',sound['symbol'])
        self.assertEqual(self.units['title-screen-draw-replay-save']['size'],800)
        self.assertNotIn('title-draw-replay-menu',self.units)

if __name__=='__main__':unittest.main()

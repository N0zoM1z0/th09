"""The exact TitleScreen owner borrows its native 0x8E replay input slot."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleMainNativeInputSlot(unittest.TestCase):
    def test_no_duplicate_main_input_prefix(self):
        title=(ROOT/'src/TitleScreen.cpp').read_text()
        inp=(ROOT/'src/TitleInputView.hpp').read_text()
        self.assertIn('#include "TitleInputView.hpp"',title)
        self.assertNotIn('struct InputView {',title)
        self.assertNotIn('extern InputView g_TitleInput;',title)
        self.assertIn('struct InputView : ReplayInputState',inp)
        self.assertIn('sizeof(InputView) == 0x8E',inp)
        self.assertIn('extern InputView g_TitleInput;',inp)
        self.assertIn('g_TitleInput.IsPressedScrolling(0x40)',title)
        self.assertIn('g_TitleInput.currentInput',title)

    def test_same_original_bound_global_and_independent_exact_leaf(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        symbols=[r for owner in u.values() if owner['source']=='src/TitleScreen.cpp'
                 for r in owner.get('relocations',[]) if r['symbol']=='?g_TitleInput@@3UInputView@@A']
        self.assertTrue(symbols)
        self.assertTrue(all(r['type']=='DIR32' and r['target']==0x004ACF34 for r in symbols))
        self.assertEqual(u['input-is-pressed-scrolling']['size'],42)
        self.assertEqual(u['title-screen-start-menu']['size'],1560)

if __name__=='__main__':unittest.main()

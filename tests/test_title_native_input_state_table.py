"""Both exact Title cursor movement methods use the native three-slot input array."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleNativeInputStateTable(unittest.TestCase):
    def test_original_table_is_three_real_input_records(self):
        s=(ROOT/'src/TitleScreen.cpp').read_text()
        header=(ROOT/'src/TitleInputView.hpp').read_text()
        self.assertIn('typedef InputView TitleInputStatePhysicalView;',s)
        self.assertNotIn('struct TitleInputStatePhysicalView {',s)
        self.assertIn('TitleInputStatePhysicalView states[3];',s)
        self.assertIn('sizeof(TitleInputStatePhysicalView) == 0x8E',s)
        self.assertIn('sizeof(TitleInputStateTableView) == sizeof(ReplayInputState) * 3',s)
        self.assertIn('struct InputView : ReplayInputState',header)
        self.assertIn('g_TitleInputStateTable.states[cursorIndex].IsPressedScrolling(0x40)',s)
        self.assertIn('g_TitleInputStateTable.states[inputIndex].IsPressedScrolling(0x80)',s)

    def test_two_original_cursor_bodies_and_eight_genuine_callees(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:units=tomllib.load(stream)['units']
        callee=units['input-is-pressed-scrolling']
        count=0
        for name,size,offsets in (
           ('title-screen-move-character-cursor-horizontal',167,(0x2D,0x52,0x61,0x70)),
           ('title-screen-move-character-cursor-horizontal-for-input',167,(0x2A,0x51,0x5F,0x6D))):
            owner=units[name]
            self.assertEqual(owner['size'],size)
            rows={r['offset']:r for r in owner['relocations']}
            for offset in offsets:
                with self.subTest(owner=name,offset=offset):
                    row=rows[offset]
                    self.assertEqual((row['symbol'],row['type'],row['target']),
                                     (callee['symbol'],'REL32',callee['target_address']))
                    count+=1
        self.assertEqual(count,8)
        # The separate last-slot alias has the real array's physical address.
        self.assertEqual(0x004ACE18 + 2*0x8E,0x004ACF34)

if __name__=='__main__':unittest.main()

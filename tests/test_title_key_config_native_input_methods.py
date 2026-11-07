"""The exact key-config boss shares the independently exact InputView method."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleKeyConfigNativeInputMethods(unittest.TestCase):
    def test_shared_eight_byte_input_window_and_original_callee(self):
        header=(ROOT/'src/TitleInputRuntimeView.hpp').read_text()
        helper=(ROOT/'src/InputPressedScrolling.cpp').read_text()
        key=(ROOT/'src/TitleScreenKeyConfig.cpp').read_text()
        self.assertIn('struct InputView',header)
        self.assertIn('struct TitleKeyInputView : InputView {};',header)
        self.assertIn('sizeof(InputView) == 0x08',header)
        self.assertIn('sizeof(TitleKeyInputView) == sizeof(InputView)',header)
        for field,offset in (('held00','0'),('previous02','0x02'),
                             ('repeatOutput04','0x04'),('frontState06','0x06'),
                             ('pressed06','0x06')):
            self.assertIn('offsetof(InputView, '+field+') == '+offset,header)
        self.assertIn('#include "TitleInputRuntimeView.hpp"',helper)
        self.assertIn('#include "TitleInputRuntimeView.hpp"',key)
        self.assertNotIn('struct TitleKeyInputView\n{',key)
        self.assertIn('g_TitleInput.IsPressedScrolling(0x40)',key)
        self.assertIn('g_TitleInput.IsPressedScrolling(0x80)',key)
        self.assertIn('g_TitleInput.pressed06',key)

    def test_original_bound_method_has_genuine_callee_identity(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:u=tomllib.load(stream)['units']
        callee=u['input-is-pressed-scrolling'];key=u['title-screen-update-key-config']
        self.assertEqual((callee['target_address'],callee['size']),(0x00423158,42))
        self.assertEqual((key['size'],len(key['relocations'])),(2803,118))
        rows={r['offset']:r for r in key['relocations']}
        for offset in (0x470,0x6F4):
            with self.subTest(offset=offset):
                row=rows[offset]
                self.assertEqual((row['symbol'],row['target'],row['type']),
                                 (callee['symbol'],callee['target_address'],'REL32'))

if __name__=='__main__':unittest.main()

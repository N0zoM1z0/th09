"""Exact Options update retains native input layout and inlined scrolling semantics."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]

class TitleOptionsFullNativeInlineInput(unittest.TestCase):
    def test_native_storage_without_fake_eight_byte_object(self):
        options = (ROOT / 'src/TitleScreenOptions.cpp').read_text()
        replay = (ROOT / 'src/ReplayInputState.hpp').read_text()
        self.assertIn('struct InputView : ReplayInputState', options)
        self.assertIn('sizeof(InputView) == sizeof(ReplayInputState)', options)
        self.assertIn('sizeof(InputView) == 0x8E', options)
        self.assertNotIn('struct InputView {', options)
        self.assertNotIn('OptionsInputSizeIs08', options)
        self.assertIn('u16 pressed = word06 & mask;', options)
        self.assertIn('pressed = repeatOutput & mask', options)
        self.assertIn('sizeof(ReplayInputState) == 0x8E', replay)
        self.assertEqual(options.count('reinterpret_cast<InputView *>(&g_ReplayInputStates[2])'), 2)

    def test_original_exact_callee_identity_kept_not_fake_local_method(self):
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        caller = units['title-screen-options-update']
        leaf = units['input-is-pressed-scrolling']
        self.assertEqual((caller['size'], len(caller['relocations'])), (2045,135))
        self.assertEqual((leaf['size'],leaf['target_address']), (42,0x00423158))
        by_offset = {r['offset']:r for r in caller['relocations']}
        for offset in (0x2E2, 0x53C):
            with self.subTest(offset=offset):
                field = by_offset[offset]
                self.assertEqual((field['symbol'],field['type'],field['target']),
                                 (leaf['symbol'],'REL32',leaf['target_address']))
        self.assertEqual(units['title-screen-play-menu-sound']['size'], 34)

if __name__ == '__main__':
    unittest.main()

"""The exact Supervisor AddedCallback calls native TextHelper::CreateTextBuffer."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SupervisorAddedNativeTextBuffer(unittest.TestCase):
    def test_no_free_fake_text_buffer_provider(self):
        s=(ROOT/'src/SupervisorAddedCallback.cpp').read_text()
        h=(ROOT/'src/TextHelper.hpp').read_text()
        self.assertIn('#include "TextHelper.hpp"',s)
        self.assertIn('    TextHelper::CreateTextBuffer();',s)
        self.assertNotIn('SupervisorAddedCreateTextBuffer',s)
        self.assertIn('static void CreateTextBuffer();',h)

    def test_original_0x4362c0_callee_and_caller_are_bound(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        leaf=u['text-helper-create-text-buffer'];boss=u['supervisor-added-callback']
        self.assertEqual((leaf['target_address'],leaf['size']),(0x004362C0,26))
        self.assertEqual((boss['target_address'],boss['size'],len(boss['relocations'])),
                         (0x00432090,238,27))
        row=next(x for x in boss['relocations'] if x['offset']==0xA7)
        self.assertEqual((row['symbol'],row['type'],row['target']),
                         (leaf['symbol'],'REL32',leaf['target_address']))

if __name__=='__main__':unittest.main()

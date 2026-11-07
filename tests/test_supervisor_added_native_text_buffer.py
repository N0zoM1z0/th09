"""Exact Supervisor AddedCallback directly invokes the native TextHelper buffer leaf."""
import tomllib
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]

class NativeSupervisorTextBufferCall(unittest.TestCase):
    def test_true_text_helper_method(self):
        source=(ROOT/'src/SupervisorAddedCallback.cpp').read_text()
        helper=(ROOT/'src/TextHelper.hpp').read_text()
        self.assertIn('#include "TextHelper.hpp"',source)
        self.assertNotIn('extern int SupervisorAddedCreateTextBuffer();',source)
        self.assertIn('TextHelper::CreateTextBuffer();',source)
        self.assertIn('static void CreateTextBuffer();',helper)

    def test_exact_callee_and_caller_original_referents(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        parent=u['supervisor-added-callback'];leaf=u['text-helper-create-text-buffer']
        self.assertEqual((parent['size'],len(parent['relocations'])),(238,27))
        self.assertEqual((leaf['size'],leaf['target_address']),(26,0x004362C0))
        row=next(r for r in parent['relocations'] if r['offset']==0xA7)
        self.assertEqual((row['type'],row['symbol'],row['target']),
                         ('REL32',leaf['symbol'],leaf['target_address']))

if __name__=='__main__':unittest.main()

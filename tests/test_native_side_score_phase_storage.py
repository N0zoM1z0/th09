"""Native 0xA0 game side score/phase storage shared by ASCII and gameplay setup.

The 1230-byte exact GameManager OnUpdate and the ReplayInput 12 exact
functions are independently recompiled and PE/COFF-compared as protection.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeGameSideScoreStorage(unittest.TestCase):
    def test_one_score_phase_owner_used_by_ascii_and_setup(self):
        shared=((ROOT/'src/GameManagerRuntimeView.hpp').read_text() +
                (ROOT/'src/GameManagerSideState.hpp').read_text())
        setup=(ROOT/'src/GameManagerSetup.cpp').read_text()
        header=(ROOT/'src/GameManagerSetupView.hpp').read_text()
        self.assertIn('struct AsciiGameSideRuntimeView',shared)
        self.assertIn('typedef AsciiGameSideRuntimeView SetupStateBuffer;',shared)
        self.assertIn('SetupStateBufferSizeIsA0',setup)
        self.assertNotIn('struct SetupStateBuffer\n{',setup)
        self.assertNotIn('struct SetupStateBuffer;',header)
        self.assertIn('AsciiGameSideNativeScoreSizeIsA0',shared)
        self.assertIn('sizeof(AsciiGameSideRuntimeView) == 0xA0',shared)
        self.assertIn('offsetof(AsciiGameSideRuntimeView, phase10) == 0x10',shared)
        self.assertIn('offsetof(AsciiGameSideRuntimeView, value14) == 0x14',shared)
        for token in ('float value00;','unsigned int value04;',
                      'unsigned int value08;', 'unsigned int value0C;',
                      'int phase10;', 'unsigned char value14;'):
            self.assertIn(token,shared)
        self.assertIn('offsetof(AsciiGameManagerSideView, state) == 0x1C',shared)
        self.assertIn('SetupStateBuffer *state =',setup)

    def test_original_game_and_replay_exact_manifest_owners_remain_pinned(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        cases={
            'game-manager-on-update':(1230,87),
            'game-manager-cleanup-gameplay-state':(425,43),
            'game-manager-added-callback':(187,13),
            'supervisor-setup-loading-vms-and-capture':(214,17),
            'replay-manager-begin-recording-stage':(1323,82),
        }
        for name,(size,fields) in cases.items():
            with self.subTest(name=name):
                u=units[name]
                self.assertEqual(u['size'],size)
                self.assertEqual(len(u['relocations']),fields)

if __name__=='__main__':
    unittest.main()

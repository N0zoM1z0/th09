"""Native GameManager replay selectors and pause state in original exact setup.

Source owner and ABI guards are complemented by focused TH09 1.50a original
PE/COFF checks of the 1230-byte OnUpdate boss and its separate leaf owners.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class GameManagerNativeReplayInSetup(unittest.TestCase):
    def test_setup_big_owner_consumes_real_replay_state_fields(self):
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        header=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        self.assertIn('#include "GameManagerRuntimeView.hpp"',source)
        self.assertIn('SetupGameManagerNativeReplayOffsets',source)
        self.assertIn('offsetof(GameManagerSetupLayout, replayPauseRecorded)',source)
        self.assertIn('offsetof(AsciiGameManagerView, replayPauseRecorded130)',source)
        self.assertIn('offsetof(AsciiGameManagerSideView, replaySelector28)',source)
        for expression in ('g_GameManager.gameMode',
                           'g_GameManager.sides[0].replaySelector28',
                           'g_GameManager.sides[1].replaySelector28',
                           'reinterpret_cast<AsciiGameManagerView *>(gameManager)->replayPauseRecorded130'):
            self.assertIn(expression,source)
        for field in ('replayMode114','replayPauseRecorded130','replaySelector28'):
            self.assertIn(field,header)
        self.assertIn('int GameManagerSetupLayout::OnUpdate(',source)

    def test_original_setup_large_and_leaf_referents_are_frozen(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        expected={
            'game-manager-on-update':(1230,87),
            'game-manager-cleanup-gameplay-state':(425,43),
            'game-manager-deleted-callback':(422,41),
            'game-manager-added-callback':(187,13),
            'supervisor-setup-loading-vms-and-capture':(214,17),
        }
        for unit,(size,relocs) in expected.items():
            with self.subTest(unit=unit):
                entry=units[unit]
                self.assertEqual(entry['size'],size)
                self.assertEqual(len(entry['relocations']),relocs)
                self.assertEqual(entry['source'],'src/GameManagerSetup.cpp')


if __name__=='__main__':
    unittest.main()

"""One native 0x38-byte game-side owner across setup, Replay and Enemy.

The target-dependent original code bytes are independently recompiled with
pinned VC7.1/PE comparisons, not treated as proven by these source assertions.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeGameSideAcrossOwners(unittest.TestCase):
    def test_one_game_side_type_replaces_duplicate_setup_structure(self):
        shared=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        setup=(ROOT/'src/GameManagerSetupView.hpp').read_text()
        code=(ROOT/'src/GameManagerSetup.cpp').read_text()
        self.assertIn('typedef AsciiGameManagerSideView SetupSideState;',setup)
        self.assertNotIn('struct SetupSideState\n{',setup)
        self.assertIn('#include "GameManagerRuntimeView.hpp"',setup)
        self.assertIn('AsciiGameManagerNativeSetupSideOffsets',shared)
        self.assertIn('AsciiGameManagerSideView sides[2];',shared)
        self.assertIn('AsciiGameManagerSideViewSizeIs38',shared)
        self.assertIn('SetupSideStateSizeCheck',setup)
        for name,offset in (
            ('subsystem0','0x00'),('subsystem1','0x04'),('subsystem2','0x08'),
            ('subsystem3','0x0C'),('subsystem4','0x10'),('subsystem5','0x14'),
            ('subsystem6','0x18'),('state','0x1C'),('value20','0x20'),
            ('value24','0x24'),('selector','0x28'),('flags','0x34'),
        ):
            with self.subTest(name=name):
                self.assertIn('offsetof(AsciiGameManagerSideView, '+name+') == '+offset,
                              shared)
        self.assertIn('SetupSideState *side = &gameManager->sides[0];',code)
        self.assertIn('globalManager.sides[0].state',code)
        self.assertIn('globalManager.sides[1].subsystem4',code)
        self.assertIn('g_GameManager.sides[1].replaySelector28',code)

    def test_original_large_game_manager_and_leaf_owners_pinned(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        pinned={
            'game-manager-on-update':(1230,87),
            'game-manager-cleanup-gameplay-state':(425,43),
            'game-manager-deleted-callback':(422,41),
            'supervisor-setup-loading-vms-and-capture':(214,17),
            'replay-manager-begin-recording-stage':(1323,82),
        }
        for name,(size,count) in pinned.items():
            with self.subTest(name=name):
                unit=units[name]
                self.assertEqual((unit['size'],len(unit['relocations'])),(size,count))


if __name__ == '__main__':
    unittest.main()

"""TH09 native GameManager setup: two playable sides, one shared effect slot.

Cold exact and nonexact-original boss comparisons are independent evidence;
these tests guard the C++ layout and which loops may visit the third slot.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class GameplaySetupNativeSidesTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.layout = (ROOT / 'src/GameManagerSetupView.hpp').read_text()
        cls.native = (ROOT / 'src/GameplaySetupNativeSides.hpp').read_text()
        cls.source = (ROOT / 'src/GameManagerSetup.cpp').read_text()
        cls.units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']

    def test_full_three_slot_storage_and_real_two_side_alias(self):
        self.assertIn('SideRecordCount = sizeof(((AsciiGameManagerView *)0)->sides)',self.layout)
        self.assertIn('sizeof(((AsciiGameManagerView *)0)->sides[0]) + 1',self.layout)
        self.assertIn('SetupSideState sides[SideRecordCount];',self.layout)
        self.assertIn('typedef AsciiGameManagerSideView SetupSideState;',self.layout)
        h=self.native
        for assertion in ('PlayableCount = sizeof(((AsciiGameManagerView *)0)->sides)',
                          'PhysicalCount = sizeof(((GameManagerSetupLayout *)0)->sides)',
                          'SharedEffectIndex = PlayableCount',
                          'InputFocusCount = sizeof(((GameConfiguration *)0)->sideFocusMode)',
                          'PlayableCount == 2', 'PhysicalCount == 3',
                          'SharedEffectIndex + 1 ==',
                          'offsetof(GameManagerSetupLayout, sides) ==',
                          'sizeof(SetupSideState) == sizeof(AsciiGameManagerSideView)',
                          '0x70', '0x7C', 'GameplaySetupNativeSidesLayout'):
            with self.subTest(assertion=assertion):
                self.assertIn(assertion,h)
        self.assertEqual(2 * 0x38, 0x70)
        self.assertEqual(2 * 0x38 + 0x0c, 0x7c)
        self.assertEqual(3 * 0x38, 0xa8)

    def test_source_distinguishes_playable_side_loops_and_shared_effect(self):
        source=self.source
        self.assertIn('#include "GameplaySetupNativeSides.hpp"',source)
        self.assertEqual(source.count('sideIndex < GameplaySetupNativeSides::PlayableCount'), 4)
        self.assertIn('int sideCount = GameplaySetupNativeSides::PlayableCount;',source)
        self.assertGreaterEqual(source.count('sides[GameplaySetupNativeSides::SharedEffectIndex].effectManager0C'), 5)
        self.assertNotIn('globalManager.sides[2].effectManager0C',source)
        self.assertNotIn('gameManager->sides[2].effectManager0C',source)
        self.assertNotIn('sideIndex < 3;',source)
        self.assertIn('EffectManager::Create(GameplaySetupNativeSides::SharedEffectIndex,',source)

    def test_exact_cleanup_and_nonexact_setup_must_stay_separate(self):
        for unit in ('game-manager-cleanup-gameplay-state',
                     'game-manager-deleted-callback',
                     'game-manager-on-update',
                     'supervisor-setup-loading-vms-and-capture'):
            with self.subTest(unit=unit):
                self.assertEqual(self.units[unit]['source'],'src/GameManagerSetup.cpp')
                self.assertGreater(self.units[unit]['size'],0)
        self.assertNotIn('gameplay-setup-thread',self.units)


if __name__ == '__main__':
    unittest.main()

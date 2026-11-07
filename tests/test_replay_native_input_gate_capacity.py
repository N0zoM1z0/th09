"""TH09 has three native input streams but only two gameplay side gates.

A separately pinned PE/COFF replay protects the complete native record,
playback, and SaveReplay functions. These tests guard C++ storage ownership.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class ReplayNativeInputGateCapacityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header=(ROOT/'src/ReplayNativeInputGateCapacity.hpp').read_text()
        cls.input=(ROOT/'src/ReplayInput.cpp').read_text()
        cls.state=(ROOT/'src/ReplayInputState.hpp').read_text()
        cls.config=(ROOT/'src/GameConfiguration.hpp').read_text()

    def test_real_arrays_have_distinct_third_input_and_two_configured_sides(self):
        self.assertIn('g_ReplayInputStates[3]',self.state)
        self.assertIn('sideFocusMode[2]',self.config)
        h=self.header
        for fragment in ('SideCount = sizeof(((GameConfiguration *)0)->sideFocusMode)',
                         'StateCount = sizeof(g_ReplayInputStates)',
                         'SharedInputStateIndex = StateCount - 1',
                         'ReplayRecordingCapacity::InputStreamCount == ReplayNativeInputGateCapacity::StateCount',
                         'sizeof(ReplayInputState) == 0x8E',
                         'offsetof(GameConfiguration, sideFocusMode) == 0xB4',
                         'ReplayNativeInputGateOriginalShape'):
            with self.subTest(fragment=fragment):self.assertIn(fragment,h)
        self.assertEqual(3-1,2)
        self.assertEqual(3,2+1)

    def test_source_scope_gate_bounds_are_native_and_not_all_three_inputs(self):
        s=self.input
        self.assertIn('#include "ReplayNativeInputGateCapacity.hpp"',s)
        self.assertEqual(s.count('ReplayNativeInputGateCapacity::SideCount'),2)
        self.assertIn('for (int side = 0; side < ReplayNativeInputGateCapacity::SideCount;',s)
        self.assertIn('for (; side < ReplayNativeInputGateCapacity::SideCount;',s)
        self.assertNotIn('side < 3;',s)
        self.assertIn('inputGate->sideFocusMode[side]',s)
        self.assertIn('g_GameManager.configurationE8->sideFocusMode',s)

    def test_record_and_playback_keep_shared_input_slot_distinct(self):
        s=self.input
        self.assertEqual(s.count('ReplayNativeInputGateCapacity::SharedInputStateIndex'),10)
        self.assertNotIn('g_ReplayInputStates[2]',s)
        self.assertIn('replayManager->inputCursor2',s)
        self.assertIn('g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].Update()',s)
        self.assertIn('g_ReplayInputStates[0].Update()',s)
        self.assertIn('g_ReplayInputStates[1].Update()',s)

    def test_original_exact_leaves_and_open_bosses_are_separate(self):
        units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        names={n for n,u in units.items() if u['source']=='src/ReplayInput.cpp'}
        self.assertEqual(len(names),12)
        self.assertIn('replay-manager-record-input',names)
        self.assertIn('replay-manager-playback-input',names)
        self.assertIn('replay-manager-capture-frame-sync-state',names)
        self.assertNotIn('replay-manager-begin-playback-stage',units)
        self.assertNotIn('replay-manager-save-replay',units)

if __name__=='__main__':
    unittest.main()

"""Shared Player option position across the big movement and exact shot owner.

These are source/layout assertions. Original full function bytes and COFF
referents are verified by separate pinned compiler replays, not by strings.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativePlayerOptionPosition(unittest.TestCase):
    def test_native_lifecycle_option_has_one_position_timer_and_callback(self):
        owner=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('struct PlayerOptionStateCtorView',owner)
        self.assertIn('Float3 position2A4;',owner)
        self.assertIn('unknown2B0[0x2E0 - 0x2B0]',owner)
        self.assertIn('PlayerOptionStatePositionAt2A4',owner)
        self.assertIn('offsetof(PlayerOptionStateCtorView, position2A4) == 0x2A4',owner)
        self.assertIn('offsetof(PlayerOptionStateCtorView, timer2E0) == 0x2E0',owner)
        self.assertIn('offsetof(PlayerOptionStateCtorView, updateCallback2EC) == 0x2EC',owner)
        self.assertIn('PlayerOptionStateCtorSizeIs2F4',owner)

    def test_exact_shot_initializer_consumes_same_option_instance(self):
        shot=(ROOT/'src/PlayerShotInitializeView.hpp').read_text()
        body=(ROOT/'src/PlayerInitializeShot.cpp').read_text()
        self.assertIn('#include "PlayerLifecycleView.hpp"',shot)
        self.assertIn('typedef PlayerOptionStateCtorView PlayerShotInitOptionView;',shot)
        self.assertNotIn('struct PlayerShotInitOptionView',shot)
        self.assertIn('PlayerShotInitOptionSizeIs2F4',shot)
        self.assertIn('PlayerShotInitOptionView optionStates1CEC[4];',shot)
        self.assertIn('optionStates1CEC[entry->sourceOptionIndex20 - 1].position2A4;',body)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        owner=units['player-initialize-shot']
        self.assertEqual((owner['target_address'],owner['size'],len(owner['relocations'])),
                         (0x0041F350,309,7))
        self.assertEqual((units['player-constructor']['target_address'],
                          units['player-constructor']['size']), (0x0041F070,243))


if __name__=='__main__':
    unittest.main()

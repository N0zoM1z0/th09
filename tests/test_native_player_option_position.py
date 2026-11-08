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

    def test_option_vm_is_real_anm_with_original_2a4_byte_stride(self):
        owner=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        anm=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('#include "AnmVmLifecycle.hpp"',owner)
        self.assertIn('AnmVm vm00;',owner)
        self.assertNotIn('unsigned char vm00[0x2A4];',owner)
        self.assertIn('PlayerOptionNativeAnmVmAt00',owner)
        for field,offset in (('vm00','0x00'),('position2A4','0x2A4')):
            with self.subTest(field=field):
                self.assertIn('offsetof(PlayerOptionStateCtorView, '+field+') == '+offset,owner)
        for name,offset in (('pos','0x208'),('loadedSprite','0x224'),('pos2','0x288')):
            with self.subTest(vm_field=name):
                self.assertIn('offsetof(AnmVm, '+name+') == '+offset,owner)
        self.assertIn('AnmVmLifecycleSizeIs2A4',anm)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        self.assertEqual((u['anm-vm-constructor']['target_address'],
                          u['anm-vm-constructor']['size']),(0x403A10,45))
        self.assertEqual((u['zun-timer-constructor']['target_address'],
                          u['zun-timer-constructor']['size']),(0x403390,12))

    def test_shot_has_32_native_constructed_vectors_then_hitbox(self):
        owner=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        init=(ROOT/'src/PlayerShotInitializeView.hpp').read_text()
        self.assertIn('PlayerConstructedVector3View trailVectors2B0[32];',owner)
        self.assertIn('PlayerPositionView hitbox430;',owner)
        self.assertNotIn('unknown2B0[0x43C - 0x2B0]',owner)
        self.assertIn('PlayerShotNativeConstructedVectors',owner)
        self.assertIn('sizeof(((PlayerShotCtorView *)0)->trailVectors2B0) == 0x180',owner)
        self.assertIn('sizeof(((PlayerShotCtorView *)0)->trailVectors2B0[0]) == 0x0C',owner)
        self.assertIn('offsetof(PlayerShotCtorView, hitbox430) == 0x430',owner)
        self.assertIn('offsetof(PlayerShotCtorView, velocity43C) == 0x43C',owner)
        self.assertIn('PlayerShotInitFloat3 hitbox430;',init)
        self.assertIn('PlayerShotInitFloat3 velocity43C;',init)

    def test_exact_shot_initializer_consumes_same_option_instance(self):
        shot=(ROOT/'src/PlayerShotInitializeView.hpp').read_text()
        body=(ROOT/'src/PlayerInitializeShot.cpp').read_text()
        self.assertIn('#include "PlayerLifecycleView.hpp"',shot)
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertNotIn('struct PlayerShotInitPlayerView',shot)
        self.assertIn('PlayerOptionStateCtorView optionStates[4];',native)
        self.assertIn('PlayerLifecycleOptionsAt1CEC',native)
        self.assertIn('offsetof(PlayerLifecycleView, optionStates) == 0x1CEC',native)
        self.assertIn('PlayerOptionStateCtorSizeIs2F4',native)
        self.assertIn('nativePlayer.optionStates[nativeEntry.sourceOptionIndex20 - 1].position2A4;',body)
        self.assertIn('nativeShot.position2A4 = nativePlayer.position1B88;',body)
        self.assertIn('nativePlayer.header24.anmFile98->SetAndExecuteScriptIdx(',body)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        owner=units['player-initialize-shot']
        self.assertEqual((owner['target_address'],owner['size'],len(owner['relocations'])),
                         (0x0041F350,309,7))
        self.assertEqual((units['player-constructor']['target_address'],
                          units['player-constructor']['size']), (0x0041F070,243))


if __name__=='__main__':
    unittest.main()

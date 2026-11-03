"""Source-side contract for Effect ring/fan geometry and exact Effect manager.

This keeps polymorphic float/int reinterpretations explicit and ensures that
the Enemy boss and large Effect callbacks consume one original-byte-backed
0xD8 runtime object. Whole-owner equivalence is checked separately by the
focused ECL/Enemy/Effect VC7.1 replays.
"""

from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class SharedEffectNativeGeometry(unittest.TestCase):
    def test_one_effect_storage_preserves_integer_and_float_interpretations(self):
        header=(ROOT/'src/EffectManager.hpp').read_text()
        self.assertIn('struct Effect',header)
        self.assertIn('EffectSizeIsD8',header)
        self.assertIn('Float3 origin48;',header)
        self.assertIn('Float3 axis54;',header)
        self.assertIn('Float3 axis60;',header)
        for pair in ('int value7C; float radius7C;', 'int value80; float angle80;',
                     'int value88; float thickness88;', 'int value8C; int segments8C;',
                     'int value94; float ellipseY94;',
                     'int value98; float rotation98; float playerRewardDelay98;',
                     'float value9C; float frequency9C;'):
            self.assertIn('union { '+pair+' };',header)
        for field,offset in (
            ('origin48',0x48),('axis54',0x54),('axis60',0x60),
            ('radius7C',0x7C),('angle80',0x80),('segments8C',0x8C),
            ('value7C',0x7C),('fixedSlotIndex',0x90),('value9C',0x9C)):
            with self.subTest(field=field):
                self.assertIn('offsetof(Effect, '+field+') == 0x'+format(offset,'X'),header)

    def test_ring_view_is_actual_effect_not_duplicate_prefix(self):
        ring=(ROOT/'src/EffectRingRuntimeView.hpp').read_text()
        self.assertIn('typedef Effect EffectRingRuntimeView;',ring)
        self.assertNotIn('struct EffectRingRuntimeView',ring)
        self.assertIn('offsetof(EffectRingRuntimeView, unknown0A0) == 0xA0',ring)
        self.assertIn('EffectRingRadiusAt7C',ring)
        self.assertIn('EffectRingSegmentsAt8C',ring)
        self.assertIn('EffectRingFrequencyAt9C',ring)

    def test_original_exact_effect_manager_and_enemy_callee_still_manifested(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for unit,addr,length in (
            ('effect-manager-initialize-effect',0x0040CA60,407),
            ('effect-manager-update',0x0040CDD0,475),
            ('enemy-attached-effect-update',0x0040F4C0,151)):
            with self.subTest(unit=unit):
                self.assertEqual(units[unit]['target_address'],addr)
                self.assertEqual(units[unit]['size'],length)
        source=(ROOT/'src/EnemyManagerCore.inl').read_text()
        self.assertIn('EffectRingRuntimeView *effect =',source)
        self.assertIn('effect->origin48 =', source)
        self.assertIn('reinterpret_cast<const Float3 &>(nativeEnemy->position2D74);', source)


if __name__=='__main__':
    unittest.main()

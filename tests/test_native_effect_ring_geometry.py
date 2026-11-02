"""Shared Effect geometry is an original-bound object, not a duplicate view."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeEffectRingGeometry(unittest.TestCase):
    def test_one_effect_definition_supports_float_and_integer_consumers(self):
        effect = (ROOT / 'src/EffectManager.hpp').read_text()
        ring = (ROOT / 'src/EffectRingRuntimeView.hpp').read_text()
        self.assertIn('typedef Effect EffectRingRuntimeView;', ring)
        self.assertNotIn('struct EffectRingRuntimeView', ring)
        self.assertIn('EffectSizeIsD8', effect)
        self.assertIn('EffectRingPrefixSizeIsA0', ring)
        for field in ('origin48', 'axis54', 'axis60', 'radius7C', 'angle80',
                      'thickness88', 'segments8C', 'ellipseY94', 'rotation98',
                      'frequency9C'):
            with self.subTest(field=field):
                self.assertIn(field, effect)
                self.assertNotIn('float '+field+';',ring)
        for left,right in [('value7C','radius7C'),('value80','angle80'),
                           ('value88','thickness88'),('value94','ellipseY94'),
                           ('value98','rotation98'),('value9C','frequency9C')]:
            with self.subTest(alias=left):
                self.assertIn(left, effect)
                self.assertIn(right, effect)
        self.assertIn('offsetof(Effect, origin48) == 0x48', effect)
        self.assertIn('offsetof(Effect, radius7C) == 0x7C', effect)
        self.assertIn('offsetof(Effect, angle80) == 0x80', effect)

    def test_exact_effect_manager_and_enemy_callee_share_physical_owners(self):
        manager = (ROOT / 'src/EffectManager.cpp').read_text()
        enemy = (ROOT / 'src/EnemyManagerCore.inl').read_text()
        self.assertIn('#include "EffectRingRuntimeView.hpp"', enemy)
        self.assertIn('EffectRingRuntimeView *effect =', enemy)
        self.assertIn('effect->origin48 = enemy->position2D74;', enemy)
        self.assertIn('effect->radius7C += 0.3f;', enemy)
        self.assertIn('effect->angle80', enemy)
        self.assertIn('effect->value7C = vm->value288;', manager)
        with (ROOT / 'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        self.assertEqual(units['enemy-attached-effect-update']['target_address'],0x0040F4C0)
        self.assertEqual(units['effect-manager-update']['target_address'],0x0040CDD0)


if __name__ == '__main__':
    unittest.main()

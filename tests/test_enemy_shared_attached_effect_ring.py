"""Guard the original shared Effect runtime state consumed by the Enemy boss.

This static source contract is backed separately by the focused 151-byte
exact COFF owner and the Enemy full-owner/linked-original diagnostics.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class EnemyAttachedEffectRingStorage(unittest.TestCase):
    def test_one_observed_origin_radius_and_angle_view(self):
        header=(ROOT/'src/EffectRingRuntimeView.hpp').read_text()
        enemy=(ROOT/'src/EnemyManagerCore.inl').read_text()
        native=(ROOT/'src/EffectManager.hpp').read_text()
        self.assertIn('typedef Effect EffectRingRuntimeView;',header)
        self.assertNotIn('struct EffectRingRuntimeView\n{',header)
        for field in ('Float3 origin48;','float radius7C;','float angle80;'):
            self.assertIn(field,native)
        for field,offset in (('origin48','0x48'),('radius7C','0x7C'),('angle80','0x80')):
            self.assertIn('offsetof(EffectRingRuntimeView, '+field+') == '+offset,enemy)
        self.assertNotIn('struct EnemyAttachedEffectUpdateView',enemy)
        self.assertIn('EffectRingRuntimeView *effect =',enemy)
        self.assertIn('effect->origin48 = enemy->position2D74;',enemy)
        self.assertIn('effect->radius7C += 0.3f;',enemy)
        self.assertIn('AddNormalizeAngle(effect->angle80, 0.031415928f);',enemy)

    def test_callee_is_original_exact_and_not_fake_helper(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']['enemy-attached-effect-update']
        self.assertEqual((u['source'],u['object']),('src/EclManager.cpp','build/matching/EclManager.obj'))
        self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),(0x0040F4C0,151,2))
        source=(ROOT/'src/EnemyManagerCore.inl').read_text()
        self.assertIn('static void EnemyCoreUpdateAttachedEffects(EnemyCoreView *enemy)',source)
        self.assertIn('enemy->attachedEffectState53B4.effects00[i]',source)
        self.assertIn('enemy->attachedEffectState53B4.distance64',source)
        self.assertNotIn('EnemyAttachedEffectUpdateView *',source)


if __name__=='__main__':
    unittest.main()

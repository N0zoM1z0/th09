"""The shared native ScreenEffect envelope subrecord and signed timer view.

Static provenance checks accompany a *separate* complete PE/COFF producer
replay. This test does not claim canonical exactness for CalcShakeEnvelope.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class ScreenEffectNativeEnvelopePhaseLayout(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header = (ROOT/'src/ScreenEffect.hpp').read_text()
        cls.source = (ROOT/'src/ScreenEffect.cpp').read_text()

    def test_named_phases_alias_the_same_three_native_dwords(self):
        header = self.header
        self.assertIn('ScreenEffectEnvelopePhaseNativeLayout', header)
        for name, offset in (('fadeInFrames','0x1C'),('holdFrames','0x20'),
                             ('fadeOutFrames','0x24')):
            with self.subTest(field=name):
                self.assertIn('int '+name+';', header)
                self.assertIn('offsetof(ScreenEffect, shakeEnvelope.'+name+') == '+offset,header)
        for name, offset in (('rawParameter0','0x1C'),('rawParameter1','0x20'),
                             ('rawParameter2','0x24')):
            with self.subTest(field=name):
                self.assertIn('int '+name+';',header)
                self.assertIn('offsetof(ScreenEffect, '+name+') == '+offset,header)
        self.assertIn('sizeof(((ScreenEffect *)0)->shakeEnvelope) == 0x0C', header)
        self.assertIn('ScreenEffectSizeIs38',header)
        self.assertIn('ScreenEffectTimerAt2C',header)

    def test_envelope_borrows_real_timer_and_preserves_independent_fade_reads(self):
        owner = self.source.split('int ScreenEffect::CalcShakeEnvelope(',1)[1].split(
            'int ScreenEffect::InitializeTimer(',1)[0]
        self.assertIn('ZunTimer &clock = screenEffect->timer;',owner)
        self.assertIn('clock++;',owner)
        self.assertIn('int endFrame = screenEffect->shakeEnvelope.fadeInFrames;',owner)
        self.assertIn('clock < endFrame + screenEffect->shakeEnvelope.holdFrames',owner)
        fade='const volatile int &fade = screenEffect->shakeEnvelope.fadeOutFrames;'
        hold='const volatile int &hold = screenEffect->shakeEnvelope.holdFrames;'
        self.assertLess(owner.index(fade),owner.index(hold))
        self.assertIn('int update = hold + fade;',owner)
        self.assertIn('endFrame += update;',owner)
        self.assertIn('(unsigned int)screenEffect->shakeEnvelope.fadeOutFrames',owner)
        self.assertIn('(float)clock',owner)
        # Other ScreenEffect modes still use the unmodified shared raw words.
        self.assertIn('screenEffect->rawParameter0',self.source)
        self.assertIn('screenEffect->rawParameter1',self.source)
        self.assertIn('screenEffect->rawParameter2',self.source)

    def test_target_bound_original_remains_nonexact_with_affected_exact_owners(self):
        units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        s=units['screen-effect-calc-shake']
        self.assertEqual((s['size'],s['target_address']),(316,0x00422660))
        self.assertNotIn('screen-effect-calc-shake-envelope',units)
        owners=[n for n,u in units.items() if u['source']=='src/ScreenEffect.cpp']
        self.assertEqual(len(owners),18)

if __name__=='__main__':
    unittest.main()

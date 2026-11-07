"""StartAttack's transition effects use the genuine Background member offsets."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class CardAttackNativeBackgroundTransition(unittest.TestCase):
    def test_uses_real_background_owner(self):
        source=(ROOT/'src/CardAttackStart.cpp').read_text()
        header=(ROOT/'src/Background.hpp').read_text()
        self.assertIn('#include "Background.hpp"',source)
        self.assertNotIn('struct CardAttackStartStateView',source)
        self.assertIn('CardAttackStartNativeSideRecord',source)
        self.assertIn('startSideState04->background00->transitionMode',source)
        self.assertIn('offsetof(Background, transitionMode) == 0x1C',source)
        self.assertIn('offsetof(Background, transitionFrames) == 0x20',source)
        self.assertEqual(source.count('->background00->transitionMode'),2)
        self.assertEqual(source.count('->background00->transitionFrames'),2)
        self.assertIn('int transitionMode;',header)
        self.assertIn('int transitionFrames;',header)

    def test_exact_start_owner_constant(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertEqual(units['card-attack-start']['size'],923)
        self.assertEqual(len(units['card-attack-start']['relocations']),29)

if __name__=='__main__':unittest.main()

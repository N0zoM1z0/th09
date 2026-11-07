"""The independently exact StartAttack uses the real FrontSide auxiliary VMs."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]

class CardAttackStartNativeFrontSide(unittest.TestCase):
    def test_front_side_uses_native_hud_record(self):
        s = (ROOT / 'src/CardAttackStart.cpp').read_text()
        header = (ROOT / 'src/FrontSide.hpp').read_text()
        self.assertIn('#include "FrontSide.hpp"', s)
        self.assertIn('CardAttackStartNativeFrontAux', s)
        self.assertNotIn('struct CardAttackStartFrontAuxView', s)
        self.assertNotIn('struct CardAttackStartFrontSideView', s)
        self.assertIn('FrontSide *frontSide18;', s)
        self.assertIn('offsetof(FrontSide, auxA678) == 0xA678', s)
        self.assertIn('sizeof(FrontSideAuxView) == 0x558', s)
        self.assertIn('startOtherSideState08->frontSide18->auxA678', s)
        self.assertIn('FrontSideAuxView auxA678;', header)
        self.assertIn('AnmVm vms0C[2];', header)

    def test_original_start_attack_owner_unchanged(self):
        with (ROOT / 'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        u = units['card-attack-start']
        self.assertEqual(u['size'],923)
        self.assertEqual(u['target_address'],0x00403E50)
        self.assertEqual(len(u['relocations']),29)

if __name__ == '__main__':
    unittest.main()

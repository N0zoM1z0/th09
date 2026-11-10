"""RunEcl's +0x14 Side owner is the genuine exact CardAttack allocation.

Tests are structural and original-address guards, not replacement byte proof.
The complete RunEcl 15,564-byte/598-field diagnostic remains separate.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class RunEclNativeCardAttackOwner(unittest.TestCase):

    def test_native_owner_has_independently_exact_create_and_update(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        self.assertEqual(u['card-attack-create']['size'],264)
        self.assertEqual(u['card-attack-on-update']['size'],536)
        self.assertEqual(u['card-attack-start']['size'],923)
        self.assertEqual(u['ecl-post-update-movement']['compare_size'],992)
        self.assertEqual(u['ecl-compare-operands']['compare_size'],892)

if __name__=='__main__': unittest.main()

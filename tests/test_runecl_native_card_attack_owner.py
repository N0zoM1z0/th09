"""RunEcl's +0x14 Side owner is the genuine exact CardAttack allocation.

Tests are structural and original-address guards, not replacement byte proof.
The complete RunEcl 15,564-byte/598-field diagnostic remains separate.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class RunEclNativeCardAttackOwner(unittest.TestCase):
    def test_enemy_ecl_reads_native_card_counters(self):
        state=(ROOT/'src/EclRunState.inl').read_text()
        late=(ROOT/'src/EclRunLate.inl').read_text()
        enemy=(ROOT/'src/EnemyManager.hpp').read_text()
        card=(ROOT/'src/CardAttack.hpp').read_text()
        native=(ROOT/'src/GameManagerSideState.hpp').read_text()
        self.assertIn('#include "CardAttack.hpp"',state)
        self.assertNotIn('struct EnemyCoreSideOwnerView',state)
        self.assertIn('typedef CardAttack EnemyCoreSideOwnerView;',state)
        self.assertIn('CardAttack *owner14;',enemy)
        self.assertIn('offsetof(EnemySideStateView, owner14) == 0x14',enemy)
        self.assertIn('CardAttack *cardAttack14;',native)
        self.assertIn('offsetof(AsciiGameManagerSideView, cardAttack14) == 0x14',native)
        for name,off in (('primaryPresentationCounterA4','0xA4'),
                         ('secondaryPresentationCounterA8','0xA8')):
            self.assertIn(f'offsetof(SideOwnerView, {name}) == {off}',late)
            self.assertIn(f'owner14->{name}',late)
            self.assertIn(f'int {name};',card)
        self.assertNotIn('owner14->primaryStateA4',late)
        self.assertNotIn('owner14->alternateStateA8',late)

    def test_native_owner_has_independently_exact_create_and_update(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        self.assertEqual(u['card-attack-create']['size'],264)
        self.assertEqual(u['card-attack-on-update']['size'],536)
        self.assertEqual(u['card-attack-start']['size'],923)
        self.assertEqual(u['ecl-post-update-movement']['compare_size'],992)
        self.assertEqual(u['ecl-compare-operands']['compare_size'],892)

if __name__=='__main__': unittest.main()

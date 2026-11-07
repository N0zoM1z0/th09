"""The exact 923-byte StartAttack method shares CardAttack's real allocation."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class CardAttackStartNativeInheritance(unittest.TestCase):
    def test_start_method_inherits_real_card_storage(self):
        start=(ROOT/'src/CardAttackStartView.hpp').read_text()
        base=(ROOT/'src/CardAttack.hpp').read_text()
        implementation=(ROOT/'src/CardAttackStart.cpp').read_text()
        self.assertIn('struct CardAttackStartView : CardAttack',start)
        self.assertIn('sizeof(CardAttackStartView) == sizeof(CardAttack)',start)
        self.assertIn('sizeof(CardAttackStartView) == 0x1368',start)
        self.assertNotIn('int primaryRow134C;',start)
        self.assertNotIn('AnmVm secondaryVms5F4[5];',start)
        self.assertIn('int primaryRow134C;',base)
        self.assertIn('int secondaryRow1358;',base)
        self.assertIn('CardAttackStartSideView *startSideState04;',base)
        self.assertIn('CardAttackStartSideView *startOtherSideState08;',base)
        self.assertIn('startOtherSideState08->player04->header24.state7C',implementation)
        self.assertIn('startSideState04->enemyManager10',implementation)
        self.assertNotIn('reinterpret_cast<CardAttack *>(this)',implementation)
        self.assertIn('CardAttackStartView::StartAttack(',implementation)

    def test_original_bound_start_owner_and_related_caller_stay_distinct(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        owner=u['card-attack-start']
        self.assertEqual(owner['size'],923)
        self.assertEqual(owner['target_address'],0x00403E50)
        self.assertEqual(owner['symbol'],'?StartAttack@CardAttackStartView@@QAEXHHHPBD@Z')
        self.assertEqual(len(owner['relocations']),29)
        for n in ('card-attack-on-update','card-attack-on-draw','card-attack-create'):
            self.assertEqual(u[n]['source'],'src/CardAttack.cpp')

if __name__=='__main__':
    unittest.main()

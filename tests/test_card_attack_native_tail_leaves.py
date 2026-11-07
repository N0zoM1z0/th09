"""The exact CardAttack begin/end callbacks use one real 0x1368-byte record."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class CardAttackNativeTailLeaves(unittest.TestCase):
    def test_shared_tail_owned_by_real_card_attack(self):
        header=(ROOT/'src/CardAttack.hpp').read_text()
        self.assertIn('CardAttackNativeTailFields',header)
        self.assertNotIn('unknown1340[0x28]',header)
        for field,offset in (
            ('primaryValue1348','0x1348'),('primaryRow134C','0x134C'),
            ('primaryClass1350','0x1350'),('secondaryValue1354','0x1354'),
            ('secondaryRow1358','0x1358'),('secondaryClass135C','0x135C'),
            ('extra1360','0x1360'),('extra1364','0x1364')):
            self.assertIn(f'offsetof(CardAttack, {field}) == {offset}',header)
        for filename,shadow in (('CardAttackBeginCallbacks.cpp','CardAttackBeginTailView'),
                                ('CardAttackEndCallbacks.cpp','CardAttackEndTailView')):
            with self.subTest(file=filename):
                source=(ROOT/'src'/filename).read_text()
                self.assertNotIn(shadow,source)
                self.assertIn('cardAttack->extra1360',source)
                self.assertIn('cardAttack->extra1364',source)

    def test_independent_original_bound_begin_end_owners(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        expected={'card-attack-begin-pl00-type0':75,'card-attack-begin-pl00-type1':75,
                  'card-attack-begin-pl00-type2':78,'card-attack-begin-pl01-type0':75,
                  'card-attack-begin-pl01-type1':75,'card-attack-begin-pl01-type2':78,
                  'card-attack-end-free1360':38,'card-attack-end-free1364':38,
                  'card-attack-start':923,'card-attack-on-update':536,
                  'card-attack-on-draw':334}
        for name,size in expected.items():
            with self.subTest(unit=name):
                self.assertEqual(units[name]['size'],size)
                self.assertTrue(units[name]['source'].startswith('src/CardAttack'))

if __name__=='__main__':
    unittest.main()

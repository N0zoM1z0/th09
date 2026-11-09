"""Noncrediting tests for the two physical Enemy ECL constructor boundaries."""
from __future__ import annotations

import csv
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'enemy_ctor_chain', ROOT / 'scripts/inspect-enemy-ecl-constructors.py'
)
assert spec is not None and spec.loader is not None
ctor = importlib.util.module_from_spec(spec)
spec.loader.exec_module(ctor)


class EnemyEclConstructorChain(unittest.TestCase):
    def test_real_native_array_stays_eight_twelve_byte_vectors_at_100(self):
        header = (ROOT / 'src/EnemyManager.hpp').read_text()
        source = (ROOT / 'src/EnemyManagerLifecycle.cpp').read_text()
        self.assertIn('Float3 constructedPositions100[8];', header)
        self.assertIn('offsetof(EnemyEclTimelineStateView, constructedPositions100) == 0x100', header)
        self.assertIn('sizeof(((EnemyEclTimelineStateView *)0)->constructedPositions100) == 8 * 0x0C', header)
        self.assertIn('sizeof(EnemyEclTimelineStateView) == 0x180', header)
        self.assertIn('EnemyEclTimelineStateView::EnemyEclTimelineStateView()', source)
        self.assertIn('EnemyEclManagerView::EnemyEclManagerView()', source)
        self.assertNotIn('??_H@YGXPAXIHP6EPAX0@Z@Z', source)

    def test_fixed_target_roles_and_failure_on_any_operand_drift(self):
        first, second = ctor.CHAIN
        self.assertEqual((first['base'], first['size']), (0x0040FCA0, 28))
        self.assertEqual((second['base'], second['size']), (0x00412090, 15))
        self.assertEqual(first['fields'], (
            (2, 'DIR32', '??0Float3@@QAE@XZ', 0x004343D0),
            (0x14, 'REL32', '??_H@YGXPAXIHP6EPAX0@Z@Z', 0x00401470),
        ))
        self.assertEqual(second['fields'], (
            (7, 'REL32', '??0EnemyEclTimelineStateView@@QAE@XZ', 0x0040FCA0),
        ))
        for entry in ctor.CHAIN:
            rows = [dict(offset=o, type=k, symbol=s, addend=0)
                    for o, k, s, _ in entry['fields']]
            ctor.validate_fields(rows, entry['fields'])
            for bad in (rows[:-1], rows+[rows[-1]],
                        [dict(rows[0], offset=rows[0]['offset']+1), *rows[1:]],
                        [dict(rows[0], addend=1), *rows[1:]],
                        [dict(rows[0], symbol='??0Wrong@@QAE@XZ'), *rows[1:]]):
                with self.subTest(entry=entry['name'], bad=bad):
                    with self.assertRaises(ValueError):
                        ctor.validate_fields(bad, entry['fields'])

    def test_constructor_origins_stay_unclassified_not_authored(self):
        with (ROOT / 'config/functions.csv').open(newline='') as stream:
            rows = {r['address']: r for r in csv.DictReader(stream)}
        for address in ('0x0040FCA0', '0x00412090'):
            self.assertEqual(rows[address]['status'], 'unclassified')
        self.assertIn('unknown', ctor.audit.__doc__ or 'unknown')
        self.assertIn('exactness_credit', (ROOT/'scripts/inspect-enemy-ecl-constructors.py').read_text())


if __name__ == '__main__':
    unittest.main()

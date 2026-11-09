"""Noncrediting tests for the complete Enemy ECL constructor hierarchy."""
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
        self.assertIn('Th09EclInterpolationSlotView::Th09EclInterpolationSlotView()',source)
        self.assertIn('Th09EclContextView::Th09EclContextView()',source)
        opcode=(ROOT/'src/EclOpcodes.hpp').read_text()
        self.assertIn('Th09EclInterpolationSlotView();',opcode)
        self.assertIn('Th09EclContextView();',opcode)
        self.assertIn('Th09EclInterpolationSlotSizeIs30',opcode)
        self.assertIn('Th09EclContextSizeIs22C',opcode)
        self.assertIn('Th09EclContextInterpolationsAt0A0',opcode)
        self.assertIn('EnemyView::EnemyView()',source)
        self.assertIn('EnemyBulletDescriptorView::EnemyBulletDescriptorView()',source)
        self.assertIn('ZunTimer frameTimer3370;',header)
        self.assertIn('EnemyTrailVertexView trailVertices3E68[194];',header)
        self.assertIn('EnemyViewSizeIs5430',header)

    def test_fixed_target_roles_and_failure_on_any_operand_drift(self):
        first, second, slot, context, descriptor, enemy = ctor.CHAIN
        self.assertEqual((first['base'], first['size']), (0x0040FCA0, 28))
        self.assertEqual((second['base'], second['size']), (0x00412090, 15))
        self.assertEqual(first['fields'], (
            (2, 'DIR32', '??0Float3@@QAE@XZ', 0x004343D0),
            (0x14, 'REL32', '??_H@YGXPAXIHP6EPAX0@Z@Z', 0x00401470),
        ))
        self.assertEqual(second['fields'], (
            (7, 'REL32', '??0EnemyEclTimelineStateView@@QAE@XZ', 0x0040FCA0),
        ))
        self.assertEqual((slot['base'], slot['size']), (0x004120C0, 15))
        self.assertEqual((context['base'], context['size']), (0x004120D0, 47))
        self.assertEqual(slot['fields'], (
            (7, 'REL32', '??0ZunTimer@@QAE@XZ', 0x00403390),
        ))
        self.assertEqual(context['fields'], (
            (7, 'REL32', '??0ZunTimer@@QAE@XZ', 0x00403390),
            (0x12, 'REL32', '??0ZunTimer@@QAE@XZ', 0x00403390),
            (0x17, 'DIR32', '??0Th09EclInterpolationSlotView@@QAE@XZ', 0x004120C0),
            (0x27, 'REL32', '??_H@YGXPAXIHP6EPAX0@Z@Z', 0x00401470),
        ))
        self.assertEqual((descriptor['base'], descriptor['size']), (0x0040D500, 28))
        self.assertEqual(descriptor['fields'], ())
        self.assertEqual((enemy['base'], enemy['size']), (0x00412100, 207))
        self.assertEqual(len(enemy['fields']), 18)
        self.assertEqual(enemy['fields'][8],
                         (0x63, 'REL32', '??0EnemyBulletDescriptorView@@QAE@XZ', 0x0040D500))
        self.assertEqual(enemy['fields'][10],
                         (0x79, 'REL32', '??0EnemyBulletDescriptorView@@QAE@XZ', 0x0040D500))
        self.assertEqual(enemy['fields'][12],
                         (0x89, 'DIR32', '??0EnemyTrailSampleView@@QAE@XZ', 0x004343D0))
        self.assertEqual(enemy['fields'][14],
                         (0x9E, 'DIR32', '??0EnemyTrailVertexView@@QAE@XZ', 0x004343D0))
        for entry in ctor.CHAIN:
            if not entry['fields']:
                continue  # The descriptor alias has zero native relocation fields.
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
        for address in ('0x0040FCA0', '0x00412090', '0x004120C0', '0x004120D0', '0x00412100'):
            self.assertEqual(rows[address]['status'], 'unclassified')
        self.assertEqual(rows['0x0040D500']['status'], 'matching')
        self.assertIn('unknown', ctor.audit.__doc__ or 'unknown')
        self.assertIn('exactness_credit', (ROOT/'scripts/inspect-enemy-ecl-constructors.py').read_text())


if __name__ == '__main__':
    unittest.main()

"""Target-independent guards for the shared TH09 Etama/Effect draw ownership.

Actual retail PE/COFF original byte verification runs in the two canonical
match units and the complete OnDraw diagnostic, not in these pure-input tests.
"""

import csv
import importlib.util
from pathlib import Path
import tomllib
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]


def manifest():
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        return tomllib.load(stream)['units']


def ledger(filename):
    with (ROOT / 'config' / filename).open(newline='') as stream:
        return list(csv.DictReader(stream))


class EtamaSharedEffectOwner(TestCase):
    def test_real_effect_cpp_receiver_and_original_field_layout(self):
        header = (ROOT / 'src/EffectManager.hpp').read_text()
        leaf = (ROOT / 'src/EffectBulletLayerDraw.cpp').read_text()
        boss = (ROOT / 'src/EtamaControllerDraw.cpp').read_text()
        self.assertIn('int DrawBulletLayerEffects();', header)
        self.assertIn('EffectManagerBulletLayerHeadAt1E8', header)
        self.assertIn('offsetof(Effect, nextInDrawGroup) == 0x1E8', header)
        self.assertIn('int EffectManager::DrawBulletLayerEffects()', leaf)
        self.assertIn('EffectManager *manager = this;', leaf)
        self.assertIn('manager->drawSentinel1.nextInDrawGroup', leaf)
        self.assertIn('#include "EffectManager.hpp"', boss)
        self.assertIn('EffectManager *effectManager0C;', boss)
        self.assertIn('EtamaDrawSideEffectManagerAt0C', boss)
        self.assertIn('->effectManager0C->DrawBulletLayerEffects();', boss)
        self.assertNotIn('BulletDrawEffectManagerView', leaf + boss)

    def test_exact_leaf_and_large_nonexact_call_resolve_to_same_original_va(self):
        units = manifest()
        leaf = units['effect-draw-bullet-layer']
        self.assertEqual(leaf['target_address'], 0x0040D110)
        self.assertEqual(leaf['size'], 169)
        self.assertEqual(leaf['symbol'], '?DrawBulletLayerEffects@EffectManager@@QAEHXZ')
        self.assertEqual(leaf['functions'], ['EffectManager::DrawBulletLayerEffects'])
        self.assertEqual(len(leaf['relocations']), 7)
        self.assertEqual(leaf['source'], 'src/EffectBulletLayerDraw.cpp')
        spec = importlib.util.spec_from_file_location(
            'etama_effect_original', ROOT / 'scripts/inspect-etama-draw.py')
        inspect = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(inspect)
        self.assertEqual(inspect.BASE, 0x00413BE0)
        self.assertEqual(inspect.SIZE, 600)
        self.assertEqual(inspect.DESTINATIONS[leaf['symbol']], 0x0040D110)
        self.assertEqual(inspect.expected_kind(leaf['symbol']), 'REL32')
        with self.assertRaisesRegex(ValueError, 'unknown Etama draw relocation'):
            inspect.expected_kind('?DrawBulletLayerEffects@BulletDrawEffectManagerView@@QAEHXZ')

    def test_all_ledgers_identify_one_actual_exact_source_owner(self):
        name = 'EffectManager::DrawBulletLayerEffects'
        addr = '0x0040D110'
        rows = [r for r in ledger('functions.csv') if r['address'] == addr]
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]['proposed_name'], name)
        self.assertEqual(rows[0]['status'], 'matching')
        self.assertEqual(rows[0]['source_file'], 'src/EffectBulletLayerDraw.cpp')
        rows = [r for r in ledger('matches.csv') if r['address'] == addr]
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]['name'], name)
        self.assertEqual(rows[0]['unit'], 'effect-draw-bullet-layer')
        rows = [r for r in ledger('reccmp-functions.csv') if r['address'] == addr]
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]['name'], name)
        self.assertIn(name, (ROOT / 'config/implemented.csv').read_text().splitlines())

    def test_private_label_refresh_never_changes_any_target_address(self):
        units = manifest()
        expected = {
            'bullet-draw-single': {0x17: 0x00413BC8, 0xD8: 0x00413B0B,
                                   0xDC: 0x00413B13, 0xE0: 0x00413B1B,
                                   0xE4: 0x00413B23},
            'etama-controller-added-callback': {
                0x17F: 0x004140A4, 0x186: 0x00414098,
                0x1EB: 0x0041411C, 0x1F2: 0x00414110,
                0x258: 0x00413FCA, 0x25C: 0x00413FDB,
                0x260: 0x00413FFC, 0x2D0: 0x00414036,
                0x2D4: 0x00414044, 0x2D8: 0x00414052},
        }
        for unit, approved in expected.items():
            fields = {r['offset']: r for r in units[unit]['relocations']
                      if r['symbol'].startswith('$L')}
            self.assertEqual(set(fields), set(approved))
            for offset, destination in approved.items():
                with self.subTest(unit=unit, offset=hex(offset)):
                    self.assertEqual(fields[offset]['target'], destination)
                    self.assertEqual(fields[offset]['type'], 'DIR32')

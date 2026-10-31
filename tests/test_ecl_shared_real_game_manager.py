"""Genuine TH09 GameManager owns one native C++ symbol across ECL/ExAttack.

These target-independent tests check manifest identities. Complete original
PE/COFF replay of affected owners remains the exactness Oracle.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
GM = '?g_GameManager@@3UAsciiGameManagerView@@A'
TRANSFORM_X = '?TransformPopupX@AsciiGameManagerView@@QAEMM@Z'
TRANSFORM_Y = '?TransformPopupY@AsciiGameManagerView@@QAEMM@Z'


class SharedRealGameManagerEclOwners(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units = tomllib.load(f)['units']

    def test_exact_ecl_source_has_only_original_real_global_and_methods(self):
        actual = [entry for spec in self.units.values()
                  if spec.get('source')=='src/EclManager.cpp'
                  for entry in spec.get('relocations',[])]
        fake = ('ExAttackType8GameManagerView@@','ExAttackType7GameManagerView@@')
        self.assertTrue(actual)
        self.assertTrue(all(not any(s in r['symbol'] for s in fake) for r in actual))
        globals_ = [r for r in actual if r['symbol']==GM]
        real_calls = [r for r in actual if r['symbol'] in (TRANSFORM_X,TRANSFORM_Y)]
        self.assertGreaterEqual(len(globals_),20)
        # Fan draw callbacks retain genuine native GameManager XY calls.
        self.assertEqual(len(real_calls),120)
        self.assertTrue(all(r['target']==0x004A7D90 for r in globals_))
        self.assertTrue(all((r['symbol'],r['target']) in (
            (TRANSFORM_X,0x00401680),(TRANSFORM_Y,0x004016B0))
            for r in real_calls))

    def test_exact_movement_speed_and_exattack_difficulty_addends(self):
        movement=self.units['ecl-post-update-movement']
        self.assertEqual((movement['size'],movement['compare_size'],
                          len(movement['relocations'])),(967,992,41))
        r={x['offset']:x for x in movement['relocations']}
        for off in (0x53,0xD2):
            self.assertEqual((r[off]['symbol'],r[off]['target'],r[off].get('addend')),
                             (GM,0x4A7D90,0xEC))
        for name in ('exattack-type7-update','exattack-type8-9-update'):
            rows=self.units[name]['relocations']
            self.assertTrue(any(r['symbol']==GM and
                                r.get('addend',0)==0x11C and r['target']==0x4A7D90
                                for r in rows))
        for name in ('exattack-type8-init','exattack-type7-init','exattack-type9-init',
                     'exattack-type6-initialize','exattack-type19-init',
                     'exattack-type21-init'):
            rows=self.units[name]['relocations']
            self.assertEqual(sum(r['symbol'] in (TRANSFORM_X,TRANSFORM_Y)
                                 for r in rows),4)

    def test_native_type6_to9_setup_callees_and_side_global(self):
        side = self.units['exattack-record-get-side-state']
        allocate = self.units['ex-attack-allocate-dynamic-data']
        self.assertEqual(side['relocations'], [
            {'offset': 7, 'type': 'DIR32', 'symbol': GM, 'target': 0x4A7D90}])
        for name in ('exattack-type6-initialize', 'exattack-type7-init',
                     'exattack-type8-init', 'exattack-type9-init'):
            calls = [r for r in self.units[name]['relocations']
                     if r['type'] == 'REL32']
            for callee in (side, allocate):
                bound = [r for r in calls if r['target'] == callee['target_address']]
                with self.subTest(unit=name, callee=callee['symbol']):
                    self.assertEqual(len(bound), 1)
                    self.assertEqual(bound[0]['symbol'], callee['symbol'])

    def test_pinned_original_pause_and_etama_exact_private_destinations(self):
        # Labels are compiler spelling only, never substitute for actual
        # original VA checks. Cold PE/COFF comparison still owns exactness.
        controls={
            'ascii-menu-state4-update':{
                0x3D:0x435A68,0x478:0x435631,0x47C:0x435670,
                0x480:0x4357B9,0x484:0x4358D8,0x488:0x435A14,
                0x48C:0x4359F8,0x490:0x435A30},
            'bullet-draw-single':{
                0x17:0x413BC8,0xD8:0x413B0B,0xDC:0x413B13,
                0xE0:0x413B1B,0xE4:0x413B23},
            'etama-controller-added-callback':{
                0x17F:0x4140A4,0x186:0x414098,
                0x1EB:0x41411C,0x1F2:0x414110,
                0x258:0x413FCA,0x25C:0x413FDB,0x260:0x413FFC,
                0x2D0:0x414036,0x2D4:0x414044,0x2D8:0x414052},
        }
        for name, expected in controls.items():
            unit=self.units[name]
            fields={r['offset']:r for r in unit['relocations']
                    if r['symbol'].startswith('$L')}
            self.assertEqual(set(fields),set(expected))
            for off,va in expected.items():
                with self.subTest(unit=name,offset=hex(off)):
                    self.assertEqual((fields[off]['type'],fields[off]['target']),
                                     ('DIR32',va))
        self.assertEqual((self.units['ascii-menu-state4-update']['size'],
                          self.units['ascii-menu-state4-update']['compare_size']),
                         (1144,1172))
        self.assertEqual((self.units['bullet-draw-single']['size'],
                          self.units['bullet-draw-single']['compare_size']),
                         (215,232))
        self.assertEqual((self.units['etama-controller-added-callback']['size'],
                          self.units['etama-controller-added-callback']['compare_size']),
                         (598,832))


if __name__=='__main__':
    unittest.main()

"""Genuine TH09 GameManager owns one native C++ symbol across ECL/ExAttack.

Tests are target-independent guard rails. The cold full 15,564-byte RunEcl,
Type19/21 callbacks, 26 exact ECL source units, and unrelated Etama/Pause
exact source units must also pass complete original PE/COFF replay.
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
        cls.header = (ROOT/'src/AsciiGameManagerView.hpp').read_text()
        cls.overlay = (ROOT/'src/ExAttackType8GameManagerView.hpp').read_text()
        cls.ecl = (ROOT/'src/EclManager.cpp').read_text()
        cls.post = (ROOT/'src/EclPostRuntime.inl').read_text()
        cls.type19 = (ROOT/'src/ExAttackInitType19.inl').read_text()
        cls.type21 = (ROOT/'src/ExAttackInitType21.inl').read_text()
        cls.trail = (ROOT/'scripts/inspect-exattack-trail-init.py').read_text()
        cls.update21 = (ROOT/'scripts/inspect-exattack-type21.py').read_text()
        cls.update7 = (ROOT/'scripts/inspect-exattack-type7.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units = tomllib.load(f)['units']

    def test_single_global_cpp_identity_and_real_coordinate_methods(self):
        self.assertIn('extern AsciiGameManagerView g_GameManager;',self.header)
        self.assertIn('typedef AsciiGameManagerView ExAttackType8GameManagerView;',self.overlay)
        self.assertNotIn('struct ExAttackType8GameManagerView\n',self.overlay)
        self.assertNotIn('struct ExAttackType8GameManagerView\n',self.header)
        self.assertEqual(self.overlay.count('#include "AsciiGameManagerView.hpp"'),1)
        self.assertIn('float TransformPopupX(float value);',self.header)
        self.assertIn('float TransformPopupY(float value);',self.header)
        for text in (self.type19,self.type21):
            self.assertEqual(text.count('#include "ExAttackType8GameManagerView.hpp"'),1)
            self.assertEqual(text.count('g_GameManager.TransformPopupX('),2)
            self.assertEqual(text.count('g_GameManager.TransformPopupY('),2)

    def test_shared_game_manager_physical_field_layout_is_genuine(self):
        for clause in ('sizeof(AsciiGameManagerSideView) == 0x38',
                       'offsetof(AsciiGameManagerSideView, player) == 0x04',
                       'offsetof(AsciiGameManagerSideView, etama08) == 0x08',
                       'offsetof(AsciiGameManagerSideView, anmOwner0C) == 0x0C',
                       'offsetof(AsciiGameManagerSideView, runtime) == 0x1C',
                       'offsetof(AsciiGameManagerView, speedEC) == 0xEC',
                       'offsetof(AsciiGameManagerView, difficulty) == 0x11C',
                       'offsetof(AsciiGameManagerView, coordinateOffsetX) == 0x350'):
            with self.subTest(clause=clause):
                self.assertIn(clause,self.header)
        self.assertIn('union { int difficulty; int difficulty11C; };',self.header)
        self.assertIn('PlayerLifecycleView *player04;',self.header)
        self.assertIn('EtamaController *etama08;',self.header)
        self.assertIn('ExAttackType8AnmOwner *anmOwner0C;',self.header)
        self.assertEqual(self.ecl.count('g_GameManager.speedEC'),2)
        self.assertEqual(self.post.count('g_GameManager.speedEC'),2)

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
        self.assertEqual(len(real_calls),60)
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

    def test_original_callsite_auditors_exclude_second_class_alias(self):
        for script in (self.trail,self.update21,self.update7):
            self.assertIn(GM,script)
            self.assertNotIn('ExAttackType8GameManagerView@@',script)
        self.assertIn(TRANSFORM_X,self.trail)
        self.assertIn(TRANSFORM_Y,self.trail)
        self.assertNotIn('ExAttackType7GameManagerView@@',self.update7)
        self.assertIn('0x401680',self.trail)
        self.assertIn('0x4016b0',self.trail)
        self.assertIn('0x4a7d90',self.trail)

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

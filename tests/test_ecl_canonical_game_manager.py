"""One real TH09 g_GameManager for ECL, ExAttack and exact rendering leaves.

These source/manifest tests protect original-bound C++ symbol identity and
layout after source- and profile-cold entire-owner PE/COFF verification.
They neither credit the open 15,564-byte RunEcl nor the Type19/21 init bosses.
"""

from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
GLOBAL = '?g_GameManager@@3UAsciiGameManagerView@@A'
TRANSFORM_X = '?TransformPopupX@AsciiGameManagerView@@QAEMM@Z'
TRANSFORM_Y = '?TransformPopupY@AsciiGameManagerView@@QAEMM@Z'
OLD_GLOBAL = '?g_GameManager@@3UExAttackType8GameManagerView@@A'
OLD_X = '?TransformPopupX@ExAttackType8GameManagerView@@QAEMM@Z'
OLD_Y = '?TransformPopupY@ExAttackType8GameManagerView@@QAEMM@Z'


class CanonicalGameManagerRealABI(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.shared = (ROOT/'src/AsciiGameManagerView.hpp').read_text()
        cls.ecl_alias = (ROOT/'src/ExAttackType8GameManagerView.hpp').read_text()
        cls.ecl = (ROOT/'src/EclManager.cpp').read_text()
        cls.type19 = (ROOT/'src/ExAttackInitType19.inl').read_text()
        cls.type21 = (ROOT/'src/ExAttackInitType21.inl').read_text()
        cls.trail_inspector = (ROOT/'scripts/inspect-exattack-trail-init.py').read_text()
        cls.type7_inspector = (ROOT/'scripts/inspect-exattack-type7.py').read_text()
        cls.type21_inspector = (ROOT/'scripts/inspect-exattack-type21.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            cls.units = tomllib.load(stream)['units']

    def test_single_true_global_declaration_and_native_class(self):
        self.assertIn('extern AsciiGameManagerView g_GameManager;',self.shared)
        self.assertEqual(self.shared.count('extern AsciiGameManagerView g_GameManager;'),1)
        self.assertIn('typedef AsciiGameManagerView ExAttackType8GameManagerView;',self.ecl_alias)
        self.assertNotIn('struct ExAttackType8GameManagerView\n',self.ecl_alias)
        self.assertNotIn('extern ExAttackType8GameManagerView g_GameManager;',self.ecl_alias)
        self.assertIn('#include "AsciiGameManagerView.hpp"',self.ecl_alias)
        self.assertIn('#include "ExAttackType8GameManagerView.hpp"',self.ecl)

    def test_shared_layout_preserves_original_side_and_speed_offsets(self):
        expected=(
            '(sizeof(AsciiGameManagerSideView) == 0x38)',
            'offsetof(AsciiGameManagerSideView, player) == 0x04',
            'offsetof(AsciiGameManagerSideView, etama08) == 0x08',
            'offsetof(AsciiGameManagerSideView, anmOwner0C) == 0x0C',
            'offsetof(AsciiGameManagerSideView, runtime) == 0x1C',
            'offsetof(AsciiGameManagerView, speedEC) == 0xEC',
            'offsetof(AsciiGameManagerView, difficulty) == 0x11C',
            'offsetof(AsciiGameManagerView, field0FC) == 0x0FC',
        )
        for fragment in expected:
            with self.subTest(fragment=fragment):
                self.assertIn(fragment,self.shared)
        self.assertIn('union { int difficulty; int difficulty11C; };',self.shared)
        self.assertIn('PlayerLifecycleView *player04;',self.shared)
        self.assertIn('AsciiPlayerPopupPositionView *player;',self.shared)
        self.assertIn('EtamaController *etama08;',self.shared)
        self.assertIn('ExAttackType8AnmOwner *anmOwner0C;',self.shared)
        self.assertIn('g_GameManager.speedEC;',self.ecl)
        self.assertIn('g_GameManager.TransformPopupX(',self.type19)
        self.assertIn('g_GameManager.TransformPopupY(',self.type21)

    def test_original_game_manager_exact_methods_are_independent(self):
        for name,va,symbol in (
            ('game-manager-transform-popup-x',0x401680,TRANSFORM_X),
            ('game-manager-transform-popup-y',0x4016B0,TRANSFORM_Y),
            ('game-manager-inverse-transform-popup-x',0x442420,
             '?InverseTransformPopupX@AsciiGameManagerView@@QAEMM@Z'),
            ('game-manager-inverse-transform-popup-y',0x442450,
             '?InverseTransformPopupY@AsciiGameManagerView@@QAEMM@Z'),
        ):
            unit=self.units[name]
            self.assertEqual((unit['source'],unit['target_address'],unit['size'],unit['symbol']),
                             ('src/GameManagerCoordinates.cpp',va,46,symbol))

    def test_no_fake_symbol_names_remain_in_current_boss_oracle(self):
        self.assertEqual(self.trail_inspector.count('"'+GLOBAL+'"'),0)
        for audit in (self.trail_inspector,self.type7_inspector,self.type21_inspector):
            self.assertNotIn(OLD_GLOBAL,audit)
            self.assertNotIn(OLD_X,audit)
            self.assertNotIn(OLD_Y,audit)
        self.assertIn("'"+GLOBAL+"': 0x4a7d90",self.trail_inspector)
        self.assertIn("'"+GLOBAL+"': 0x4a7d90",self.type21_inspector)
        self.assertIn("'"+GLOBAL+"': 0x4A7D90",self.type7_inspector)
        self.assertIn("'"+TRANSFORM_X+"': 0x401680",self.trail_inspector)
        self.assertIn("'"+TRANSFORM_Y+"': 0x4016b0",self.trail_inspector)

    def test_every_ecl_exact_manifest_field_retargets_same_original_va(self):
        affected=[(name,spec) for name,spec in self.units.items()
                  if spec['source']=='src/EclManager.cpp']
        names=set()
        for unit,spec in affected:
            for field in spec['relocations']:
                name=field['symbol'];names.add(name)
                if name in (GLOBAL,TRANSFORM_X,TRANSFORM_Y):
                    address={GLOBAL:0x4A7D90,TRANSFORM_X:0x401680,
                             TRANSFORM_Y:0x4016B0}[name]
                    with self.subTest(unit=unit,offset=hex(field['offset'])):
                        self.assertEqual(field['target'],address)
                self.assertNotIn('ExAttackType8GameManagerView@@',name)
        self.assertIn(GLOBAL,names)
        self.assertIn(TRANSFORM_X,names)
        self.assertIn(TRANSFORM_Y,names)

    def test_exact_switch_label_manifest_destinations_never_change(self):
        # Complete source-cold comparison independently verifies these VAs.
        for unit, expected in (
            ('ascii-menu-state4-update',{0x3D:0x435A68,0x478:0x435631,
                0x47C:0x435670,0x480:0x4357B9,0x484:0x4358D8,
                0x488:0x435A14,0x48C:0x4359F8,0x490:0x435A30}),
            ('bullet-draw-single',{0x17:0x413BC8,0xD8:0x413B0B,
                0xDC:0x413B13,0xE0:0x413B1B,0xE4:0x413B23}),
            ('etama-controller-added-callback',{0x17F:0x4140A4,
                0x186:0x414098,0x1EB:0x41411C,0x1F2:0x414110,
                0x258:0x413FCA,0x25C:0x413FDB,0x260:0x413FFC,
                0x2D0:0x414036,0x2D4:0x414044,0x2D8:0x414052}),
        ):
            rows={r['offset']:r for r in self.units[unit]['relocations']
                  if r['symbol'].startswith('$')}
            self.assertEqual(set(rows),set(expected))
            for off,va in expected.items():
                with self.subTest(unit=unit,off=hex(off)):
                    self.assertEqual((rows[off]['target'],rows[off]['type']),
                                     (va,'DIR32'))


if __name__=='__main__':
    unittest.main()

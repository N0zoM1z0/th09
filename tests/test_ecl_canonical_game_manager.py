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


class CanonicalGameManagerRealABI(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            cls.units = tomllib.load(stream)['units']

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

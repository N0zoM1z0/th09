"""Original-retail math ABI for large Etama OnDraw's exact bullet leaf.

The original 600-byte draw boss remains nonexact. Pure-input tests do not
replace original-PE/COFF exact comparator or independent Factory receipts.
"""
from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch,call

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('etama_real_angle',ROOT/'scripts/check-boss-frontiers.py')
frontier=importlib.util.module_from_spec(spec);spec.loader.exec_module(frontier)

class EtamaRealMathABI(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_boss_and_bullet_leaf_share_true_native_runtime_math(self):
        code=(ROOT/'src/EtamaControllerDraw.cpp').read_text()
        header=(ROOT/'src/RuntimeAngleMath.hpp').read_text()
        self.assertIn('#include "RuntimeAngleMath.hpp"',code)
        self.assertNotIn('extern float __stdcall AddNormalizeAngle',code)
        self.assertIn('float __fastcall AddNormalizeAngle(float angle, float delta);',header)
        self.assertEqual(code.count('AddNormalizeAngle('),1)
        frontier.validate_etama_real_angle_abi(self.units)
        self.assertEqual(frontier.ETAMA_TRUE_ANGLE_CALL,
            ('bullet-draw-single',0x00413AF0,215,232,0xB7,
             '?AddNormalizeAngle@@YIMMM@Z',0x0042AED0))
        src=self.units['bullet-draw-single']
        self.assertEqual(len(src['relocations']),16)
        self.assertEqual([(r['offset'],r['type'],r['target']) for r in src['relocations']
                          if 'AddNormalizeAngle' in r['symbol']],
                         [(0xB7,'REL32',0x0042AED0)])

    def test_false_callee_targets_types_profiles_and_source_rejected(self):
        cases=(
            ('bullet-draw-single','target_address',0x413AF1),
            ('bullet-draw-single','size',216),
            ('bullet-draw-single','compare_size',231),
            ('bullet-draw-single','profile',['/Ob0']),
            ('bullet-draw-single','source','src/False.cpp'),
            ('bullet-draw-single','object','build/matching/Other.obj'),
            ('runtime-add-normalize-angle','source','src/Other.cpp'),
            ('runtime-add-normalize-angle','symbol','?AddNormalizeAngle@@YGMMM@Z'),
            ('runtime-add-normalize-angle','target_address',0x42AED1),
            ('runtime-add-normalize-angle','size',112),
            ('runtime-add-normalize-angle','profile',['/Ob2']),
            ('etama-controller-added-callback','target_address',0x413E41),
            ('etama-controller-added-callback','size',599),
            ('etama-controller-added-callback','compare_size',831),
        )
        for name,key,value in cases:
            evidence=deepcopy(self.units);evidence[name][key]=value
            with self.subTest(unit=name,property=key),self.assertRaises(RuntimeError):
                frontier.validate_etama_real_angle_abi(evidence)
        for missing in ('bullet-draw-single','runtime-add-normalize-angle',
                        'etama-controller-added-callback'):
            evidence=deepcopy(self.units);del evidence[missing]
            with self.subTest(missing=missing),self.assertRaises(RuntimeError):
                frontier.validate_etama_real_angle_abi(evidence)

    def test_physical_callsite_byte_and_mangled_callee_corruption_rejected(self):
        for key,value in (
            ('offset',0xB8),('target',0x42AED1),
            ('type','DIR32'),('addend',1),
            ('symbol','?AddNormalizeAngle@@YGMMM@Z')):
            data=deepcopy(self.units)
            r=next(r for r in data['bullet-draw-single']['relocations']
                   if 'AddNormalizeAngle' in r['symbol'])
            r[key]=value
            with self.subTest(field=key),self.assertRaises(RuntimeError):
                frontier.validate_etama_real_angle_abi(data)
        data=deepcopy(self.units)
        data['bullet-draw-single']['relocations'].append(
          {'offset':0xBD,'type':'REL32','symbol':'?AddNormalizeAngle@@YIMMM@Z',
           'target':0x42AED0})
        with self.assertRaises(RuntimeError):
            frontier.validate_etama_real_angle_abi(data)

    def test_focused_replay_cold_builds_real_111_byte_leaf_once(self):
        with (patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical',side_effect=lambda n:{'unit':n}) as exact):
            self.assertEqual(frontier.replay_etama_real_angle_exact(already_checked=False),
                             [{'unit':'runtime-add-normalize-angle'}])
        build.assert_called_once_with('runtime-add-normalize-angle')
        exact.assert_called_once_with('runtime-add-normalize-angle')
        with (patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical') as exact):
            self.assertEqual(frontier.replay_etama_real_angle_exact(already_checked=True),[])
        build.assert_not_called();exact.assert_not_called()

    def test_fake_angle_math_rejected_before_attempting_exact_source_credit(self):
        modified=deepcopy(self.units)
        modified['bullet-draw-single']['relocations'][7]['target']=0xDEADBEEF
        # Simulate a forged alias of the actual angle call, not an unrelated field.
        target_row=next(r for r in modified['bullet-draw-single']['relocations']
                        if 'AddNormalizeAngle' in r['symbol'])
        target_row['symbol']='?AddNormalizeAngle@@YGMMM@Z'
        with (patch.object(frontier.tomllib,'load',return_value={'units':modified}),
              patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical') as exact,
              self.assertRaises(RuntimeError)):
            frontier.replay_etama_real_angle_exact(already_checked=False)
        build.assert_not_called();exact.assert_not_called()

if __name__=='__main__':unittest.main()

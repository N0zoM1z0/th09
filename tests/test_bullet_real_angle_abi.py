"""Original-v1.50a true AddNormalizeAngle symbols across the big Etama TU.

Actual original PE 2,216-byte OnUpdate and 18 canonical exact source units
are verified separately by --owner bullet; these are input-corruption guards.
"""
from copy import deepcopy
from pathlib import Path
import importlib.util
import tomllib
import unittest
from unittest.mock import patch

ROOT=Path(__file__).resolve().parents[1]
sp=importlib.util.spec_from_file_location('bullet_math_native',ROOT/'scripts/check-boss-frontiers.py')
frontier=importlib.util.module_from_spec(sp);sp.loader.exec_module(frontier)

class BulletMathNative(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:cls.units=tomllib.load(f)['units']

    def test_five_real_global_math_calls_includes_big_onupdate(self):
        src=(ROOT/'src/BulletManager.cpp').read_text()
        self.assertIn('#include "RuntimeAngleMath.hpp"',src)
        self.assertNotIn('extern float __stdcall AddNormalizeAngle',src)
        self.assertEqual(src.count('AddNormalizeAngle('),5)
        frontier.validate_bullet_real_angle_abi(self.units)
        self.assertEqual(frontier.BULLET_REAL_ANGLE_CALLS,{
          'bullet-update-polar-acceleration':0x52,
          'bullet-update-aimed-direction-change':0x81,
          'bullet-update-boundary-bounce':0x8D,
          'etama-spawn-single-bullet':0x400,
          'etama-controller-update':0x546,
        })
        for unit,off in frontier.BULLET_REAL_ANGLE_CALLS.items():
            row=next(x for x in self.units[unit]['relocations']
                     if 'AddNormalizeAngle' in x['symbol'])
            with self.subTest(unit=unit):
                self.assertEqual((row['offset'],row['type'],row['symbol'],row['target']),
                    (off,'REL32','?AddNormalizeAngle@@YIMMM@Z',0x42AED0))
        self.assertEqual((self.units['etama-controller-update']['size'],
                          self.units['etama-controller-update']['compare_size'],
                          len(self.units['etama-controller-update']['relocations'])),
                         (2195,2216,88))

    def test_true_mathematical_callee_and_bad_profiles_fail_closed(self):
        cases=(
          ('runtime-add-normalize-angle','source','src/Fake.cpp'),
          ('runtime-add-normalize-angle','symbol','?AddNormalizeAngle@@YGMMM@Z'),
          ('runtime-add-normalize-angle','target_address',0x42AED1),
          ('runtime-add-normalize-angle','size',112),
          ('runtime-add-normalize-angle','profile',['/O1']),
          ('etama-controller-update','size',2196),
          ('etama-controller-update','compare_size',2215),
          ('etama-controller-update','target_address',0x4146F1),
          ('etama-controller-update','source','src/Wrong.cpp'),
          ('etama-controller-update','profile',['/Ob0']),
        )
        for unit,key,value in cases:
            units=deepcopy(self.units);units[unit][key]=value
            with self.subTest(unit=unit,key=key),self.assertRaises(RuntimeError):
                frontier.validate_bullet_real_angle_abi(units)

    def test_moved_duplicate_or_mangled_callsite_fails(self):
        for name in frontier.BULLET_REAL_ANGLE_CALLS:
            index=next(i for i,r in enumerate(self.units[name]['relocations'])
                       if 'AddNormalizeAngle' in r['symbol'])
            for key,value in (
              ('offset',self.units[name]['relocations'][index]['offset']+1),
              ('type','DIR32'),
              ('symbol','?AddNormalizeAngle@@YGMMM@Z'),
              ('target',0x42AED1),('addend',4)):
                copied=deepcopy(self.units);copied[name]['relocations'][index][key]=value
                with self.subTest(name=name,change=key),self.assertRaises(RuntimeError):
                    frontier.validate_bullet_real_angle_abi(copied)
            copied=deepcopy(self.units)
            copied[name]['relocations'].append(dict(copied[name]['relocations'][index]))
            with self.subTest(name=name,change='duplicate'),self.assertRaises(RuntimeError):
                frontier.validate_bullet_real_angle_abi(copied)

    def test_focused_cold_replay_builds_real_math_callee_only_if_needed(self):
        with (patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical',side_effect=lambda s:{'unit':s}) as canon):
            self.assertEqual(frontier.replay_bullet_real_angle_exact(already_checked=False),
                             [{'unit':'runtime-add-normalize-angle'}])
        build.assert_called_once_with('runtime-add-normalize-angle')
        canon.assert_called_once_with('runtime-add-normalize-angle')
        with (patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical') as canon):
            self.assertEqual(frontier.replay_bullet_real_angle_exact(already_checked=True),[])
        build.assert_not_called();canon.assert_not_called()

if __name__=='__main__':unittest.main()

"""TH09 RunEcl's real global float-angle callee and complete physical graph.

Target-independent protection for the genuine fastcall C++ source identity.
The private SHA-verified retail executable, 15,564-byte owner and affected exact
EclManager siblings are cold-replayed independently by --owner runecl.
"""

from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch, call

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'runecl_original_angle_owner',ROOT/'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)


class TrueRunEclAngle(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units = tomllib.load(f)['units']

    @staticmethod
    def synthetic_audit():
        calls=[{'candidate_offset':i,'symbol':'?UnrelatedOriginalCall%d'%i}
               for i in range(375-8)]
        for off in sorted(frontier.RUNECL_REAL_ANGLE_CALL_OFFSETS):
            calls.append({
                'candidate_offset':off,
                'symbol':frontier.RUNECL_REAL_ANGLE_SYMBOL,
                'expected_destination':frontier.RUNECL_REAL_ANGLE_ORIGINAL_VA,
                'target_destination':frontier.RUNECL_REAL_ANGLE_ORIGINAL_VA,
                'identity_agrees':True,
                'provenance':{'canonical_units':['runtime-add-normalize-angle']},
            })
        return {
            'diagnostic_only':True,'exactness_credit':'none',
            'audit_passed':True,'failures':[],
            'candidate_physical_bytes':15564,'target_physical_bytes':15564,
            'accounted_relocations':598,'total_relocations':598,
            'direct_calls':calls,
        }

    def test_genuine_float_fastcall_header_and_ecl_fragment_share_one_function(self):
        text=(ROOT/'src/RuntimeAngleMath.hpp').read_text()
        self.assertIn('float __fastcall AddNormalizeAngle(float angle, float delta);',text)
        source=(ROOT/'src/RuntimeAngleMath.cpp').read_text()
        self.assertIn('#include "RuntimeAngleMath.hpp"',source)
        self.assertIn('f32 AddNormalizeAngle(f32 a, f32 b)',source)
        control=(ROOT/'src/EclRunControl.inl').read_text()
        self.assertIn('#include "RuntimeAngleMath.hpp"',control)
        self.assertIn('using ::AddNormalizeAngle;',control)
        self.assertNotIn('float __stdcall AddNormalizeAngle',control)
        for name in frontier.RUNECL_REAL_ANGLE_EXATTACK_FRAGMENTS:
            with self.subTest(fragment=name):
                s=(ROOT/'src'/name).read_text()
                self.assertIn('#include "RuntimeAngleMath.hpp"',s)
                self.assertNotIn('float __stdcall AddNormalizeAngle',s)
        self.assertNotIn('?AddNormalizeAngle@Th09EclRunControl@@YGMMM@Z',
                         (ROOT/'scripts/audit-ecl-callsite-identities.py').read_text())
        frontier.validate_runecl_real_angle_owners(self.units)

    def test_true_callee_and_all_32_exact_relocation_referents(self):
        self.assertEqual(frontier.RUNECL_REAL_ANGLE_SYMBOL,'?AddNormalizeAngle@@YIMMM@Z')
        self.assertEqual(frontier.RUNECL_REAL_ANGLE_ORIGINAL_VA,0x42AED0)
        leaf=self.units['runtime-add-normalize-angle']
        self.assertEqual((leaf['target_address'],leaf['size'],leaf['symbol'],len(leaf['relocations'])),
                         (0x42AED0,111,frontier.RUNECL_REAL_ANGLE_SYMBOL,6))
        vector=self.units['runtime-vector-angle']
        self.assertEqual((vector['target_address'],vector['size'],vector['symbol'],len(vector['relocations'])),
                         (0x405700,15,'?VectorAngle@Th09EclRunControl@@YGMMM@Z',0))
        refs=[]
        for unit in self.units.values():
            if unit.get('source')!='src/EclManager.cpp': continue
            for row in unit['relocations']:
                if 'AddNormalizeAngle' in row['symbol']:refs.append(row)
        self.assertEqual(len(refs),32)
        self.assertEqual({(x['symbol'],x['type'],x['target'],x.get('addend',0))
                          for x in refs},
                         {(frontier.RUNECL_REAL_ANGLE_SYMBOL,'REL32',0x42AED0,0)})
        self.assertEqual(len(frontier.RUNECL_REAL_ANGLE_CALL_OFFSETS),8)

    def test_canonical_original_body_audit_protects_eight_true_boss_calls(self):
        result=self.synthetic_audit()
        self.assertEqual(frontier.guard_runecl_real_angle_callsite_report(result),{
            'diagnostic_only':True,'exactness_credit':'none',
            'original_bound_angle_calls':8,
        })
        for change in ({'diagnostic_only':False},{'exactness_credit':'accepted'},
                       {'audit_passed':False},{'failures':['bad']},
                       {'total_relocations':597},{'accounted_relocations':597},
                       {'candidate_physical_bytes':15563},
                       {'target_physical_bytes':15563},
                       {'direct_calls':result['direct_calls'][:-1]},
                       {'direct_calls':None}):
            with self.subTest(change=str(change)[:100]),self.assertRaises(RuntimeError):
                frontier.guard_runecl_real_angle_callsite_report({**result,**change})

    def test_reject_wrong_symbol_target_provenance_offset_or_duplicate(self):
        valid=self.synthetic_audit()
        for field,bad in (
                ('symbol','?AddNormalizeAngle@Th09EclRunControl@@YGMMM@Z'),
                ('expected_destination',0x42AED1),
                ('target_destination',0x42AED1),
                ('candidate_offset',0x0C37),
                ('identity_agrees',False),
                ('provenance',{'independent_alias_review':'not a true source'})):
            broken=deepcopy(valid)
            broken['direct_calls'][-1][field]=bad
            with self.subTest(field=field),self.assertRaises(RuntimeError):
                frontier.guard_runecl_real_angle_callsite_report(broken)
        broken=deepcopy(valid)
        broken['direct_calls'][-1]['candidate_offset']=broken['direct_calls'][-2]['candidate_offset']
        with self.assertRaises(RuntimeError):
            frontier.guard_runecl_real_angle_callsite_report(broken)

    def test_genuine_exact_unit_and_source_profile_corruption_rejected(self):
        for name,key,bad in (
            ('runtime-add-normalize-angle','size',112),
            ('runtime-add-normalize-angle','source','src/Counterfeit.cpp'),
            ('runtime-add-normalize-angle','symbol','?AddNormalizeAngle@@YGMMM@Z'),
            ('runtime-add-normalize-angle','target_address',0x42AED1),
            ('runtime-add-normalize-angle','object','build/matching/Bogus.obj'),
            ('runtime-add-normalize-angle','profile',['/Ob0']),
            ('runtime-vector-angle','size',14),
            ('runtime-vector-angle','target_address',0x405701)):
            units=deepcopy(self.units)
            units[name][key]=bad
            with self.subTest(unit=name,key=key),self.assertRaises(RuntimeError):
                frontier.validate_runecl_real_angle_owners(units)
        for name in ('runtime-add-normalize-angle','runtime-vector-angle'):
            units=deepcopy(self.units);units.pop(name)
            with self.subTest(missing=name),self.assertRaises(RuntimeError):
                frontier.validate_runecl_real_angle_owners(units)
        allrefs=[(n,i) for n,u in self.units.items() if u.get('source')=='src/EclManager.cpp'
                 for i,r in enumerate(u['relocations']) if 'AddNormalizeAngle' in r['symbol']]
        self.assertEqual(len(allrefs),32)
        for key,bad in (('symbol','?AddNormalizeAngle@@YGMMM@Z'),
                        ('type','DIR32'),('target',0x42AED1),('addend',4)):
            units=deepcopy(self.units)
            n,i=allrefs[0];units[n]['relocations'][i][key]=bad
            with self.subTest(key=key),self.assertRaises(RuntimeError):
                frontier.validate_runecl_real_angle_owners(units)

    def test_both_original_exact_angle_math_members_cold_replay_one_carrier(self):
        with (patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical',side_effect=lambda name:{'unit':name}) as exact):
            result=frontier.replay_runecl_real_angle_exact()
        self.assertEqual([x['unit'] for x in result],
                         ['runtime-add-normalize-angle','runtime-vector-angle'])
        build.assert_called_once_with('runtime-add-normalize-angle')
        self.assertEqual(exact.call_args_list,[call('runtime-add-normalize-angle'),
                                                call('runtime-vector-angle')])
        forged=deepcopy(self.units)
        forged['runtime-add-normalize-angle']['symbol']='?FakeAngle@@YGMMM@Z'
        with (patch.object(frontier.tomllib,'load',return_value={'units':forged}),
              patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical') as exact,
              self.assertRaises(RuntimeError)):
            frontier.replay_runecl_real_angle_exact()
        build.assert_not_called()
        exact.assert_not_called()


if __name__=='__main__':
    unittest.main()

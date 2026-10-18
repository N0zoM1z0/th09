"""True TH09 EtamaController method ABI consumed by the RunEcl boss.

These pure-input tests check genuine source/manifest/callee provenance. Strict
VC7.1/retail-PE equality is separately revalidated by --owner runecl.
"""

from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch, call

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('runecl_etama_true_methods',
                                             ROOT/'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)


class RealEtamaRemoveCallees(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units = tomllib.load(f)['units']

    def test_ecl_late_controller_uses_real_side_etama_owner_and_two_callees(self):
        late=(ROOT/'src/EclRunLate.inl').read_text()
        audit=(ROOT/'scripts/audit-ecl-callsite-identities.py').read_text()
        self.assertIn('EtamaController *bulletController08;',late)
        self.assertIn('inline EtamaController *BulletController(',late)
        self.assertIn('return Manager(enemy)->sideState320->bulletController08;',late)
        self.assertIn('->RemoveAllBullets(',late)
        self.assertIn('->RemoveBulletsInRadius(',late)
        self.assertNotIn('BulletControllerLateView',late)
        self.assertNotIn('BulletControllerLateView',audit)
        header=(ROOT/'src/BulletManager.hpp').read_text()
        self.assertIn('void RemoveAllBullets(int mode);',header)
        self.assertIn('void RemoveBulletsInRadius(const Float3 *position, float radius);',header)
        frontier.validate_runecl_real_etama_callees(self.units)
        actual=[]
        for name,address,symbol in frontier.RUNECL_REAL_ETAMA_REMOVE_CALLEES:
            u=self.units[name]
            actual.append((u['target_address'],u['size'],u['symbol'],u['relocations']))
            self.assertEqual((u['source'],u['object']),
                             ('src/EtamaRemoval.cpp','build/matching/EtamaRemoval.obj'))
        self.assertEqual(actual,[
            (0x412590,3,'?RemoveAllBullets@EtamaController@@QAEXH@Z',[]),
            (0x4125A0,3,'?RemoveBulletsInRadius@EtamaController@@QAEXPBUFloat3@@M@Z',[]),
        ])

    def test_original_exact_leaves_do_not_give_run_ecl_exactness_credit(self):
        with (ROOT/'config/matches.csv').open(encoding='utf-8') as file:
            matches=file.read()
        self.assertNotIn('EclManager::RunEcl,',matches)
        self.assertIn('etama-controller-remove-all-bullets',matches)
        self.assertIn('etama-controller-remove-bullets-in-radius',matches)

    def test_original_method_and_profile_corruption_rejected(self):
        for name in ('etama-controller-remove-all-bullets',
                     'etama-controller-remove-bullets-in-radius'):
            for key,bad in (('target_address',0x412591),
                            ('size',4),('object','build/matching/Fake.obj'),
                            ('source','src/Other.cpp'),('profile',['/Ob0']),
                            ('symbol','?RemoveAllBullets@BulletControllerLateView@@QAEXH@Z'),
                            ('relocations',[{'offset':0,'type':'REL32'}])):
                corrupted=deepcopy(self.units)
                corrupted[name][key]=bad
                with self.subTest(unit=name,key=key),self.assertRaises(RuntimeError):
                    frontier.validate_runecl_real_etama_callees(corrupted)
            corrupted=deepcopy(self.units); corrupted.pop(name)
            with self.subTest(missing=name),self.assertRaises(RuntimeError):
                frontier.validate_runecl_real_etama_callees(corrupted)

    def test_both_three_byte_original_methods_cold_replay_from_one_carrier(self):
        with (patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical',side_effect=lambda unit:{'unit':unit}) as exact):
            result=frontier.replay_runecl_real_etama_exact()
        self.assertEqual([r['unit'] for r in result],
                         ['etama-controller-remove-all-bullets',
                          'etama-controller-remove-bullets-in-radius'])
        build.assert_called_once_with('etama-controller-remove-all-bullets')
        self.assertEqual(exact.call_args_list,[
            call('etama-controller-remove-all-bullets'),
            call('etama-controller-remove-bullets-in-radius'),
        ])

    def test_forged_true_callee_does_not_execute_match_comparator(self):
        tampered=deepcopy(self.units)
        tampered['etama-controller-remove-bullets-in-radius']['symbol']=(
            '?RemoveBulletsInRadius@BulletControllerLateView@@QAEXPBUFloat3@@M@Z')
        with (patch.object(frontier.tomllib,'load',return_value={'units':tampered}),
              patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical') as exact,
              self.assertRaises(RuntimeError)):
            frontier.replay_runecl_real_etama_exact()
        build.assert_not_called()
        exact.assert_not_called()


if __name__=='__main__':
    unittest.main()

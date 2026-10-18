"""Real original AnmManager::ExecuteScript ABI in 2,216-byte Etama OnUpdate.

The source/original opcode comparator checks complete code and 88 COFF
referents separately. These pure manifest guards do not grant exact credit.
"""
import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT=Path(__file__).resolve().parents[1]
sp=importlib.util.spec_from_file_location('true_etama_script',ROOT/'scripts/check-boss-frontiers.py')
front=importlib.util.module_from_spec(sp);sp.loader.exec_module(front)

class BulletTrueAnmManager(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_genuine_manager_and_full_2216_byte_boss_callsite_identity(self):
        source=(ROOT/'src/BulletManager.cpp').read_text()
        header=(ROOT/'src/AnmManager.hpp').read_text()
        self.assertEqual(source.count('#include "AnmManager.hpp"'),1)
        self.assertEqual(source.count('g_AnmManager->ExecuteScript('),6)
        self.assertNotIn('BulletAnmManagerView',source)
        self.assertIn('extern AnmManager *g_AnmManager;',header)
        front.validate_bullet_true_anm_manager_abi(self.units)
        u=self.units['etama-controller-update']
        fields=[(r['offset'],r['type'],r['symbol'],r['target']) for r in u['relocations']
                if 'AnmManager' in r['symbol'] or 'ExecuteScript@' in r['symbol']]
        self.assertEqual(fields,list(front.BULLET_TRUE_ANM_SITES))
        self.assertEqual((u['size'],u['compare_size'],len(u['relocations'])),(2195,2216,88))
        self.assertEqual(len(front.BULLET_TRUE_ANM_SITES),8)
        self.assertEqual([x[0] for x in front.BULLET_TRUE_ANM_SITES],
                         [0x176,0x17B,0x3F4,0x3F9,0x596,0x5A2,0x837,0x83D])
        leaf=self.units['anm-manager-execute-script']
        self.assertEqual((leaf['size'],leaf['compare_size'],leaf['symbol']),
          (9358,9776,'?ExecuteScript@AnmManager@@QAEHPAUAnmVm@@@Z'))

    def test_forged_site_offset_callee_class_or_kind_is_rejected(self):
        name='etama-controller-update'
        rows=self.units[name]['relocations']
        for index,site in enumerate(front.BULLET_TRUE_ANM_SITES):
            off=site[0]
            ri=next(i for i,x in enumerate(rows) if x['offset']==off)
            for key,value in (
                ('offset',off+1),('symbol','?ExecuteScript@BulletAnmManagerView@@QAEHPAUAnmVm@@@Z'),
                ('target',0x00436F31),('type','REL32' if site[1]=='DIR32' else 'DIR32'),
                ('addend',4)):
                u=copy.deepcopy(self.units);u[name]['relocations'][ri][key]=value
                with self.subTest(site=index,field=key),self.assertRaises(RuntimeError):
                    front.validate_bullet_true_anm_manager_abi(u)
        bad=copy.deepcopy(self.units);bad[name]['relocations'].pop()
        with self.assertRaises(RuntimeError):front.validate_bullet_true_anm_manager_abi(bad)

    def test_reject_bad_big_boss_or_fake_9776_byte_callee_source(self):
        for name,key,bad in (
            ('etama-controller-update','source','src/False.cpp'),
            ('etama-controller-update','object','build/matching/Other.obj'),
            ('etama-controller-update','target_address',0x4146F1),
            ('etama-controller-update','size',2196),
            ('etama-controller-update','compare_size',2215),
            ('etama-controller-update','profile',['/Ob0']),
            ('anm-manager-execute-script','source','src/Other.cpp'),
            ('anm-manager-execute-script','object','build/matching/Other.obj'),
            ('anm-manager-execute-script','target_address',0x436F31),
            ('anm-manager-execute-script','symbol','?ExecuteScript@BulletAnmManagerView@@QAEHPAUAnmVm@@@Z'),
            ('anm-manager-execute-script','size',9357),
            ('anm-manager-execute-script','compare_size',9775),
            ('anm-manager-execute-script','profile',['/Ob1']),
        ):
            data=copy.deepcopy(self.units);data[name][key]=bad
            with self.subTest(name=name,key=key),self.assertRaises(RuntimeError):
                front.validate_bullet_true_anm_manager_abi(data)
        for missing in ('etama-controller-update','anm-manager-execute-script'):
            data=copy.deepcopy(self.units);del data[missing]
            with self.subTest(missing=missing),self.assertRaises(RuntimeError):
                front.validate_bullet_true_anm_manager_abi(data)

    def test_no_duplicate_replay_when_boss_callee_already_validated_in_all(self):
        with (patch.object(front,'build') as build,
              patch.object(front,'canonical',side_effect=lambda name:{'unit':name}) as compare):
            result=front.replay_bullet_true_anm_exact(already_checked=False)
        self.assertEqual(result,[{'unit':'anm-manager-execute-script'}])
        build.assert_called_once_with('anm-manager-execute-script')
        compare.assert_called_once_with('anm-manager-execute-script')
        with (patch.object(front,'build') as build,
              patch.object(front,'canonical') as compare):
            result=front.replay_bullet_true_anm_exact(already_checked=True)
        self.assertEqual(result,[])
        build.assert_not_called();compare.assert_not_called()

    def test_injected_fake_abi_fails_before_cold_build_or_exact_claim(self):
        data=copy.deepcopy(self.units)
        row=next(f for f in data['etama-controller-update']['relocations']
                 if f['symbol']==front.BULLET_TRUE_ANM_SCRIPT)
        row['symbol']='?ExecuteScript@BulletAnmManagerView@@QAEHPAUAnmVm@@@Z'
        with (patch.object(front.tomllib,'load',return_value={'units':data}),
              patch.object(front,'build') as build,
              patch.object(front,'canonical') as compare,
              self.assertRaises(RuntimeError)):
            front.replay_bullet_true_anm_exact(already_checked=False)
        build.assert_not_called();compare.assert_not_called()

if __name__=='__main__':unittest.main()

"""Genuine AnmLoaded native C++ owner of EffectManager::InitializeEffect.

The independent original PE 407-byte caller and full exact sibling carrier
are cold-replayed separately. These target-independent adversarial tests reject
fake ANM source/prototype and original-VA relocation identity drift.
"""
from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('effect_true_anm',ROOT/'scripts/check-boss-frontiers.py')
frontier=importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)

class EffectTrueAnmLoaded(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            cls.units=tomllib.load(stream)['units']

    def test_real_cpp_method_shared_with_exact_large_caller(self):
        effect=(ROOT/'src/EffectManager.cpp').read_text()
        header=(ROOT/'src/EffectManager.hpp').read_text()
        self.assertIn('#include "AnmManager.hpp"', effect)
        self.assertIn('this->effectAnm->SetAndExecuteScriptIdx(',effect)
        self.assertNotIn('EffectAnmLoadedMethodView',effect)
        self.assertIn('AnmLoaded *effectAnm;',header)
        frontier.validate_effect_real_anm_loaded_abi(self.units)
        actual=self.units['effect-manager-initialize-effect']
        sites=[(row['offset'],row['type'],row['symbol'],row['target']) for row in
               actual['relocations'] if 'SetAndExecuteScriptIdx' in row['symbol']]
        self.assertEqual(sites,[(0xE6,'REL32',
            '?SetAndExecuteScriptIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z',0x00403E00)])
        callee=self.units['anm-loaded-set-and-execute-script-idx']
        self.assertEqual((callee['target_address'],callee['size'],callee['source']),
                         (0x00403E00,41,'src/AnmLoadedExecute.cpp'))
        self.assertEqual(self.units['effect-manager-update']['size'],475)

    def test_wrong_real_callee_and_original_field_va_rejected(self):
        for name,field,bad in (
            ('effect-manager-initialize-effect','target_address',0x0040CA61),
            ('effect-manager-initialize-effect','size',408),
            ('effect-manager-initialize-effect','source','src/Other.cpp'),
            ('effect-manager-initialize-effect','object','build/matching/Other.obj'),
            ('effect-manager-initialize-effect','profile',['/Ob0']),
            ('anm-loaded-set-and-execute-script-idx','symbol','?SetAndExecuteScriptIdx@EffectAnmLoadedMethodView@@QAEXPAUAnmVm@@H@Z'),
            ('anm-loaded-set-and-execute-script-idx','target_address',0x00403E01),
            ('anm-loaded-set-and-execute-script-idx','source','src/Other.cpp'),
            ('anm-loaded-set-and-execute-script-idx','size',40),
            ('anm-loaded-set-and-execute-script-idx','profile',['/Ob0']),
        ):
            wrong=deepcopy(self.units);wrong[name][field]=bad
            with self.subTest(name=name,field=field),self.assertRaises(RuntimeError):
                frontier.validate_effect_real_anm_loaded_abi(wrong)
        caller='effect-manager-initialize-effect'
        field_index=next(i for i,r in enumerate(self.units[caller]['relocations'])
                     if 'SetAndExecuteScriptIdx' in r['symbol'])
        for key,bad in (('offset',0xE7),('target',0x00403E01),('type','DIR32'),
                        ('symbol','?SetAndExecuteScriptIdx@EffectAnmLoadedMethodView@@QAEXPAUAnmVm@@H@Z'),
                        ('addend',1)):
            wrong=deepcopy(self.units);wrong[caller]['relocations'][field_index][key]=bad
            with self.subTest(original_field=key),self.assertRaises(RuntimeError):
                frontier.validate_effect_real_anm_loaded_abi(wrong)
        for absent in ('effect-manager-initialize-effect','anm-loaded-set-and-execute-script-idx'):
            wrong=deepcopy(self.units);del wrong[absent]
            with self.subTest(missing=absent),self.assertRaises(RuntimeError):
                frontier.validate_effect_real_anm_loaded_abi(wrong)

    def test_exact_41_byte_callee_replayed_only_from_real_source(self):
        with (patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical',side_effect=lambda name:{'unit':name}) as canon):
            replay=frontier.replay_effect_real_anm_loaded_exact()
        self.assertEqual(replay,[{'unit':'anm-loaded-set-and-execute-script-idx'}])
        build.assert_called_once_with('anm-loaded-set-and-execute-script-idx')
        canon.assert_called_once_with('anm-loaded-set-and-execute-script-idx')

    def test_fake_runtime_method_never_invokes_target_exact_comparator(self):
        tampered=deepcopy(self.units)
        caller=tampered['effect-manager-initialize-effect']
        row=next(x for x in caller['relocations'] if 'SetAndExecuteScriptIdx' in x['symbol'])
        row['symbol']='?SetAndExecuteScriptIdx@EffectAnmLoadedMethodView@@QAEXPAUAnmVm@@H@Z'
        with (patch.object(frontier.tomllib,'load',return_value={'units':tampered}),
              patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical') as canon,
              self.assertRaises(RuntimeError)):
            frontier.replay_effect_real_anm_loaded_exact()
        build.assert_not_called();canon.assert_not_called()

if __name__=='__main__':unittest.main()

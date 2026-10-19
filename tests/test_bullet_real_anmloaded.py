"""Native AnmLoaded::SetSprite ABI shared by four TH09 large gameplay owners.

Source exactness is independently proven by original-bound PE/VC7.1 cold
comparisons, not by a source header alias or manifest-only evidence. Guards
reject fake class names, changed original physical offsets and callee VAs.
"""
from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch,call

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('bullet_loaded_real',ROOT/'scripts/check-boss-frontiers.py')
front=importlib.util.module_from_spec(spec);spec.loader.exec_module(front)


class RealAnmLoadedBullet(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_real_side_controller_pointer_and_true_189_byte_sprite_member(self):
        header=(ROOT/'src/BulletManager.hpp').read_text()
        self.assertIn('class AnmLoaded;',header)
        self.assertIn('AnmLoaded *bulletAnm;',header)
        self.assertIn('offsetof(EtamaController, bulletAnm) == 0x25E1BC',header)
        self.assertNotIn('struct BulletAnmLoadedView',header)
        real_header=(ROOT/'src/AnmManager.hpp').read_text()
        self.assertIn('int SetSprite(AnmVm *vm, int spriteIndex);',real_header)
        self.assertEqual(len(front.BULLET_TRUE_LOADED_CALLS),4)
        self.assertEqual(sum(len(v[-1]) for v in front.BULLET_TRUE_LOADED_CALLS.values()),8)
        for source in ('BulletManager.cpp','BulletAdvanceTransformProgram.cpp',
                       'EnemyEclOpcode2.cpp','EtamaControllerDraw.cpp','BulletLaserSpawn.cpp'):
            with self.subTest(source=source):
                code=(ROOT/'src'/source).read_text()
                self.assertIn('#include "AnmManager.hpp"',code)
                self.assertNotIn('BulletAnmLoadedView',code)
        self.assertIn('controller->bulletAnm = anm;',
                      (ROOT/'src/EtamaControllerDraw.cpp').read_text())
        self.assertIn('this->bulletAnm->SetSprite(',
                      (ROOT/'src/BulletLaserSpawn.cpp').read_text())
        front.validate_bullet_true_loaded_abi(self.units)
        callee=self.units['anm-loaded-set-sprite']
        self.assertEqual((callee['target_address'],callee['size'],callee['symbol']),
                         (0x436AC0,189,front.BULLET_TRUE_LOADED_SET_SPRITE))

    def test_pin_original_big_callback_sizes_and_all_eight_true_calls(self):
        for name,(source,va,size,physical,offsets) in front.BULLET_TRUE_LOADED_CALLS.items():
            u=self.units[name]
            with self.subTest(name=name):
                self.assertEqual((u['source'],u['target_address'],u['size'],
                                  u.get('compare_size',size)),(source,va,size,physical))
                calls=[(r['offset'],r['type'],r['symbol'],r['target'],r.get('addend',0))
                       for r in u['relocations'] if 'SetSprite@' in r['symbol']]
                self.assertEqual(calls,[
                    (i,'REL32',front.BULLET_TRUE_LOADED_SET_SPRITE,0x436AC0,0)
                    for i in offsets])
        u=self.units['etama-controller-update']
        self.assertEqual((u['size'],u['compare_size'],len(u['relocations'])),
                         (2195,2216,88))
        u=self.units['bullet-advance-transform-program']
        self.assertEqual((u['size'],u['compare_size'],len(u['relocations'])),
                         (1236,1320,26))

    def test_reject_false_native_members_moved_offsets_and_original_vas(self):
        for name,(source,va,size,physical,offsets) in front.BULLET_TRUE_LOADED_CALLS.items():
            for offset in offsets:
                index=next(i for i,x in enumerate(self.units[name]['relocations'])
                           if x['offset']==offset)
                for key,bad in (
                    ('offset',offset+1),('type','DIR32'),('target',0x436AC1),
                    ('symbol','?SetSprite@BulletAnmLoadedView@@QAEHPAUAnmVm@@H@Z'),
                    ('addend',4)):
                    copy=deepcopy(self.units)
                    copy[name]['relocations'][index][key]=bad
                    with self.subTest(owner=name,offset=hex(offset),field=key),self.assertRaises(RuntimeError):
                        front.validate_bullet_true_loaded_abi(copy)
            for key,bad in (('source','src/Wrong.cpp'),('size',size+1),
                            ('target_address',va+1),('profile',['/Ob0'])):
                copy=deepcopy(self.units);copy[name][key]=bad
                with self.subTest(owner=name,property=key),self.assertRaises(RuntimeError):
                    front.validate_bullet_true_loaded_abi(copy)
            copy=deepcopy(self.units)
            copy[name]['relocations'].append({
                'offset':size-4,'type':'REL32',
                'symbol':'?SetSprite@BulletAnmLoadedView@@QAEHPAUAnmVm@@H@Z',
                'target':0x436AC0})
            with self.subTest(owner=name,property='new-fake'),self.assertRaises(RuntimeError):
                front.validate_bullet_true_loaded_abi(copy)

    def test_bad_single_true_callee_and_false_carrier_source_rejected(self):
        for key,value in (
            ('symbol','?SetSprite@BulletAnmLoadedView@@QAEHPAUAnmVm@@H@Z'),
            ('target_address',0x436AC1),('source','src/Wrong.cpp'),
            ('size',188),('profile',['/Ob0']),('object','build/matching/Wrong.obj')):
            corrupt=deepcopy(self.units);corrupt['anm-loaded-set-sprite'][key]=value
            with self.subTest(field=key),self.assertRaises(RuntimeError):
                front.validate_bullet_true_loaded_abi(corrupt)
        for missing in ('anm-loaded-set-sprite','enemy-ecl-opcode2',
                        'etama-spawn-laser-pattern','bullet-draw-single'):
            wrong=deepcopy(self.units);wrong.pop(missing)
            with self.subTest(absent=missing),self.assertRaises(RuntimeError):
                front.validate_bullet_true_loaded_abi(wrong)

    def test_focused_cold_replays_entire_cross_source_family_and_single_true_callee(self):
        def run(source,carriers):
            return [{'unit':x+'-dummy'} for x in carriers]
        with (patch.object(front,'cold_replay_exact_source',side_effect=run) as replay,
              patch.object(front,'build') as build,
              patch.object(front,'canonical',side_effect=lambda x:{'unit':x}) as canonical):
            result=front.replay_bullet_true_loaded_exact(etama_prechecked=False)
        self.assertEqual([x['unit'] for x in result],[
            'build/matching/EnemyEclOpcode2.obj-dummy',
            'build/matching/BulletLaserSpawn.obj-dummy',
            'build/matching/EtamaControllerDraw.obj-dummy',
            'anm-loaded-set-sprite'])
        self.assertEqual([x.args[0] for x in replay.call_args_list],
                         list(front.BULLET_TRUE_LOADED_CARRIERS))
        build.assert_called_once_with('anm-loaded-set-sprite')
        canonical.assert_called_once_with('anm-loaded-set-sprite')
        with (patch.object(front,'cold_replay_exact_source',side_effect=run) as replay,
              patch.object(front,'build') as build,
              patch.object(front,'canonical',side_effect=lambda x:{'unit':x}) as canon):
            result=front.replay_bullet_true_loaded_exact(etama_prechecked=True)
        self.assertEqual([x.args[0] for x in replay.call_args_list],
                         ['src/EnemyEclOpcode2.cpp','src/BulletLaserSpawn.cpp'])
        self.assertEqual(len(result),3)
        build.assert_called_once_with('anm-loaded-set-sprite')
        canon.assert_called_once_with('anm-loaded-set-sprite')

    def test_forged_call_rejected_before_any_canonical_replay(self):
        bad=deepcopy(self.units)
        bad['enemy-ecl-opcode2']['relocations'][2]['symbol']='?SetSprite@BulletAnmLoadedView@@QAEHPAUAnmVm@@H@Z'
        with (patch.object(front.tomllib,'load',return_value={'units':bad}),
              patch.object(front,'build') as build,
              patch.object(front,'cold_replay_exact_source') as replay,
              patch.object(front,'canonical') as canon,
              self.assertRaises(RuntimeError)):
            front.replay_bullet_true_loaded_exact(etama_prechecked=False)
        build.assert_not_called();replay.assert_not_called();canon.assert_not_called()


if __name__=='__main__':unittest.main()

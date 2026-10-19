"""Shared TH09 ExAttack record/side/EffectManager owner ABI checks.

Target-independent assertions only. The original 12/81-byte exact leaves,
full 763/727-byte exact owners, and EclManager same-TU exact functions
require separate authenticated VC7.1 cold replays.
"""

import csv
import importlib.util
from pathlib import Path
import tomllib
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]


def reviewed_row(address):
    with (ROOT/'config/functions.csv').open(newline='') as inp:
        result=[r for r in csv.DictReader(inp) if int(r['address'],16)==address]
    if len(result)!=1:raise ValueError('unreviewed original function identity')
    return result[0]


class OriginalSharedRecordOwner(TestCase):
    def test_real_allocation_and_side_accessor_are_independent_exact_leaves(self):
        for address,owner,filename in (
                (0x00440D90,'ExAttackAllocateRecordView::AllocateDynamicData',
                 'ExAttackAllocateDynamicData.cpp'),
                (0x00440CB0,'ExAttackRecordSideView::GetSideState',
                 'ExAttackRecordSideState.cpp')):
            with self.subTest(owner=owner):
                r=reviewed_row(address)
                self.assertEqual(r['status'],'matching')
                self.assertEqual(r['proposed_name'],owner)
                self.assertTrue(r['source_file'].endswith(filename))
                self.assertIn('#include "ExAttackRecordRuntimeView.hpp"',
                              (ROOT/'src'/filename).read_text())

    def test_shared_record_and_actual_effect_manager_offsets(self):
        header=(ROOT/'src/ExAttackRecordRuntimeView.hpp').read_text()
        effect=(ROOT/'src/EffectManager.hpp').read_text()
        for key in ('ExAttackGameSideSize38','ExAttackGameSideEffectAt0C',
                    'ExAttackEffectManagerAnmAt2D4','ExAttackRecordSideIndexAt08',
                    'ExAttackRecordSideViewSizeIs0C','ExAttackAllocateSize38',
                    'ExAttackAllocateExtraAt34','ExAttackAllocateVmCountAt2C',
                    'ExAttackAllocateDynamicAt1C'):
            self.assertIn(key,header)
        self.assertIn('EffectManager *effectManager0C;',header)
        self.assertIn('ExAttackGameManagerSideView *GetSideState();',header)
        self.assertIn('void AllocateDynamicData(int vmCount, int extraBytes);',header)
        self.assertIn('AnmLoaded *effectAnm;',effect)
        self.assertIn('EffectManagerSizeIs2DC',effect)

    def test_full_original_bosses_use_same_true_receiver_and_canonical_owner(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        for type_number,address,size,nfields in (
                ('19',0x0044A030,763,57),('21',0x0044B220,727,53)):
            source=(ROOT/f'src/ExAttackInitType{type_number}.inl').read_text()
            with self.subTest(type=type_number):
                self.assertIn('#include "ExAttackRecordRuntimeView.hpp"',source)
                self.assertIn('reinterpret_cast<ExAttackAllocateRecordView *>(record)',source)
                self.assertIn('reinterpret_cast<ExAttackRecordSideView *>(record)',source)
                self.assertIn('->GetSideState()->effectManager0C->effectAnm->ExecuteAnmIdx(',source)
                self.assertIn('g_ReplayRng.GetRandomF32SignedInRange(0.06666667f)',source)
                self.assertEqual(reviewed_row(address)['status'],'matching')
                unit=units[f'exattack-type{type_number}-init']
                self.assertEqual((unit['source'],unit['target_address'],unit['size'],
                                  len(unit['relocations'])),
                                 ('src/EclManager.cpp',address,size,nfields))
                self.assertEqual(unit['functions'],
                                 [f'ExAttackInitializeCallbackType{type_number}'])
                for false_type in ('ExAttackType'+type_number+'AllocateView',
                                   'ExAttackType'+type_number+'SideLookupView',
                                   'ExAttackType'+type_number+'AnmOwner'):
                    self.assertNotIn(false_type,source)

    def test_original_bound_inspector_rejects_old_unreviewed_abi_names(self):
        spec=importlib.util.spec_from_file_location(
            'exattack_realrecord_oracle',ROOT/'scripts/inspect-exattack-trail-init.py')
        m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
        self.assertEqual(m.DESTINATIONS['?AllocateDynamicData@ExAttackAllocateRecordView@@QAEXHH@Z'],
                         0x00440D90)
        self.assertEqual(m.DESTINATIONS['?GetSideState@ExAttackRecordSideView@@QAEPAUExAttackGameManagerSideView@@XZ'],
                         0x00440CB0)
        for symbol in list(m.DESTINATIONS):
            for bad in ('ExAttackType19AllocateView','ExAttackType21AllocateView',
                        'ExAttackType19SideLookupView','ExAttackType21SideLookupView'):
                self.assertNotIn(bad,symbol)

"""Genuine native TH09 ZunMemory provenance across both Replay compiler TUs.

Twelve large/leaf exact sources still need their full original PE/COFF cold
replay; these offline input tests forbid guessed global, method and offset
identities or corrupt real independently exact allocator callees.
"""
import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import call, patch

ROOT=Path(__file__).resolve().parents[1]
sp=importlib.util.spec_from_file_location('native_replay_memory',ROOT/'scripts/check-boss-frontiers.py')
frontier=importlib.util.module_from_spec(sp);sp.loader.exec_module(frontier)


class ReplayTrueZunMemory(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_both_original_compiler_profiles_and_76_true_memory_sites(self):
        source=(ROOT/'src/ReplayInput.cpp').read_text()
        header=(ROOT/'src/ZunMemory.hpp').read_text()
        self.assertIn('#include "ZunMemory.hpp"',source)
        self.assertNotIn('ZunMemoryReplayView',source)
        self.assertIn('extern ZunMemory g_ZunMemory;',header)
        self.assertEqual(len(frontier.REPLAY_REAL_MEMORY_SITES),6)
        self.assertEqual(len(frontier.REPLAY_REAL_MEMORY_CALLEES),3)
        self.assertEqual(sum(len(offsets) for _,_,groups in
             frontier.REPLAY_REAL_MEMORY_SITES.values()
             for offsets in groups.values()),76)
        self.assertEqual(sum(v[0] for v in frontier.REPLAY_REAL_MEMORY_SITES.values()),3157)
        frontier.validate_replay_real_memory_abi(self.units)
        for name,(_,_,groups) in frontier.REPLAY_REAL_MEMORY_SITES.items():
            row=self.units[name]
            observed=[(r['offset'],r['symbol'],r['type'],r['target'])
                      for r in row['relocations']
                      if r['symbol'] in {v[0] for v in frontier.REPLAY_REAL_MEMORY_SYMBOLS.values()}]
            expected=sorted((off,*frontier.REPLAY_REAL_MEMORY_SYMBOLS[s])
                            for s,positions in groups.items() for off in positions)
            with self.subTest(name=name):
                self.assertEqual(observed,expected)

    def test_negative_callee_source_size_abi_flags_and_alias(self):
        for name in frontier.REPLAY_REAL_MEMORY_CALLEES:
            for field,value in (
                    ('source','src/Fake.cpp'),
                    ('object','build/matching/Invalid.obj'),
                    ('symbol','?Alloc@ZunMemoryReplayView@@QAEPAXIPBD@Z'),
                    ('target_address',0x401350),
                    ('size',21),
                    ('profile',['/Ob0'])):
                copy_units=copy.deepcopy(self.units)
                copy_units[name][field]=value
                with self.subTest(unit=name,key=field),self.assertRaises(RuntimeError):
                    frontier.validate_replay_real_memory_abi(copy_units)
        for name in frontier.REPLAY_REAL_MEMORY_CALLEES:
            copy_units=copy.deepcopy(self.units);del copy_units[name]
            with self.subTest(absent=name),self.assertRaises(RuntimeError):
                frontier.validate_replay_real_memory_abi(copy_units)

    def test_original_big_replay_callsite_and_global_storage_corruption_fails(self):
        for name in frontier.REPLAY_REAL_MEMORY_SITES:
            for target_symbol in ('?g_ZunMemory@@3VZunMemory@@A',
                                  '?Free@ZunMemory@@QAEXPAX@Z',
                                  '?Alloc@ZunMemory@@QAEPAXIPBD@Z',
                                  '?AddToRegistry@ZunMemory@@QAEPAXPAXIPAD@Z'):
                indices=[i for i,r in enumerate(self.units[name]['relocations'])
                         if r['symbol']==target_symbol]
                if not indices:continue
                idx=indices[0]
                for key,value in (
                       ('offset',self.units[name]['relocations'][idx]['offset']+1),
                       ('target',0x004AD019),
                       ('type','REL32' if target_symbol.startswith('?g_') else 'DIR32'),
                       ('symbol','?g_ZunMemory@@3UZunMemoryReplayView@@A'),
                       ('addend',4)):
                    bad=copy.deepcopy(self.units);bad[name]['relocations'][idx][key]=value
                    with self.subTest(name=name,symbol=target_symbol,field=key),self.assertRaises(RuntimeError):
                        frontier.validate_replay_real_memory_abi(bad)
            corrupt=copy.deepcopy(self.units)
            corrupt[name]['relocations'].pop()
            with self.subTest(name=name,field='field-count'),self.assertRaises(RuntimeError):
                frontier.validate_replay_real_memory_abi(corrupt)
            corrupt=copy.deepcopy(self.units)
            corrupt[name]['size']+=1
            with self.subTest(name=name,field='whole-extent'),self.assertRaises(RuntimeError):
                frontier.validate_replay_real_memory_abi(corrupt)

    def test_fake_abi_wrong_profile_or_sneaked_fake_alias_in_other_exact_rejected(self):
        for name,field,value in (
                ('replay-manager-begin-recording-stage','profile',['/EHsc']),
                ('replay-input-update','profile',['/Ob0']),
                ('replay-manager-release','source','src/Wrong.cpp'),
                ('replay-manager-begin-recording-stage','object','build/matching/Fake.obj')):
            bad=copy.deepcopy(self.units);bad[name][field]=value
            with self.subTest(name=name,field=field),self.assertRaises(RuntimeError):
                frontier.validate_replay_real_memory_abi(bad)
        bad=copy.deepcopy(self.units)
        bad['replay-manager-playback-input']['relocations'].append({
            'offset':0x10,'type':'DIR32',
            'symbol':'?g_ZunMemory@@3UZunMemoryReplayView@@A','target':0x004AD018})
        with self.assertRaises(RuntimeError):
            frontier.validate_replay_real_memory_abi(bad)

    def test_all_real_callees_cold_replayed_from_one_source_and_fake_fails_first(self):
        with (patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical',side_effect=lambda n:{'unit':n}) as canon):
            replays=frontier.replay_replay_real_memory_exact()
        self.assertEqual(replays,[{'unit':name} for name in frontier.REPLAY_REAL_MEMORY_CALLEES])
        build.assert_called_once_with('zun-memory-alloc')
        self.assertEqual(canon.call_args_list,[call(name) for name in frontier.REPLAY_REAL_MEMORY_CALLEES])
        bad=copy.deepcopy(self.units)
        row=next(r for r in bad['replay-manager-begin-recording-stage']['relocations']
                 if 'Free@ZunMemory' in r['symbol'])
        row['symbol']='?Free@ZunMemoryReplayView@@QAEXPAX@Z'
        with (patch.object(frontier.tomllib,'load',return_value={'units':bad}),
              patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical') as canon,
              self.assertRaises(RuntimeError)):
            frontier.replay_replay_real_memory_exact()
        build.assert_not_called();canon.assert_not_called()

if __name__=='__main__':unittest.main()

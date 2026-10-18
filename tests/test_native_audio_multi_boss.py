"""Original-linked true SoundPlayer provenance across five large TH09 owners.

This independent target-free regression guard only checks input identity.
Source-cold VC7.1 function-byte equality and exact credit remain the work of
--owner audio / canonical original-PE comparison / Factory forced replay.
"""
from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch, call

ROOT=Path(__file__).resolve().parents[1]
sp=importlib.util.spec_from_file_location('native_multi_boss',ROOT/'scripts/check-boss-frontiers.py')
frontier=importlib.util.module_from_spec(sp);sp.loader.exec_module(frontier)

class SoundAcrossBosses(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_all_five_large_source_translation_units_are_true_cplusplus(self):
        self.assertEqual(len(frontier.AUDIO_TRUE_CARRIERS),5)
        self.assertEqual(len(frontier.AUDIO_TRUE_EXACT_OWNERS),12)
        self.assertEqual(sum(x[3] for x in frontier.AUDIO_TRUE_EXACT_OWNERS.values()),4100)
        self.assertEqual(sum(x[2] for x in frontier.AUDIO_TRUE_EXACT_OWNERS.values()),4081)
        for source,carriers in frontier.AUDIO_TRUE_CARRIERS.items():
            with self.subTest(source=source):
                code=(ROOT/source).read_text()
                self.assertIn('#include "SoundPlayer.hpp"',code)
                self.assertIn('g_SoundPlayer.',code)
                self.assertNotIn('SoundPlayerView',code)
                groups=frontier.exact_source_carriers(source,carriers,self.units)
                self.assertEqual(set(groups),set(carriers))
        frontier.validate_audio_true_cpp_owners(self.units)

    def test_callsite_offsets_and_exact_original_true_sound_targets(self):
        seen=0
        for name,expected in frontier.AUDIO_TRUE_ORIGINAL_FIELDS.items():
            row=self.units[name]
            actual=[(r['offset'],r['symbol'],r['type'],r['target'])
                    for r in row['relocations'] if r['symbol'] in frontier.AUDIO_TRUE_METHODS]
            required=[(off,sym,*frontier.AUDIO_TRUE_METHODS[sym]) for off,sym in expected]
            with self.subTest(name=name):
                self.assertEqual(actual,required)
            seen+=len(expected)
        self.assertEqual(seen,25)
        self.assertEqual(self.units['supervisor-audio-has-flag-bit1']['symbol'],
          '?HasFlagBit1@SupervisorAudioGameManagerView@?A0xfb5c156d@@QAEHXZ')
        for name,(va,size,physical,symbol) in frontier.AUDIO_TRUE_EXACT_CALLEES.items():
            u=self.units[name]
            self.assertEqual((u['target_address'],u['size'],u.get('compare_size',size),u['symbol']),
                             (va,size,physical,symbol))

    def test_forged_source_abi_callsite_and_compiler_flags_rejected(self):
        samples=('exattack-type14-22-update','front-calc-callback','game-window-render',
                 'supervisor-play-music','supervisor-stop-audio','supervisor-deleted-callback')
        for name in samples:
            first=next(i for i,r in enumerate(self.units[name]['relocations'])
                       if r['symbol'] in frontier.AUDIO_TRUE_METHODS)
            for field,bad in (
                ('offset',self.units[name]['relocations'][first]['offset']+1),
                ('symbol','?g_SoundPlayer@@3UOldSoundPlayerView@@A'),
                ('target',0x004DC699),('type','REL32'),('addend',4)):
                bad_units=deepcopy(self.units)
                bad_units[name]['relocations'][first][field]=bad
                with self.subTest(name=name,field=field),self.assertRaises(RuntimeError):
                    frontier.validate_audio_true_cpp_owners(bad_units)
        for name in ('front-calc-callback','exattack-type14-22-update',
                     'supervisor-deleted-callback'):
            for key,value in (('source','src/Unknown.cpp'),('target_address',0x410000),
                              ('size',50),('profile',['/O1'])):
                wrong=deepcopy(self.units);wrong[name][key]=value
                with self.subTest(name=name,key=key),self.assertRaises(RuntimeError):
                    frontier.validate_audio_true_cpp_owners(wrong)
        for name in ('soundplayer-play-sound-by-idx','soundplayer-queue-command',
                     'soundplayer-process-queues'):
            wrong=deepcopy(self.units);wrong[name]['symbol']='?QueueCommand@FakeClass@@QAEXHHPAD@Z'
            with self.subTest(name=name),self.assertRaises(RuntimeError):
                frontier.validate_audio_true_cpp_owners(wrong)

    def test_anonymous_namespace_source_path_fingerprint_must_be_canonical(self):
        for name in ('supervisor-play-audio','supervisor-load-music',
                     'supervisor-deleted-callback','supervisor-audio-has-flag-bit1'):
            wrong=deepcopy(self.units)
            if name=='supervisor-audio-has-flag-bit1':
                wrong[name]['symbol']=wrong[name]['symbol'].replace(
                    '@?A0xfb5c156d@@','@?A0x0349a4d3@@')
            else:
                changed=False
                for r in wrong[name]['relocations']:
                    if '@?A0x' in r['symbol']:
                        r['symbol']=r['symbol'].replace('@?A0xfb5c156d@@','@?A0x0349a4d3@@')
                        r['symbol']=r['symbol'].replace('@?A0x4471bc32@@','@?A0xe89b9897@@')
                        changed=True
                        break
                self.assertTrue(changed)
            with self.subTest(name=name),self.assertRaises(RuntimeError):
                frontier.validate_audio_true_cpp_owners(wrong)

    def test_no_duplicate_or_unverified_exact_siblings(self):
        fake=[{'unit':n,'result':'exact'} for n in frontier.AUDIO_TRUE_EXACT_OWNERS]
        fake.extend([{'unit':n,'result':'exact'} for n in frontier.AUDIO_TRUE_EXACT_CALLEES])
        with (patch.object(frontier,'cold_replay_exact_source',side_effect=lambda source,carriers:[
                  {'unit':n,'result':'exact'} for n in frontier.AUDIO_TRUE_EXACT_OWNERS
                  if frontier.AUDIO_TRUE_EXACT_OWNERS[n][0]==source]) as run,
              patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical',side_effect=lambda n:{'unit':n,'result':'exact'}) as canonical):
            owners,exact=frontier.run_audio_true_exact(protect_exact=True)
        self.assertEqual(owners,{})
        self.assertEqual([x['unit'] for x in exact],[x['unit'] for x in fake])
        self.assertEqual(run.call_count,5)
        self.assertEqual(build.call_args_list,[call(name) for name in frontier.AUDIO_TRUE_EXACT_CALLEES])
        self.assertEqual(canonical.call_count,3)
        with (patch.object(frontier,'cold_replay_exact_source') as run,
              patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical') as canonical):
            owners,exact=frontier.run_audio_true_exact(protect_exact=False)
        self.assertEqual((owners,exact),({},[]))
        run.assert_not_called();build.assert_not_called();canonical.assert_not_called()

if __name__=='__main__':unittest.main()

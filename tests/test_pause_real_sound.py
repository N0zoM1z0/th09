"""TH09 Pause big menu callbacks and exact SoundPlayer native class identity.

Source/manifest-only adversarial tests; actual original-target-cold comparison
of 13 canonical menus + two separately exact leaves happens in --owner pause.
"""
from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch,call

ROOT=Path(__file__).resolve().parents[1]
sp=importlib.util.spec_from_file_location('pause_real_sound',ROOT/'scripts/check-boss-frontiers.py')
frontier=importlib.util.module_from_spec(sp);sp.loader.exec_module(frontier)

class TruePauseAudio(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_two_big_menu_callbacks_have_real_audio_abi(self):
        for file in ('AsciiManagerMenu.cpp','AsciiMenuState5Update.cpp'):
            src=(ROOT/'src'/file).read_text()
            self.assertIn('#include "SoundPlayer.hpp"',src)
            self.assertIn('g_SoundPlayer.PlaySoundByIdx(',src)
            self.assertNotIn('struct AsciiSoundPlayerView',src)
            self.assertNotIn('extern AsciiSoundPlayerView',src)
        self.assertIn('void SoundPlayer::ResumeAfterPause()',
                      (ROOT/'src/SoundPlayerPause.cpp').read_text())
        self.assertEqual((ROOT/'src/SoundPlayer.hpp').read_text().count('void ResumeAfterPause();'),1)
        frontier.validate_pause_real_sound(self.units)
        self.assertEqual(len(frontier.PAUSE_REAL_SOUND_CARRIERS),4)
        expected={'ascii-menu-state5-update':(0x434e90,1213,1236),
                  'soundplayer-resume-after-pause':(0x423498,15,15),
                  'soundplayer-play-sound-by-idx':(0x43e2f0,140,140),
                  'soundplayer-queue-command':(0x43ee40,105,105)}
        for row in frontier.PAUSE_REAL_SOUND_CARRIERS:
            with self.subTest(owner=row[0]):
                self.assertEqual(row[1:4],expected[row[0]])

    def test_original_symbols_and_fields_bind_native_shared_global(self):
        for name,n in (('ascii-menu-state4-update',18),('ascii-menu-state5-update',8)):
            fields=[v for v in self.units[name]['relocations']
                    if 'SoundPlayer' in v['symbol'] or 'PlaySound' in v['symbol']]
            with self.subTest(name=name):
                self.assertEqual(len(fields),n)
                self.assertEqual([v['symbol'] for v in fields[::2]],
                                 [frontier.PAUSE_REAL_SOUND_GLOBAL]*(n//2))
                self.assertEqual([v['symbol'] for v in fields[1::2]],
                                 [frontier.PAUSE_REAL_SOUND_PLAY]*(n//2))
                self.assertTrue(all(v.get('addend',0)==0 for v in fields))
        wrapper=self.units['soundplayer-resume-after-pause']
        self.assertEqual(wrapper['functions'],['SoundPlayer::ResumeAfterPause'])
        self.assertEqual(wrapper['symbol'],frontier.PAUSE_REAL_SOUND_RESUME)
        self.assertEqual([(v['offset'],v['symbol']) for v in wrapper['relocations']],
                         [(1,'??_C@_07OGJCKFMD@UnPause?$AA@'),(10,frontier.PAUSE_REAL_SOUND_QUEUE)])
        self.assertNotIn('AsciiSoundPlayerView',(ROOT/'scripts/inspect-pause-menu.py').read_text())

    def test_shared_cpp_owner_has_one_exact_source_ledger_name(self):
        for name in ('functions.csv','reccmp-functions.csv','matches.csv','implemented.csv'):
            text=(ROOT/'config'/name).read_text()
            self.assertIn('SoundPlayer::ResumeAfterPause',text)
            self.assertNotIn('AsciiSoundPlayerView::ResumeAfterPause',text)
        self.assertIn('SoundPlayer::ResumeAfterPause',
                      (ROOT/'src/SoundPlayerPause.cpp').read_text())
        self.assertEqual(self.units['soundplayer-resume-after-pause']['size'],15)

    def test_forged_native_audio_symbols_or_missing_source_unit_rejected(self):
        for name in ('ascii-menu-state4-update','ascii-menu-state5-update'):
            u=self.units[name]
            i=next(i for i,r in enumerate(u['relocations']) if r['symbol']==frontier.PAUSE_REAL_SOUND_GLOBAL)
            for field,bad in (('symbol','?g_SoundPlayer@@3UAsciiSoundPlayerView@@A'),
                              ('type','REL32'),('target',0x4dc699),('offset',u['relocations'][i]['offset']+1),('addend',4)):
                data=deepcopy(self.units);data[name]['relocations'][i][field]=bad
                with self.subTest(unit=name,field=field),self.assertRaises(RuntimeError):
                    frontier.validate_pause_real_sound(data)
        for name,key,value in (
            ('ascii-menu-state5-update','source','src/Fake.cpp'),
            ('ascii-menu-state5-update','object','build/matching/Wrong.obj'),
            ('ascii-menu-state5-update','size',1214),
            ('soundplayer-resume-after-pause','symbol','?ResumeAfterPause@AsciiSoundPlayerView@@QAEXXZ'),
            ('soundplayer-resume-after-pause','target_address',0x423499),
            ('soundplayer-queue-command','size',104),
            ('soundplayer-play-sound-by-idx','profile',['/Ob0']),
        ):
            data=deepcopy(self.units);data[name][key]=value
            with self.subTest(unit=name,key=key),self.assertRaises(RuntimeError):
                frontier.validate_pause_real_sound(data)
        data=deepcopy(self.units);data['soundplayer-resume-after-pause']['relocations'][1]['symbol']='?QueueCommand@AsciiSoundPlayerView@@QAEXHHPAD@Z'
        with self.assertRaises(RuntimeError):frontier.validate_pause_real_sound(data)

    def test_focus_rebuilds_true_big_callback_and_both_exact_sound_leaves(self):
        with (patch.object(frontier,'build') as build,
              patch.object(frontier,'canonical',side_effect=lambda name:{'unit':name}) as exact,
              patch.object(frontier,'parsed', return_value={
                  'diagnostic_only': True, 'exactness_credit': 'none',
                  'native_resume': {'complete_equal': True, 'bytes': 15, 'fields': 2},
                  'resume_literal_bytes': 8}) as data):
            result=frontier.replay_pause_real_sound_exact()
        self.assertEqual([x['unit'] for x in result],
                         [r[0] for r in frontier.PAUSE_REAL_SOUND_CARRIERS])
        self.assertEqual(build.call_args_list,[call(r[0]) for r in frontier.PAUSE_REAL_SOUND_CARRIERS])
        self.assertEqual(exact.call_args_list,[call(r[0]) for r in frontier.PAUSE_REAL_SOUND_CARRIERS])
        data.assert_called_once_with(['scripts/probe-pause-native-audio.py',
                                      '--check-resume-object',
                                      'build/matching/SoundPlayerPause.obj'])

    def test_instruction_equality_does_not_waive_resume_data(self):
        for proof in ({}, {'native_resume': {'complete_equal': True}},
                      {'diagnostic_only': True, 'exactness_credit': 'none',
                       'native_resume': {'complete_equal': True, 'bytes': 15, 'fields': 2},
                       'resume_literal_bytes': 7}):
            with (self.subTest(proof=proof), patch.object(frontier,'build'),
                  patch.object(frontier,'canonical',return_value={}),
                  patch.object(frontier,'parsed',return_value=proof),
                  self.assertRaisesRegex(RuntimeError,'actual UnPause')):
                frontier.replay_pause_real_sound_exact()

if __name__=='__main__':unittest.main()

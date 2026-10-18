"""One proven SoundPlayer ABI for Title result/cursor and MusicRoom owners.

Real original PE-linked complete-owner strict compilation is separate from
these source/manifest regression contracts. No new 940th-plus credit here.
"""
from pathlib import Path
import importlib.util
import tomllib
import unittest
from unittest.mock import patch
from copy import deepcopy

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('boss','scripts/check-boss-frontiers.py')
BOSS=importlib.util.module_from_spec(spec)
spec.loader.exec_module(BOSS)
REAL_GLOBAL='?g_SoundPlayer@@3VSoundPlayer@@A'
REAL_SOUND='?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z'
REAL_QUEUE='?QueueCommand@SoundPlayer@@QAEXHHPAD@Z'

class TrueTitleAudioABI(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.main=(ROOT/'src/TitleScreen.cpp').read_text()
        cls.music=(ROOT/'src/TitleScreenMusicRoom.cpp').read_text()
        cls.sound=(ROOT/'src/SoundPlayer.hpp').read_text()
        cls.music_audit=(ROOT/'scripts/inspect-title-music-room.py').read_text()
        cls.result_audit=(ROOT/'scripts/inspect-title-update-result.py').read_text()
        cls.character_audit=(ROOT/'scripts/inspect-title-character-selection.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            cls.units=tomllib.load(stream)['units']

    def test_real_shared_type_declarations_across_both_large_title_owners(self):
        for source in (self.main,self.music):
            self.assertIn('#include "SoundPlayer.hpp"',source)
            self.assertNotIn('struct SoundPlayerView',source)
            self.assertNotIn('extern SoundPlayerView g_SoundPlayer;',source)
            self.assertIn('g_SoundPlayer',source)
        self.assertIn('class SoundPlayer',self.sound)
        self.assertIn('extern SoundPlayer g_SoundPlayer;',self.sound)
        self.assertIn('void PlaySoundByIdx(int index, int pan);',self.sound)
        self.assertIn('void QueueCommand(int opcode, int argument, char *path);',self.sound)
        self.assertIn('g_SoundPlayer.PlaySoundByIdx(',self.main)
        self.assertIn('g_SoundPlayer.QueueCommand(',self.music)

    def test_original_bound_diagnostics_accept_only_real_external_symbols(self):
        for code in (self.music_audit,self.result_audit,self.character_audit):
            self.assertNotIn('SoundPlayerView@@',code)
            self.assertIn(REAL_GLOBAL,code)
        self.assertIn(REAL_SOUND,self.result_audit)
        self.assertIn(REAL_SOUND,self.character_audit)
        self.assertIn(REAL_QUEUE,self.music_audit)
        self.assertIn("'"+REAL_QUEUE+"': 0x43EE40",self.music_audit)

    def test_every_main_and_music_exact_beat_matches_original_global_target(self):
        total=0
        for name,unit in self.units.items():
            if unit['source'] not in ('src/TitleScreen.cpp','src/TitleScreenMusicRoom.cpp'):
                continue
            for row in unit['relocations']:
                if row['symbol'] in (REAL_GLOBAL,REAL_SOUND,REAL_QUEUE):
                    with self.subTest(name=name,offset=hex(row['offset'])):
                        assert_dest={REAL_GLOBAL:0x4DC698,
                                     REAL_SOUND:0x43E2F0,
                                     REAL_QUEUE:0x43EE40}[row['symbol']]
                        self.assertEqual(row['target'],assert_dest)
                        self.assertEqual(row['type'], 'DIR32' if row['symbol']==REAL_GLOBAL else 'REL32')
                        total+=1
                self.assertNotIn('SoundPlayerView@@',row['symbol'])
        self.assertEqual(total,35) # 12 global,20 sound calls,3 queue calls

    def test_exact_38_main_and_two_music_owners_remain_source_cold(self):
        main=[(name,u) for name,u in self.units.items() if u['source']=='src/TitleScreen.cpp']
        music=[(name,u) for name,u in self.units.items() if u['source']=='src/TitleScreenMusicRoom.cpp']
        self.assertEqual((len(main),len(music)),(38,2))
        self.assertEqual((sum(u.get('compare_size',u['size']) for _,u in main),
                          sum(u.get('compare_size',u['size']) for _,u in music)),(18411,2467))
        self.assertEqual((sum(len(u['relocations']) for _,u in main),
                          sum(len(u['relocations']) for _,u in music)),(943,80))
        self.assertEqual(self.units['title-screen-music-room']['size'],2258)
        self.assertEqual(self.units['title-screen-setup-thread']['size'],534)

    def test_start_menu_private_switch_names_did_not_change_original_va(self):
        u=self.units['title-screen-start-menu']
        self.assertEqual((u['size'],u['compare_size']),(1560,1592))
        expected={0x3C2:0x42A43B,0x618:0x42A1E9,
                  0x61C:0x42A208,0x620:0x42A23C,0x624:0x42A286,
                  0x628:0x42A2D2,0x62C:0x42A293,0x630:0x42A302,0x634:0x42A380}
        actual={r['offset']:r for r in u['relocations'] if r['symbol'].startswith('$')}
        self.assertEqual(set(actual),set(expected))
        for off,dest in expected.items():
            with self.subTest(off=hex(off)):
                self.assertEqual((actual[off]['type'],actual[off]['target']),('DIR32',dest))

    def test_two_original_exact_audio_callees_are_separate_cold_owners(self):
        expected=(('soundplayer-play-sound-by-idx',0x43E2F0,REAL_SOUND,140),
                  ('soundplayer-queue-command',0x43EE40,REAL_QUEUE,105))
        for name,addr,symbol,size in expected:
            row=self.units[name]
            with self.subTest(name=name):
                self.assertEqual((row['target_address'],row['symbol'],row['size'],row['source']),
                                 (addr,symbol,size,'src/SoundPlayerProcessQueues.cpp'))
                self.assertEqual(row['profile'],list(BOSS.TITLE_REAL_SOUND_VC71_PROFILE))
        self.assertEqual(len(BOSS.TITLE_REAL_SOUND_CARRIERS),2)

    def test_title_focused_cold_builds_both_and_all_mode_only_queue(self):
        with patch.object(BOSS,'build') as build,patch.object(BOSS,'canonical',side_effect=lambda n:{'unit':n}) as exact:
            focused=BOSS.replay_title_real_sound_exact(play_sound_prechecked=False)
        self.assertEqual([x['unit'] for x in focused],
                         ['soundplayer-play-sound-by-idx','soundplayer-queue-command'])
        self.assertEqual([c.args[0] for c in build.call_args_list],
                         ['soundplayer-play-sound-by-idx','soundplayer-queue-command'])
        self.assertEqual(exact.call_count,2)
        with patch.object(BOSS,'build') as build,patch.object(BOSS,'canonical',side_effect=lambda n:{'unit':n}):
            all_mode=BOSS.replay_title_real_sound_exact(play_sound_prechecked=True)
        self.assertEqual(all_mode,[{'unit':'soundplayer-queue-command'}])
        build.assert_called_once_with('soundplayer-queue-command')

    def test_forged_real_audio_referent_source_object_or_profile_fails_closed(self):
        for name,field,bad in (
            ('soundplayer-play-sound-by-idx','target_address',0x43E300),
            ('soundplayer-play-sound-by-idx','symbol','?PlaySoundByIdx@SoundPlayerView@@QAEXHH@Z'),
            ('soundplayer-queue-command','source','src/Fake.cpp'),
            ('soundplayer-queue-command','object','build/matching/Different.obj'),
            ('soundplayer-queue-command','profile',['/Ob0']),
        ):
            tampered=deepcopy(self.units)
            tampered[name][field]=bad
            with self.subTest(name=name,field=field), patch.object(
                    BOSS.tomllib,'load',return_value={'units':tampered}), patch.object(
                    BOSS,'build'), patch.object(BOSS,'canonical'),self.assertRaises(RuntimeError):
                BOSS.replay_title_real_sound_exact(play_sound_prechecked=False)

if __name__=='__main__':
    unittest.main()

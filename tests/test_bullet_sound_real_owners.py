"""True SoundPlayer field identities across the Etama/Bullet exact carriers.

The full original target and compiler cold replay are checked separately by
--owner bullet; this suite rejects invented source classes or source-manifest
identity drift even under CI without the private executable/VC7.1 compiler.
"""

from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch,call

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('bullet_real_audio',
                                             ROOT/'scripts/check-boss-frontiers.py')
frontiers = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontiers)


class BulletRealSound(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            cls.units = tomllib.load(stream)['units']

    def test_genuine_sound_class_is_shared_across_both_gameplay_carriers(self):
        for name,fake in (('BulletManager.cpp','BulletSoundPlayerView'),
                          ('BulletAdvanceTransformProgram.cpp','BulletAdvanceSoundPlayerView')):
            with self.subTest(name=name):
                text=(ROOT/'src'/name).read_text()
                self.assertIn('#include "SoundPlayer.hpp"',text)
                self.assertNotIn(fake,text)
                self.assertIn('g_SoundPlayer.PlaySoundByIdx(',text)
                self.assertIn('g_SoundPlayer.PlaySoundPositionedByIdx(',text)
        self.assertNotIn('PlaySoundAtPosition', (ROOT/'src/BulletManager.cpp').read_text())
        header=(ROOT/'src/SoundPlayer.hpp').read_text()
        self.assertIn('extern SoundPlayer g_SoundPlayer;', header)
        self.assertIn('void PlaySoundPositionedByIdx(int index, float positionX);',header)
        frontiers.validate_bullet_sound_real_owners(self.units)

    def test_exact_carriers_cover_all_19_bullet_units_and_the_real_callees(self):
        for src,carriers,count in (
                ('src/BulletManager.cpp',frontiers.BULLET_MAIN_EXACT_CARRIERS,18),
                ('src/BulletAdvanceTransformProgram.cpp',frontiers.BULLET_ADVANCE_EXACT_CARRIERS,1)):
            with self.subTest(source=src):
                groups=frontiers.exact_source_carriers(src,carriers,self.units)
                self.assertEqual(sum(len(group) for group in groups.values()),count)
                self.assertEqual({k:len(v) for k,v in groups.items()},carriers)
        expected=frontiers.BULLET_REAL_SOUND_FIELDS
        observed={key:0 for key in expected}
        for name,v in self.units.items():
            if v.get('source') not in ('src/BulletManager.cpp','src/BulletAdvanceTransformProgram.cpp'):
                continue
            for row in v['relocations']:
                if row['symbol'] in observed:
                    self.assertEqual((row['type'],row['target'],row.get('addend',0)),
                                     (*expected[row['symbol']],0))
                    observed[row['symbol']]+=1
                self.assertNotIn('BulletSoundPlayerView',row['symbol'])
                self.assertNotIn('BulletAdvanceSoundPlayerView',row['symbol'])
        self.assertEqual(observed,{
            '?g_SoundPlayer@@3VSoundPlayer@@A':9,
            '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z':6,
            '?PlaySoundPositionedByIdx@SoundPlayer@@QAEXHM@Z':3,
        })

    def test_reject_real_va_relabeling_fake_symbols_and_missing_origins(self):
        for name,index in (('bullet-pattern-primary',0),('bullet-pattern-primary',1),
                           ('bullet-update-boundary-bounce',0),
                           ('bullet-advance-transform-program',0),
                           ('bullet-advance-transform-program',1)):
            u=self.units[name]
            targeted=[i for i,r in enumerate(u['relocations']) if r['symbol'] in frontiers.BULLET_REAL_SOUND_FIELDS]
            assert len(targeted) >= index+1
            off=targeted[index]
            for key,bad in (('symbol','?g_SoundPlayer@@3UBulletSoundPlayerView@@A'),
                            ('target',0x4dc699),('addend',1),
                            ('type','REL32' if u['relocations'][off]['type']=='DIR32' else 'DIR32')):
                malformed=deepcopy(self.units)
                malformed[name]['relocations'][off][key]=bad
                with self.subTest(name=name,index=index,key=key),self.assertRaises(RuntimeError):
                    frontiers.validate_bullet_sound_real_owners(malformed)
        for name in ('bullet-update-boundary-bounce','bullet-pattern-primary',
                     'bullet-advance-transform-program'):
            invalid=deepcopy(self.units)
            invalid.pop(name)
            with self.subTest(name=name),self.assertRaises(RuntimeError):
                frontiers.validate_bullet_sound_real_owners(invalid)
        for name,field,bad in (
            ('bullet-update-boundary-bounce','profile',['/Ob0']),
            ('bullet-pattern-primary','object','build/matching/Other.obj'),
            ('soundplayer-play-positioned-sound-by-idx','size',152),
            ('soundplayer-play-sound-by-idx','source','src/Fake.cpp'),
            ('soundplayer-play-sound-by-idx','symbol','?PlaySoundByIdx@BulletSoundPlayerView@@QAEXHH@Z')):
            invalid=deepcopy(self.units);invalid[name][field]=bad
            with self.subTest(name=name,field=field),self.assertRaises(RuntimeError):
                frontiers.validate_bullet_sound_real_owners(invalid)

    def test_focused_replay_builds_all_19_and_both_140_153_byte_callees(self):
        def replay(src,carriers):
            if src == 'src/BulletManager.cpp':
                return [{'unit':'first-main'},{'unit':'second-main'}]
            self.assertEqual(src,'src/BulletAdvanceTransformProgram.cpp')
            return [{'unit':'only-advance'}]
        with (patch.object(frontiers,'cold_replay_exact_source',side_effect=replay) as units,
              patch.object(frontiers,'build') as build,
              patch.object(frontiers,'replay_bullet_true_loaded_exact',return_value=[]) as loaded,
              patch.object(frontiers,'canonical',side_effect=lambda s:{'unit':s}) as canonical):
            owners,checked=frontiers.run_bullet_sound(protect_exact=True)
        loaded.assert_called_once_with(etama_prechecked=False)
        self.assertEqual(owners,{})
        self.assertEqual([x['unit'] for x in checked],
            ['first-main','second-main','only-advance',
             'soundplayer-play-sound-by-idx','soundplayer-play-positioned-sound-by-idx',
             'runtime-add-normalize-angle','anm-manager-execute-script'])
        self.assertEqual(units.call_count,2)
        self.assertEqual(build.call_args_list,[call('soundplayer-play-sound-by-idx'),
                                              call('runtime-add-normalize-angle'),
                                              call('anm-manager-execute-script')])
        self.assertEqual(canonical.call_args_list,[call('soundplayer-play-sound-by-idx'),
                                                    call('soundplayer-play-positioned-sound-by-idx'),
                                                    call('runtime-add-normalize-angle'),
                                                    call('anm-manager-execute-script')])
        with (patch.object(frontiers,'cold_replay_exact_source',side_effect=replay),
              patch.object(frontiers,'replay_bullet_true_loaded_exact',return_value=[]) as loaded,
              patch.object(frontiers,'build') as build,
              patch.object(frontiers,'canonical') as canonical):
            _,checked=frontiers.run_bullet_sound(protect_exact=True,sound_prechecked=True,
                                              real_angle_prechecked=True,real_anm_prechecked=True,
                                              etama_prechecked=True)
        loaded.assert_called_once_with(etama_prechecked=True)
        self.assertEqual([r['unit'] for r in checked],['first-main','second-main','only-advance'])
        build.assert_not_called();canonical.assert_not_called()
        with (patch.object(frontiers,'cold_replay_exact_source') as units,
              patch.object(frontiers,'build') as build,
              patch.object(frontiers,'canonical') as canonical):
            self.assertEqual(frontiers.run_bullet_sound(protect_exact=False),({},[]))
        units.assert_not_called();build.assert_not_called();canonical.assert_not_called()

    def test_duplicate_exact_regressions_not_counted_as_two_original_functions(self):
        d=[{'unit':'etama-controller-update','logical_bytes':2195,'result':'exact'},
           {'unit':'bullet-pattern-primary','logical_bytes':203,'result':'exact'},
           {'unit':'etama-controller-update','logical_bytes':2195,'result':'exact'}]
        self.assertEqual(frontiers.unique_canonical_siblings(d),d[:2])
        with self.assertRaises(RuntimeError):
            frontiers.unique_canonical_siblings(d[:-1]+[{'unit':'etama-controller-update','logical_bytes':2194,'result':'exact'}])
        for fake in (None,{'unit':''},{'result':'exact'}):
            with self.subTest(fake=fake),self.assertRaises(RuntimeError):
                frontiers.unique_canonical_siblings(d+[fake])


if __name__=='__main__':
    unittest.main()

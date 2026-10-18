"""TH09 original large CardAttack callbacks use real SoundPlayer C++ identities.

Actual full original-PE/VC7.1 function replays are performed separately by
check-boss-frontiers.py --owner card, not by these no-target pure guards.
"""

from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch,call

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('card_real_audio_boss',
                                             ROOT/'scripts/check-boss-frontiers.py')
checker=importlib.util.module_from_spec(spec)
spec.loader.exec_module(checker)


class CardRealSound(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_large_original_Card_callbacks_have_no_invented_audio_class(self):
        for src,fake in (('CardAttack.cpp','CardAttackSoundPlayerView'),
                         ('CardAttackStart.cpp','CardAttackStartSoundPlayerView')):
            with self.subTest(source=src):
                source=(ROOT/'src'/src).read_text()
                self.assertIn('#include "SoundPlayer.hpp"',source)
                self.assertNotIn(fake,source)
                self.assertIn('g_SoundPlayer.PlaySoundByIdx(',source)
        checker.validate_card_real_sound_owners(self.units)
        self.assertEqual((self.units['card-attack-on-update']['size'],
                          self.units['card-attack-start']['size']),(536,923))
        for name in ('card-attack-on-update','card-attack-start'):
            sound=[r for r in self.units[name]['relocations']
                   if r['symbol'] in ('?g_SoundPlayer@@3VSoundPlayer@@A',
                                      '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z')]
            self.assertEqual(len(sound),2)
            self.assertEqual([(r['type'],r['target']) for r in sound],
                             [('DIR32',0x4dc698),('REL32',0x43e2f0)])

    def test_all_nine_real_source_owners_and_separate_original_callee(self):
        for src,carriers,expected in (
            ('src/CardAttack.cpp',checker.CARD_REAL_SOUND_EXACT_CARRIERS,8),
            ('src/CardAttackStart.cpp',checker.CARD_START_SOUND_EXACT_CARRIERS,1)):
            groups=checker.exact_source_carriers(src,carriers,self.units)
            self.assertEqual(sum(map(len,groups.values())),expected)
            self.assertEqual({name:len(units) for name,units in groups.items()},carriers)
        self.assertEqual(self.units['soundplayer-play-sound-by-idx']['size'],140)
        self.assertEqual(self.units['soundplayer-play-sound-by-idx']['symbol'],
                         '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z')

    def test_fake_sound_globals_methods_wrong_fields_or_compiler_rejected(self):
        for name in ('card-attack-on-update','card-attack-start'):
            fields=self.units[name]['relocations']
            targeted=[i for i,row in enumerate(fields)
                      if 'SoundPlayer' in row['symbol']]
            self.assertEqual(len(targeted),2)
            for idx in targeted:
                for key,value in (
                    ('symbol','?g_SoundPlayer@@3UCardAttackSoundPlayerView@@A'),
                    ('offset',fields[idx]['offset']+1),
                    ('target',fields[idx]['target']+1),
                    ('addend',4),
                    ('type','DIR32' if fields[idx]['type']=='REL32' else 'REL32')):
                    altered=deepcopy(self.units);altered[name]['relocations'][idx][key]=value
                    with self.subTest(name=name,key=key,index=idx),self.assertRaises(RuntimeError):
                        checker.validate_card_real_sound_owners(altered)
        for name,field,value in (
            ('card-attack-on-update','source','src/Fake.cpp'),
            ('card-attack-on-update','profile',['/Ob2']),
            ('card-attack-on-update','object','build/matching/Other.obj'),
            ('card-attack-constructor','profile',['/O1']),
            ('card-attack-start','size',924),
            ('soundplayer-play-sound-by-idx','symbol','?Fake@@QAEXXZ'),
            ('soundplayer-play-sound-by-idx','target_address',0x43e2f1)):
            altered=deepcopy(self.units);altered[name][field]=value
            with self.subTest(name=name,field=field),self.assertRaises(RuntimeError):
                checker.validate_card_real_sound_owners(altered)

    def test_card_focus_replays_nine_exact_owners_and_true_sound_once(self):
        def replay(source,carriers):
            if source=='src/CardAttack.cpp':return [{'unit':'big-update'},{'unit':'render'}]
            self.assertEqual(source,'src/CardAttackStart.cpp')
            return [{'unit':'start'}]
        with (patch.object(checker,'cold_replay_exact_source',side_effect=replay) as full,
              patch.object(checker,'build') as build,
              patch.object(checker,'canonical',side_effect=lambda name:{'unit':name}) as exact):
            owner,rows=checker.run_card_real_sound(protect_exact=True)
        self.assertEqual(owner,{})
        self.assertEqual(rows,[{'unit':'big-update'},{'unit':'render'},
                               {'unit':'start'},{'unit':'soundplayer-play-sound-by-idx'}])
        self.assertEqual(full.call_count,2)
        build.assert_called_once_with('soundplayer-play-sound-by-idx')
        exact.assert_called_once_with('soundplayer-play-sound-by-idx')
        with (patch.object(checker,'cold_replay_exact_source',side_effect=replay),
              patch.object(checker,'build') as build,
              patch.object(checker,'canonical') as exact):
            _,rows=checker.run_card_real_sound(protect_exact=True,sound_prechecked=True)
        self.assertEqual([r['unit'] for r in rows],['big-update','render','start'])
        build.assert_not_called();exact.assert_not_called()
        with (patch.object(checker,'cold_replay_exact_source') as full,
              patch.object(checker,'build') as build,
              patch.object(checker,'canonical') as exact):
            self.assertEqual(checker.run_card_real_sound(protect_exact=False),({},[]))
        full.assert_not_called();build.assert_not_called();exact.assert_not_called()


if __name__=='__main__':
    unittest.main()

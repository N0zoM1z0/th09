"""Native TH09 MusicRoom big-callback original PE C++ linker provenance.

Source-independent original-VA field positions, two exact full owners, three
original-source callees, and negative corruptions are guarded without using
private target in CI. Complete actual VC7.1 PE/COFF comparisons happen in
`check-boss-frontiers.py --owner title` and separately in Factory receipts.
"""
from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch,call

ROOT=Path(__file__).resolve().parents[1]
sp=importlib.util.spec_from_file_location('title_music_native',ROOT/'scripts/check-boss-frontiers.py')
front=importlib.util.module_from_spec(sp);sp.loader.exec_module(front)


class NativeMusicRoom(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as inp:
            cls.units=tomllib.load(inp)['units']

    def test_big_music_room_real_native_cpp_anm_supervisor_global(self):
        source=(ROOT/'src/TitleScreenMusicRoom.cpp').read_text()
        inspector=(ROOT/'scripts/inspect-title-music-room.py').read_text()
        self.assertIn('#include "AnmManager.hpp"',source)
        self.assertIn('#include "Supervisor.hpp"',source)
        for bad in ('struct TitleAnmManagerView','struct TitleAnmLoadedView',
                    'struct TitleSupervisorView','DrawTitleMusicText(',
                    'g_TitleAnmManager','g_TitleSupervisor'):
            self.assertNotIn(bad,source)
        self.assertIn('AnmLoaded *musicAnm;',source)
        self.assertIn('extern AnmLoaded *g_TitleMusicDescriptionAnm;',source)
        self.assertIn('g_AnmManager->DrawTextLeft(',source)
        self.assertIn('g_Supervisor.PlayAudio(',source)
        self.assertIn('g_TitleMusicDescriptionAnm->SetAndExecuteScriptIdx(',source)
        self.assertIn('int SetSprite(AnmVm *vm, int spriteIndex);',
                      (ROOT/'src/AnmManager.hpp').read_text())
        self.assertIn('extern Supervisor g_Supervisor;',
                      (ROOT/'src/Supervisor.hpp').read_text())
        self.assertNotIn('DrawTitleMusicText',inspector)
        self.assertIn('?DrawTextLeft@AnmManager@@QAAXPAUAnmVm@@KKPBDZZ',inspector)
        front.validate_title_music_native_abi(self.units)

    def test_all_29_independent_original_va_field_positions_across_two_complete_owners(self):
        self.assertEqual([len(v) for v in front.TITLE_MUSIC_NATIVE_ORIGINAL_SITES.values()],[25,4])
        for name,sites in front.TITLE_MUSIC_NATIVE_ORIGINAL_SITES.items():
            with self.subTest(owner=name):
                row=self.units[name]
                self.assertEqual((row['target_address'],row['size'],len(row['relocations'])),
                                 front.TITLE_MUSIC_NATIVE_BOUNDS[name])
                expected=[(offset,*front.TITLE_MUSIC_NATIVE_SYMBOLS[sym],0)
                          for offset,sym in sites]
                actual=[(r['offset'],r['symbol'],r['type'],r['target'],r.get('addend',0))
                        for r in row['relocations'] if r['offset'] in {x[0] for x in sites}]
                self.assertEqual(actual,expected)
        self.assertEqual(len(front.TITLE_MUSIC_REAL_INDEPENDENT_CALLEES),3)
        self.assertEqual(sum(x[3] for x in front.TITLE_MUSIC_REAL_INDEPENDENT_CALLEES.values()),697)

    def test_corrupt_any_of_original_29_field_offsets_symbols_types_targets_is_rejected(self):
        for name,sites in front.TITLE_MUSIC_NATIVE_ORIGINAL_SITES.items():
            for offset,typ in sites:
                row=self.units[name]
                index=next(i for i,r in enumerate(row['relocations']) if r['offset']==offset)
                current=row['relocations'][index]
                errors=(('offset',offset+1),('symbol','?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A'),
                        ('target',0x42ffff),('type','REL32' if current['type']=='DIR32' else 'DIR32'),
                        ('addend',4))
                for key,bad in errors:
                    forged=deepcopy(self.units)
                    forged[name]['relocations'][index][key]=bad
                    with self.subTest(owner=name,site=hex(offset),field=key),self.assertRaises(RuntimeError):
                        front.validate_title_music_native_abi(forged)

    def test_missing_exact_source_profiles_and_callees_fail_closed(self):
        for name in front.TITLE_MUSIC_NATIVE_BOUNDS:
            for key,value in (('size',1),('source','src/Fake.cpp'),
                              ('object','build/matching/Fake.obj'),('target_address',0x426e06),
                              ('profile',['/O2'])):
                invalid=deepcopy(self.units);invalid[name][key]=value
                with self.subTest(name=name,key=key),self.assertRaises(RuntimeError):
                    front.validate_title_music_native_abi(invalid)
            invalid=deepcopy(self.units);invalid[name]['relocations'].pop()
            with self.subTest(name=name,key='missing-field'),self.assertRaises(RuntimeError):
                front.validate_title_music_native_abi(invalid)
        for name in front.TITLE_MUSIC_REAL_INDEPENDENT_CALLEES:
            for key,value in (('symbol','?FakeMethod@TitleAnmManagerView@@QAEXXZ'),
                              ('source','src/Fake.cpp'),('size',777),
                              ('target_address',0x400000),('profile',['/O1'])):
                wrong=deepcopy(self.units);wrong[name][key]=value
                with self.subTest(name=name,key=key),self.assertRaises(RuntimeError):
                    front.validate_title_music_native_abi(wrong)

    def test_independent_real_callee_compilers_are_replayed_once_each(self):
        with (patch.object(front,'build') as build,
              patch.object(front,'canonical',side_effect=lambda u:{'unit':u}) as exact):
            checked=front.replay_title_music_native_callees()
        self.assertEqual([x['unit'] for x in checked],list(front.TITLE_MUSIC_REAL_INDEPENDENT_CALLEES))
        self.assertEqual(build.call_args_list,[call(u) for u in front.TITLE_MUSIC_REAL_INDEPENDENT_CALLEES])
        self.assertEqual(exact.call_args_list,[call(u) for u in front.TITLE_MUSIC_REAL_INDEPENDENT_CALLEES])

    def test_forged_draw_text_callee_rejected_before_any_exactness_credit(self):
        forged=deepcopy(self.units)
        row=next(r for r in forged['title-screen-music-room']['relocations'] if r['offset']==0x21b)
        row['symbol']='?DrawTitleMusicText@@YAXPAUTitleAnmManagerView@@PAUAnmVmView@@IIPBDZZ'
        with (patch.object(front.tomllib,'load',return_value={'units':forged}),
              patch.object(front,'build') as build,
              patch.object(front,'canonical') as canon,
              self.assertRaises(RuntimeError)):
            front.replay_title_music_native_callees()
        build.assert_not_called();canon.assert_not_called()


if __name__=='__main__':unittest.main()

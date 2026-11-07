"""Original-bound Title methods borrow the real Supervisor audio callees."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleSupervisorNativeAudioMethods(unittest.TestCase):
    def test_title_zero_storage_owner_inherits_real_audio_methods(self):
        title=(ROOT/'src/TitleScreen.cpp').read_text()
        native=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('struct TitleSupervisorView : SupervisorThreadStartView',title)
        self.assertNotIn('void PlayMusic(i32 track, i32 unused);',title)
        self.assertNotIn('int LoadMusic(i32 trackId);',title)
        self.assertNotIn('    int StopAudio();',title)
        for method in ('LoadMusic','StopAudio','PlayMusic'):
            self.assertIn('g_TitleSupervisor.'+method+'(',title)
        for signature in ('int PlayMusic(int trackId, int unused);','int LoadMusic(int trackId);','int StopAudio();'):
            self.assertIn(signature,native)

    def test_all_five_original_caller_fields_are_true_native_symbols(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        bindings=(
            ('title-screen-setup-thread',435,'supervisor-load-music'),
            ('title-screen-start-menu',179,'supervisor-play-music'),
            ('title-screen-result-name-entry',618,'supervisor-stop-audio'),
            ('title-screen-character-select-ordinary',431,'supervisor-stop-audio'),
            ('title-screen-update-screen16',177,'supervisor-stop-audio'),
        )
        for caller,offset,leafname in bindings:
            with self.subTest(caller=caller,offset=offset):
                rel=next(r for r in units[caller]['relocations'] if r['offset']==offset)
                callee=units[leafname]
                self.assertEqual((rel['symbol'],rel['target'],rel['type']),
                                 (callee['symbol'],callee['target_address'],'REL32'))
                self.assertEqual(units[caller]['source'],'src/TitleScreen.cpp')

if __name__=='__main__':unittest.main()

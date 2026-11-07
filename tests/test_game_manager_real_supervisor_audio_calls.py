"""The exact 1230-byte GameManager Update calls real Supervisor audio APIs."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class GameManagerNativeAudio(unittest.TestCase):
    def test_methods_are_native_inherited_supervisor_functions(self):
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        sup=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('struct SupervisorSetupLayout : Supervisor',source)
        self.assertNotIn('int StartStageAudio(int mode, int index);',source)
        self.assertNotIn('int FadeOutMusic(float seconds);',source)
        self.assertIn('supervisor->PlayMusic(0, 0);',source)
        self.assertIn('supervisor->FadeOutMusic(3.0f);',source)
        self.assertIn('int PlayMusic(int trackId, int unused);',sup)
        self.assertIn('int FadeOutMusic(float durationSeconds);',sup)

    def test_original_bound_calls_keep_exact_referents(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        update=u['game-manager-on-update']
        self.assertEqual((update['size'],len(update['relocations'])),(1230,87))
        for off,unit_name,size in ((0xe5,'supervisor-play-music',254),
                                   (0x1a0,'supervisor-fade-out-music',148)):
            with self.subTest(offset=off):
                method=u[unit_name]
                row=next(x for x in update['relocations'] if x['offset']==off)
                self.assertEqual(method['size'],size)
                self.assertEqual((row['symbol'],row['target'],row['type']),
                                 (method['symbol'],method['target_address'],'REL32'))

if __name__=='__main__':unittest.main()

"""GameManager Update and AddedCallback reuse the exact ScoreSupervisor leaves."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SharedScoreSupervisorTimekeeping(unittest.TestCase):
    def test_zero_extra_storage_inherited_exact_provider(self):
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        header=(ROOT/'src/ScoreSupervisorView.hpp').read_text()
        self.assertIn('#include "SupervisorThreadStartView.hpp"',source)
        self.assertIn('#include "ScoreSupervisorView.hpp"', (ROOT/'src/SupervisorStartEffectView.hpp').read_text())
        self.assertIn('struct SupervisorSetupLayout : SupervisorLoadingSetupView', (ROOT/'src/SupervisorSetupLayoutView.hpp').read_text())
        self.assertNotIn('struct SupervisorSetupLayout : Supervisor\n',source)
        self.assertIn('struct ScoreSupervisorView : Supervisor',header)
        self.assertIn('sizeof(ScoreSupervisorView) == sizeof(Supervisor)',header)
        self.assertNotIn('void UpdateGameTime();',source)
        self.assertNotIn('void UpdatePlayTime();',source)
        self.assertEqual(source.count('supervisor->UpdateGameTime();'),2)
        self.assertEqual(source.count('supervisor->UpdatePlayTime();'),1)

    def test_all_three_original_bound_calls_use_actual_native_method(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        for caller,off,callee_name in (
            ('game-manager-on-update',0x14D,'supervisor-update-game-time'),
            ('game-manager-added-callback',0xAA,'supervisor-update-game-time'),
            ('game-manager-added-callback',0xB1,'supervisor-update-play-time'),
        ):
            with self.subTest(caller=caller,off=off):
                row=next(r for r in units[caller]['relocations'] if r['offset']==off)
                callee=units[callee_name]
                self.assertEqual((row['symbol'],row['target'],row['type']),
                                 (callee['symbol'],callee['target_address'],'REL32'))
        self.assertEqual(units['game-manager-on-update']['size'],1230)
        self.assertEqual(units['game-manager-added-callback']['size'],187)

if __name__=='__main__':unittest.main()

"""Score timekeeping and SaveTitleScoreData share the native Supervisor."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class ScoreNativeSupervisorRuntime(unittest.TestCase):
    def test_single_native_supervisor_allocation(self):
        score=(ROOT/'src/ScoreData.cpp').read_text()
        native=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('struct ScoreSupervisorView : Supervisor',score)
        self.assertIn('sizeof(ScoreSupervisorView) == sizeof(Supervisor)',score)
        self.assertNotIn('u8 unknown000[0x798];',score)
        for member,offset in (('totalPlayTime','0x798'),('systemTime','0x79C'),
                              ('exeChecksum7A0','0x7A0'),('exeSize7A4','0x7A4')):
            with self.subTest(member=member):
                self.assertIn(f'offsetof(ScoreSupervisorView, {member}) == {offset}',score)
                self.assertIn(f'offsetof(Supervisor, {member}) == {offset}',native)
        self.assertIn('vrsm.exeSize = supervisor->exeSize7A4;',score)
        self.assertIn('vrsm.exeChecksum = supervisor->exeChecksum7A0;',score)

    def test_original_methods_remain_separately_bound(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        expected={
            'supervisor-update-play-time':233,
            'supervisor-update-game-time':237,
            'save-title-score-data':745,
        }
        for name,size in expected.items():
            with self.subTest(unit=name):
                self.assertEqual(units[name]['size'],size)
                self.assertEqual(units[name]['source'],'src/ScoreData.cpp')
        self.assertIn('ScoreSupervisorView',units['supervisor-update-game-time']['symbol'])

if __name__=='__main__':unittest.main()

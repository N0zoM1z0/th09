"""Recording and playback copy the same real 0xCC GameConfiguration object.

The original PE/COFF replay protects the complete 1323-byte recording exact
unit and open 540-byte playback/1238-byte SaveReplay bosses separately.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class ReplayNativeConfigSnapshot(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.game=(ROOT/'src/GameConfiguration.hpp').read_text()
        cls.wire=(ROOT/'src/ReplayDataOriginalView.hpp').read_text()
        cls.manager=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        cls.source=(ROOT/'src/ReplayInput.cpp').read_text()

    def test_wire_and_runtime_share_real_game_configuration_type(self):
        self.assertIn('struct GameConfiguration',self.game)
        self.assertIn('GameConfigurationSizeCheck[(sizeof(GameConfiguration) == 0xCC)',self.game)
        self.assertIn('GameConfiguration configSnapshot;',self.wire)
        self.assertIn('ReplayDataConfigAtDC',self.wire)
        self.assertIn('offsetof(ReplayDataView, configSnapshot) == 0xDC',self.wire)
        self.assertIn('GameConfiguration *configurationE8;',self.manager)
        self.assertIn('ReplayNativeConfigurationSnapshotIdentity',self.source)
        self.assertIn('sizeof(*((AsciiGameManagerView *)0)->configurationE8)',self.source)
        self.assertIn('sizeof(GameConfiguration) == 0xCC',self.source)

    def test_recording_and_playback_use_inverse_cxx_value_assignments(self):
        s=self.source
        recorded='replayData->configSnapshot = *g_GameManager.configurationE8;'
        restored='*g_GameManager.configurationE8 = replayManager->replayData->configSnapshot;'
        self.assertEqual(s.count(recorded),1)
        self.assertEqual(s.count(restored),1)
        self.assertLess(s.index(recorded),s.index(restored))
        self.assertNotIn('memcpy(&replayData->configSnapshot,',s)
        self.assertNotIn('memcpy(g_GameManager.configurationE8, &replayManager->replayData->configSnapshot',s)
        self.assertIn('replayManager->inputDelay = 0;',s)
        self.assertIn('g_ReplayRng.SetSeed(frame0->stageSeed);',s)

    def test_exact_recording_and_nonexact_playback_are_distinct_owners(self):
        units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        record=units['replay-manager-begin-recording-stage']
        self.assertEqual((record['source'],record['size'],len(record['relocations'])),
                         ('src/ReplayInput.cpp',1323,82))
        self.assertNotIn('replay-manager-begin-playback-stage',units)
        self.assertNotIn('replay-manager-save-replay',units)
        self.assertEqual(len([n for n,u in units.items()
                              if u['source']=='src/ReplayInput.cpp']),12)

if __name__=='__main__':
    unittest.main()

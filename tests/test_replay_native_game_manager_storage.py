"""Replay input, recording and save use the genuine shared GameManager layout.

This is a source identity check. Exactness is separately proven by pinned
TH09 1.50a PE/VC7.1 replays of both independent ReplayInput.cpp profiles.
SaveReplay remains nonexact with three actual residual bytes.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeReplayGameManagerStorage(unittest.TestCase):
    def test_recording_and_replay_read_one_game_manager(self):
        header=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        replay=(ROOT/'src/ReplayInput.cpp').read_text()
        self.assertNotIn('struct GameManagerReplayView',replay)
        self.assertNotIn('struct GameManagerReplaySideView',replay)
        self.assertIn('ReplayNativeSideAndGateOffsets',replay)
        for owner,field,offset in (
            ('AsciiGameManagerSideView','character20','0x20'),
            ('AsciiGameManagerSideView','replaySelector28','0x28'),
            ('AsciiGameManagerView','replayInputGateE8','0xE8'),
            ('AsciiGameManagerView','replayMode114','0x114'),
            ('AsciiGameManagerView','gameMode','0x118'),
            ('AsciiGameManagerView','difficulty','0x11C'),
            ('AsciiGameManagerView','replayPauseRecorded130','0x130'),
            ('AsciiGameManagerView','flags','0x134'),
        ):
            with self.subTest(field=field):
                self.assertIn('offsetof('+owner+', '+field+') == '+offset,replay)
                self.assertIn(field,header)
        self.assertIn('g_GameManager.sides[0].replaySelector28',replay)
        # The separately source-bound configuration global occupies exactly
        # GameManager+0xE8; the two replay input bytes are the same +0xB4 pair.
        self.assertIn('g_GameConfiguration->sideFocusMode',replay)
        self.assertIn('GameConfiguration *inputGate = g_GameConfiguration',replay)
        self.assertNotIn('struct ReplayInputGateView',replay)
        self.assertIn('ReplayNativeInputGateConfigModes',replay)
        self.assertIn('g_GameManager.replayPauseRecorded130',replay)
        self.assertIn('g_GameManager.sides[1].character20',replay)

    def test_full_save_and_independent_exact_replay_callees_pinned(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        pins={
            'replay-manager-begin-recording-stage':(1323,82),
            'replay-manager-load-replay-data':(591,14),
            'replay-manager-record-input':(460,25),
            'replay-manager-playback-input':(299,22),
            'replay-manager-create':(361,28),
        }
        for n,(size,fields) in pins.items():
            with self.subTest(unit=n):
                u=units[n]
                self.assertEqual((u['size'],len(u['relocations'])),(size,fields))
                self.assertEqual(u['source'],'src/ReplayInput.cpp')
        for name, offset in [('replay-manager-capture-frame-sync-state', 0xAE),
                             ('replay-manager-playback-input', 0x89)]:
            with self.subTest(native_configuration=name):
                fields = [r for r in units[name]['relocations']
                          if r['offset'] == offset]
                self.assertEqual(fields, [{
                    'offset': offset, 'type': 'DIR32',
                    'symbol': '?g_GameConfiguration@@3PAUGameConfiguration@@A',
                    'target': 0x004A7E78}])
        source=(ROOT/'src/ReplayInput.cpp').read_text()
        self.assertIn('int ReplayManagerView::SaveReplay(',source)
        self.assertIn('g_GameManager.replayMode114',source)
        self.assertIn('g_GameManager.difficulty',source)
        self.assertIn('g_GameManager.sides[0].character20',source)
        self.assertIn('g_GameManager.sides[1].character20',source)


if __name__=='__main__':
    unittest.main()

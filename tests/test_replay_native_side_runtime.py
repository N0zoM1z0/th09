"""Replay stage header fields use native GameManager side score-buffer storage."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class ReplayNativeSideRuntime(unittest.TestCase):
    def test_side_state_shape_is_native_game_side_score_buffer(self):
        source=(ROOT/'src/ReplayInput.cpp').read_text()
        native=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        self.assertNotIn('struct ReplaySideStateView\n{',source)
        self.assertIn('typedef AsciiGameSideRuntimeView ReplaySideStateView;',source)
        self.assertIn('ReplayNativeSideRuntimeFields',source)
        for field,offset in (('value00','0x00'),('value04','0x04'),
                             ('value08','0x08'),('phase10','0x10')):
            with self.subTest(field=field):
                self.assertIn('offsetof(ReplaySideStateView, '+field+') == '+offset,source)
        self.assertIn('offsetof(AsciiGameManagerSideView, runtime) == 0x1C',source)
        self.assertIn('sizeof(ReplaySideStateView) == 0xA0',source)
        self.assertIn('struct AsciiGameSideRuntimeView',native)
        self.assertIn('AsciiGameSideRuntimeView *runtime;',native)
        self.assertIn('g_ReplaySideState0->phase10',source)
        self.assertIn('g_ReplaySideState0->value04',source)
        self.assertIn('g_ReplaySideState1->value04',source)

    def test_original_replay_input_side_pointer_aliases_keep_real_addresses(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        owner=u['replay-manager-begin-recording-stage']
        self.assertEqual((owner['size'],len(owner['relocations'])),(1323,82))
        original_fields=[r for r in owner['relocations'] if 'g_ReplaySideState' in r['symbol']]
        self.assertEqual([(r['offset'],r['target']) for r in original_fields],
                         [(0x373,0x4A7DAC),(0x383,0x4A7DE4),(0x393,0x4A7DAC),
                          (0x39E,0x4A7DE4),(0x3BA,0x4A7DAC)])
        for r in original_fields:
            self.assertIn('AsciiGameSideRuntimeView@@A',r['symbol'])
            self.assertEqual(r['type'],'DIR32')
        self.assertEqual((0x4A7DAC,0x4A7DE4),
                         (0x4A7D90+0x1C,0x4A7D90+0x38+0x1C))
        self.assertIn('replay-manager-playback-input',u)


if __name__=='__main__':unittest.main()

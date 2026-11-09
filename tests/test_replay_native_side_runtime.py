"""Replay stage header fields use native GameManager side score-buffer storage."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class ReplayNativeSideRuntime(unittest.TestCase):
    def test_side_state_shape_is_native_game_side_score_buffer(self):
        source=(ROOT/'src/ReplayInput.cpp').read_text()
        native=((ROOT/'src/GameManagerRuntimeView.hpp').read_text() +
                (ROOT/'src/GameManagerSideState.hpp').read_text())
        self.assertNotIn('struct ReplaySideStateView\n{',source)
        self.assertNotIn('ReplaySideStateView',source)
        self.assertNotIn('g_ReplaySideState0',source)
        self.assertNotIn('g_ReplaySideState1',source)
        self.assertIn('ReplayNativeSideRuntimeFields',source)
        for field,offset in (('value00','0x00'),('value04','0x04'),
                             ('value08','0x08'),('phase10','0x10')):
            with self.subTest(field=field):
                self.assertIn('offsetof(AsciiGameSideRuntimeView, '+field+') == '+offset,source)
        self.assertIn('offsetof(AsciiGameManagerSideView, runtime) == 0x1C',source)
        self.assertIn('sizeof(AsciiGameSideRuntimeView) == 0xA0',source)
        self.assertIn('struct AsciiGameSideRuntimeView',native)
        self.assertIn('AsciiGameSideRuntimeView *runtime;',native)
        self.assertIn('g_GameManager.sides[0].runtime->phase10',source)
        self.assertIn('g_GameManager.sides[0].runtime->value04',source)
        self.assertIn('g_GameManager.sides[1].runtime->value04',source)

    def test_original_replay_input_side_pointer_aliases_keep_real_addresses(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        owner=u['replay-manager-begin-recording-stage']
        self.assertEqual((owner['size'],len(owner['relocations'])),(1323,82))
        original_offsets=(0x373,0x383,0x393,0x39E,0x3BA)
        original_fields=[r for r in owner['relocations'] if r['offset'] in original_offsets]
        self.assertEqual([(r['offset'],r['target'],r.get('addend',0)) for r in original_fields],
                         [(0x373,0x4A7D90,0x1C),(0x383,0x4A7D90,0x54),
                          (0x393,0x4A7D90,0x1C),(0x39E,0x4A7D90,0x54),
                          (0x3BA,0x4A7D90,0x1C)])
        for r in original_fields:
            self.assertEqual(r['symbol'],'?g_GameManager@@3UAsciiGameManagerView@@A')
            self.assertEqual(r['type'],'DIR32')
        self.assertEqual((0x4A7DAC,0x4A7DE4),
                         (0x4A7D90+0x1C,0x4A7D90+0x38+0x1C))
        self.assertIn('replay-manager-playback-input',u)
        source = (ROOT/'src/ReplayInput.cpp').read_text()
        self.assertIn('AsciiGameSideRuntimeView *volatile &side0RuntimeSlot =',source)
        self.assertIn('g_GameManager.sides[0].runtime;',source)
        self.assertNotIn('ReplaySideRuntimeSlotView',source)
        inspector=(ROOT/'scripts/inspect-replay-begin-playback.py').read_text()
        self.assertNotIn('?g_ReplaySideState',inspector)
        self.assertIn("'?g_GameManager@@3UAsciiGameManagerView@@A', 'DIR32', 0x004A7D90, 0x54",inspector)


if __name__=='__main__':unittest.main()

"""The 3238-byte selector's so-called pattern protocol is native replay input."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSelectorNativeReplayInputProtocol(unittest.TestCase):
    def test_replay_history_words_are_original_selector_protocol_storage(self):
        src=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        replay=(ROOT/'src/ReplayInputState.hpp').read_text()
        self.assertIn('#include "ReplayInputState.hpp"',src)
        self.assertNotIn('struct PlayerSideProtocolView',src)
        self.assertNotIn('extern PlayerSideProtocolView g_PlayerSideProtocols',src)
        self.assertNotIn('g_PlayerSideProtocols',src)
        self.assertIn('sizeof(ReplayInputState) == 0x8E',src)
        for field,offset in (('historyCurrent','0x2C'),
                             ('historyPressed','0x32'),
                             ('historyReleased','0x34')):
            with self.subTest(field=field):
                self.assertIn(f'offsetof(ReplayInputState, {field}) == {offset}',src)
                self.assertIn(f'{field};',replay)
                self.assertIn('g_ReplayInputStates[',src)
        self.assertIn('g_ReplayInputStates[header->player78->sideIndex].historyCurrent',src)
        self.assertIn('g_ReplayInputStates[player->sideIndex].historyReleased',src)
        self.assertIn('&g_ReplayInputStates[header->player78->sideIndex]',src)
        self.assertIn('0x58);',src)

    def test_all_28_original_bound_global_referents_are_real_input_array(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        boss=u['player-update-selector-state']
        self.assertEqual((boss['target_address'],boss['size'],len(boss['relocations'])),
                         (0x004049A0,3238,136))
        native='?g_ReplayInputStates@@3PAUReplayInputState@@A'
        before='?g_PlayerSideProtocols@?A0x68766efb@@3PAUPlayerSideProtocolView@1@A'
        rows=[r for r in boss['relocations'] if r['symbol']==native]
        self.assertEqual(len(rows),28)
        self.assertFalse(any(r['symbol']==before for r in boss['relocations']))
        for row in rows:
            self.assertEqual((row['target'],row['type'],row.get('addend',0)),
                             (0x004ACE18,'DIR32',0))
        self.assertTrue(any(r['symbol']==native and r['target']==0x004ACE18
                            for n,v in u.items() if n!='player-update-selector-state'
                            for r in v.get('relocations',[])))
        resolver=u['player-resolve-pattern-offset']
        self.assertEqual((resolver['size'],resolver.get('compare_size'),len(resolver['relocations'])),
                         (346,412,18))

if __name__=='__main__':unittest.main()

"""The original selector's 'side protocols' are actual ReplayInputState slots."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSelectorNativeReplayProtocol(unittest.TestCase):
    def test_one_true_input_array_and_native_history_words(self):
        selector=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        native=(ROOT/'src/ReplayInputState.hpp').read_text()
        self.assertIn('#include "ReplayInputState.hpp"',selector)
        self.assertNotIn('struct PlayerSideProtocolView',selector)
        self.assertNotIn('g_PlayerSideProtocols',selector)
        self.assertIn('memset(\n        &g_ReplayInputStates[header->player78->sideIndex]',selector)
        self.assertIn('g_ReplayInputStates[header->player78->sideIndex].historyCurrent',selector)
        self.assertIn('g_ReplayInputStates[header->player78->sideIndex].historyReleased',selector)
        self.assertIn('g_ReplayInputStates[player->sideIndex].historyPressed',selector)
        for name,offset in (('historyCurrent','0x2C'),
                            ('historyPressed','0x32'),
                            ('historyReleased','0x34')):
            self.assertIn(f'offsetof(ReplayInputState, {name}) == {offset}',selector)
        self.assertIn('sizeof(ReplayInputState) == 0x8E',native)
        self.assertIn('extern ReplayInputState g_ReplayInputStates[3];',native)

    def test_all_28_true_original_global_referents_remain_frozen(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        owner=units['player-update-selector-state']
        self.assertEqual((owner['target_address'],owner['size'],len(owner['relocations'])),
                         (0x004049A0,3238,136))
        native='?g_ReplayInputStates@@3PAUReplayInputState@@A'
        old='?g_PlayerSideProtocols@?A0x68766efb@@3PAUPlayerSideProtocolView@1@A'
        fields=[r for r in owner['relocations'] if r['symbol']==native]
        self.assertEqual(len(fields),28)
        self.assertFalse(any(r['symbol']==old for r in owner['relocations']))
        for r in fields:
            self.assertEqual((r['type'],r['target'],r.get('addend',0)),
                             ('DIR32',0x004ACE18,0))
        self.assertTrue(any(r['symbol']==native and r['target']==0x004ACE18
                            for n,v in units.items() if n!='player-update-selector-state'
                            for r in v.get('relocations',[])))
        resolver=units['player-resolve-pattern-offset']
        self.assertEqual((resolver['size'],resolver.get('compare_size'),len(resolver['relocations'])),
                         (346,412,18))

if __name__=='__main__':unittest.main()

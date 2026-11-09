"""The original signed block gate belongs to the constructed native Front.

Static source tests complement, but never replace, the whole-function
original PE/COFF exactness and relocation checks.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
FRONT = '?g_GameManager@@3UAsciiGameManagerView@@A'
GATE = '?IsBlocked@FrontFactoryConstructView@@QAEHXZ'


class NativeSharedPlayerRuntime(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as fd:
            cls.units = tomllib.load(fd)['units']

    def test_actual_native_front_gate_and_legacy_projection(self):
        front = (ROOT/'src/FrontFactoryConstructView.hpp').read_text()
        message = (ROOT/'src/FrontMessageRuntimeView.hpp').read_text()
        projected = (ROOT/'src/PlayerSharedRuntimeView.hpp').read_text()
        leaf = (ROOT/'src/PlayerSharedRuntimeLeaves.cpp').read_text()
        self.assertIn('int FrontFactoryConstructView::IsBlocked()', leaf)
        self.assertIn('int value = messageRuntimeE944.currentIndex08;', leaf)
        self.assertIn('if (value >= 0 || value == -2)', leaf)
        self.assertIn('offsetof(FrontFactoryConstructView, messageRuntimeE944) == 0xE944',front)
        self.assertIn('offsetof(FrontMessageRuntimeView, currentIndex08) == 0x08', message)
        self.assertIn('offsetof(FrontMessageRuntimeView, currentIndex08) == 0xE94C',leaf)
        self.assertIn('sizeof(FrontFactoryConstructView) == 0x1293C',front)
        self.assertNotIn('int IsBlocked();', projected)
        for field, offset in (('blockStateE94C','0xE94C'),
                              ('updateBlock1095C','0x1095C'),
                              ('drawCounter10960','0x10960')):
            self.assertIn('offsetof(PlayerSharedRuntimeView, '+field+') == '+offset, projected)
        self.assertIn('sizeof(PlayerSharedRuntimeView) == 0x10964', projected)

    def test_original_selector_player_replay_and_game_manager_callers(self):
        leaf = self.units['player-shared-is-blocked']
        self.assertEqual((leaf['target_address'],leaf['size'],leaf['symbol']),
                         (0x404980,24,GATE))
        cases = (('player-update-selector-state',0x42,0x67),
                 ('player-on-update',0xAC,0xB1),
                 ('player-on-update',0x104,0x109),
                 ('replay-control-frame-advance',0x13,0x18))
        for owner, ptr, call in cases:
            with self.subTest(owner=owner,offset=ptr):
                rows = {r['offset']:r for r in self.units[owner]['relocations']}
                root = rows[ptr]
                self.assertEqual((root['symbol'],root['target'],root.get('addend',0),root['type']),
                                 (FRONT,0x4A7D90,0xA8,'DIR32'))
                method = rows[call]
                self.assertEqual((method['symbol'],method['target'],method['type']),
                                 (GATE,0x404980,'REL32'))
        game = {r['offset']:r for r in self.units['game-manager-on-update']['relocations']}
        self.assertEqual((game[0x1F6]['symbol'],game[0x1F6]['target']),
                         (GATE,0x404980))
        self.assertEqual(self.units['player-update-selector-state']['size'],3238)
        self.assertEqual(self.units['player-on-update']['size'],522)


    def test_player_runtime_native_front_transition_reads(self):
        runtime = (ROOT/'src/PlayerRuntime.cpp').read_text()
        self.assertNotIn('g_PlayerSharedRuntime->', runtime)
        self.assertNotIn('#include "PlayerSharedRuntimeView.hpp"', runtime)
        self.assertEqual(runtime.count('g_GameManager.frontFactoryA8->transitionVmCount1095C'),5)
        self.assertEqual(runtime.count('g_GameManager.frontFactoryA8->transitionFrame10960'),1)
        reference = '?g_GameManager@@3UAsciiGameManagerView@@A'
        affected = {'player-on-update':(0x1AD,0x1CD),
                    'player-update-bomb-state':(0x01,),
                    'player-draw-high-prio':(0x2F,),
                    'player-spawn-reward-tail-state':(0x05,)}
        for name, offsets in affected.items():
            actual = {r['offset']:r for r in self.units[name]['relocations']}
            for offset in offsets:
                with self.subTest(unit=name, offset=offset):
                    r = actual[offset]
                    self.assertEqual((r['symbol'],r['target'],r.get('addend',0),r['type']),
                                     (reference,0x004A7D90,0xA8,'DIR32'))
        self.assertEqual(self.units['player-on-update']['size'],522)
        self.assertEqual(self.units['player-update-bomb-state']['size'],335)
        self.assertEqual(self.units['player-draw-high-prio']['size'],166)


if __name__ == '__main__':
    unittest.main()

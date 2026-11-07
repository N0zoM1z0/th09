"""Exact CardAttack large owners use native VM state and GameManager flags."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class CardAttackNativeVmAndManager(unittest.TestCase):
    def test_native_anm_vm_fields_replace_duplicate_card_view(self):
        header=(ROOT/'src/CardAttack.hpp').read_text()
        source=(ROOT/'src/CardAttack.cpp').read_text()
        self.assertIn('typedef AnmVm CardAttackVmView;',header)
        self.assertNotIn('struct CardAttackVmView\n{',header)
        self.assertIn('CardAttackVmNativeStorage',header)
        for name,offset in (('scale','0x18'),('color1','0x1F0'),
                            ('flagsWord','0x1F8'),('pos','0x208'),('pos2','0x288')):
            with self.subTest(name=name):
                self.assertIn('offsetof(CardAttackVmView, '+name+') == '+offset,header)
        self.assertIn('AnmVm mainVms0AC[2];',header)
        self.assertIn('AnmVm secondaryVms5F4[5];',header)
        self.assertIn('return vm;',source)
        self.assertIn('main->pos.x',source)
        self.assertIn('main->pos2.x',source)
        self.assertIn('main->flagsWord',source)
        self.assertNotIn('reinterpret_cast<CardAttackVmView *>(vm)',source)

    def test_card_attack_side_uses_the_one_native_game_manager_record(self):
        header=(ROOT/'src/CardAttack.hpp').read_text()
        real=(ROOT/'src/GameManagerSideState.hpp').read_text()
        source=(ROOT/'src/CardAttack.cpp').read_text()
        self.assertIn('#include "GameManagerSideState.hpp"',header)
        self.assertIn('typedef AsciiGameManagerSideView CardAttackSideStateView;',header)
        self.assertNotIn('struct CardAttackSideStateView',header)
        self.assertIn('offsetof(CardAttackSideStateView, background00) == 0x00',header)
        self.assertIn('struct AsciiGameManagerSideView',real)
        self.assertIn('CardAttackSideStateView *sideState04;',header)
        self.assertIn('cardAttack->sideState04 = &g_GameManager.sides[sideIndex];',source)

    def test_start_attack_uses_real_game_manager_transition_bitfield(self):
        native=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        start=(ROOT/'src/CardAttackStart.cpp').read_text()
        self.assertIn('unsigned int transitionBits134 : 2;',native)
        self.assertIn('AsciiGameManagerNativeCardAttackTransitionFlags',native)
        self.assertNotIn('struct CardAttackStartGameManagerView',start)
        self.assertNotIn('extern CardAttackStartGameManagerView g_GameManager;',start)
        self.assertIn('CardAttackStartNativeManagerFields',start)
        self.assertIn('&g_GameManager.valueB0)->SubtractSaturating(500);',start)
        self.assertIn('g_GameManager.transitionBits134 |=',start)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        u=units['card-attack-start']
        self.assertEqual((u['size'],len(u['relocations'])),(923,29))
        refs=[r for r in u['relocations'] if r['symbol']=='?g_GameManager@@3UAsciiGameManagerView@@A']
        self.assertEqual([(r['offset'],r['target'],r.get('addend',0)) for r in refs],
                         [(0x2B,0x4A7D90,0xB0),(0x2D4,0x4A7D90,0x134),
                          (0x2F7,0x4A7D90,0x134)])
        self.assertTrue(any(r['symbol']=='?SubtractSaturating@CardAttackStartValueCCView@@QAEXH@Z' and
                            r['target']==0x403E30 for r in u['relocations']))
        for n,size in (('card-attack-on-update',536),('card-attack-on-draw',334),
                       ('card-attack-create',264),('card-attack-constructor',66)):
            self.assertEqual(units[n]['size'],size)


if __name__=='__main__':unittest.main()

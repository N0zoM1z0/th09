"""CardAttack's exact update/draw/create owners use true GameManager ABI."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class CardAttackRealGameManagerABI(unittest.TestCase):
    def test_native_global_and_methods_no_shadow(self):
        src=(ROOT/'src/CardAttack.cpp').read_text()
        native=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        self.assertIn('#include "GameManagerRuntimeView.hpp"',src)
        self.assertNotIn('struct CardAttackGameManagerView',src)
        self.assertNotIn('extern CardAttackGameManagerView g_GameManager',src)
        self.assertIn('g_GameManager.transitionBits134',src)
        self.assertIn('g_GameManager.TransformPopupX(',src)
        self.assertIn('g_GameManager.TransformPopupY(',src)
        self.assertIn('reinterpret_cast<CardAttackSideStateView *>(&g_GameManager.sides[sideIndex])',src)
        self.assertIn('unsigned int transitionBits134 : 2;',native)

    def test_13_original_referents_are_true_global_or_exact_leaf(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertEqual(units['game-manager-transform-popup-x']['target_address'],0x00401680)
        self.assertEqual(units['game-manager-transform-popup-y']['target_address'],0x004016B0)
        changed=0
        for name in ('card-attack-create','card-attack-on-update','card-attack-on-draw'):
            for rel in units[name]['relocations']:
                if rel['symbol']=='?g_GameManager@@3UAsciiGameManagerView@@A':
                    self.assertEqual(rel['target'],0x004A7D90)
                    changed+=1
                elif rel['symbol'] in (units['game-manager-transform-popup-x']['symbol'],
                                       units['game-manager-transform-popup-y']['symbol']):
                    method=next(u for u in (units['game-manager-transform-popup-x'],
                                             units['game-manager-transform-popup-y'])
                                if rel['symbol']==u['symbol'])
                    self.assertEqual(rel['target'],method['target_address'])
                    changed+=1
        self.assertEqual(changed,13)

if __name__=='__main__':
    unittest.main()

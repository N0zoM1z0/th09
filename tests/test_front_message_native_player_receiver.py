"""Complete TH09 FrontMessage script uses a real Player allocation.

Original byte exactness is independently guarded by the full 2,968-byte
VC7.1 PE/COFF physical owner comparison (including its private switch tail).
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
CALL='?SetUpdateState@PlayerLifecycleView@@QAEXH@Z'

class FrontMessageNativePlayerReceiver(unittest.TestCase):
    def test_one_native_player_instead_of_private_front_prefix(self):
        src=(ROOT/'src/FrontMessageUpdate.cpp').read_text()
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('#include "PlayerLifecycleView.hpp"',src)
        self.assertNotIn('struct FrontMessagePlayerView',src)
        self.assertIn('PlayerLifecycleView *player04;',src)
        self.assertIn('FrontMessageNativePlayerStorage',src)
        self.assertIn('sizeof(PlayerLifecycleView) == 0x30F70',src)
        for offset in ('0x303C8','0x303F0','0x303F8','0x303FC'):
            self.assertIn(offset,src)
        self.assertIn('g_GameManager.sides[current04->argument04].player04->SetUpdateState(1);',src)
        self.assertIn('g_GameManager.sides[current04->argument04].player04->timer303C8 = 0;',src)
        self.assertIn('void SetUpdateState(int state);',native)
        self.assertIn('PlayerTimerCtorView timer303C8;',native)
        self.assertIn('PlayerLifecycleTimer303C8At303C8',native)
        self.assertNotIn('FrontMessagePlayerView',src)

    def test_original_complete_frontmessage_owner_and_true_state_call(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        boss=units['front-message-update']
        self.assertEqual(boss['size'],2852)
        self.assertEqual(boss['compare_size'],2968)
        self.assertEqual(len(boss['relocations']),167)
        calls=[r for r in boss['relocations'] if r['symbol']==CALL]
        self.assertEqual(len(calls),1)
        self.assertEqual((calls[0]['offset'],calls[0]['type'],calls[0]['target'],calls[0].get('addend',0)),
                         (0xA6,'REL32',0x00403C20,0))
        self.assertEqual(units['player-set-update-state']['target_address'],0x00403C20)
        self.assertEqual(units['player-set-update-state']['size'],12)
        self.assertEqual(units['player-set-update-state']['symbol'],CALL)
        self.assertEqual(len([r for r in boss['relocations'] if r['symbol'].startswith('$')]),30)

if __name__=='__main__':unittest.main()

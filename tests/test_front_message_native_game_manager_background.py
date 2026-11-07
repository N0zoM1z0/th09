"""Original full FrontMessage script uses real GameManager/Background owners.

Whole physical PE/COFF comparisons, not text-only assertions, prove bytes.
In particular opcode 0x1C writes a Background stage-label field at +0x18,
not the unrelated gameplay side score/phase buffer.
"""
from pathlib import Path
from collections import Counter
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
GLOBAL='?g_GameManager@@3UAsciiGameManagerView@@A'
MODE={
 'IsGameMode0':0x0040E470,
 'IsGameMode1':0x00404970,
 'IsGameMode2':0x00415D30,
}

class FrontMessageNativeGameManagerBackground(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_native_two_side_gamemgr_and_background_opcode_owner(self):
        src=(ROOT/'src/FrontMessageUpdate.cpp').read_text()
        gm=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        side=(ROOT/'src/GameManagerSideState.hpp').read_text()
        bg=(ROOT/'src/Background.hpp').read_text()
        self.assertIn('#include "GameManagerRuntimeView.hpp"',src)
        self.assertIn('#include "Background.hpp"',src)
        self.assertIn('extern AsciiGameManagerView g_GameManager;',src)
        self.assertIn('AsciiGameManagerSideView *side = &g_GameManager.sides[1];',src)
        for obsolete in ('struct FrontMessageGameManagerView',
                         'struct FrontMessageSideView',
                         'struct FrontMessageSideStateView',
                         'state00->value18'):
            self.assertNotIn(obsolete,src)
        self.assertIn('AsciiGameManagerSideView sides[2];',gm)
        self.assertIn('Background *background00;',side)
        self.assertIn('PlayerLifecycleView *player04;',side)
        self.assertIn('FrontSide *frontSide18;',side)
        self.assertIn('int pendingStageLabel18;',bg)
        self.assertIn('FrontMessageNativeGameManagerOwners',src)
        for phrase in ('sizeof(AsciiGameManagerView) == 0x390',
                       'sizeof(AsciiGameManagerSideView) == 0x38',
                       'offsetof(AsciiGameManagerSideView, background00) == 0x00',
                       'offsetof(Background, pendingStageLabel18) == 0x18',
                       'offsetof(AsciiGameManagerView, gameMode) == 0x118'):
            self.assertIn(phrase,src)
        for i in (0,1):
            self.assertIn('g_GameManager.sides[%d].background00->pendingStageLabel18 ='%i,src)
        self.assertIn('case 0x1C:',src)
        self.assertIn('current04->argument04;',src)

    def test_original_global_37_bound_fields_and_real_game_mode_targets(self):
        u=self.units['front-message-update']
        self.assertEqual((u['target_address'],u['size'],u['compare_size'],len(u['relocations'])),
                         (0x00416590,2852,2968,167))
        gm=[r for r in u['relocations'] if r['symbol']==GLOBAL]
        self.assertEqual(len(gm),37)
        self.assertTrue(all(r['type']=='DIR32' and r['target']==0x004A7D90 for r in gm))
        self.assertTrue(all(r.get('addend',0) in (0,4,0x18,0x38,0x3C,0x50) for r in gm))
        for name,target in MODE.items():
            with self.subTest(name=name):
                expected='?%s@AsciiGameManagerView@@QAEHXZ'%name
                rows=[r for r in u['relocations'] if r['symbol']==expected]
                self.assertEqual(len(rows),1)
                self.assertEqual((rows[0]['type'],rows[0]['target'],rows[0].get('addend',0)),
                                 ('REL32',target,0))
        self.assertEqual(len([r for r in u['relocations'] if r['symbol'].startswith('$')]),30)
        self.assertTrue(any(r['symbol']=='?SetUpdateState@PlayerLifecycleView@@QAEXH@Z'
                            and r['target']==0x00403C20 for r in u['relocations']))

    def test_independent_exact_background_consumer_and_mode_leaves(self):
        path=(ROOT/'src/BackgroundPendingStageLabel.cpp').read_text()
        self.assertIn('int pending = this->pendingStageLabel18;',path)
        self.assertIn('this->pendingStageLabel18 = 0;',path)
        bg=self.units['background-pending-stage-label']
        self.assertEqual((bg['target_address'],bg['size']), (0x004018A0,79))
        for name,address in MODE.items():
            with self.subTest(name=name):
                method=self.units['game-manager-is-game-mode-'+name[-1]]
                self.assertEqual(method['target_address'],address)
                self.assertEqual(method['symbol'],'?%s@AsciiGameManagerView@@QAEHXZ'%name)

if __name__=='__main__':unittest.main()

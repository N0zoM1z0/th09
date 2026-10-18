"""TH09 Type3/Type4 share the proven g_GameManager + inverse popup ABI.

These tests do not credit byte equivalence: full 780-byte Type3 PE/COFF and
368-byte exact Type4 plus genuine coordinate-transform carrier are rebuilt
by check-boss-frontiers.py with pinned original-target comparisons.
"""
import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sp = importlib.util.spec_from_file_location('real_type3_game_manager', ROOT/'scripts/check-boss-frontiers.py')
BOSS = importlib.util.module_from_spec(sp)
sp.loader.exec_module(BOSS)

REAL_GLOBAL = '?g_GameManager@@3UAsciiGameManagerView@@A'
REAL_X = '?InverseTransformPopupX@AsciiGameManagerView@@QAEMM@Z'
REAL_Y = '?InverseTransformPopupY@AsciiGameManagerView@@QAEMM@Z'


class SharedOriginalGameManagerOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.type3 = (ROOT/'src/ExAttackUpdateType3.cpp').read_text()
        cls.type4 = (ROOT/'src/ExAttackInitType4.cpp').read_text()
        cls.gm = (ROOT/'src/AsciiGameManagerView.hpp').read_text()
        cls.methods = (ROOT/'src/GameManagerCoordinates.cpp').read_text()
        cls.bind_type3 = (ROOT/'scripts/inspect-exattack-type3.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units = tomllib.load(f)['units']

    def test_two_large_callbacks_share_one_real_native_global(self):
        self.assertIn('extern AsciiGameManagerView g_GameManager;',self.gm)
        self.assertEqual(self.type3.count('#include "AsciiGameManagerView.hpp"'),1)
        self.assertEqual(self.type4.count('#include "AsciiGameManagerView.hpp"'),1)
        for body in (self.type3,self.type4):
            self.assertNotIn('extern ExAttackType3UpdateGameManagerView g_GameManager;',body)
            self.assertNotIn('extern ExAttackType4GameManagerView g_GameManager;',body)
            self.assertNotIn('struct ExAttackType3UpdateGameManagerView',body)
            self.assertNotIn('struct ExAttackType4GameManagerView',body)
            self.assertIn('g_GameManager.InverseTransformPopupX(',body)
            self.assertIn('g_GameManager.InverseTransformPopupY(',body)
        self.assertIn('float AsciiGameManagerView::InverseTransformPopupX(',self.methods)
        self.assertIn('float AsciiGameManagerView::InverseTransformPopupY(',self.methods)

    def test_real_side_offsets_and_casts_preserve_observed_player_ABI(self):
        self.assertIn('(offsetof(AsciiGameManagerSideView, player) == 0x04)',self.gm)
        self.assertIn('(sizeof(AsciiGameManagerSideView) == 0x38)',self.gm)
        self.assertEqual(self.type3.count(
            'reinterpret_cast<PlayerLifecycleView *>(g_GameManager.sides[record->opponentSide04].player)->CheckBulletCollision('),3)
        self.assertEqual(self.type4.count(
            'reinterpret_cast<ExAttackType4PlayerView *>(g_GameManager.sides[record->side08].player)->anmFileBC'),5)
        self.assertIn('offsetof(ExAttackType4PlayerView, anmFileBC) == 0xBC',self.type4)
        self.assertNotIn('player04->CheckBulletCollision',self.type3)
        self.assertNotIn('player04->anmFileBC',self.type4)

    def test_type3_original_38_fields_bind_true_not_fabricated_global(self):
        for symbol,va in ((REAL_GLOBAL,'0x004A7D90'),
                          (REAL_X,'0x00442420'),(REAL_Y,'0x00442450')):
            with self.subTest(symbol=symbol):
                self.assertIn('"'+symbol+'": '+va,self.bind_type3)
        self.assertNotIn('ExAttackType3UpdateGameManagerView@@',self.bind_type3)
        self.assertIn('diagnostic.SIZE = 780',self.bind_type3)
        self.assertIn('len(fields) != 38',self.bind_type3)

    def test_exact_type4_original_manifest_has_true_global_and_callees(self):
        unit=self.units['exattack-type4-initialize']
        self.assertEqual((unit['source'],unit['size'],len(unit['relocations']),unit['target_address']),
                         ('src/ExAttackInitType4.cpp',368,22,0x442A60))
        reloc={r['offset']:r for r in unit['relocations']}
        self.assertEqual(len(reloc),22)
        for off in (0x2E,0x48,0x6B,0x8E,0xB1,0xF8,0x108):
            self.assertEqual((reloc[off]['symbol'],reloc[off]['target'],reloc[off]['type']),
                             (REAL_GLOBAL,0x4A7D90,'DIR32'))
        for off,symbol,va in ((0xFD,REAL_X,0x442420),(0x10D,REAL_Y,0x442450)):
            self.assertEqual((reloc[off]['symbol'],reloc[off]['target'],reloc[off]['type']),
                             (symbol,va,'REL32'))
        self.assertNotIn('ExAttackType4GameManagerView@@',str(reloc))

    def test_coordinate_callee_carrier_replays_all_four_original_leaves(self):
        groups = BOSS.EXATTACK_TYPE3_REAL_GAME_MANAGER_CARRIERS
        self.assertEqual([len(row[2]) for row in groups],[4,1])
        self.assertEqual(len(groups),2)
        self.assertEqual(groups[0][1],'build/matching/GameManagerCoordinates.obj')
        self.assertEqual(groups[1][1],'build/matching/ExAttackInitType4.obj')
        for source, obj, group in groups:
            for name,va,symbol,size in group:
                unit = self.units[name]
                with self.subTest(name=name):
                    self.assertEqual((unit['source'],unit['object'],unit['target_address'],
                                      unit['symbol'],unit['size'],unit['profile']),
                                     (source,obj,va,symbol,size,
                                      list(BOSS.EXATTACK_TYPE3_GAME_MANAGER_VC71_PROFILE)))
        self.assertEqual([r[1] for r in groups[0][2]],
                         [0x401680,0x4016B0,0x442420,0x442450])

    def test_cold_exact_callees_build_once_per_true_source_carrier(self):
        with patch.object(BOSS,'build') as build,patch.object(
            BOSS,'canonical',side_effect=lambda n:{'unit':n}) as exact:
            result=BOSS.replay_exattack_type3_real_game_manager_exact()
        self.assertEqual(len(result),5)
        self.assertEqual(len({x['unit'] for x in result}),5)
        self.assertEqual([b.args[0] for b in build.call_args_list],
                         ['game-manager-transform-popup-x','exattack-type4-initialize'])
        self.assertEqual(exact.call_count,5)

    def test_incorrect_original_address_C_plus_plus_symbol_or_profile_fail(self):
        for name,field,value in (
            ('game-manager-inverse-transform-popup-x','symbol',
             '?InverseTransformPopupX@ExAttackType3UpdateGameManagerView@@QAEMM@Z'),
            ('game-manager-inverse-transform-popup-y','target_address',0x442420),
            ('game-manager-transform-popup-x','object','build/matching/Different.obj'),
            ('game-manager-transform-popup-y','profile',['/Ob0']),
            ('exattack-type4-initialize','source','src/Different.cpp'),
            ('exattack-type4-initialize','target_address',0x442A61),
        ):
            tampered=copy.deepcopy(self.units)
            tampered[name][field]=value
            with self.subTest(name=name,field=field),patch.object(
                BOSS.tomllib,'load',return_value={'units':tampered}),patch.object(
                BOSS,'build'),patch.object(BOSS,'canonical'),self.assertRaises(RuntimeError):
                BOSS.replay_exattack_type3_real_game_manager_exact()


if __name__=='__main__':
    unittest.main()

"""Game-specific exact 23-byte PlayerOwnerState constructor: two ZunTimers.

Original PE/COFF full 23-byte byte equality is verified independently by
pinned VC7.1 replay; this guards original targets, extent, and caller ABI.
"""
from pathlib import Path
import csv
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerOwnerStateConstructorExact(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as fp:
            cls.units=tomllib.load(fp)['units']

    def test_exact_23_bytes_two_original_timer_constructors(self):
        ctor=self.units['player-owner-state-constructor']
        self.assertEqual((ctor['target_address'],ctor['size'],len(ctor['relocations'])),
                         (0x0041EE30,23,2))
        self.assertEqual(ctor['symbol'],'??0PlayerOwnerStateView@@QAE@XZ')
        self.assertEqual(ctor['source'],'src/PlayerOwnerStateConstructor.cpp')
        self.assertIn('/Ob0',ctor['profile'])
        self.assertEqual([(r['offset'],r['type'],r['symbol'],r['target'])
                          for r in ctor['relocations']],
                         [(off,'REL32','??0ZunTimer@@QAE@XZ',0x00403390)
                          for off in (0x07,0x0F)])
        self.assertEqual(self.units['zun-timer-constructor']['size'],12)
        self.assertEqual(self.units['zun-timer-constructor']['target_address'],0x00403390)

    def test_true_0x44_byte_owner_alloc_and_original_player_constructor_call(self):
        source=(ROOT/'src/PlayerOwnerStateConstructor.cpp').read_text()
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('PlayerOwnerStateView::PlayerOwnerStateView()',source)
        self.assertIn('ZunTimer timer14;',header)
        self.assertIn('ZunTimer timer20;',header)
        self.assertIn('PlayerOwnerStateNativeFullMembers',header)
        self.assertIn('offsetof(PlayerOwnerStateView, timer14) == 0x14',header)
        self.assertIn('offsetof(PlayerOwnerStateView, timer20) == 0x20',header)
        self.assertIn('PlayerOwnerStateSizeIs44',header)
        self.assertNotIn('asm',source)
        self.assertNotIn('volatile',source)
        player=self.units['player-constructor']
        self.assertEqual((player['target_address'],player['size']),(0x0041F070,243))
        calls=[r for r in player['relocations']
               if r['symbol']=='??0PlayerOwnerStateView@@QAE@XZ']
        self.assertEqual([(r['offset'],r['type'],r['target']) for r in calls],
                         [(0xD3,'REL32',0x0041EE30)])
        self.assertEqual(self.units['player-owner-apply-reward']['compare_size'],1540)
        self.assertEqual(self.units['player-added-owner-reset']['size'],41)

    def test_mapped_game_specific_ctor_with_unknown_original_explicitness(self):
        with (ROOT/'config/functions.csv').open() as fd:
            rows={r['address']:r for r in csv.DictReader(fd)}
        row=rows['0x0041EE30']
        self.assertEqual(row['status'],'matching')
        self.assertEqual(row['match_percent'],'100.00')
        self.assertEqual(row['source_file'],'src/PlayerOwnerStateConstructor.cpp')
        self.assertIn('implicit special-member C++ authorship is unknown',row['notes'])
        with (ROOT/'config/function-origins.csv').open() as fd:
            origins={r['address']:r for r in csv.DictReader(fd)}
        self.assertEqual(origins['0x0041EE30']['disposition'],'authored')
        self.assertEqual(origins['0x0041EE30']['subsystem'],'Player')
        self.assertIn('PlayerOwnerStateView::PlayerOwnerStateView',
                      (ROOT/'config/implemented.csv').read_text())
        self.assertIn('player-owner-state-constructor',(ROOT/'config/matches.csv').read_text())
        self.assertIn('PlayerOwnerStateView::PlayerOwnerStateView,0x0041EE30,function',
                      (ROOT/'config/reccmp-functions.csv').read_text())

if __name__=='__main__':unittest.main()

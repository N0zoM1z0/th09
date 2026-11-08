"""Independent pinned VC7.1 source-cold 28-byte native Player Header ctor.

This class tests the original reviewed extent, three original addresses,
compiler profile and cross-function caller. Whole PE/COFF comparator owns
byte equality; explicit versus implicit retail C++ authorship is unknown.
"""
from pathlib import Path
import csv
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class OriginalPlayerHeaderConstructor(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as fp:
            cls.units=tomllib.load(fp)['units']

    def test_exact_full_original_28_bytes_and_three_timer_rel32_fields(self):
        u=self.units['player-header-constructor']
        self.assertEqual((u['target_address'],u['size'],u['source']),
                         (0x0041EE00,28,'src/PlayerHeaderConstructor.cpp'))
        self.assertEqual(u['symbol'],'??0PlayerHeaderStateView@@QAE@XZ')
        self.assertEqual(u['functions'],['PlayerHeaderStateView::PlayerHeaderStateView'])
        self.assertEqual(u['profile'].count('/Ob0'),1)
        self.assertNotIn('/Ob1',u['profile'])
        self.assertEqual([(r['offset'],r['type'],r['symbol'],r['target'],r.get('addend',0))
                          for r in u['relocations']],
                         [(off,'REL32','??0ZunTimer@@QAE@XZ',0x00403390,0)
                          for off in (0x04,0x0C,0x14)])
        self.assertEqual(self.units['zun-timer-constructor']['target_address'],0x00403390)
        self.assertEqual(self.units['zun-timer-constructor']['size'],12)

    def test_native_empty_special_member_and_243_byte_player_caller(self):
        source=(ROOT/'src/PlayerHeaderConstructor.cpp').read_text()
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('PlayerHeaderStateView::PlayerHeaderStateView()',source)
        self.assertIn('ZunTimer timer00;',header)
        self.assertIn('ZunTimer timer5C;',header)
        self.assertIn('ZunTimer timer68;',header)
        self.assertIn('PlayerHeaderNativeConstructedTimers',header)
        self.assertNotIn('asm',source)
        self.assertNotIn('volatile',source)
        player=self.units['player-constructor']
        self.assertEqual((player['target_address'],player['size']), (0x0041F070,243))
        caller=[r for r in player['relocations']
                if r['symbol']=='??0PlayerHeaderStateView@@QAE@XZ']
        self.assertEqual([(r['offset'],r['type'],r['target']) for r in caller],
                         [(0x07,'REL32',0x0041EE00)])

    def test_exact_ledger_allows_reproducing_without_claiming_original_spelling(self):
        with (ROOT/'config/functions.csv').open() as f:
            functions={r['address']:r for r in csv.DictReader(f)}
        row=functions['0x0041EE00']
        self.assertEqual(row['source_file'],'src/PlayerHeaderConstructor.cpp')
        self.assertEqual(row['status'],'matching')
        self.assertEqual(row['match_percent'],'100.00')
        self.assertEqual(row['calling_convention'],'__thiscall')
        self.assertIn('compiler-generated implicitly remains unknown',row['notes'])
        with (ROOT/'config/function-origins.csv').open() as f:
            origins={r['address']:r for r in csv.DictReader(f)}
        self.assertEqual(origins['0x0041EE00']['disposition'],'authored')
        self.assertEqual(origins['0x0041EE00']['subsystem'],'Player')
        self.assertIn('PlayerHeaderStateView::PlayerHeaderStateView',
                      (ROOT/'config/implemented.csv').read_text())
        self.assertIn('player-header-constructor',
                      (ROOT/'config/matches.csv').read_text())
        self.assertIn('PlayerHeaderStateView::PlayerHeaderStateView,0x0041EE00,function',
                      (ROOT/'config/reccmp-functions.csv').read_text())

if __name__ == '__main__':unittest.main()

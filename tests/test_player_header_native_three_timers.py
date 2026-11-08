"""Player Header's physical 0x9C bytes have 3 native ZunTimer members.

Original independent pinned VC7.1 constructor probe demonstrates exact
28 bytes / 3 ZunTimer constructor calls. Actual source-cold PE/COFF
replays, not these assertions, establish complete original exactness.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]

class PlayerHeaderNativeThreeTimers(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_genuine_construction_storage_and_no_duplicate_reset_view(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        added=(ROOT/'src/PlayerAddedState.cpp').read_text()
        runtime=(ROOT/'src/PlayerRuntime.cpp').read_text()
        self.assertIn('struct PlayerHeaderStateView',header)
        self.assertIn('PlayerHeaderStateSizeIs9C',header)
        self.assertIn('PlayerHeaderNativeConstructedTimers',header)
        self.assertIn('ZunTimer timer00;',header)
        self.assertIn('ZunTimer timer5C;',header)
        self.assertIn('ZunTimer timer68;',header)
        self.assertIn('unsigned int flags74;',header)
        self.assertNotIn('unsigned char unknown00[0x78];',header)
        for name,offset in (('timer00','0x00'),('timer5C','0x5C'),
                            ('timer68','0x68'),('flags74','0x74'),
                            ('selfLink78','0x78')):
            with self.subTest(name=name):
                self.assertIn('offsetof(PlayerHeaderStateView, '+name+') == '+offset,header)
        self.assertIn('void Reset();',header)
        self.assertIn('void PlayerHeaderStateView::Reset()',added)
        self.assertIn('this->header24.Reset();',added)
        self.assertNotIn('PlayerAddedHeaderResetView',added)
        self.assertIn('this->timer5C = 0;',added)
        self.assertIn('this->timer68 = 0;',added)
        self.assertIn('this->flags74 &= ~3u;',added)
        self.assertIn('&opponent->header24.timer5C',runtime)
        self.assertNotIn('header24.unknown00[0x5C]',runtime)
        self.assertIn('offsetof(PlayerHeaderStateView, timer5C) == 0x5C',runtime)
        self.assertIn('offsetof(PlayerLifecycleView, header24) == 0x24',runtime)

    def test_complete_original_29_byte_reset_572_byte_initializer_and_constructor(self):
        reset=self.units['player-added-header-reset']
        self.assertEqual((reset['target_address'],reset['size'],len(reset['relocations'])),
                         (0x0041BDC0,29,2))
        self.assertEqual(reset['symbol'],'?Reset@PlayerHeaderStateView@@QAEXXZ')
        self.assertTrue(all(r['symbol']=='??4ZunTimer@@QAEXH@Z' and r['target']==0x00401500
                            for r in reset['relocations']))
        caller=self.units['player-initialize-added-state']
        self.assertEqual((caller['target_address'],caller['size'],caller['compare_size'],len(caller['relocations'])),
                         (0x0041EBC0,552,572,25))
        calls=[r for r in caller['relocations'] if r['symbol']==reset['symbol']]
        self.assertEqual([(r['offset'],r['target']) for r in calls],[(0x20E,0x0041BDC0)])
        player=self.units['player-constructor']
        self.assertEqual((player['target_address'],player['size']), (0x0041F070,243))
        self.assertTrue(any(r['symbol']=='??0PlayerHeaderStateView@@QAE@XZ'
                            and r['target']==0x0041EE00 for r in player['relocations']))
        header=self.units['player-header-constructor']
        self.assertEqual((header['target_address'],header['size'],len(header['relocations'])),
                         (0x0041EE00,28,3))
        self.assertEqual(header['symbol'],'??0PlayerHeaderStateView@@QAE@XZ')
        self.assertEqual(header['source'],'src/PlayerHeaderConstructor.cpp')

if __name__ == '__main__':unittest.main()

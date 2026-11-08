"""Full original 12-byte Player+0x3031C state constructor, native timer.

Original PE/COFF source-cold Oracle separately proves the full bytes and
relocation, while this checks size, caller and independent shot pointer.
"""
from pathlib import Path
import csv
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerState3031CConstructorExact(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as fp:
            cls.units=tomllib.load(fp)['units']

    def test_original_twelve_bytes_and_one_genuine_timer_ctor_call(self):
        u=self.units['player-state3031c-constructor']
        self.assertEqual((u['target_address'],u['size'],u['source']),
                         (0x0041EE20,12,'src/PlayerState3031CConstructor.cpp'))
        self.assertEqual(u['symbol'],'??0PlayerState3031CView@@QAE@XZ')
        self.assertIn('/Ob0',u['profile'])
        self.assertEqual([(r['offset'],r['type'],r['symbol'],r['target'],r.get('addend',0))
                          for r in u['relocations']],
                         [(0x04,'REL32','??0ZunTimer@@QAE@XZ',0x00403390,0)])
        self.assertEqual(self.units['zun-timer-constructor']['target_address'],0x00403390)
        self.assertEqual(self.units['zun-timer-constructor']['size'],12)

    def test_real_player_state_layout_and_independent_shot_writer(self):
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source=(ROOT/'src/PlayerState3031CConstructor.cpp').read_text()
        shot=(ROOT/'src/PlayerShotSpawnCallback1.cpp').read_text()
        runtime=(ROOT/'src/PlayerRuntime.cpp').read_text()
        self.assertIn('PlayerState3031CView::PlayerState3031CView()',source)
        self.assertNotIn('asm',source)
        self.assertNotIn('volatile',source)
        self.assertIn('PlayerState3031CSizeIs1C',native)
        self.assertIn('ZunTimer timer00;',native)
        self.assertIn('PlayerShotCtorView *shot0C;',native)
        self.assertNotIn('unsigned char unknown00[0x10];',
                         native.split('struct PlayerState3031CView',1)[1].split('};',1)[0])
        for member,offset in (('timer00','0x00'),('shot0C','0x0C'),
                              ('stateWord10','0x10'),('transitionCounter18','0x18')):
            self.assertIn('offsetof(PlayerState3031CView, '+member+') == '+offset,native)
        self.assertIn('PlayerState3031CView state3031C;',native)
        self.assertIn('player->state3031C.timer00 = 30;',shot)
        self.assertIn('player->state3031C.shot0C = reinterpret_cast<PlayerShotCtorView *>(shot);',shot)
        self.assertIn('PlayerState3031CView state3031C;',shot)
        self.assertNotIn('PlayerShotSpawn1StateView',shot)
        self.assertIn('player->state3031C.transitionCounter18 = 60;',runtime)
        self.assertIn('player->state3031C.stateWord10 = 0;',runtime)

    def test_original_full_player_ctor_and_exact_shot_spawn(self):
        outer=self.units['player-constructor']
        self.assertEqual((outer['target_address'],outer['size']),(0x0041F070,243))
        call=[r for r in outer['relocations']
              if r['symbol']=='??0PlayerState3031CView@@QAE@XZ']
        self.assertEqual([(r['offset'],r['type'],r['target']) for r in call],
                         [(0x7B,'REL32',0x0041EE20)])
        shot=self.units['player-shot-spawn-callback-type1']
        self.assertEqual(shot['size'],151)
        self.assertEqual(shot['source'],'src/PlayerShotSpawnCallback1.cpp')
        with (ROOT/'config/functions.csv').open() as fd:
            mapping={r['address']:r for r in csv.DictReader(fd)}
        row=mapping['0x0041EE20']
        self.assertEqual((row['source_file'],row['status'],row['match_percent']),
                         ('src/PlayerState3031CConstructor.cpp','matching','100.00'))
        self.assertIn('special-member authorship remains unknown',row['notes'])
        with (ROOT/'config/function-origins.csv').open() as fd:
            origins={r['address']:r for r in csv.DictReader(fd)}
        self.assertEqual(origins['0x0041EE20']['disposition'],'authored')
        self.assertIn('PlayerState3031CView::PlayerState3031CView',
                      (ROOT/'config/implemented.csv').read_text())
        self.assertIn('player-state3031c-constructor',
                      (ROOT/'config/matches.csv').read_text())
        self.assertIn('PlayerState3031CView::PlayerState3031CView,0x0041EE20,function',
                      (ROOT/'config/reccmp-functions.csv').read_text())

if __name__=='__main__':unittest.main()

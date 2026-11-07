"""The independently exact Title interpolation methods use true ZunTimer words."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleNativeInterpolationTimers(unittest.TestCase):
    def test_native_timer_words_and_original_inline_data_flow(self):
        title=(ROOT/'src/TitleScreen.cpp').read_text()
        native=(ROOT/'src/ZunTimer.hpp').read_text()
        self.assertNotIn('struct TitleZunTimerView',title)
        self.assertIn('static __forceinline void AssignInterpolatedZunTimer(ZunTimer &timer, int current)',title)
        for statement in ('timer.current = current;',
                          'timer.subFrame = (float)current;',
                          'timer.previous = -999999;'):
            self.assertIn(statement,title)
        for row,value in (('interpCurrentTimers[0]','0'),('interpEndTimers[0]','duration'),
                          ('interpCurrentTimers[1]','0'),('interpEndTimers[1]','duration')):
            self.assertIn(f'AssignInterpolatedZunTimer({row}, {value});',title)
        self.assertIn('int previous;',native)
        self.assertIn('float subFrame;',native)
        self.assertIn('int current;',native)
        self.assertIn('sizeof(ZunTimer) == 0x0C',native)

    def test_original_bound_method_extents_stay_exact(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        for name,size in (('anm-vm-configure-position-interpolation',81),
                          ('anm-vm-configure-color-interpolation',117)):
            with self.subTest(name=name):
                record=units[name]
                self.assertEqual((record['size'],len(record['relocations'])),(size,0))
                self.assertEqual(record['source'],'src/TitleScreen.cpp')

if __name__=='__main__':unittest.main()

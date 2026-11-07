"""The 0x2A4-byte Title VM now shares native AnmVm storage and preserves methods."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleNativeAnmVmInheritance(unittest.TestCase):
    def test_no_second_vm_allocation_or_color_snapshot(self):
        title=(ROOT/'src/TitleScreen.cpp').read_text()
        native=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('struct AnmVmView : AnmVm',title)
        self.assertIn('sizeof(AnmVmView) == sizeof(AnmVm)',title)
        self.assertNotIn('u8 unknown000[0x50];',title)
        self.assertNotIn('struct TitleColorView',title)
        self.assertIn('AnmVmView embeddedVms[57];',title)
        for field,offset in (('pos','0x208'),('color1Initial','0x278'),
                             ('color1Final','0x27C'),('interpCurrentTimers','0x050'),
                             ('interpEndTimers','0x0A4'),('fontHeight','0x299')):
            self.assertIn('offsetof(AnmVmView, '+field+') == '+offset,title)
        self.assertIn('unsigned char color1Bytes[4];',native)
        self.assertIn('unsigned short flagsLow; unsigned short flagsHigh;',native)
        self.assertIn('marker->pos.x',title)

    def test_two_true_original_interpolation_method_bodies(self):
        title=(ROOT/'src/TitleScreen.cpp').read_text()
        self.assertIn('AnmVmView::ConfigurePositionInterpolation(',title)
        self.assertIn('AnmVmView::ConfigureColorInterpolation(',title)
        self.assertIn('reinterpret_cast<TitleZunTimerView *>(&interpCurrentTimers[0])',title)
        self.assertIn('reinterpret_cast<TitleFloat3View *>(&posInitial)',title)
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        self.assertEqual(u['anm-vm-configure-position-interpolation']['size'],81)
        self.assertEqual(u['anm-vm-configure-color-interpolation']['size'],117)
        self.assertEqual(u['title-screen-setup-thread']['size'],534)
        self.assertEqual(u['title-screen-start-menu']['size'],1560)

if __name__=='__main__': unittest.main()

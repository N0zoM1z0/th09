"""The exact FrontSide HUD calls native ZunTimer getters and modulo leaves."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class FrontSideRealZunTimer(unittest.TestCase):
    def test_no_private_timer_method_projection(self):
        source=(ROOT/'src/FrontSide.cpp').read_text()
        self.assertNotIn('FrontSideTimerModuloView',source)
        self.assertIn('FrontSideNativeTimerRoles',source)
        self.assertIn('static_cast<int>(pulsePlayer->pulseTimer30430)',source)
        self.assertEqual(source.count('(frontSide->updateTimerABDC % 2)'),4)
        self.assertIn('offsetof(FrontSidePlayerRuntimeView, pulseTimer30430) == 0x30430',source)
        self.assertIn('offsetof(FrontSide, updateTimerABDC) == 0xABDC',source)

    def test_original_call_fields_target_independently_exact_real_methods(self):
        with (ROOT/'config/match-units.toml').open('rb') as file:
            units=tomllib.load(file)['units']
        owner=units['front-side-on-update']
        self.assertEqual(owner['size'],2381)
        refs={r['offset']:r for r in owner['relocations']}
        getter=units['zun-timer-current-value-alias']
        modulo=units['zun-timer-modulo']
        self.assertEqual(getter['target_address'],0x00435F00)
        self.assertEqual(modulo['target_address'],0x0040F830)
        self.assertEqual((refs[0x133]['symbol'],refs[0x133]['target']),
                         (getter['symbol'],getter['target_address']))
        for off in (0x4C4,0x500,0x536,0x56C):
            self.assertEqual((refs[off]['symbol'],refs[off]['target']),
                             (modulo['symbol'],modulo['target_address']))

if __name__=='__main__':
    unittest.main()

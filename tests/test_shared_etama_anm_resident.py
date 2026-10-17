"""PE-linked +0x04 AnmLoaded state shared by Etama/Effect callbacks.

These target-independent guards reject inventing two unrelated resource
layouts, redirecting compiler-private switch labels, or silently giving the
still-different Effect callback new exact credit.
"""

from pathlib import Path
import csv
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class SharedEtamaResidentLayout(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header = (ROOT / 'src/AnmLoadedResidentView.hpp').read_text()
        cls.etama = (ROOT / 'src/EtamaControllerDraw.cpp').read_text()
        cls.effect = (ROOT / 'src/EffectManager.cpp').read_text()
        with (ROOT / 'config/match-units.toml').open('rb') as file:
            cls.units = tomllib.load(file)['units']

    def test_real_eight_byte_prefix_and_loaded_pointer_offset(self):
        self.assertIn('struct AnmLoadedResidentView', self.header)
        self.assertIn('unsigned char unknown00[4];', self.header)
        self.assertIn('void *resident04;', self.header)
        self.assertIn('sizeof(AnmLoadedResidentView) == 0x08', self.header)
        self.assertIn('offsetof(AnmLoadedResidentView, resident04) == 0x04',
                      self.header)
        self.assertIn('original game reads', self.header.lower())
        # No guessed tail: this source view deliberately knows only the
        # +0x04 slot, not the stripped AnmLoaded retail class declaration.
        self.assertEqual(self.header.count('void *resident04;'), 1)

    def test_exact_etama_and_open_effect_use_the_same_target_read(self):
        for label, source in (('Etama', self.etama), ('Effect', self.effect)):
            with self.subTest(label=label):
                self.assertEqual(source.count('#include "AnmLoadedResidentView.hpp"'), 1)
                self.assertEqual(source.count('->resident04'), 1)
                self.assertIn('->PreloadAnm(8,', source)
                self.assertIn('->GetAnm(8)', source)
                self.assertIn('"etama.anm"', source)
        self.assertNotIn('EtamaAnmLoadedStateView', self.etama)
        self.assertNotIn('reinterpret_cast<void **>(effectAnm)[1]', self.effect)
        self.assertIn('reinterpret_cast<AnmLoadedResidentView *>(anm)->resident04',
                      self.etama)
        self.assertIn('reinterpret_cast<AnmLoadedResidentView *>(effectAnm)->resident04',
                      self.effect)
        self.assertIn('controller->bulletAnm =', self.etama)
        self.assertIn('manager->effectAnm = effectAnm;', self.effect)

    def test_real_callers_retain_proven_original_addresses_and_sizes(self):
        target = {
            'bullet-draw-single': (0x00413AF0, 232, 'src/EtamaControllerDraw.cpp'),
            'etama-controller-added-callback': (
                0x00413E40, 832, 'src/EtamaControllerDraw.cpp'),
            'effect-manager-update': (0x0040CDD0, 475, 'src/EffectManager.cpp'),
        }
        for unit, (address, size, source) in target.items():
            with self.subTest(unit=unit):
                spec = self.units[unit]
                self.assertEqual(spec['source'], source)
                self.assertEqual(spec['target_address'], address)
                self.assertEqual(spec.get('compare_size', spec['size']), size)
        self.assertNotIn('effect-manager-added-callback', self.units)

    def test_compiler_local_label_renames_preserve_all_original_destinations(self):
        target = {
            'bullet-draw-single': {
                0x017: 0x00413BC8,
                0x0D8: 0x00413B0B, 0x0DC: 0x00413B13,
                0x0E0: 0x00413B1B, 0x0E4: 0x00413B23,
            },
            'etama-controller-added-callback': {
                0x17F: 0x004140A4, 0x186: 0x00414098,
                0x1EB: 0x0041411C, 0x1F2: 0x00414110,
                0x258: 0x00413FCA, 0x25C: 0x00413FDB,
                0x260: 0x00413FFC, 0x2D0: 0x00414036,
                0x2D4: 0x00414044, 0x2D8: 0x00414052,
            },
        }
        for name, expected in target.items():
            with self.subTest(unit=name):
                actual = {r['offset']: r for r in self.units[name]['relocations']
                          if r['symbol'].startswith('$L')}
                self.assertEqual(set(actual), set(expected))
                for offset, value in expected.items():
                    self.assertEqual(actual[offset]['target'], value)
                    self.assertEqual(actual[offset]['type'], 'DIR32')
        self.assertEqual(sum(map(len, target.values())), 15)


if __name__ == '__main__':
    unittest.main()

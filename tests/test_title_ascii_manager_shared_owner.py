"""TH09 Title callback uses real AsciiManager, not a separately mangled global.

The unrelated folded one-byte callee at 0x0043D2B0 is origin unknown: this
check intentionally does NOT invent an AsciiManager implementation for it.
Target-dependent full-byte exactness still requires the cold PE/COFF Oracle.
"""

import csv
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
GLOBAL = '?g_AsciiManager@@3VAsciiManager@@A'
RESET = '?Reset@AsciiManager@@QAEXXZ'
FOLDED = '?InitializeVms@TitleAsciiManagerView@@QAEXXZ'


class RealAsciiManagerTitleOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.title = (ROOT/'src/TitleScreen.cpp').read_text()
        cls.ascii_header = (ROOT/'src/AsciiManager.hpp').read_text()
        cls.ascii_reset = (ROOT/'src/AsciiManagerReset.cpp').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as file:
            cls.units = tomllib.load(file)['units']
        with (ROOT/'config/function-origins.csv').open(newline='') as file:
            cls.origins = {r['address']: r for r in csv.DictReader(file)}
        with (ROOT/'config/matches.csv').open(newline='') as file:
            cls.accepted = list(csv.DictReader(file))

    def test_real_source_definition_and_no_duplicate_global_type(self):
        self.assertEqual(self.title.count('#include "AsciiManager.hpp"'), 1)
        self.assertIn('class AsciiManager', self.ascii_header)
        self.assertIn('void AsciiManager::Reset()', self.ascii_reset)
        self.assertIn('extern AsciiManager g_AsciiManager;', self.title)
        self.assertNotIn('extern TitleAsciiManagerView g_AsciiManager;', self.title)
        self.assertNotIn('void Reset();\n    void InitializeVms();', self.title)
        self.assertIn('g_AsciiManager.Reset();', self.title)

    def test_unknown_folded_callback_retains_its_independent_provenance(self):
        self.assertIn('struct TitleAsciiManagerView {\n    void InitializeVms();', self.title)
        self.assertIn('reinterpret_cast<TitleAsciiManagerView *>(&g_AsciiManager)',
                      self.title)
        self.assertNotIn('void InitializeVms();', self.ascii_header)
        row = self.origins['0x0043D2B0']
        self.assertEqual((row['origin'], row['disposition'], row['confidence']),
                         ('unknown', 'review', 'unknown'))
        self.assertFalse(any('0x0043D2B0' in row.values() for row in self.accepted))

    def test_actual_added_callback_real_symbol_targets_and_folded_call(self):
        unit = self.units['title-screen-actual-added-callback']
        self.assertEqual((unit['size'], unit['target_address']), (335, 0x00424BF7))
        mapping = {r['offset']: (r['symbol'], r['target'], r['type'])
                   for r in unit['relocations']}
        self.assertEqual(len(mapping), 27)
        self.assertEqual(mapping[0xC], (GLOBAL, 0x004CE458, 'DIR32'))
        self.assertEqual(mapping[0x1B], (RESET, 0x004343E0, 'REL32'))
        self.assertEqual(mapping[0x22], (FOLDED, 0x0043D2B0, 'REL32'))
        self.assertNotIn('TitleAsciiManagerView@@A', str(mapping))
        self.assertNotIn('Reset@TitleAsciiManagerView', str(mapping))
        assert_reset = self.units['ascii-manager-reset']
        self.assertEqual((assert_reset['target_address'], assert_reset['symbol']),
                         (0x004343E0, RESET))

    def test_other_real_title_exact_owners_keep_original_references(self):
        start = self.units['title-screen-start-menu']
        self.assertEqual((start['size'], start['compare_size'],
                          start['target_address']),
                         (1560, 1592, 0x00429E23))
        targets = {
            0x3C2: 0x0042A43B,
            0x618: 0x0042A1E9, 0x61C: 0x0042A208,
            0x620: 0x0042A23C, 0x624: 0x0042A286,
            0x628: 0x0042A2D2, 0x62C: 0x0042A293,
            0x630: 0x0042A302, 0x634: 0x0042A380,
        }
        local_labels = {r['offset']: r for r in start['relocations']
                        if r['symbol'].startswith('$L')}
        self.assertEqual(set(local_labels), set(targets))
        for off, va in targets.items():
            with self.subTest(offset=hex(off)):
                self.assertEqual((local_labels[off]['type'],
                                  local_labels[off]['target']), ('DIR32', va))
        self.assertEqual(self.units['title-screen-setup-thread']['size'], 534)


if __name__ == '__main__':
    unittest.main()

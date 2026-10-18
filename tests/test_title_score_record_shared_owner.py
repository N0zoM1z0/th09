"""Producer/renderer share the genuine TH09 0x2C score-record owner.

This target-independent contract prevents a separately mangled C++ global
from masquerading as the same original 16x5x5 table. Cold target-bound
comparison remains required for byte exactness.
"""

from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
HEADER = 'TitleScoreRecordView.hpp'
SCORE_SYMBOL = '?g_TitleScoreTable@@3PAY144UTitleScoreRecordView@@A'
OLD_RENDER_SYMBOL = '?g_TitleScoreTable@@3PAY144UResultDrawScoreRecord@@A'


class SharedTitleScoreRecordOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header = (ROOT / 'src' / HEADER).read_text()
        cls.producer = (ROOT / 'src/TitleScreen.cpp').read_text()
        cls.renderer = (ROOT / 'src/TitleScreenResultDraw.cpp').read_text()
        cls.oracle = (ROOT / 'scripts/inspect-title-result-draw.py').read_text()
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            cls.units = tomllib.load(stream)['units']

    def test_single_record_type_and_real_table_declarations(self):
        self.assertEqual(self.header.count('struct TitleScoreRecordView\n'), 1)
        self.assertEqual(self.header.count('extern TitleScoreRecordView g_TitleScoreTable[16][5][5];'), 1)
        self.assertEqual(self.header.count('extern TitleScoreRecordView g_TitleScoreRecord;'), 1)
        for source in (self.producer, self.renderer):
            self.assertEqual(source.count('#include "' + HEADER + '"'), 1)
            self.assertNotIn('struct TitleScoreRecordView\n', source)
            self.assertNotIn('struct ResultDrawScoreRecord', source)
            self.assertNotIn('extern TitleScoreRecordView g_TitleScoreTable', source)
        self.assertNotIn('ResultDrawScoreRecord', self.renderer)
        self.assertIn('TitleScoreRecordView (*scoreGroup)[5];', self.renderer)
        self.assertIn('TitleScoreRecordView *entry = *scoreGroup;', self.renderer)
        self.assertIn('g_TitleScoreRecord.InsertIntoTable()', self.producer)

    def test_one_original_layout_covers_both_consumers(self):
        for fragment in ('sizeof(TitleScoreRecordView) == 0x2C',
                         'offsetof(TitleScoreRecordView, score) == 0x0C',
                         'offsetof(TitleScoreRecordView, category0) == 0x14',
                         'offsetof(TitleScoreRecordView, category1) == 0x15',
                         'offsetof(TitleScoreRecordView, rank) == 0x16',
                         'offsetof(TitleScoreRecordView, name) == 0x18',
                         'offsetof(TitleScoreRecordView, scoreDigit) == 0x2B',
                         'char scoreDigit;'):
            with self.subTest(fragment=fragment):
                self.assertIn(fragment, self.header)
        self.assertIn('entry->scoreDigit', self.renderer)
        self.assertIn('static_cast<i32>(entry->score)', self.renderer)
        self.assertIn('scoreGroup = g_TitleScoreTable[nameBankIndex];', self.renderer)
        self.assertEqual(16 * 5 * 5 * 0x2C, 0x44C0)
        self.assertEqual(5 * 5 * 0x2C, 0x44C)

    def test_exact_score_insert_and_open_draw_bind_same_original_address(self):
        insert = self.units['score-record-insert']
        self.assertEqual(insert['source'], 'src/TitleScreen.cpp')
        self.assertEqual(insert['target_address'], 0x00421800)
        fields = [row for row in insert['relocations']
                  if row['symbol'] == SCORE_SYMBOL]
        self.assertEqual(len(fields), 5)
        self.assertEqual([row['offset'] for row in fields],
                         [0x1E, 0x67, 0x8C, 0xAE, 0xD2])
        self.assertTrue(all(row['target'] == 0x004A8380 and
                            row['type'] == 'DIR32' and
                            row.get('addend', 0) == 0 for row in fields))
        self.assertIn("'" + SCORE_SYMBOL + "': 0x4A8380", self.oracle)
        self.assertNotIn(OLD_RENDER_SYMBOL, self.oracle)
        self.assertNotIn('ResultDrawScoreRecord', self.oracle)

    def test_start_menu_private_switch_fields_still_have_original_targets(self):
        # Header placement changed VC7's private $L spellings, not any
        # original branch/exception destinations. Strict original comparison
        # is required for the full 1,592-byte physical carrier separately.
        unit = self.units['title-screen-start-menu']
        self.assertEqual(unit['source'], 'src/TitleScreen.cpp')
        self.assertEqual((unit['size'], unit['compare_size']), (1560, 1592))
        originals = {
            0x3C2: 0x0042A43B,
            0x618: 0x0042A1E9, 0x61C: 0x0042A208,
            0x620: 0x0042A23C, 0x624: 0x0042A286,
            0x628: 0x0042A2D2, 0x62C: 0x0042A293,
            0x630: 0x0042A302, 0x634: 0x0042A380,
        }
        labels = {row['offset']: row for row in unit['relocations']
                  if row['symbol'].startswith('$L')}
        self.assertEqual(set(labels), set(originals))
        for offset, address in originals.items():
            with self.subTest(offset=hex(offset)):
                self.assertEqual((labels[offset]['type'], labels[offset]['target']),
                                 ('DIR32', address))


if __name__ == '__main__':
    unittest.main()

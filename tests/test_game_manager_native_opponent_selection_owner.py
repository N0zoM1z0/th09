"""799-byte exact opponent selection and 1689-byte setup boss share GameManager.

Original source-cold PE/COFF and full-boss noncrediting comparison, not text
assertions, validate physical code and call-site equivalence.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
CALL='?SelectOpponentConfiguration@AsciiGameManagerView@@QAEXXZ'

class GameManagerNativeOpponentSelectionOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_one_original_manager_and_no_duplicate_opponent_prefix(self):
        native=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        side=(ROOT/'src/GameManagerSideState.hpp').read_text()
        old=(ROOT/'src/GameManagerOpponentSelectionView.hpp').read_text()
        owner=(ROOT/'src/GameManagerSelectOpponent.cpp').read_text()
        self.assertIn('void SelectOpponentConfiguration();',native)
        self.assertIn('void AsciiGameManagerView::SelectOpponentConfiguration()',owner)
        self.assertNotIn('void GameManagerOpponentSelectionView::',owner)
        self.assertNotIn('struct GameManagerOpponentSelectionView',old)
        self.assertNotIn('struct OpponentSelectionSide',old)
        self.assertNotIn('struct OpponentSelectionState',old)
        self.assertIn('struct OpponentSelectionEntry',old)
        self.assertIn('OpponentSelectionEntrySizeIs10',old)
        self.assertIn('AsciiGameSideRuntimeView *state = sides[0].state;',owner)
        self.assertNotIn('reinterpret_cast<AsciiGameManagerView *>(this)',owner)
        self.assertIn('NativeGameManagerOpponentSelectionOffsets',native)
        self.assertIn('union { unsigned char clearD8[0x10]; unsigned char usedCharactersD8[16]; };',native)
        self.assertIn('OpponentSelectionEntry *selectedEntryF0;',native)
        self.assertIn('int selectedValueF4;',native)
        self.assertIn('int selector138;',native)
        self.assertIn('int selectedParameter2C;',side)
        for field,offset in (('usedCharactersD8','0xD8'),
                             ('selectedEntryF0','0xF0'),
                             ('selectedValueF4','0xF4'),
                             ('counterFC','0xFC'),
                             ('setupMode11C','0x11C'),
                             ('selector138','0x138')):
            with self.subTest(field=field):
                self.assertIn('offsetof(AsciiGameManagerView, '+field+') == '+offset,native)
        self.assertIn('offsetof(AsciiGameManagerSideView, selectedParameter2C) == 0x2C',native)
        self.assertIn('sizeof(AsciiGameManagerView) == 0x390',native)

    def test_four_gameplay_setup_calls_target_original_exact_method(self):
        worker=(ROOT/'src/GameManagerSetup.cpp').read_text()
        self.assertEqual(worker.count('g_GameManager.SelectOpponentConfiguration();'),4)
        self.assertNotIn('reinterpret_cast<GameManagerOpponentSelectionView *>(&g_GameManager)',worker)
        self.assertNotIn('#include "GameManagerOpponentSelectionView.hpp"',worker)
        leaf=self.units['game-manager-select-opponent']
        self.assertEqual((leaf['target_address'],leaf['size'],len(leaf['relocations'])),
                         (0x00415910,799,32))
        self.assertEqual(leaf['symbol'],CALL)
        self.assertEqual(leaf['source'],'src/GameManagerSelectOpponent.cpp')
        inspector=(ROOT/'scripts/inspect-gameplay-setup.py').read_text()
        frontier=(ROOT/'scripts/check-boss-frontiers.py').read_text()
        self.assertIn("'%s': 0x415910"%CALL,inspector)
        self.assertIn(CALL,frontier)
        self.assertNotIn('SelectOpponentConfiguration@GameManagerOpponentSelectionView',inspector+frontier)
        self.assertIn('TARGET_FIELDS = 172',inspector)
        self.assertIn('TARGET_CALLS = 35',inspector)

    def test_source_metadata_keeps_separate_big_boss_nonexact(self):
        self.assertEqual(self.units['game-manager-on-update']['size'],1230)
        self.assertEqual(self.units['game-manager-cleanup-gameplay-state']['size'],425)
        self.assertNotIn('gameplay-setup-thread',self.units)
        for file in ('functions.csv','implemented.csv','matches.csv','reccmp-functions.csv'):
            s=(ROOT/'config'/file).read_text()
            self.assertIn('AsciiGameManagerView::SelectOpponentConfiguration',s)
            self.assertNotIn('GameManagerOpponentSelectionView::SelectOpponentConfiguration',s)

if __name__=='__main__':unittest.main()

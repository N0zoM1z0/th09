"""StartAttack shares the 0x38-byte native GameManager side record."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class CardAttackStartNativeSideRecord(unittest.TestCase):
    def test_one_side_allocation_with_typed_player_receiver(self):
        s=(ROOT/'src/CardAttackStart.cpp').read_text()
        h=(ROOT/'src/CardAttack.hpp').read_text()
        common=(ROOT/'src/GameManagerSideState.hpp').read_text()
        self.assertNotIn('struct CardAttackStartSideView',s)
        self.assertIn('typedef AsciiGameManagerSideView CardAttackStartSideView;',h)
        self.assertIn('CardAttackStartNativeSideRecord',s)
        self.assertIn('offsetof(CardAttackStartSideView, player04) == 0x04',s)
        self.assertIn('offsetof(CardAttackStartSideView, enemyManager10) == 0x10',s)
        self.assertIn('offsetof(CardAttackStartSideView, frontSide18) == 0x18',s)
        self.assertIn('PlayerLifecycleView *player04;',common)
        self.assertIn('startOtherSideState08->player04->tableValue303F0',s)
        self.assertIn('startOtherSideState08->background00->transitionFrames',s)

    def test_exact_owner_identity_unchanged(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        record=units['card-attack-start']
        self.assertEqual((record['size'],len(record['relocations'])),(923,29))

if __name__=='__main__':unittest.main()

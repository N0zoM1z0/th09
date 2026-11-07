"""StartAttack consumes true Player header and +0x303F0 state, not a clone."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class CardAttackStartNativePlayerHeader(unittest.TestCase):
    def test_header_state_reads_use_native_player_storage(self):
        source=(ROOT/'src/CardAttackStart.cpp').read_text()
        player=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        shared=(ROOT/'src/GameManagerSideState.hpp').read_text()
        self.assertNotIn('struct CardAttackStartPlayerView',source)
        self.assertNotIn('attackPlayer04',source)
        self.assertNotIn('CardAttackStartPlayerView *attackPlayer04;',shared)
        self.assertIn('PlayerLifecycleView *player04;',shared)
        self.assertIn('CardAttackStartNativePlayerHeader',source)
        for name,offset in (('state7C','0xA0'),('state80','0xA4'),
                             ('state8C','0xB0'),('state90','0xB4'),('state94','0xB8')):
            with self.subTest(member=name):
                self.assertIn(f'offsetof(PlayerHeaderStateView, {name}) + 0x24 == {offset}',source)
                self.assertIn('->player04->header24.'+name,source)
                self.assertIn('int '+name+';',player)
        self.assertIn('offsetof(PlayerLifecycleView, tableValue303F0) == 0x303F0',source)
        self.assertIn('->player04->tableValue303F0',source)

    def test_same_original_owner_and_field_count(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        u=units['card-attack-start']
        self.assertEqual((u['size'],len(u['relocations'])),(923,29))
        self.assertEqual(u['target_address'],0x00403E50)

if __name__=='__main__':unittest.main()

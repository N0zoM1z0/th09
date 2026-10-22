"""Source regression guard for two distinct GameManager-owned object types.

These checks bind maintained owner expressions, not executable exactness. The
canonical complete original-bound replay remains the code-generation oracle.
"""
from collections import Counter
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[1]


class GameManagerPlayerCounterOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = (ROOT / 'src/GameManagerSetup.cpp').read_text()
        cls.player = (ROOT / 'src/PlayerLifecycleView.hpp').read_text()

    def test_state_allocation_has_no_unrelated_player_tail(self):
        state = self.source.split('struct SetupStateBuffer\n{', 1)[1].split('};', 1)[0]
        self.assertIn('unsigned char unknown_15[0x8B];', state)
        self.assertNotRegex(state, r'\b(?:valueA0|valueA4|state7C|state80)\b')
        self.assertIn('(sizeof(SetupStateBuffer) == 0xA0) ? 1 : -1', self.source)
        self.assertIn('static_cast<SetupStateBuffer *>(operator new(0xA0))', self.source)
        self.assertIn('memset(manager->sides[sideIndex].state, 0, 0xA0);', self.source)

    def test_existing_player_header_members_keep_observed_offsets(self):
        self.assertIn('int state7C;', self.player)
        self.assertIn('int state80;', self.player)
        self.assertIn('PlayerHeaderStateView header24;', self.player)
        for member, offset in [('state7C', '0xA0'), ('state80', '0xA4')]:
            self.assertRegex(self.source,
                r'offsetof\(PlayerLifecycleView, header24\)\s*\+\s*'
                r'offsetof\(PlayerHeaderStateView, ' + member + r'\) == ' + offset)

    def test_setup_twenty_stores_use_player_not_state_receivers(self):
        setup = self.source.split('void __fastcall GameplaySetupThread(void *unused)\n{', 1)[1]
        setup = setup.split('void SupervisorSetupLayout::SetupLoadingVmsAndInitCapture(', 1)[0]
        stores = re.findall(
            r'static_cast<PlayerLifecycleView \*>\(manager->sides\[([01])\]\.subsystem1\)'
            r'->header24\.(state7C|state80) = (\d+);', setup)
        self.assertEqual(Counter(stores), Counter(
            (side, member, value) for side in ['0', '1']
            for member in ['state7C', 'state80'] for value in ['10', '8', '4', '1', '1']))
        self.assertNotRegex(setup, r'\bvalueA[04]\b')

    def test_on_update_four_guards_and_increments_use_player_receivers(self):
        update = self.source.split('int GameManagerSetupLayout::OnUpdate(', 1)[1]
        update = update.split('\nint GameManagerSetupLayout::OnDraw(', 1)[0]
        for side in ['0', '1']:
            for member in ['state7C', 'state80']:
                expr = (f'static_cast<PlayerLifecycleView *>(g_GameManager.sides[{side}].subsystem1)'
                        f'->header24.{member}')
                self.assertEqual(update.count(expr + ' < 16)'), 1)
                self.assertEqual(update.count(expr + '++;'), 1)
        self.assertNotRegex(update, r'\bvalueA[04]\b')


if __name__ == '__main__':
    unittest.main()

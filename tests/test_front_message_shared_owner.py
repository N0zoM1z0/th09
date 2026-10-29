"""Game, Front Added and Message initializer share one real TH09 method symbol.

Target-independent source/manifest checks: the complete source-cold PE/COFF
oracles remain necessary; a shared C++ name is not source-commit-bound exactness.
"""
from pathlib import Path
import importlib.util
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
SYMBOL = '?InitializeMessageRuntime@FrontMessageOwnerView@@QAEXXZ'
VA = 0x004181E0


class SharedFrontMessageOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header = (ROOT/'src/FrontMessageOwnerView.hpp').read_text()
        cls.impl = (ROOT/'src/FrontMessageInitializeRuntime.cpp').read_text()
        cls.game = (ROOT/'src/GameManagerSetup.cpp').read_text()
        cls.front = (ROOT/'src/FrontAddedCallback.cpp').read_text()
        cls.game_inspector = (ROOT/'scripts/inspect-game-manager-cleanup.py').read_text()
        cls.game_legacy = (ROOT/'scripts/inspect-gameplay-cleanup.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units = tomllib.load(f)['units']

    def test_one_real_owner_layout_with_target_offsets(self):
        self.assertEqual(self.header.count('struct FrontMessageOwnerView\n'), 1)
        for fragment in ('offsetof(FrontMessageOwnerView,messageRuntimeE944)==0xE944',
                         'offsetof(FrontMessageOwnerView,transitionVmCount1095C)==0x1095C',
                         'offsetof(FrontMessageOwnerView,transitionState11EA8)==0x11EA8',
                         'sizeof(FrontMessageOwnerView)==0x11EAC',
                         'offsetof(FrontMessageOwnerView,transitionFrame10960)==0x10960',
                         'offsetof(FrontMessageOwnerView,transitionSide10964)==0x10964',
                         'void InitializeMessageRuntime();'):
            with self.subTest(fragment=fragment):
                self.assertIn(fragment, self.header)

    def test_cleanup_calls_the_complete_native_message_owner(self):
        # The actual original CALL at +0x1B binds its REL32 field at +0x1C.
        # Local names, casts and include counts are not acceptance criteria.
        rows = self.units['game-manager-cleanup-gameplay-state']['relocations']
        calls = [r for r in rows if r['target'] == VA]
        self.assertEqual(calls, [dict(offset=0x1C, type='REL32', symbol=SYMBOL, target=VA)])
        self.assertEqual(self.units['front-message-initialize-runtime']['size'], 133)

    def test_same_genuine_member_referent_preserved_in_both_auditors(self):
        real = self.units['front-message-initialize-runtime']
        self.assertEqual(real['target_address'], VA)
        self.assertEqual(real['symbol'], SYMBOL)
        self.assertEqual(real['source'], 'src/FrontMessageInitializeRuntime.cpp')
        self.assertEqual(real['size'], 133)
        for script in (self.game_inspector, self.game_legacy):
            self.assertIn(SYMBOL, script)
            self.assertNotIn('SetupFrontMessageResetView', script)
        self.assertIn("'" + SYMBOL + "':0x004181E0", self.game_inspector)

    def test_preserves_both_distinct_nonexact_callers(self):
        # Cleanup now has a complete canonical425-byte replay. Front Added
        # remains a distinct nonexact caller; the signature alone proves neither.
        self.assertIn('CleanupGameplayState()', self.game)
        self.assertIn('int FrontAddedCallback(void *frontArg)', self.front)
        self.assertIn('425', self.game_inspector)
        self.assertIn('41', self.game_inspector)


if __name__ == '__main__':
    unittest.main()

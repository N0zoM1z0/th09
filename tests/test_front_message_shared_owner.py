"""Game, Front Added and Message initializer share one real TH09 method symbol.

Target-independent source/manifest checks: the complete source-cold PE/COFF
oracles remain necessary; a shared C++ name is not source-commit-bound exactness.
"""
from pathlib import Path
import importlib.util
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
SYMBOL = '?InitializeMessageRuntime@FrontFactoryConstructView@@QAEXXZ'
VA = 0x004181E0


class SharedFrontMessageOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header = (ROOT/'src/FrontFactoryConstructView.hpp').read_text()
        cls.old_header = (ROOT/'src/FrontMessageOwnerView.hpp').read_text()
        cls.impl = (ROOT/'src/FrontMessageInitializeRuntime.cpp').read_text()
        cls.game = (ROOT/'src/GameManagerSetup.cpp').read_text()
        cls.front = (ROOT/'src/FrontAddedCallback.cpp').read_text()
        cls.game_inspector = (ROOT/'scripts/inspect-game-manager-cleanup.py').read_text()
        cls.game_legacy = (ROOT/'scripts/inspect-gameplay-cleanup.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units = tomllib.load(f)['units']

    def test_one_real_owner_layout_with_target_offsets(self):
        # The initializer now works on the whole 0x1293C-byte factory object;
        # an independent 0x11EAC-byte allocation is no longer a valid owner.
        self.assertEqual(self.header.count('struct FrontFactoryConstructView\n'), 1)
        self.assertNotIn('struct FrontMessageOwnerView', self.old_header)
        for fragment in ('offsetof(FrontFactoryConstructView, messageRuntimeE944) == 0xE944',
                         'offsetof(FrontFactoryConstructView, transitionVmCount1095C) == 0x1095C',
                         'offsetof(FrontFactoryConstructView, transitionState11EA8) == 0x11EA8',
                         'sizeof(FrontFactoryConstructView) == 0x1293C',
                         'offsetof(FrontFactoryConstructView, transitionFrame10960) == 0x10960',
                         'offsetof(FrontFactoryConstructView, transitionSide10964) == 0x10964',
                         'void InitializeMessageRuntime();'):
            with self.subTest(fragment=fragment):
                self.assertIn(fragment, self.header)
        self.assertIn('void FrontFactoryConstructView::InitializeMessageRuntime()', self.impl)

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
        # Cleanup and FrontAdded are distinct original-exact owners; both
        # must continue to call the exact 133-byte native Front member.
        self.assertIn('CleanupGameplayState()', self.game)
        self.assertIn('int FrontAddedCallback(void *frontArg)', self.front)
        self.assertEqual(self.units['game-manager-cleanup-gameplay-state']['size'], 425)
        self.assertEqual(self.units['front-added-callback']['size'], 706)
        self.assertIn('gui->InitializeMessageRuntime()', self.front)
        self.assertIn('41', self.game_inspector)


if __name__ == '__main__':
    unittest.main()

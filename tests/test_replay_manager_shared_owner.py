"""Cross-source authentic ReplayManager member/0x164-byte layout guards.

The source-bearing type is still a TH09 reconstruction *view*: the original
class name/TU split is not inferred from a matching machine code symbol.
"""

from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class ReplayManagerSharedOriginalOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header = (ROOT/'src/ReplayManagerRuntimeView.hpp').read_text()
        cls.replay = (ROOT/'src/ReplayInput.cpp').read_text()
        cls.title = (ROOT/'src/TitleScreen.cpp').read_text()
        cls.game = (ROOT/'src/GameManagerSetup.cpp').read_text()
        cls.inspector = (ROOT/'scripts/inspect-gameplay-setup.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as file:
            cls.units = tomllib.load(file)['units']

    def test_single_canonical_0x164_layout_across_three_real_callers(self):
        for label,src in [('replay',self.replay),('game',self.game),
                          ('title',self.title)]:
            with self.subTest(source=label):
                self.assertEqual(src.count('#include "ReplayManagerRuntimeView.hpp"'), 1)
                self.assertNotIn('struct ReplayManagerView\n{',src)
                self.assertNotIn('struct ReplayManagerView {',src)
        self.assertEqual(self.header.count('struct ReplayManagerView\n{'),1)
        for label in ('ReplayManagerSizeIs164', 'ReplayManagerMainChainAt150',
                      'ReplayManagerFrameSyncAt15C',
                      'ReplayManagerInput0At0C', 'ReplayManagerStageBuffersAt1C',
                      'ReplayManagerCurrentBufferAt10C'):
            self.assertIn(label,self.header)
        self.assertIn('ReplayBufferLinkSizeIs18', self.header)
        self.assertIn('ReplayBufferLinkNextAt14', self.header)

    def test_gameplay_calls_real_release_with_no_fake_overlaid_abi(self):
        self.assertNotIn('ReplayManagerSetupReleaseView',self.game+self.inspector)
        self.assertIn('reinterpret_cast<ReplayManagerView *>(manager->stageObject)',
                      self.game)
        self.assertIn('->Release();', self.game)
        self.assertIn("'?Release@ReplayManagerView@@QAEXXZ': 0x420AD0",self.inspector)
        self.assertIn('void ReplayManagerView::Release()',self.replay)
        self.assertIn('void Release();', self.header)
        self.assertNotIn('void Release();',self.game)

    def test_full_release_and_alternate_teardown_are_not_conflated(self):
        release=self.units['replay-manager-release']
        alternate=self.units['replay-stage-release'] if 'replay-stage-release' in self.units else None
        self.assertEqual(release['target_address'],0x00420AD0)
        self.assertEqual(release['size'],318)
        self.assertEqual(release['symbol'],'?Release@ReplayManagerView@@QAEXXZ')
        # The distinct 42-byte alternate teardown is NOT the source callee
        # in GameplaySetupThread; it does not walk the same stage buffers.
        self.assertIn("'?ReleaseStageObject@@YIXPAX@Z': 0x420C10",self.inspector)
        self.assertNotEqual(0x00420C10,release['target_address'])
        if alternate:
            self.assertNotEqual(alternate['target_address'],release['target_address'])

    def test_create_and_release_replay_across_real_distinct_tu_profiles(self):
        create=self.units['replay-manager-create']
        release=self.units['replay-manager-release']
        self.assertEqual(create['target_address'],0x00421670)
        self.assertEqual(create['size'],361)
        self.assertNotEqual(create['object'],release['object'])
        self.assertNotEqual(create['profile'],release['profile'])
        self.assertEqual(create['source'],release['source'])
        self.assertIn('ReplayManagerView *Create(',self.header)

    def test_inspector_external_symbol_is_real_canonical_exact_destination(self):
        # This must also run in the intentionally site-packages-free CI test
        # environment. The Capstone-backed full PE/COFF inspection is an
        # independent cold gate under --owner gamemgr.
        name=self.units['replay-manager-release']['symbol']
        self.assertIn(f"'{name}': 0x420AD0",self.inspector)
        self.assertEqual(self.inspector.count(f"'{name}': 0x420AD0"),1)
        self.assertNotIn('?Release@ReplayManagerSetupReleaseView@@QAEXXZ',
                         self.inspector)


if __name__=='__main__':
    unittest.main()

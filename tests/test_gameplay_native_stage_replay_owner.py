"""Native +0xD4 gameplay stage resource owner and authentic replay ABI.

The complete original GameplaySetupThread remains nonexact, but all real
Create/Release/Dispatch call targets and its physical source-cold frontier are
checked independently alongside the 1230-byte GameManager OnUpdate exact owner.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeGameStageReplayOwner(unittest.TestCase):
    def test_one_target_slot_has_true_replay_and_dispatch_type(self):
        shared=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        setup=(ROOT/'src/GameManagerSetupView.hpp').read_text()
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        self.assertIn('ReplayManagerView *replayManagerD4;',shared)
        self.assertIn('ReplayStageDispatchView *replayDispatchD4;',shared)
        self.assertIn('void *stageObjectD4;',shared)
        self.assertIn('void *stageObject;',setup)
        self.assertIn('ReplayManagerView *replayManagerD4;',setup)
        self.assertIn('ReplayStageDispatchView *replayDispatchD4;',setup)
        for member in ('stageObjectD4','replayManagerD4','replayDispatchD4'):
            self.assertIn('offsetof(AsciiGameManagerView, '+member+') == 0xD4',shared)
        self.assertIn('manager->replayManagerD4->Release();',source)
        self.assertIn('manager->replayManagerD4 = ReplayManagerView::Create(',source)
        self.assertIn('DispatchReplayStage(',source)
        self.assertIn('manager->replayDispatchD4',source)
        self.assertNotIn('reinterpret_cast<ReplayManagerView *>(manager->stageObject)',source)
        self.assertNotIn('static_cast<ReplayStageDispatchView *>(manager->stageObject)',source)

    def test_original_large_setup_and_replay_callee_owners_are_frozen(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for name,size,fields in (
            ('game-manager-on-update',1230,87),
            ('game-manager-cleanup-gameplay-state',425,43),
            ('replay-manager-create',361,28),
            ('replay-manager-release',318,20),
            ('supervisor-setup-loading-vms-and-capture',214,17),
        ):
            with self.subTest(unit=name):
                self.assertEqual((units[name]['size'],len(units[name]['relocations'])),
                                 (size,fields))

if __name__=='__main__':
    unittest.main()

"""Native +0xD4 gameplay stage resource owner and authentic replay ABI.

GameplaySetupThread is now canonically exact (1,689 bytes, 172 relocations).
The authentic Create/Release/Dispatch contracts and its source-cold owner
remain independently checked alongside the exact GameManager OnUpdate.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeGameStageReplayOwner(unittest.TestCase):
    def test_one_target_slot_has_true_replay_and_dispatch_type(self):
        shared=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        setup=(ROOT/'src/GameManagerSetupView.hpp').read_text()
        self.assertIn('ReplayManagerView *replayManagerD4;',shared)
        self.assertIn('ReplayStageDispatchView *replayDispatchD4;',shared)
        self.assertIn('void *stageObjectD4;',shared)
        self.assertIn('void *stageObject;',setup)
        self.assertIn('ReplayManagerView *replayManagerD4;',setup)
        self.assertIn('ReplayStageDispatchView *replayDispatchD4;',setup)
        for member in ('stageObjectD4','replayManagerD4','replayDispatchD4'):
            self.assertIn('offsetof(AsciiGameManagerView, '+member+') == 0xD4',shared)
        # The full COFF auditor checks true Create/Release/Dispatch operands
        # and counts independently of local pointer or global access spelling.

    def test_original_large_setup_and_replay_callee_owners_are_frozen(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for name,size,fields in (
            ('game-manager-on-update',1230,87),
            ('game-manager-gameplay-setup-thread',1689,172),
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

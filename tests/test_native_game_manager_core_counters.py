"""Native GameManager core stage/counter fields shared by exact OnUpdate.

This source identity contract is supplemented by separate fresh original
PE/COFF comparisons for the complete 1230-byte owner and its exact siblings.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeGameManagerCoreCounters(unittest.TestCase):
    def test_one_original_field_owner_for_stage_and_counter_words(self):
        hdr=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        self.assertIn('AsciiGameManagerSetupCountersAtB0',hdr)
        self.assertIn('union { int valueB4; int limitB4; };',hdr)
        self.assertIn('union { ReplayInputGateView *replayInputGateE8; void *heapE8; };',hdr)
        for name,off in (
            ('valueB0','0xB0'),('limitB4','0xB4'),('valueB8','0xB8'),
            ('valueC0','0xC0'),('valueC4','0xC4'),
            ('valueC8','0xC8'),('valueCC','0xCC'),('valueD0','0xD0'),
            ('stageObjectD4','0xD4'),('clearD8','0xD8'),('heapE8','0xE8')):
            with self.subTest(name=name):
                self.assertIn('offsetof(AsciiGameManagerView, '+name+') == '+off,hdr)
        for name in ('valueB0','limitB4','valueB8','valueC0',
                     'valueC4','valueC8','valueCC','valueD0'):
            with self.subTest(native_access=name):
                self.assertIn('g_GameManager.'+name,source)
        self.assertIn('GameManagerSetupLayout::OnUpdate(',source)

    def test_original_game_manager_and_replay_units_are_immutable(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for name,size,relocs in (
            ('game-manager-on-update',1230,87),
            ('game-manager-cleanup-gameplay-state',425,43),
            ('game-manager-added-callback',187,13),
            ('replay-manager-begin-recording-stage',1323,82),
            ('replay-manager-load-replay-data',591,14)):
            with self.subTest(name=name):
                u=units[name]
                self.assertEqual((u['size'],len(u['relocations'])),(size,relocs))

if __name__=='__main__':
    unittest.main()

"""Original 405-byte Supervisor DeletedCallback frees native GameManager sides.

Keep DeletedGameManagerLayout's original external data symbol identity and
source-cold original PE/COFF proof while using the shared +0x1C allocations.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class DeletedGameSideNativeAllocations(unittest.TestCase):
    def test_shared_side_allocations_are_original_layout(self):
        src=(ROOT/'src/SupervisorDeletedCallback.cpp').read_text()
        hdr=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        self.assertIn('#define TH09_GAME_MANAGER_TYPES_ONLY',src)
        self.assertIn('#undef TH09_GAME_MANAGER_TYPES_ONLY',src)
        self.assertIn('struct DeletedGameManagerLayout : AsciiGameManagerView',src)
        self.assertNotIn('unsigned char unknown000[0x1C]',src)
        self.assertIn('DeletedGameManagerNativeSideAllocationOffsets',src)
        self.assertIn('offsetof(AsciiGameManagerSideView, runtime) == 0x1C',src)
        self.assertIn('sizeof(AsciiGameManagerSideView) == 0x38',src)
        self.assertIn('extern DeletedGameManagerLayout g_GameManager;',src)
        self.assertIn('AsciiGameManagerSideView sides[2];',hdr)
        for index in (0,1):
            word=f'g_GameManager.sides[{index}].runtime'
            self.assertEqual(src.count(word),3)
        self.assertNotIn('g_GameManager.heap1C',src)
        self.assertNotIn('g_GameManager.heap54',src)

    def test_original_callers_still_pinned(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for name,size,fields in (
            ('supervisor-deleted-callback',405,44),
            ('supervisor-startup-thread',882,106),
            ('game-manager-on-update',1230,87),
            ('replay-manager-begin-recording-stage',1323,82)):
            with self.subTest(unit=name):
                x=units[name]
                self.assertEqual((x['size'],len(x['relocations'])),(size,fields))

if __name__=='__main__':
    unittest.main()

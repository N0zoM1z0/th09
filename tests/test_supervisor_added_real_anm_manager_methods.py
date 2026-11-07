"""Supervisor's exact 238-byte AddedCallback invokes real ANM manager methods.

Every call field binds to its independently complete native TH09 leaf.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SupervisorAddedNativeAnmManager(unittest.TestCase):
    def test_source_uses_real_anm_manager(self):
        source=(ROOT/'src/SupervisorAddedCallback.cpp').read_text()
        anm=(ROOT/'src/AnmManager.hpp').read_text()
        self.assertIn('#include "AnmManager.hpp"',source)
        self.assertNotIn('struct SupervisorAddedAnmManagerView',source)
        self.assertNotIn('reinterpret_cast<SupervisorAddedAnmManagerView *>',source)
        self.assertEqual(source.count('g_AnmManager->LoadSurface('),1)
        self.assertEqual(source.count('g_AnmManager->LoadAnm('),1)
        self.assertEqual(source.count('g_AnmManager->ReleaseSurface('),2)
        self.assertEqual(source.count('g_AnmManager->SetupVertexBuffer();'),1)
        self.assertIn('extern AnmManager *g_AnmManager;',anm)

    def test_exact_original_caller_and_callee_symbols(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        owner=units['supervisor-added-callback']
        self.assertEqual((owner['target_address'],owner['size'],len(owner['relocations'])),
                         (0x00432090,238,27))
        by_offset={r['offset']:r for r in owner['relocations']}
        for offset,name,size in (
            (0x34,'anm-manager-load-surface',486),
            (0x46,'anm-manager-load-anm',59),
            (0x5c,'anm-manager-release-surface',97),
            (0x8d,'anm-manager-release-surface',97),
            (0xa2,'anm-manager-setup-vertex-buffer',472),
        ):
            with self.subTest(offset=offset):
                r=by_offset[offset];callee=units[name]
                self.assertEqual(callee['size'],size)
                self.assertEqual((r['type'],r['symbol'],r['target']),
                                 ('REL32',callee['symbol'],callee['target_address']))

if __name__=='__main__':unittest.main()

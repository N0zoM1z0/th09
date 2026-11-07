"""Exact ANM resource script start uses the native AnmManager +0x0C counter."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmRuntimeNativeStartedCounter(unittest.TestCase):
    def test_true_started_counter_and_anm_file_slot_stride(self):
        src=(ROOT/'src/AnmManagerRuntime.cpp').read_text()
        header=(ROOT/'src/AnmManager.hpp').read_text()
        reset=(ROOT/'src/AnmManagerStateReset.cpp').read_text()
        self.assertIn('struct AnmManagerRuntimeView : AnmManager',src)
        self.assertIn('unsigned char unknown024[0x38 - 0x24];',src)
        self.assertIn('AnmFileRuntimeSlot anmFiles[1];',src)
        self.assertIn('sizeof(AnmManager) == 0x24',src)
        self.assertIn('offsetof(AnmManager, scriptsStartedThisFrame0C) == 0x0C',src)
        self.assertIn('offsetof(AnmManagerRuntimeView, anmFiles) == 0x38',src)
        self.assertIn('sizeof(AnmFileRuntimeSlot) == 0x120',src)
        self.assertIn('g_AnmManager->scriptsStartedThisFrame0C++;',src)
        self.assertNotIn('reinterpret_cast<AnmManagerRuntimeView *>(g_AnmManager)->scriptsStartedThisFrame++',src)
        self.assertNotIn('int scriptsStartedThisFrame;',src)
        self.assertIn('int scriptsStartedThisFrame0C;',header)
        self.assertIn('scriptsStartedThisFrame0C = 0;',reset)

    def test_all_seven_independent_original_bound_runtime_leaves(self):
        with (ROOT/'config/match-units.toml').open('rb') as inp:units=tomllib.load(inp)['units']
        rows={n:v for n,v in units.items() if v['source']=='src/AnmManagerRuntime.cpp'}
        self.assertEqual(len(rows),7)
        self.assertEqual(sum(v.get('compare_size',v['size']) for v in rows.values()),507)
        for name,size in (('anm-loaded-set-and-execute-script',143),
                          ('anm-manager-sprite-has-texture',47),
                          ('anm-manager-draw-player-bullet',84),
                          ('anm-manager-set-interrupt-array',72),
                          ('anm-manager-execute-script-array',53)):
            with self.subTest(name=name):
                self.assertEqual(rows[name]['size'],size)
        self.assertEqual(len(rows['anm-manager-draw-player-bullet']['relocations']),13)

if __name__=='__main__':unittest.main()

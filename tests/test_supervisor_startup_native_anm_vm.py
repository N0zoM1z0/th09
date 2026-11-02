"""Original startup VM has the same +0x208 XYZ backing as the main ANM VM.

StartupAnmVm's original constructor ABI and StartupAnmLoaded callee spellings
must remain distinct. Exactness relies on focused VC7.1/TH09 PE comparisons.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeStartupVm(unittest.TestCase):
    def test_native_2a4_vm_replaces_separate_startup_memory_overlay(self):
        startup=(ROOT/'src/SupervisorStartup.cpp').read_text()
        native=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('#include "AnmVmLifecycle.hpp"',startup)
        self.assertIn('struct StartupAnmVm : AnmVm',startup)
        self.assertNotIn('unsigned char unknown000[0x208]',startup)
        self.assertIn('StartupAnmVm();',startup)
        self.assertIn('sizeof(StartupAnmVm) == 0x2A4',startup)
        self.assertIn('offsetof(AnmVmBase, pos) == 0x208',startup)
        self.assertIn('Float3 pos;',native)
        self.assertIn('AnmVmLifecycleSizeIs2A4',native)
        for c in ('x','y','z'):
            self.assertIn(f'StartupLayout()->startupVm584->pos.{c} = ',startup)
        self.assertIn('StartupLayout()->textAnm5CC->InitializeAndSetSprite(',startup)
        self.assertIn('StartupLayout()->startupVm584, 31);',startup)
        self.assertIn('new StartupAnmVm',startup)

    def test_native_boss_and_leaf_original_extents(self):
        with (ROOT/'config/match-units.toml').open('rb') as file:
            units=tomllib.load(file)['units']
        for name,size,fields in (
            ('supervisor-startup-thread',882,106),
            ('supervisor-added-callback',238,27),
            ('anm-vm-constructor',45,2),
            ('anm-manager-execute-script',9358,368),
        ):
            with self.subTest(unit=name):
                u=units[name]
                self.assertEqual((u['size'],len(u['relocations'])),(size,fields))
        self.assertEqual(units['anm-manager-execute-script']['compare_size'],9776)

if __name__=='__main__':
    unittest.main()

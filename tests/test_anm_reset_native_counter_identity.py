"""True ANM frame counters are +0x0C started / +0x10 executed / +0x14 state."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmResetNativeCounterIdentity(unittest.TestCase):
    def test_true_anm_counter_order_across_three_exact_sources(self):
        header=(ROOT/'src/AnmManager.hpp').read_text()
        reset=(ROOT/'src/AnmManagerStateReset.cpp').read_text()
        script=(ROOT/'src/AnmManagerExecuteScript.cpp').read_text()
        runtime=(ROOT/'src/AnmManagerRuntime.cpp').read_text()
        render=(ROOT/'src/AnmManagerRender.cpp').read_text()
        slots=(('scriptsStartedThisFrame0C','0x0C'),
               ('scriptsExecutedThisFrame10','0x10'),
               ('renderStateChangesThisFrame14','0x14'),
               ('flushesThisFrame18','0x18'))
        for name,offset in slots:
            with self.subTest(name=name):
                self.assertIn('int '+name+';',header)
                self.assertIn('offsetof(AnmManager, '+name+') == '+offset,reset)
        self.assertIn('this->scriptsExecutedThisFrame10++;',script)
        self.assertIn('g_AnmManager->scriptsStartedThisFrame0C++;',runtime)
        self.assertEqual(render.count('this->renderStateChangesThisFrame14++;'), 2)
        self.assertNotIn('struct AnmManagerStateResetView',reset)
        self.assertIn('reinterpret_cast<AnmManagerRenderCacheView *>(this)->cameraMode = 0xff;',reset)
        # Original reset is precisely +0x10, +0x14, +0x0C, +0x18.
        stores=[reset.index('    '+key+' = 0;') for key in (
             'scriptsExecutedThisFrame10','renderStateChangesThisFrame14',
             'scriptsStartedThisFrame0C','flushesThisFrame18')]
        self.assertEqual(stores,sorted(stores))
        self.assertIn('offsetof(AnmManagerRenderCacheView, cameraMode) == 0x12888',reset)

    def test_original_leaf_extent_and_interpreter_accounting(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        self.assertEqual(u['anm-manager-reset-frame-debug-info']['size'],15)
        self.assertEqual(u['anm-manager-clear-camera-settings']['size'],8)
        self.assertEqual((u['anm-manager-execute-script']['size'],
                          u['anm-manager-execute-script'].get('compare_size')),(9358,9776))
        self.assertEqual(len(u['anm-manager-execute-script']['relocations']),368)

if __name__=='__main__':unittest.main()

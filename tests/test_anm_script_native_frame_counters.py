"""ANM 9358-byte interpreter reads/writes real frame-debug counter storage."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmScriptNativeFrameCounters(unittest.TestCase):
    def test_one_anm_manager_allocation_and_true_counter_words(self):
        header=(ROOT/'src/AnmManager.hpp').read_text()
        script=(ROOT/'src/AnmManagerExecuteScript.cpp').read_text()
        for field in ('scriptsStartedThisFrame0C',
                      'scriptsExecutedThisFrame10',
                      'renderStateChangesThisFrame14',
                      'flushesThisFrame18'):
            with self.subTest(field=field):
                self.assertIn('int '+field+';',header)
        self.assertIn('unsigned char unknown000000[0x1C];',header)
        self.assertIn('int captureSurfaceIndex08;',header)
        self.assertIn('offsetof(AnmManager, scriptsExecutedThisFrame10) == 0x10',script)
        self.assertIn('this->scriptsExecutedThisFrame10++;',script)
        self.assertNotIn('struct AnmScriptExecutionCountersView',script)
        self.assertNotIn('reinterpret_cast<AnmScriptExecutionCountersView *>(this)',script)

    def test_giant_interpreter_and_four_independent_leaves_full_bounds(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        known=('anm-manager-execute-script','anm-vm-get-int-var',
               'anm-vm-get-float-var-ptr','anm-vm-get-int-var-ptr','anm-vm-get-float-var')
        exact=[(n,v) for n,v in units.items() if v['source']=='src/AnmManagerExecuteScript.cpp']
        self.assertEqual({n for n,v in exact},set(known))
        self.assertEqual((units['anm-manager-execute-script']['size'],
                          units['anm-manager-execute-script'].get('compare_size'),
                          len(units['anm-manager-execute-script']['relocations'])),
                         (9358,9776,368))
        self.assertEqual(sum(v.get('compare_size',v['size']) for n,v in exact),10496)
        self.assertEqual(units['anm-manager-reset-frame-debug-info']['size'],15)
        self.assertEqual(units['anm-manager-clear-camera-settings']['size'],8)

if __name__=='__main__':unittest.main()

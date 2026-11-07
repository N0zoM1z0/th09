"""Two exact ANM state render owners update native manager +0x14 counter."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmRenderTrueFrameStateCounter(unittest.TestCase):
    def test_native_anm_counter_is_shared_with_reset_and_script_owners(self):
        render=(ROOT/'src/AnmManagerRender.cpp').read_text()
        header=(ROOT/'src/AnmManager.hpp').read_text()
        reset=(ROOT/'src/AnmManagerStateReset.cpp').read_text()
        self.assertIn('offsetof(AnmManager, renderStateChangesThisFrame14) == 0x14',render)
        self.assertEqual(render.count('this->renderStateChangesThisFrame14++;'),2)
        self.assertNotIn('int renderStateChangesThisFrame;',render)
        self.assertIn('unsigned char unknown014[4];',render)
        self.assertIn('int renderStateChangesThisFrame14;',header)
        self.assertIn('renderStateChangesThisFrame14 = 0;',reset)
        self.assertIn('int scriptsExecutedThisFrame10;',header)

    def test_both_large_original_bound_render_states_and_full_owner_range(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        sources=[(n,v) for n,v in units.items() if v['source']=='src/AnmManagerRender.cpp']
        self.assertEqual(len(sources),16)
        self.assertEqual(sum(v.get('compare_size',v['size']) for n,v in sources),5814)
        self.assertEqual(units['anm-manager-set-render-state-for-vm3d']['size'],411)
        self.assertEqual(units['anm-manager-set-render-state-for-vm']['size'],188)
        self.assertEqual(units['anm-manager-draw-inner']['size'],1067)
        self.assertEqual(units['anm-manager-reset-frame-debug-info']['size'],15)

if __name__=='__main__':unittest.main()

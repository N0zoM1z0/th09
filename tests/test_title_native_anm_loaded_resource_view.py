"""The original Title sprite/script calls now share exact native AnmLoaded methods."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleNativeAnmLoadedResourceView(unittest.TestCase):
    def test_shared_native_resource_object_and_methods(self):
        source=(ROOT/'src/TitleScreen.cpp').read_text()
        native=(ROOT/'src/AnmManager.hpp').read_text()
        shared=(ROOT/'src/TitleScreenReplayView.hpp').read_text()
        self.assertIn('struct TitleAnmView : AnmLoaded {};',shared)
        self.assertNotIn('struct TitleAnmView : AnmLoaded {};',source)
        self.assertIn('sizeof(TitleAnmView) == sizeof(AnmLoaded)',source)
        self.assertIn('sizeof(TitleAnmView) == 0x1C',source)
        self.assertNotIn('void ExecuteAnmIdxArray(AnmVmView *vms, i32 start, i32 count);',source)
        self.assertNotIn('int SetSprite(AnmVmView *vm, i32 spriteIndex);',source)
        self.assertIn('class AnmLoaded',native)
        self.assertIn('void ExecuteAnmIdxArray(AnmVm *vm, int scriptIndex, int count);',native)
        self.assertIn('int SetSprite(AnmVm *vm, int spriteIndex);',native)

    def test_actual_original_bound_method_references(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        sprite=units['anm-loaded-set-sprite']
        array=units['anm-loaded-execute-anm-idx-array']
        self.assertEqual((sprite['size'],sprite['target_address']),(189,0x00436AC0))
        self.assertEqual((array['size'],array['target_address']),(66,0x00439630))
        group=[v for v in units.values() if v['source']=='src/TitleScreen.cpp']
        counts={sprite['symbol']:0,array['symbol']:0}
        for unit in group:
            for r in unit['relocations']:
                if r['symbol'] in counts:
                    expected=sprite if r['symbol']==sprite['symbol'] else array
                    self.assertEqual((r['target'],r['type'],r.get('addend',0)),
                                     (expected['target_address'],'REL32',0))
                    counts[r['symbol']]+=1
        self.assertEqual(counts[sprite['symbol']],29)
        self.assertEqual(counts[array['symbol']],2)

if __name__=='__main__':unittest.main()

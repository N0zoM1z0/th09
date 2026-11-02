"""ANM LoadTextureData/ReadAnmEntries use the single exact 0x1C resource.

Target-dependent byte equivalence is checked via the focused native build
and original PE/COFF comparisons, rather than source naming alone.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeLoadedSlotInResourceLoader(unittest.TestCase):
    def test_loader_reuses_real_anm_loaded_without_parallel_full_record(self):
        source=(ROOT/'src/AnmManagerLoad.cpp').read_text()
        header=(ROOT/'src/AnmManager.hpp').read_text()
        self.assertNotIn('struct AnmLoadedLoadView',source)
        self.assertNotIn('AnmLoadedLoadView',source)
        self.assertIn('AnmLoadedLoadSizeIs1C',source)
        self.assertIn('AnmLoadedNativeSizeIs1C',header)
        self.assertIn('AnmLoaded loaded;',source)
        for fld,offset in (
            ('rawData04','0x04'),('totalEntries08','0x08'),
            ('sprites0C','0x0C'),('scripts10','0x10'),
            ('textures14','0x14'),('postloadEntryNumber18','0x18')):
            with self.subTest(field=fld):
                self.assertIn('offsetof(AnmLoaded, '+fld+') == '+offset,source)
                self.assertIn(fld,header)
        self.assertIn('anmLoaded->textures14[entryNumber]',source)
        self.assertIn('anmLoaded->scripts10[currentScriptNumber]',source)
        self.assertIn('anmLoaded->rawData04',source)
        self.assertIn('slot->loaded.postloadEntryNumber18',source)
        self.assertNotIn('slot->loaded.rawData ',source)

    def test_full_anm_loading_and_interpreter_owners_remain_frozen(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        pin={
            'anm-manager-load-texture-data':(558,16),
            'anm-manager-read-anm-entries':(390,17),
            'anm-manager-release-anm':(163,9),
            'anm-loaded-load-sprite':(138,0),
            'anm-manager-constructor':(341,29),
            'anm-manager-execute-script':(9358,368),
        }
        for name,(size,field_count) in pin.items():
            with self.subTest(name=name):
                u=units[name]
                self.assertEqual((u['size'],len(u['relocations'])),(size,field_count))
        self.assertEqual(units['anm-manager-execute-script']['compare_size'],9776)

if __name__=='__main__':
    unittest.main()

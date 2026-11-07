"""All 52 TitleScreen.cpp ANM-manager global references bind native g_AnmManager."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleNativeAnmManagerGlobalIdentity(unittest.TestCase):
    def test_global_storage_and_noncanonical_help_text_abi_distinguished(self):
        src=(ROOT/'src/TitleScreen.cpp').read_text()
        shared=(ROOT/'src/AnmManager.hpp').read_text()
        self.assertIn('extern AnmManager *g_AnmManager;',shared)
        self.assertNotIn('g_TitleAnmManager',src)
        self.assertNotIn('extern TitleAnmManagerView *g_TitleAnmManager;',src)
        self.assertIn('g_AnmManager->PreloadAnm(15, "title01.anm")',src)
        self.assertIn('g_AnmManager->captureSurfaceIndex08',src)
        self.assertIn('g_AnmManager->ExecuteScript(&vms[i]);',src)
        # No guessed original signature: the help-text ABI is separately bound.
        self.assertIn('DrawTitleHelpText(static_cast<TitleAnmManagerView *>(g_AnmManager)',src)
        self.assertIn('extern void __cdecl DrawTitleHelpText(TitleAnmManagerView *manager',src)

    def test_all_52_true_manager_root_fields_preserve_original_address(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        native='?g_AnmManager@@3PAVAnmManager@@A'
        old='?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A'
        native_count=0
        for name,owner in u.items():
            if owner['source']!='src/TitleScreen.cpp':continue
            for row in owner.get('relocations',[]):
                self.assertNotEqual(row['symbol'],old,(name,row['offset']))
                if row['symbol']!=native:continue
                self.assertEqual((row['type'],row['target'],row.get('addend',0)),
                                 ('DIR32',0x004DC550,0))
                native_count+=1
        self.assertEqual(native_count,52)
        self.assertTrue(any(r['symbol']==native and r['target']==0x004DC550
                            for n,owner in u.items() if owner['source']!='src/TitleScreen.cpp'
                            for r in owner.get('relocations',[])))

if __name__=='__main__':unittest.main()

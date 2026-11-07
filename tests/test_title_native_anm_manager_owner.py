"""Title's original Anm manager references now borrow exact native AnmManager APIs."""
from copy import deepcopy
from pathlib import Path
import importlib.util
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('cursor_owner',ROOT/'scripts/inspect-title-character-cursors.py')
cursor=importlib.util.module_from_spec(spec);spec.loader.exec_module(cursor)

class TitleNativeAnmManagerOwner(unittest.TestCase):
    def test_manager_has_no_second_prefix_or_fake_method_set(self):
        src=(ROOT/'src/TitleScreen.cpp').read_text()
        native=(ROOT/'src/AnmManager.hpp').read_text()
        self.assertIn('struct TitleAnmManagerView : AnmManager {};',src)
        self.assertIn('sizeof(TitleAnmManagerView) == sizeof(AnmManager)',src)
        self.assertIn('offsetof(TitleAnmManagerView, captureSurfaceIndex08) == 0x08',src)
        self.assertIn('g_TitleAnmManager->captureSurfaceIndex08',src)
        self.assertNotIn('u8 unknown000[8];\n    i32 captureSurfaceIndex;',src)
        self.assertNotIn('void SetInterruptArray(AnmVmView *vms, i32 count, i32 interrupt);',src)
        self.assertNotIn('int LoadSurface(i32 slot, const char *path);',src)
        self.assertIn('offsetof(AnmManager, captureSurfaceIndex08) == 0x08',native)
        self.assertIn('float frameShakeX1C;',native)
        self.assertIn('float frameShakeY20;',native)

    def test_all_emitted_native_method_addresses_are_independent_exact_owners(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        actual=[r for name,u in units.items() if u['source']=='src/TitleScreen.cpp'
                for r in u.get('relocations',[]) if r['type']=='REL32' and 'AnmManager@@' in r['symbol']]
        expected={
            'anm-manager-set-interrupt-array':0x00436DE0,
            'anm-manager-load-surface':0x0043CAE0,
            'anm-manager-execute-script-array':0x00439560,
            'anm-manager-preload-anm':0x0043C7A0,
            'anm-manager-preload-surface':0x0043CCD0,
            'anm-manager-release-anm':0x0043CA30,
            'anm-manager-release-surface':0x0043BF30,
            'anm-manager-execute-script':0x00436F30,
        }
        for name,address in expected.items():
            with self.subTest(callee=name):
                leaf=units[name]
                self.assertEqual(leaf['target_address'],address)
                sites=[r for r in actual if r['symbol']==leaf['symbol']]
                self.assertTrue(sites)
                for r in sites:
                    self.assertEqual((r['target'],r['type'],r.get('addend',0)),(address,'REL32',0))
        self.assertEqual(len(actual),51)

    def test_cursors_require_exact_true_native_script_callee(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        cursor.require_native_cursor_script_leaf(units)
        self.assertEqual(cursor.SCRIPT_SYMBOL,units['anm-manager-execute-script']['symbol'])
        for field,wrong in (('symbol','?FakeExecute@AnmManager@@QAEHXZ'),
                            ('target_address',0x00436F34),('size',9357),
                            ('source','src/FakeAnmManager.cpp')):
            forged=deepcopy(units)
            forged['anm-manager-execute-script'][field]=wrong
            with self.subTest(field=field),self.assertRaises(ValueError):
                cursor.require_native_cursor_script_leaf(forged)

if __name__=='__main__':unittest.main()

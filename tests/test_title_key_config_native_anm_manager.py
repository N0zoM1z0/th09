"""The exact 2803-byte key-config boss uses genuine AnmManager calls/root."""
import tomllib
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]

class TitleKeyConfigNativeAnmManager(unittest.TestCase):
    def test_one_real_manager_with_frozen_help_text_abi(self):
        src=(ROOT/'src/TitleScreenKeyConfig.cpp').read_text()
        self.assertIn('#include "AnmManager.hpp"',src)
        self.assertIn('struct TitleKeyAnmManagerView : AnmManager {};',src)
        self.assertIn('sizeof(TitleKeyAnmManagerView) == sizeof(AnmManager)',src)
        self.assertNotIn('extern TitleKeyAnmManagerView *g_TitleAnmManager;',src)
        self.assertNotIn('g_TitleAnmManager->',src)
        self.assertIn('g_AnmManager->SetInterruptArray(reinterpret_cast<AnmVm *>(vms), vmCount, 4)',src)
        self.assertIn('g_AnmManager->ExecuteScriptArray(reinterpret_cast<AnmVm *>(vms), vmCount)',src)
        self.assertIn('static_cast<TitleKeyAnmManagerView *>(g_AnmManager)',src)
        self.assertIn('TitleKeyAnmManagerView *manager,',src)

    def test_five_actual_original_bound_symbols_keep_physical_targets(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        boss=u['title-screen-update-key-config'];rows={r['offset']:r for r in boss['relocations']}
        self.assertEqual((boss['size'],len(rows)),(2803,118))
        for off in (0x37,0x54,0x1B3):
            with self.subTest(off=off):
                r=rows[off]
                self.assertEqual((r['symbol'],r['target'],r['type'],r.get('addend',0)),
                                 ('?g_AnmManager@@3PAVAnmManager@@A',0x004DC550,'DIR32',0))
        for off,name in ((0x4C,'anm-manager-set-interrupt-array'),
                         (0x5F,'anm-manager-execute-script-array')):
            with self.subTest(callee=name):
                row=rows[off];leaf=u[name]
                self.assertEqual((row['symbol'],row['target'],row['type']),
                                 (leaf['symbol'],leaf['target_address'],'REL32'))
        self.assertEqual(u['title-screen-swap-duplicate-character-index']['size'],106)
        # Leave the unproven nominal helper signature original-bound.
        self.assertEqual(rows[0x1B8]['target'],0x0043BE50)
        self.assertIn('TitleKeyAnmManagerView',rows[0x1B8]['symbol'])

if __name__=='__main__':unittest.main()

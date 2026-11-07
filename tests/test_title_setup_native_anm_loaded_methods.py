"""The exact TitleSetupThread uses genuine AnmLoaded VM/sprite providers."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleSetupNativeAnmLoadedMethods(unittest.TestCase):
    def test_true_loaded_resource_receiver_and_vm_arguments(self):
        source=(ROOT/'src/TitleScreen.cpp').read_text()
        self.assertEqual(source.count('reinterpret_cast<AnmLoaded *>(g_TitleSupervisor.textAnm5CC)->'),3)
        self.assertIn('reinterpret_cast<AnmVm *>(&title->embeddedVms[i]), 5',source)
        self.assertIn('reinterpret_cast<AnmVm *>(&g_TitleScreen->embeddedVms[i])',source)
        self.assertIn('reinterpret_cast<AnmVm *>(&g_TitleScreen->embeddedVms[i + 35])',source)
        self.assertNotIn('reinterpret_cast<TitleAnmView *>(g_TitleSupervisor.textAnm5CC)',source)

    def test_independent_original_bound_method_targets(self):
        with (ROOT/'config/match-units.toml').open('rb') as inp:units=tomllib.load(inp)['units']
        setup=units['title-screen-setup-thread'];rows={r['offset']:r for r in setup['relocations']}
        self.assertEqual((setup['size'],len(rows)),(534,41))
        for off,leafname in ((0x8D,'anm-loaded-set-and-execute-script-idx'),
                             (0xB2,'anm-loaded-set-sprite'),
                             (0xCE,'anm-loaded-set-sprite')):
            with self.subTest(off=off):
                leaf=units[leafname]
                self.assertEqual((rows[off]['symbol'],rows[off]['target'],rows[off]['type']),
                                 (leaf['symbol'],leaf['target_address'],'REL32'))
        self.assertEqual(units['title-screen-start-menu']['size'],1560)

if __name__=='__main__':unittest.main()

"""One original-bound 0x2A4 ANM VM class across lifecycle and runtime calls.

Protects the genuine AnmVm method ABI and recovered bitfield/byte offsets.
Canonical proof comes from focused VC7.1 original-PE/COFF replays, not text.
"""
import tomllib
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]


class AnmRuntimeNativeVm(unittest.TestCase):
    def test_runtime_uses_single_vm_lifecycle_type_with_byte_boundaries(self):
        lifecycle=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        runtime=(ROOT/'src/AnmManagerRuntime.cpp').read_text()
        self.assertIn('#include "AnmVmLifecycle.hpp"',runtime)
        self.assertNotIn('struct AnmVm\n{',runtime)
        self.assertIn('struct AnmVm : AnmVmBase',lifecycle)
        for member in ('updateRotation','updateScale','usePosOffset','flip',
                       'stopped','playerBulletHitAnimationType',
                       'beginningOfScript','baseSpriteIndex'):
            with self.subTest(member=member):
                self.assertIn(member,lifecycle)
        self.assertIn('AnmVmBaseSizeIs228',lifecycle)
        self.assertIn('AnmVmLifecycleSizeIs2A4',lifecycle)
        self.assertIn('offsetof(AnmVmBase, playerBulletHitAnimationType) == 0x200',lifecycle)
        self.assertIn('offsetof(AnmVmBase, beginningOfScript) == 0x21C',lifecycle)
        self.assertIn('offsetof(AnmVmBase, baseSpriteIndex) == 0x218',lifecycle)
        self.assertIn('vm->flip = 0;',runtime)
        self.assertIn('vm->beginningOfScript = beginningOfScript;',runtime)
        self.assertIn('vm->baseSpriteIndex = vm->activeSpriteIndex;',runtime)
        self.assertIn('void AnmVm::SetInterrupt(i16 interrupt)',runtime)

    def test_focused_native_runtime_exact_manifest_units(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        names=('anm-vm-set-interrupt','anm-manager-sprite-has-texture',
               'anm-manager-draw-player-bullet','anm-manager-set-interrupt-array',
               'anm-manager-execute-script-array','anm-loaded-set-and-execute-script',
               'anm-loaded-execute-anm-idx-array')
        self.assertEqual(len(names),7)
        for n in names:
            with self.subTest(unit=n):
                self.assertEqual(units[n]['source'],'src/AnmManagerRuntime.cpp')
                self.assertGreater(units[n]['size'],0)
        self.assertEqual((units['anm-vm-set-interrupt']['target_address'],
                          units['anm-vm-set-interrupt']['size']),
                         (0x00406790,18))


if __name__=='__main__':
    unittest.main()

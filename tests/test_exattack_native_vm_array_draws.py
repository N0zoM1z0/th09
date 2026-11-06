"""Check ExAttack's reconstructed native VM owner and unchanged PE boundaries."""
from pathlib import Path
import unittest
import tomllib

ROOT=Path(__file__).resolve().parents[1]

class ExAttackNativeVmArrayDraws(unittest.TestCase):
    def test_both_draws_use_the_same_native_anm_vm_array(self):
        source=(ROOT/'src/ExAttackDraw.cpp').read_text()
        native=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('ExAttackNativeVmTypeAndStride',source)
        self.assertIn('offsetof(AnmVmBase, type) == 0x1FC',source)
        self.assertIn('sizeof(AnmVm) == 0x2A4',source)
        self.assertIn('short type;',native)
        self.assertEqual(source.count('record->vms1C[vmIndex].type'),2)
        self.assertEqual(source.count('record->vms1C[vmIndex].SetZRotation'),2)
        self.assertEqual(source.count('&record->vms1C[vmIndex]'),2)
        self.assertNotIn('vmOffset',source)
        self.assertNotIn('reinterpret_cast<unsigned char *>(record->vms1C)',source)

    def test_original_function_owner_sizes_and_field_counts(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for name,addr,size,fields in (
            ('exattack-on-draw0',0x00415460,286,9),
            ('exattack-on-draw1',0x00415580,193,5),
        ):
            with self.subTest(unit=name):
                spec=units[name]
                self.assertEqual((spec['target_address'],spec['size'],len(spec['relocations'])),
                                 (addr,size,fields))
                self.assertEqual(spec['source'],'src/ExAttackDraw.cpp')

if __name__=='__main__':unittest.main()

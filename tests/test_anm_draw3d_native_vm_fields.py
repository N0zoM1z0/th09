"""Native 3D renderer fields must match the same original VM and ANM."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class AnmDraw3dNativeFields(unittest.TestCase):
    def test_real_vm_fields_and_manager_shake(self):
        source = (ROOT/'src/AnmManagerDraw3D.cpp').read_text()
        self.assertIn('Draw3DNativeVmFieldOffsets', source)
        for name, offset in [('rotation','0x000'),('scale','0x018'),
                             ('spriteSize','0x028'),('uvScrollPos','0x030'),
                             ('matrix1','0x130'),('matrix2','0x170'),
                             ('matrix3','0x1B0'),('flagsWord','0x1F8'),
                             ('pos','0x208'),('loadedSprite','0x224')]:
            with self.subTest(name=name):
                self.assertIn('offsetof(AnmVmBase, '+name+') == '+offset,source)
        for name in ['rotation','scale','spriteSize','uvScrollPos','flagsWord','pos','loadedSprite']:
            self.assertNotIn('drawVm->'+name,source)
        self.assertIn('this->frameShakeX1C',source)
        self.assertIn('this->frameShakeY20',source)
        self.assertIn('drawVm->matrix2',source)  # nominal D3DX matrix identity remains open

    def test_complete_original_owner(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            owner=tomllib.load(stream)['units']['anm-manager-draw-3d']
        self.assertEqual(owner['source'],'src/AnmManagerDraw3D.cpp')
        self.assertEqual(owner.get('compare_size',owner['size']),811)
        self.assertEqual(len(owner['relocations']),26)


if __name__ == '__main__':
    unittest.main()

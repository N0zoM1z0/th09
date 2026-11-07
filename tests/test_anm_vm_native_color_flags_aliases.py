"""The original 0x2A4-byte ANM VM has native color-byte and flag-word aliases."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmVmNativeColorFlagsAliases(unittest.TestCase):
    def test_byte_and_word_representations_are_one_original_allocation(self):
        s=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('unsigned char color1Bytes[4];',s)
        self.assertIn('unsigned int flagsWord;',s)
        self.assertIn('unsigned int flags;',s)
        self.assertIn('unsigned short flagsLow; unsigned short flagsHigh;',s)
        for field,offset in (('color1Bytes','0x1F0'),('color1Script','0x1F0'),
                             ('flagsWord','0x1F8'),('flags','0x1F8'),
                             ('flagsLow','0x1F8'),('flagsHigh','0x1FA')):
            self.assertIn('offsetof(AnmVmBase, '+field+') == '+offset,s)
        self.assertIn('sizeof(AnmVm) == 0x2A4',s)

    def test_existing_native_owners_stay_original_bound(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        for name,size in (('anm-vm-set-interrupt',18),
                          ('anm-manager-set-interrupt-array',72),
                          ('card-attack-on-draw',334)):
            self.assertEqual(units[name]['size'],size)

if __name__=='__main__':unittest.main()

"""Native ANM runtime resource owner used by exact scripting and VM methods.

Compiler-specific private labels remain non-crediting; complete original PE
bytes and immutable call targets are separately checked by match-unit replays.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeAnmRuntimeLoadedSlot(unittest.TestCase):
    def test_native_slot_removes_runtime_specific_storage(self):
        src=(ROOT/'src/AnmManagerRuntime.cpp').read_text()
        header=(ROOT/'src/AnmManager.hpp').read_text()
        self.assertNotIn('struct AnmLoadedRuntimeView',src)
        self.assertIn('typedef char AnmRuntimeNativeLoadedFields[',src)
        for fld,offset in (('anmIdx','0x00'),('scripts10','0x10'),
                           ('numberEntriesToBeLoaded18','0x18')):
            self.assertIn('offsetof(AnmLoaded, '+fld+') == '+offset,src)
            self.assertIn(fld,header)
        self.assertIn('AnmLoaded *self = this;',src)
        self.assertIn('self->numberEntriesToBeLoaded18 != 0',src)
        self.assertIn('vm->anmFileIndex = static_cast<short>(self->anmIdx);',src)
        self.assertIn('vm->anmFile = this;',src)
        self.assertIn('vm->beginningOfScript = beginningOfScript;',src)

    def test_native_exact_callees_keep_original_signatures(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        pins={'anm-loaded-set-and-execute-script':(143,5),
              'anm-loaded-execute-anm-idx-array':(66,1),
              'anm-vm-set-interrupt':(18,0),
              'anm-manager-draw-player-bullet':(84,13)}
        for name,(size,fields) in pins.items():
            with self.subTest(unit=name):
                u=units[name]
                self.assertEqual((u['size'],len(u['relocations'])),(size,fields))
                self.assertEqual(u['source'],'src/AnmManagerRuntime.cpp')

if __name__=='__main__':
    unittest.main()

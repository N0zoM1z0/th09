"""Two native signed 16-bit halves share the existing four-byte ECL word.

Exactness is established by complete original-bound RunEcl and source-cold
PE/COFF comparisons, not merely by this static type/index check.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class EclNativeWireHalfWordIndexing(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.opcodes = (ROOT/'src/EclOpcodes.hpp').read_text()
        cls.source = (ROOT/'src/EclRunState.inl').read_text()

    def test_native_halfwords_share_int_float_byte_union(self):
        for token in ('union Th09EclWireArgument', 'int integer;',
                      'float real;', 'unsigned char bytes[4];',
                      'short halfWords[2];', 'Th09EclWireArgumentSizeIs04',
                      'Th09EclWireHalfWordsAreTwoPerWord',
                      'Th09EclWireArgumentsAt0C'):
            with self.subTest(token=token):
                self.assertIn(token, self.opcodes)
        self.assertIn('sizeof(((Th09EclWireArgument *)0)->halfWords[0]) == 2',
                      self.opcodes)
        self.assertIn('TH09_ECL_OPCODE_SET_DEATH_CALLBACK = 130',self.opcodes)
        self.assertIn('TH09_ECL_OPCODE_INTERRUPT_SECONDARY_ANM = 150',self.opcodes)

if __name__ == '__main__':
    unittest.main()

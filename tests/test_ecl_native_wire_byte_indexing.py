"""TH09 original ECL arguments are four-byte union words, not ad-hoc bytes.

Original-bound 15,564-byte boss replay is the code-generation evidence;
these tests only guard the source-facing type's indexing and probe invariants.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class EclNativeWireByteIndexing(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.opcodes = (ROOT / 'src/EclOpcodes.hpp').read_text()
        cls.state = (ROOT / 'src/EclRunState.inl').read_text()
        cls.control = (ROOT / 'src/EclRunControl.inl').read_text()

    def test_one_native_four_byte_word_shared_by_boss_operands(self):
        self.assertIn('union Th09EclWireArgument', self.opcodes)
        for part in ('int integer;', 'float real;', 'unsigned char bytes[4];',
                     'Th09EclWireArgumentSizeIs04',
                     'Th09EclWireArgument arguments[0];',
                     'Th09EclWireArgumentsAt0C'):
            self.assertIn(part, self.opcodes)
        self.assertIn('instruction->arguments[operandIndex].integer', self.control)
        self.assertIn('instruction->arguments[operandIndex].real', self.control)

    def test_byte_accessor_flattens_words_without_raw_address_cast(self):
        s = self.state
        self.assertIn('instruction->arguments[byteOffset / sizeof(Th09EclWireArgument)]', s)
        self.assertIn('.bytes[byteOffset % sizeof(Th09EclWireArgument)]', s)
        self.assertNotIn('reinterpret_cast<unsigned char *>(instruction->arguments)[byteOffset]',s)
        for offset in range(128):
            with self.subTest(byte_offset=offset):
                word, byte = divmod(offset, 4)
                self.assertEqual(word * 4 + byte, offset)
                self.assertLess(byte, 4)
        # The current opcode consumers use byte offset zero, not negative
        # header-relative offsets. Nonzero byte offsets remain representable.
        self.assertEqual(s.count('Th09EclRunState::RawByte(instruction, 0)'), 5)

    def test_timeout_bit_remains_masked_xor_not_nonlocal_flag_clear(self):
        self.assertIn('ENEMY_STATE_TIMEOUT_SPELL = 0x01000000',self.state)
        self.assertIn('return old ^ ((old ^ (static_cast<unsigned int>(value) << 24)) &',self.state)
        self.assertIn('Th09EclRunState::AssignTimeoutSpellBit(enemy->primaryFlags337C, value)',self.state)


if __name__ == '__main__':
    unittest.main()

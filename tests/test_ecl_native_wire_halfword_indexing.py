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

    def test_raw_short_uses_true_argument_and_half_indices(self):
        source=self.source
        start=source.index('inline short &RawShort(')
        end=source.index('\n}',start)
        body=source[start:end]
        self.assertIn('instruction->arguments[byteOffset / sizeof(Th09EclWireArgument)]',body)
        self.assertIn('.halfWords[(byteOffset % sizeof(Th09EclWireArgument)) / sizeof(short)]',body)
        self.assertNotIn('reinterpret_cast<unsigned char *>(instruction->arguments)',body)
        self.assertEqual(source.count('Th09EclRunState::RawShort(instruction, 0)'),1)
        self.assertEqual(source.count('Th09EclRunState::RawShort(instruction, 4)'),1)
        for offset in range(0,128,2):
            with self.subTest(offset=offset):
                word=offset//4
                half=(offset%4)//2
                self.assertEqual(4*word+2*half,offset)
                self.assertIn(half,(0,1))

    def test_source_probe_recognizes_only_reviewed_native_word_extensions(self):
        import importlib.util
        path=ROOT/'scripts/probe-runecl-raw-context.py'
        spec=importlib.util.spec_from_file_location('runecl_halfword_probe',path)
        probe=importlib.util.module_from_spec(spec)
        spec.loader.exec_module(probe)
        self.assertIn('short halfWords[2];',probe.WIRE_ARGUMENT_NATIVE_HALFWORDS)
        inputs=probe.collect_inputs()
        self.assertEqual(probe.reviewed_wire_storage(inputs),None)
        # Reject size/width changes even when the rest of the native union
        # retains the old byte/integer/float members.
        bad=dict(inputs)
        bad['EclOpcodes.hpp']=bad['EclOpcodes.hpp'].replace(
            b'short halfWords[2];', b'int halfWords[2];')
        with self.assertRaisesRegex(ValueError,'typed wire storage'):
            probe.reviewed_wire_storage(bad)

    def test_wire_value_semantics_not_changed_for_timeout_flag(self):
        self.assertIn('AssignTimeoutSpellBit(enemy->primaryFlags337C, value)',self.source)
        self.assertIn('ENEMY_STATE_TIMEOUT_SPELL = 0x01000000',self.source)
        self.assertIn('RawByte(instruction, 0)',self.source)


if __name__ == '__main__':
    unittest.main()

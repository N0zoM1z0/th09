"""A real literal definition must agree with its name and original PE value."""
import importlib.util
from pathlib import Path
import struct
from types import SimpleNamespace as NS
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('effect34_literal_guard',
                                             ROOT / 'scripts/inspect-effect-type34.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


def literal_object(bits=0x40490FDB):
    header = struct.pack('<HHIIIHH', 0x14C, 1, 0, 0, 0, 0, 0)
    section = struct.pack('<8sIIIIIIHHI', b'.rdata\0\0', 0, 0, 4, 60,
                          0, 0, 0, 0, 0x40000040)
    return header + section + struct.pack('<I', bits)


class Effect34LiteralDefinitions(TestCase):
    def setUp(self):
        self.row = dict(symbol='__real@40490fdb', addend=0,
                        symbol_section=1, symbol_value=0)
        self.original = struct.pack('<I', 0x40490FDB)

    def test_actual_definition_matches_name_and_original(self):
        data = literal_object()
        self.assertEqual(audit.scalar_payload(data, self.row), self.original)
        audit.require_scalar_definition(data, self.row, self.original)

    def test_wrong_definition_or_original_fails_despite_unchanged_symbol(self):
        with self.assertRaisesRegex(ValueError, 'scalar definition differs'):
            audit.require_scalar_definition(literal_object(0x3F800000),
                                            self.row, self.original)
        with self.assertRaisesRegex(ValueError, 'scalar definition differs'):
            audit.require_scalar_definition(literal_object(), self.row,
                                            struct.pack('<I', 0x3F800000))

    def test_undefined_symbol_bad_provenance_and_partial_payload_fail(self):
        for change in ({'symbol_section': 0}, {'symbol_section': 2},
                       {'symbol_section': True}, {'symbol_value': -1},
                       {'symbol_value': 1}, {'symbol_value': False}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                audit.scalar_payload(literal_object(), {**self.row, **change})
        for data in (b'', literal_object()[:19], literal_object()[:-1]):
            with self.subTest(length=len(data)), self.assertRaises(ValueError):
                audit.scalar_payload(data, self.row)
        for offset, value, fmt in ((0, 0x8664, '<H'), (16, 1, '<H'),
                                   (40, 0, '<I'), (40, 1000, '<I')):
            data = bytearray(literal_object())
            struct.pack_into(fmt, data, offset, value)
            with self.subTest(offset=offset, value=value), self.assertRaises(ValueError):
                audit.scalar_payload(data, self.row)

    def test_nonzero_literal_addend_cannot_borrow_definition(self):
        with self.assertRaisesRegex(ValueError, 'scalar definition differs'):
            audit.require_scalar_definition(literal_object(),
                                            {**self.row, 'addend': 4}, self.original)


class EffectActualOperandCensus(TestCase):
    def test_indexed_global_is_counted_and_stack_displacement_is_not(self):
        api = NS(x86=NS(X86_OP_IMM=2, X86_OP_MEM=3))
        def instruction(address, displacement):
            return NS(address=address, mnemonic='mov', disp_size=4, disp_offset=2,
                      imm_size=0, operands=[NS(type=3, mem=NS(base=1, index=2,
                                                             disp=displacement))])
        decoded = [instruction(0x444AF0, 0x4A7D94), instruction(0x444AF6, -32)]
        self.assertEqual(audit.addressed_fields(decoded, 0x444AF0, api),
                         [dict(offset=2, type='DIR32', destination=0x4A7D94)])

    def test_direct_call_is_one_relative_field(self):
        api = NS(x86=NS(X86_OP_IMM=2, X86_OP_MEM=3))
        call = NS(address=0x444AF0, mnemonic='call', imm_size=4, imm_offset=1,
                  disp_size=0, operands=[NS(type=2, imm=0x401680)])
        self.assertEqual(audit.addressed_fields([call], 0x444AF0, api),
                         [dict(offset=1, type='REL32', destination=0x401680)])

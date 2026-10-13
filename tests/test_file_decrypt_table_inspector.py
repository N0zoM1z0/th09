"""Pure fail-closed checks for complete TH09 FileSystem decryption code operands."""

import importlib.util
from pathlib import Path
import struct
from types import SimpleNamespace
import unittest

spec = importlib.util.spec_from_file_location(
    'file_decrypt_diagnostic',
    Path(__file__).resolve().parents[1] / 'scripts' / 'inspect-file-decrypt-table.py')
D = importlib.util.module_from_spec(spec)
spec.loader.exec_module(D)


class DecryptTableDiagnosticTests(unittest.TestCase):
    @staticmethod
    def entries():
        bases = {
            0x004A1EC0: ('?g_CryptSignature@FileSystem@@3PAEA', 0),
            0x004A1EC1: ('?g_CryptSignature@FileSystem@@3PAEA', 1),
            0x004A1EC2: ('?g_CryptSignature@FileSystem@@3PAEA', 2),
            0x004A1E60: ('?g_DecryptParams@FileSystem@@3PAUDecryptParams@1@A', 0),
            0x004A1E6C: ('?g_DecryptParams@FileSystem@@3PAUDecryptParams@1@A', 12),
            0x004A1E68: ('?g_DecryptParams@FileSystem@@3PAUDecryptParams@1@A', 8),
            0x004A1E64: ('?g_DecryptParams@FileSystem@@3PAUDecryptParams@1@A', 4),
            0x004A1E62: ('?g_DecryptParams@FileSystem@@3PAUDecryptParams@1@A', 2),
            0x004A1E61: ('?g_DecryptParams@FileSystem@@3PAUDecryptParams@1@A', 1),
            0x0042C180: ('?Decrypt@FileSystem@@YIPAEPAEHEEHH@Z', 0),
            0x004AD018: ('?g_ZunMemory@@3VZunMemory@@A', 0),
            0x00401360: ('?Free@ZunMemory@@QAEXPAX@Z', 0),
        }
        return [
            dict(offset=4*i, type=kind, symbol=bases[dest][0],
                 addend=bases[dest][1])
            for i, (kind, dest) in enumerate(D.FIELD_IDENTITIES)
        ]

    def test_twelve_actual_fields_bind_to_independent_destinations(self):
        raw = bytes(70)
        linked, fields = D.bind_candidate(raw, self.entries())
        self.assertEqual(raw, bytes(70))
        self.assertEqual(len(fields), 12)
        self.assertEqual([(kind, dest) for _, kind, dest in fields], D.FIELD_IDENTITIES)
        self.assertEqual(struct.unpack_from('<I', linked, 0)[0], 0x004A1EC0)
        self.assertEqual(struct.unpack_from('<I', linked, 4)[0], 0x004A1EC1)
        self.assertEqual(struct.unpack_from('<I', linked, 9*4)[0],
                         (0x0042C180 - D.BASE - 9*4 - 4) & 0xFFFFFFFF)
        self.assertEqual(linked[48:], raw[48:])

    def test_unknown_symbols_displaced_addends_and_types_reject(self):
        cases = [
            {'symbol': '?Unknown@@3HA'},
            {'addend': 7},
            {'type': 'REL32'},
            {'type': 'DIR16'},
        ]
        for change in cases:
            records = self.entries()
            records[0].update(change)
            with self.subTest(change=change), self.assertRaises(ValueError):
                D.bind_candidate(bytes(70), records)

    def test_missing_duplicate_out_of_order_and_owner_escape_reject(self):
        with self.assertRaisesRegex(ValueError, 'twelve'):
            D.bind_candidate(bytes(70), self.entries()[:-1])
        for edits in [(0,4,4), (0,4,5), (0,4,56), (1,0,0)]:
            records = self.entries()
            record_index, other_index, other_value = edits
            records[record_index]['offset'] = other_value
            with self.subTest(edits=edits), self.assertRaises(ValueError):
                D.bind_candidate(bytes(50), records)

    def test_decoder_includes_indexed_absolute_data_but_not_stack_displacements(self):
        imm, mem = 1, 2
        capstone = SimpleNamespace(x86=SimpleNamespace(X86_OP_IMM=imm, X86_OP_MEM=mem))
        def op(tp, literal=0, disp=0, base=0):
            return SimpleNamespace(type=tp, imm=literal,
                                   mem=SimpleNamespace(disp=disp, base=base))
        def ins(offset, mnemonic, operands, isize=0, ioff=0, dsize=0, doff=0):
            return SimpleNamespace(address=D.BASE+offset, mnemonic=mnemonic,
                                   operands=operands, imm_size=isize, imm_offset=ioff,
                                   disp_size=dsize, disp_offset=doff)
        items = [
            ins(0, 'movzx', [op(mem, disp=0x004A1E6C, base=1)], dsize=4, doff=3),
            ins(7, 'mov', [op(mem, disp=0x004A1E60)], dsize=4, doff=2),
            ins(14, 'jne', [op(imm, literal=0x0042C2F1)], isize=4, ioff=2),
            ins(20, 'mov', [op(mem, disp=0x34, base=5)], dsize=4, doff=3),
            ins(27, 'call', [op(imm, literal=0x0042C180)], isize=4, ioff=1),
        ]
        fields, calls = D.decoded_fields(items, capstone)
        self.assertEqual(fields, [(3, 'DIR32', 0x004A1E6C),
                                  (9, 'DIR32', 0x004A1E60),
                                  (28, 'REL32', 0x0042C180)])
        self.assertEqual(calls, [0x0042C180])

    def test_indirect_or_non_dword_call_rejects(self):
        cs = SimpleNamespace(x86=SimpleNamespace(X86_OP_IMM=1, X86_OP_MEM=2))
        for tp, size in [(2, 4), (1, 1)]:
            bad = [SimpleNamespace(mnemonic='call', address=D.BASE,
                operands=[SimpleNamespace(type=tp)], imm_size=size)]
            with self.subTest(tp=tp, size=size), self.assertRaises(ValueError):
                D.decoded_fields(bad, cs)


if __name__ == '__main__':
    unittest.main()

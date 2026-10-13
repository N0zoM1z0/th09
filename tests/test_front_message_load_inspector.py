"""Target-independent regression gates for the TH09 FrontMessage loader."""
import importlib.util
from pathlib import Path
from types import SimpleNamespace
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'front_load', ROOT / 'scripts' / 'inspect-front-message-load.py')
DIAG = importlib.util.module_from_spec(spec)
spec.loader.exec_module(DIAG)


class FrontMessageLoadTests(unittest.TestCase):
    @staticmethod
    def fields():
        rows = [dict(offset=4 * i, type='DIR32',
                     symbol='?g_FrontMessagePrimaryCache@@3PAUFrontMessageTableView@@A',
                     addend=0) for i in range(18)]
        rows[1].update(type='REL32',
                       symbol='?ReleaseForSideCount@FrontMessageRuntimeView@@QAEXH@Z')
        return rows

    def test_bind_eighteen_verified_fields_without_target_byte_fitting(self):
        blob, fields = DIAG.bind_candidate(bytes(110), self.fields())
        self.assertEqual(len(fields), 18)
        self.assertEqual(struct.unpack_from('<I', blob, 0)[0], 0x4A7D88)
        self.assertEqual(struct.unpack_from('<I', blob, 4)[0],
                         (0x417520 - (DIAG.BASE + 8)) & 0xFFFFFFFF)
        self.assertEqual(fields[1], (4, 'REL32', 0x417520))
        self.assertEqual(blob[72:], bytes(110 - 72))

    def test_unknown_symbol_kind_addend_fail_closed(self):
        for change in [dict(symbol='?Unverified@@YAXXZ'), dict(type='DIR32'),
                       dict(type='REL16'), dict(addend=1)]:
            rows = self.fields()
            rows[1].update(change)
            with self.subTest(change=change), self.assertRaises(ValueError):
                DIAG.bind_candidate(bytes(110), rows)

    def test_missing_overlap_reversed_out_of_range_fail_closed(self):
        with self.assertRaisesRegex(ValueError, '18'):
            DIAG.bind_candidate(bytes(110), self.fields()[:-1])
        for left, right in [(4, 4), (6, 8), (40, 36), (108, 12)]:
            rows = self.fields()
            rows[0]['offset'], rows[1]['offset'] = left, right
            with self.subTest(left=left, right=right), self.assertRaises(ValueError):
                DIAG.bind_candidate(bytes(110), rows)

    def test_decoder_excludes_jump_displacements_indexed_memory_and_scalar_constants(self):
        imm, memory = 1, 2
        cs = SimpleNamespace(x86=SimpleNamespace(X86_OP_IMM=imm, X86_OP_MEM=memory))
        def operand(tp, *, literal=0, base=0, index=0, disp=0):
            return SimpleNamespace(type=tp, imm=literal,
                mem=SimpleNamespace(base=base, index=index, disp=disp))
        instructions = [
            SimpleNamespace(address=DIAG.BASE, mnemonic='mov',
                operands=[operand(imm, literal=0x4A7D88)], imm_size=4,
                imm_offset=1, disp_size=0, disp_offset=0),
            SimpleNamespace(address=DIAG.BASE+5, mnemonic='call',
                operands=[operand(imm, literal=0x417520)], imm_size=4,
                imm_offset=1, disp_size=0, disp_offset=0),
            SimpleNamespace(address=DIAG.BASE+10, mnemonic='je',
                operands=[operand(imm, literal=0x418400)], imm_size=4,
                imm_offset=2, disp_size=0, disp_offset=0),
            SimpleNamespace(address=DIAG.BASE+16, mnemonic='mov',
                operands=[operand(memory, disp=0x4A7D90)], imm_size=0,
                imm_offset=0, disp_size=4, disp_offset=2),
            SimpleNamespace(address=DIAG.BASE+22, mnemonic='mov',
                operands=[operand(memory, base=1, disp=0x4A7D90)], imm_size=0,
                imm_offset=0, disp_size=4, disp_offset=2),
            SimpleNamespace(address=DIAG.BASE+28, mnemonic='push',
                operands=[operand(imm, literal=30)], imm_size=4,
                imm_offset=1, disp_size=0, disp_offset=0),
        ]
        fields, calls = DIAG.decoded_fields(instructions, cs)
        self.assertEqual(fields, [(1,'DIR32',0x4A7D88), (6,'REL32',0x417520),
                                  (18,'DIR32',0x4A7D90)])
        self.assertEqual(calls, [0x417520])

    def test_indirect_call_is_not_a_reviewed_callee(self):
        cs = SimpleNamespace(x86=SimpleNamespace(X86_OP_IMM=1, X86_OP_MEM=2))
        invalid = [SimpleNamespace(address=DIAG.BASE, mnemonic='call',
                   operands=[SimpleNamespace(type=2)], imm_size=4)]
        with self.assertRaisesRegex(ValueError, 'unreviewed call'):
            DIAG.decoded_fields(invalid, cs)


if __name__ == '__main__':
    unittest.main()

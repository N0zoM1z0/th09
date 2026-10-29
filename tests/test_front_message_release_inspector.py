"""Pure input validation for the non-crediting TH09 FrontMessage release audit."""

import importlib.util
from pathlib import Path
import struct
from types import SimpleNamespace
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'front_message_release_audit', ROOT / 'scripts' / 'inspect-front-message-release.py')
AUDIT = importlib.util.module_from_spec(spec)
spec.loader.exec_module(AUDIT)


class FrontMessageReleaseInspectorTests(unittest.TestCase):
    @staticmethod
    def fields():
        fields = [
            {'offset': n * 4, 'type': 'DIR32', 'addend': 0,
             'symbol': '?g_FrontMessagePrimaryCache@@3PAUFrontMessageTableView@@A'}
            for n in range(14)
        ]
        fields[1].update(type='REL32',
            symbol='?IsGameMode2@AsciiGameManagerView@@QAEHXZ')
        return fields

    def test_valid_complete_field_inventory_is_bound_without_target_bytes(self):
        blob, bound = AUDIT.bind_candidate(bytes(80), self.fields())
        self.assertEqual(len(bound), 14)
        self.assertEqual(struct.unpack_from('<I', blob, 0)[0], 0x004A7D88)
        self.assertEqual(struct.unpack_from('<I', blob, 4)[0],
                         (0x00415D30 - AUDIT.BASE - 8) & 0xFFFFFFFF)
        self.assertEqual(bound[1], (4, 'REL32', 0x00415D30))
        self.assertEqual(blob[56:], bytes(24))

    def test_missing_extra_unknown_and_bad_kind_are_rejected(self):
        rows = self.fields()
        for field_set in [rows[:-1], rows + rows[:1]]:
            with self.assertRaisesRegex(ValueError, '14'):
                AUDIT.bind_candidate(bytes(80), field_set)
        for change in [dict(symbol='?Unverified@@3HA'),
                       dict(type='DIR32'), dict(addend=3)]:
            candidate = self.fields()
            candidate[1].update(change)
            with self.subTest(change=change), self.assertRaises(ValueError):
                AUDIT.bind_candidate(bytes(80), candidate)

    def test_overlapping_reversed_or_outside_code_fields_are_rejected(self):
        for offsets in [(4, 4), (7, 8), (72, 8), (30, 28)]:
            candidate = self.fields()
            candidate[0]['offset'], candidate[1]['offset'] = offsets
            with self.subTest(offsets=offsets), self.assertRaises(ValueError):
                AUDIT.bind_candidate(bytes(80), candidate)

    def test_decoder_only_accepts_absolute_data_operands_and_real_calls(self):
        imm, mem = 9, 10
        capstone = SimpleNamespace(x86=SimpleNamespace(
            X86_OP_IMM=imm, X86_OP_MEM=mem))
        def operand(tp, *, value=0, address=0, base=0, index=0):
            return SimpleNamespace(type=tp, imm=value,
                mem=SimpleNamespace(disp=address, base=base, index=index))
        def instruction(addr, mnemonic, op, imm_size=0, imm_offset=0,
                        disp_size=0, disp_offset=0):
            return SimpleNamespace(address=AUDIT.BASE+addr, mnemonic=mnemonic,
                operands=[op], imm_size=imm_size, imm_offset=imm_offset,
                disp_size=disp_size, disp_offset=disp_offset)
        code = [
            instruction(0, 'mov', operand(imm, value=0x004A7D90), 4, 1),
            instruction(5, 'call', operand(imm, value=0x00415D30), 4, 1),
            instruction(10, 'je', operand(imm, value=0x00417520), 4, 2),
            instruction(16, 'mov', operand(mem, address=0x004AD018), disp_size=4, disp_offset=2),
            instruction(22, 'mov', operand(mem, address=0x004AD018, base=1), disp_size=4, disp_offset=2),
            instruction(28, 'push', operand(imm, value=10), 4, 1),
        ]
        fields, calls = AUDIT.decoded_fields(code, capstone)
        self.assertEqual(fields, [(1, 'DIR32', 0x004A7D90),
                                  (6, 'REL32', 0x00415D30),
                                  (18, 'DIR32', 0x004AD018)])
        self.assertEqual(calls, [0x00415D30])

    def test_indirect_call_is_unreviewed_not_silently_skipped(self):
        capstone = SimpleNamespace(x86=SimpleNamespace(X86_OP_IMM=1, X86_OP_MEM=2))
        ins = [SimpleNamespace(mnemonic='call',
            operands=[SimpleNamespace(type=2)], imm_size=4, address=AUDIT.BASE)]
        with self.assertRaisesRegex(ValueError, 'unreviewed'):
            AUDIT.decoded_fields(ins, capstone)


if __name__ == '__main__':
    unittest.main()

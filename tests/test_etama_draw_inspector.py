"""Pure-input guards for the complete, non-crediting Etama draw audit.

These checks deliberately work in the project's Python -S CI: Capstone and
private PE data are used only by the optional full-extent diagnostic runner.
"""

import importlib.util
from pathlib import Path
import struct
from types import SimpleNamespace
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'etama_draw_inspector', ROOT / 'scripts' / 'inspect-etama-draw.py')
DIAG = importlib.util.module_from_spec(spec)
spec.loader.exec_module(DIAG)


class EtamaDrawInspectorTests(unittest.TestCase):
    @staticmethod
    def fields():
        rows = [dict(offset=4 * index, type='DIR32',
                     symbol='?g_Supervisor@@3VSupervisor@@A', addend=0)
                for index in range(27)]
        rows[1].update(type='REL32',
                       symbol='?ConfigureGameplayViewport@Supervisor@@QAEXH@Z')
        return rows

    def test_bind_complete_field_inventory_with_independent_addresses(self):
        raw = bytes(600)
        resolved, fields = DIAG.bind_candidate(raw, self.fields())
        self.assertEqual(raw, bytes(600))
        self.assertEqual(len(resolved), 600)
        self.assertEqual(len(fields), 27)
        self.assertEqual(struct.unpack_from('<I', resolved, 0)[0], 0x4B3100)
        expected_rel = (0x401390 - (DIAG.BASE + 4 + 4)) & 0xFFFFFFFF
        self.assertEqual(struct.unpack_from('<I', resolved, 4)[0], expected_rel)
        self.assertEqual(fields[1], (4, 'REL32', 0x401390))

    def test_reject_unreviewed_symbol_kind_addend(self):
        for change in (dict(symbol='?MissingUnknownSymbol@@3HA'),
                       dict(type='DIR32'), dict(type='REL16'),
                       dict(addend=4)):
            fields = self.fields()
            fields[1].update(change)
            with self.subTest(change=change), self.assertRaises(ValueError):
                DIAG.bind_candidate(bytes(600), fields)

    def test_reject_missing_overlapping_out_of_order_and_out_of_extent_fields(self):
        with self.assertRaisesRegex(ValueError, '27'):
            DIAG.bind_candidate(bytes(600), self.fields()[:-1])
        for first, second in ((4, 4), (5, 8), (120, 116), (598, 8)):
            fields = self.fields()
            fields[0]['offset'] = first
            fields[1]['offset'] = second
            with self.subTest(first=first, second=second), self.assertRaises(ValueError):
                DIAG.bind_candidate(bytes(600), fields)

    def test_kind_is_bound_to_real_symbol_class(self):
        self.assertEqual(DIAG.expected_kind('__real@41800000'), 'DIR32')
        self.assertEqual(DIAG.expected_kind('?g_AnmManager@@3PAVAnmManager@@A'), 'DIR32')
        self.assertEqual(DIAG.expected_kind('?DrawSingleBullet@@YIHPAUBullet@@@Z'), 'REL32')
        with self.assertRaises(ValueError):
            DIAG.expected_kind('__real@unknown')

    def test_instruction_field_parser_ignores_branches_and_non_absolute_memory(self):
        immediate = 5
        memory = 6
        capstone_stub = SimpleNamespace(x86=SimpleNamespace(
            X86_OP_IMM=immediate, X86_OP_MEM=memory))
        def op(kind, **attrs):
            d = dict(type=kind, imm=0, mem=SimpleNamespace(base=0,index=0,disp=0))
            d.update(attrs)
            return SimpleNamespace(**d)
        instructions = [
            SimpleNamespace(mnemonic='mov', address=DIAG.BASE,
                            operands=[op(immediate,imm=0x4B3100)],
                            imm_offset=1, imm_size=4, disp_offset=0, disp_size=0),
            SimpleNamespace(mnemonic='call', address=DIAG.BASE+5,
                            operands=[op(immediate,imm=0x401390)],
                            imm_offset=1, imm_size=4, disp_offset=0, disp_size=0),
            SimpleNamespace(mnemonic='je', address=DIAG.BASE+10,
                            operands=[op(immediate,imm=0x413BE0)],
                            imm_offset=2, imm_size=4, disp_offset=0, disp_size=0),
            SimpleNamespace(mnemonic='mov', address=DIAG.BASE+16,
                            operands=[op(memory, mem=SimpleNamespace(base=0,index=0,disp=0x490F54))],
                            imm_offset=0, imm_size=0, disp_offset=2, disp_size=4),
            SimpleNamespace(mnemonic='mov', address=DIAG.BASE+22,
                            operands=[op(memory, mem=SimpleNamespace(base=1,index=0,disp=0x490F54))],
                            imm_offset=0, imm_size=0, disp_offset=2, disp_size=4),
        ]
        fields, calls = DIAG.decoded_fields(instructions, capstone_stub)
        self.assertEqual(fields, [(1,'DIR32',0x4B3100),
                                  (6,'REL32',0x401390),
                                  (18,'DIR32',0x490F54)])
        self.assertEqual(calls, [0x401390])


if __name__ == '__main__':
    unittest.main()

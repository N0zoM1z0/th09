"""Pure-input guards for the source-present Type14/22 full-owner diagnostic.

This suite intentionally runs under the project's python -S CI environment:
Capstone and the private original executable are needed only by the optional
actual PE/COFF comparison, never by these deterministic binder tests.
"""

import importlib.util
from pathlib import Path
import struct
from types import SimpleNamespace
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'exattack_1422_draw_inspector',
    ROOT / 'scripts' / 'inspect-exattack-type14-22-draw.py',
)
INSPECT = importlib.util.module_from_spec(spec)
spec.loader.exec_module(INSPECT)


class Type1422DrawBindingTests(unittest.TestCase):
    def rows(self):
        return [
            dict(offset=4 * i, type='DIR32',
                 symbol='?g_AnmManager@@3PAVAnmManager@@A', addend=0)
            for i in range(53)
        ]

    def test_binds_only_known_fields_without_changing_raw_input(self):
        raw = bytes(974)
        linked, fields = INSPECT.bind_candidate(raw, self.rows())
        self.assertEqual(len(fields), 53)
        self.assertEqual(len(linked), 974)
        self.assertEqual(raw, bytes(974))
        self.assertEqual(struct.unpack_from('<I', linked, 0)[0], 0x004DC550)
        self.assertEqual(struct.unpack_from('<I', linked, 4 * 52)[0], 0x004DC550)

    def test_unknown_symbol_type_and_unreviewed_addend_are_rejected(self):
        changes = (
            dict(symbol='?NotATargetOwnedSymbol@@3H'),
            dict(type='REL32'),
            dict(type='NOT_A_RELOCATION'),
            dict(addend=8),
        )
        for change in changes:
            rows = self.rows()
            rows[0].update(change)
            with self.subTest(change=change), self.assertRaises(ValueError):
                INSPECT.bind_candidate(bytes(974), rows)

    def test_wrong_field_count_overlap_and_extent_are_rejected(self):
        rows = self.rows()
        with self.assertRaisesRegex(ValueError, 'all 53'):
            INSPECT.bind_candidate(bytes(974), rows[:-1])
        for bad in (0, -1, 972):
            rows = self.rows()
            rows[1]['offset'] = bad
            with self.subTest(offset=bad), self.assertRaises(ValueError):
                INSPECT.bind_candidate(bytes(974), rows)

    def test_supervisor_field_addend_is_precise(self):
        rows = self.rows()
        rows[1].update(
            symbol='?g_Supervisor@@3VSupervisor@@A',
            addend=8,
        )
        linked, fields = INSPECT.bind_candidate(bytes(974), rows)
        self.assertEqual(fields[1][2], 0x004B3108)
        self.assertEqual(struct.unpack_from('<I', linked, 4)[0], 0x004B3108)
        for addend in (0, 4, 12):
            rows[1]['addend'] = addend
            with self.subTest(addend=addend), self.assertRaisesRegex(ValueError, 'addend'):
                INSPECT.bind_candidate(bytes(974), rows)

    def test_fictitious_supervisor_global_is_not_a_native_object_binding(self):
        rows = self.rows()
        rows[1].update(
            symbol='?g_Supervisor@@3UExAttackType1422DrawSupervisorView@@A',
            addend=8)
        with self.assertRaisesRegex(ValueError, 'unreviewed'):
            INSPECT.bind_candidate(bytes(974), rows)

    @unittest.skipUnless(importlib.util.find_spec('capstone') is not None,
                         'CI python -S intentionally has no Capstone decoder')
    def test_truncated_or_invalid_return_fails_decoding(self):
        with self.assertRaises(ValueError):
            INSPECT.decode_owner(b'\x90')
        with self.assertRaises(ValueError):
            INSPECT.decode_owner(b'\xC2\x08\x00')


class Type1422AlignedShapeTests(unittest.TestCase):
    """Target-independent 32-bit field and CFG gates for large callback probes."""

    @staticmethod
    def ins(address, code, mnemonic='mov', op_str='', imm_offset=0, imm_size=0):
        return SimpleNamespace(address=address, bytes=code, mnemonic=mnemonic,
                               op_str=op_str, imm_offset=imm_offset,
                               imm_size=imm_size)

    def stream(self):
        base = 0x1000
        return [
            self.ins(base, b'\x8b\x0d\x78\x56\x34\x12'),
            self.ins(base+6, b'\x75\x05', 'jne', '0x100d', 1, 1),
            self.ins(base+8, b'\xb8\x01\x00\x00\x00'),
            self.ins(base+13, b'\xc3', 'ret'),
        ]

    def test_masks_only_bound_fields_and_valid_internal_branch_displacements(self):
        normalized, branches = INSPECT.normalized_owner_shape(
            self.stream(), [(2, 'DIR32', 0x4A7D90)], base=0x1000)
        self.assertEqual(normalized, [
            (0, b'\x8b\x0d\0\0\0\0'),
            (6, b'\x75\0'),
            (8, b'\xb8\x01\0\0\0'),
            (13, b'\xc3'),
        ])
        self.assertEqual(branches, [(6, 13, 'jne')])
        # Scalar constant stays visible; known targets/displacements are masked.
        variant = self.stream()
        variant[0] = self.ins(0x1000, b'\x8b\x0d\x11\x22\x33\x44')
        variant[2] = self.ins(0x1008, b'\xb8\x02\x00\x00\x00')
        normalized_variant, _ = INSPECT.normalized_owner_shape(
            variant, [(2, 'DIR32', 0x4A7D90)], base=0x1000)
        changes = INSPECT.aligned_shape_changes(normalized_variant, normalized)
        self.assertEqual([r['change'] for r in changes], ['replace'])
        self.assertEqual(changes[0]['candidate'], [(8, 'b802000000')])
        self.assertEqual(changes[0]['target'], [(8, 'b801000000')])

    def test_rejects_unbound_partial_duplicate_and_unknown_fields(self):
        bad_fields = (
            [(5, 'DIR32', 0x4A7D90)],
            [(2, 'DIR32', 0x4A7D90), (2, 'DIR32', 0x4A7D90)],
            [(2, 'DIR32', 0x4A7D90), (4, 'DIR32', 0x4A7D90)],
            [(20, 'DIR32', 0x4A7D90)],
            [(2, 'REL8', 0x4A7D90)],
        )
        for fields in bad_fields:
            with self.subTest(fields=fields), self.assertRaises(ValueError):
                INSPECT.normalized_owner_shape(self.stream(), fields, base=0x1000)

    def test_rejects_noninstruction_jump_targets_and_unsupported_encoding(self):
        for dst in ('0x1009', '0x1010', 'dword ptr [eax]', 'garbled'):
            stream = self.stream()
            stream[1] = self.ins(0x1006, b'\x75\x05', 'jne', dst, 1, 1)
            with self.subTest(target=dst), self.assertRaises(ValueError):
                INSPECT.normalized_owner_shape(stream, [], base=0x1000)
        stream = self.stream()
        stream[1] = self.ins(0x1006, b'\x75\x05', 'jne', '0x100d', 5, 1)
        with self.assertRaisesRegex(ValueError, 'displacement'):
            INSPECT.normalized_owner_shape(stream, [], base=0x1000)
        stream[1] = self.ins(0x1006, b'\x75\x05', 'jne', '0x100d', 0, 1)
        with self.assertRaisesRegex(ValueError, 'displacement'):
            INSPECT.normalized_owner_shape(stream, [], base=0x1000)

    def test_inconsistent_encoded_jump_and_noncontiguous_stream_fail(self):
        altered = self.stream()
        altered[1] = self.ins(0x1006, b'\x75\x04', 'jne', '0x100d', 1, 1)
        with self.assertRaisesRegex(ValueError, 'displacement disagree'):
            INSPECT.normalized_owner_shape(altered, [], base=0x1000)
        altered = self.stream()
        altered[2] = self.ins(0x1009, b'\xb8\x01\x00\x00\x00')
        with self.assertRaisesRegex(ValueError, 'noncontiguous'):
            INSPECT.normalized_owner_shape(altered, [], base=0x1000)

    def test_duplicate_boundaries_are_rejected(self):
        stream = self.stream()
        stream[1] = self.ins(0x1000, b'\x75\x05', 'jne', '0x100d', 1, 1)
        with self.assertRaisesRegex(ValueError, 'duplicate'):
            INSPECT.normalized_owner_shape(stream, [], base=0x1000)

if __name__ == '__main__':
    unittest.main()

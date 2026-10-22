"""Pure-input guards for the source-present Type14/22 full-owner diagnostic.

This suite intentionally runs under the project's python -S CI environment:
Capstone and the private original executable are needed only by the optional
actual PE/COFF comparison, never by these deterministic binder tests.
"""

import importlib.util
from pathlib import Path
import struct
import sys
from types import SimpleNamespace
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'exattack_1422_draw_inspector',
    ROOT / 'scripts' / 'inspect-exattack-type14-22-draw.py',
)
INSPECT = importlib.util.module_from_spec(spec)
spec.loader.exec_module(INSPECT)


class Type1422ReachableGraphTests(unittest.TestCase):
    def test_only_next_unconditional_edges_normalize_and_calls_stay_ordered(self):
        calls = [('direct', 0x441890), ('indirect-slot', 0xFC)]
        fallthrough = [('fallthrough', [1], calls), ('ret', [], [])]
        jump = [('jmp', [1], calls), ('ret', [], [])]
        self.assertEqual(INSPECT.normalize_next_edges(fallthrough),
                         INSPECT.normalize_next_edges(jump))
        for changed in ([('jne', [1], calls), ('ret', [], [])],
                        [('jmp', [0], calls), ('ret', [], [])],
                        [('jmp', [1], list(reversed(calls))), ('ret', [], [])]):
            self.assertNotEqual(INSPECT.normalize_next_edges(fallthrough),
                                INSPECT.normalize_next_edges(changed))

    @staticmethod
    def alignment_stream(last=b'\x90', mnemonic='nop'):
        def ins(address, code, name, operands=()):
            return SimpleNamespace(address=address, size=len(code), bytes=code,
                                   mnemonic=name, operands=operands)
        return [ins(0x1000, b'\xeb\x08', 'jmp',
                    [SimpleNamespace(type=2, imm=0x100A)]),
                ins(0x1002, b'\x8d\xa4\x24\0\0\0\0', 'lea'),
                ins(0x1009, last, mnemonic), ins(0x100A, b'\xc3', 'ret')]

    def test_unreachable_alignment_is_counted_and_only_graph_nodes_are_removed(self):
        fake = SimpleNamespace(x86=SimpleNamespace(X86_OP_IMM=2, X86_OP_MEM=3))
        with patch.dict(sys.modules, {'capstone': fake}):
            starts, graph, count, padding, jumps = INSPECT.reachable_owner_graph(self.alignment_stream())
        self.assertEqual(starts, [0, 10])
        self.assertEqual((count, padding, jumps), (2, 8, 1))
        self.assertEqual(graph, [('next', [1], []), ('ret', [], [])])
        self.assertEqual(sum(i.size for i in self.alignment_stream()), 11)

    def test_arbitrary_dead_instructions_do_not_inherit_alignment_proof(self):
        fake = SimpleNamespace(x86=SimpleNamespace(X86_OP_IMM=2, X86_OP_MEM=3))
        with patch.dict(sys.modules, {'capstone': fake}):
            with self.assertRaisesRegex(ValueError, 'unreviewed unreachable'):
                INSPECT.reachable_owner_graph(self.alignment_stream(b'\x40', 'inc'))


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

class Type1422ComArgumentTests(unittest.TestCase):
    @staticmethod
    def events():
        def reg(name): return dict(kind='reg', reg=name, size=4)
        def imm(value): return dict(kind='imm', imm=value, size=4)
        def mem(base, disp=0):
            return dict(kind='mem', base=base, index=None, disp=disp, size=4)
        def event(name, operands, writes=()):
            return dict(mnemonic=name, operands=operands, writes=list(writes), entry=False)
        events = []
        for row in INSPECT.reference_com_calls():
            events.extend((event('mov', [reg('eax'), mem(None, 0x004B3108)], ['eax']),
                           event('mov', [reg('edx'), mem('eax')], ['edx'])))
            for arg in row['reverse_arguments']:
                operand = reg('esi') if arg == 'dynamic' else imm(arg)
                events.append(event('push', [operand], ['esp']))
            events.append(event('push', [reg('eax')], ['esp']))
            events.append(event('call', [mem('edx', row['slot'])], ['esp']))
        return events

    def test_com_dataflow_checks_the_actual_device_vtable_and_sdk_arguments(self):
        rows = INSPECT.trace_com_events(self.events())
        self.assertEqual(len(rows), 12)
        self.assertEqual(rows[0], dict(slot=0xFC, reverse_arguments=[2, 4, 0],
                                      receiver_address=0x004B3108))
        self.assertEqual(rows[7]['reverse_arguments'], [20, 'dynamic', 31, 3])
        self.assertEqual(rows[-1]['reverse_arguments'], [2, 2, 0])

    def test_com_dataflow_rejects_clobbered_receivers_and_incomplete_stack(self):
        import copy
        baseline = self.events()
        variants = []
        for index, operand, field, value in (
                (0, 1, 'disp', 0x004B310C), (1, 1, 'disp', 4),
                (6, 0, 'disp', 0xF8), (6, 0, 'index', 'ebx'),
                (2, 0, 'imm', 4), (5, 0, 'kind', 'imm')):
            events = copy.deepcopy(baseline)
            events[index]['operands'][operand][field] = value
            if index == 5: events[index]['operands'][operand]['imm'] = 0
            variants.append(events)
        events = copy.deepcopy(baseline); events.pop(2); variants.append(events)
        events = copy.deepcopy(baseline); events.insert(2, copy.deepcopy(events[2])); variants.append(events)
        for index, register in ((1, 'al'), (2, 'dl')):
            events = copy.deepcopy(baseline)
            events.insert(index, dict(mnemonic='xor', operands=[], writes=[register], entry=False))
            variants.append(events)
        events = copy.deepcopy(baseline); events[5]['entry'] = True; variants.append(events)
        events = copy.deepcopy(baseline); events[6]['operands'] = [dict(kind='reg', reg='edx', size=4)]; variants.append(events)
        for index, events in enumerate(variants):
            with self.subTest(variant=index), self.assertRaises(ValueError):
                INSPECT.trace_com_events(events)

    def test_com_argument_summary_cannot_replace_typed_evidence_with_a_boolean(self):
        import copy
        baseline = INSPECT.reference_com_calls()
        for row_index in range(12):
            for field, value in (('slot', True), ('receiver_address', 0x004B3100),
                                 ('reverse_arguments', [])):
                rows = copy.deepcopy(baseline); rows[row_index][field] = value
                with self.subTest(row=row_index, field=field), self.assertRaises(ValueError):
                    INSPECT.require_com_calls(rows)
        for rows in (None, True, [], baseline[:-1], baseline + baseline[:1],
                     list(reversed(baseline))):
            with self.subTest(rows=rows), self.assertRaises(ValueError):
                INSPECT.require_com_calls(rows)


if __name__ == '__main__':
    unittest.main()

"""Fail-closed, target-independent SaveReplay evidence checks."""
import copy
import importlib.util
from pathlib import Path
import struct
from types import SimpleNamespace
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('replay_save', ROOT/'scripts/inspect-replay-save.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


def binding_rows():
    result = []
    for off, kind, destination in audit.TARGET_FIELDS:
        choices = [(symbol, 0) for symbol, va in audit.REFERENTS.items() if va == destination]
        if not choices:
            choices = [('?g_GameManager@@3UGameManagerModeView@@A', destination-0x4A7D90)]
        symbol, addend = choices[0]
        result.append({'offset': off, 'type': kind, 'symbol': symbol, 'addend': addend})
    return result


def frontier():
    return dict(diagnostic_only=True, exactness_credit='none',
                target_bytes=1238, candidate_bytes=1238, target_sha256=audit.TARGET_SHA256,
                target_instructions=380, candidate_instructions=380,
                original_fields=53, bound_fields=53, literal_definitions=2,
                ordered_calls=23, call_operands_agree=True, call_offsets_agree=True,
                target_blocks=49, candidate_blocks=49, direct_graphs_agree=True,
                missing_target_bytes=0, extra_candidate_bytes=0,
                field_offset_deltas=[0]*53, difference_offsets=[0xB7, 0xBB, 0x3F6],
                overlap_differences=3, complete_bytes_equal=False)


class ReplaySaveTests(unittest.TestCase):
    def test_all_source_fields_are_linked_at_their_actual_positions(self):
        rows = binding_rows()
        # Shift source operands: source offsets must not be guessed from PE offsets.
        for row in rows:
            row['offset'] += 1
        linked, fields = audit.bind_candidate(bytes(audit.SIZE+1), rows)
        self.assertEqual(len(fields), 53)
        for row, (off, kind, va) in zip(rows, fields):
            self.assertEqual(off, row['offset'])
            expected = va-(audit.BASE+off+4 if kind == 'REL32' else 0)
            self.assertEqual(struct.unpack_from('<I', linked, off)[0], expected & 0xFFFFFFFF)

    def test_same_effective_address_does_not_allow_an_invented_owner(self):
        rows = binding_rows()
        for changes in ({'symbol': '?invented@@3HA'}, {'addend': 1},
                        {'type': 'DIR16'}, {'offset': -1}):
            mutated = copy.deepcopy(rows)
            mutated[0].update(changes)
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                audit.bind_candidate(bytes(audit.SIZE), mutated)
        for mutated in (rows[:-1], list(reversed(rows)), [rows[0]]+rows[:-1]):
            with self.assertRaises(ValueError):
                audit.bind_candidate(bytes(audit.SIZE), mutated)
        gm = next(i for i, row in enumerate(rows) if row['symbol'].startswith('?g_GameManager'))
        changed = copy.deepcopy(rows)
        changed[gm]['addend'] -= 4
        with self.assertRaises(ValueError):
            audit.bind_candidate(bytes(audit.SIZE), changed)

    def test_literal_names_cannot_substitute_for_real_definitions(self):
        data = bytearray(72)
        struct.pack_into('<HH', data, 0, 0x14C, 1)
        struct.pack_into('<II', data, 36, 12, 60)
        data[60:68] = b'rep tmp\0'
        data[68:72] = struct.pack('<f', 0.5)
        rows = [{'symbol': symbol, 'symbol_section': 1, 'symbol_value': value}
                for symbol, value in ((next(iter(audit.LITERALS)), 0), ('__real@3f000000', 8))]
        by_address = {audit.REFERENTS[symbol]: value for symbol, value in audit.LITERALS.items()}
        coff = SimpleNamespace(pe_bytes_at=lambda image, va, size: by_address[va])
        self.assertEqual(audit.check_literals(data, rows, b'image', coff), 2)
        for offset in (60, 67, 68, 71):
            changed = bytearray(data)
            changed[offset] ^= 1
            with self.subTest(offset=offset), self.assertRaises(ValueError):
                audit.check_literals(changed, rows, b'image', coff)
        for changed in (rows[:-1], [dict(rows[0], symbol_section=0), rows[1]],
                        [dict(rows[0], symbol_value=6), rows[1]]):
            with self.assertRaises(ValueError):
                audit.check_literals(data, changed, b'image', coff)
        by_address[audit.REFERENTS['__real@3f000000']] = b'bad!'
        with self.assertRaises(ValueError):
            audit.check_literals(data, rows, b'image', coff)

    def test_replay_only_encoder_alias_is_rejected(self):
        rows = binding_rows()
        encode = next(row for row in rows if row['offset'] == 0x3A2)
        self.assertEqual(encode['symbol'], '?Encode@Lzss@@SIPAEPAEHPAH@Z')
        audit.bind_candidate(bytes(audit.SIZE), rows)
        encode['symbol'] = '?Encode@LzssReplayView@@SIPAEPAEHPAH@Z'
        with self.assertRaisesRegex(ValueError, 'symbol/root/addend/order'):
            audit.bind_candidate(bytes(audit.SIZE), rows)

    def test_target_extent_and_hash_are_both_mandatory(self):
        for data in (b'', bytes(audit.SIZE-1), bytes(audit.SIZE), bytes(audit.SIZE+1)):
            with self.assertRaises(ValueError):
                audit.require_original(data)

    def test_same_count_new_mismatch_is_rejected(self):
        original = frontier()
        audit.guard(original)
        bad = dict(original, difference_offsets=[0xB6, 0xBB, 0x3F6])
        with self.assertRaises(ValueError):
            audit.guard(bad)
        for offsets in ([0xB7], []):
            audit.guard(dict(original, difference_offsets=offsets,
                             overlap_differences=len(offsets), complete_bytes_equal=not offsets))
        for changes in ({'complete_bytes_equal': True}, {'target_blocks': 48},
                        {'literal_definitions': 1}, {'field_offset_deltas': [0]*52+[-1]},
                        {'call_offsets_agree': False}, {'candidate_bytes': 1237},
                        {'diagnostic_only': False}, {'exactness_credit': 'exact'},
                        {'difference_offsets': [True, 0xBB, 0x3F6]},
                        {'difference_offsets': [0xB7, 0xB7, 0x3F6]}):
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                audit.guard(dict(original, **changes))


if __name__ == '__main__':
    unittest.main()

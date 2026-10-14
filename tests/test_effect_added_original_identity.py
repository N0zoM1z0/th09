"""No-target operand, hash and code-boundary guards for the Effect leaf."""

import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest

try:
    import capstone
except ImportError:  # The public CI runs without site packages.
    capstone = None

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'effect_added_original_gate', ROOT / 'scripts/inspect-effect-added.py')
inspect = importlib.util.module_from_spec(spec)
spec.loader.exec_module(inspect)


class EffectOriginalGuard(unittest.TestCase):
    @staticmethod
    def synthetic_original():
        target = bytearray(inspect.SIZE)
        for offset, kind, destination in inspect.TARGET_FIELDS:
            encoded = (destination - (inspect.BASE + offset + 4 if kind == 'REL32' else 0)) & 0xffffffff
            struct.pack_into('<I', target, offset, encoded)
        return bytes(target)

    @staticmethod
    def reviewed_source_rows():
        rows = []
        for offset, kind, symbol in (
                (6, 'DIR32', '?g_AnmManager@@3PAUEffectAnmManagerView@@A'),
                (15, 'REL32', '?GetAnm@EffectAnmManagerView@@QAEPAXH@Z'),
                (27, 'DIR32', '??_C@_09NMANGJFI@etama?4anm?$AA@'),
                (36, 'REL32', '?PreloadAnm@EffectAnmManagerView@@QAEPAXHPBD@Z')):
            rows.append({'offset': offset, 'type': kind,
                         'symbol': symbol, 'addend': 0})
        return rows

    def test_independently_identified_original_four_operands(self):
        source = self.synthetic_original()
        inspect.require_original(source, expected_sha256=hashlib.sha256(source).hexdigest())
        self.assertEqual(len(inspect.TARGET_FIELDS), 4)
        self.assertEqual(inspect.TARGET_BLOCKS, 3)

    def test_corrupted_original_sha_and_field_encoding_are_both_rejected(self):
        source = self.synthetic_original()
        with self.assertRaises(ValueError):
            inspect.require_original(source, expected_sha256='0'*64)
        with self.assertRaises(ValueError):
            inspect.require_original(source[:-1], expected_sha256=hashlib.sha256(source[:-1]).hexdigest())
        for offset, *_ in inspect.TARGET_FIELDS:
            changed = bytearray(source)
            changed[offset] ^= 1
            with self.subTest(offset=offset), self.assertRaisesRegex(ValueError, 'operand identity'):
                inspect.require_original(bytes(changed),
                    expected_sha256=hashlib.sha256(changed).hexdigest())

    def test_all_candidate_relocations_link_to_actual_addresses(self):
        rows = self.reviewed_source_rows()
        source = bytes(inspect.SIZE)
        bound, fields = inspect.bind_candidate(source, rows)
        self.assertEqual(len(bound), inspect.SIZE)
        self.assertEqual(len(fields), 4)
        for row, (_, kind, destination) in zip(rows, inspect.TARGET_FIELDS):
            expected = destination - (inspect.BASE + row['offset'] + 4 if kind == 'REL32' else 0)
            self.assertEqual(struct.unpack_from('<I', bound, row['offset'])[0], expected & 0xffffffff)

    def test_missing_wrong_order_addend_or_overlap_is_rejected(self):
        source = bytes(inspect.SIZE)
        valid = self.reviewed_source_rows()
        cases = [valid[:-1], list(reversed(valid)),
                 [dict(valid[0], addend=1)] + valid[1:],
                 [dict(valid[0], type='REL32')] + valid[1:],
                 [dict(valid[0], symbol='?invented@@3HA')] + valid[1:],
                 valid[:1] + [dict(valid[1], offset=valid[0]['offset'] + 1)] + valid[2:]]
        for case in cases:
            with self.subTest(case=case[:2]), self.assertRaises(ValueError):
                inspect.bind_candidate(source, case)

    @unittest.skipIf(capstone is None, 'Capstone unavailable in python -S')
    def test_return_abi_and_indirect_calls_are_checked(self):
        for source in (bytes.fromhex('ffd0c3'), bytes.fromhex('c20400')):
            with self.subTest(source=source.hex()), self.assertRaises(ValueError):
                inspect.decode_owner(source)


if __name__ == '__main__':
    unittest.main()

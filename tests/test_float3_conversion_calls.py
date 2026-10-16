"""Pure byte-operand regression tests; original PE access is a separate audit."""
import importlib.util
from pathlib import Path
import struct
import unittest

SCRIPT = Path(__file__).resolve().parents[1] / 'scripts/audit-float3-conversion-calls.py'
spec = importlib.util.spec_from_file_location('th09_float3_call_audit', SCRIPT)
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


class FoldedConversionCallCensus(unittest.TestCase):
    def test_relocate_both_positive_and_negative_displacements(self):
        base = 0x400000
        for callee in (base + 0x100, base - 0x100):
            stream = bytearray(20)
            stream[4] = 0xE8
            struct.pack_into('<i', stream, 5, callee - (base + 9))
            self.assertEqual(audit.rel32_call_candidates(stream, base, callee),
                             [base + 4])
            self.assertEqual(audit.rel32_call_candidates(stream, base, callee+1), [])

    def test_reject_truncated_and_wrong_opcode(self):
        self.assertEqual(audit.rel32_call_candidates(b'\xe8\x01\x02\x03', 0, 9), [])
        self.assertEqual(audit.rel32_call_candidates(b'\xe9\x00\x00\x00\x00', 0, 5), [])
        self.assertEqual(audit.rel32_call_candidates(b'', 0, 5), [])

    def test_read_only_owner_range_at_reviewed_boundaries(self):
        sample = [
            {'begin': 0x100, 'end': 0x102, 'name': 'owner-a'},
            {'begin': 0x110, 'end': 0x119, 'name': 'owner-b'},
        ]
        self.assertEqual(audit.owner_at(0x100, sample)['name'], 'owner-a')
        self.assertEqual(audit.owner_at(0x102, sample)['name'], 'owner-a')
        self.assertEqual(audit.owner_at(0x119, sample)['name'], 'owner-b')
        for address in (0xFF, 0x103, 0x10F, 0x11A):
            with self.subTest(address=address):
                self.assertIsNone(audit.owner_at(address, sample))

    def test_exact_code_xrefs_guard_byte_search_against_false_positives(self):
        expected = list(audit.REVIEWED_CODE_CALLS)
        self.assertEqual(len(expected), 27)
        audit.require_reviewed_code_calls(expected)
        for damaged in (expected[:-1], expected[1:], expected + [0x4343D0],
                        expected[:-1] + [0x407F54], list(reversed(expected))):
            with self.subTest(damaged=damaged[:2]), self.assertRaisesRegex(
                    ValueError, 'differ from IDA code xrefs'):
                audit.require_reviewed_code_calls(damaged)

    def test_known_original_xrefs_are_not_interpreted_as_authorship(self):
        self.assertEqual(audit.FOLDED_VA, 0x004343D0)
        self.assertIn(0x00407F53, audit.KNOWN_SITES)
        self.assertEqual(len(set(audit.KNOWN_SITES)), len(audit.KNOWN_SITES))
        self.assertEqual(len(audit.FOLDED_BODY_SHA), 64)
        self.assertEqual(len(audit.BIASED_Y_LEA_SHA), 64)


if __name__ == '__main__':
    unittest.main()

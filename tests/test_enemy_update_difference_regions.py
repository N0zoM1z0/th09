"""Fail-closed, target-independent tests of Enemy big-owner delta explanation."""

import importlib.util
from pathlib import Path
from types import SimpleNamespace
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'enemy_difference_spans', ROOT / 'scripts/diagnostic_byte_spans.py')
INSPECT = importlib.util.module_from_spec(spec)
spec.loader.exec_module(INSPECT)


class DifferenceRegionsTests(unittest.TestCase):
    def test_four_target_locality_groups_without_partial_credit(self):
        groups = INSPECT.nearby_difference_regions([
            0x19C, 0x1A3,
            0x252, 0x263, 0x26D, 0x272, 0x294, 0x29A, 0x29C,
            0x2A5, 0x2A9, 0x2B7, 0x2BD,
            0x845, 0x846, 0x847, 0x849, 0x84A, 0x84C, 0x84D,
            0x84E, 0x84F, 0x850,
            0x9C0, 0x9C6, 0x9DF, 0x9E2, 0x9EC, 0x9F3,
            0xA1F, 0xA23, 0xA29, 0xA2E, 0xA34, 0xA3A,
            0xA42, 0xA45, 0xA61, 0xA7E])
        self.assertEqual([(x['start'], x['end_exclusive'], len(x['offsets']))
                          for x in groups], [
                              (0x19C, 0x1A4, 2),
                              (0x252, 0x2BE, 11),
                              (0x845, 0x851, 10),
                              (0x9C0, 0xA7F, 16)])
        self.assertEqual(sum(len(x['offsets']) for x in groups), 39)

    def test_empty_sorted_and_invalid_offsets(self):
        self.assertEqual(INSPECT.nearby_difference_regions([]), [])
        for offsets in ([2, 2], [3, 1], [-1], ['3']):
            with self.subTest(offsets=offsets), self.assertRaises(ValueError):
                INSPECT.nearby_difference_regions(offsets)
        with self.assertRaises(ValueError):
            INSPECT.nearby_difference_regions([1, 2], gap=-1)

    def test_include_full_instructions_not_partial_operands(self):
        base = 0x410730
        # Already-decoded MOV EAX,0x12345678 (five bytes), PUSH EAX and RET.
        # Synthetic metadata tests full-operand selection without Capstone.
        def inst(offset, bytes_, name, operands):
            return SimpleNamespace(address=base + offset, size=len(bytes_),
                                   bytes=bytes_, mnemonic=name, op_str=operands)
        a = [inst(0, b'\xb8\x78\x56\x34\x12', 'mov', 'eax, 0x12345678'),
             inst(5, b'\x50', 'push', 'eax'), inst(6, b'\xc3', 'ret', '')]
        b = [inst(0, b'\xb8\x78\x56\x35\x12', 'mov', 'eax, 0x12355678'),
             inst(5, b'\x50', 'push', 'eax'), inst(6, b'\xc3', 'ret', '')]
        rows = INSPECT.describe_difference_regions(a, b, [3],
                                                  base=base, context=0)
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]['different_bytes'], 1)
        self.assertEqual(rows[0]['target_instructions'][0]['bytes'],
                         'b878563412')
        self.assertEqual(rows[0]['candidate_instructions'][0]['bytes'],
                         'b878563512')
        self.assertEqual(len(rows[0]['target_instructions']), 1)
        with self.assertRaises(ValueError):
            INSPECT.describe_difference_regions(a, b, [3], base=base, context=-1)


if __name__ == '__main__':
    unittest.main()

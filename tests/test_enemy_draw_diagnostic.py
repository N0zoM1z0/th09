"""Guards for complete, non-crediting Enemy draw diagnostics."""
import importlib.util
from pathlib import Path
import struct
import unittest

try:
    import capstone
except ImportError:
    capstone = None

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('enemy_draw_diagnostic', ROOT/'scripts/inspect-enemy-draw.py')
INSPECT = importlib.util.module_from_spec(spec)
spec.loader.exec_module(INSPECT)


class CompleteComparisonTests(unittest.TestCase):
    def test_equal_prefix_does_not_hide_missing_or_excess_bytes(self):
        short = INSPECT.compare_extents(b'\xc3\x90', b'\xc3')
        long = INSPECT.compare_extents(b'\xc3', b'\xc3\x90')
        self.assertFalse(short['complete_bytes_equal'])
        self.assertFalse(long['complete_bytes_equal'])
        self.assertEqual(short['missing_target_bytes'], 1)
        self.assertEqual(long['extra_candidate_bytes'], 1)

    def test_relocated_operand_difference_is_counted(self):
        result = INSPECT.compare_extents(b'\xe8\x01\0\0\0', b'\xe8\x02\0\0\0')
        self.assertEqual(result['overlapping_differences'], [dict(offset=1, target=1, candidate=2)])

    def test_missing_duplicate_and_misbound_fields_fail_closed(self):
        field = dict(offset=1, type='REL32', effective_destination=0x401000)
        for bound in ([], [field, field], [dict(field, effective_destination=0x402000)]):
            with self.subTest(bound=bound), self.assertRaisesRegex(ValueError, 'actual COFF fields'):
                INSPECT.require_field_coverage([field], bound)


@unittest.skipIf(capstone is None, 'optional Capstone diagnostic dependency')
class DecodedFieldsTests(unittest.TestCase):
    def test_object_immediate_and_data_operands_keep_scalar_mask_distinct(self):
        # MOV ECX, GameManager; TEST EAX, flag; FLD [literal]; CALL next.
        code = b'\xb9'+struct.pack('<I', 0x4A7D90)+b'\xa9'+struct.pack('<I', 0x400000)
        code += b'\xd9\x05'+struct.pack('<I', 0x48E310)+b'\xe8\0\0\0\0'
        rows, count = INSPECT.decode_fields(code)
        self.assertEqual(count, 4)
        self.assertEqual(rows, [
            dict(offset=1, type='DIR32', effective_destination=0x4A7D90),
            dict(offset=12, type='DIR32', effective_destination=0x48E310),
            dict(offset=17, type='REL32', effective_destination=INSPECT.BASE+21),
        ])

    def test_truncated_instruction_fails_closed(self):
        with self.assertRaisesRegex(ValueError, 'incomplete instruction'):
            INSPECT.decode_fields(b'\xc3\xe8\0')


if __name__ == '__main__':
    unittest.main()

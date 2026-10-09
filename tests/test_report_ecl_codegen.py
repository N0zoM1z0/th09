"""Target-independent checks for RunEcl code extent and evidence scope."""

import importlib.util
from pathlib import Path
import unittest
from unittest.mock import patch
from types import SimpleNamespace


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "report_ecl_codegen", ROOT / "scripts/report-ecl-codegen.py"
)
REPORT = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(REPORT)


def candidate(displacement: int, copy: bytes):
    code = bytearray(32)
    code[8:10] = b"\x8d\x85"
    code[10:14] = displacement.to_bytes(4, "little", signed=True)
    code[14:20] = b"\x50\xe8\x00\x00\x00\x00"
    code[20 : 20 + len(copy)] = copy
    relocations = [
        {"offset": 16, "type": "REL32", "symbol": REPORT.FLOAT3_ADD_SYMBOL}
    ]
    return code, relocations


class FirstWorldResultTests(unittest.TestCase):
    def test_returned_eax_copy_and_target_home(self):
        code, relocations = candidate(-0x168, b"\x8b\x10")
        result = REPORT.first_world_result_shape(code, relocations)
        self.assertEqual(result["home"], "-0x168")
        self.assertEqual(result["copy_source"], "returned_eax")

    def test_named_local_copy_is_distinguished(self):
        code, relocations = candidate(-0x10C, b"\x8b\x95")
        result = REPORT.first_world_result_shape(code, relocations)
        self.assertEqual(result["home"], "-0x10C")
        self.assertEqual(result["copy_source"], "stack_local")

    def test_unreviewed_setup_fails_closed(self):
        code, relocations = candidate(-0x168, b"\x8b\x10")
        code[8] = 0x90
        result = REPORT.first_world_result_shape(code, relocations)
        self.assertEqual(result["status"], "unrecognized")

    def test_unreviewed_copy_fails_closed(self):
        code, relocations = candidate(-0x168, b"\x90\x90")
        result = REPORT.first_world_result_shape(code, relocations)
        self.assertEqual(result["status"], "unrecognized")


class EvidenceScopeTests(unittest.TestCase):
    def test_aggregate_counts_do_not_claim_unknown_callee_identity(self):
        # Deliberately fill the non-resolver remainder with an unbound name.
        # Aggregate counts alone must never turn that into identity evidence.
        names = [name for name, count in REPORT.EXPECTED_RESOLVER_CALLS.items()
                 for _ in range(count)]
        names += ['?UnreviewedCallee@@YAXXZ'] * (REPORT.CANDIDATE_DIRECT_CALL_COUNT - len(names))
        relocations = [dict(offset=16 + i * 5, type='REL32', symbol=name)
                       for i, name in enumerate(names)]
        table_count = REPORT.EASING_TABLE_COUNT + REPORT.OPCODE_TABLE_COUNT
        relocations += [dict(offset=2000 + i * 4, type='DIR32', symbol=f'$table{i}')
                        for i in range(table_count)]
        code = bytearray(2000 + table_count * 4)
        code[:9] = b'\x55\x8b\xec\x81\xec\x68\x01\x00\x00'
        reader = SimpleNamespace(verified_target=lambda: None,
                                 object_function=lambda *_: (code, relocations),
                                 pe_bytes_at=lambda *_: bytes(REPORT.TARGET_LOGICAL_SIZE))
        with patch.object(REPORT, 'load_compare_module', return_value=reader), \
                patch.object(REPORT, 'code_and_alignment_sizes',
                             side_effect=[(1999, 1), (REPORT.TARGET_LOGICAL_SIZE, 0)]):
            result = REPORT.report(Path('unreviewed-fixture.obj'))
        self.assertEqual(result['candidate']['logical_code_size'], 1999)
        self.assertEqual(result['candidate']['post_return_alignment_size'], 1)
        self.assertEqual(result['candidate']['compiler_tables']['offset'], 2000)
        self.assertEqual(result['call_identity_validation'], 'not_performed')
        self.assertIsNone(result['known_callsite_mismatches'])
        self.assertIn('not validated', result['claim'])
        self.assertEqual(result['status'], 'NON-EXACT')


@unittest.skipUnless(importlib.util.find_spec('capstone'), 'optional Capstone not installed')
class CodeExtentTests(unittest.TestCase):
    def extent(self, encoded):
        return REPORT.code_and_alignment_sizes(bytes.fromhex(encoded))

    def test_return_with_and_without_alignment(self):
        self.assertEqual(self.extent('c2 04 00'), (3, 0))
        self.assertEqual(self.extent('c2 04 00 90'), (3, 1))

    def test_return_bytes_inside_immediate_are_not_a_boundary(self):
        self.assertEqual(self.extent('b8 c2 04 00 90 c2 04 00 90'), (8, 1))

    def test_self_lea_alignment_preserves_the_register(self):
        self.assertEqual(self.extent('c2 04 00 8d 49 00'), (3, 3))
        with self.assertRaisesRegex(ValueError, 'unreviewed instruction'):
            self.extent('c2 04 00 8d 51 00')

    def test_real_tail_instruction_is_rejected(self):
        with self.assertRaisesRegex(ValueError, 'unreviewed instruction'):
            self.extent('c2 04 00 b8 01 00 00 00')

    def test_truncated_tail_is_rejected(self):
        with self.assertRaisesRegex(ValueError, 'incomplete'):
            self.extent('c2 04 00 8d 49')

    def test_branch_into_alignment_is_rejected(self):
        with self.assertRaisesRegex(ValueError, 'direct branch'):
            self.extent('eb 03 c2 04 00 90')

    def test_missing_return_and_excessive_alignment_are_rejected(self):
        with self.assertRaisesRegex(ValueError, 'no terminal return'):
            self.extent('90 90')
        with self.assertRaisesRegex(ValueError, 'exceeds'):
            self.extent('c2 04 00 ' + '90 ' * 16)


if __name__ == "__main__":
    unittest.main()

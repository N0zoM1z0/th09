"""Target-independent tests for RunEcl's first vector-result diagnostic."""

import importlib.util
from pathlib import Path
import unittest


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


if __name__ == "__main__":
    unittest.main()

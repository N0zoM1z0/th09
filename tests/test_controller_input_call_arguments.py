"""Byte-level contract tests, independent of the private game input."""
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location("input_calls", ROOT / "scripts/audit-controller-input-calls.py")
AUDIT = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(AUDIT)


def synthetic_owner():
    # Only the reviewed instructions are synthesized. This is not a copy of
    # the original function, nor evidence that a game function is exact.
    result = bytearray(AUDIT.SIZE)
    def put(address, value):
        offset = address - AUDIT.BASE
        result[offset:offset + len(value)] = value
    put(0x430E98, b"\x8b\x3d" + struct.pack("<I", AUDIT.STATE_ROOT))
    put(0x430E9E, bytes.fromhex("8b87b000000085c0"))
    put(0x430EA6, bytes.fromhex("751a"))
    put(0x430EC2, bytes.fromhex("8b8fac000000"))
    put(0x431041, b"\xb9" + struct.pack("<I", 2))
    for address in AUDIT.CALLS:
        put(address, b"\xe8" + struct.pack("<i", AUDIT.INPUT_BASE - address - 5))
    return result


class InputCallArguments(unittest.TestCase):
    def test_decodes_both_ecx_sources(self):
        result = AUDIT.audit_call_inputs(synthetic_owner())
        self.assertEqual(result["calls"][0]["source"], "current network state + 0xAC")
        self.assertEqual(result["calls"][1]["value"], 2)
        self.assertTrue(all(x["ecx_width"] == 32 for x in result["calls"]))

    def assert_rejected_change(self, address, value):
        owner = synthetic_owner()
        owner[address - AUDIT.BASE] = value
        with self.assertRaises(ValueError):
            AUDIT.audit_call_inputs(owner)

    def test_rejects_absent_side_preparation(self):
        self.assert_rejected_change(0x430EC2, 0x90)

    def test_rejects_different_side_field(self):
        self.assert_rejected_change(0x430EC4, 0xA8)

    def test_rejects_different_network_root(self):
        self.assert_rejected_change(0x430E9A, 0xD4)

    def test_rejects_different_ready_edge(self):
        self.assert_rejected_change(0x430EA7, 0x19)

    def test_rejects_different_selector(self):
        self.assert_rejected_change(0x431042, 3)

    def test_rejects_redirected_callee(self):
        self.assert_rejected_change(AUDIT.CALLS[0] + 1, 0)

    def test_rejects_absent_call(self):
        self.assert_rejected_change(AUDIT.CALLS[1], 0x90)

    def test_rejects_incomplete_owner(self):
        with self.assertRaises(ValueError):
            AUDIT.audit_call_inputs(synthetic_owner()[:-1])


if __name__ == "__main__":
    unittest.main()

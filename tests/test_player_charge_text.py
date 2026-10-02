import importlib.util
from pathlib import Path
import struct
import unittest

spec = importlib.util.spec_from_file_location(
    "charge_text", Path(__file__).resolve().parents[1] / "scripts/inspect-player-charge-text.py")
charge = importlib.util.module_from_spec(spec)
spec.loader.exec_module(charge)


class ChargeTextRegionTests(unittest.TestCase):
    def test_replays_only_the_independently_bound_call(self):
        row = {"offset": 1, "type": "REL32", "addend": 0}
        offset, region = charge.replay_region(b"\xe8\0\0\0\0", [row], row, 0x410000, 0x410000)
        self.assertEqual(offset, 0)
        self.assertEqual(struct.unpack_from("<i", region, 1)[0], 0x403E50 - 0x410005)

    def test_rejects_extra_field_and_nonzero_addend(self):
        row = {"offset": 1, "type": "REL32", "addend": 0}
        extra = {"offset": 2, "type": "DIR32", "addend": 0}
        with self.assertRaises(ValueError):
            charge.replay_region(b"\xe8\0\0\0\0", [row, extra], row, 0x410000, 0x410000)
        row["addend"] = 1
        with self.assertRaises(ValueError):
            charge.replay_region(b"\xe8\0\0\0\0", [row], row, 0x410000, 0x410000)

    def test_rejects_missing_region_and_wrong_opcode(self):
        row = {"offset": 1, "type": "REL32", "addend": 0}
        with self.assertRaises(ValueError):
            charge.replay_region(b"\xe8\0\0\0\0", [row], row, 0x40FFFF, 0x410000)
        with self.assertRaises(ValueError):
            charge.replay_region(b"\x90\0\0\0\0", [row], row, 0x410000, 0x410000)

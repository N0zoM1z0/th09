import importlib.util
from pathlib import Path
import struct
import unittest

spec = importlib.util.spec_from_file_location(
    "charge_text", Path(__file__).resolve().parents[1] / "scripts/inspect-player-charge-text.py")
charge = importlib.util.module_from_spec(spec)
spec.loader.exec_module(charge)


class ChargeTextRegionTests(unittest.TestCase):
    def test_native_global_fields_keep_actual_roots_and_addends(self):
        fields = [dict(offset=o, symbol=charge.NATIVE_GAME_MANAGER, type='DIR32',
                       target=0x4A7D90, addend=0xE8, effective_target=0x4A7E78)
                  for o in (38, 68, 259)]
        fields.append(dict(offset=616, symbol=charge.NATIVE_SUPERVISOR,
            type='DIR32', target=0x4B3100, addend=0x5B8, effective_target=0x4B36B8))
        charge.require_native_globals(fields)
        for index, key, value in ((0,'symbol','?g_GameManager@@3UGameManagerReplayView@@A'),
                (3,'symbol','?g_PlayerSupervisorRuntime@@3UPlayerSupervisorRuntimeView@@A'),
                (0,'target',0x4A7E78),(0,'addend',0),(3,'effective_target',0x4B3100),
                (0,'type','REL32'),(0,'offset',68)):
            bad=[dict(r) for r in fields];bad[index][key]=value
            with self.subTest(index=index,key=key),self.assertRaises(ValueError):
                charge.require_native_globals(bad)
        for bad in (fields[:-1], fields+fields[:1]):
            with self.assertRaises(ValueError):charge.require_native_globals(bad)

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

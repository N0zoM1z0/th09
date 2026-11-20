import importlib.util
from pathlib import Path
import struct
import unittest


spec = importlib.util.spec_from_file_location(
    "coff_eh", Path(__file__).resolve().parents[1] / "scripts/coff_exception_handlers.py")
eh = importlib.util.module_from_spec(spec)
spec.loader.exec_module(eh)


class ExceptionHandlerTests(unittest.TestCase):
    def setUp(self):
        self.code = bytearray.fromhex("8b45f050e80000000059c3b800000000e900000000")
        self.metadata = bytearray(struct.pack("<i8I", -1, 0, 0x19930520, 1, 0, 0, 0, 0, 0))
        self.code_fields = [(5, 7, 20), (12, 4, 6), (17, 6, 20)]
        self.data_fields = [(4, 3, 6), (16, 5, 6)]
        self.destinations = {"??3@YAXPAX@Z": 0x405000, "___CxxFrameHandler": 0x406000}
        self.handler = dict(type_id=6, addend=0, symbol_section=1, symbol_value=11, owner_section=3)
        self.code_address = 0x401000
        self.data_address = 0x402000
        self.handler_address = self.code_address + 11
        code = self.code[:]
        struct.pack_into("<I", code, 5, 0x405000 - (self.code_address + 9))
        struct.pack_into("<I", code, 12, self.data_address + 8)
        struct.pack_into("<I", code, 17, 0x406000 - (self.code_address + 21))
        metadata = self.metadata[:]
        struct.pack_into("<I", metadata, 4, self.code_address)
        struct.pack_into("<I", metadata, 16, self.data_address)
        self.target = code + bytes(self.data_address - self.code_address - len(code)) + metadata
        aux = bytearray(18)
        struct.pack_into("<HB", aux, 12, 3, 5)
        self.symbols = {
            1: dict(name=".text$x", section=1, storage=3, aux=bytes(aux)),
            2: dict(name=".xdata$x", section=2, storage=3, aux=bytes(aux)),
            3: dict(name="$L1", section=1, value=0),
            4: dict(name="$T2", section=2, value=8),
            5: dict(name="$T3", section=2, value=0),
            6: dict(name="___CxxFrameHandler", section=0),
            7: dict(name="??3@YAXPAX@Z", section=0),
        }

    def verify(self):
        raw = bytearray(b"\0")
        sections = []
        for name, code, fields, flag in [
            (b".text$x", self.code, self.code_fields, 0x20),
            (b".xdata$x", self.metadata, self.data_fields, 0x40),
        ]:
            start = len(raw)
            raw.extend(code)
            relocations = len(raw)
            for row in fields:
                raw.extend(struct.pack("<IIH", *row))
            sections.append(dict(name_raw=name, size=len(code), flags=flag | 0x1000,
                                 raw_offset=start, reloc_offset=relocations, reloc_count=len(fields)))

        def read_target(address, size):
            offset = address - self.code_address
            return self.target[offset:offset + size]

        return eh.verify((raw, sections, self.symbols), self.handler, self.handler_address,
                         read_target, self.destinations)

    def test_full_graph_matches(self):
        proof = self.verify()
        self.assertEqual((proof["code_bytes"], proof["metadata_bytes"], proof["relocations"]),
                         (21, 36, 5))

    def test_wrong_cleanup_stack_slot_fails(self):
        self.code[2] = 0xEC
        with self.assertRaisesRegex(ValueError, "cleanup/handler code differs.*0x00401002"):
            self.verify()

    def test_wrong_unwind_action_target_fails(self):
        self.symbols[3]["value"] = 1
        with self.assertRaisesRegex(ValueError, "unwind action differs at slot 0"):
            self.verify()

    def test_changed_state_fails(self):
        self.metadata[0] = 0xFE
        with self.assertRaisesRegex(ValueError, "unwind metadata differs"):
            self.verify()

    def test_handler_must_use_the_pinned_crt_entry(self):
        self.code_fields[-1] = (17, 7, 20)
        with self.assertRaisesRegex(ValueError, "pinned CRT frame handler"):
            self.verify()

    def test_different_associative_owner_fails(self):
        aux = bytearray(self.symbols[2]["aux"])
        struct.pack_into("<H", aux, 12, 4)
        self.symbols[2]["aux"] = bytes(aux)
        with self.assertRaisesRegex(ValueError, "not associated with the caller"):
            self.verify()

    def test_missing_cleanup_relocation_fails(self):
        self.code_fields.pop(0)
        with self.assertRaisesRegex(ValueError, "cleanup relocation count differs"):
            self.verify()

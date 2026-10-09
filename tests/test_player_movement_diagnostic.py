"""Target-independent COFF symbol tests for the non-crediting diagnostic."""
import importlib.util
from pathlib import Path
import struct
import tempfile
import unittest

try:
    import capstone
except ImportError:
    capstone = None


ROOT = Path(__file__).resolve().parents[1]


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / "scripts" / filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


INSPECT = module("movement_diagnostic", "inspect-player-movement.py")
COFF = module("movement_coff", "compare-coff-function.py")


def fixture(rows):
    records = b"".join(
        struct.pack("<8sIhHBB", name.encode().ljust(8, b"\0"), value, section, 0, 3, 0)
        for name, section, value in rows
    )
    return struct.pack("<HHIIIHH", 0x14C, 0, 0, 20, len(rows), 0, 0) + records + struct.pack("<I", 4)


class CoffSymbolsTests(unittest.TestCase):
    def inspect_symbols(self, rows, names):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "fixture.obj"
            path.write_bytes(fixture(rows))
            return INSPECT.coff_symbols(path, COFF, names)

    def test_duplicate_debug_sections_do_not_obscure_owner(self):
        result = self.inspect_symbols(
            [(".debug$S", 1, 0), (".debug$S", 2, 0), ("owner", 3, 4)], {"owner"}
        )
        self.assertEqual(result, {"owner": (3, 4)})

    def test_relevant_ambiguous_symbol_fails_closed(self):
        with self.assertRaisesRegex(ValueError, "ambiguous COFF symbol"):
            self.inspect_symbols([("owner", 1, 0), ("owner", 2, 0)], {"owner"})

    def test_identical_relevant_alias_is_unambiguous(self):
        self.assertEqual(
            self.inspect_symbols([("owner", 1, 4), ("owner", 1, 4)], {"owner"}),
            {"owner": (1, 4)},
        )


class CompleteExtentTests(unittest.TestCase):
    def test_alignment_and_table_differences_are_counted(self):
        result = INSPECT.compare_extents(b"\xc3\x90\x00\x00", b"\xc3\xcc\x00\x01")
        self.assertFalse(result["full_byte_agreement"])
        self.assertEqual(result["difference_offsets"], [1, 3])
        self.assertEqual(result["full_difference_count"], 2)

    def test_equal_prefix_does_not_hide_missing_or_excess_bytes(self):
        short = INSPECT.compare_extents(b"\xc3\x90", b"\xc3")
        long = INSPECT.compare_extents(b"\xc3", b"\xc3\x90")
        self.assertFalse(short["full_byte_agreement"])
        self.assertFalse(long["full_byte_agreement"])
        self.assertEqual(short["missing_target_bytes"], 1)
        self.assertEqual(long["extra_candidate_bytes"], 1)

    def test_relocation_operand_disagreement_is_not_masked(self):
        target = b"\xe8" + struct.pack("<I", 1)
        candidate = b"\xe8" + struct.pack("<I", 2)
        self.assertEqual(INSPECT.compare_extents(target, candidate)["difference_offsets"], [1])


@unittest.skipIf(capstone is None, "optional Capstone diagnostic dependency")
class DecodedFieldsTests(unittest.TestCase):
    def test_call_absolute_operand_and_every_table_entry_are_decoded(self):
        decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        decoder.detail = True
        code = b"\xe8\x00\x00\x00\x00\xa1" + struct.pack("<I", 0x004A80F0)
        table = struct.pack("<16I", *range(INSPECT.BASE, INSPECT.BASE + 16))
        rows = INSPECT.decoded_fields(list(decoder.disasm(code, INSPECT.BASE)), table, 10)
        self.assertEqual(rows[:2], [
            {"offset": 1, "type": "REL32", "effective_destination": INSPECT.BASE + 5},
            {"offset": 6, "type": "DIR32", "effective_destination": 0x004A80F0},
        ])
        self.assertEqual(len(rows), 18)
        self.assertEqual([row["offset"] for row in rows[2:]], list(range(10, 74, 4)))
        self.assertEqual([row["effective_destination"] for row in rows[2:]],
                         list(range(INSPECT.BASE, INSPECT.BASE + 16)))

    def test_truncated_table_fails_closed(self):
        with self.assertRaisesRegex(ValueError, "complete eight-entry tables"):
            INSPECT.decoded_fields([], bytes(60), 0)


if __name__ == "__main__":
    unittest.main()

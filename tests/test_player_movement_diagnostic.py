"""Target-independent COFF symbol tests for the non-crediting diagnostic."""
import importlib.util
from pathlib import Path
import struct
import tempfile
import unittest


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


if __name__ == "__main__":
    unittest.main()

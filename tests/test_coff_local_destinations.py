"""Reject manifest-fitted local labels without needing the private target."""
import importlib.util
from pathlib import Path
import struct
import tempfile
import unittest
from unittest import mock

spec = importlib.util.spec_from_file_location(
    "coff_compare", Path(__file__).resolve().parents[1] / "scripts/compare-coff-function.py")
coff = importlib.util.module_from_spec(spec)
spec.loader.exec_module(coff)


class OwnerLocalDestinationTests(unittest.TestCase):
    def row(self, **changes):
        row = dict(offset=244, symbol="$Lcase", symbol_section=2,
                   owner_section=2, symbol_value=0x151, owner_value=0x100, addend=0)
        row.update(changes)
        return row

    def check(self, row, target):
        coff.validate_owner_local_destinations(
            [row], [dict(offset=244, target=target)], 0x436E20, 256)

    def test_actual_owner_relative_label_passes(self):
        self.check(self.row(), 0x436E71)

    def test_retargeted_switch_case_fails(self):
        with self.assertRaisesRegex(ValueError, "owner-local COFF destination"):
            self.check(self.row(), 0x436E80)

    def test_addend_does_not_change_symbol_base(self):
        self.check(self.row(addend=4), 0x436E71)
        with self.assertRaises(ValueError):
            self.check(self.row(addend=4), 0x436E75)

    def test_external_section_needs_separate_identity_review(self):
        for section in (0, 3):
            self.check(self.row(symbol_section=section), 0x401000)

    def test_same_section_other_owner_is_not_inferred(self):
        for value in (0xFF, 0x200):
            self.check(self.row(symbol_value=value), 0x401000)

    def test_replay_accepts_only_location_verified_private_label_renames(self):
        original = struct.pack("<I", 0x401006) + b"\xCC" * 4
        manifest = """[units.owner]
object = "build/owner.obj"
symbol = "owner"
size = 8
target_address = 0x401000
relocations = [{ offset = 0, type = "DIR32", symbol = "$L42", target = 0x401006, addend = 0 }]
"""
        local = self.row(offset=0, type_id=6, type="DIR32", symbol="$L999",
                         owner_value=0, symbol_value=6)
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "units.toml"
            path.write_text(manifest)
            with mock.patch.multiple(coff, UNITS_MANIFEST=path,
                                     verified_target=lambda: original,
                                     pe_bytes_at=lambda image, address, size: image):
                for row, rejection in [
                    (local, None),
                    (dict(local, symbol_value=7), "owner-local COFF destination"),
                    (dict(local, symbol_section=3), "COFF relocations differ"),
                    (dict(local, type_id=0x14), "COFF relocations differ"),
                    (dict(local, symbol="external"), "COFF relocations differ"),
                ]:
                    with self.subTest(row=row), mock.patch.object(
                        coff, "object_function", return_value=(bytearray(4) + original[4:], [row])
                    ):
                        if rejection:
                            with self.assertRaisesRegex(ValueError, rejection):
                                coff.compare_unit("owner")
                        else:
                            report = coff.compare_unit("owner")
                            self.assertEqual(report["result"], "exact")
                            self.assertEqual(report["relocations"][0]["symbol"], "$L999")
                path.write_text(manifest.replace("addend = 0", "addend = 1"))
                with mock.patch.object(
                    coff, "object_function", return_value=(bytearray(4) + original[4:], [local])
                ), self.assertRaisesRegex(ValueError, "COFF addend differs from manifest"):
                    coff.compare_unit("owner")

    def test_auxless_extent_cannot_be_shortened_by_the_manifest(self):
        code = b"\x90\x90\x90\xC3"
        header = struct.pack("<HHIIIHH", 0x14C, 1, 0, 60 + len(code), 1, 0, 0)
        section = struct.pack("<8sIIIIIIHHI", b".text", 0, 0, len(code), 60,
                              0, 0, 0, 0, 0x20)
        symbol = struct.pack("<8sIhHBB", b"owner", 0, 1, 0x20, 2, 0)
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "owner.obj"
            path.write_bytes(header + section + code + symbol + struct.pack("<I", 4))
            actual, relocations = coff.object_function(path, "owner", expected_size=2)
        self.assertEqual(bytes(actual), code)
        self.assertEqual(relocations, [])

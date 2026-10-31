"""Reject fitted external bases without requiring the private game or compiler."""
import importlib.util
from pathlib import Path
import struct
import tempfile
import tomllib
import unittest
from unittest import mock

spec = importlib.util.spec_from_file_location(
    "coff_symbol_compare", Path(__file__).resolve().parents[1] / "scripts/compare-coff-function.py")
coff = importlib.util.module_from_spec(spec)
spec.loader.exec_module(coff)


class SymbolDestinationTests(unittest.TestCase):
    def row(self, **changes):
        row = dict(offset=0x9C, type="DIR32", symbol="quad", target=0x4DC628, addend=0)
        row.update(changes)
        return row

    def test_empty_and_single_binding_pass(self):
        coff.validate_symbol_destinations([])
        coff.validate_symbol_destinations([self.row()])

    def test_array_members_use_one_base_and_distinct_addends(self):
        coff.validate_symbol_destinations([
            self.row(), self.row(offset=0xB9, addend=0x18),
            self.row(offset=0x120, addend=0x10)])

    def test_reference_kind_does_not_change_function_identity(self):
        coff.validate_symbol_destinations([
            self.row(symbol="callee", type="REL32"),
            self.row(symbol="callee", offset=0x120, type="DIR32")])

    def test_distinct_symbols_may_share_folded_address(self):
        coff.validate_symbol_destinations([
            self.row(symbol="first"), self.row(symbol="second", offset=0xB9)])

    def test_wrong_stride_cannot_be_hidden_by_moving_the_base(self):
        with self.assertRaisesRegex(ValueError, "multiple manifest destinations"):
            coff.validate_symbol_destinations([
                self.row(), self.row(offset=0xB9, addend=0x14, target=0x4DC62C)])

    def test_nonzero_addend_does_not_authorize_a_second_base(self):
        with self.assertRaises(ValueError):
            coff.validate_symbol_destinations([
                self.row(addend=4), self.row(offset=0xB9, addend=0, target=0x4DC62C)])

    def test_known_function_owner_overrides_a_callers_single_binding(self):
        manifest = '[units.callee]\nsymbol = "callee"\ntarget_address = 4198400\n'
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "units.toml"
            path.write_text(manifest)
            with mock.patch.object(coff, "UNITS_MANIFEST", path):
                for kind in ("REL32", "DIR32"):
                    with self.subTest(kind=kind):
                        coff.validate_symbol_destinations([
                            self.row(symbol="callee", type=kind, target=0x401000, addend=4)])
                        with self.assertRaisesRegex(ValueError, "canonical function owner"):
                            coff.validate_symbol_destinations([
                                self.row(symbol="callee", type=kind, target=0x401004)])

    def test_changed_manifest_content_cannot_reuse_an_old_owner_index(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "units.toml"
            with mock.patch.object(coff, "UNITS_MANIFEST", path):
                path.write_text('[units.callee]\nsymbol = "callee"\ntarget_address = 4198400\n')
                coff.validate_symbol_destinations([self.row(symbol="callee", target=0x401000)])
                path.write_text('[units.callee]\nsymbol = "callee"\ntarget_address = 4198404\n')
                with self.assertRaisesRegex(ValueError, "canonical function owner"):
                    coff.validate_symbol_destinations([self.row(symbol="callee", target=0x401000)])

    def test_reviewed_multiple_owners_are_allowed_without_fitting_a_new_base(self):
        manifest = '''[units.first]
symbol = "callee"
target_address = 4198400
[units.second]
symbol = "callee"
target_address = 4202496
'''
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "units.toml"
            path.write_text(manifest)
            with mock.patch.object(coff, "UNITS_MANIFEST", path):
                for address in (0x401000, 0x402000):
                    coff.validate_symbol_destinations([self.row(symbol="callee", target=address)])
                with self.assertRaisesRegex(ValueError, "canonical function owner"):
                    coff.validate_symbol_destinations([self.row(symbol="callee", target=0x401004)])

    def test_all_manifest_references_keep_canonical_function_owner_bases(self):
        units = tomllib.loads(coff.UNITS_MANIFEST.read_text())["units"]
        for name, unit in units.items():
            with self.subTest(unit=name):
                coff.validate_symbol_destinations(unit.get("relocations", []))

    def test_equal_linked_bytes_cannot_redirect_a_canonical_callee(self):
        field = dict(offset=0, type_id=6, type="DIR32", symbol="callee", addend=0,
                     symbol_section=0, symbol_value=0, owner_section=1, owner_value=0)
        manifest = '''[units.callee]
symbol = "callee"
target_address = 4198400
[units.caller]
object = "build/unused.obj"
symbol = "caller"
size = 4
target_address = 4202496
relocations = [{ offset = 0, type = "DIR32", symbol = "callee", target = 4198512 }]
'''
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "units.toml"
            path.write_text(manifest)
            with mock.patch.object(coff, "UNITS_MANIFEST", path), \
                    mock.patch.object(coff, "object_function", return_value=(bytearray(4), [field])), \
                    mock.patch.object(coff, "verified_target", return_value=b"verified"), \
                    mock.patch.object(coff, "pe_bytes_at", return_value=struct.pack("<I", 0x401070)), \
                    self.assertRaisesRegex(ValueError, "canonical function owner"):
                coff.compare_unit("caller")

    def test_reviewed_crt_entries_are_distinct(self):
        coff.validate_native_crt_destinations([
            self.row(symbol="_free", target=0x47B163),
            self.row(symbol="??3@YAXPAX@Z", target=0x47B249)])

    def test_all_canonical_units_keep_reviewed_crt_entries(self):
        root = Path(__file__).resolve().parents[1]
        units = tomllib.loads((root / "config/match-units.toml").read_text())["units"]
        for name, unit in units.items():
            with self.subTest(unit=name):
                coff.validate_native_crt_destinations(unit.get("relocations", []))

    def test_free_cannot_be_rebound_to_delete_even_at_one_field(self):
        with self.assertRaisesRegex(ValueError, "native CRT destination differs"):
            coff.validate_native_crt_destinations([
                self.row(symbol="_free", target=0x47B249)])

    def test_addend_cannot_move_the_reviewed_delete_base(self):
        with self.assertRaises(ValueError):
            coff.validate_native_crt_destinations([
                self.row(symbol="??3@YAXPAX@Z", target=0x47B245, addend=4)])

    def test_canonical_replay_rejects_false_free_identity_despite_equal_bytes(self):
        field = dict(offset=0, type_id=6, type="DIR32", symbol="_free",
                     addend=0, symbol_section=0, symbol_value=0,
                     owner_section=1, owner_value=0)
        manifest = '''[units.false_free]
object = "build/unused.obj"
symbol = "owner"
size = 4
target_address = 4198400
relocations = [
  { offset = 0, type = "DIR32", symbol = "_free", target = 4698697 },
]
'''
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "units.toml"
            path.write_text(manifest)
            with mock.patch.object(coff, "UNITS_MANIFEST", path), \
                    mock.patch.object(coff, "object_function", return_value=(bytearray(4), [field])), \
                    mock.patch.object(coff, "verified_target", return_value=b"verified"), \
                    mock.patch.object(coff, "pe_bytes_at", return_value=struct.pack("<I", 0x47B249)), \
                    self.assertRaisesRegex(ValueError, "native CRT destination differs"):
                coff.compare_unit("false_free")

    def test_canonical_replay_rejects_fitted_bases_even_when_bytes_would_match(self):
        code = bytearray(struct.pack("<II", 0, 0x14))
        target = struct.pack("<II", 0x4DC628, 0x4DC640)
        fields = [dict(offset=i, type_id=6, type="DIR32", symbol="quad",
                       addend=addend, symbol_section=0, symbol_value=0,
                       owner_section=1, owner_value=0)
                  for i, addend in [(0, 0), (4, 0x14)]]
        manifest = '''[units.fitted]
source = "unused.cpp"
object = "build/unused.obj"
symbol = "owner"
size = 8
target_address = 4198400
relocations = [
  { offset = 0, type = "DIR32", symbol = "quad", target = 5097000 },
  { offset = 4, type = "DIR32", symbol = "quad", target = 5097004 },
]
'''
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "units.toml"
            path.write_text(manifest)
            with mock.patch.object(coff, "UNITS_MANIFEST", path), \
                    mock.patch.object(coff, "object_function", return_value=(code, fields)), \
                    mock.patch.object(coff, "verified_target", return_value=b"verified"), \
                    mock.patch.object(coff, "pe_bytes_at", return_value=target), \
                    self.assertRaisesRegex(ValueError, "multiple manifest destinations"):
                coff.compare_unit("fitted")

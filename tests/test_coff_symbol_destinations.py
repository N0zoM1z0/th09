"""Reject fitted external bases without requiring the private game or compiler."""
import importlib.util
from pathlib import Path
import struct
import tempfile
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

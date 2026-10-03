"""Reject manifest-fitted local labels without needing the private target."""
import importlib.util
from pathlib import Path
import unittest

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

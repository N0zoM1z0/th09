"""Target-independent operand binding guards; no private target or decoder."""

import importlib.util
from pathlib import Path
import struct
import unittest


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "supervisor_update_diagnostic", ROOT / "scripts/inspect-supervisor-update.py"
)
DIAGNOSTIC = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(DIAGNOSTIC)


def local_field(offset=4, symbol_value=28):
    return dict(offset=offset, type="DIR32", symbol="$switch_table", addend=0,
                symbol_section=3, symbol_value=symbol_value,
                owner_section=3, owner_value=4)


class SupervisorBindingTests(unittest.TestCase):
    def test_local_destination_uses_actual_symbol_coordinate_not_field_offset(self):
        # Moving a field must not move the table that its actual symbol names.
        for offset in (4, 12):
            linked, rows, table = DIAGNOSTIC.bind(bytes(32), [local_field(offset)])
            self.assertEqual(table, 24)
            self.assertEqual(struct.unpack_from("<I", linked, offset)[0], DIAGNOSTIC.BASE + 24)
            self.assertEqual(rows[0]["destination"], DIAGNOSTIC.BASE + 24)

    def test_unknown_external_is_not_resolved_from_plausible_raw_address(self):
        raw = bytearray(32)
        struct.pack_into("<I", raw, 12, DIAGNOSTIC.BASE)
        unknown = dict(local_field(12), symbol="unreviewed_external", symbol_section=0,
                       symbol_value=0, addend=DIAGNOSTIC.BASE)
        with self.assertRaisesRegex(ValueError, "unreviewed external symbol"):
            DIAGNOSTIC.bind(raw, [local_field(), unknown])

    def test_partially_overlapping_fields_are_rejected(self):
        with self.assertRaisesRegex(ValueError, "overlapping fields"):
            DIAGNOSTIC.bind(bytes(32), [local_field(4), local_field(6)])

    def test_field_and_label_must_belong_to_complete_owner(self):
        with self.assertRaisesRegex(ValueError, "field outside complete owner"):
            DIAGNOSTIC.bind(bytes(32), [local_field(30)])
        with self.assertRaisesRegex(ValueError, "local label outside owner"):
            DIAGNOSTIC.bind(bytes(32), [local_field(symbol_value=36)])


if __name__ == "__main__":
    unittest.main()

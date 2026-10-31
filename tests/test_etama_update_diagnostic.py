"""Target-independent checks for Etama's compiler-private label binding."""

import importlib.util
from pathlib import Path
import subprocess
import sys
import tomllib
import unittest


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "inspect_etama_update", ROOT / "scripts/inspect-etama-update.py"
)
INSPECT = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(INSPECT)


def private_row(label="$L9999", *, offset=0x0D0, value=0x894, section=7):
    return {
        "offset": offset,
        "type": "DIR32",
        "symbol": label,
        "addend": 0,
        "symbol_section": section,
        "symbol_value": value,
        "owner_section": 7,
        "owner_value": 0,
    }


class PrivateLabelTests(unittest.TestCase):
    def test_external_bindings_agree_with_complete_canonical_owner(self):
        with (ROOT / "config/match-units.toml").open("rb") as stream:
            unit = tomllib.load(stream)["units"]["etama-controller-update"]
        for row in unit["relocations"]:
            if row["offset"] in INSPECT.PRIVATE_FIELDS:
                continue
            with self.subTest(offset=row["offset"], symbol=row["symbol"]):
                self.assertEqual(
                    INSPECT.relocation_destination(dict(row, addend=row.get("addend", 0))),
                    row["target"] + row.get("addend", 0),
                )

    def test_help_requires_no_optional_decoder(self):
        result = subprocess.run(
            [sys.executable, "-S", str(ROOT / "scripts/inspect-etama-update.py"), "--help"],
            capture_output=True, text=True, check=False,
        )
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("usage:", result.stdout)

    def test_renamed_label_at_reviewed_offset(self):
        self.assertEqual(
            INSPECT.relocation_destination(private_row()),
            INSPECT.BASE + 0x894,
        )

    def test_wrong_destination_fails(self):
        with self.assertRaises(ValueError):
            INSPECT.relocation_destination(private_row(value=0x895))

    def test_other_section_fails(self):
        with self.assertRaises(ValueError):
            INSPECT.relocation_destination(private_row(section=8))

    def test_unreviewed_private_field_fails(self):
        with self.assertRaises(ValueError):
            INSPECT.relocation_destination(private_row(offset=0x0D4))


if __name__ == "__main__":
    unittest.main()

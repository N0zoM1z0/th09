from __future__ import annotations

import importlib.util
from pathlib import Path
import sys
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "th09_tracking_csv", ROOT / "scripts/tracking_csv.py"
)
if SPEC is None or SPEC.loader is None:
    raise RuntimeError("cannot load tracking CSV module")
TRACKING = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = TRACKING
SPEC.loader.exec_module(TRACKING)


class TrackingCsvTests(unittest.TestCase):
    def test_rewrite_preserves_unselected_legacy_line(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "rows.csv"
            original = "address,value\n0x1,keep,legacy-extra\n0x2,old\n"
            path.write_text(original, encoding="utf-8")

            def mutate(row: dict[str, str]) -> None:
                row["value"] = "new"

            self.assertEqual(
                TRACKING.rewrite_selected_rows(path, {"0x2"}, mutate, False), 1
            )
            self.assertEqual(path.read_text(encoding="utf-8"), original)
            self.assertEqual(
                TRACKING.rewrite_selected_rows(path, {"0x2"}, mutate, True), 1
            )
            self.assertEqual(
                path.read_text(encoding="utf-8"),
                "address,value\n0x1,keep,legacy-extra\n0x2,new\n",
            )

    def test_rewrite_rejects_missing_selected_address(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "rows.csv"
            path.write_text("address,value\n0x1,old\n", encoding="utf-8")
            with self.assertRaises(ValueError):
                TRACKING.rewrite_selected_rows(path, {"0x2"}, lambda row: None, False)

    def test_rows_by_address_rejects_duplicates(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "rows.csv"
            path.write_text(
                "address,value\n0x1,first\n0x1,second\n", encoding="utf-8"
            )
            with self.assertRaises(ValueError):
                TRACKING.rows_by_address(path)


if __name__ == "__main__":
    unittest.main()

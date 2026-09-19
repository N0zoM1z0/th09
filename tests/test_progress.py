from __future__ import annotations

import csv
import importlib.util
from pathlib import Path
import sys
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "th09_progress", ROOT / "scripts/progress.py"
)
if SPEC is None or SPEC.loader is None:
    raise RuntimeError("cannot load progress module")
PROGRESS = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = PROGRESS
SPEC.loader.exec_module(PROGRESS)


class ProgressMeasureTests(unittest.TestCase):
    def test_reviewed_unknown_is_not_unreviewed_pending(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            config = Path(directory)
            with (config / "functions.csv").open("w", newline="") as stream:
                writer = csv.writer(stream)
                writer.writerow(("address", "size", "source_file"))
                writer.writerows(
                    (
                        ("0x1", "10", "src/example.cpp"),
                        ("0x2", "20", ""),
                        ("0x3", "30", ""),
                        ("0x4", "40", ""),
                    )
                )
            with (config / "function-origins.csv").open("w", newline="") as stream:
                writer = csv.writer(stream)
                writer.writerow(("address", "disposition", "evidence_id"))
                writer.writerows(
                    (
                        ("0x1", "authored", "review-a"),
                        ("0x2", "exclude", "review-b"),
                        ("0x3", "review", "reviewed-but-unknown"),
                        ("0x4", "review", "ida-1.50a-initial-inventory"),
                    )
                )
            with (config / "matches.csv").open("w", newline="") as stream:
                writer = csv.writer(stream)
                writer.writerow(("address", "size"))
                writer.writerow(("0x1", "10"))
            (config / "implemented.csv").write_text("src/example.cpp\n", encoding="utf-8")

            previous = PROGRESS.CONFIG
            PROGRESS.CONFIG = config
            self.addCleanup(setattr, PROGRESS, "CONFIG", previous)
            values = PROGRESS.measures()

        self.assertEqual(values["reviewed"], 3)
        self.assertEqual(values["pending"], 1)
        self.assertEqual(values["unresolved"], 1)
        self.assertEqual(values["authored"], 1)
        self.assertEqual(values["excluded"], 1)
        self.assertEqual(values["source_present_bytes"], 10)
        self.assertEqual(values["source_nonexact"], 0)
        self.assertEqual(values["without_source"], 0)


if __name__ == "__main__":
    unittest.main()

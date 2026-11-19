"""Ledger guard for exact owners versus active nonexact frontiers."""
from __future__ import annotations

import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    "th09_validate_tracking", ROOT / "scripts/validate-tracking.py"
)
tracking = importlib.util.module_from_spec(spec)
spec.loader.exec_module(tracking)


class ExactFrontierConsistencyTests(unittest.TestCase):
    def test_distinct_frontier_is_allowed(self) -> None:
        tracking.validate_nonexact_frontiers(
            {"remaining-owner": {"address": 0x00410000}},
            {"0x00420000"},
        )

    def test_canonical_exact_owner_cannot_remain_nonexact(self) -> None:
        with self.assertRaisesRegex(ValueError, "contradicts canonical exact"):
            tracking.validate_nonexact_frontiers(
                {"stale-rotate": {"address": 0x0042AF40}},
                {"0x0042AF40"},
            )

    def test_duplicate_frontier_address_is_rejected(self) -> None:
        with self.assertRaisesRegex(ValueError, "duplicate frontier address"):
            tracking.validate_nonexact_frontiers(
                {"a": {"address": 0x00410000}, "b": {"address": 0x00410000}},
                set(),
            )

    def test_missing_numeric_address_is_rejected(self) -> None:
        with self.assertRaisesRegex(ValueError, "numeric target address"):
            tracking.validate_nonexact_frontiers(
                {"broken": {"address": "0x00410000"}},
                set(),
            )


if __name__ == "__main__":
    unittest.main()

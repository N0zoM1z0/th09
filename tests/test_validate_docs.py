from __future__ import annotations

import importlib.util
from pathlib import Path
import sys
import unittest
from unittest.mock import Mock, patch


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
SPEC = importlib.util.spec_from_file_location(
    "th09_validate_docs", ROOT / "scripts/validate-docs.py"
)
if SPEC is None or SPEC.loader is None:
    raise RuntimeError("cannot load documentation validator")
VALIDATE_DOCS = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = VALIDATE_DOCS
SPEC.loader.exec_module(VALIDATE_DOCS)


class KnowledgeStateTests(unittest.TestCase):
    def test_exact_primary_stale_state_markers_are_detected(self) -> None:
        for state in (
            "source-present / non-exact",
            "source-present / codegen-nearmatch",
            "source-present / codegen-blocked",
            "hard-frontier / stable non-exact",
        ):
            self.assertIsNotNone(VALIDATE_DOCS.STALE_EXACT_STATE.search(state))

    def test_mixed_exact_state_is_not_treated_as_stale(self) -> None:
        self.assertIsNone(
            VALIDATE_DOCS.STALE_EXACT_STATE.search("mixed exact/source-present")
        )


class HandoffDerivedSummaryTests(unittest.TestCase):
    ACTUAL = {
        "authored_bytes": 275770, "exact_bytes": 224124,
        "implemented": 980, "matches": 935, "source_nonexact": 45,
    }
    SUMMARY = (
        "**224,124 / 275,770 reviewed-authored bytes = 81.2721% exact**.\n"
        "To reach 95%, another **37,858\nbytes** must be exact.\n"
        "counts are the verified **980 source-present / 935 exact / "
        "45 non-exact**.\n"
    )

    def test_live_summary_is_accepted(self) -> None:
        VALIDATE_DOCS.validate_handoff_derived_summary(self.SUMMARY, self.ACTUAL)

    def test_old_percentage_is_rejected_even_if_table_is_current(self) -> None:
        stale = self.SUMMARY.replace("81.2721%", "81.1252%")
        with self.assertRaisesRegex(ValueError, "percentage"):
            VALIDATE_DOCS.validate_handoff_derived_summary(stale, self.ACTUAL)

    def test_old_95_percent_gap_is_rejected(self) -> None:
        stale = self.SUMMARY.replace("37,858", "38,263")
        with self.assertRaisesRegex(ValueError, "95% gap"):
            VALIDATE_DOCS.validate_handoff_derived_summary(stale, self.ACTUAL)

    def test_old_restart_summary_is_rejected(self) -> None:
        stale = self.SUMMARY.replace("935 exact / 45", "934 exact / 46")
        with self.assertRaisesRegex(ValueError, "restart snapshot"):
            VALIDATE_DOCS.validate_handoff_derived_summary(stale, self.ACTUAL)

    def test_zero_denominator_does_not_divide_by_zero(self) -> None:
        with self.assertRaisesRegex(ValueError, "denominator"):
            VALIDATE_DOCS.validate_handoff_derived_summary(
                self.SUMMARY, {**self.ACTUAL, "authored_bytes": 0}
            )


class AcceptedOwnerTableTests(unittest.TestCase):
    ADDRESS = "0x004182E0"
    UNIT = "front-message-load-for-side-count"
    RECEIPT = "a" * 64
    ROW = (
        "| `FrontMessageRuntimeView::LoadForSideCount` | `0x004182E0`, "
        "**405/405**, 18 fields | `front-message-load-for-side-count` | "
        "`4d921a2`; `receipt:" + RECEIPT + "` |"
    )
    HEADER = "## Closed codegen landmarks and accepted receipts\n"
    TABLE = (
        "| Exact owner | Target/bytes | Canonical unit | Accepted source commit and receipt |\n"
        "| --- | --- | --- | --- |\n"
    )
    UNITS = {UNIT: {
        "size": 405, "target_address": 0x004182E0,
        "functions": ["FrontMessageRuntimeView::LoadForSideCount"],
        "relocations": [{}] * 18,
    }}
    MATCHES = [{
        "address": ADDRESS, "name": "FrontMessageRuntimeView::LoadForSideCount",
        "unit": UNIT, "status": "matching",
    }]

    def test_complete_recent_acceptance_row_is_valid(self) -> None:
        VALIDATE_DOCS.validate_accepted_owner_table(
            self.HEADER + self.TABLE + self.ROW, self.MATCHES, self.UNITS
        )

    def test_stale_address_or_field_count_is_rejected(self) -> None:
        for row in (
            self.ROW.replace("0x004182E0", "0x004182E1"),
            self.ROW.replace("18 fields", "17 fields"),
            self.ROW.replace("405/405", "403/405"),
        ):
            with self.subTest(row=row), self.assertRaises(ValueError):
                VALIDATE_DOCS.validate_accepted_owner_table(
                    self.HEADER + self.TABLE + row, self.MATCHES, self.UNITS
                )

    def test_invalid_receipt_or_stale_pending_caption_is_rejected(self) -> None:
        broken = self.ROW.replace("receipt:" + self.RECEIPT, "receipt:pending")
        with self.assertRaisesRegex(ValueError, "incomplete"):
            VALIDATE_DOCS.validate_accepted_owner_table(
                self.HEADER + self.TABLE + broken, self.MATCHES, self.UNITS
            )
        with self.assertRaisesRegex(ValueError, "pending receipt"):
            VALIDATE_DOCS.validate_accepted_owner_table(
                self.HEADER + "receipt pending\n" + self.TABLE + self.ROW,
                self.MATCHES, self.UNITS
            )


class AcceptedReceiptArchiveRoutingTests(unittest.TestCase):
    def test_actual_archive_table_is_bound_to_live_canonical_manifest(self) -> None:
        receipt_text = VALIDATE_DOCS.RECEIPTS_ARCHIVE.read_text(encoding="utf-8")
        with (ROOT / "config" / "match-units.toml").open("rb") as stream:
            import tomllib
            units = tomllib.load(stream)["units"]
        VALIDATE_DOCS.validate_accepted_owner_table(
            receipt_text, VALIDATE_DOCS.rows("matches.csv"), units
        )
        handoff = VALIDATE_DOCS.HANDOFF.read_text(encoding="utf-8")
        self.assertNotIn(
            "## Closed codegen landmarks and accepted receipts", handoff
        )
        self.assertIn("[the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md)", handoff)

    def test_historical_receipts_do_not_transmute_into_new_exact_credit(self) -> None:
        handoff = VALIDATE_DOCS.HANDOFF.read_text(encoding="utf-8")
        self.assertIn("980 source-present / 939 exact / 41 non-exact", handoff)
        self.assertIn("**939 exact**", handoff)


class MarkdownLinkTests(unittest.TestCase):
    def test_code_examples_are_not_local_links(self) -> None:
        text = """`g_EclExInsn[READ_I(...)](enemy, instruction)` and [handoff](docs/RE_HANDOFF.md)
```cpp
g_ExInstructionCallbacks[stateIndex](enemy, instruction)
```
~~~cpp
g_ExInstructionCallbacks[stateIndex](enemy, instruction)
~~~
"""
        self.assertEqual(
            VALIDATE_DOCS.prose_link_targets(text), ["docs/RE_HANDOFF.md"]
        )


class ActiveFrontierTests(unittest.TestCase):
    HEADER = "| Owner | Target address | Current comparison | Remaining work |\n| --- | --- | --- | --- |\n"
    FUNCTIONS = [
        {"address": "0x004086C0", "owner": "authored", "source_file": "src/EclManager.cpp"},
        {"address": "0x0040CDD0", "owner": "authored", "source_file": "src/EffectManager.cpp"},
    ]

    def test_partial_frontier_can_group_nonexact_owners(self) -> None:
        VALIDATE_DOCS.validate_active_frontiers(
            self.HEADER + "| Group | 0x004086C0 / 0x0040CDD0 | measured differences | scheduling |\n",
            self.FUNCTIONS, set(),
        )

    def test_closed_owner_is_rejected_even_with_exact_wording(self) -> None:
        with self.assertRaisesRegex(ValueError, "exact owner remains"):
            VALIDATE_DOCS.validate_active_frontiers(
                self.HEADER + "| Effect | 0x0040CDD0 | 475/475 exact | closed |\n",
                self.FUNCTIONS, {"0x0040CDD0"},
            )

    def test_unknown_or_unmapped_owner_is_rejected(self) -> None:
        for functions in ([], [{**self.FUNCTIONS[0], "source_file": ""}],
                          [{**self.FUNCTIONS[0], "owner": "compiler"}]):
            with self.subTest(functions=functions):
                with self.assertRaisesRegex(ValueError, "lacks maintained authored source"):
                    VALIDATE_DOCS.validate_active_frontiers(
                        self.HEADER + "| RunEcl | 0x004086C0 | 30 differences | scheduling |\n",
                        functions, set(),
                    )

    def test_duplicate_address_is_rejected(self) -> None:
        row = "| RunEcl | 0x004086C0 | 30 differences | scheduling |\n"
        with self.assertRaisesRegex(ValueError, "duplicate active frontier"):
            VALIDATE_DOCS.validate_active_frontiers(self.HEADER + row + row, self.FUNCTIONS, set())

    def test_missing_or_empty_table_is_rejected(self) -> None:
        for text in ("", self.HEADER):
            with self.subTest(text=text):
                with self.assertRaisesRegex(ValueError, "missing or empty"):
                    VALIDATE_DOCS.validate_active_frontiers(text, self.FUNCTIONS, set())


class ReplaceInPlaceHandoffTests(unittest.TestCase):
    SAMPLE = """# TH09 handoff
## Live ledger snapshot
current
## Active frontiers
current
## Restart and verification
commands
## Evidence and cleanup boundaries
historical references are routed by Git
"""

    def test_snapshot_only_structure_is_accepted(self) -> None:
        VALIDATE_DOCS.validate_handoff_as_current_snapshot(self.SAMPLE)
        VALIDATE_DOCS.validate_handoff_as_current_snapshot(
            VALIDATE_DOCS.HANDOFF.read_text(encoding="utf-8")
        )

    def test_dated_packets_are_rejected(self) -> None:
        for heading in ("## October 12 session notes", "## Packet 991",
                        "## 2026-10-12 replay log"):
            with self.subTest(heading=heading):
                with self.assertRaisesRegex(ValueError, "dated packet journal"):
                    VALIDATE_DOCS.validate_handoff_as_current_snapshot(
                        self.SAMPLE + heading + "\nold checkpoint\n"
                    )

    def test_missing_or_duplicate_live_section_is_rejected(self) -> None:
        with self.assertRaisesRegex(ValueError, "requires one live section"):
            VALIDATE_DOCS.validate_handoff_as_current_snapshot(
                self.SAMPLE.replace("## Active frontiers", "## Active functions")
            )
        with self.assertRaisesRegex(ValueError, "requires one live section"):
            VALIDATE_DOCS.validate_handoff_as_current_snapshot(
                self.SAMPLE + "## Live ledger snapshot\nformer totals\n"
            )


class HandoffTotalsTests(unittest.TestCase):
    def test_duplicate_total_cannot_hide_an_older_conflicting_count(self) -> None:
        text = VALIDATE_DOCS.HANDOFF.read_text(encoding="utf-8")
        text = "| Canonical exact functions | 1 |\n" + text
        with patch.object(VALIDATE_DOCS, "HANDOFF", Mock(read_text=Mock(return_value=text))):
            with self.assertRaisesRegex(ValueError, "duplicate handoff total"):
                VALIDATE_DOCS.validate_handoff_totals()


if __name__ == "__main__":
    unittest.main()

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

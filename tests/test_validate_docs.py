from __future__ import annotations

import importlib.util
from pathlib import Path
import sys
import unittest


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


if __name__ == "__main__":
    unittest.main()

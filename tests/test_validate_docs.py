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


if __name__ == "__main__":
    unittest.main()

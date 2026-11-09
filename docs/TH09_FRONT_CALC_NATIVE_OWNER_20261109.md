# FrontCalcCallback uses the native Front factory allocation

The complete original TH09 Front calculation owner at `0x00417630`
(1,491 physical bytes and 107 original relocations) now uses the same
`FrontFactoryConstructView` as the original-exact Front constructor,
AddedCallback and 1,474-byte draw callback. The previous independently
maintained `FrontCalcView` duplicated the 0x1293C-byte allocation, hid 81
of the 88 front animation VMs behind opaque padding, and re-declared
scalar result and transition fields. These duplicate object layouts have
been removed.

The 88 native animation VMs, two-by-five panel vertices, message runtime,
dynamic VM, transition VMs, result and marker VMs, and all consumed fields now
have the same C++ storage authority. The existing `FrontCalcMessageRuntimeView`
remains only a narrow **method ABI adapter** for the three exact target-linked
`Update` / `Setup` calls. Its C++ signatures and exact original destinations
`0x00416590` / `0x00416160` are intentionally retained: changing the
nominal method symbol merely to rename the receiver would not demonstrate
historical identity or original-bound relocation equality.

Recompiling the changed `src/FrontCalcCallback.cpp` with its pinned VC7.1
`/O2 /Ob1` profile produces **1,491 / 1,491 original bytes, 107 / 107
relocations, and zero differences**. The real nested message receiver address
remains +0xE944. No explicit assembly, padding, dummy register writes or
new call boundaries were used.

Affected owner/consumer checks are complete, not excerpts:

| Source owner | Bytes | Relocations | Result |
| --- | ---: | ---: | --- |
| FrontCalcCallback | 1491 | 107 | exact |
| FrontDrawCallback | 1474 | 127 | exact |
| FrontAddedCallback | 706 | 37 | exact |
| CreateGameSubsystem | 220 | 22 | exact |

Reproduce the changed owner with `python3 -B scripts/build-match-unit.py
--unit front-calc-callback` then `python3 -B scripts/compare-coff-function.py
--unit front-calc-callback --json`. Exact source-equivalent constructor
members are still origin-unclassified; no new authored-function credit or
runtime/game-product completeness is asserted. No full-project replay ran.

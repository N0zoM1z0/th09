# TH09 Player movement native reference models (2026-11-06)

The Japanese v1.50a `PlayerLifecycleView::UpdateMovementAndOptions` remains an
**open** byte-exact reconstruction. Its complete original physical owner is
1,900 bytes (1,835 code plus the compiler-owned alignment/tables) with 66
independently bound fields, 23 calls and two real dispatch tables. The current
maintained candidate still has 122 differing physical bytes. None of the
natural source experiments here is claimed as new exact gameplay credit.

## Actual native movement storage

The source now binds `PlayerMovementFields &runtime` directly to the existing
`player->movementFields1CCC` member, rather than carrying an avoidable pointer
to the same subobject. A compile-time assertion checks the native
`+0x1CCC` offset and `0x20` record size, also used by the 3,238-byte exact
Player selector and the Player lifecycle owners. All velocity, scale and angle
operations are now ordinary `runtime.member` expressions. There is no
secondary movement allocation, ABI change, forced register choice or binary
patch. Original member spellings remain hypotheses.

## Ten natural source hypotheses, fixed compiler profile

Ten complete `src/PlayerMovement.cpp` compilations were compared to the
verified target under the existing `/O2 /Ob1 /EHsc` profile:

| Source hypothesis | Complete result | Interpretation |
| --- | --- | --- |
| Native movement record reference | 1,900 bytes, 66 fields, 122 unlike | Adopted; no regression |
| Explicit Player position float-pointer operator | 1,900/66/122 | Equivalent but not adopted |
| Equivalent masked focus assignment | 1,900/66/122 | Equivalent but not adopted |
| Direct field address for position X | 1,884 bytes, 65 fields, 631 unlike | Loses actual Float3 provider call |
| Local lower-X bound snapshot | 1,900 bytes, 65 fields, 382 unlike | Original field identity lost |
| Local lower-Y bound snapshot | 1,900 bytes, 65 fields, 373 unlike | Original field identity lost |
| Local upper-X bound snapshot | 1,900 bytes, 66 fields, 387 unlike | Existing equal bytes regressed |
| Local upper-Y bound snapshot | Rejected by original extent/alignment guard | Not adopted |
| Direct native focus-input field | Rejected by COFF/decoded operand census | Not adopted |
| Indexed native focus-input field | Rejected by COFF/decoded operand census | Not adopted |

The inspector checks every physical code/data byte, verifies original scalar
literal definitions, the actual original referents, and separately validates
the 66 COFF fields and exact 23-call sequence. A smaller difference count
alone cannot justify a source change if it damages an already matching byte.
The adopted movement-reference candidate preserves **all 1,778 originally
equal physical positions** and the same 122 unlike positions.

The bounded focused command
`python3 -B scripts/check-boss-frontiers.py --owner movement` passes both
the full noncrediting 1,900/66/122 movement audit and two original-bound exact
nearby/dependency owners (540 bytes / 40 fields). No whole-project cold replay
was run. Private local trial sources/objects/reports remain in
`.analysis/gpt-web-player-movement-natural-20261106/`; the validated
source-dependent result is `.analysis/gpt-web-player-movement-native-ref-focused.json`.

The target SHA-256 is
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The 13 still-nonexact function frontier, original C++ TU identity, full Windows
build, runtime semantics, and later port stages remain open.

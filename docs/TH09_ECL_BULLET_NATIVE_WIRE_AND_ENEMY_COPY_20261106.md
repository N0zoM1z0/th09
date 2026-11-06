# TH09 ECL bullet wire and Enemy descriptor copy (2026-11-06)

This checkpoint touches two **large, still-open** original-PE codegen owners.
It does not claim either has become byte-exact. The source improvements use
ordinary C++ on the real native instruction and bullet descriptor layouts,
not new compiled opcode functions, hand-selected registers, assembly, padding,
volatile barriers or a new compiler profile.

## Recover native wire operand pointers

`Th09EclRawInstructionHeaderView` already owns the zero-length trailing
`arguments` array at native `+0x0C`, proven by the existing i386 `offsetof`
assertion in `EclOpcodes.hpp`. The bullet/laser reader in
`src/EclRunBullet.inl` now derives its float, int, color and laser-argument
windows from `instruction->arguments` instead of reproducing the instruction
header's `+0x0C` or `+0x0E` as untyped byte arithmetic. The matching lexical
`DispatchShotInstruction` in `src/EclPostRuntime.inl` uses the same real
trailing array for its typed shot payload.

Five fixed-profile source variants (scalar only, short only, typed laser
record, all bullet accesses, and the same with the post-update shot payload)
were compiled as the **entire** real `EclManager.cpp` owner. All five retained
15,564 physical RunEcl bytes, 598 actual fields, zero field/table differences,
and the same 30 ordinary differences. No separate opcode bytes were counted as
new functions.

## Avoid Enemy XYZ aliasing without changing codegen

The 3,900-byte `EnemyManagerView::OnUpdate` previously copied its 12-byte native
Enemy XYZ into `BulletSpawnDescriptor::position` using a cast between distinct
POD vector types. The source now uses ordinary `memcpy` with the destination's
actual `sizeof`. This preserves the 12-byte object representation without
reinterpreting either C++ struct as an unrelated lvalue. The original
constructor remains the real `0x214`-byte descriptor carrier; no helper is
invented or called.

The complete target-bound Enemy comparison still reports 3,900 physical
bytes, 97 independently bound fields, 69 ordered calls, 170 matching direct
CFG blocks, and **13** unequal ordinary bytes. The residual belongs to the
original index, side/threshold and descriptor scheduling; the typed copy does
not resolve it, but also does not damage any previously matching position.

## Focused replay and original binding

After the combined source change, the fixed main compiler's 77 exact ECL
siblings remained fully linked-byte equal. The genuine compiler renamed one
local label in `ecl-post-update-movement` and one in `ecl-compare-operands`;
both actual COFF section/value positions independently resolve to their
unchanged original targets at `0x00408548` and `0x00407A4C`. Only their
private `$L` names were refreshed in the canonical manifest. No relocation
offset, kind, addend, external call identity or target address changed.

The bounded `--owner runecl` replay passed **99 complete exact comparisons**
(30,461 physical bytes / 1,404 fields), including the independent exact
movement and effect callees, plus the complete noncrediting RunEcl
15564/598/30 audit, 146/148 matching opcode handler shapes and both checked
semantic windows (opcode4 and opcode155). The separate `--owner enemy
--skip-exact-siblings` replay passed the full 3900/97/13 Enemy frontier and
the 82-byte exact native collision append. Neither is a whole-project cold
replay, and neither confers exactness or source-origin credit.

The ignored local trials and reports are at
`.analysis/gpt-web-ecl-bullet-wire-20261106/`,
`.analysis/gpt-web-bullet-wire-current-focused.json` and
`.analysis/gpt-web-enemy-typed-descriptor-focused.json`. Reproduce only these
two owners after changing their source:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/check-boss-frontiers.py --owner runecl
python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
```

Original target: Japanese v1.50a SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Authored totals stay 1,034 source-present / 1,021 exact / 13 nonexact;
whole-product build/runtime, semantic and port phases remain open.

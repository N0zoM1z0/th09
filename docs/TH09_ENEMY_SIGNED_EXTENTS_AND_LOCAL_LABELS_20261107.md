# Enemy OnUpdate signed bounds and original-bound ECL labels — 2026-11-07

Target: original Japanese TH09 v1.50a, verified disk SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Only the real registered `th09` live worktree was edited. The original PE
is unmodified, and none of the diagnostics below grants new exact credit.

## Native Enemy scan and callback extents

The source-facing `EnemyManagerView` has `EnemyView enemies5758[129]`.
The exact owner and spawn-failure context distinguish the 129th sentinel from
the **128 scanned active records**. `EnemyView` independently owns
`secondaryVms2AC[2]`, `lifeCallbackThresholds33B0[4]`, and
`childEclBlocks33D8[4]`.

`EnemyManager::OnUpdate` now uses the native arrays as the source of those
four bounds (the callback threshold array is traversed twice). The compiler
must observe an **int**, not the `size_t` resulting from a raw
`sizeof(array) / sizeof(element)` comparison with a signed loop index.
An interrupted raw-size candidate emitted one extra unlike byte at physical
owner `+0xF0D` and changed the complete direct control-flow graph; restoring
`static_cast<int>(...)` retains the original branch. These are natural C++
integer conversions, not compiler-profile tuning or register hints.

Cold original-bound `--owner enemy` now retains 3,900/3,900 bytes, 3,883 code
bytes and 17 physical alignment/table bytes, 97/97 bound original fields,
69 ordered calls, 170 direct blocks and **13** previously known unlike bytes.
The real 82-byte collision-append callee remains exact. Seven private
natural C++ probes of position-copy and first draw-list address (with
whole-source compiler carriers) reproduce the 13 remaining differences;
none closes the gap or justifies accepting a shorter local fragment.

## Correcting stale source-local label names, not targets

A fresh cold build of the **pre-change HEAD** independently exposed two
already-stale canonical match-unit rows: `ecl-post-update-movement` and
`ecl-compare-operands`. Their VC7 `$L...` compiler-local spelling had drifted
from the manifest, even though their physical original addresses and full
bytes were not wrong. The same compilation in the changed source continued
to expose these two rows. The former strict verifier correctly rejected
names that differed: we did not weaken that verifier or skip either function.

Exactly **20** local `DIR32` relocation symbol names were rebound inside those
two canonical units. For each, the change is **symbol name only**: original
field offset/type, source owner section, same-section definition offset,
addend zero, and independently declared target VA were checked. The original
PE operand encodes the same VA. The 20 target addresses and all other
manifest fields are unchanged. The private checker in
`.analysis/gptweb-enemy-native-limits-20261107/rebind.py` and its
`proof.json` retain the SHA-bound old/new mapping; the published manifest
holds the currently emitted names.

A new pinned-compiler, source-cold `--owner enemy` replay passes **all 91
affected exact units**, totaling **41,868 physical bytes and 1,818 relocation
fields**; this includes both formerly stale entries. The private, separately
reviewed main carrier passed all 77 exact units / 24,368 bytes / 1,096 fields
before promoting the manifest changes. The full Enemy target remains
**nonexact with 13 differences**. RunEcl remains a separate 15,564-byte
nonexact boss (30 differences, opcode 4 and opcode 155), not counted as
original-equal because its sibling functions are exact.

No whole-product link, full cold replay, runtime Oracle, portability,
Factory acceptance or remote publication is inferred from this checkpoint.

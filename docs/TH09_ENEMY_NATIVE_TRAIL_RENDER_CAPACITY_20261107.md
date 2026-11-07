# Enemy original 96-sample/192-vertex trail and draw group capacity (2026-11-07)

Original source of truth: TH09 Japanese 1.50a
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint modifies both the **open 3,900-byte EnemyManager::OnUpdate
boss** and the **already-exact 1,758-byte EnemyManagerDrawImpl big function**,
plus two exact OnDraw leaf wrappers. The names are native source-facing
abstractions, not uniquely recovered original C++ identifiers.

## One physical object behind both implementations

The real `EnemyView` and `EnemyManagerView` provide the same native source
storage to the calc, ECL interpreter, drawing and ANM callback owners:

| Native array | Exact physical size | Derived cardinality |
| --- | --- | ---: |
| `EnemyView::trailSamples33E8[96]` | 96 × 0x1C = 0xA80 bytes | 96 |
| `EnemyView::trailVertices3E68[192]` | 192 × 0x1C = 0x1500 bytes | 192 |
| `EnemyView::secondaryVms2AC[2]` | 2 × 0x2A4 bytes | 2 |
| `EnemyManagerView::drawGroupHeads2AC410[4]` | 4 native pointers | 4 |

The history ends at +0x3E68, exactly where the vertex array begins. Each
sample provides **two textured strip vertices**, and the draw implementation
already increments the vertex pointer twice per sample; this is observed
source/target behavior, not just a convenient numerical coincidence.

`EnemyNativeTrailRenderCapacity.hpp` now derives all four cardinalities from
real native member arrays and pins layout/adjacency, sample/vertex strides,
and the 192:96 ratio with C++ compile-time assertions. The **Enemy OnUpdate**
reset of the last draw group and its secondary VM loop use that type; the
**DrawImpl** strip vertex counts, UV sampling geometry, both two-VM loops,
and the high/low priority draw wrappers consume the same physical limits.
No extra buffers, clamp behavior, gameplay branch or new helper function is
introduced. In particular the descending four-store reset is retained; a
new loop would alter register scheduling without original evidence.

## Source-cold original-bound results and rejected models

The complete open Enemy OnUpdate was separately compiled from real first-
include ECL source under its existing pinned VC7.1 profile, then checked
against every 3,900 original physical byte, 97 original-bound fields,
69 ordered calls and 170 CFG blocks. Its **same 13 ordinary byte differences**
remain; the other 3,887 positions are still equal. An independent
`EnemyManagerDraw.cpp` compilation verified **4 existing exact functions,
1,922 physical bytes and 61 relocations**, including every byte of the
1,758-byte DrawImpl. The isolated controls reproduced the identical result
before any source edit.

Direct experiments also tested 13 natural Enemy register/dataflow variants,
including `memcpy` and three-scalar position copies, type aliases, local draw
index widths, alternative effect manager receivers, and local Timer threshold
views. None improved the **whole** original-bound Enemy owner. Two explicit
Float3 construction variants displaced source literal/field positions and
were rejected. This is finite negative evidence, not a claim that historical
source spelling or a future exact solution is impossible.

A source-cold replay of the **3 directly affected ECL/Draw compiler-profile
objects** passed 87 original-exact units (31,778 physical bytes, 1,450
relocations). Two additional internal VC7 `$L...` symbol spellings changed;
for each, the newly emitted local same-section address and the original
PE table operand independently established an unchanged absolute target,
kind, field offset and addend. A separately reviewed manifest passed the
unchanged strict PE/COFF comparator on all 87 functions before the two
name-only amendments were adopted.

Finally, the standard `--owner enemy` scoped replay recompiled the affected
main ECL carrier and its other required exact dependencies: **91 complete
exact functions, 41,868 original physical bytes, 1,818 relocations**, all
passed. The separately built draw carrier again passed all 4 exact owners
(1,922 bytes / 61 fields). These are bounded original codegen checks, **not**
a full-project cold replay or new canonical exact credit. Global totals
remain 1,034 source-present / 1,021 exact / 13 nonexact. Product build,
runtime, semantic, port, remote-push and Factory acceptance gates remain open.

Private evidence and exact source/object hashes:
`.analysis/gptweb-enemy-trail-geometry-20261107/` and
`.analysis/gptweb-enemy-residual-natural-20261107/`. Original TH09 target
bytes and independently reviewed target referents were never modified.

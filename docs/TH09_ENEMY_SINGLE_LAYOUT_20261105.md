# Native Enemy layout consolidation, 2026-11-05

The bounded continuation starts at clean `0fd33fc0`. Original target,
tracking and status checks pass. Fresh Ghidra exports attest the original
Japanese disk/Program SHA, i386, base/entry and all 685056 file-backed mapped
bytes for Enemy update, RunEcl, both post-ECL callbacks and mass-clear.

The two post-ECL callbacks now access the shared `EnemyView`. The duplicate
0x5430-byte `EnemyCoreView` layout is removed. Its opaque tag remains only at
three independently bound helper interfaces; their actual definitions and
private calling contracts are preserved. ECL state layout assertions now use
native fields, and mass-clear checks native absolute offsets. Float3 arithmetic
bridges the existing POD coordinate members as other Enemy owners already do.
The direction reader preserves its raw-byte interpretation and 0xFF sentinel
through unsigned conversion of the native signed-byte member.

Both ECL profiles and the three other affected source carriers are rebuilt
serially with their existing pinned VC7.1 profiles. All **85 canonical units**
pass complete original byte/relocation comparison: **27847 physical bytes and
1252 fields**. This includes the 992-byte movement callback with its NOP and
six-entry table, and the 349-byte shot/animation callback. The 20 private label
spellings changed in two functions. Every actual same-section definition,
relative offset and original PE operand is verified before changing only the
manifest spelling; no target or field destination changes.

Full current-carrier guards also pass. Enemy remains3900/97/13 and RunEcl
15564/598/30. Effect29 remains2205/2220 with129 fields and1170 overlapping
unlike bytes; MoveRandomBiased remains517/516 with148 unlike overlap bytes.
The other exact and diagnostic owners in the same TU are preserved. These
are unchanged frontiers, not new exactness credit or product/runtime proof.

The focused 82 tests pass. Obsolete source-text assertions that demanded the
deleted duplicate layout are removed; native field/owner checks and complete
byte/relocation guards remain. Tracking, generated progress, documentation and
whitespace checks pass. Totals remain1019 exact/16 nonexact.

Rebuild the affected carriers with `build-match-unit.py --unit` using
`enemy-attached-effect-update`, `player-position-callback-type11`,
`ecl-interpolate-linear`, `ecl-initialize-subroutine` and
`enemy-manager-kill-all-non-boss`, then compare their canonical units. No full
cold replay is needed.

Ignored `.analysis/enemy-native-20261105/` retains frozen inputs/hashes,
source controls, attested Ghidra exports, label-definition proof, all85 exact
receipts, complete owner diagnostics, direction-byte evidence and test logs.
`probe.py` and `remove-core.py` reproduce the source controls; `verify.py`
reuses the freshly built carrier for all its remaining diagnostic owners.
Generated experiment objects remain below `build/analysis/enemy-native-20261105/`.

A subsequent per-case cleanup removes only SHA-identical input copies, with
path/SHA/recovery receipts in `duplicate-cleanup.json`. Frozen inputs and
changed source controls are retained, and the probe recreates the copies.

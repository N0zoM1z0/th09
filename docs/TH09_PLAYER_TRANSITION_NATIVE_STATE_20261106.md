# TH09 Player transition / effect native storage (2026-11-06)

The original Player update and transition callbacks use **one Player allocation**.
Three of its source-visible words were previously reconstructed with opaque
`player + 0x...` expressions. Direct original-PE and complete VC7.1 owner
comparison now support naming these fields inside the existing actual
`PlayerLifecycleView`, not creating a second mirror object:

| Absolute offset | Native source ownership | Observed original use |
| --- | --- | --- |
| `+0x3032C` | `state3031C.stateWord10` (+0x10) | Set to zero during transition; detailed role remains unknown |
| `+0x30334` | `state3031C.transitionCounter18` (+0x18) | Initialize to 60 and decrement when nonzero during common update |
| `+0x303E0` | `stateEffect303E0` | Active Effect pointer, updated and released by common state transitions |

The previously opaque `PlayerState3031CView` still occupies exactly 0x1C
bytes, retains its original constructor, and leaves unreviewed words opaque.
The Player +0x303E0 region retains its original full extent through the next
known field at +0x303F0. Compile-time `sizeof` and `offsetof` assertions
independently pin both subobject offsets and all three absolute addresses.
The native `Effect` pointer type is consistent with the existing exact
Effect owner and the actual target call/reference sequence.

`PlayerTransitionSpecial` and `PlayerUpdateCommon` now use ordinary typed C++
member assignment/reference expressions for these words; `PlayerStateEffect`
returns a genuine `Effect*&` lvalue rather than a byte-offset cast. Timer and
color branches, pointer publication/clear, and the original call sequence
remain intact. No forced register, compiler-profile roulette, dummy helper,
new allocation or executable patch is involved. Original retail class names
and the meaning of the neutral `stateWord10` are not asserted.

## Original-bound focused multi-owner proof

Against Japanese TH09 1.50a SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`,
the current **41 distinct, selected, originally exact function units** from
Player, Player selector, Player added-state, Player reward, Etama and ExAttack
reproduce **18,121 complete physical bytes / 846 actual COFF fields**. This
includes original-bound 522-byte Player OnUpdate, 320-byte common state update,
293-byte transition, 852-byte physical UpdateBeforeState, 572-byte physical
Player InitializeAddedState, 1,540-byte physical Player owner reward,
3,238-byte selector, 412-byte resolver, 2,216-byte Etama OnUpdate,
1,932-byte SpawnSingleBullet and 824-byte ExAttack Type14/22 update.
Each changed carrier was built under its existing fixed VC7.1 compiler profile;
no whole-project cold replay was run.

The shared header's more precise native type declarations renumbered
compiler-private `$L` identifiers in **seven** exact owners across six source
files. For each changed local label, the actual COFF section/value and
owner-relative coordinate were independently checked against its **unchanged
original target VA**. Complete linked PE bytes, field offsets/types/addends,
external symbols, tables, call sites and physical extents were checked
**before** refreshing only the local label spellings in the manifest.
No target destination was fitted or changed. Other source files not in this
bounded cohort are not represented as exhaustively replayed.

Separately, the still-nonexact Enemy OnUpdate maintains the **3,900-byte /
97-field / 13-difference** original-bound frontier; Player ChargeAttack
maintains its **1,210-byte original / 1,197-byte candidate / 70-field**
frontier, with the same 13 missing bytes and no new exactness credit. These
diagnostics are not included among the 41 exact units above.

Reproducibility packets and original-location audits are retained locally at
`.analysis/gpt-web-player-native-state-20261106/`. The authoritative
source units and actual original-linking recipes are in
`config/match-units.toml` and are checked through
`scripts/build-match-unit.py` and `scripts/compare-coff-function.py`.
This checkpoint changes C++ source/data ownership; the project's authored
1021/1034 exact count remains unchanged, as do later product/runtime gates.

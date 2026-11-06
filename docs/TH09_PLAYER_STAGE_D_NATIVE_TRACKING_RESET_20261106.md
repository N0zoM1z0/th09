# Player stage D: native tracked-position and reset storage (2026-11-06)

The Japanese TH09 v1.50a `PlayerUpdateStageD` routine resets two 12-byte
positions and an adjacent integer word each Player update. Previous C++
reconstructed those as independent absolute `Player* + 0x30364`, `+0x30370`
and `+0x30380` casts. These are actually three windows of the **existing
Player +0x30364 tracking storage**, shared with the exact Enemy and shot
homing/nearest-position consumers.

The source now starts from the real Player `unknown30364` member:

- Its `PlayerEnemyTrackingStateView::trackedEnemyPosition00` at relative
  `+0x00` is initialized using the **original** `Float3(-999,-999,0)`
  constructor and assignment sequence.
- The second observed 12-byte vector starts at relative `+0x0C` and retains
  the **same** original Float3 construction and write, without claiming its
  unreviewed gameplay meaning.
- The dword reset at relative `+0x1C` is the original `Player +0x30380`
  write. Its semantics are kept neutral.

A compile-time assertion checks the actual Player storage offset/extent and
the referenced tracking-view fields. Crucially, the Player constructor still
owns the original opaque bytes: this does **not** install a second
nontrivially constructed embedded tracking object, change allocations,
use register forcing or introduce an artificial helper.

## Complete current-source owner checks

Several ordinary C++ candidate expressions were actually compiled. Direct
member-relative offsets and their combination retained the exact 121-byte
stage-D leaf and its exact 522-byte OnUpdate caller. Constructing one
additional borrowed tracking-reference local emitted **122 rather than 121
bytes** and was rejected.

The accepted source was rebuilt under all five existing VC7.1 compiler
profiles of `src/PlayerRuntime.cpp`. All **21 original-bound complete exact
owners** pass: **5,391 physical bytes and 288 relocation fields**. Only
11 compiler-private `$L` names changed in the 852-byte physical
`UpdateBeforeState` sibling. Before updating those names, each current COFF
section/value was resolved independently to the *unchanged* original target
VA, and the entire physically linked owner was compared against the
SHA-attested original. No symbol retargeting or incomplete comparison occurred.

Reproduce locally (affected source owners only):

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/build-match-unit.py --unit player-on-update
python3 -B scripts/compare-coff-function.py --unit player-update-stage-d
python3 -B scripts/compare-coff-function.py --unit player-on-update
python3 -B scripts/compare-coff-function.py --unit player-update-before-state
```

Japanese original SHA-256:
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The saved ignored trial context is
`.analysis/gpt-web-player-stage-d-20261106/`. This is source-layout
progress in already-exact owners, not new credited bytes, a whole-project
cold replay, native product closure, remote publication or Factory acceptance.

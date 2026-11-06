# Player OnUpdate: native opponent-header metric receiver (2026-11-06)

When the original Japanese TH09 v1.50a Player OnUpdate transfers timing data
to the opposing side, it calls the independently reviewed `GetValue` leaf
with a receiver at **opponent Player +0x80**. The maintained source had
previously derived that receiver from a generic GameManager side slot with
`reinterpret_cast<unsigned char *>(opponentState->manager04) + 0x80`, even
though the same slot has a confirmed native `PlayerLifecycleView *player04`
alias and the real Player embeds `header24` at `+0x24`.

The reconstructed source now obtains the actual opposing Player from
`opponentState->player04`, then passes a pointer to
`&opponent->header24.unknown00[0x5C]` to the existing
`PlayerMetricRuntimeView::GetValue` receiver. This is the *same* original
Player +0x80 address. The header's inner byte range remains anonymous:
this source change does **not** guess the metric's nominal class, invent
an additional object, or rebind the original method/call target.
A compile-time assertion checks the Player header offset, available native
header storage and +0x5C / +0x80 geometry.

Four genuine C++ source models were compiled: member-index address,
member-array pointer, header-relative byte pointer and a local native Player
pointer. All retained the exact **522-byte OnUpdate**. The adopted ordinary
native Player local/pointer form also preserves the other owner codegen;
compiler-private label churn is limited to the already-reviewed same-profile
`UpdateBeforeState` sibling. Eleven `$L` definitions were checked against
the actual COFF section/value and **unchanged original destinations** before
updating their names in the canonical match recipe. No external callee
identity, operand location, field kind or original target was changed.

All **21 complete `PlayerRuntime.cpp` original-bound units** in their five
actual VC7.1 compiler profiles pass: **5,391 physical bytes and 288 fields**.
In addition to OnUpdate, the complete 852-byte `UpdateBeforeState` owner,
121-byte stage-D leaf, 243-byte Player constructor and the relevant
collision/bomb updates remain exact. The source was not recompiled with a
noncanonical profile or padded/forced to fit a target byte sequence.

Focused reproduction:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/build-match-unit.py --unit player-on-update
python3 -B scripts/compare-coff-function.py --unit player-on-update
python3 -B scripts/compare-coff-function.py --unit player-update-before-state
```

Original target SHA-256:
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Private failed-model/compiled results are in
`.analysis/gpt-web-player-metric-native-20261106/`. These owners were
already exact. Source quality improved but no new exactness credit, whole
cold replay, Factory/remote acceptance or native runtime claim follows.

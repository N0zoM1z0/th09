# ExAttack Type4 update: real Player and ANM VM owners — 2026-11-07

Verified original Japanese TH09 v1.50a executable, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Source-facing C++ names are reconstructed views and not a declaration of
unique historic original class or translation-unit boundaries.

## Recover both native objects inside the 739-byte boss

The complete original-exact `ExAttackUpdateCallbackType4` at **0x00442BD0**
uses a Player query at Player +0x36C and five consecutive animation VMs of
native **0x2A4 bytes**. The VMs' second position vector is at +0x288 and
their `SetInterrupt` method is the independent **18-byte original-exact
`AnmVm::SetInterrupt` at 0x00406790**. The ExAttack callback calls that
same method five times when exiting its active animation state.

The previous code still declared two private classes with partial opaque
storage: `ExAttackType4PlayerView` and `ExAttackType4VmView`. Those duplicate
physical layouts and decorated method identities were not supported by
separate original allocations. The Type4 source now directly uses the real
`PlayerLifecycleView *` in its side record and `AnmVm *` in its dynamic VM
record. Compile-time assertions pin the same physical Player query, VM size,
and VM position offset. The previously recovered shared
`PlayerCollisionQueryStateView::AppendCircleRecord` remains the five-call
original method at **0x00440D30**. All Player and ANM calls retain their
original receiver offsets, argument values, branches and chronology.

## Whole-owner PE/COFF check, not a local similarity score

Four natural source models were compiled independently with the fixed VC7.1
Type4 profile: maintained source, native Player alone, native VM alone,
and both together. Native Player alone reproduced **739/739** original
bytes with the existing manifest. Native VM substitution correctly changed
five COFF symbol names but retained all source-cold instruction bytes.
Before updating the manifest, a separate audit confirmed each of the five
new `AnmVm::SetInterrupt` symbol references at original field offsets
+0x296/+0x2A6/+0x2B6/+0x2C6/+0x2D6, with original target 0x00406790,
REL32 kind, addend 0. Every one of the **28 original relocation fields**
was checked against the immutable original PE operand, and all **739
physical bytes** matched after binding. No other relocation or field was
changed, and the real `AnmVm::SetInterrupt` has its own independent exact
unit. Those five VM method names are reviewed separately from five additional
Type4 `g_GameManager` source-symbol changes, which are audited with the
other five native ExAttack runtime owners in the
[native GameManager/Player review](TH09_EXATTACK_NATIVE_GAMEMANAGER_PLAYER_20261107.md).

Focused source-cold replay of Type4 and the actual two pinned compiler
profiles of its ANM provider passes **eight complete original-exact
functions / 1,246 physical bytes / 52 actual relocation fields**.
Evidence and independently reproducible probe/audit scripts:
`.analysis/gpt-web-exattack4-native-player-20261107/`, including
`native-vm-original-proof.json` and `final-exact.log`.

This is substantive improvement to original-ABI fidelity of a large exact
callback, not a new exact function or proof of whole-product Windows linkage,
TH09 gameplay behavior, Factory acceptance, remote publication or portability.
The existing ledger still reports 1,034 source-present / 1,021 exact /
13 nonexact; no full cold replay was run.

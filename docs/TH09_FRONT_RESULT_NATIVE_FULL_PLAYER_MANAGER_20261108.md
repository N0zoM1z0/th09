# TH09 FrontResult: original-exact 553-byte full Player/GameManager receiver

Original target: Japanese TH09 1.50a executable with SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint recovers ordinary C++ object/field ownership and the
complete original result-score source function, not unique historic names,
new exact-match credit or gameplay/product runtime acceptance.

## No third GameManager, side or Player object in the result score

The maintained exact `FrontResultBonusView::InitializeResultBonus` at
**0x00415E50 (553 bytes/27 PE-bound fields)** previously declared a
separate, incomplete `FrontResultGameManagerLayout`, two prefix-only
`FrontResultSideView` records and a `FrontResultPlayerView` with almost
all Player bytes hidden as padding. The real result screen already has
one `AsciiGameManagerView g_GameManager` with its two native 0x38-byte
sides, a real full `PlayerLifecycleView` at side +0x04 and a real
0xA0-byte side runtime at side +0x1C. No shadow class owns any storage.

The complete result calculation now reads real Player and GameManager
members directly. Specifically the four integer counters at original
**Player +0xA8/+0xB0/+0xB4/+0xB8** are fields
`header24.state84/state8C/state90/state94` in the genuine Player header.
The original result bonus comparison at **Player +0x30418** reads
`ownerState30410.maxState08` (the same +0x08 owner-state word used by
the independently exact 1,540-byte ApplyReward). This 4-byte member is
now explicitly typed in the native 0x44-byte Player owner-state storage
without changing constructor behavior or its overall extent.

The result's side runtime float at +0x00, phase word at +0x10, side
shot-type at +0x20, GameManager mode at +0xFC, mode checks and final
`AsciiGameManagerSideView::AddScore @0x0040F860` are all native members
already reviewed elsewhere. The source deliberately preserves the
original *two distinct reads* of Player +0x30418: compare through the
current local Player pointer, then reload through the GameManager side
pointer before multiplication. An earlier inferred compiler common-
subexpression plateau was corrected by this very register-lifetime
behavior; no volatile, inline assembly, padding, register forcing or
unreviewed skip path is introduced.

## Natural compiler and complete original PE/COFF closure

A copy-on-write pinned VC7.1 trial removed all three temporary
FrontResult object views and instead used real Player and GameManager
C++ fields. The compiler emitted **all 553 function machine bytes and
all 27 COFF relocations unchanged**. The adopted source passes strict
whole-original PE comparison with every call address and addend intact.

The shared full Player header's visibility renumbered **two ECL,
17 PlayerRuntime/AddedState, and six PlayerReward compiler-private
`$L` labels**. Each changed COFF identity was proved against its
real same-section definition/owner-relative address and the unchanged
original PE operand before rebinding *only the label spelling*.
Targeted original-bound source-cold replay of FrontResultBonus,
PlayerRuntime, PlayerAddedState, EclManager, PlayerCollisionHelpers,
PlayerOwnerStateReward, Player State3 and shot-callback carriers passes
**119 existing complete exact functions / 39,978 physical bytes /
1,891 relocation fields**.

Separately nonexact large functions must not be promoted by this source
improvement. `EnemyManagerView::OnUpdate` remains 3,900 target/candidate
bytes with 97 original fields, 69 calls and 13 unlike ordinary bytes;
`PlayerLifecycleView::CheckBulletCollision` remains 862 original /
849 candidate bytes, 26 bound fields, 12 calls and 13 missing bytes.
Source-cold and full-target proof packets are kept locally in
`.analysis/gpt-web-native-front-result-player-20261108/`.

No full-project cold replay, linked Windows game product, runtime
acceptance, Factory exactness receipt or remote publication is claimed.
Global count remains **1,034 source-present / 1,021 exact / 13 nonexact**.

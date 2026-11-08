# TH09 full Player reward state machine uses real GameManager and Supervisor

Original target: Japanese TH09 1.50a, SHA-256 verified as
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint restores ordinary C++ object ownership, without claiming
unique historic type spelling, new source-present exact credit or runtime
acceptance of the complete Windows game.

## One original reward state machine and the true GameManager/Supervisor

The original **`PlayerOwnerStateView::ApplyReward` at 0x0041D150** is a
complete **1,519-byte function, 1,540 physical bytes including the
original switch table, with 91 original-bound COFF relocation fields and
33 ordered calls**. It performs resource-threshold handling, native
score-popup coordinate translation, RNG-driven reward-effect spawning,
side selection and meter/time updates.

Previously its source maintained two separate, incomplete globals:
`PlayerRewardSupervisorView g_PlayerSupervisorRuntime` and
`PlayerRewardGameManagerView g_PlayerGameManagerRuntime`. Their observed
addresses were actually the genuine native `g_Supervisor` at
**0x004B3100** and native `g_GameManager` at **0x004A7D90**. The source
now includes `Supervisor.hpp` and `GameManagerRuntimeView.hpp`, directly
calls the real `Supervisor::SelectSide` **0x00401440** four times, and
uses the real GameManager `TransformPopupX` **0x00401680** and
`TransformPopupY` **0x004016B0** twice each. Neither shadow class,
extra 0x7B0-byte Supervisor, alternate GameManager allocation nor
invented virtual method remains. Original effect, attack and score flow
has not been reordered or modified.

## Full original PE/COFF proof across 16 external source identities

Independent natural fixed-VC7.1 models tried *just* Supervisor migration,
*just* GameManager migration, and both together. All three emitted the
identical **1,540 complete physical bytes** and **91 COFF relocation
positions/types/addends**. The final source was then independently
bound to the immutable original PE: all 16 affected external field names
retain their original global addresses and the same precise call targets.
The compiler renamed six `$L` jump labels as a consequence of the native
headers; their real same-section COFF definitions, owner-relative
positions and immutable PE jump-table operands were proved individually
before updating only the six manifest names.

The full canonical comparator proves **1,519/1,519 function bytes,
1,540/1,540 physical code/table bytes and all 91 fields** are exact.
The separate full reward interpreter additionally verifies the **33
ordered original calls** and complete post-return switch table. Scoped
source-cold replay with both independent real `GameManagerCoordinates`
and `SupervisorViewport` providers passes **13 existing exact functions,
3,254 physical bytes and 205 relocation fields**. No new complete exact
function is credited.

Original binary validation, source trials and label audit are retained in
`.analysis/gpt-web-player-owner-native-manager-20261108/`.
The independently open **862-byte `CheckBulletCollision`**, 1,900-byte
Player movement and 1,210-byte ChargeAttack bosses remain nonexact; this
checkpoint does not claim partial match credit for them.
There was no complete cold replay, Windows product link, runtime semantic
acceptance, Factory receipt or remote publication. Global ledger remains
**1,034 source-present / 1,021 exact / 13 nonexact**.

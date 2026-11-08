# TH09 exact 309-byte shot initialization borrows real Player, Shot and SHT

Target: independently SHA-256 verified Japanese TH09 v1.50a executable
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint recovers natural C++ object and original field ownership
within an already-exact function; it does not claim newly earned exact
source credit, whole-game linking or runtime acceptance.

## An exact 309-byte source function with three previously separate overlays

`PlayerShotInitPlayerView::InitializeShot` at original **0x0041F350** has
309 machine bytes and **seven original relocation fields**. Its maintained
source previously read three independently defined prefix views, even
though the real allocations have already been independently recovered:

- The **0x30F70-byte `PlayerLifecycleView`** owns position +0x1B88,
  four native 0x2F4-byte options starting at +0x1CEC and the actual
  `PlayerHeaderStateView::anmFile98` at absolute Player +0xBC.
- The **0x484-byte `PlayerShotCtorView`** owns shot position +0x2A4,
  32 constructed vector trail samples +0x2B0, hitbox +0x430, velocity
  +0x43C, timer +0x454, damage +0x460, type +0x464, animation +0x46C,
  tint +0x46E and the original +0x470 short word. The previously opaque
  native +0x460/+0x470 fields are now `damage460` and `field470`.
- The **0x38-byte `PlayerShtDescriptorView`** already used by SHT loading
  and Type1 spawn owns position floats +0x04/+0x08, hitbox floats
  +0x0C/+0x10, angle +0x14, speed +0x18, damage +0x1C, option index
  +0x20, shot type +0x22, animation index +0x24 and sound index +0x26.

The exact initializer implementation now borrows these three genuine
native C++ objects, preserving the original center/option position copy,
cos/sin velocity, collision hitbox initialization, timer and shot state
writes, positional sound and ANM execution. It does not add any
constructed option, view instance or altered script/material resource.
The Float3 conversion on the underlying native shot position is a
non-owning **three-float view** retaining the independently exact original
`Float3::operator float*` call at 0x004343D0. The member-backed native
`PlayerLifecycleView::position1B88` first-arm assignment uses the
actual matching PlayerPositionView type.

## Natural fixed-compiler trials and original PE/COFF protection

Six isolated pinned VC7.1 natural C++ compile models—native field
layout alone, native descriptor, native Player, native Shot, Player plus
descriptor, and Shot plus descriptor—each reproduced **309/309 physical
bytes and all seven original relocation offsets/types/addends**. The
adopted triple-native model also passes the complete canonical original
PE comparator with the same seven original targets: Float3 pointer
0x4343D0, cosf/sinf 0x401060/0x401070, ZunTimer int assignment
0x401500, SoundPlayer global 0x4DC698 and method 0x43E380, and
AnmLoaded SetAndExecuteScriptIdx 0x403E00.

Scoped original-bound VC7.1 replay of PlayerInitializeShot, PlayerRuntime,
PlayerAddedState, PlayerOwnerStateReward, Type1 shot spawn and ECL
sibling owners passes **111 complete previously exact functions,
37,912 physical bytes and 1,807 relocation fields**. The native header
renumbered two ECL, seventeen PlayerRuntime/AddedState and six reward
compiler-private `$L` names; every replacement was checked against its
real COFF same-section definition and immutable PE operand target before
updating only the original-bound manifest name.

The **1,900-byte Player Movement** remains 122 ordinary bytes unlike
with 66 original fields/23 calls; Enemy OnUpdate remains 3,900/3,900
with 13 unlike; Player CheckBulletCollision remains 862/849 with 13
missing; RunEcl remains 30 unlike bytes in 15,564 physical bytes.
At the time of this original field-recovery checkpoint the public
method still used legacy `PlayerShotInit*View` parameter types. That
remaining step was **subsequently completed** in `e861abb0`:
`PlayerLifecycleView::InitializeShot` now owns the real method and takes
native PlayerShotCtorView/SHT parameter types. All seven exact caller
objects were original PE/COFF-verified to retain the same 0x41F350 target
(8 complete owners / 981 physical bytes / 21 fields). See the
[current November 8 handoff](TH09_NEXT_AGENT_HANDOFF_20261108.md).

Local bounded compiler evidence and strict source-cold proof:
`.analysis/gpt-web-player-shot-native-init-20261108/`.
No full-project cold replay, native Windows game product, Factory receipt
or remote push is claimed. Current canonical ledger remains
**1,037 source-present / 1,024 exact / 13 nonexact**.

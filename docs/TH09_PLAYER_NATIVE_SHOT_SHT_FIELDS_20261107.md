# Recover native Player shot tail and actual SHT descriptor across SpawnShots

Original-bound TH09 1.50a Japanese PE SHA-256:
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint reconstructs physical C++ layout and data flow; it grants
no new accepted complete matching function, semantic gate, or game product.

## Actual native storage used by the 190-byte SpawnShots producer

The original Player allocation's `shots[128]` records are 0x484 bytes
apart, starting at Player +0xC11C. The separately original-backed
`PlayerShotCtorView` already supplied its 0x2A4 ANM VM and real +0x462
state, +0x474 update callback and +0x478 draw callback. The last two
previously opaque pointer words have now been restored as native
`collisionCallback47C` and `PlayerShtDescriptorView *descriptor480`.
Compile-time checks preserve the actual +0x47C/+0x480 offsets and 0x484
stride. This uses the same original shot array processed in PlayerRuntime's
UpdateShots and draw callbacks rather than overlaying independent shadow
record objects.

The source now obtains the real `PlayerShtDescriptorView *` through
`primaryShtFile->shotPowerLevels[powerLevel].descriptors`, matching the
independently exact SHT loader's +0x42C power-level table. Its +0x28
callback word is still an original encoded void pointer in the native
loader; `SpawnShots` narrows it to a native function-pointer type at the
actual call site. The source now checks the real `fireInterval`, invokes
`SpawnShotWhenFrameMatches` on the existing original callback path,
sets `shot->vm.flagsWord` at +0x1F8, and publishes the exact four callback
and descriptor words after successful spawn. No hidden record was added,
all callback argument/return chronology remains unchanged, and the loop
capacity still derives from the real `shots[128]` array.

## Fixed compiler original-bound proof

A copy-on-write natural C++ candidate was independently compiled using
the established pinned VC7.1 `player-spawn-shots` profile. **All 190 raw
bytes were identical** to the previous source, with the same single COFF
relocation at +0x52. The adopted implementation also passes the strict
original PE comparator with the same source symbol and target 0x0041F4C0.

The header's larger shared native types were propagated through related
PlayerRuntime and Effect Type35 source carriers. Only the changed/affected
profiles were source-cold rebuilt. **24 complete existing exact functions,
5,691 physical bytes and 292 relocation fields** passed. The widened
header caused exactly **17 compiler-private `$L` name shifts** in two
PlayerRuntime/AddedState physical owners; actual same-owner local label
offsets and all old immutable original PE targets were checked before
updating *only* the corresponding names in the strict manifest. The
1,210-byte original ChargeAttack still has its independent **1,197-byte
candidate / 13 missing bytes**, so no exactness is claimed for that boss.

Private original-bound source probes and receipts:
`.analysis/gpt-web-native-shot-wire-20261107/`, including the natural
compiler experiment and `player-label-proof.json`.

Global ledger remains 1,034 source-present / 1,021 canonical exact /
13 nonexact. No all-function cold replay, Windows executable link, runtime
acceptance, remote push, or Factory exactness receipt is claimed.

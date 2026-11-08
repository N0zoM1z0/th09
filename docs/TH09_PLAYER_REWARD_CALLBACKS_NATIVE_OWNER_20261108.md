# TH09: all 16 original-exact reward callbacks use the native Player owner state

Original Japanese TH09 v1.50a executable: SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint reconstructs ordinary C++ receiver/storage ownership,
not unique historic names, product link/runtime acceptance or new
byte-exact source-present coverage.

The original `PlayerOwnerRewardCallbackType0` through `Type15`
fastcall methods are independently exact ExAttack producers called
through the genuine `PlayerOwnerStateView` callback at native state +0x40.
Despite receiving the *actual 0x44-byte PlayerOwnerState* in ECX, their
maintained source defined a separate `PlayerOwnerRewardCallbackStateView`
containing opaque bytes before the reward counter. All sixteen methods
cast the pointer to that fake class; Type7 additionally updated its
separate +0x30 word.

The callback code now directly borrows the original `PlayerOwnerStateView`
from its existing fastcall parameter. Its true owner pointer at +0x00
points to `PlayerLifecycleView`, the reward accumulator `value34` is
at +0x34, and Type7's original bounded counter is exactly the
`spawnCounter30` at +0x30 also operated on by full ApplyReward. No extra
state array, conversion, allocation, callback table entry or new call
sequence is introduced. Each of the sixteen attack-type, rate, timer,
source-position and enemy-side values is preserved verbatim.

Copy-on-write pinned original VC7.1 compiler experiments tried a real
PlayerOwnerState pointer, a const native pointer variable and a native
reference for *every* callback, rather than just one example. All three
natural C++ models preserved **16/16 complete original functions,
1,604/1,604 physical bytes and 79/79 relocation fields**, without any
field-name rebinding or altered call target. The adopted source-cold
original PE/COFF closure covering these 16 callback owners, the
1,540-byte exact reward boss and exact PlayerRuntime callers passes
**38 complete exact functions / 8,535 physical bytes / 458 fields**.

The separately nonexact 862-byte Player CheckBulletCollision retains
its 849-byte candidate, 26 reviewed original fields, 12 calls,
799 overlapping differences and 13 missing bytes. No nonexact function
is promoted, and no full-project cold replay was performed. Bounded
original-profile trials and strict receipts are kept under
`.analysis/gpt-web-native-reward-callbacks-20261108/`.

No Windows game link, gameplay acceptance, Factory receipt or remote push
is claimed. Global ledger is unchanged at **1,034 source-present /
1,021 exact / 13 nonexact**.

# TH09 Type1 shot spawn uses the real SHT-loader-owned descriptor

The original 151-byte `PlayerShotSpawnCallbackType1` at **0x00441EF0**
now receives the genuine 0x38-byte `PlayerShtDescriptorView *`, not a
separate `PlayerShotSpawn1DescriptorView` prefix. The SHT file loader
already produces this native descriptor and the independently exact
shot initializer reads the same original offsets.

Native `PlayerShtDescriptorView` now exposes the original firing interval
at +0x00, two position-offset floats at +0x04/+0x08 and the short option
index at +0x20. Unknown intervening bytes are still explicitly opaque.
These fields share exactly the same physical allocation as the existing
angle/speed +0x14/+0x18 and spawn/update/draw/collision callback pointers
at +0x28/+0x2C/+0x30/+0x34. No second descriptor is allocated.

Two bounded fixed-VC7.1 natural C++ models—direct member access and a
named native descriptor reference—both compiled to **all 151 original
bytes and both relocation fields**, without changed call operands.
The adopted source uses the original `PlayerLifecycleView *`,
`PlayerShotCtorView *`, and `PlayerShtDescriptorView *` together with
the genuine `PlayerState3031CView` timer and shot pointer. The callback's
original exact timer assignment and shot initialization retain targets
**0x00401500** and **0x0041F350**.

An original PE/COFF source-cold replay across EclManager, PlayerRuntime,
PlayerAddedState, PlayerOwnerStateReward, FrontResult and Type1 callback
passes **111 already-exact complete functions / 38,156 physical bytes /
1,827 original relocation fields**. Compiler-private ECL (2), Player
Runtime/AddedState (17) and reward (6) name changes were individually
verified against same-section COFF definitions and immutable PE addresses
before rebinding names only.

The separately nonexact **1,900-byte Player Movement** retains its
1,900-byte candidate, 66 bound fields, 23 calls and 122 unlike bytes;
**Enemy OnUpdate** remains 13 bytes unlike, Player CheckBulletCollision
remains 862/849 with 13 missing bytes, and RunEcl retains 30 unlike
opcode bytes. These structural changes are **not newly credited exact
functions**. The original target is Japanese TH09 v1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Focused natural trials and original evidence are retained at
`.analysis/gpt-web-type1-native-sht-descriptor-20261108/`.
No full cold replay, linked Windows game executable, gameplay acceptance
or Factory receipt is claimed.

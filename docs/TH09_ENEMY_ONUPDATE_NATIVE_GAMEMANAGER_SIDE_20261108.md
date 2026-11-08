# TH09 Enemy OnUpdate: genuine GameManager side for special bullet spawns

Target: SHA-256-verified Japanese TH09 v1.50a i386 PE
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint restores ordinary C++ object/receiver ownership inside a
still-*nonexact* **3,900-byte boss**, without claiming a new byte-exact
function, game link or runtime acceptance.

## The native 0x38-byte GameManager side is the one Etama producer

`EnemyManagerView::OnUpdate` at **0x00410730** already has the actual
`EnemyManagerView::sideState320` pointer at +0x320. Its special-attack
branch selects a bullet count from side +0x20 and invokes the real
`EtamaController` at side +0x08 with the original 0x214-byte
`BulletSpawnDescriptor`. Maintained source previously borrowed a
separate `EnemySideStateView &spawnSide`, although GameManager setup,
ECL/Etama and native Player have independently identified this exact
physical side allocation as the **0x38-byte `AsciiGameManagerSideView`**.

That branch now uses
`AsciiGameManagerSideView &spawnSide =
*reinterpret_cast<AsciiGameManagerSideView *>(manager->sideState320)`.
Its actual `character20` (+0x20) and `etama08` (+0x08) members name
the genuine GameManager side fields. Complete compile-time assertions
cross-check both 0x38-byte layouts, both Etama/character offsets and
EnemyManager side pointer +0x320. No extra side instance, constructor,
modified Etama ABI or register-forcing hint was introduced.

## Challenge older assumptions about the thirteen remaining bytes

Ten bounded natural VC7.1 variants explored the native side reference,
side pointer/direct expression, alternate lifetime before position copy,
side-character local, threshold local, position pointer/byte/scalar copies
and draw-list index expression. Seven passed the full original-bound
Enemy inspector but retained the same 13 unlike original bytes. Three
others failed its *unchanged* relocation/scalar/physical checks and
were not adopted. No variant eliminated these differences; that is not
proof no natural source expression can improve the frontier.

With the native side adopted, the complete original-byte comparison
still has **3,900 target/candidate physical bytes**, **97 fully bound
original relocation fields**, **69 ordered calls**, identical 170-block
CFG and the same thirteen ordinary offsets:
`0x19C, 0x1A3, 0x252, 0x263, 0x26D, 0x272, 0x294, 0x29A,
0x29C, 0x2A5, 0x2A9, 0x2B7, 0x2BD`.
Disassembly locates the first two in an EAX versus ECX draw-list index
and the others in side/threshold register transport and the 3-float
position copy inside the descriptor construction block.

Targeted complete source-cold replay of the affected EclManager.cpp
carrier passes **83 previously exact full functions / 29,856 physical
bytes / 1,389 fields**. Two compiler-private ECL label spellings that
changed under the authentic source recompilation were independently
checked against same-section COFF symbol definitions and immutable PE
operand targets before rebinding only their names. The canonical
3,900-byte Enemy boss remains nonexact: no partial bytes are credited.

Private natural-source trials, complete original PE/COFF audits and
baseline boss reports are retained at
`.analysis/gpt-web-enemy-native-side-nov8/`.
No full cold replay, Windows product link, Factory receipt, gameplay
acceptance or remote Git push is asserted.

Supplementary independent evidence: [native GameManager Etama side whole-boss audit](TH09_ENEMY_BOSS_NATIVE_GAMEMANAGER_SPAWN_SIDE_20261108.md).

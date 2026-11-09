# Player runtime uses constructed Front transition fields (2026-11-09)

The Japanese TH09 v1.50a `PlayerRuntime.cpp` includes multiple physical
reads of the Front allocation's +0x1095C transition count and +0x10960
transition frame. Earlier source retained a partial source-only
`PlayerSharedRuntimeView` projection plus an unrelated
`g_PlayerSharedRuntime` declaration. The source now uses the authentic
`g_GameManager.frontFactoryA8` pointer to the constructor-verified
0x1293C-byte `FrontFactoryConstructView`, reading the real
`transitionVmCount1095C` and `transitionFrame10960` members.

This changes the real source of five original absolute DIR32 references,
not their physical destinations. Every affected original PE operand is
0x4A7E38, the value of GameManager root **0x4A7D90 +0xA8**. The changed
caller references are:

- `PlayerLifecycleView::OnUpdate` (522 bytes): +0x1AD, +0x1CD.
- `UpdatePlayerBombState` (335 bytes): +0x01.
- `PlayerLifecycleView::OnDrawHighPrio` (166 bytes): +0x2F.
- `PlayerLifecycleView::SpawnRewardTailState` (138 bytes): +0x05.

All i386 caller instruction bytes and field positions are unchanged.
No new registers, relocated objects or ABI adapter calls are introduced.
The code also removes the now-unused projection header and global from the
Player runtime implementation; its remaining compatibility uses elsewhere
are independent and not silently deleted.

Fresh pinned VC7.1 builds of the affected PlayerRuntime translation unit
and its distinct ctor/load/collision/bomb carriers establish **21/21
complete exact functions / 5,391 original physical bytes / 288 relocation
fields**. Eleven compiler-private `$L` symbol spellings in the 852-byte
Player update-before-state helper changed. Every definition was checked
against its actual same-COFF-section offset and the original immutable PE
jump target before rebinding just its label. The existing 522-byte Player
update, 335-byte bomb-state, 166-byte high-priority draw and 138-byte tail
state functions remain fully original-exact. Eighty-two focused native
Player, selector and shared-runtime tests pass, including strengthened
checks for all five original Front pointer references.

The canonical authored exact count remains **1,024**; no new exact function
was earned from this source provenance consolidation, and no full cold
replay or Windows product-link/runtime proof was performed.

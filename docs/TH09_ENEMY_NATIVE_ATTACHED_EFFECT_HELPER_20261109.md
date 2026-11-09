# The exact attached-effects updater takes a native Enemy

`EnemyCoreUpdateAttachedEffects` at 0x40F4C0 is a 151-byte helper in the
Enemy/ECL translation unit. Its body already uses the real Enemy's
attached-effect slots at +0x53B4, position at +0x2D74 and the shared
`EffectRingRuntimeView` radius and phase. The helper now accepts
`EnemyView *` directly rather than taking an `EnemyCoreView *` only to cast
it back, and the 3,900-byte Enemy OnUpdate passes its native pointer.

Complete pinned VC7.1 comparison proves the helper **151/151 bytes and
2/2 relocations**. The original Enemy boss REL32 at +0xEAA still resolves
to 0x40F4C0; its **3,900 bytes, 97 bound references, 69 calls, original CFG
and 13 unlike bytes** are unchanged. All 77 exact ECL siblings (24,368
bytes, 1,096 fields) still match. Two private same-section label identities
were rebound only after actual COFF values matched original PE targets.
Related Enemy tests pass. No broad cold replay or new exact credit.

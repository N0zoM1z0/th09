# TH09 Player respawn-resource leaf returns to native Player across collision boss

Original executable: independently SHA-256 verified Japanese TH09 v1.50a,
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint is substantive natural C++ method/receiver reconstruction,
not historical nominal class-name proof, a newly byte-exact function or a
whole-product build/semantic acceptance.

## One native Player, not a separate four-byte method owner

The complete original **162-byte `AddRespawnResource` at 0x0041BC90**
changes the real Player +0x30388 resource meter, adds the native
shot-type-one 15-percent bonus, clamps at 400, and calls the original
FrontSide meter update only when the integer-hundreds threshold changes.
Its maintained method was declared on an artificial `PlayerState4OpsView`
containing only the Player +0x00 state word, although every original caller
passes the native full Player receiver.

It is now declared and defined on the actual `PlayerLifecycleView`, with
zero new storage and unchanged thiscall method arguments. The original
**400-byte `PlayerCheckState4`**, **852-byte `UpdateBeforeState`**,
Player reward/item branches, and the still nonexact **862-byte
`PlayerLifecycleView::CheckBulletCollision`** now call the actual native
method without casts through a second source class. The collision helper
no longer includes the obsolete four-byte state view solely for this call.
The state prefix still exists for its independently exact setter and the
two independently addressed transition methods; those are not silently
renamed here.

## Whole original-function and nonexact boss validation

A separate original-profile pinned VC7.1 experiment proved the natural
receiver migration preserves **all 162 raw bytes** of the provider,
**all 400 bytes** of PlayerCheckState4 and **all 852 bytes** of
UpdateBeforeState, including original call-site offsets and addends. The
latter two old-to-new external C++ names target **the same original
0x0041BC90 VA**. The original Player collision boss independently retains
**862 original / 849 candidate bytes, 26 original-bound fields, 12 ordered
calls, 799 differing overlap positions and 13 missing bytes**. The full
3,900-byte Enemy OnUpdate keeps 97 fields, 69 calls and 13 ordinary
differences, without new exact credit for either large function.

Fresh source-cold strict PE/COFF replay of the changed PlayerRuntime,
PlayerCollisionHelpers and EclManager profiles covers **109 complete
already-exact owners / 36,467 physical bytes / 1,709 relocation fields**.
The native class declaration caused 17 PlayerRuntime and eight ECL local
`$L` names to shift; the real same-section COFF symbol-definition offsets,
complete physical owners, each immutable original PE operand and target
VA were checked before **only the proven local names** were rebound in
the strict match manifest. No compiler profile, target binary, function
size, original external call target or comparator strictness was altered.

The fixed-profile natural trial, source-cold and nonexact-boss diagnostics
are retained locally under
`.analysis/gpt-web-player-native-respawn-20261107/`. No full cold replay,
whole Windows game link, Factory acceptance, runtime semantic acceptance or
remote push is claimed. Tracking remains **1,034 source-present / 1,021
exact / 13 nonexact**.

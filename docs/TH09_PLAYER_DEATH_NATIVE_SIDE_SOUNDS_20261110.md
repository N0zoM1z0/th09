# Original-exact Player death uses native side, SoundPlayer and FrontSide

The 753-byte `PlayerLifecycleView::EnterDeathState` at original TH09
v1.50a VA 0x41E420 previously redeclared a duplicate gameplay-side record
and shadow SoundPlayer, EffectManager and FrontSide receivers. Those owners
are actually the established native `AsciiGameManagerSideView` at Player
+0x0C/+0x10/+0x14/+0x18 and the real global SoundPlayer at 0x4DC698.

The C++ death handler now reads its native `sideState->shotType20`, calls
`sideState->effectManager0C->SpawnEffectInFixedSlot` and
`sideState->frontSide18->ClearState`, and plays both sounds through the
real `g_SoundPlayer`. Its opponent-side attack interfaces remain bounded
projections where their historic owner type is still unproven. This does
not assume those projections are equal to CardAttack.

The six changed original PE operands (+0x2C/+0x31/+0x1D1/+0x1FD/+0x202/
+0x20E) were read directly, decoded and checked against the old target
addresses before the real C++ symbols replaced the facades. Pinned VC7.1
regenerates **753/753 physical bytes and 38/38 relocation fields**, all
strict-original-exact. Focused Player death, shared runtime and FrontMessage
tests pass. This improves C++ ownership of an existing exact function;
there is no new exact credit or whole-project cold replay.

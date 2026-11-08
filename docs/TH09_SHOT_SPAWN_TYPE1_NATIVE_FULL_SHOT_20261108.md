# TH09 exact Type1 shot spawn uses the native full 0x484-byte Shot

The original `PlayerShotSpawnCallbackType1` at **0x00441EF0** receives
the real Player, its exact 0x484-byte Player shot and an SHT descriptor.
Its maintained C++ had a duplicate `PlayerShotSpawn1ShotView` with
0x2A4 opaque VM bytes, a private trail type and a partial shot tail.
That second shot structure has been deleted: the callback now accepts
the genuine `PlayerShotCtorView *` created by Player.

The real Shot carries `PlayerConstructedVector3View trailVectors2B0[32]`
at +0x2B0 and its position at +0x2A4; the shot tail's formerly opaque
+0x468, +0x46A and +0x46C halfwords are now `sourceOption468`,
`field46A` and `animationIndex46C`. Together with `shotType464` and
`velocity43C`, these fields preserve the existing original
0x484-byte allocation, constructor count and shot state transitions.
The real Player `state3031C.shot0C` now receives the same shot pointer
without an intermediate cast. The SHT descriptor still uses its existing
callback-local view; its full native identity is a separate question.

Two bounded natural fixed-VC7.1 source models used the true native shot
pointer, one directly and one with an ordinary shot reference alias.
Both reproduced **all 151/151 original bytes and both relocation fields**.
The adopted direct form retains the original timer-assign call target
0x00401500 and shot initializer target 0x0041F350, with no dummy locals,
register hints or assembly. The native shot header reorders no physical
field: an original-bound source-cold replay of EclManager, PlayerRuntime,
PlayerAddedState, PlayerOwnerStateReward, FrontResult and shot spawn
checks **111 preexisting exact functions / 38,156 physical bytes /
1,827 relocation fields**. Two ECL, seventeen PlayerRuntime/AddedState,
and six reward compiler-private labels were independently checked against
original PE destinations and same-section COFF definitions before
rebinding names only.

The still-open original 3,900-byte Enemy update remains 13 ordinary bytes
unlike; the 862/849-byte Player collision remains 13 bytes short and
15,564-byte RunEcl remains 30 bytes unlike. This checkpoint recovers
native C++ storage, **not newly credited byte-exact function count**.
Target original is Japanese TH09 v1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Evidence is retained in `.analysis/gpt-web-spawn1-native-shot-20261108/`.
No whole-project cold replay, final Windows game link or Factory
acceptance is claimed.

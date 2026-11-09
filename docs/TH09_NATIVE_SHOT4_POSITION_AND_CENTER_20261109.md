# Type-4 shot update uses the constructed PlayerPositionView

The original TH09 v1.50a `PlayerShotUpdateCallbackType4` at 0x443930 is a
249-byte `__fastcall` callback whose original external parameter identity
is retained. Its source previously declared two independent 12-byte vector
records (`PlayerShotUpdate4Vec3` and `PlayerShotUpdate4RegionPointView`)
even though their source/destination storage is already the complete
`PlayerLifecycleView` position at +0x1B88 and the constructed
`PlayerShotCtorView` position at +0x2A4.

The callback now uses `PlayerPositionView *position = &nativeShot->position2A4`,
a native copy `*position = lifecycle->position1B88`, and a native
`PlayerPositionView center` for its Type0/Type1 rectangle creators. Neither
redundant vector declaration remains. The original velocity snapshot and
x87 arithmetic are preserved, with original Player movement scalars and
shot timer data unchanged.

Six natural C++ variants were compiled under the pinned VC7.1 profile.
The chosen all-native variant reproduces **249/249 PE bytes, 9/9
relocation fields**. Other complete exact neighboring owners:
Type2 update (643 bytes/16 fields), Type0/Type1 rectangle creators
(61 and 55 bytes/3 fields each). Sixty focused Player native and shot
tests pass. This was already authored-exact; no count increase, full cold
replay or Windows product/runtime claim is made.

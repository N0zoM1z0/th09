# Complete Player damage boss uses constructed Player shot storage

The original TH09 v1.50a `PlayerLifecycleView::CalcDamageToEnemy`
(0x41FCD0, 996 bytes) now iterates the actual 128-element `shots`
array of `PlayerShotCtorView` rather than an independently redeclared
0x484-byte `PlayerDamageShotView` array. The body consumes the real
constructed `PlayerAnmVmView` rotation/animation fields, +0x2A4 position,
+0x430 hitbox, +0x43C velocity, ZunTimer at +0x454, damage/state/type at
+0x460..+0x464, and collision callback at +0x47C. Its first-shot effect
position is `&shots[0].position2A4` on the original allocation.

Only the separately reviewed callback ABI and region record projection
remain as explicit compatibility casts. Two changed compiler symbols
are now `ZunTimer::operator%` at original target 0x40F830 and
`PlayerPositionView::operator float *` at original target 0x4343D0. Both
REL32 operands (+0xEB/+0x16B) were independently checked against the
original PE before rebinding the canonical target-based symbol records.

The complete pinned VC7.1 original-bound compare passes **996/996 bytes,
17/17 fields**. Its same-source 55-byte AABB helper remains exact.
The independent 3,900-byte Enemy boss still calls the same Player damage
function at 0x41FCD0 twice and has 13 nonfield unlike bytes, unchanged.
Native Player and Enemy/player-state tests pass. Already-authored exact
count remains unchanged; no broad cold replay or runtime equivalence is
claimed.

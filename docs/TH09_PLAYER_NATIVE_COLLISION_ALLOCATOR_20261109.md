# Region allocator now belongs to native Player

The original TH09 v1.50a `AllocateCollisionRegion` at 0x41CF30 operates
on the real Player's reusable collision pool. Its 0x44-byte embedded
fallback record at +0xB0BC, two 514-pointer lists at +0xB100/+0xB908,
and counts at +0xC110/+0xC114 are already constructor- and
collision-boss-backed `PlayerLifecycleView` members. The maintained C++
allocator now uses those actual fields rather than a separate
`PlayerCollisionRegionCreateView` prefix allocation.

Seven exact region creators, including the two previously retyped native
circle creators, now call `PlayerLifecycleView::AllocateCollisionRegion`.
Each original PE REL32 operand was independently checked against the
unchanged 0x41CF30 target before rebinding C++ symbols. The full eight
creator/allocator functions match all **477 original bytes and 14
relocation fields**. `FrontCalcCallback` (1,491 bytes/107 fields) and
`Player::OnUpdate` (522 bytes/40 fields) retain exact machine code after
recompilation against the shared native header. Native collision and Player
tests, including negative pool-layout controls, pass.

The remaining five region-creator method definitions still use a
compatible source-facing receiver projection; these are the next
bounded native-owner migration. No exact ledger credit or broad cold
replay is claimed for this ownership correction.

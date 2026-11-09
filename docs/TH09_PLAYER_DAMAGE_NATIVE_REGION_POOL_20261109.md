# The 996-byte Player damage boss scans its genuine collision region pool

The TH09 v1.50a `PlayerLifecycleView::CalcDamageToEnemy` at 0x41FCD0 now
scans `PlayerCollisionRegionCreateRegionView **slot = activeB100` directly.
This is the true 514-pointer Player allocation at +0xB100, populated by
the recovered native region allocator and seven exact region creators.
The damage loop now uses its real `interval34`, `hitCount2C`, `hitCap30`,
`type38` and geometry members rather than a redundant
`PlayerDamageRegionView` overlay. The native region class exposes the
small `BottomY()` inline geometric accessor, retaining the original
floating-point operation scheduling at the +0x2B9/+0x2BC instructions.

Two otherwise equivalent ordinary inline arithmetic expressions were
compiled as negative controls: both matched only 994/996 bytes because
VC7.1 swapped the half-height and center-Y memory operands. The selected
native method matches **996/996 physical bytes and 17/17 relocations**,
including the original x87 instruction order. The original Japanese PE
and all relocation destinations are unchanged.

Scoped recompilation verifies **99 original-exact functions, 36,807
physical bytes and 1,700 bound relocations** across the affected Player,
region creators, FrontCalc, Player gameplay and Enemy/ECL source units.
Two compiler-private ECL label spellings were rebound only after their
true same-section COFF values equaled the original PE jump destinations.
The still-nonexact Enemy OnUpdate has 3,900 bytes, 97 bound fields and
13 unlike bytes, without regression. Native Player, Enemy-state and
collision-pool tests pass. No broad cold replay or new authored exact
credit is claimed.

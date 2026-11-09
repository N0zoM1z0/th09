# Enemy OnUpdate uses the complete native Front indicator (2026-11-09)

The Japanese TH09 1.50a 3,900-byte `EnemyManagerView::OnUpdate` made two
calls to the original 213-byte `SetSideEnemyIndicatorPosition` at 0x416080.
Its historical source views named a second, truncated `FrontEnemyIndicatorView`
and a `g_PlayerSharedRuntime` pointer at 0x4A7E38. The actual allocation is
now independently constructor-proven `FrontFactoryConstructView` (0x1293C
bytes), reachable via the authentic `g_GameManager.frontFactoryA8` pointer
at GameManager base 0x4A7D90 + 0xA8.

The indicator callback now belongs to the real Front C++ owner and accesses
`markerVms123F4[sideIndex]`: this is the same native pair of 0x2A4-byte
`AnmVm` members the Front factory constructs and the Draw callback consumes.
The obsolete duplicate prefix class/header was retired; offset/color/position
assertions are preserved at the genuine owner function boundary.

Complete pinned VC7.1 original-PE/COFF checks:

- `FrontFactoryConstructView::SetSideEnemyIndicatorPosition`:
  **213/213 bytes, ten relocation fields**, zero differences.
- `EnemyManagerView::OnUpdate`: **3,900 target/source physical bytes,
  97/97 independently resolved references, 69 ordered calls and unchanged
  switch/CFG**, with the same **13 residual nonfield byte differences**.
  The two changed root fields at +0x34 and +0xE91 are now strict
  `g_GameManager` +0xA8 bindings. The original PE operand for each is
  0x4A7E38; both corresponding direct calls retain 0x416080 as destination.
- **77 exact EclManager.cpp sibling owners**, 24,368 whole physical bytes
  and 1,096 fields, still fully original-exact. Twenty VC7.1-private `$L`
  labels in two sibling switch-bearing owners were rebound only after
  validating their actual same-section definition offsets against original
  unchanged PE target addresses.
- Front factory/calc/draw callbacks (220, 1,491 and 1,474 bytes) still
  match fully. 16 Enemy inspector tests, 31 Front-message tests and two
  native Effect tests pass.

The independent 3,900-byte Enemy owner is **still nonexact**; no partial
credit or authored exact count increase is claimed. This checkpoint restores
real C++ object ownership and testable source linkage without changing the
original i386 game binary or invoking a complete cold replay.

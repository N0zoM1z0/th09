# FrontCalc's four circle calls now use the real collision-region producer

The original TH09 v1.50a FrontCalc callback at `0x417630` creates two
circle-region types for each of the game's two native Player objects when
its transition frame reaches zero. Its previous source declared a separate
`FrontCalcCollisionView` with method signatures returning untyped `void*`,
and a separate three-float point structure.

FrontCalc now calls the maintained native `PlayerCollisionRegionCreateView`
producer at the same Player receiver address. The stack point is the
already-maintained three-float `PlayerPositionView`; the 2D region API
receives a bounded view of its first two coordinates without removing the
original third (zero) coordinate store. The existing Player collision
memory projection is independently anchored to the real Player region
storage at +0xB0BC/+0xB100; no independent allocation was introduced.

The pinned VC7.1 target-bound replay verifies complete original equality:

| Owner | Bytes | Relocations | Result |
| --- | ---: | ---: | --- |
| `FrontCalcCallback` | 1,491/1,491 | 107/107 | exact |
| `PlayerCollisionRegionCreateView::CreateCircleType1` | 43/43 | 1/1 | exact |
| `PlayerCollisionRegionCreateView::CreateCircleType4` | 49/49 | 1/1 | exact |

All four changed REL32 operand encodings (+0x32C/+0x34E/+0x36B/+0x38D)
were checked against the immutable original PE before rebinding their
source identities. The targets remain `0x41CFE0` and `0x41D0D0` in the
original call order. Targeted Player native, collision, Front message,
Front result and CardAttack tests pass. The authored exact count is
unchanged; no full cold replay was used.

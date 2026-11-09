# FrontDrawCallback now uses the native Front factory allocation

The complete original TH09 `FrontDrawCallback` at `0x00417C10`
(1,474 bytes, 127 relocations) is unchanged after replacing its separate
opaque `FrontDrawView` with the constructor-proven native
`FrontFactoryConstructView`. Its two panel vertex strips, seven front animation
VMs, two marker VMs, score and transition fields, and nested message runtime
are now read through the same physical allocation as the factory and Added
callback. The line-strip call retains its original source-linked C++ parameter
symbol: the native panel vertices have the independently checked 0x14-byte
XYZRHW/diffuse layout, cast at that explicit API boundary.

The formerly opaque scalar ranges at Front +0xE934, +0x10958, and +0x11E88
now expose the actual score, panel and transition fields consumed by the
original-exact draw and calc callbacks. This is a layout and source-interface
recovery; no additional gameplay behavior or source-only calling convention
was introduced. The native message runtime also declares its existing
`Draw()` method. Its implementation remains at its independently reviewed
original symbol. The constructor continues to default-construct the same
AnmVm arrays and timers; scalar field names add no calls.

## Complete affected verification

Recompiled from the current worktree with the pinned VC7.1 original profiles:

| Actual owner | Physical bytes | Relocations | Result |
| --- | ---: | ---: | --- |
| FrontDrawCallback | 1,474 | 127 | exact |
| FrontAddedCallback | 706 | 37 | exact |
| CreateGameSubsystem | 220 | 22 | exact |
| FrontMessageRuntimeView::LoadForSideCount | 405 | 18 | exact |
| FrontMessageRuntimeView::ReleaseForSideCount | 271 | 14 | exact |

All five original-bound construction interfaces also remain exact under the
pinned constructor-only `/O2 /Ob0` profile: FrontSide 307/26,
FrontSideAuxView 28/2, FrontFactory 143/12, message runtime 85/7 and
shared three-byte panel vertex alias 3/0. The constructor origin classification
remains unresolved. No canonical authored exact count has been changed.

Use `scripts/build-match-unit.py --unit front-draw-callback` and
`scripts/compare-coff-function.py --unit front-draw-callback --json` for the
complete draw owner. For the constructors, run
`scripts/inspect-front-constructors.py build/matching/FrontSideCtor.obj
--factory-object build/matching/FrontFactoryCtor.obj` after the two original
profile builds. This change did not invoke a full-project cold replay.

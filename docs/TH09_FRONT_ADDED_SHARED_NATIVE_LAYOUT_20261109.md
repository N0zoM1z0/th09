# Front AddedCallback uses the native constructor allocation (2026-11-09)

`FrontAddedCallback` at original TH09 1.50a VA `0x00418480` now operates
on the exact same typed `FrontFactoryConstructView` allocation that
`CreateGameSubsystem` constructs. The former source declared a second
0x1293C-byte view with only seven of 88 animation VMs and several large
opaque regions, even though the native factory constructor has already
established the complete member layout.

The constructor's ten 0x14-byte panel vertices are modeled as a real
`[2][5]` array. Their XYZ position is a plain twelve-byte aggregate,
not the nontrivially constructed `Float3`: introducing ten `Float3`
constructors would contradict the original 143-byte construction chain.
The view's two result VMs and two marker VMs are accessed as real grouped
`AnmVm` arrays rather than four independently named scalar fields.
The genuine message runtime subobject remains at +0xE944.

The old `FrontAddedView` definition, oversized unknown padding and duplicate
layout assertions have been removed. No new allocation, callback, synthetic
method, copy or side effect has been added.

## Affected complete-original checks

- `FrontAddedCallback`: 706 / 706 physical bytes, 37 / 37 original
  relocations, zero differences under the pinned VC7.1 source profile.
- `CreateGameSubsystem`: 220 / 220 physical bytes, 22 / 22 original
  relocations, zero differences after rebuilding its native factory view.
- Native constructor-only `/O2 /Ob0` carrier: factory 143 / 143 with
  12 relocations, nested message 85 / 85 with 7, and shared vertex
  3 / 3 with none. Full linked bytes and original referents match.
- Header consumers `FrontMessageLoadForSideCount` (405 / 405),
  `ReleaseForSideCount` (271 / 271) and the existing `FrontAddedCallback`
  were also recompiled and checked in this session.

Reproduce the affected source with `scripts/build-match-unit.py --unit
front-added-callback`, `--unit front-factory`, and the original-bound
`scripts/inspect-front-constructors.py --factory-object` invocation recorded
in `TH09_FRONT_FACTORY_NATIVE_CTORS_20261109.md`. All construction special
members retain unresolved explicit-versus-implicit origin, so canonical
1024 exact authored functions and 13 nonexact bosses remain unchanged.

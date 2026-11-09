# Front message drawing uses the native constructed runtime

The 995-byte original `FrontMessageRuntimeView::Draw` at `0x00417130`
is now compiled against the same `FrontMessageRuntimeView` type used by
its 85-byte original-exact constructor and its independently exact
load/release methods. The prior draw TU declared a second entire 0x1D70-byte
runtime with eleven nominally contiguous animation VMs and an opaque
constructor area. That competing definition is removed.

The constructor proves three distinct native animation subgroups: eight
`AnmVm` at +0x14, two at +0x1534, one at +0x1A7C. The complete drawing
function now reads each actual group directly, rather than indexing an
invented single eleven-element C++ array. It uses the native +0x1D58
ZunTimer member. Draw's observed color arrays at +0x1D20/+0x1D30,
font state, text selectors and visibility flags now name their actual
fields in the shared layout, with compile-time offset and size assertions.
There is no extra subobject, call, padding or assembly.

The pinned VC7.1 `/O2 /Ob0` comparison covers all **995 physical bytes,
73 original relocation records and original PE field targets** with zero
differences. The affected shared-header cohort was freshly recompiled with
its own pinned profiles: LoadForSideCount 405/18, ReleaseForSideCount 271/14,
FrontAddedCallback 706/37, CreateGameSubsystem 220/22,
FrontDrawCallback 1474/127 and FrontCalcCallback 1491/107. All seven full
original-bound comparisons pass, totaling **5,562 bytes / 398 fields**.
The original-exact FrontSide and Front factory/message constructor group
(five source-equivalent interfaces) was also rerun without changes.

To reproduce the edited draw owner only, run
`python3 -B scripts/build-match-unit.py --unit front-message-runtime-draw`
and `python3 -B scripts/compare-coff-function.py --unit
front-message-runtime-draw --json`. Original special-member authorship and
the nonexact bosses remain independent open questions; this change does not
increase the canonical authored exact-function ledger or establish runtime
product completion. Only affected local carriers were rebuilt.

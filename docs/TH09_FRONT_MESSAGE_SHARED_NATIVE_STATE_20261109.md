# TH09 Front message native state shared across draw, loading and construction

This original-bound checkpoint replaces the remaining opaque scalar tail of
`FrontMessageRuntimeView` with the original time/portrait/text lifecycle fields.
The complete 0x1D70-byte runtime belongs to the native Front factory's
constructor at 0x00418750 and is consumed by Front message draw, calc,
load, release and random setup. Previously these members were duplicated
as independently typed fields inside the `FrontMessageUpdate.cpp` projection.

The primary message instruction time is now `unsigned short` in the shared
`FrontMessageTableView.hpp` wire header. The original Japanese TH09 v1.50a
interpreter at 0x00416590 explicitly uses `MOVZX` (not `MOVSX`) for the
word at its current-instruction +0x00 at 0x004165DD and 0x004165EF.
This target instruction evidence corrects the earlier shared signed-short
model; no pointer-size or variable-stride assumption was imported from a
neighboring game.

The actual runtime scalar members now identify mode +0x10,
`framesElapsed` +0x1D4C, wait threshold +0x1D50, glyph size +0x1D54,
secondary timer +0x1D58, color/portrait/text flags +0x1D64..0x1D67,
ignore-wait word +0x1D68, skip state +0x1D6C and box visibility +0x1D6D.
The two last ANM VMs are named text VMs at +0x1534 and intro VM at
+0x1A7C. Their physical constructors remain unchanged; all sizes and
single-field offsets are compile-time asserted. An `Update()` declaration
now names the existing original-bound method without inventing a new body.

## Affected scoped VC7.1 oracle

The pinned profiles independently recompiled nine actual canonical functions,
**6,612 complete original physical bytes / 423 original relocation fields**:
Front message draw, added callback, calc, draw, factory, message load,
message release, message random setup, and message setup. All nine compare
**exact** over their complete original extents. The constructor-only carrier
also matches all five previously accepted original bodies (FrontSide and
FrontFactory chains). The physical instruction/reference match, not a
source name or stored historical receipt, determines that status.

Reproduce only the changed source consumers with
`scripts/build-match-unit.py --unit front-message-runtime-draw` and the
corresponding `scripts/compare-coff-function.py --unit ... --json`; repeat
for the eight other units listed above. `scripts/inspect-front-constructors.py`
checks the two source-equivalent constructor carriers. No full cold replay,
new authored-exact classification or Windows product link is claimed.

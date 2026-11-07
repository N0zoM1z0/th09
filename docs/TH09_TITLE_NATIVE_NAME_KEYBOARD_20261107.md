# TH09 Title native 6×16 name-entry keyboard — 2026-11-07

Original executable: Japanese TH09 1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint modifies actual C++ source for **TitleScreen::UpdateReplaySave
(1,825 original bytes)**, **OnUpdateResultNameEntry (966 bytes)** and
**DrawReplaySave (800 bytes)**. The source-facing `TitleNativeNameKeyboard`
type does not claim the historical class or member names were identical.

## Native keyboard geometry and real use sites

Both original Title controller callbacks navigate **six rows of sixteen
keys (96 slots)**, while the original save renderer draws the same grid.
The keyboard cursor wraps at 96; vertical movement advances by 16, and
horizontal movement wraps across the 15-column offset. The last displayed
row, row index 5, assigns two distinct glyphs to columns 14 and 15:
`0x7F` for space and `0x80` for accept. These positions correspond to
**indices 94 and 95** in the actual controller. Index **93** is skipped in
input navigation; its existing special skip destinations (77, 13, 92, 94)
are preserved. The first 93 keyboard entries are not reinterpreted.

The narrowly included C++ header `TitleNativeNameKeyboard.hpp` provides
`Rows`, `Columns`, `KeyCount`, `LastRow`, `GapKey`, `SpaceKey`, `ConfirmKey`,
`SpaceColumn` and `ConfirmColumn` with compile-time cardinality checks.
Both large Title update methods use these values for keyboard wraps,
selection and write/accept behavior. The existing 800-byte original-exact
DrawReplaySave now draws its row/column positions using the same constant
layout and its final two original glyphs. Separate 50-entry replay arrays
and ten-stage input records remain owned by the previously verified
`TitleReplayNativeSlots` type; **the 96 keyboard keys are not replay slots**.
No source type, allocation, callback ABI, animation phase timer, or side
state is fabricated.

## Independent, full original-bound codegen controls

The untouched baseline and paired naturally written C++ variant were cold
compiled under the **three actual pinned VC7.1 source/profile carriers**:
`TitleScreen.obj` (/O1, 40 exact functions), `TitleScoreRecord.obj`
(/O2, one exact function), and `TitleReplaySave.obj` (/O1, one exact
function). Every physical instruction and table byte, every original PE
relocation operand and its source COFF referent, and all literal definitions
were compared. The paired model preserves **all 42 previously exact source
functions, 19,772 complete bytes and 994 relocation fields**.

The source-only keyboard enum changes nine private compiler `$L...` label
spellings inside the /O1 TitleScreen carrier. For each, the actual
same-section COFF symbol definition and original PE pointer word were
independently checked at the same relocation offset/type/addend. The nine
proven name-only changes were accepted by an isolated manifest and the
**unchanged strict original-bound comparator** on all 42 functions before
the canonical manifest was updated. No original target VA or field was
changed merely to accommodate a candidate.

A separate production `--owner title` source-cold replay also verifies
**49 exact Title functions, 22,710 bytes and 1,083 relocation fields**, plus
all required nonexact Title owner guards. `TitleScreen::OnUpdateResult`
remains **484/485 bytes, 218 unlike overlaps and one missing byte**;
`DrawReplayMenu` retains **1,280 bytes, 57 bound fields and 29 unlike bytes**.
The 800-byte DrawReplaySave remains exact with 41 original fields.

The per-profile compiler/object SHA receipts, source diff, tested header,
private manifest and original-field proof are in ignored
`.analysis/gptweb-title-keyboard-native-20261107/`. This is semantic
source ownership work, **not additional canonical exact credit**. The
whole Windows executable, runtime, portability, remote publication and
Factory acceptance remain independent open gates. No all-owner cold replay
was performed. Current classification remains 1,034 source-present / 1,021
exact / 13 nonexact functions.

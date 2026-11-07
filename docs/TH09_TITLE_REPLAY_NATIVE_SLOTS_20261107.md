# Title replay display: 50 native record slots, two columns, ten stages

Target is TH09 v1.50a original Japanese PE, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint edits a **1,280-byte nonexact DrawReplayMenu big function**
and its independently **exact 800-byte DrawReplaySave renderer**. It records
native array bounds, not an assertion that the reconstructed C++ member
names were those used historically.

The physically shared `TitleScreenView` stores exactly 50 replay paths of
0x200 bytes each at +0x28C and 50 complete 0x1EC-byte `ReplayDataView`
records at +0x68E4. The original display traverses both sets in two
columns of 25 rows. Each replay has three player input streams, with ten
stage records per stream; the replay menu enumerates stage row count ten.
The runtime used the same native arrays in both the nonexact menu renderer
and the original-exact save renderer, not unrelated copies of the replay
header or per-screen storage.

The source-only `TitleReplayNativeSlots.hpp` derives `ReplayCount` and
`PathCount` from the real arrays, `RowsPerColumn` from two displayed columns,
and `StageCount` from one native frame table dimension. Compile-time checks
pin actual 50/50/25/10 counts and native 0x1EC replay record. The 50 replay
records occupy `50 × 0x1EC = 0x6018` contiguous bytes, precisely from
`+0x68E4` to `+0xC8FC`; the next original 0x0C-byte gap ends at the
`selectedReplay` field at `+0xC908`. No alternate shadow replay allocation
is introduced. Both real source files now use these native counts rather
than separate numeric 50, 25 and 10 loop bounds. Position and data-selection
logic, replay names/time and `frameStart` pointer semantics are unchanged.
The ten-frame selection animation and the six-row by sixteen-column name
entry keyboard use independent control counts; they are **not** derived from
the ten serialized stage entries or the 25-row replay menu columns.

Four complete source-cold pinned VC7.1 controls were built independently:
original, menu-only typed loops, save-only typed row bound and both typed
renderers. All four retained the 1,280-byte DrawReplayMenu shape, 57
original-bound fields, 16 verified literal definitions, 30 literal
references, 13 ordered calls, 72 original/control blocks, and the **same
29 ordinary unlike bytes**. The actual production `--owner replaymenu`
check reverified this complete, still-open owner. The matching 800-byte
DrawReplaySave was strictly original-exact in all four controls and in the
production pinned build (all 41 original relocation fields).

The private A/B compiler objects, source/profile hashes and full original
owner diagnostics are under `.analysis/gptweb-title-replay-slots-20261107/`.
This checkpoint improves native type/array ownership, not canonical exact
function count. Original names, other near-match bosses, full cold replay,
Windows executable linkage, runtime oracle, semantic/port gates, remote
publication and Factory acceptance are not claimed. Ledger remains 1,034
source-present / 1,021 exact / 13 nonexact.

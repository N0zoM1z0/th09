# TH09 exact 1,939-byte ResultDraw uses the shared native Title keyboard

Original target: Japanese TH09 v1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This change reconstructs ordinary C++ source relationships, not a newly
byte-exact full function or confirmed original historic TU partition.

The already exact `TitleScreenView::DrawResult` at 0x00423D16 uses the
same **6-row × 16-column = 96-key native name-entry grid** as the exact
`DrawReplaySave` and the Title name entry. Previously the full ResultDraw
source still repeated literal `6`, `16`, last row `5`, and special columns
`14`/`15`, although the two Title keyboard consumers already shared a
compile-time checked `TitleNativeNameKeyboard`. The source now refers to
its `Rows`, `Columns`, `LastRow`, `SpaceColumn`, and `ConfirmColumn` as real
named members. The 93rd slot is the gap, followed by native space and
confirm keys 94 and 95. No loop bounds, behavior, UI positions, or
character-table indexing have been changed.

The nontrivial original ResultDraw body remains an independently complete
**1,939-byte exact function with all 82 original-bound PE/COFF fields**.
The original-bound VC7.1 cold build matches **every 1,939 machine bytes**
with no new local label rebinding. The 800-byte exact ReplaySave renderer
continues consuming the same keyboard dimensions. The 1,280-byte
ReplayMenu boss remains nonexact with its existing 29 ordinary residual
bytes, and the 485-byte OnUpdateResult remains separately nonexact;
no canonical ledger entry has been promoted.

A focused regression test guards the real shared keyboard extent and both
source references. The existing 178 Title tests also pass. Source-cold
receipts are in `.analysis/gpt-web-title-result-keyboard-native-20261107/`.
No project-wide cold replay, final Windows product link, runtime gameplay
acceptance, Factory receipt or remote push is asserted. Totals remain
**1,034 source-present / 1,021 exact / 13 nonexact**.

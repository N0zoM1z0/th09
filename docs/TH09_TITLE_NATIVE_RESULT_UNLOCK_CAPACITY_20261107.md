# TH09 Title result full-character unlock capacity from native PLST storage

Original: Japanese TH09 v1.50a, independently checked target SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a natural C++ source/physical-layout reconstruction checkpoint for
the **complete 485-byte original TitleScreen::OnUpdateResult boss**.
The historic type names and original translation-unit split remain unproven.

## One native sixteen-character extent, with distinct 14-character subset

The real `PlayStatsRecordView` at global `g_PlayStatsRecord` contains a
**3-by-16 byte `characterUnlocked` array** beginning at original record
+0x4C. The Title result page also has one actual
`g_TitleResultCharacterOrder[16]` sequence used for cursor selection.
These native arrays have exactly the same full 16-character extent; their
shared capacity is compile-time checked without introducing a second copy
of the unlock flags or cursor order.

`OnUpdateResult` now defines `FullCharacterSlots` from
`sizeof(g_PlayStatsRecord.characterUnlocked[0])` and reuses that value in
the full-unlock loop, cursor wrap bounds, and both original
`SetCharacterCursorInactive` calls. The **separate normal/MODE4 unlock
subset** in the original uses character indices 0..13; it remains
intentionally bounded at 14, rather than being incorrectly extended to
the physical 16-character row. It also continues to read the native
`g_ReplayInputStates[2].word06` masks, the exact cursor callee, and the
existing result-state/surface choreography without changing branch order.

## Full original physical owner and unaffected exact functions

Four independent natural source variants and the untouched baseline were
compiled with the unchanged pinned VC7.1 `/O1 /Ob1 /EHsc` profile. All
five produced the **same 484-byte candidate**, **147 instructions**,
**34 original-bound COFF fields, 9 ordered calls, 40 direct-flow blocks**,
and exactly **218 differing overlap bytes plus 1 missing original byte**.
The complete 485-byte original `OnUpdateResult` remains explicitly
**nonexact**. The source improvement does not create partial exact credit.

The changed source carrier was cold rebuilt under the existing canonical
profiles and compared against the immutable original PE for **41 complete
already-exact functions / 18,972 physical bytes / 953 relocation fields**.
These include 40 original-exact TitleScreen core owners and one independently
profiled 216-byte TitleScoreRecord insert owner. One 1,592-byte physical
`OnUpdateStartMenu` owner had **nine compiler-private `$L` name changes**.
Every field was audited against its genuine same-section COFF definition,
owner-relative offset, original immutable PE operand and destination; all
1,592 original bytes matched after normal relocation binding. Only the
nine reviewed *names*, not a target VA/addend, comparator rule or original
binary, were updated in `config/match-units.toml`. The 170 affected Title
tests also pass. Private original-bound evidence:
`.analysis/gpt-web-title-native-unlock-20261107/`, including
`title-private-label-proof.json`, the five source trial reports and the
strict 41-unit source-cold replay.

No full-project cold replay, native game product link, runtime/port
acceptance, remote publication or Factory exactness receipt is claimed.
Global status remains **1,034 source-present / 1,021 exact / 13 nonexact**.

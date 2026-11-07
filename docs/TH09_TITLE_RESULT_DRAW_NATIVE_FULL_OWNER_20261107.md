# Exact Title ResultDraw uses the same complete native 0x1B388 Title owner

Original: Japanese TH09 v1.50a, independently verified SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a C++ physical owner and cross-function layout reconstruction,
not new complete exact credit or historic original-class-name proof.

## Eliminate the final result renderer's duplicate truncated Title model

The exact **1,939-byte `TitleScreenView::DrawResult`** at 0x00423D16
previously maintained a separate 0x1B234-byte prefix named
`TitleScreenView` in `TitleScreenResultDraw.cpp`, with an opaque padding
region from +0x30 up to +0x1B228. Its own use of the original Title
`nameBankIndex` +0x08, `nameSlotIndex` +0x0C, `currentScreenState` +0x28,
`currentScreen` +0x1B228 and `stateTimer2` +0x1B230 matched the full
native Title layout, but the local class was structurally incompatible
with the independently maintained full 0x1B388-byte core/Options/Replay
owner.

The renderer now includes the same `TitleScreenReplayView.hpp` as the
main Title state machine, the complete 2,045-byte Options boss, the
1,280-byte ReplayMenu boss and the exact 800-byte ReplaySave drawing
owner. Its method declaration lives on the one full native class. No
shadow Title object, second C++ type with the same spelling, fabricated
constructor, extra allocation or altered scoreboard layout remains.
The original score table pointer, five rank/difficulty groups, selection
cursor, name-input animation and ANM color updates are unchanged.

## Whole exact function, original-bound sibling and open-frontier controls

A copy-on-write pinned VC7.1 trial compiled the real ResultDraw file
without its local class, demonstrating **all 1,939 original machine bytes
and 82 original relocation fields unchanged**. The independently compiled
adopted source also passes strict canonical full-owner original PE/COFF
comparison. A cold replay of the affected Title core, Options, ReplaySave
and ResultDraw carriers passes **45 already-exact complete functions /
23,790 physical bytes / 1,214 relocation fields** over five targeted
compiler profiles.

The shared title header's visibility caused exactly **nine compiler-only
`$L` name shifts** in the 1,592-byte physical Title OnUpdateStartMenu
owner. A whole 40-function main-source comparison proved every original
physical byte and all 948 fields, including same-COFF-section private
label offsets and immutable original PE operands, before updating only
those names in `config/match-units.toml`. No original VA, field type or
addend, compiler flag, or comparator strictness was changed.

The **1,280-byte ReplayMenu boss** still has 57 fields, 13 calls, 72
blocks and 29 ordinary byte differences. The separate **485-byte
OnUpdateResult** boss still compares as 484 candidate bytes, 34 fields,
9 calls, 218 unlike overlapping bytes and 1 missing byte. Neither is
promoted to exactness. Focused native-source and Title tests validate
that full gameplay/read context remains consistent. Private bounded
compiler receipts are retained at
`.analysis/gpt-web-title-result-draw-native-20261107/`.

The complete project ledger remains **1,034 source-present / 1,021
exact / 13 nonexact**, with no whole-project cold replay, Windows game
link, runtime semantic acceptance, Factory receipt or remote push claimed.

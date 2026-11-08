# TH09: real GameManager opponent selector across the 1,689-byte setup boss

Original: Japanese TH09 v1.50a with independently verified SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is ordinary C++ class/storage ownership recovery, not proof of historic
nominal type spelling, whole-game runtime acceptance or a new exact match.

## The 799-byte exact selector was already inside the native GameManager

`SelectOpponentConfiguration` at original **0x00415910** is an independently
exact **799-byte, 32-field** method. Previously it was implemented on a
separate, incomplete `GameManagerOpponentSelectionView` with two private
0x38-byte gameplay side records and an approximately +0x13C-byte manager
prefix. The complete native `AsciiGameManagerView` already owns the exact
0x390-byte original GameManager allocation and its real two gameplay-side
records. The temporary opponent manager and side/state models did not own
extra storage or a separate GameManager lifetime.

The real `AsciiGameManagerView` now declares and defines
`SelectOpponentConfiguration`. Its already allocated native fields are
exposed as correctly aligned aliases, without enlarging the object:

| Original GameManager offset | Field |
| --- | --- |
| +0xD8, 16 bytes | used-character table, shared with existing `clearD8` storage |
| +0xF0 | native 0x10-byte opponent-entry pointer |
| +0xF4 | selected value, shared with `valueF4` |
| +0xFC | existing round counter |
| +0x11C | existing setup mode |
| +0x138 | opponent selector, shared with `value138` |
| side +0x2C | native signed selected-parameter field |

The selector's original side-runtime read now uses the genuine
`AsciiGameSideRuntimeView *` at side +0x1C, not an independent
`OpponentSelectionState *`. It directly calls the independently exact
`AsciiGameManagerView::IsGameMode0/1` leaves rather than casting its `this`
receiver back to the same underlying object. Only the actual 0x10-byte
`OpponentSelectionEntry` wire record remains in the former opponent-header
file; the invented `GameManagerOpponentSelectionView`,
`OpponentSelectionSide`, and `OpponentSelectionState` are gone.

## Four calls in the complete nonexact GameplaySetupThread

The entire original **1,689-byte `GameplaySetupThread` at 0x0041AF2D** now
calls `g_GameManager.SelectOpponentConfiguration()` naturally at the same
four original call sites. Its four target-verified REL32 calls still reach
**0x00415910**. The separate source-facing setup layout retains its third
0x38-byte record for the shared EffectManager; the two playable side records
are unchanged. The 1,230-byte original-exact GameManager update and 425-byte
exact cleanup remain independently compiled source owners.

An isolated VC7.1 source experiment, followed by adopted source-cold builds,
reproduced **all 799 machine bytes** and **all 32 COFF relocation
positions/types/addends** of the selector with the true native method
identity. The full setup worker's independent noncrediting original-PE/COFF
frontier has **1,689 original / 1,682 candidate bytes**, **172 original
fields, 35 ordered calls, 694 differing overlapping byte positions and
seven missing original bytes**—precisely its previous frontier. Its
original direct control-flow graph still differs; no byte-exact credit is
claimed for this large boss.

The bounded original-bound replay covers **18 existing complete exact
functions / 6,854 physical bytes / 442 relocation fields** from
GameManagerSetup, GameManagerSelectOpponent, GameManagerMode and FrontMessage
sources, including the full 2,968-byte FrontMessage physical owner. Adding
the shared GameManager type aliases changed **30 private `$L` / `$run_scripts$`
spellings** in the latter; real same-section COFF label locations and
original immutable PE destinations were audited before only those names
were rebound in the strict manifest. This does not relax the comparator,
change a target VA, hide jump-table bytes, or substitute original binaries.

Old tests/inspectors naming the discarded opponent view have been updated
to the original-bound native method identity without loosening their
1689-byte extent, 172-field inventory, 35-call count or nonexact residual
gates. Bounded trial and proof packets are under
`.analysis/gpt-web-native-opponent-20261108/`. No full-project cold replay,
Windows product linking, gameplay runtime acceptance, Factory exactness
receipt or remote push is asserted. Overall tracking remains
**1,034 source-present / 1,021 exact / 13 nonexact**.

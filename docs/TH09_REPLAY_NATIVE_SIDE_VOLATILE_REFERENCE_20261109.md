# BeginPlaybackStage accesses a real native GameManager side runtime slot

The TH09 1.50a original callback `BeginPlaybackStage` at `0x420840`
reads the GameManager side-0 runtime pointer at original absolute
`0x4A7DAC` (GameManager root `0x4A7D90` + 0x1C) after publishing
`replayData->value1E7` and reading `value1E8`. VC7.1's candidate code
requires a volatile pointer-slot access to preserve this ordering.

The source now uses ordinary C++ `AsciiGameSideRuntimeView *volatile &`
reference to `g_GameManager.sides[0].runtime`, rather than an unrelated
`ReplaySideRuntimeSlotView` typedef and a reinterpret cast. Twelve natural
candidate expressions were compiled under the pinned profile; this version
retains the complete prior baseline and literal original call identities.
The unchanged 540-byte original produces a **538-byte** candidate with **32
source/target-bound fields, five correctly ordered calls, 12 agreeing direct
blocks, 39 differing overlap bytes and two missing bytes**. Thus the owner
remains NONEXACT; neither the two-byte gap nor register-order discrepancy
is claimed fixed. The complete 1,238-byte `SaveReplay` remains nonexact by
two bytes (+0xB7/+0xBB), independently reviewed.

The replay's 12 already-exact same-source owner functions, 4,093 bytes total,
were rechecked. All 22 focused Replay native tests pass, including the
original side-pointer target and row-index checks. No broad cold replay.

# Music-room 2,258-byte boss and 209-byte draw share the real Title allocation

The original TH09 v1.50a `TitleScreenView::OnUpdateMusicRoom` at
**0x426E05** (2,258 bytes) and `DrawMusicRoom` at **0x4244A9**
(209 bytes) were already complete PE/COFF exact but maintained a second,
truncated `TitleScreenView` in their own compilation unit. The main
Title, Replay, TitleCalc and TitleDraw source instead use a complete
**0x1B388-byte constructed TitleScreen**.

The real C++ owner now embeds **32 POD `MusicRoomTrackDescriptor` records**
from +0xC934 through +0x11B73. Each record has the original 0x292-byte
path/title/eight-description layout, and the complete array spans 0x5240
bytes. Native music selection, track count, listing index and pause words
follow at +0x11B74/+0x11B78/+0x11B7C/+0x11B80. The original
`musicAnm` at +0x11B90 is the already-known `titleAnm` member on this
same object. The separate Title class and descriptor definitions in
`TitleScreenMusicRoom.cpp` are gone. The source still calls native ANM,
ReplayInput, Supervisor and SoundPlayer implementations.

Pinned VC7.1 whole-function comparisons reproduce **2,258/2,258 bytes,
74/74 fields** for Update and **209/209 bytes, 6/6 fields** for Draw.
Eleven old music-room manifest fields omitted the nonzero root-relative
index addend. Every original PE DIR32 was re-read before adding the true
seven warning-text array offsets (+4 through +0x1C) and four side-2
ReplayInput offsets (+0x122/+0x123). The independent callsite validator
now fails closed on each of those eleven values; focused negative mutation
tests cover the new addends.

The shared type change renumbered nine private `$L` symbols in the
1,592-byte Title StartMenu and seventeen in the 328-byte TitleCalc
switch tables. All 26 actual same-section COFF definition addresses were
checked against the immutable original PE destinations before updating
only the local symbol spellings. Scoped recompilation confirms **49
original-exact Title functions, 26,914 physical bytes and 1,350 relocation
fields**. The still-nonexact 1,280-byte Replay menu retains its **29
ordinary unlike bytes and 57 bound fields**. No new authored exact credit,
whole-project cold replay or Windows product/runtime equivalence is claimed.

# Replay playback reads the native side runtime through a volatile reference

Original Japanese TH09 v1.50a `ReplayManagerView::BeginPlaybackStage` at
`0x420840` reads the stage header's +0x1E8 byte after publishing +0x1E7,
then accesses the real `g_GameManager.sides[0].runtime` pointer at the
original absolute address `0x4A7DAC`. An ordinary pointer read gets moved
before the value1E7 publication by pinned VC7.1. The maintained source now
uses a natural typed volatile reference to the actual native side runtime
pointer member, rather than reinterpreting its address as a second alias type.

The changed source expression is `AsciiGameSideRuntimeView *volatile
&side0RuntimeSlot = g_GameManager.sides[0].runtime`, followed by a local
pointer snapshot. Its current pinned VC7.1 compilation retains the entire
nonexact **538-byte candidate against the 540-byte original**, **32/32
original-bound fields**, all five ordered call destinations and the same
12-block direct graph. The **39 unlike overlapping bytes and two missing
bytes are unchanged**. `SaveReplay` likewise retains its separate 1,238-byte,
53-field candidate with only two unlike bytes.

Twelve alternative natural expressions for volatile/nonvolatile native
side pointers and the replay seed were locally compiled and checked as
full functions. Direct nonvolatile access regressed to 49 unlike bytes;
volatile value copies changed the body size; unsigned seed snapshots,
union and reference models optimized back to the same 538-byte candidate.
The original-safe typed reference is the only source simplification adopted.
The unchanged scoped ReplayInput source carriers retain 12 complete exact
functions / 4,093 bytes. All 22 replay-native tests pass. This is a source
ownership improvement to a **still nonexact** large function, not an exact
promotion or full cold replay.

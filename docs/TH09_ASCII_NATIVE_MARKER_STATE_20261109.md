# AsciiManager BossMarker state no longer uses a shadow object

The original TH09 v1.50a `AsciiManager::SetBossMarkerState` at 0x40F890
writes the state dword at `AsciiManager + 0x2254 + slot*4`. Its source
now uses `bossMarkerStates2254[slot]` directly instead of casting `this`
to an unrelated 0x2254-byte prefix struct. That array follows the four
native BossMarker `AnmVm` members added in the previous checkpoint.

The complete pinned VC7.1 body still matches **20/20 bytes with zero
relocations**. Neighboring native 32-byte interrupt and 42-byte position
methods, 251-byte Ascii reset, 253-byte Ascii update and 503-byte draw
impl remain original-exact. The Enemy boss still has 97 correct fields
and 13 unlike bytes. All selected owner tests pass. No cold replay.

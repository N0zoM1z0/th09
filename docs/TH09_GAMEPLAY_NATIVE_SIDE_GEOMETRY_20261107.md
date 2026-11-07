# TH09 Gameplay setup: playable and shared side geometry — 2026-11-07

Original-bound source reconstruction, against Japanese 1.50a TH09 with
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint incorporates previously uncommitted live-worktree changes;
it does not discard dirty work or reinterpret a local Git commit as an
accepted matching proof.

## One native +0x00..+0xA8 side allocation, two distinct roles

`AsciiGameManagerView` and replay represent exactly **two playable sides**,
each one physical 0x38-byte `AsciiGameManagerSideView`. In the separate
GameManager setup/cleanup allocation, the same side view occupies a physical
**third slot** after these two, just before the shared +0xA8 subsystem
pointer. The third record is not a third player: only its shared
`EffectManager *effectManager0C` at original setup-manager offset +0x7C
is used for the shared effect pool. The slot begins at +0x70; its flags
word is +0xA4. Native reset handles the flag words at +0x34/+0x6C/+0xA4,
while creation, input-focus gates and playable controllers only process
slots 0 and 1.

`GameManagerSetupLayout::SideRecordCount` now derives the physical setup
record count as the actual playable-array length plus one shared slot,
keeping `sizeof(GameManagerSetupLayout) == 0x390`. The separate
`GameplaySetupNativeSides` C++ view derives `PlayableCount`, `PhysicalCount`,
`SharedEffectIndex`, and `InputFocusCount` from real source structures, with
compile-time checks against the original +0x70, +0x7C and +0xA8 boundaries.
The big setup thread and its exact OnUpdate, CleanupGameplayState and
DeletedCallback siblings use these typed counts for actual loops and shared
EffectManager creation, release and reset; the third slot is never included
in playable-side loops. Type names and original TU ownership remain open.

## Original-target selective verification

All **12 exact functions / 3,987 complete physical bytes / 335 relocations**
across **three** affected native GameManager and Supervisor VC7.1 compiler
carriers pass fresh scoped matching (both GameManager source profiles).
An independent boss diagnostic confirms the actual **1,689-byte gameplay
setup worker** still generates **1,682 bytes** with all **172 canonical
relocation referents**, **35 calls**, **694 differing overlapping bytes** and
**seven missing bytes**. The 425-byte cleanup boss/leaf stays exact at
all 43 fields. Neither is falsely promoted to new exact credit.

**61 focused GameManager/gameplay tests** pass (3 new native sides tests,
43 existing GameManager tests, 15 gameplay setup tests). The source test
checks the typed two-plus-one relation and asserts no third-player loop;
these structural assertions do not substitute for original PE/COFF equality.

No unrelated source carriers were cold-built, no full cold replay was run,
no whole-image or runtime validation was claimed and the aggregate ledger
remains 1,034 source-present / 1,021 exact / 13 nonexact.

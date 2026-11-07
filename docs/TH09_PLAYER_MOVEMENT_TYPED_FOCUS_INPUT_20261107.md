# TH09 movement: one typed replay-input array for direction and focus

Original target: Japanese TH09 v1.50a, independently checked SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint changes the complete **1,900-byte Player movement boss** and
keeps its existing 122 non-field byte differences. It does not claim a newly
exact original or unique historic C++ class spelling.

## Native Player side and input state

The real `ReplayInputState g_ReplayInputStates[3]` has a **0x8E-byte stride**
and a genuine unsigned-short `auxiliary2A` at +0x2A, as independently
established by its original input methods and other Replay/Controller owners.
The full Player movement owner first snapshots native `player->sideIndex`
and then calls multiple `ReplayInputState::IsHeld` methods for directional
input. The focus-mode branch reads `auxiliary2A` from that **same side**.

Earlier source needed an unrelated integer `inputOffset = inputSide *
sizeof(ReplayInputState)` and a `reinterpret_cast` through an unsigned-byte
base just to arrive at that same input slot. The source now uses ordinary
`g_ReplayInputStates[inputSide].auxiliary2A`, without a redundant byte
calculation or second C++ view. The existing native input pointer is still
used for directional `IsHeld` calls; no extra memory allocation, bounds
clamp, signedness reinterpretation or gameplay branch is introduced.

## Full original-bound comparison and negative controls

Eleven fresh natural C++ movement variants were compiled under the same
pinned VC7.1 `/O2 /Ob1 /EHsc` profile, with complete original PE and COFF
comparison. The adopted `g_ReplayInputStates[inputSide]` read and removal of
the redundant offset produce **the identical 1,900-byte compiled body**,
**66 real fields, 23 ordered calls, both original switch tables, and the
same 122 different ordinary byte positions** as the committed baseline.
Three other natural pointer/reference/index expressions also retained the
same machine code. Other candidates were rejected: reindexing from the
byte offset grows the physical owner to 1,916 bytes and changes the field
identities; replacing the position float-pointer conversion with direct
member access loses an exact original call and produces only 1,884 bytes.
No register-selection trick, inline assembly, manual byte padding or
retargeting was adopted.

`check-boss-frontiers.py --owner movement` source-cold compiles the entire
nonexact movement source and separately verifies its **522-byte exact
Player OnUpdate caller / 40 fields** and **18-byte exact
`AnmVm::SetInterrupt` leaf**, for **two complete exact functions, 540
bytes and 40 fields**. The direct `ReplayInputState::IsHeld` source unit
also retains its unchanged original exact identity. Focused movement
input tests were updated to check the actual field and side relationship,
not demand the obsolete untyped pointer expression. All 44 scoped
movement tests pass.

Private copy-on-write recipes and source diagnostics are in
`.analysis/gpt-web-movement-natural-nov7/` and
`.analysis/gpt-web-movement-native-focus-20261107/`.
No all-project cold replay, complete Windows game link, runtime Oracle,
Factory acceptance or remote push is claimed. Global ledger remains
**1,034 source-present / 1,021 exact / 13 nonexact**.

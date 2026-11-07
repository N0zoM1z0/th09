# TH09 FrontMessage: real GameManager sides and Background stage-label opcode

Original target: Japanese TH09 v1.50a, independently verified executable
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a natural C++ storage and instruction-semantics recovery, **not**
a new byte-exact source owner, runtime compatibility proof, or historically
unique class-name inference.

## Revisit the message interpreter's GameManager/side-state hypothesis

`FrontMessageRuntimeView::Update` at 0x00416590 is an original-bound
**2,852-byte source function plus a 116-byte physical script-switch tail**.
Previous source checkpoints recovered the real Player at side +0x04 and
the real FrontSide HUD at side +0x18, but left a second
`FrontMessageGameManagerView`, two 0x38-byte `FrontMessageSideView` records,
and a separate `FrontMessageSideStateView *state00` at side +0x00.

**The old `state00` / `value18` interpretation was wrong as an object
owner.** The native `AsciiGameManagerSideView` already identifies side
+0x00 as a `Background *`; the independently exact Background's
`pendingStageLabel18` lives at Background +0x18, and
`Background::ApplyPendingStageLabel` reads this exact field to locate a
stage-label instruction. Thus opcode **`0x1C`** in the full FrontMessage
script writes the pending stage label on each side's **real Background**,
not on an invented side-score/phase object. Both writes have the same
original address arithmetic and original instruction semantics.

`FrontMessageUpdate.cpp` now includes the complete
`GameManagerRuntimeView.hpp` and `Background.hpp`, uses the true
`AsciiGameManagerView g_GameManager`, and consumes the native
`AsciiGameManagerSideView` side records and exact GameManager methods
`IsGameMode0/1/2`. Compile-time assertions pin one 0x390-byte native
GameManager, two 0x38-byte playable sides, Background +0x00, Player
+0x04, FrontSide +0x18, shot type +0x20, GameManager gameMode +0x118,
Background pending stage label +0x18, and the unchanged native Player
and HUD member offsets. No extra Background, Player, front-side object,
constructor or allocation is introduced.

## Complete immutable original PE and real COFF proof

Before adoption, two independent natural-C++ fixed-profile VC7.1 models
with native Background member access produced the **identical 2,968-byte
full script owner and 167 COFF field offsets/kinds/addends** as the
prior exact source. An independent complete original-PE-bound proof then
audited all 167 source fields against actual original field operands:

- **37 `g_GameManager` global referents** retain the immutable original
  address 0x004A7D90 and all original field addends;
- three native `AsciiGameManagerView::IsGameMode0/1/2` calls retain
  original target VAs **0x0040E470, 0x00404970, 0x00415D30**;
- **30 compiler-private `$L`/`$run_scripts$` names** were rebound *only*
  after their real same-owner COFF symbol definition offsets, section IDs
  and original PE jump-table operands were independently verified.

The strict canonical source/physical comparator still reports
**2,852/2,852 exact function bytes and 2,968/2,968 physical bytes**, all
167 fields including the original jump data. The fixed-profile source-cold
closure against the same retail original also passed **22 existing exact
functions, 7,230 physical bytes and 344 relocation fields** across
FrontMessage, the three GameManager mode leaves, the 79-byte
`Background::ApplyPendingStageLabel` and independently exact FrontSide
owners. None of these source-present functions has been newly credited.

Older source-shape regression tests that required synthetic
`FrontMessageSideView` / `FrontMessageSideStateView` are revised to require
the actual native GameManagerSideState, Player and FrontSide members.
A new guard checks the opcode-0x1C Background field interpretation,
independent original mode-function targets and all 37 global referents.
Private fixed-profile experiments and immutable PE/COFF proof packets:
`.analysis/gpt-web-front-native-gamemgr-20261108/`.

No all-project cold replay, Windows game product link, gameplay acceptance,
Factory receipt or remote push is asserted. The complete source ledger
remains **1,034 source-present / 1,021 canonical exact / 13 nonexact**.

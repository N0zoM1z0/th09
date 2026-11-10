# TH09 current handoff

Start here, then read [AGENTS.md](../AGENTS.md), [workflow](RE_WORKFLOW.md)
and [Oracles](ORACLES.md). Current counts come from the canonical ledgers:
run `python3 -B scripts/report-reconstruction-status.py` or read the generated
[progress](PROGRESS.md). Useful findings and failed approaches are in the
[knowledge base](KNOWLEDGE_BASE.md).

## Current work

The earlier Title-source reconstruction checkpoint is `746b0e6d`. Title draw, calc
and MusicRoom use the complete native Title owner. Their already-exact
functions stay exact; Replay-menu drawing remains nonexact. Recent Player,
Front, Enemy/ECL and Ascii native-owner work likewise preserves existing
exact owners. Current ECL/Enemy native-word changes preserve both large
owner frontiers: the initial two-carrier proof covers 83 exact units; the
follow-up opcode-4 relative-jump change cold-replays 91 units in six actual
carriers with 299 checked same-section label rebindings. Reproduce the latter
with `python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunControl.inl`.
SaveReplay's native serialized-byte arithmetic now compiles with unchanged
1,238-byte/two-register residual. The linked BeginPlaybackStage row check also
uses the original native `0x20 + stage*0x28` address without out-of-range C++
row indexing; both bosses retain their original residuals. The affected
ReplayInput source cold-passes 12 exact siblings in two real carriers.
The four ECL raw operand views now use the original `+0x0C` variable-length
wire address arithmetic rather than indexing a zero-size C++ member. Complete
RunEcl and Enemy boss frontiers remain unchanged; the six affected compiler
carriers cold-pass 91 exact owners / 36,301 bytes / 1,783 relocations with
291 independently verified compiler-label rebindings.
Both ECL operand fragments now read the zero-length trailing wire data with
native unsigned address arithmetic. The combined change was cold-rebuilt in
six affected carriers and kept 91 exact owners/36,301 bytes/1,783 fields,
with all 311 changed private labels independently checked against original
addresses; the RunEcl boss remains nonexact at 30 ordinary differences.
All remaining direct zero-length ECL operand users are now routed through
native union-pointer views, including the polar and biased-movement leaves,
shared jump union copy and trail record. The `/Ob0` movement leaf still replays
516/516 original bytes, the `/Ob1` RunEcl retains its complete 15,564-byte
frontier, and 91 affected exact units cold-pass in six carriers. See the newest
knowledge-base section for the rejected out-of-line helper variant.
The exact `CompareOperands` branch target also uses native unsigned i386
addition for signed ECL jumps. A two-carrier, 83-unit selected replay retained
all original bytes/fields, and eight private labels were rechecked.
The `CompareOperands` jump-success path now adds signed wire displacements
using native i386 address words. The specific `EclManager.cpp` two-carrier
replay passes 83 existing exact functions / 29,856 bytes / 1,389 fields;
RunEcl and Enemy OnUpdate preserve their complete owner frontiers.
The laser float/int/color readers and packed shot/laser packet casts now
address the native variable-length ECL payload instead of a zero-size C++
array. Five changed carriers cold-pass 87 exact units / 30,583 physical bytes
/ 1,409 fields, with the 516-byte biased-movement leaf still exact. Both big
ECL/Enemy owner frontiers remain unchanged.
Laser and shot ECL wire payloads now use native unsigned i386 operand
addresses in `EclRunBullet.inl`/`EclPostRuntime.inl`. The five affected
carrier profiles pass all 87 exact owners, 30,583 bytes and 1,409 relocation
fields; the full RunEcl/Enemy owner frontiers are unchanged.
Replay `LoadReplayData` now reads its complete packed file using native
32-bit byte addresses for codec/checksum walks, stream concatenation and
frame/FPS offset fixups. The exact 591-byte leaf and both nonexact Replay
owners retain their complete target-bound frontiers; two affected carriers
cold-pass 12 exact functions / 4,093 bytes / 232 relocation fields.
Git records the individual source changes.

Native Windows i386 product closure is still open. Exact-source functions,
origin-unclassified constructor equivalents, runtime validation and
portability are separate facts.

## Reconstruction frontiers

All 13 entries below are **nonexact** complete physical owners, not exact
credit. Offsets, calls, fields and residuals are starting evidence from
focused original-bound checks; refresh the chosen owner with the actual
pinned compiler when you make a source change. Use only its focused
`--owner` diagnostic, **never** `--owner all` as routine work.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | 15,564 physical bytes, 598 original fields, **30** ordinary unlike | Opcodes 4 (timer transport, 7 bytes) and 155 (flag write, 23 bytes); preserve complete owned jump tables. `--owner runecl`. Source `src/EclManager.cpp`. |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900 physical, 97 fields, 69 calls, **13** unlike | Draw-index register and descriptor position/threshold transport. Current source uses actual GameManager Etama side. `--owner enemy`. Source `src/EnemyManagerCore.inl`. |
| **Player movement** | 0x0041C170 | 1,900/1,900, 66 fields, 23 calls, **122** unlike | Loop/index register scheduling; retain authentic native option/position arrays. `--owner movement`. Source `src/PlayerMovement.cpp`. |
| **Player ChargeAttack** | 0x0041F810 | **1,210/1,210**, 70 fields, 58 calls, **235** unlike, 0 missing; 350 instructions, 53 direct blocks, matching CFG | Mode byte [EBP-1] and input receiver register allocation; complete extent has already been recovered. `--owner charge`. |
| **Title OnUpdateResult** | 0x004266B5 | 485 original / 484 candidate, 34 fields, 9 calls; 218 unlike overlaps + 1 missing | Character/score snapshot and result branch transport. `--owner resultdraw`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280, 57 fields, 13 calls, **29** unlike | Selected ReplayData register chronology; pure Float3 math and early index snapshots regressed. `--owner replaymenu`. |
| **GameplaySetupThread** | 0x0041AF2D | 1,689 original / 1,682 candidate, 172 fields, 35 calls, 694 unlike overlaps + 7 missing | Gameplay rate and side/rank ordering. `--owner gamemgr`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | 862 original / 849 candidate, 26 fields, 12 calls, 799 unlike overlaps + 13 missing | Actual collision region allocation and branch chronology. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238, 53 fields, 23 calls, **2** unlike bytes | Original EDI/ECX destination-register selection at +0xB7/+0xBB. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | 540 original / 538 candidate, 32 fields, 5 calls, 39 unlike overlaps + 2 missing | Seed/tail chronology. `--owner replay`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336, 20 fields, 8 calls, **26** unlike | ESI/EDI receiver lifetime and fade transport. `--owner screen`. |
| **Supervisor network** | 0x00430AA0 | 1,633 original / 1,627 candidate, 106 fields; 1,381 unlike overlaps + 6 missing | Queue/input transport and stack lifetime. `--owner network`. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51, 4 fields, 2 calls, **8** unlike | Receiver register choice. `--owner effect`. |


## Resume a bounded owner

```bash
git status --short --branch
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

# Recreate an exact unit's generated object, then compare its complete extent.
nice -n 12 python3 -B scripts/build-match-unit.py --unit title-screen-music-room
python3 -B scripts/compare-coff-function.py --unit title-screen-music-room --json

# Select one unfinished owner and its complete diagnostic.
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
```

When a source/header changes, also replay its affected exact siblings and
callees. Use `scripts/replay-scoped-exact.py --changed-file PATH --dry-run`
to inspect the scope. See [scoped replay](TH09_SCOPED_EXACT_REPLAY.md).
Do not use `--owner all` for routine iteration.

## Local inputs and scratch

Keep `resources/th09.exe`, `.tools/` and the two saved projects under
`.analysis/ghidra-direct/{1fd4b11bcd723dd1,243bffea8dcbe99a}/`.
The shared tool/runtime configuration is described in [SHARED_TOOLS.md](SHARED_TOOLS.md).
These private inputs do not travel with a Git clone.

Historical experiment outputs and local archives were removed under the
operator's simplified retention policy. There is no pending source candidate
or unresolved claim that requires an old compiler object/context. Rebuild
selected units from maintained source and the pinned profiles. `build/` may
be absent. Keep future scratch only while useful; record the conclusion and
remove obsolete output without making another archive or cleanup report.

## Routine checks

Run `python3 -B scripts/ci.py` for the fast basic public checks (about one
second locally). Tool changes also need relevant focused tests; source changes
still need their complete affected original-bound comparison. The complete
unit suite is available through `--full` and is optional, including for CI edits.

The stale native-owner source snapshots from the previous full-suite failure
were retired, the still-useful Enemy/Title probe fixtures were corrected, and
the affected test modules pass. Complete PE/COFF comparators, match recipes and
exact ledgers are unchanged.

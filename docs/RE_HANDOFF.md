# TH09 reconstruction handoff

This file is a **current-state restart note**, not an investigation journal.
Historical experiments, rejected hypotheses, packet-local byte counts, and old
candidate sizes belong in Git history and the historical sections of
`docs/KNOWLEDGE_BASE.md`.

If this file conflicts with `config/functions.csv`, `config/matches.csv`,
`config/match-units.toml`, or a fresh repository diagnostic, the ledgers and
fresh diagnostic win. Rerun the status commands before starting target-dependent
work; none of the numeric snapshots below are timeless facts.

## Current ledger snapshot

Fresh `python3 scripts/report-reconstruction-status.py` at this checkpoint:

| Measure | Count |
| --- | ---: |
| Function candidates | 2,192 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 980 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 980 |
| Canonical exact functions | 913 |
| Source-present non-exact functions | 67 |
| Source-present non-exact bytes | 68,673 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 207,097 |

All 980 confirmed authored functions have maintained source. Faithful whole
Windows i386 product closure remains open; semantic reconstruction and
portability have not started. `docs/PROGRESS.md` is generated from the ledgers
and is the preferred compact status snapshot.

## Closed items that must not stay on the roadmap

The following formerly-large frontiers are already canonical exact and should
not be reopened merely because an older packet or roadmap says they are pending:

- `EtamaController::OnUpdate @ 0x004146F0` — 2,195 authored bytes.
- `TitleScreenView::TitleSetupThread @ 0x004249E1` — 534 bytes.
- `TitleScreenView::OnUpdateMusicRoom @ 0x00426E05` — 2,258 bytes.
- `EnemyManagerView::AddedCallback @ 0x00411DB0` — 372 bytes.
- `ExAttackUpdateCallbackType20 @ 0x0044ADE0` — 405 bytes.

Their current exactness comes from `config/matches.csv` plus their configured
match units, not from historical prose.

## Priority frontier 1: EclManager::RunEcl

`EclManager::RunEcl @ 0x004086C0` remains source-present and **NON-EXACT**.

A fresh current-source rebuild during this cleanup produced RunEcl raw function
SHA-256
`93206b3996d97e62d6470b795e32c9cfdc9228a9bc8806efed073688b7420eef`.
`scripts/report-ecl-codegen.py` reports:

- 14,792 candidate logical code bytes, matching the target logical extent;
- 15,564 candidate physical code/table bytes, matching the target physical extent;
- stack frame `0x168`;
- 375 direct calls;
- 598 relocations;
- 193 compiler-table entries (6 easing + 187 opcode entries);
- final status: **NON-EXACT**.

The same cold object still replays
`Th09EclRunControl::PopContext @ 0x00406680` exactly at 147/147 bytes.
Structural size, frame, table, call-count, and relocation-count agreement are
diagnostics only and do not grant whole-owner exactness.

Fresh handler-table comparison narrows the current physical frontier further.
scripts/report-ecl-handler-boundaries.py compares the verified target opcode
table with the current COFF compiler table and reports 167/187 opcode entry
offsets exact. All 20 mismatching entries are exactly -1. Opcode 155 itself
starts at the correct +0x30B0, while opcode 156 starts at candidate +0x30DC
versus target +0x30DD, isolating one missing byte inside opcode 155. Target
uses ECX for the shifted timeout-spell input and EAX for the merged flag value;
the candidate uses EAX/ECX respectively, so target emits the six-byte generic
AND ECX,0x01000000 while the candidate emits the five-byte EAX-special
encoding. The physical tail is correspondingly shifted by one byte and the
candidate ends with a compensating NOP.

This is an allocator/TU frontier, not a license to force registers. Bounded
natural probes of the opcode-155 expression (direct bitfield, helper/mask
forms, signed/unsigned locals, pointer/reference state, function-scope scratch
reuse, and the adjacent TH08-style spelling) all reproduce the same 44-byte
candidate handler. Reverting the recent instruction-time context spelling and
opcode-2 source shape also leaves opcode 155 unchanged. Do **not** repeat those
controls without new cross-case/TU evidence.

Useful restart commands:

~~~bash
python3 scripts/build-match-unit.py --unit ecl-pop-context
python3 scripts/compare-coff-function.py --unit ecl-pop-context --json
python3 scripts/report-ecl-codegen.py --json build/matching/EclManager.obj
python3 scripts/report-ecl-handler-boundaries.py build/matching/EclManager.obj
python3 scripts/audit-ecl-callsite-identities.py build/matching/EclManager.obj
~~~

The callsite audit is a structural diagnostic and not an exactness Oracle.

## Priority frontier 2: ExAttack type 6

`ExAttackUpdateCallbackType6 @ 0x00446060` remains **NON-EXACT**.

The maintained source is target-sized at 698/698 bytes with 214 instructions,
30 independently bound relocations, 19 ordered calls, 18 direct blocks, and the
same complete direct CFG. The current diagnostic has seven complete byte
differences, all in the collision sequence's volatile-register coloring:

- opponent-side index path: target ECX vs candidate EAX;
- `collisionSize` address: target EAX vs candidate EDX;
- collision-success rotation copy: target EDX vs candidate EAX.

The continuation after that collision branch matches under the current complete
diagnostic. Previous direct-receiver and alias/lifetime variants are bounded
negative evidence, not prohibitions on better natural source. Do not force
registers, add volatile steering, padding, assembly, fake ABI changes, or
profile roulette.

`ExAttackInitializeCallbackType6 @ 0x00445EC0` is independently non-exact and
should not be conflated with the update callback.

## Priority frontier 3: ExAttack type 18/24

`ExAttackUpdateCallbackType18_24 @ 0x004491E0` remains **NON-EXACT**.

The maintained target-correct source is 1,442 bytes against the 1,436-byte
target. Fresh target review proved that the older 1,435-byte candidate was
misleadingly close because it tail-merged a state-2 completion path that the
target keeps as a distinct early return-one epilogue. Keep the corrected case
ordering and direct `extra->angle4C` use unless new target evidence contradicts
them.

The current hard seam is local lifetime/allocation: target collision size is at
`[ebp-0x24]`, three distinct Float3 history-delta slots are
`-0x30/-0x3C/-0x48`, and collision point is `-0x18`. Same-TU Float3
visibility can move some slots but coalesces the three deltas, so it is
diagnostic only.

## Other large owners

After the three frontiers above, select the next owner from the live ledgers
rather than an old roadmap. At this checkpoint the largest source-present
non-exact owners include:

- `EnemyManagerView::OnUpdate` — 3,883 bytes;
- `TitleScreenView::OnUpdateOptions` — 2,045 bytes;
- `TitleScreenView::DrawResult` — 1,939 bytes;
- `PlayerLifecycleView::UpdateMovementAndOptions` — 1,835 bytes;
- `EnemyManagerDrawImpl` — 1,758 bytes;
- `PauseMenu::OnUpdate` — 1,734 bytes;
- `GameplaySetupThread` — 1,689 bytes;
- `SupervisorServiceUpdate` — 1,633 bytes;
- `FrontCalcCallback` — 1,491 bytes;
- `ReplayManagerView::SaveReplay` — 1,238 bytes;
- `GameManagerSetupLayout::OnUpdate` — 1,230 bytes.

This list is routing information only. Recompute it from `config/functions.csv`
and `config/matches.csv` before treating it as exhaustive.

For short functions, use `docs/SMALL_FUNCTION_FRONTIER.md`; its current filter
contains 11 non-exact authored/source-present functions at or below 256 bytes.
Large owners and leaf functions should both continue to receive serious work.

## Session restart and checkpoint rules

Follow `AGENTS.md` first. The minimum repository-side preflight is:

~~~bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
~~~

From GPT-web, attest the registered `th09-ida` provider separately as described
in `AGENTS.md`; IDA remains provisional semantic evidence, not an exactness
Oracle.

After a coherent source/ledger batch:

~~~bash
python3 scripts/validate-tracking.py --require-target
python3 scripts/progress.py
python3 scripts/ci.py
git diff --check
git status --short --branch
~~~

Run focused canonical replays for every affected exact unit. Exactness credit is
accepted only by the target-bound replay/acceptance path; a local commit is a
checkpoint, not proof.

Use commit messages of the form `gpt-web: ...`. Do not push.

## Documentation hygiene

- Replace this handoff in place; do not append packet journals to it.
- Current exact/non-exact state comes from the ledgers, not packet prose.
- `docs/KNOWLEDGE_BASE.md` is durable evidence/history. Its recorded state
  labels and packet-local "current/remains" wording are historical unless a
  live ledger independently agrees.
- Prefer commands and durable structural facts over transient whole-owner byte
  counts in long-lived documentation.
- When a function is promoted to exact, remove it from active frontier lists in
  the same checkpoint.
- Keep `.analysis/` only for unresolved evidence that is still useful and not
  reproducible from tracked source/config; stale status snapshots should be
  regenerated or removed.

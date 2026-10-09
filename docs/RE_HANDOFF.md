# TH09 reconstruction handoff

## Active objective

The user explicitly resumed work on **2026-10-09**: reach **95% exact reviewed
authored bytes**, work seriously on large functions, use direct IDA Pro MCP
and local Bash/compiler Oracles without Factory MCP, commit locally as
`gpt-6.1-sol: ...`, and periodically clean reproducible intermediates. Do not push.
The goal remains **active-incomplete**. No new exactness credit was obtained in
this checkpoint. The latest source repair reconnects PauseMenu state9 to the
real Supervisor::StopAudio boundary; new continuation models remain rejected. The October 7 stopped-state checkpoint is preserved in Git at
7347cb3; its cleanup and evidence limits are historical, not current stop orders.

## Live ledger snapshot

These totals are checked against the live ledgers by scripts/validate-docs.py.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,192 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 980 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 980 |
| Canonical exact functions | 928 |
| Source-present non-exact functions | 52 |
| Source-present non-exact bytes | 57,269 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 218,501 |

The fixed reviewed-authored denominator is **275,770 bytes**. Exact coverage is
**79.2331%**; reaching 95% requires another **43,481 bytes**.
The 35 unresolved origins are separate. These numbers do not measure the entire
executable or establish a complete game. The faithful Windows i386 product and
runtime gates remain open; semantic reconstruction and portability have not
started. No new exactness credit or acceptance receipt is claimed by this checkpoint.

## Active frontiers

- RunEcl: 14,792 authored /15,564 physical bytes. Complete replay still has
  2,161 differences, with handler frontiers 4/7/86/155/156/157. Use
  `scripts/inspect-ecl-complete.py`; normalized shape agreement is diagnostic.
- Enemy OnUpdate: 3,883 authored /3,900 physical bytes, 97 independently bound
  fields and 39 full differences. Four residual windows concern early draw
  index, special descriptor/effect scheduling, trail copy and homing registers.
- Player movement: 1,835 authored /1,900 physical bytes, 66 fields and 144 full
  differences. The maintained inspector now reports complete physical bytes and
  independently decoded operands/table entries.
- ExAttack type18/type24: 1,436 target bytes; corrected candidate 1,441/44 fields.
  All 31 ordered calls and 23 direct blocks agree; full replay still has 1,051
  overlap differences plus five excess bytes. Target stack homes remain open.
- DirectPlay message handler: 768 authored /808 physical bytes, 46 fields and
  125 complete differences. Its 34-block switch graph is independently checked.
  Word-call, helper visibility and value/loop contexts are rejected; ABI and
  maintained source stay unchanged.
- PauseMenu update: 1,734 authored /1,776 physical bytes; complete replay has
  1,492 overlap differences plus 44 missing bytes. Use inspect-pause-menu.py.
- Enemy draw: 1,758 bytes and four complete differences around the second
  subtraction/Abs argument. Other nonexact UI, gameplay, network and callbacks
  remain in config/functions.csv. Consult docs/KNOWLEDGE_BASE.md before probes.

## Latest reviewed investigation

PauseMenu state9 now calls Supervisor::StopAudio @0x42FD30 through its real
maintained declaration, replacing the unimplemented private PrepareResultScreen
alias. Target 0x434DDC supplies g_Supervisor, no stack arguments and ignores the
return. State11/menu-disable/timestamp publications remain ordered. Two
canonical-path cold objects and one isolated repair agree on all nondebug
sections and field identities. Eight state4 private names refresh only after
actual same-section destinations and complete 1172-byte equality pass.
Eleven same-TU exact functions retain 2448 physical bytes/158 fields;
StopAudio replays 81/81. This dependency repair adds no exact credit.

New scripts/inspect-pause-menu.py independently decodes 106 target fields and
requires complete candidate operand/table coverage. Current PauseMenu is 1692
code/1732 physical bytes, 107 fields, 453 instructions versus target 1734/1776 and
449 instructions. Full replay finds 1492 overlap differences plus 44 missing.
Three early-footer/timestamp/whole-closing continuations are rejected; none is
integrated. Consult the knowledge base before reusing those exact contexts.

RunEcl was also reattested/replayed without compilation: its 2161 complete
differences and six-handler frontier are unchanged. This bounded review found
no fresh supported contradiction. The full 95% objective stays active; original
source/TU/native runtime ownership remains open.

## Evidence and artifact lifecycle

Entry at 4df3c91 is clean; private target, direct IDA metadata, entry and five
mapped-byte samples pass. No Factory MCP, IDA writes, target patch or delegation.
Current compact evidence is below
.analysis/gpt-6.1-sol-pause-continuations-20261009/: source recipes, actual
include logs, complete PE/COFF receipts, independent inventory and label proof.
Of 78 actual includes, eight have pre/post observations; seventy vendor inputs
have post-only observations. Historical environment is not retrospectively
attested.

All six caller compiles and the focused callee build are terminal. Cleanup
removes 31 current-session reproducible probes/objects/PDBs and duplicate or
superseded reports totaling 1,438,316 bytes. Canonical caches and inherited
evidence remain; previous paired-PDB cleanup is not rerun. The user requests
batching changes before cold replay: accumulate one coherent batch, use focused
probes between changes, then close affected Oracles together. Reuse unchanged
evidence.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/inspect-pause-menu.py
python3 scripts/compare-coff-function.py --unit ascii-menu-state4-update
python3 scripts/compare-coff-function.py --unit supervisor-stop-audio
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
python3 scripts/report-ecl-handler-shapes.py build/matching/EclManager.obj
```

Call direct IDA `get_metadata` with exactly `{}` during entry attestation.
ECL's complete inspector returns1 for the measured nonexact result; Player's
inspector exits successfully after a completed diagnostic. Neither compiles
or proves arbitrary supplied-object source provenance. New exact promotion
requires bound source/includes, a fresh pinned canonical build and complete
zero-difference replay.

Validation: eleven affected same-TU exact Oracles, StopAudio 81/81,
complete PauseMenu comparison, tracking,
progress and whitespace pass. Isolated CI runs 67 tests (65 pass, two optional
Capstone tests skipped). The worktree checkpoint remains local and is not pushed.

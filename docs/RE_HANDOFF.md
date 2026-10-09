# TH09 reconstruction handoff

## Active objective

The user explicitly resumed work on **2026-10-09**: reach **95% exact reviewed
authored bytes**, work seriously on large functions, use direct IDA Pro MCP
and local Bash/compiler Oracles without Factory MCP, commit locally as
`gpt-6.1-sol: ...`, and periodically clean reproducible intermediates. Do not push.
The goal remains **active-incomplete**. No new exactness credit was obtained in
this checkpoint. DrawReplayMenu now expresses the target-observed alternating
Float3 interpolation stages and recovers the target frame/buffer home. One
batch-end canonical compile reproduces the reviewed isolated body; complete
bytes and the missing caption-call structure remain nonexact. Prior Enemy draw
operand verification remains in the knowledge base and Git.
The October 7 stopped-state checkpoint is preserved in Git at
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

DrawReplayMenu owns 1280 bytes at 0x4234F6. Its interpolation alternates two real
Float3 value groups before the final position copy. The source now expresses
those four existing subtraction/frame/scaling/addition stages through scaled
and target. This recovers frame 0x134 and the actual 256-byte buffer home;
36 raw stack/frame bytes change, with all 54 relocation descriptors unchanged.
No buffer enlargement, extra arithmetic, helper or profile change is introduced.

Three isolated models were inspected before one canonical-path compile.
Direct indexed mode access emits 1250 bytes with 1181 overlap differences and 30
absent bytes. Per-case unsigned caption lengths retain counts but change nine
raw bytes, moving one strlen call and deferring cleanup. Neither is retained.
The Float3 stages are retained on target storage evidence; their canonical
raw SHA256 is 0fc1b7fd16ddc3ef4ceda4fb8cc0a2881b621171cedaee108ef5ba89026a533b.

Complete comparison still gives 1244/1280 bytes, 54 fields, 1180 overlap
differences plus 36 absent bytes,366/376 instructions,11/13 calls and71/72
blocks. All actual operands cover COFF fields independently; target has 57
separately decoded fields. Every collateral copy and nondebug data remains
unchanged. Those three copies are 37 bytes/two fields and have no target
ownership. No exact receipt or partial credit is granted. Call merging and
remaining stack/register allocation stay open.

## Evidence and artifact lifecycle

Entry at 145d1c6 is clean; private target, direct IDA metadata, entry and five
mapped-byte samples pass. No Factory MCP, target patch, IDA write or delegation.
Current packet: .analysis/gpt-6.1-sol-replay-mode-20261009/.
It retains patches, Git-bound reconstruct.py, source/include/backend manifests,
all full byte/operand reports and independent four-owner COFF cross-check.
Six actual includes per carrier have matching before/after hashes. The old
actual-Float3 baseline is reused with main/header/vendor/backend bindings;
its historical environment is not retrospectively attested.

All four compiles are terminal. Cleanup receipts remove 15 file events totaling
298,753 bytes, including recreated probe sources and canonical PDB. The small
source-bound canonical object, inherited baseline and compact proofs remain.
No unchanged baseline compile or inherited cleanup is repeated.

Batch coherent changes before cold replay; every new exact owner requires
complete pinned canonical source/byte/relocation proof. A same-size or same-count
trial is not byte neutrality: compare actual bodies and field descriptors.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 .analysis/gpt-6.1-sol-replay-mode-20261009/audit.py --canonical-only
python3 scripts/compare-coff-function.py build/gpt-6.1-sol-replay-mode-20261009/canonical.obj '?DrawReplayMenu@TitleScreenView@@QAEHXZ' 0x4234F6 1280
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
```

Call direct IDA get_metadata with exactly {} during entry attestation.
The generic menu Oracle returns1 for its expected 1244/1280 extent mismatch;
complete audit exits successfully after reporting all mismatch/absence bytes.
It compares the supplied canonical object to the retained reviewed input proof;
no new cold build is implied. Reconstruct the cleaned isolated sources through
the packet's Git-bound recipe before a fresh trial compile.

Validation: complete canonical owner/decoded operands and all collateral code,
tracking, progress, documentation and whitespace pass. Isolated CI runs 72 tests
(68 pass, four optional Capstone checks skipped). Worktree checkpoints remain
local and are not pushed. Native product/runtime and later phase gates remain open.

# TH09 reconstruction handoff

## Active objective

The user explicitly resumed work on **2026-10-09**: reach **95% exact reviewed
authored bytes**, work seriously on large functions, use direct IDA Pro MCP
and local Bash/compiler Oracles without Factory MCP, commit locally as
`gpt-6.1-sol: ...`, and periodically clean reproducible intermediates. Do not push.
The goal remains **active-incomplete**. No new exactness credit was obtained in
this checkpoint. The latest batch reviews complete DirectPlay call/value
hypotheses; maintained game source is unchanged. The October 7 stopped-state checkpoint is preserved in Git at
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
- Enemy draw: 1,758 bytes and four complete differences around the second
  subtraction/Abs argument. Other nonexact UI, gameplay, network and callbacks
  remain in config/functions.csv. Consult docs/KNOWLEDGE_BASE.md before probes.

## Latest reviewed investigation

The DirectPlay message handler remains 768 code /808 physical bytes, 234
instructions and 46 fields against target 235 instructions. All 17 direct calls
and two GetPeerInfo vtable-slot 0x54 calls agree, as does its complete 34-block
graph including the six switch destinations and sixteen-case byte map. The
full linked comparison has 125 differences. Current source and all actually
included project files match the retained October 6 baseline; no unchanged
cold baseline compile was repeated.

Fresh queue-callee review establishes low-word packed-input use, not unique
original C++ signedness or a wrong maintained int API. Fifteen isolated
compiles test natural word-call views, consumed values, actual RNG visibility,
prefill control and packet dispatch. Word/completion restores one argument
window but retains 108 differences and loses the loop-entry jump. Helper
visibility gives 242 differences; prefill do gives 113; packet switch retaining
ACK exit gives 820 bytes with wrong call order. Neutral snapshots/copy/signedness
are recorded precisely in the knowledge base; do not repeat these contexts.
No shared declaration, native transport, wire layout, source, profile, exact
ledger or denominator changes.

The latest maintained game-source correction remains d465250's type18/type24
reflection/Float3 repair. Original source/TU/native runtime ownership remains
open. Correct graph or partial argument agreement receives no byte credit.

## Evidence and artifact lifecycle

Entry at d465250 is clean. Disk identity, direct IDA metadata, entry point and
five distributed mapped-byte samples pass. No Factory MCP, IDA write, target
patch or delegation is used. Current compact evidence is below
`.analysis/gpt-6.1-sol-network-values-20261009/`: complete owner/field/switch
comparison, actual includes, source patches, pre/post observations and a
separate COFF inventory. All 1283 pre-observed inputs are unchanged; the final
receipt retains only actual includes/backends. Eighty included files have
pre/post observations; five have post-only observations. Historical source
hashes do not retrospectively attest the historical compiler/environment.

Current producers are terminal. Reproducible current-batch sources, objects,
PDBs and redundant reports totaling 2,033,585 bytes (71 files) are removed
after complete records are retained.
Inherited evidence and canonical caches remain. The previous paired-PDB
cleanup receipt remains below
`.analysis/gpt-6.1-sol-pdb-cleanup-20261009/` and is not rerun.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/compare-coff-function.py --unit supervisor-network-message-thunk
python3 .analysis/gpt-6.1-sol-network-values-20261009/inspect_owner.py baseline=build/gpt-dots-network-dispatch-20261006/baseline.obj
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
python3 scripts/report-ecl-handler-shapes.py build/matching/EclManager.obj
```

Call direct IDA `get_metadata` with exactly `{}` during entry attestation.
ECL's complete inspector returns1 for the measured nonexact result; Player's
inspector exits successfully after a completed diagnostic. Neither compiles
or proves arbitrary supplied-object source provenance. New exact promotion
requires bound source/includes, a fresh pinned canonical build and complete
zero-difference replay.

Validation: focused canonical message thunk 15/15, complete DirectPlay
comparison, tracking,
progress and whitespace pass. Isolated CI runs 67 tests (65 pass, two optional
Capstone tests skipped). The worktree checkpoint remains local and is not pushed.

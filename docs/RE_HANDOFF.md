# TH09 reconstruction handoff

## Active objective

The user explicitly resumed work on **2026-10-09**: reach **95% exact reviewed
authored bytes**, work seriously on large functions, use direct IDA Pro MCP
and local Bash/compiler Oracles without Factory MCP, commit locally as
`gpt-6.1-sol: ...`, and periodically clean reproducible intermediates. Do not push.
The goal remains **active-incomplete**. No new exactness credit was obtained in
this checkpoint; the latest source correction concerns an Enemy Player view. The October 7 stopped-state checkpoint is preserved in Git at
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
- Enemy draw: 1,758 bytes and four complete differences around the second
  subtraction/Abs argument. Other nonexact UI, gameplay, network and callbacks
  remain in config/functions.csv. Consult docs/KNOWLEDGE_BASE.md before probes.

## Latest reviewed source change

The private Enemy Player view now declares Player+0x364 as `Effect *focusEffect364`
and checks its offset. Direct TH09 movement stores/clears the returned focus
effect there; Enemy update only checks whether it is null before updating the
existing side timer. The former integer-state name/type was unsupported.

Isolated and canonical-path builds preserve all five emitted bodies /4,060
physical bytes /99 fields and all seven nondebug noncode sections. Enemy
OnUpdate retains all 97 independently bound target fields and the same 39
differences; its raw hash is
`51d8510ea19461e0bef2f658c616277805ea138ba431be646c10b158053a1bc6`.
Canonical attached-effect helper still replays 151/151. No exact ledger row,
profile, ABI, reviewed extent or denominator is changed. Native product/runtime
and later semantic/port gates remain open.

Two new controls are rejected: a consumed descriptor-speed local is fully
neutral; conditional taper assignment regresses to 698 complete differences.
Earlier playfield/Pause/Enemy draw/ECL controls are recorded in the knowledge
base. These are context-bound negatives, not impossibility proofs.

## Evidence and artifact lifecycle

Entry at 8f83ac3 was clean. Disk identity, direct IDA metadata, entry point and
five distributed mapped-byte samples pass. No Factory MCP, IDA write, target
patch or agent delegation is used. Four compiles are terminal; current-batch
probe sources, objects and PDBs totaling 376,339 bytes are removed. Compact
source deltas, actual include logs, hashes and complete comparisons remain in
`.analysis/gpt-6.1-sol-enemy-update-values-20261009/`.

A separate audit removes 1,678 noncanonical compiler PDBs /151,584,768 bytes
(144.56 MiB). Each has a retained i386 COFF object whose actual type-server
GUID/age and file path match the PDB info stream. Object hashes are verified
before and after deletion; no object is removed. Canonical PDBs, unpaired or
symlink cases, and a path mismatch remain. No pre-existing PDB comparison consumer was found in
tracked/analysis tooling; the cleanup audit reads identity metadata. The receipt and paired hashes remain below
`.analysis/gpt-6.1-sol-pdb-cleanup-20261009/`. This audited cleanup does not
remove the whole inherited analysis/build trees, private target or toolchains.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/compare-coff-function.py --unit enemy-attached-effect-update
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
python3 scripts/report-ecl-handler-shapes.py build/matching/EclManager.obj
```

Call direct IDA `get_metadata` with exactly `{}` during entry attestation.
ECL's complete inspector returns1 for the measured nonexact result; Player's
inspector exits successfully after a completed diagnostic. Neither compiles
or proves arbitrary supplied-object source provenance. New exact promotion
requires bound source/includes, a fresh pinned canonical build and complete
zero-difference replay.

Validation: focused canonical helper, complete Enemy comparison, tracking,
progress and whitespace pass. Isolated CI runs67 tests (65 pass, two optional
Capstone tests skipped); the eight focused Player diagnostic tests previously
pass with Capstone. The worktree checkpoint remains local and is not pushed.

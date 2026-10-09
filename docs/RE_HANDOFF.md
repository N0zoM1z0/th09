# TH09 reconstruction handoff

## Active objective

The user explicitly resumed work on **2026-10-09**: reach **95% exact reviewed
authored bytes**, work seriously on large functions, use direct IDA Pro MCP
and local Bash/compiler Oracles without Factory MCP, commit locally as
`gpt-6.1-sol: ...`, and periodically clean reproducible intermediates. Do not push.
The goal remains **active-incomplete**. No new exactness credit was obtained in
this checkpoint; the latest correction concerns ExAttack type18/type24 reflection. The October 7 stopped-state checkpoint is preserved in Git at
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
- Enemy draw: 1,758 bytes and four complete differences around the second
  subtraction/Abs argument. Other nonexact UI, gameplay, network and callbacks
  remain in config/functions.csv. Consult docs/KNOWLEDGE_BASE.md before probes.

## Latest reviewed source change

Direct TH09 review corrects the shared ExAttack type18/type24 reflection:
left and right tests are mutually exclusive, and pi-angle is stored to
extra+0x4C before the shared normalization call. The forward collision point
is now a real Float3 object. This supersedes Packet433's independent-if
requirement and Packet809's claim that only local allocation remained. The
normalizer reads arguments/constants without changing record/position; no
gameplay bug or native runtime validation is claimed.

Two independent canonical-path pinned cold builds agree on 1,441 bytes, 475
instructions and 44 records, raw hash
`3574f1fb7c085bba0f88d2d98ca9dfa5d1806cd0bba34af8fe1732f3a8f60dcb`.
All 31 ordered calls, field-identity occurrences and 23 direct CFG blocks agree.
The complete replay remains NON-EXACT:1,051 overlap differences plus five
excess bytes. Correct stores/control earn no partial byte credit. The local
Float3 emits an uncalled three-byte empty constructor with no target ownership;
five nondebug noncode sections preserve their bytes and zero relocations.

Nine isolated controls and two canonical compiles are terminal. Direct delta
expressions coalesce return slots; exclusive-only and staging-only reflection
remain wrong; const values/references on the corrected context are neutral.
Original stack/value/TU context and inherited private-view aliasing remain
open. Exact ledgers, profiles, ABI, extents and denominator stay unchanged.

## Evidence and artifact lifecycle

Entry at 5b1393a is clean. Disk identity, direct IDA metadata, entry point and
five distributed mapped-byte samples pass. No Factory MCP, IDA write, target
patch or delegation is used. Compact current evidence is below
`.analysis/gpt-6.1-sol-type1824-values-20261009/`: source deltas, actual includes,
source/header/backend hashes, complete independently decoded byte/field/CFG
comparisons and repeat/collateral inventory. The audit checks float identities
in the verified PE and rejects unknown fields; it compares all bytes even for
a different extent. Current-batch reproducible sources, objects and PDBs are
removed after the checks: 31 files /923,463 bytes. Inherited evidence and
canonical caches are retained.
The previous paired-PDB cleanup receipt remains below
`.analysis/gpt-6.1-sol-pdb-cleanup-20261009/` and is not rerun.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/compare-coff-function.py --unit exattack-type18-init
python3 scripts/compare-coff-function.py --unit exattack-type24-init
python3 scripts/compare-coff-function.py --unit runtime-add-normalize-angle
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
python3 scripts/report-ecl-handler-shapes.py build/matching/EclManager.obj
```

Call direct IDA `get_metadata` with exactly `{}` during entry attestation.
ECL's complete inspector returns1 for the measured nonexact result; Player's
inspector exits successfully after a completed diagnostic. Neither compiles
or proves arbitrary supplied-object source provenance. New exact promotion
requires bound source/includes, a fresh pinned canonical build and complete
zero-difference replay.

Validation: focused type18/type24 initializer and normalization Oracles,
complete ExAttack comparison, tracking,
progress and whitespace pass. Isolated CI runs 67 tests (65 pass, two optional
Capstone tests skipped). The worktree checkpoint remains local and is not pushed.

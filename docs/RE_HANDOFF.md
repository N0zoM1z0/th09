# TH09 reconstruction handoff

## Active objective

The user explicitly resumed work on **2026-10-09**: reach **95% exact reviewed
authored bytes**, work seriously on large functions, use direct IDA Pro MCP
and local Bash/compiler Oracles without Factory MCP, commit locally as
`gpt-6.1-sol: ...`, and periodically clean reproducible intermediates. Do not push.
The goal remains **active-incomplete**. No new exactness credit was obtained in
this checkpoint. Enemy draw's complete inspector now independently decodes
all operands, reports extent deficits/excess, checks every COFF field and names
supplied-object provenance limits. Two return/expression precision controls
regress and are rejected. Prior ECL controls and Enemy member repair remain
recorded in the knowledge base and Git.
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

Enemy draw's target interpolation helper at 0x40F5E0 returns its final multiply
and sum directly through ST0, with no float32 return store. Both an alternative
long-double helper return declaration/definition and a wide right-hand operand
in the second Abs subtraction were compiled as a batch. The helper retains all
91 bytes/two fields, but both callers regress to 657 complete differences at
1,758 bytes/53 fields. Their raw caller bodies are identical. No production
signature, expression or profile change is retained; original return precision
is still unknown. These differ from the prior left-side double/Abs-return trials.

scripts/inspect-enemy-draw.py now checks complete instruction decoding, actual
call/data/object-immediate operands against all COFF fields, target counts and
ordered direct call identities. The image-like TEST mask 0x400000 remains a
scalar. It reports missing/excess bytes and resolved/target extent hashes, and
explicitly does not bind supplied objects to source. This is a diagnostic,
not an acceptance gate or CFG correspondence proof. Optional Capstone is needed.

The bound baseline still decodes 495 instructions/53 fields/28 direct calls,
with four complete differences at +0x3F3..+0x3F6 and no missing/excess bytes.
All three collateral exact bodies retain 164 bytes/eight fields. Five targeted
guard tests pass, including extent mismatches, unmasked operand differences,
missing/duplicate/misbound COFF records, scalar masks and truncated decoding.

## Evidence and artifact lifecycle

Entry at 1a4c781 is clean; private target, direct IDA metadata, entry and five
mapped-byte samples pass. No Factory MCP, IDA writes, target patch or delegation.
Current packet: .analysis/gpt-6.1-sol-enemy-return-precision-20261009/.
It retains patches, reconstruct.py, input manifest, actual include logs, full
operand/byte reports and independent four-owner inventory. Eight actual includes
per carrier match preobserved inputs. Source/backend hashes match after builds.
The inherited conjunction baseline is source/dependency/object-hash matched;
its historical environment is not retrospectively attested.

Two compiles are terminal. Cleanup removes eight copied sources/objects/PDBs,
178,552 bytes. Reconstruction is hash-checked before cleanup and preserves the
terminal manifest. Bound baseline and canonical caches remain. No unchanged
canonical compile or inherited cleanup is repeated.

Accumulate coherent source changes before one batch-end cold replay. Reuse
unchanged source-bound evidence; every new exact owner still requires a pinned
canonical build and complete zero-difference byte/relocation proof.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/inspect-enemy-draw.py build/gpt-dots-enemy-draw-conjunction-20261006/EnemyManagerDraw.obj
python3 scripts/compare-coff-function.py --unit enemy-manager-draw-high-prio
python3 scripts/compare-coff-function.py --unit enemy-manager-draw-low-prio
python3 scripts/compare-coff-function.py --unit enemy-draw-interpolate-wrapped-angle
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
```

Call direct IDA get_metadata with exactly {} during entry attestation.
Enemy draw's inspector exits successfully for a completed nonexact diagnostic;
ECL's complete inspector returns1 for its measured mismatch. Neither compiles
or establishes supplied-object source provenance. Reconstruct the current
cleaned trial sources before rerunning audit.py and its compiler commands.

Validation: focused draw units 35/35,38/38,91/91; independent complete
four-owner/61-field inventory; five targeted tests; tracking, progress and
whitespace pass. Isolated CI runs72 tests (68 pass, four optional Capstone
checks skipped). Worktree checkpoints are local and not pushed.

# TH09 reconstruction handoff

## Active objective

The user resumed on **2026-10-09**: reach **95% exact reviewed-authored bytes**,
work on large functions, use direct IDA Pro MCP and local Bash/compiler Oracles
without Factory MCP, commit locally as `gpt-6.1-sol: ...`, and do not push.
Batch coherent trials before cold replay; reuse unchanged baselines only after
checking their source/backend/object bindings. The goal remains active-incomplete.
The October 7 stop at 7347cb3 is historical.

This checkpoint starts clean at 1a716e0. Target SHA, direct IDA metadata, entry
and five distributed mapped-byte samples pass. No target patch, IDA write or
delegation. Three RunEcl expression controls are completely neutral and rejected.
The codegen diagnostic now distinguishes actual code from compiler alignment;
no game source, ABI, profile, match row or exactness credit changes.

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

The fixed authored denominator is **275,770 bytes**: **79.2331%** exact.
Another **43,481 bytes** are needed for 95%. Origin-unresolved candidates are
separate. The faithful Windows i386 product/runtime gates remain open;
semantic reconstruction and portability have not started.

## Active frontiers

- RunEcl: 14,792 target code /15,564 physical bytes; current candidate has
  **14,791 code + one alignment + 772 compiler-table bytes**. All 598 fields
  are independently checked, with 2,161 complete differences. Handler frontiers
  remain 4/7/86/155/156/157. Equal pre-table sizes had hidden the code-size gap.
- Enemy OnUpdate: 3,883 authored /3,900 physical bytes, 97 fields, 39 full
  differences; early draw index, descriptor/effect scheduling, trail and homing.
- Player movement: 1,835 authored /1,900 physical bytes, 66 fields, 144 full
  differences. Actual Float3 declaration/body visibility controls are neutral.
- ExAttack type18/type24: 1,436 target bytes; corrected candidate 1,441/44 fields,
  all 31 calls and 23 direct blocks agree; 1,051 overlap differences plus five
  excess bytes. Target stack homes remain open.
- DirectPlay message handler: 768 authored /808 physical bytes, 46 fields,
  125 complete differences, complete 34-block switch graph checked.
- PauseMenu update: 1,734 authored /1,776 physical bytes; 1,492 overlap
  differences plus 44 absent bytes. Use inspect-pause-menu.py.
- Enemy draw: 1,758 bytes, four differences around the second subtraction/Abs.
- Type21 update: 1,074 bytes, 29 fields, 15 ring-preheader differences. Real
  descriptor-member storage is maintained; two record-owned acquisitions are neutral.
- Remaining nonexact owners and maintained source corrections are recorded in
  config/functions.csv, docs/KNOWLEDGE_BASE.md and Git; consult these before trials.

## Current ECL evidence

Current source-bound canonical carrier:
`build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj`, SHA256
`94875d0fa89059d7decded90e9987f79d42388f12b0980cf163cefd154d4d6b9`.
The prior packet binds 43 current source/header/backend inputs and five Oracle
inputs. Its canonical-only audit checks all 67 owners /28,838 physical bytes /
1,215 effective fields, 35 nondebug noncode sections and all 23 configured
exact siblings /9,533 physical bytes /439 fields. No unchanged baseline compile.

New packet: `.analysis/gpt-6.1-sol-ecl-mutation-expressions-20261009/`.
Three isolated controls remove the opcode155 scalar capture when writing its
existing bitfield, group opcode4's genuine ordered mutations in one built-in
comma expression, and snapshot the actual opcode's promoted value before the
same switch. Every owner byte/effective field and data section remains neutral.
Private compiler labels rename only at the independently verified same actual
COFF destinations. All three complete RunEcl replays still have 2,161 differences;
batch-end strict PopContext checks pass 147/147 in each. None is integrated.
Each carrier's 36 actual includes agrees with the preobserved input hashes.
Historical loaded environment state is not retrospectively attested.

The maintained report now decodes all pre-table instructions, checks the actual
last RET, accepts only bounded NOP/self-LEA alignment, and rejects incomplete
instructions, active tail operations and direct branches leaving code. Target
code extent is independently decoded from the verified PE. Candidate table
positions remain 14,792; code is 14,791. The corresponding ledger evidence is
corrected without changing target size/status. Full call identity belongs to
inspect-ecl-complete.py, not the aggregate report.

Full proof is losslessly retained in audit.json.gz; actual compile logs, patches,
source-restoration recipe, independent parser, selected direct IDA windows,
code-extent reports and cleanup receipts remain. All processes are terminal.
Cleanup removes 31 owned copies/COFFs/PDBs/redundant reports, **5,990,015 bytes**;
canonical and inherited evidence stay. The retained-only audit verifies captured
candidate proof against the actual current canonical baseline; it performs no
new inspection of deleted candidate COFFs. Do not repeat these precise controls.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 -B .analysis/gpt-6.1-sol-ecl-mutation-expressions-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-type21-preheader-20261009/audit.py --canonical-only
python3 scripts/report-ecl-codegen.py build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj
python3 scripts/inspect-ecl-complete.py build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj
```

Call direct IDA get_metadata with exactly {} during entry attestation. Complete
RunEcl inspection returns 1 for its expected differences. prepare.py --restore
reproduces all 21 isolated sources without compiling; original trial COFFs were
intentionally cleaned. Earlier movement proof remains in
`.analysis/gpt-6.1-sol-movement-vector-context-20261009/`; restore its sources
before its full audit. Earlier retained corrections are documented in the KB.

Validation: focused decoder tests, complete canonical/retained proof, existing
exact sibling Oracles, tracking, progress, isolated CI and whitespace pass.
CI runs 79 tests: 68 pass, eleven optional Capstone checks skipped. Local
checkpoints are not pushed. Native product/runtime and later phase gates remain open.

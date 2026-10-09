# TH09 reconstruction handoff

## Active objective

The user explicitly resumed work on **2026-10-09**: reach **95% exact reviewed
authored bytes**, work seriously on large functions, use direct IDA Pro MCP
and local Bash/compiler Oracles without Factory MCP, commit locally as
`gpt-6.1-sol: ...`, and periodically clean reproducible intermediates. Do not push.
The goal remains **active-incomplete**. No new exactness credit was obtained in
this checkpoint. Type21 update now accesses its temporary descriptors through
real members instead of byte-buffer casts. One batch-end canonical compile
preserves all ECL owner bytes/effective fields and all 23 configured exact
siblings. Type21's 15 preheader differences remain unresolved. Prior DrawResult,
DrawReplayMenu and Enemy source corrections remain maintained and recorded in
the knowledge base and Git.
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
- Type21 update: 1,074 bytes /29 actual fields and 15 full differences in the
  ring preheader. Two record-owned acquisition paths are now verified neutral;
  no simple record/extra alias-path explanation survives those exact controls.

## Latest reviewed investigation

ExAttackUpdateCallbackType21 owns 1074 bytes at 0x44B500. Both bullet-conversion
paths construct a 0x214-byte temporary via target 0x40D500. Independent target
decoding confirms its 28-byte constructor clears 0x214 bytes and sets +0x204 to
-1. The existing private constructor view now contains a real
BulletSpawnDescriptor member at checked offset zero, instead of char storage
reinterpreted as a descriptor. Its constructor declaration/call and every
field write remain. No constructor definition or shared-header change is added.
Original class/subobject construction and native definition ownership remain
unknown; this is a bounded maintained source model.

Three isolated trials precede one batch-end canonical-path compile. Reading
only the UV-angle seed or only the history cursor through record->extra34 is
wholly neutral and neither is retained. The descriptor-member trial and its
canonical replay also preserve all raw code. Type21 remains 1074 bytes,
288 instructions, 29 independently decoded fields, 17 ordered calls, 28 blocks
and 15 full preheader differences. Raw SHA256:
fb14aea858101f030ca4b22f9c7c0d51699465dc1d39118e65717eb594c0fcb4.

Independent COFF parsing and separate repository extraction cover all 67 ECL
owners, 28,838 physical bytes and 1,215 fields, with all 35 nondebug noncode
sections unchanged. Four private labels rename at unchanged actual local
positions; only the two affected exact manifests need new spellings. All23
configured exact siblings reproduce 9,533 physical bytes and 439 fields through
the existing strict Oracle with a lossless object-path-only adapter. RunEcl
remains 2,161 differences over 15,564 bytes/598 fields. No new exact credit.

## Evidence and artifact lifecycle

Entry at 44f62e4 is clean; private target, direct IDA metadata, entry and five
mapped-byte samples pass. No Factory MCP, target patch, IDA write or delegation.
Current packet: .analysis/gpt-6.1-sol-type21-preheader-20261009/.
It retains Git-bound reconstruct.py, patches, actual include logs, input/backend
manifests, independent COFF parser, full losslessly compressed audit.json.gz and
the small source-bound canonical COFF. All 43 baseline input hashes and its COFF
hash match before reuse. All four carriers have exactly the 35 recorded unique
include inputs with before/after observations. Historical loaded compiler or
environment state is not retrospectively attested. No baseline rebuild.

All four compiler processes are terminal. Six source copies reconstruct to
their recorded hashes before cleanup. Receipts remove the three isolated COFFs,
four PDBs, six source copies and redundant full reports, totaling 1,904,960 bytes
including generated bytecode. Full report compression is lossless; the
canonical-only report equals its exact retained subset. Inherited evidence,
canonical caches and provider state remain untouched. Previous DrawResult
member evidence is in .analysis/gpt-6.1-sol-result-difficulty-20261009/ and Git 44f62e4.

Batch coherent changes before cold replay; every new exact owner requires
complete pinned canonical source/byte/relocation proof. Same sizes/counts do
not establish byte neutrality; compare actual bytes and full field descriptors.
The remaining Type21 preheader schedule needs different target-supported
source/callee evidence; do not repeat these two owner-path controls unchanged.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 .analysis/gpt-6.1-sol-type21-preheader-20261009/audit.py --canonical-only
python3 scripts/inspect-exattack-type21.py build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj --diff
python3 scripts/inspect-ecl-complete.py build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj
```

Call direct IDA get_metadata with exactly {} during entry attestation.
The full Type21 diagnostic reports 15 differing bytes and grants no credit.
The canonical-only audit binds source/backend/object hashes and checks the
entire affected carrier plus all 23 existing exact siblings, without compiling.
RunEcl's complete diagnostic returns 1 for its expected 2,161 differences.
Reconstruct isolated sources from their bound Git revision before a genuinely
new trial; their original COFFs are intentionally cleaned. Absolute global
memory displacements may be indexed by a register; actual decoding covers them
independently of configured fields. Private labels use actual local positions.

Validation: complete canonical owner/decoded operands and all collateral code,
tracking, progress, documentation and whitespace pass. Isolated CI runs 72 tests
(68 pass, four optional Capstone checks skipped). Worktree checkpoints remain
local and are not pushed. Native product/runtime and later phase gates remain open.

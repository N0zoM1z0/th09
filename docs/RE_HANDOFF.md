# TH09 reconstruction handoff

## Active objective

The user explicitly resumed work on **2026-10-09**: reach **95% exact reviewed
authored bytes**, work seriously on large functions, use direct IDA Pro MCP
and local Bash/compiler Oracles without Factory MCP, commit locally as
`gpt-6.1-sol: ...`, and periodically clean reproducible intermediates. Do not push.
The goal remains **active-incomplete**. No new exactness credit was obtained in
this checkpoint. DrawResult now reads difficulty through the target-proven
GameManager root plus 0x11C. One canonical-path compile preserves the complete
linked baseline; its 22 entry-byte differences remain unresolved. DrawReplayMenu's
earlier recovered Float3 stages/frame and Enemy operand verification remain
in maintained source, the knowledge base and Git.
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

DrawResult owns 1939 bytes at 0x423D16. Its screen14 difficulty-mask read at
0x423D53 addresses 0x4A7EAC. Independent target GameManager registration at
0x41B9E0 uses root 0x4A7D90; exact Ending also reads 0x4A7EAC at 0x40E64A/0x40E66A.
Current source reuses GameManagerModeView's checked difficulty +0x11C instead
of declaring a separate g_TitleNameTableIndex. This minimal view establishes
neither original identifiers/full layout nor native data-definition ownership.

One canonical-path pinned-profile compile changes only one DIR32 identity and
its two raw addend bytes. The full field now names g_GameManager with addend 284.
All 1939 linked bytes equal the verified old source-bound baseline. Complete
target replay still has 22 entry differences, 577 instructions, 82 fields,
16 ordered calls and 92 direct blocks. Target currentScreen loads into EAX after
the score-group store; candidate hoists it into ECX before bank multiplication.
The field correction does not resolve this scheduling. No exact unit or partial
credit is added. Current raw SHA256:
449827f92c3a23f3427b71b12f44719aa62c4a459a5b91734085a08c834e447a.

The independent audit decodes every target/candidate field, verifies 15 complete
literal payloads, all 18 nondebug sections and both emitted bodies. The uncalled
3-byte Float3 constructor remains unchanged. The inspector retains the old
alias binding solely for source-bound historical-object diagnosis. No extra
helper, profile change, forced register, ABI change or target patch is used.

## Evidence and artifact lifecycle

Entry at bd7a36c is clean; private target, direct IDA metadata, entry and five
mapped-byte samples pass. No Factory MCP, IDA write or delegation.
Current packet: .analysis/gpt-6.1-sol-result-difficulty-20261009/.
It retains the complete audit, independent COFF parser, source patch, actual
include log and source/header/backend/object manifests. All five actual includes
have unchanged before/after hashes; the old baseline and its recorded source,
headers, vendor/backend inputs are verified before reuse. Historical compiler
environment is not retrospectively attested. No unchanged baseline is rebuilt.

The single canonical compile is terminal. Cleanup removes its 53,248-byte
reproducible PDB. The small canonical object and compact proof remain;
inherited evidence/caches are untouched. Previous DrawReplayMenu evidence is
in .analysis/gpt-6.1-sol-replay-mode-20261009/ and Git bd7a36c.

Batch coherent changes before cold replay; every new exact owner requires
complete pinned canonical source/byte/relocation proof. Same sizes/counts do
not establish byte neutrality; compare actual bytes and full field descriptors.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 .analysis/gpt-6.1-sol-result-difficulty-20261009/audit.py
python3 scripts/inspect-title-result-draw.py build/gpt-6.1-sol-result-difficulty-20261009/canonical.obj --summary
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
```

Call direct IDA get_metadata with exactly {} during entry attestation.
The generic DrawResult Oracle returns 1 for its expected complete byte mismatch;
the complete audit exits successfully after reporting all 22 differing bytes.
It binds the canonical object to retained reviewed input hashes; running the
audit does not imply a new cold build. Audit address decoding distinguishes
indexed absolute displacements from object-relative members and direct JMPs
from relocation operands.

Validation: complete canonical owner/decoded operands and all collateral code,
tracking, progress, documentation and whitespace pass. Isolated CI runs 72 tests
(68 pass, four optional Capstone checks skipped). Worktree checkpoints remain
local and are not pushed. Native product/runtime and later phase gates remain open.

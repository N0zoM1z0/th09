# TH09 reconstruction handoff

## Active objective

The user explicitly resumed work on **2026-10-09**: reach **95% exact reviewed
authored bytes**, work seriously on large functions, use direct IDA Pro MCP
and local Bash/compiler Oracles without Factory MCP, commit locally as
`gpt-6.1-sol: ...`, and periodically clean reproducible intermediates. Do not push.
The goal remains **active-incomplete**. No new exactness credit was obtained in
this checkpoint. The October 7 stopped-state checkpoint is preserved in Git at
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

## Large-owner investigation

- RunEcl remains mandatory for the 95% goal: 14,792 authored bytes /15,564
  physical bytes. Current source-bound isolated compiles retain frame 0x168,
  375 direct calls, four indirect calls, 598 fields and 193 table entries.
  Graph/identity diagnostics report six handler frontiers: 4/7/86/155/156/157.
- The new `scripts/inspect-ecl-complete.py` compares all physical bytes with
  independently bound external fields and actual COFF local-label positions.
  The baseline and three new isolated controls each have **2,161 differing
  bytes**: 1,959 ordinary plus 202 encoded-field bytes, including 26 table bytes.
  Shape-normalized handler agreement is diagnostic only. In particular, the
  one-byte-short opcode155 shifts later code/table destinations; equal total
  physical size does not imply equal instruction or table layout.
- New ECL negatives: unsigned-byte timeout input, the existing remote-slot
  reference accessor for opcode86, and the already-live state receiver for
  handlers155..157 are each raw-byte neutral. Do not repeat these exact models.
- Enemy draw remains 1,758 bytes with four complete differences. Promoting the
  second subtraction's left operand to double regresses to 657 differences and
  frame 0x90. Removing the second Abs-result float conversion introduces a new
  double comparison literal and fails independent literal binding. Both are
  discarded; neither changes maintained source or established ABIs/profiles.
- The alternate float-return Abs alias is also raw-byte neutral; the same
  target callee and ST0/RET4 machine contract are retained. No ABI or source
  return-type promotion follows.
- Other retained large frontiers include Enemy OnUpdate (3,883 authored bytes,
  39 differences) and the non-exact UI/gameplay/network functions listed by
  `scripts/report-reconstruction-status.py` and config/functions.csv. Historical
  experiments remain in docs/KNOWLEDGE_BASE.md; consult them before new probes.

## Evidence, recovery and artifact lifecycle

Entry at 7347cb3 had a clean tracked/untracked worktree. Disk SHA256/MD5, direct
IDA metadata, entry point and five distributed mapped-byte samples all passed
local `scripts/check-ida-mcp.py`. No Factory MCP, target patch or IDA write was
used. Original executable, toolchain, providers and inherited ignored candidates
were preserved.

Six isolated compiles are terminal. Rejected source variants are reproducible
from the current source and compact recipes in docs/KNOWLEDGE_BASE.md. This
checkpoint removes only its task-owned probe source, objects and PDBs after
retaining input hashes and complete diagnostic summaries. Inherited build and
analysis trees have not been classified for bulk deletion and remain intact.
New compact packets are below `.analysis/gpt-6.1-sol-*-20261009/`.

The maintained game C/C++ sources, headers, profiles, extents and match ledgers
are unchanged. The new complete-diagnostic script and its guard tests are
tracked. Historical accepted Factory receipts remain historical; no new receipt
or exact owner is asserted. Native i386 product/runtime, semantic and portable
stages remain open.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
python3 scripts/report-ecl-handler-shapes.py build/matching/EclManager.obj
```

The complete inspector returns 1 for the measured non-exact owner; this is an
expected mismatch verdict, not a provider failure. It does not compile or prove
that an arbitrary supplied object was produced by current source. Before any
positive promotion, bind actual source/includes and run a fresh pinned canonical
build plus complete byte/relocation replay. Finish a batch with tracking,
progress, CI and whitespace checks; retain the full 95% objective.

Validation passed: target-required tracking, three focused canonical draw
replays, generated progress, all 62 target-independent CI tests, documentation,
explicitly open whole-build graph and whitespace checks. No background compiler
or worker remains.


## Follow-up checkpoint: complete Player movement comparison (October 9)

Recovery at 25ff491 and fresh direct IDA/disk/mapped-byte attestation pass. The
1,835-byte Player movement owner remains nonexact: 1,900 physical bytes, 489
instructions, 66 fields and 144 complete differences. The maintained inspector
now emits the complete physical-byte result and independently checks every
COFF field against decoded operands/tables. Actual local positions are retained;
no relocation is masked or fitted. Its successful exit denotes a completed
diagnostic, not an exact verdict or current-source build attestation.

Two new playfield models are closed. A private view of the four contiguous
float globals is fully neutral after relocation. Moving both clamps into an
ordinary member leaves a new 95-byte helper call; independent binding rejects
it. Neither is retained. Do not repeat these exact forms under the same source
and compiler context. Original data ownership and the remaining register/
scheduling differences remain unresolved. Earlier Pause helper-opacity/footer
controls were reviewed without recompiling or promoting them.

The retained input-owner carrier is bound to today's source via its historical
candidate hash, with all nine recorded headers unchanged; its old pre-retention
source hash is not claimed as current. Canonical Player OnUpdate still replays
522/522. All eight focused diagnostic tests pass; isolated CI runs 67 tests
with the two optional Capstone tests skipped and all remaining checks passing.
Tracking remains 928 exact owners and 218,501 /275,770 bytes (79.2331%).

Two isolated compiles are terminal. Cleanup removes 219,619 task-owned bytes
of sources, objects, PDBs and a duplicate report, retaining about 82 KiB below
`.analysis/gpt-6.1-sol-player-playfield-20261009/`. Source patches, actual include
logs, input hashes, complete comparison reports and independent collateral
audit remain; inherited evidence is preserved. No C/C++, header, profile,
extent or match-ledger change is retained. The 95% goal stays active.

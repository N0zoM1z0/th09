# TH09 reconstruction handoff

## Active objective

The user resumed on **2026-10-09**: reach **95% exact reviewed-authored bytes**,
work on large functions, use direct IDA Pro MCP and local Bash/compiler Oracles
without Factory MCP, and commit locally as `gpt-6.1-sol: ...`. Do not push.
Batch coherent source trials before cold replay. Reuse unchanged baselines after
checking source, compiler/backend and actual object bindings.

The preceding reconstruction checkpoint is `3d7d13b` (Enemy trail publication
controls). The current cleanup corrects documentation and progress reporting;
production C++, compiler profiles, boundaries and exact ledgers are unchanged.
Direct IDA metadata and mapped-byte attestation pass against the verified original
Japanese v1.50a executable. Historical environment interruptions are resolved.

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
| Canonical exact functions | 929 |
| Source-present non-exact functions | 51 |
| Source-present non-exact bytes | 55,833 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 219,937 |

The currently confirmed authored denominator is **275,770 bytes**: **79.7538%**
exact. Another **42,045 bytes** are needed for 95% of that denominator. The 35
origin-unresolved candidates are separate; later classification can change the
denominator. Faithful Windows i386 product/runtime gates remain open; semantic
reconstruction and portability have not started.

## Active frontiers

Read the knowledge-base controls for an owner before trying another source shape.
Measurements below belong to the maintained candidates and caches at this checkpoint.

| Owner | Current comparison | Remaining work |
| --- | --- | --- |
| RunEcl | 14,792 target authored / 15,564 physical; candidate 14,791 code + 1 alignment + 772 table bytes; 598 fields; 2,161 full differences | Handler frontiers 4/7/86/155/156/157; frame `0x168` |
| Enemy update | 3,883 authored / 3,900 physical; 97 fields; 39 differences | Draw index, descriptor/effect scheduling, trail and homing |
| Gameplay setup worker | 1,689 target bytes; 172 target / 171 candidate fields; 921 full differences | Reuse-base caching, flags cursor, rate/failure tail |
| GameManager update | 1,230 bytes; 87 fields; 105 differences | Input capture and entry-zero scheduling |
| Title Options | 2,045 target / 2,048 candidate; 135 fields; 610 overlap differences + 3 excess | Configuration/reference allocation; 51 calls and 121 direct blocks agree |
| Result draw | 1,939 bytes; 82 fields; 22 entry differences | Producer/entry allocation; later keyboard graph covered |
| Replay menu draw | 1,280 target / 1,258 candidate; 57 fields; 1,187 overlap differences + 22 absent | Owner, interpolation, position and row lifetimes |
| Supervisor network service | 1,633 target / 1,632 candidate; 106 target / 108 candidate fields; 1,395 differences + 1 absent | Sign test and output workspace |
| Player movement | 1,835 authored / 1,900 physical; 66 fields; 144 differences | Clamp/register allocation; direct SHT controls rejected |
| Player charge | 1,210 target / 1,197 candidate; 70 fields; 1,160 overlap differences + 13 absent | Entry register allocation and byte-mode storage |
| Enemy draw | 1,758 bytes; 53 fields; 4 differences | Second subtraction/Abs scheduling |
| Type21 update | 1,074 bytes; 29 fields; 15 differences | Ring preheader; actual descriptor member storage maintained |

Other owners, including ExAttack19, PauseMenu and DirectPlay, are routed by
`config/functions.csv` and `docs/KNOWLEDGE_BASE.md`. SaveReplay stays parked.

## Latest closure and rejected controls

The shared type18/type24 update is canonical exact: **1,436 authored bytes and
44 fields**, unit `exattack-type18-24-update`, actual object
`build/matching/ExAttackUpdateType18Type24Exact.obj`. Natural private XYZ assignment,
a comma-sequenced expression preserving three genuine Float3 result lifetimes,
and collision point/size declaration placement reproduce the target frame and
copies. Seven warm controls followed by one canonical cold establish the closure.
Retained proof: `.analysis/gpt-6.1-sol-type1824-vector-publication-20261010/`.
Original source/TU identity and native runtime remain independent unknowns.

The subsequent Enemy trail batch tests three typed publication contexts, followed
by one cold. Member assignment and a genuine source snapshot retain 3,900 physical
bytes / 97 fields / 39 differences. By-value publication reaches 3,908 physical
bytes with 2,344 overlap differences and eight excess. None is adopted; no credit
changes. Retained proof: `.analysis/gpt-6.1-sol-enemy-trail-publication-20261010/`.
Do not repeat these contexts unchanged. The exact type18/type24 model does not
establish the Enemy trail source shape; new work needs different TH09 evidence.

`EnemyView::ResolveFloat @ 0x004068A0` is already canonical exact: **1,633 credited
code bytes**, **2,044 complete physical bytes**, and **121 fields**. A focused
strict comparison passes during this cleanup. ECL-014 and TOOLCHAIN-152 now reflect
that closure. Do not reconstruct it again or credit its tables as authored code.

## Current object caches

| Owner | Cache |
| --- | --- |
| RunEcl | `build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj` |
| Enemy update | `build/matching/EnemyManagerCore.obj` |
| Enemy draw | `build/matching/EnemyManagerDraw.obj` |
| Player movement | `build/gpt-6.1-sol-movement-direct-sht-20261009/current-cold.obj` |
| Replay menu draw | `build/gpt-6.1-sol-replay-detail-frames-20261010/canonical.obj` |
| Player charge | `build/gpt-6.1-sol-charge-timer-conversion-20261009/canonical.obj` |
| Supervisor service | `build/gpt-dots-service-packet-owner-20261007/baseline.obj` |

Ignored caches may be cleaned. Rebuild from maintained source/config if an actual
object or binding is missing. Older Replay draw and type18/type24 cache sizes are
historical. Preserve the unresolved controls in the KB; repeated operational
receipts and already-indexed closed journals are recoverable from Git history.

## Restart and verification

Read the workflow/Oracle documents and review all dirty work before mutation.
Call direct IDA `get_metadata` with exactly `{}` and attest mapped bytes:

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/compare-coff-function.py --unit exattack-type18-24-update --json
python3 scripts/compare-coff-function.py --unit ecl-resolve-float --json
```

Use the focused complete inspector for the chosen non-exact owner. Such inspectors
may return 1 for expected differences. Historical proof scripts and whole-file
pins belong to their recorded revision: appending units or editing reporting
scripts can invalidate those pins without changing a compiler input. Reconcile
that distinction explicitly; never silently replace historical input hashes.
Replaying every historical cohort is unnecessary. Finish a bounded batch with
its focused Oracle, tracking validation, progress generation, isolated CI and
`git diff --check`. No compiler producer is live at this handoff.

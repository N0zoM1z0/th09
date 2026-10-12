# TH09 reconstruction handoff

## Active objective

The current local campaign was directed on **2026-10-12**: close **RunEcl**,
use target-bound pinned VC7.1 Oracles, and save verified checkpoints with
`gpt-6.1-sol: ...` commits and pushes. This supersedes the earlier Web no-push
default. The wider campaign still targets **95% exact reviewed-authored bytes**.
Inspect all dirty work rather than sidestepping it; preserve genuine progress
from recovered sessions.

The October 12 local maintenance session started at `a2e1b38` with a clean
worktree, after reviewing `/tmp/web_sth.txt` and the three latest RunEcl commits.
Disk target, direct IDA metadata, entry point and distributed mapped bytes all
agree with the original Japanese 1.50a target. Local maintenance checkpoints
use the operator-requested `gpt-6.1-sol: ...` prefix. That verified cleanup
checkpoint (`ddd583c`) has now been pushed. Native product and runtime remain open.

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
| Canonical exact functions | 931 |
| Source-present non-exact functions | 49 |
| Source-present non-exact bytes | 54,284 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 221,486 |

The currently confirmed authored denominator is **275,770 bytes**: **80.3155%**
exact. Another **40,496 bytes** are needed for 95% of that denominator. The 35
origin-unresolved candidates are separate; later classification can change the
denominator. Faithful Windows i386 product/runtime gates remain open; semantic
reconstruction and portability have not started.

## Active frontiers

Read the knowledge-base controls for an owner before trying another source shape.
The table lists selected non-exact owners, not all 49 remaining functions.
RunEcl was cold-reproduced during this maintenance session; other measurements
belong to the recorded candidates and must be replayed before further work.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| RunEcl | 0x004086C0 | 14,792 target authored / 15,564 physical; **14,792 code + 772 table bytes, 598 fields, 30 full differences (0 field/table)** | **146/148** handler shapes aligned; only opcodes 4/155 remain; frame `0x168`; still NON-EXACT |
| Enemy update | 0x00410730 | 3,883 authored / 3,900 physical; 97 fields; 39 differences | Draw index, descriptor/trail-copy and player/world register lifetimes; `scripts/inspect-enemy-manager-update.py` |
| Gameplay setup worker | 0x0041AF2D | 1,689 target bytes; 172 target / 171 candidate fields; 921 full differences | Reuse-base caching, flags cursor, rate/failure tail |
| GameManager update | 0x0041AA5F | 1,230 bytes; 87 fields; 105 differences | Input capture and entry-zero scheduling |
| Title Options | 0x004276EB | 2,045 target / 2,048 candidate; 135 fields; 610 overlap differences + 3 excess | Configuration/reference allocation; 51 calls and 121 direct blocks agree |
| Result draw | 0x00423D16 | 1,939 bytes; 82 fields; 22 entry differences | Producer/entry allocation; later keyboard graph covered |
| Title draw callback | 0x00424898 | 329 target / 328 candidate, 20 fields, 12 calls, 26 blocks | Recovered real Draw2D-first/JNP path and shared Flush; one target-only DEC at +0x101. `scripts/inspect-title-draw-callback.py` |
| Shake envelope | 0x004227A0 | 336 target / 334 candidate, 20 reviewed relocations, 8 calls, 26 blocks, 21 normalized difference spans | `scripts/inspect-screen-shake-envelope.py`; EDI/ESI receiver residency and final-parameter read order |
| Replay menu draw | 0x004234F6 | 1,280 target / 1,258 candidate; 57 fields; 1,187 overlap differences + 22 absent | Owner, interpolation, position and row lifetimes |
| Supervisor network service | 0x00430AA0 | 1,633 target / 1,632 candidate; 106 target / 108 candidate fields; 1,395 differences + 1 absent | Sign test and output workspace |
| Player movement | 0x0041C170 | 1,835 authored / 1,900 physical; 66 fields; 144 differences | Clamp/register allocation; direct SHT controls rejected |
| Player charge | 0x0041F810 | 1,210 target / 1,197 candidate; 70 fields; 1,160 overlap differences + 13 absent | Entry register allocation and byte-mode storage |
| Enemy draw | 0x00411670 | 1,758 bytes; 53 fields; 4 differences | Second subtraction/Abs scheduling |
| Supervisor received-frame queue | 0x0042E9E0 | 213 target / 213 candidate, 0 fields, 35 full differences | Typed 20-element side queue scan fixed physical extent; `scripts/inspect-supervisor-frame-queue.py`; register allocation and write scheduling remain open |
| ExAttack Type3 update | 0x00442750 | 780 bytes; 38 target-bound fields; 15 linked differences | `middle.z` / tail-X scheduling; use `scripts/inspect-exattack-type3.py` |
| ExAttack Type19/21 initialization | 0x0044A030 / 0x0044B220 | 763/727 bytes; 57/53 fields; 6/9 linked differences | History/vertex cursor scheduling, distinct from now-exact Type21 update |

Other owners, including ExAttack19, PauseMenu and DirectPlay, are routed by
`config/functions.csv` and `docs/KNOWLEDGE_BASE.md`. SaveReplay stays parked.

## Current evidence and rejected controls

RunEcl remains **NON-EXACT**. A fresh canonical carrier compile reproduces
**30 ordinary differing bytes**, with zero relocation-field or table differences:
opcode 4 contributes 7 bytes at `+0x0278..+0x0280`, and opcode 155 contributes
23 bytes at `+0x30B0..+0x30C7`. The normalized handler report agrees on
**146/148** shapes. Opcode 4 still differs in timer argument preparation;
opcode 155 emits an AND/OR update instead of the target XOR sequence.
Opcode 86 now agrees using bounded qualified reads of the genuine remote-slot
pointer and raw integer argument, calling the real exact ResolveInt helper.
The old declaration-only member shim has no known definition and is rejected.
Use the complete-byte inspector and handler reporter together; normalized
handler agreement grants no canonical or fractional byte credit. Read the
October 12 opcode7, 39-byte and opcode86 knowledge-base entries before new trials.

Enemy OnUpdate remains 39 differences over its complete 3,900-byte physical
extent and 97 bound fields. Its inspector checks the four-way switch graph and
fails closed on unknown symbols, label positions and noncode payloads. Typed
member assignment, genuine snapshot and by-value trail publication controls
are already recorded in the knowledge base; do not repeat them unchanged.

Closed owners are removed from the active table. Type21 update is canonical
exact at **1,074 bytes / 29 fields**, unit `exattack-type21-update`; its whole-Extra
byte cursor reaches aligned U/V float members with the reviewed 0x1C stride.
The shared type18/type24 update is exact at **1,436 authored bytes / 44 fields**,
unit `exattack-type18-24-update`. EffectManager OnUpdate is exact at
**475 bytes / 11 fields**, unit `effect-manager-update`, using a bounded qualified
primary-count read. ResolveFloat is exact at **1,633 authored / 2,044 physical
bytes / 121 fields**, unit `ecl-resolve-float`. Original source/TU spelling,
native linkage and runtime behavior remain independent of these exact claims.

Historical byte plateaus, rejected source controls and the inconclusive
`worker-lease-expired` Factory replay belong to `docs/KNOWLEDGE_BASE.md` and Git
history. They neither describe the current candidate nor grant Factory acceptance.

## October 12 maintenance verification and artifact audit

Cold builds of all seven configured EclManager-source objects reproduce
**24/24 canonical exact units, 10,607 complete bytes / 468 fields**. This exposed
one stale manifest label hidden by the old CompareOperands cache: at field
`+0x14`, the current `/DNDEBUG` object emits `$L8539`, not `$L8536`. Its actual
same-section value is still `+0x34C`, resolving to **0x00407A4C**. Only the symbol
spelling was corrected; target address, field kind/offset and full comparison
extent are unchanged. No game source, compiler profile or exact credit changed.

Artifact review classified the following files as generated compiler outputs.
Only `.obj` files with i386 COFF headers and `.pdb` files with MSF signatures were
removed; referenced objects and named baseline/canonical/cold controls were kept.
No producer was using these directories. Source trials, manifests, compact
results, `.analysis/`, provider state, toolchains and current matching caches stay.

| Reviewed build directory | Removed COFF/PDB files | Removed bytes |
| --- | ---: | ---: |
| `build/gpt-dots-ecl-post-20261003` | 490 | 24,001,801 |
| `build/gpt-web-type21-preheader-20261011` | 171 | 25,278,214 |
| `build/gpt-web-exattack-trail-init-20261011` | 108 | 15,775,848 |

These experimental outputs are not canonical inputs. Source-family controls and
conclusions remain in the knowledge base and retained analysis; current owners
reproduce from tracked source/config with `compile-probe.sh` or `build-match-unit.py`.
Unclassified older analysis was preserved. The active frontier table now carries
explicit target addresses and is checked against maintained authored-source and
exact ledgers; duplicate summary totals are also rejected by public CI.
Final validation passes: `validate-tracking.py`, regenerated progress,
`ci.py` (131 tests, 11 skipped), and `git diff --check`.

## Current RunEcl continuation

The local RunEcl campaign remains **active-incomplete**, continuing from pushed
`7bdf13e`. Fixed-source CPU diagnostics find identical complete RunEcl raw bytes
for /GB, /G5 and /G6, all retaining 30 linked differences. /G7 shortens the
physical extent by 68 bytes and fails the coverage gate. Four actual ZunTimer
interface controls also retain 30. Canonical source, profiles and credit stay
unchanged; the earlier 72/61/89-model batches remain excluded controls.

A bounded return-ABI audit of 203 IDA-listed direct timer-assignment calls finds
no positive return-value consumer before the checked path boundaries. The
setter physically leaves the integer input in EAX, but source-level return type,
later/indirect callers and original spelling remain unknown. The maintained
void contract is retained; do not invent a return to alter allocation.

The final current-source cold carrier reproduces 15,564 physical bytes,
598 independent field identities, 30 ordinary differences and 146/148 shapes.
All 24 configured EclManager-source exact units retain 10,607 complete bytes
/468 fields. Compiler core files are pinned and CL/_CL_ options cleared; CI
passes 135 tests, with 11 skipped. Compact scratch and precise exclusions are
routed by the latest CPU/timer-ABI knowledge-base entry.

Next review the one-bit field view's relationship to its actual backing word and
compiler lowering. Original source ownership, original switches and native
product remain unknown. Verified checkpoints are committed and pushed.

## Current object caches

| Owner | Cache |
| --- | --- |
| RunEcl and Type21 update | `build/matching/EclManager.obj` |
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
Call `get_metadata` with exactly `{}` through direct local IDA or the attested
Factory/Web `th09-ida` provider. In the local shell:

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/build-match-unit.py --unit exattack-type21-update
python3 scripts/compare-coff-function.py --unit exattack-type21-update --json
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
python3 scripts/report-ecl-handler-shapes.py --explain all build/matching/EclManager.obj
```

Factory/Web uses its attested provider instead of the host-only
`check-ida-mcp.py`. The RunEcl complete inspector returns 1 for the expected
nonzero differences; identity or input failures also return 1 and must be
resolved before using its measurements. Exact-unit comparisons must return 0.

Rebuild any chosen exact unit before comparing it after cache cleanup. Historical
proof scripts and whole-file pins belong to their recorded revision: appending
units or editing reporting scripts can invalidate those pins without changing a
compiler input. Never silently replace historical input hashes. Finish a bounded
batch with its focused Oracle, tracking validation, progress generation,
isolated CI and `git diff --check`. No compiler producer is live at this handoff.

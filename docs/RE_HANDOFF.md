# TH09 reconstruction handoff

## Current request and scope

The latest operator request is code, documentation and artifact cleanup after a
local RunEcl campaign. Use `gpt-6.1-sol: ...` for commits and push verified
checkpoints as authorized. The **95% exact reviewed-authored-byte goal** and
RunEcl closure remain open. Inspect dirty work before new changes; target and
IDA identity must be attested at each session entry. Native product and runtime
closure are separate, still-open gates.

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
RunEcl was cold-reproduced at the latest checkpoint; other measurements belong
to recorded candidates and must be replayed before further work.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| RunEcl | 0x004086C0 | **15,564 physical bytes = 14,792 code + 772 table bytes**; 598 fields, 30 full differences (0 field/table) | **146/148** handler shapes aligned; only opcodes 4/155 remain; frame `0x168`; still NON-EXACT |
| Enemy update | 0x00410730 | 3,883 authored / 3,900 physical; 97 fields; 39 differences | Draw index, descriptor/trail-copy and player/world register lifetimes; `scripts/inspect-enemy-manager-update.py` |
| Gameplay setup worker | 0x0041AF2D | 1,689 target bytes; 172 target / 171 candidate fields; 921 full differences | Reuse-base caching, flags cursor, rate/failure tail |
| GameManager update | 0x0041AA5F | 1,230 bytes; 87 fields; 105 differences | Input capture and entry-zero scheduling |
| Title Options | 0x004276EB | 2,045 target / 2,048 candidate; 135 fields; 610 overlap differences + 3 excess | Configuration/reference allocation; 51 calls and 121 direct blocks agree |
| Result draw | 0x00423D16 | 1,939 bytes; 82 fields; 22 entry differences | Producer/entry allocation; later keyboard graph covered |
| Title draw callback | 0x00424898 | 329 target / 328 candidate, 20 fields, 12 calls, 26 blocks | Recovered real Draw2D-first/JNP path and shared Flush; one target-only DEC at +0x101. `scripts/inspect-title-draw-callback.py` |
| Shake envelope | 0x004227A0 | 336 target / 334 candidate, 20 reviewed relocations, 8 calls, 26 blocks, 21 normalized difference spans | `scripts/inspect-screen-shake-envelope.py`; EDI/ESI receiver residency and final-parameter read order |
| Replay menu draw | 0x004234F6 | 1,280 target / 1,258 candidate; 57 fields; 1,187 overlap differences + 22 absent | Owner, interpolation, position and row lifetimes |
| Supervisor network service | 0x00430AA0 | 1,633 target / 1,632 candidate; 106 target / 108 candidate fields; 1,395 differences + 1 absent | Sign test and output workspace |
| Controller input | 0x0042B410 | **808 target / 796 candidate**, 32 independently decoded fields, 102 overlapping differences + 12 missing | Restored four true `JOYCAPSA` array-indexed member address operands; remaining X/Y register and ordering frontier, `scripts/inspect-controller-input.py` |
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

## Latest RunEcl evidence

The current full-owner comparison is **15,564 physical bytes** (14,792 code and
772 table bytes), **598 bound fields**, and **30 ordinary differences** with zero
field/table differences. **146/148** normalized handler shapes agree. The two
residuals are opcode 4 (7 bytes) and opcode 155 (23 bytes); RunEcl receives no
exact or fractional credit. Type21 update remains exactly 1,074 bytes /29 fields.

The October 12 inline assignment-helper batch tested 34 complete VC7.1
carriers. Twenty-two pass the complete identity gate: 16 retain 30 differences,
six regress to 2,140. Twelve fail closed on code/alignment or physical/field
coverage. None improves the maintained source. Its compact recipe, source
patches/hashes and results are in
`.analysis/gpt-6.1-sol-runecl-assign-conventions-20261012/`; generated source
copies and COFF/PDB outputs were removed after the producer finished. The
preceding old-word and `/FAs` listing controls are indexed in the latest
knowledge-base entries. Compiler listing source comments misattribute opcode
155; decoded bytes and field/CFG checks remain authoritative for codegen.

Before another RunEcl source trial, read the relevant negative controls in the
knowledge base and seek new TH09-local evidence. Original C++ spelling, retail
switches, native product and runtime remain unknown. The earlier CompareOperands
manifest-label correction and reviewed artifact audit are recorded in the
knowledge base and Git history.

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

## October 12 gpt-web cold-replay and candidate checkpoint

From clean `fd9e07d`, fresh forced-recompile Factory replays passed **and were accepted** for four previously canonical exact TH09 functions: EtamaController::OnUpdate (2,195 bytes), ExAttackUpdateCallbackType21 (1,074), TitleScreenView::OnUpdateMusicRoom (2,258) and TitleScreenView::TitleSetupThread (534). These receipts are bound to the **pre-checkpoint** source tree and must not be treated as fresh acceptance for later HEADs; use `factory_get_accepted_snapshot` or submit another clean-bound replay. No new exact function is claimed: 931/980 locally exact, 49 remaining.

New isolated full-TU compiler countermodels on both Type19/21 ExAttack initializers (six paired rim-cursor/lifetime forms), ten EffectManager::AddedCallback small-function forms and a Supervisor::OnUpdate shared-mode-1 join did not improve complete ownership. The Supervisor join even removes two required external effects from the candidate, so is explicitly rejected. The concrete full-owner scores and ignored, reproducible probe drivers are indexed at the end of `docs/KNOWLEDGE_BASE.md`. The maintained RunEcl 15,564/598/30, EnemyDraw 1,758/53/four-difference and EnemyManager OnUpdate 3,900/97/39 baselines were independently rebuilt and verified. Next priority remains source-backed closure of these boss functions and the Type19/21 initializers, together with real leaves. Do not promote a negative control or equate a compiler-codegen diagnostic with Factory accepted exactness.

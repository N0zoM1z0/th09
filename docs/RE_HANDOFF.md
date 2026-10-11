# TH09 reconstruction handoff

## Active objective

The user resumed on **2026-10-11** with Factory/Web tools: pursue **95% exact
reviewed-authored bytes**, attack both large functions and small leaves, reuse
target-bound pinned VC7.1 Oracles, and commit coherent verified checkpoints as
`gpt-web: ...` without pushing. Inspect all dirty work rather than sidestepping
it; preserve genuine progress from recovered sessions.

The starting HEAD for this batch was `8ebac88`, with no staged changes. The
recovered Type21 UV-angle source trial was inspected, improved to complete
exactness, replayed and promoted; no previous document claim was taken as
independent proof. The original Japanese 1.50a target and Factory attested IDA
module agree. Native product and runtime remain explicitly open.

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
Measurements below belong to the maintained candidates and caches at this checkpoint.

| Owner | Current comparison | Remaining work |
| --- | --- | --- |
| RunEcl | 14,792 target authored / 15,564 physical; candidate 14,791 code + 1 alignment + 772 table bytes; 598 fields; 2,161 full differences | Handler frontiers 4/7/86/155/156/157; frame `0x168` |
| Enemy update | 3,883 authored / 3,900 physical; 97 fields; 39 differences | Draw index, descriptor/trail-copy and player/world register lifetimes; `scripts/inspect-enemy-manager-update.py` |
| Gameplay setup worker | 1,689 target bytes; 172 target / 171 candidate fields; 921 full differences | Reuse-base caching, flags cursor, rate/failure tail |
| GameManager update | 1,230 bytes; 87 fields; 105 differences | Input capture and entry-zero scheduling |
| Title Options | 2,045 target / 2,048 candidate; 135 fields; 610 overlap differences + 3 excess | Configuration/reference allocation; 51 calls and 121 direct blocks agree |
| Result draw | 1,939 bytes; 82 fields; 22 entry differences | Producer/entry allocation; later keyboard graph covered |
| Title draw callback | 329 target / 328 candidate, 20 fields, 12 calls, 26 blocks | Recovered real Draw2D-first/JNP path and shared Flush; one target-only DEC at +0x101. `scripts/inspect-title-draw-callback.py` |
| Shake envelope | 336 target / 334 candidate, 20 reviewed relocations, 8 calls, 26 blocks, 21 normalized difference spans | `scripts/inspect-screen-shake-envelope.py`; EDI/ESI receiver residency and final-parameter read order |
| Replay menu draw | 1,280 target / 1,258 candidate; 57 fields; 1,187 overlap differences + 22 absent | Owner, interpolation, position and row lifetimes |
| Supervisor network service | 1,633 target / 1,632 candidate; 106 target / 108 candidate fields; 1,395 differences + 1 absent | Sign test and output workspace |
| Player movement | 1,835 authored / 1,900 physical; 66 fields; 144 differences | Clamp/register allocation; direct SHT controls rejected |
| Player charge | 1,210 target / 1,197 candidate; 70 fields; 1,160 overlap differences + 13 absent | Entry register allocation and byte-mode storage |
| EffectManager OnUpdate | **475/475 physical exact; 11 source/target relocations** | PrimaryCount read uses a target-order volatile-qualified lvalue; 14 existing same-TU exact owners intact |
| Enemy draw | 1,758 bytes; 53 fields; 4 differences | Second subtraction/Abs scheduling |
| Supervisor received-frame queue | 213 target / 213 candidate, 0 fields, 35 full differences | Typed 20-element side queue scan fixed physical extent; `scripts/inspect-supervisor-frame-queue.py`; register allocation and write scheduling remain open |
| ExAttack Type3 update | 780 bytes; 38 target-bound fields; 15 linked differences | `middle.z` / tail-X scheduling; use `scripts/inspect-exattack-type3.py` |
| ExAttack Type19/21 initialization | 763/727 bytes; 57/53 fields; 6/9 linked differences | History/vertex cursor scheduling, distinct from now-exact Type21 update |

Other owners, including ExAttack19, PauseMenu and DirectPlay, are routed by
`config/functions.csv` and `docs/KNOWLEDGE_BASE.md`. SaveReplay stays parked.

## October 11 Enemy OnUpdate whole-owner oracle and remaining fronts

A fresh pinned VC7.1 `EnemyManagerCore.cpp` compile reproduces the
**3,900-byte physical** EnemyManager OnUpdate owner with **97/97 independently
bound relocation fields, 1,026 instructions, 69 ordered calls and 170 blocks**
(including the single fully reviewed four-way switch table). All **39
complete target-linked differing bytes** remain, grouped near draw-index
register choice (+0x19C), bullet descriptor address/Float3 stores (+0x252..
+0x2BD), trail-hitbox return value copy (+0x845..+0x850) and player/world
position ESI/EDI allocation (+0x9C0..+0xA7E). This is not an exact match.

The tracked diagnostic is now `python3 scripts/inspect-enemy-manager-update.py
build/matching/EnemyManagerCore.obj`. It fails closed on unknown/ambiguous
external symbols, noncode relocations, incorrect target float payloads,
invalid owner-local label positions, unexpected indirect switches, missing
jump-table edges or incomplete decoding. Target-independent binding and
switch-graph tests guard these cases. The same-source cold replay keeps the
151-byte/two-field attached-effect sibling strictly exact. Numerous
source-faithful Player/world-address, draw-head cursor and trail-return
contexts recompile identically or worsen the full owner and were not retained.
Details are recorded at the end of `docs/KNOWLEDGE_BASE.md`.

## October 11 canonical Type21 closure

The ExAttack Type21 update at `0x0044B500` is now **canonical exact**:
**1,074 complete target bytes, 29 reviewed relocations**, unit
`exattack-type21-update`. A whole-Extra-object byte cursor pointing to the genuine adjacent U/V
float subobjects, stepped by the checked 0x1C-byte vertex stride, naturally
recovers the target EDI/EBX preheader order without undefined float-pointer
arithmetic across fields or distinct vertices;
positioning the UV-angle seed after the history-angle seed recovers target
stack publication. Separate source-bound VC7.1 compilations produce full
1,074/1,074 linked equality, 288 instructions, 17 calls, and 28 direct
control blocks. All 23 same-TU exact siblings retain **9,533 bytes/439 fields**.
Source file `src/ExAttackUpdateType21.inl` is part of the `EclManager.cpp`
compile carrier. The current cursor's byte arithmetic is confined to the real
whole Extra object; the float accesses land on declared, aligned float members
within valid rim samples. Native product, original TU ownership, and runtime
behavior remain independently unproven.

The **RunEcl** interpreter remains non-exact: 15,564 physical bytes,
598 relocated fields and 2,161 full differences, with 142/148 opcode-handler
shapes agreeing. Use the target-bound per-opcode reporter
`python3 scripts/report-ecl-handler-shapes.py --explain all build/matching/EclManager.obj`
to inspect the six remaining codegen frontiers. An isolated declaration-only
fastcall-member view makes opcode86's EDX/table-load instruction order agree,
but emits an unowned member symbol with no known linker definition; a real
inline forwarder to the existing exact resolver restores the old bad order.
Neither is integrated. The full current **142/148** count is unchanged; any
conditional 143rd opcode requires native symbol ownership and complete-source
verification before adopting it. No partial credit was granted to it. Previous Type21 15-byte
and six-byte comparisons remain historical candidates, not current status.
See the October 11 entry in `docs/KNOWLEDGE_BASE.md`.

An additional Factory Type21 replay attempt bound to commit `418d83a` was
**inconclusive**: job `job:f2439313eca24510b42c596cdc399fdd`
failed with `worker-lease-expired` before a receipt was produced. It does not
invalidate the fresh local strict VC7.1 match unit, but it also grants **no**
Factory acceptance. The repository's temporary replay lock later cleared;
inspect process/worktree state before any future factory submission rather
than automatically retrying an orphanable native process.

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
Call the attested Factory/Web `th09-ida` provider `get_metadata` with exactly
`{}` (or direct local IDA when available) and attest mapped bytes:

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/compare-coff-function.py --unit exattack-type18-24-update --json
python3 scripts/compare-coff-function.py --unit ecl-resolve-float --json
python3 scripts/build-match-unit.py --unit exattack-type21-update
python3 scripts/compare-coff-function.py --unit exattack-type21-update --json
```

Use the focused complete inspector for the chosen non-exact owner. Such inspectors
may return 1 for expected differences. Historical proof scripts and whole-file
pins belong to their recorded revision: appending units or editing reporting
scripts can invalidate those pins without changing a compiler input. Reconcile
that distinction explicitly; never silently replace historical input hashes.
Replaying every historical cohort is unnecessary. Finish a bounded batch with
its focused Oracle, tracking validation, progress generation, isolated CI and
`git diff --check`. No compiler producer is live at this handoff.

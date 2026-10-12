# TH09 reconstruction handoff

## Current request and scope

The operator requests sustained TH09 reconstruction across *large boss* and
*small leaf* functions, prioritizing RunEcl, Enemy/Etama, TitleScreen, and
ExAttack. Use **`gpt-web: ...`** for every new local commit. Commit substantive
verified progress, never abandon or silently discard existing dirty changes;
this is a sole-writer worktree. The Factory repository shell has no network,
so a local commit is **not** a remote push. The **95% exact reviewed-authored-byte
goal** and RunEcl closure remain open. Re-attest the original target and native
IDA on session entry. Native product/runtime closure and semantic/port stages
are separate, still-open gates.

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
| ExAttack Type19 update | 0x0044A330 | **821 physical / 254 instructions / 35 bound fields / 11 calls / 30 matching CFG blocks, 198 whole-byte differences** | Recovered distinct first-Player snapshot and live Side-slot reads; source in `src/ExAttackUpdateType19.cpp`, complete `scripts/inspect-exattack-type19.py`, remains NON-EXACT |
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

## October 12 gpt-web Type19 update full-slot correction

From clean `26241f1`, target-confirmed Type19 update now emits the target's **821 bytes / 254 instructions / 35 independently bound fields / 11 ordered direct calls / 30 direct blocks**. An actual first Player read plus separately retained Side slot, followed by independent Record/Extra view derivation, improves the complete resolved comparison from historical 206 to **198 differences**; it is explicitly still **NON-EXACT**, so the authored exact ledger remains **931/980**. The full target-bound comparison and source trial negatives (including the independently tested `MoveRandomBiased` Y-pointer variants) are in the final October 12 entry of `docs/KNOWLEDGE_BASE.md`. Rebuild the maintained update source with the pinned VC7.1 `/O2 /Ob1` profile, then run `scripts/inspect-exattack-type19.py build/matching/ExAttackUpdateType19.obj` (expected nonzero exit for genuine byte differences). The unchanged original and the remaining RunEcl/Enemy/Title priorities are still open.

## October 12 gpt-web fresh ECL and current-snapshot exact checkpoint

The current clean baseline was `f6aa0e9` (Type19 Player-slot correction), three
local commits ahead of origin/main. Revalidated the original JP 1.50a disk and
native IDA, **24/24** EclManager-source existing exact functions across **10,607
full physical bytes and 468 fields**, and the maintained **15,564/598/30** RunEcl
NON-EXACT owner, with only opcode4 and opcode155 still divergent. Repeat the
focused `python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj`
and `python3 scripts/report-ecl-handler-shapes.py --explain all
build/matching/EclManager.obj`; the first intentionally exits nonzero until
whole-owner exact. `python3 scripts/compare-coff-function.py --unit
exattack-type21-update --json` is a strict exact-sibling control.

Four **new Factory forced-recompile pass/accepted receipts bound specifically to
source commit `f6aa0e9`** confirm previously exact Etama OnUpdate, ExAttack
Type21 update, MusicRoom update and TitleSetupThread (6,061 target bytes;
not four new exact functions). Their exact claim IDs, job IDs, receipt IDs and
limitations are in the final October 12 KB entry. Later documentation-only
commits do not inherit a Factory-accepted *source-binding* for their new Git
trees; request a new cold replay for material compiler-input changes.

Bounded current-source negative research without production C++ edits includes
16 RunEcl opcode155 bitfield/ANM analogues, 15 ExAttack Type3 middle-Z/tail-X
contexts, 24 Type19 loop-tail permutations, 14 Type19 case/tail variants,
12 Title DrawResult producer/screen contexts and eight Controller WinMM field
qualification contexts. Full comparisons fail to improve the maintained
non-exact owners. The genuine Type3 target writes `middle.z` before preparing
tail-X; the lone 14-difference alternate moves the zero store too early and is
not adopted. Correct Type19 loop-tail scores are **12 at 198** and **12 at 201**,
not the preliminary conversational count. Full results, source hashes, rejected
input-shape reasons and reproducible ignored drivers are indexed in the
corresponding KB entry. `gpt-web:` prefix overrides older handoff prefixes.

## October 12 gpt-web Enemy four-region owner analyzer and negative trial checkpoint

Starting clean at `d4add7a`, fresh TH09 original-PE plus attested IDA identity
and current pinned VC7.1 objects reconfirm **931/980** canonical exact,
**49** non-exact. `RunEcl` remains **15,564/598/30** (only opcodes4/155);
`EnemyManager::OnUpdate` remains **3,900 physical bytes, 97 fields, 69
ordered calls, 39 ordinary linked differences**. Its new maintained `--explain`
option shows **four target-grounded delta regions** at `+0x19C` (2 bytes),
`+0x252` (11), `+0x845` (10), `+0x9C0` (16) with full target/candidate
instruction bytes. Its pure helper lives in `scripts/diagnostic_byte_spans.py`;
all three new regression tests execute even under CI's Python `-S`. Run `python3 scripts/build-match-unit.py --unit
enemy-attached-effect-update` and `python3 scripts/inspect-enemy-manager-update.py
--explain build/matching/EnemyManagerCore.obj`; the companion 151-byte
attached-effect helper is separately strict exact. This is a noncrediting
full-owner diagnostic, not a new fact about original C++ author spelling.

Cohesive exploratory source-only results cover 123 attempted models across
Enemy OnUpdate, the 82-byte collision-box leaf, 243-byte network parser,
ExAttack Type19/21 initializers, Type19 update, EffectManager callback and
TitleDrawCallback. The results and invalid forms are indexed in the final
October 12 KB entry; no game C++ was promoted because none improves complete
owner codegen with intact inputs. In particular the Type19/21 shared vertex
cursor problem is independently located at target `ADD EDI,14h` before the
history-cursor setup, and the parser's target newline-return block is before
its value scan rather than after. Ordinary pointer-declaration permutations
and goto placement were exhausted here and should not be retried unchanged.
Original target/game-source ABI, exact ledger and whole-product gates remain
unchanged. Follow the operator's `gpt-web: ...` commit convention. The previous
AGENTS.md instructions naming `gpt-6.1-sol` and a push capability are stale;
Factory Bash can make local commits but not remote pushes.

Fresh focused builds of the **existing canonical exact** units passed: Etama
OnUpdate 2,216 complete physical bytes (2,195 authored), MusicRoom update
2,258, TitleSetupThread 534 and ExAttack Type21 update 1,074; all relocations
and their comparisons are strict. RunEcl remains target-bound but non-exact
with 30 differences. Source C++ and local canonical ledger stay unchanged;
no new Factory receipt/owner exactness is inferred from this diagnostics batch.

## October 12 gpt-web collision leaf source-bound Oracle and Etama/Type19-21 addendum

The preceding Enemy four-region `--explain` report and its pure tests were
already tracked, so no redundant analyzer was created. The *new* maintained
`python3 scripts/inspect-enemy-collision-box.py
build/matching/EnemyCollisionBoxAppend.obj --json` is an independent
complete, noncrediting original-PE comparator for **82 bytes / 30 instructions /
2 genuine target-bound REL32 calls / 31 differing bytes**. It has four pure
negative tests, returns status 1 for a nonmatching owner and status 2 for
badly formed or unreviewed input, and never credits a prefix or unowned code.
The exact Float3 divide and box-append target helpers are its independent call
anchors; the callback itself is still **NON-EXACT**.

Twelve independently defined real draw-index accessor controls on the **3,900
byte/97-field/39-difference** Enemy boss are neutral or regress. Thirteen
separate `EtamaController::OnDraw` real VM/subobject/laser-cursor controls
remain **594/600 bytes** at best and do not recover the target `ESI` body
VM-position cursor. Twelve complete *paired* ExAttack Type19/21 initialization
preheader cursor-order trials remain at best **6/9 differences** respectively.
The additional trials are documented in the latest `docs/KNOWLEDGE_BASE.md`
entry and indexed by source/object digests under ignored `.analysis/gpt-web-...`
directories. All production C++/headers and the **931/980 exact** ledger remain
unchanged. The tracked `AGENTS.md` already correctly requires `gpt-web:`
commits and does not advertise remote-push support. Continue RunEcl 4/155,
Enemy four regions, and these live Type19/21 cursor and small-leaf gaps.

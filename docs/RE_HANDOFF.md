# TH09 reconstruction handoff

> **Current-state document, not a session journal.** Refresh the live ledgers
> and object files before new reconstruction work. Historical hypotheses,
> rejected trials and previous snapshots remain in `docs/KNOWLEDGE_BASE.md`
> and Git history (including `git show a02528d:docs/RE_HANDOFF.md`). Do not
> mistake a previous commit's byte counts for the present working tree.

## Operator instructions and identity

Continue original Japanese **TH09 v1.50a** reconstruction, attacking both
large boss functions and short leaves. Prioritize `RunEcl`, `EnemyManager::OnUpdate`,
`EtamaController::OnDraw`, ExAttack Type19/21 initialization and unresolved
TitleScreen callbacks. **`EtamaController::OnUpdate`, TitleSetupThread,
MusicRoom update/draw and ExAttack Type21 update are already exact**: protect
those matches with cold regressions, do not reopen them as non-exact tasks. Use **`gpt-web: ...`** for each new local commit;
commit substantive, verified progress. This is a single-writer worktree:
**inspect and finish dirty/untracked changes**, not avoid or reset them.
Factory's repository shell has no network push transport. Never claim a local
commit is a remote push.

Target: ignored `resources/th09.exe`, PE32 i386, image base `0x00400000`,
entry `0x0047D45F`, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The registered Factory provider is `th09-ida`, called as `get_metadata({})`;
its identity attestation is **not** a codegen acceptance. Follow
[`AGENTS.md`](../AGENTS.md), [`RE_WORKFLOW.md`](RE_WORKFLOW.md), and
[`ORACLES.md`](ORACLES.md) for source/profile and phase-gate rules.

## Live ledger snapshot

Checked against `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv` and `config/match-units.toml` by
`python3 scripts/validate-docs.py`; run a fresh status report after any edits.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,192 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 980 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 980 |
| Canonical exact functions | 935 |
| Source-present non-exact functions | 45 |
| Source-present non-exact bytes | 51,646 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 224,124 |

**224,124 / 275,770 reviewed-authored bytes = 81.2721% exact**.
To reach 95% of this currently classified denominator, another **37,858
bytes** must be exact. The 35 origin-unresolved functions are excluded from
that denominator and could alter it after review. Complete faithful Windows
i386 product/link/runtime closure is **open**; semantic reconstruction and
portability have **not started**. Source presence does not imply exact codegen;
exact codegen does not imply a runnable game.

## Active frontiers

These are **selected** source-present non-exact functions, **not** a complete
45-item inventory or a ranking by difficulty. Size is the target logical
extent unless marked *physical*. Measurements are tied to the maintained
candidate/profile and require fresh compilation after source edits. Consult
`config/functions.csv` and the relevant knowledge-base evidence first.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| RunEcl | 0x004086C0 | **15,564 physical** = 14,792 code + 772 table; 598 fields; **30** ordinary byte differences, zero field/table differences | 146/148 handler shapes; opcode 4 (timer load/push schedule, 7 bytes) and opcode 155 (XOR/AND/XOR now modeled, byte-load/register schedule still 23 bytes); `scripts/inspect-ecl-complete.py` |
| ECL MoveRandomBiased leaf | 0x00407E30 | **516 target / 517 candidate**, 28 independently bound fields, 14 ordered calls, 24/24 blocks; **148 overlapping differences and 1 extra byte** | Original `LEA ECX,[EAX+4]` / twice `[ECX]` versus candidate `MOV ECX,EAX` / twice `[ECX+4]`; new strict `scripts/inspect-ecl-biased-movement.py --explain`, **NON-EXACT** |
| EnemyManager OnUpdate | 0x00410730 | **3,900 physical**, 97 bound fields, 69 ordered calls, 170 direct blocks, **39** differences | Its **82-byte / two-call** collision append leaf remains **31** bytes different; both are now source-cold checked by `scripts/check-boss-frontiers.py --owner enemy`. The attached-effect update helper (**151**) and shared Enemy draw boss (**1,758**) independently replay exact; four calc register/copy windows remain. |
| ExAttack Type19/21 initializers | 0x0044A030 / 0x0044B220 | 763 / 727 bytes; 57 / 53 fields; **6 / 9** differences | Type19/21 share their typed init/update layout and Type21 exact Update is protected. Independent standalone-with-real-helper TU reproduces the same six/nine residuals; first-vertex postincrement, comma-sequencing and cursor variants regress or are neutral (October 14 KB). The genuine vertex/history-cursor scheduling remains open; `scripts/inspect-exattack-trail-init.py`. |
| ExAttack Type14/22 Draw | 0x00444130 | 959 target / 974 candidate; 53 fields, 20 direct + 12 indirect calls; **9/9 verified jumps, 43 aligned change spans** | All Type14/22 init/update/draw callbacks now share 0xF48 Extra and the original 0x38 record prefix in `src/ExAttackType1422ExtraView.hpp`; Type22 alone owns parameter +0x38; Draw register/loop scheduling still open; `scripts/inspect-exattack-type14-22-draw.py --explain`, **NON-EXACT** |
| ExAttack Type19 Update | 0x0044A330 | 821 bytes, 35 fields, **198** differences | Shared Type19 record/extra layout now enforced across init/update; player-slot and register residency remain; `scripts/inspect-exattack-type19.py` |
| Player position Type4/Type8 | 0x00443B10 / 0x00447620 | **249 target / 250 candidate** per callback; 8 independently bound fields, 6 ordered calls, same 6-block CFG | Original positive branch carries the FromAngleMagnitude receiver in ECX and uses `MOV EDX,ECX`; VC7.1 candidate rematerializes it with `LEA EDX,[EBP-28h]`. `scripts/inspect-player-position-type48.py --type 4/8`; both **NON-EXACT**. |
| ExAttack Type3 Update | 0x00442750 | **780/780 bytes**, 38 independently ordered fields, **14** differences (previously 15) | Shared `ExAttackHistory32ExtraView.hpp` with Type3/Type4 init/update; target writes middle Z **after** middle-Y call and **before** tail-X load, but candidate emits the Z store **before** middle-Y call. All 24 calls / 17 direct blocks still agree; `scripts/inspect-exattack-type3-schedule.py` verifies actual field identity and instruction chronology, **NON-EXACT** |
| Etama OnDraw | 0x00413BE0 | **600/600** physical bytes, 27 fields, 13 ordered calls; **17/17 matching direct blocks**, **268** differing bytes (previously 594/600, 465 overlapping + 6 absent, 18/17 blocks; interim dual-cursor 271) | Review-backed Laser and body VM position dual cursor restores `+0x208` anchor and loop graph; same valid body owner now supplies `inUse`, `angle` and position. Still **NON-EXACT**; pinned source uses numeric pointer-address offset for one-past terminal step, `scripts/inspect-etama-draw.py --json` |
| Title DrawResult | 0x00423D16 | **1,939/1,939** bytes, **82 independently decoded/bound fields**, 16 ordered calls, 92 matching blocks, **22** differences | Score-bank pointer / screen-state read schedule; `scripts/inspect-title-result-draw.py --json` |
| Title character cursor Active/Reverse/Inactive | 0x00424EDD / 0x00424FB0 / 0x0042505F | **211/217**, **175/175**, **175/175** target/candidate bytes; 6/2/2 independently decoded fields, 4/2/2 ordered calls | Active's 6 extra bytes and 189 overlapping differences keep its direct graph; Reverse/Inactive have **38** full byte differences each, and **5-target/4-candidate block graphs differ** despite equal extent. Source direct-array, prechecked-loop and count-stack-slot controls are rejected; `scripts/inspect-title-character-cursors.py --variant active/reverse/inactive` is **NON-CREDITING**. |
| Title DrawCallback | 0x00424898 | 329 target / 328 candidate, 20 fields | Single missing target `DEC EAX` at `+0x101`; `scripts/inspect-title-draw-callback.py` |
| Title DrawReplaySave | 0x004239F6 | **800 target / 793 candidate**, 41 original fields, 4 ordered calls, 28/28 blocks with unresolved edges | Renderer source remains NON-EXACT; `scripts/inspect-title-replay-save-draw.py --json` now shares full decoded field-identity gate |
| AnmManager camera-facing projection | 0x00439820 | **622 target / 619 maintained source** bytes, 40 independently bound fields, 11 ordered calls, matching 8-block graph; **447 overlapping differences plus 3 missing bytes** | Isolated natural source with the **real exact TranslateRotation body visible**, `origin` after the x87 FSINCOS and `Float3 delta` declared then assigned reproduces **622/622 physical bytes, only 24 remaining differences**, preserving all fields/calls/blocks. That is **diagnostic only**, not committed as a speculative helper-owner move or exact credit. `scripts/inspect-anm-camera-facing.py`; exact `Project3DQuad` (595) and `TranslateRotation` (49) protected by `--owner anm`. |
| ScreenEffect CalcShakeEnvelope | 0x004227A0 | **336/336** bytes, 20 reviewed fields, 8 ordered calls, 26/26 direct blocks; **28** remaining linked byte differences (previously 334/336 and 249) | Independent hold/fade-word loads, actual grouped endpoint sum; ESI/EDI receiver/timer coloring remains; `scripts/inspect-screen-shake-envelope.py`, **NON-EXACT** |
| Title Options | 0x004276EB | 2,045 target / 2,048 candidate; 135 fields | Configuration/reference lifetimes |
| Controller input | 0x0042B410 | 808 target / 796 candidate; 32 fields | WinMM X/Y register and read ordering; `scripts/inspect-controller-input.py` |
| Supervisor InsertReceivedFrame | 0x0042E9E0 | **213/213 bytes, 67/67 instructions, 9/9 blocks, zero relocations/calls**, **35** different bytes | Shares `SupervisorFrameQueueStorageView.hpp` with exact InsertPredictedFrame (191) / PopFrame (119). Its complete original SHA-256 and `ret 16` ABI are now checked independently under cold `scripts/check-boss-frontiers.py --owner network`. Prefix 16-bit field scheduling and side-base register lifetime remain unresolved; near-match is **NON-EXACT**. |
| Effect AddedCallback | 0x0040D1C0 | 51 bytes; 4 fields; **8** differing bytes | Original direct global-to-EDI load; `scripts/inspect-effect-added.py` |
| Enemy collision-box leaf | 0x00410250 | 82 bytes, 2 direct calls, **31** differing bytes | Shares exact receiver declaration with 3,900-byte EnemyManager OnUpdate; Float3 return-copy schedule still open; `scripts/inspect-enemy-collision-box.py` |
| FileSystem TryDecryptFromTable | 0x0042C290 | **220/220** bytes, 12 physical fields, 2 calls, 11 matching blocks; **38 byte differences** | Validated 8x12-byte source wire layout, original signature/table identities; `scripts/inspect-file-decrypt-table.py` **NON-EXACT** |
| Gameplay setup worker | 0x0041AF2D | 1,689 target/candidate; 172 / 171 operand fields; 155 aligned identities, 13 field-difference spans; 78 / 77 direct blocks | Owner/reuse-base lifetime, flags cursor and switch graph; `scripts/inspect-gameplay-setup.py --summary --field-diff`, **NON-EXACT** |
| GameManager OnUpdate | 0x0041AA5F | 1,230 bytes, 87 fields, 105 differences | Input snapshot and entry-zero schedule |
| Player movement | 0x0041C170 | 1,835 authored / 1,900 physical; 66 fields | Position clamp and register allocation |
| Supervisor network service | 0x00430AA0 | 1,633 target / 1,632 candidate, 108 source fields | Now shares the exact Supervisor queue +0x47C/+0x56C physical storage projection; 1,395 linked differences and call scheduling remain; **NON-EXACT** |
| Supervisor OnUpdate | 0x00431110 | 844 authored / 996 physical target; **952-byte** candidate (800 code + 152 switch data), **92/92 external fields, 27/27 calls** | RegisterStateObject physical registration tail merged from 3 source calls to **1**, matching target ECX=0/1, but 791 overlapping differences + 44 absent bytes, **NON-EXACT**; `scripts/inspect-supervisor-update.py` |

Other unresolved owners (including PauseMenu, Replay, network and remaining
Title helpers) live in `config/functions.csv`. The exact short-function subset
is maintained separately in [`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md).

## Closed codegen landmarks and accepted receipts

**Recent completed owners, all Factory `strict-live-v1` forced-cold
`pass / accepted` on the *specified original source commit*.** Each unit must
still be rebuilt when its compiler-input tree changes; a receipt is not a
whole-product/runtime acceptance.

| Exact owner | Target/bytes | Canonical unit | Accepted source commit and receipt |
| --- | --- | --- | --- |
| `FrontMessageRuntimeView::LoadForSideCount` | `0x004182E0`, **405/405**, 18 fields | `front-message-load-for-side-count` | `4d921a2`; `receipt:8ea1d24dda7aab61c63e8e1fb288727adb1125dff033a86fecc1c81cd8658e98` |
| `FrontMessageRuntimeView::ReleaseForSideCount` | `0x00417520`, **271/271**, 14 fields | `front-message-release-for-side-count` | `99ea00b`; `receipt:3ad8649181efbc34d2b08b5d1ead5d96ddd77d7fec074b9a0508219dfdf567a1` |
| `PlayerShotDrawCallbackType1` | `0x00442220`, **204/204**, 6 fields | `player-shot-draw-callback-type1` | `495e944`; `receipt:7f20349cb55183e13f66e4b1ecabc24133fd379d2f9c2b91a27256929bef1927` |

The FrontMessage load/release units are *two separate* exact functions.
Their source recovers real nullable cache miss/hit branches, finite
message-table iteration and pre-tested opcode traversal. These exact matches
supersede old `LoadForSideCount 403/405` and `ReleaseForSideCount 268/271`
near-match writeups; those are historical experiments in the knowledge base.

**Other existing exact owners to protect:** `EtamaController::OnUpdate`
(**2,195 authored / 2,216 complete physical bytes**),
`TitleScreenView::TitleSetupThread` (534 bytes),
`TitleScreenView::OnUpdateMusicRoom` (2,258 bytes),
`TitleScreenView::DrawMusicRoom` (209 bytes),
`ExAttackUpdateCallbackType21` (1,074 bytes),
`EnemyManagerDrawImpl` (1,758 bytes), and the FrontMessage `Reset`,
`Setup` and `FrontRelease::ReleaseCore` units. Existing accepted receipts
from earlier source commits are historical proof of *those commits only*;
current exactness comes from the match ledgers and fresh per-unit replay.

**Fresh Factory forced-cold regression receipts for source commit `faca071`**
(`strict-live-v1`, `windows.msvc71.function-exact`, `pass / accepted`):
`EtamaController::OnUpdate @ 0x004146F0` (2,195 logical / 2,216
physical bytes), `receipt:cb9b08b26b217ff97634a574c295745682d59efcf2b48e273d827146f70b491e`;
`TitleScreenView::TitleSetupThread @ 0x004249E1` (534),
`receipt:c859bcc4df3b63bb4653cb44c5719cd40233b0908ad1417c7e92e13d503dada1`;
`TitleScreenView::OnUpdateMusicRoom @ 0x00426E05` (2,258),
`receipt:9b4457007dede09614f9511a4a2a2ad7a79814598bea9d4adb85b8fea1623033`;
`TitleScreenView::DrawMusicRoom @ 0x004244A9` (209),
`receipt:4007bd438035e8eb12b5aef002104f4bf032004f9a7068ca4238436ad70fe26f`;
and `ExAttackUpdateCallbackType21 @ 0x0044B500` (1,074),
`receipt:4044e3d113b3030c39d34ac8e319c1301e04324ab05f49fe837a8337fe7beacd`.
These are five **independent regressions of already exact owners**, not five
new matches, not an acceptance of the nonexact large callbacks and not proof
of a complete Windows executable. All receipts are bound to the exact clean
`faca071e0708efa073f60c3ac2bc5678e577c130` source commit; the later
receipt-documentation commit must not be presented as itself replay-accepted.

`DrawSingleBullet @ 0x00413AF0` has **215 authored / 232 physical bytes**
including the original four-entry switch table, all 16 bound fields and the
unchanged `0x00413BC8` table pointer. The **current** same-TU manifest in
`config/match-units.toml` uses compiler-private `$L1937` at `+0x17`,
not the `$L1914` label recorded by an older source-commit receipt. These
private symbol numbers can change when adjacent `Etama::OnDraw` source
changes; verify the actual owner-relative +0xD8 selector and full table,
then update only the verified private spelling in the manifest. The source
change to OnDraw remains **268 bytes different / NON-EXACT**.

## Restart and verification

Read `AGENTS.md`, the active ledgers and dirty/untracked files **before**
mutations. Through Factory/Web call `th09-ida.get_metadata` with exactly `{}`;
its passed target attestation includes mapped-byte sampling. The host-only
`scripts/check-ida-mcp.py` is for **local direct IDA**, not Factory shell.

```bash
git status --short --branch
git diff && git diff --cached
python3 scripts/verify-target.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
# One cold, target-bound diagnostic sweep across RunEcl, EnemyManager,
# Etama, ExAttack Type3/19/21, Title renderer and character-cursor owners.
# Includes the Enemy collision leaf, camera-facing projection, received network
# frame insertion, and fourteen exact sibling regressions.
# NON-CREDITING: this command cannot promote a function to exact.
python3 scripts/check-boss-frontiers.py --owner all
python3 scripts/build-match-unit.py --unit front-message-load-for-side-count
python3 scripts/compare-coff-function.py --unit front-message-load-for-side-count --json
python3 scripts/build-match-unit.py --unit front-message-release-for-side-count
python3 scripts/compare-coff-function.py --unit front-message-release-for-side-count --json
python3 scripts/build-match-unit.py --unit player-shot-draw-callback-type1
python3 scripts/compare-coff-function.py --unit player-shot-draw-callback-type1 --json
python3 scripts/build-match-unit.py --unit bullet-draw-single
python3 scripts/compare-coff-function.py --unit bullet-draw-single --json
python3 scripts/inspect-etama-draw.py build/matching/EtamaControllerDraw.obj --json
python3 scripts/build-match-unit.py --unit etama-controller-update
python3 scripts/compare-coff-function.py --unit etama-controller-update --json
python3 scripts/build-match-unit.py --unit enemy-manager-draw-impl
python3 scripts/compare-coff-function.py --unit enemy-manager-draw-impl --json
python3 scripts/build-match-unit.py --unit exattack-type21-update
python3 scripts/compare-coff-function.py --unit exattack-type21-update --json
python3 scripts/build-match-unit.py --unit ecl-pop-context
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
python3 scripts/report-ecl-handler-shapes.py --explain all build/matching/EclManager.obj
python3 scripts/inspect-ecl-biased-movement.py build/matching/EclManager.obj --explain
scripts/compile-probe.sh src/PlayerPositionCallback30404Type4.cpp build/matching/PlayerPositionType4.obj /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src
python3 scripts/inspect-player-position-type48.py --type 4 build/matching/PlayerPositionType4.obj
scripts/compile-probe.sh src/PlayerPositionCallback30404Type8.cpp build/matching/PlayerPositionType8.obj /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src
python3 scripts/inspect-player-position-type48.py --type 8 build/matching/PlayerPositionType8.obj
python3 scripts/validate-docs.py
python3 scripts/progress.py --check
python3 scripts/ci.py
git diff --check
```

The ignored `build/` tree should be **absent after the final cleanup**, not
while builds/tests are running. The commands above regenerate the objects
they need; `bullet-draw-single` builds the exact sibling and the non-exact
`Etama::OnDraw` diagnostic from its current shared TU. Never run an
inspector against a missing, older or unverified `.obj`; first compile its
current `src/` owner under that match unit's pinned profile.

The **RunEcl**, `Etama::OnDraw` and other non-exact diagnostics can return
nonzero while correctly reporting bounded differences; always inspect their
full identity, field, call and CFG checks rather than treating a near match as
exactness. Ignored `build/` and `.analysis/` objects are not proof.
Do not replay the entire historical experiment cohort unnecessarily; preserve
all canonical exact units when cleaning and never manipulate source solely
to improve a partial comparison score.

## Evidence and cleanup boundaries

This handoff was checked on **2026-10-13** against the original executable,
the ledgers and fresh pinned VC7.1 comparisons. The current source count is
**980 source-present / 935 exact / 45 non-exact**, *not* the older
979/876/103 estimate. The 35 origin-unresolved candidates remain outside the
reviewed-authored-byte denominator. The full native Windows i386 executable,
link/resource ownership and dual-runtime/port phases are **not closed**.

`docs/KNOWLEDGE_BASE.md` is a **historical evidence and negative-experiment
archive** after its indexed facts. Phrases such as `now exact`, `still
non-exact`, earlier 931/49 counts, old private `$L...` labels, superseded
near-match sizes and source snapshots inside *dated* entries describe the
experiment's source commit, not today's worktree. To reproduce a current
claim, read the current manifest/ledger and rebuild the original-bound
owner. Historical handoffs, including `git show a02528d:docs/RE_HANDOFF.md`,
remain available in Git rather than being repeated here.

TH09's message-runtime `FrontMessageRuntimeView` also has distinct
**translation-unit-local layout projections** in Load, Release, Update,
Draw and caller code. A broad declaration merge is not an innocuous tidy-up:
those views expose different members and sometimes only forward methods, and
several TUs are exact. Do not unify them without mapping the entire ABI,
validating the common object geometry, and rerunning every dependent canonical
unit; preserving exact codegen takes priority over cosmetic refactoring.

For disposable cache cleanup, preview
`python3 scripts/clean-generated-artifacts.py` and run with `--apply` only
after all focused validations and CI. This removes ignored `build/` and
compiler binaries under ignored `.analysis/` while preserving private
`resources/th09.exe`, tracked source, pinned toolchains, IDA databases,
probe source and JSON/text evidence. The ignored local
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json` records
actual deletions across earlier sweeps; its historical file/byte totals
**are not current workspace measurements**. After cleanup a subsequent dry
run should report zero selected artifacts. `scripts/ci.py` may recreate
Python bytecode caches, so **do not rerun CI after the final sweep** unless
you clean those caches again. Never prune historical source or unique
analysis evidence automatically.

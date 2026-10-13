# TH09 reconstruction handoff

> **Current-state document, not a session journal.** Refresh the live ledgers
> and object files before new reconstruction work. Historical hypotheses,
> rejected trials and previous snapshots remain in `docs/KNOWLEDGE_BASE.md`
> and Git history (including `git show a02528d:docs/RE_HANDOFF.md`). Do not
> mistake a previous commit's byte counts for the present working tree.

## Operator instructions and identity

Continue original Japanese **TH09 v1.50a** reconstruction, attacking large
boss functions **and** short leaves, with RunEcl, Enemy/Etama, TitleScreen and
ExAttack the main priorities. Use **`gpt-web: ...`** for each new local commit;
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
| Canonical exact functions | 933 |
| Source-present non-exact functions | 47 |
| Source-present non-exact bytes | 52,322 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 223,448 |

**223,448 / 275,770 reviewed-authored bytes = 81.0269% exact**.
To reach 95% of this currently classified denominator, another **38,534
bytes** must be exact. The 35 origin-unresolved functions are excluded from
that denominator and could alter it after review. Complete faithful Windows
i386 product/link/runtime closure is **open**; semantic reconstruction and
portability have **not started**. Source presence does not imply exact codegen;
exact codegen does not imply a runnable game.

## Active frontiers

These are **selected** source-present non-exact functions, **not** a complete
48-item inventory or a ranking by difficulty. Size is the target logical
extent unless marked *physical*. Measurements are tied to the maintained
candidate/profile and require fresh compilation after source edits. Consult
`config/functions.csv` and the relevant knowledge-base evidence first.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| RunEcl | 0x004086C0 | **15,564 physical** = 14,792 code + 772 table; 598 fields; **30** ordinary byte differences, zero field/table differences | 146/148 handler shapes; opcode 4 (timer load/push schedule, 7 bytes) and opcode 155 (XOR/AND/XOR now modeled, byte-load/register schedule still 23 bytes); `scripts/inspect-ecl-complete.py` |
| ECL MoveRandomBiased leaf | 0x00407E30 | **516 target / 517 candidate**, 28 independently bound fields, 14 ordered calls, 24/24 blocks; **148 overlapping differences and 1 extra byte** | Original `LEA ECX,[EAX+4]` / twice `[ECX]` versus candidate `MOV ECX,EAX` / twice `[ECX+4]`; new strict `scripts/inspect-ecl-biased-movement.py --explain`, **NON-EXACT** |
| EnemyManager OnUpdate | 0x00410730 | **3,900 physical**, 97 fields, **39** differences | Its 82-byte collision append leaf now shares `EnemyAppendCollisionView.hpp` (unchanged physical ABI); four register/copy windows remain; `scripts/inspect-enemy-manager-update.py --explain` |
| ExAttack Type19/21 initializers | 0x0044A030 / 0x0044B220 | 763 / 727 bytes; 57 / 53 fields; **6 / 9** differences | Type19 and Type21 each use a single shared init/update layout in `ExAttackType19ExtraView.hpp` and `ExAttackType21ExtraView.hpp`; Type21 exact Update is preserved; vertex/history-cursor scheduling still open; `scripts/inspect-exattack-trail-init.py` |
| ExAttack Type14/22 Draw | 0x00444130 | 959 target / 974 candidate; 53 fields, 20 direct + 12 indirect calls; **9/9 verified jumps, 43 aligned change spans** | All Type14/22 init/update/draw callbacks now share 0xF48 Extra and the original 0x38 record prefix in `src/ExAttackType1422ExtraView.hpp`; Type22 alone owns parameter +0x38; Draw register/loop scheduling still open; `scripts/inspect-exattack-type14-22-draw.py --explain`, **NON-EXACT** |
| ExAttack Type19 Update | 0x0044A330 | 821 bytes, 35 fields, **198** differences | Shared Type19 record/extra layout now enforced across init/update; player-slot and register residency remain; `scripts/inspect-exattack-type19.py` |
| ExAttack Type3 Update | 0x00442750 | **780/780 bytes**, 38 fields, **14** differences (previously 15) | Shared `ExAttackHistory32ExtraView.hpp` with Type3/Type4 init/update; zero middle Z moved ahead of Y conversion with 24 calls / 17 matching blocks; actual middle-Z write still scheduled ahead of the target, **NON-EXACT** |
| Etama OnDraw | 0x00413BE0 | **600/600** physical bytes, 27 fields, 13 ordered calls; **17/17 matching direct blocks**, **268** differing bytes (previously 594/600, 465 overlapping + 6 absent, 18/17 blocks; interim dual-cursor 271) | Review-backed Laser and body VM position dual cursor restores `+0x208` anchor and loop graph; same valid body owner now supplies `inUse`, `angle` and position. Still **NON-EXACT**; pinned source uses numeric pointer-address offset for one-past terminal step, `scripts/inspect-etama-draw.py --json` |
| Title DrawResult | 0x00423D16 | 1,939 bytes, 82 fields, **22** differences | Score-bank pointer / screen-state read schedule |
| Title DrawCallback | 0x00424898 | 329 target / 328 candidate, 20 fields | Single missing target `DEC EAX` at `+0x101`; `scripts/inspect-title-draw-callback.py` |
| ScreenEffect CalcShakeEnvelope | 0x004227A0 | **336/336** bytes, 20 reviewed fields, 8 ordered calls, 26/26 direct blocks; **28** remaining linked byte differences (previously 334/336 and 249) | Independent hold/fade-word loads, actual grouped endpoint sum; ESI/EDI receiver/timer coloring remains; `scripts/inspect-screen-shake-envelope.py`, **NON-EXACT** |
| Title Options | 0x004276EB | 2,045 target / 2,048 candidate; 135 fields | Configuration/reference lifetimes |
| Controller input | 0x0042B410 | 808 target / 796 candidate; 32 fields | WinMM X/Y register and read ordering; `scripts/inspect-controller-input.py` |
| Supervisor InsertReceivedFrame | 0x0042E9E0 | 213/213 bytes; zero relocations, **35** different bytes | Shares `SupervisorFrameQueueStorageView.hpp` with exact InsertPredictedFrame/PopFrame and 177-byte exact Mode4 reset; initial 16-bit write and side-base scheduling remain; `scripts/inspect-supervisor-frame-queue.py`, **NON-EXACT** |
| Effect AddedCallback | 0x0040D1C0 | 51 bytes; 4 fields; **8** differing bytes | Original direct global-to-EDI load; `scripts/inspect-effect-added.py` |
| Enemy collision-box leaf | 0x00410250 | 82 bytes, 2 direct calls, **31** differing bytes | Shares exact receiver declaration with 3,900-byte EnemyManager OnUpdate; Float3 return-copy schedule still open; `scripts/inspect-enemy-collision-box.py` |
| FileSystem TryDecryptFromTable | 0x0042C290 | **220/220** bytes, 12 physical fields, 2 calls, 11 matching blocks; **38 byte differences** | Validated 8x12-byte source wire layout, original signature/table identities; `scripts/inspect-file-decrypt-table.py` **NON-EXACT** |
| FrontMessage LoadForSideCount | 0x004182E0 | 405 target / 403 candidate; 18 reviewed fields, 8 ordered calls, 32/28 graph blocks | Shared file-offset/pointer layout in `src/FrontMessageTableView.hpp`; `scripts/inspect-front-message-load.py --explain`; **NON-EXACT** |
| FrontMessage ReleaseForSideCount | 0x00417520 | 271 target / 268 candidate; 14 fields, 6 calls; **130** linked byte differences, 20/21 blocks (target/candidate) | Original logical-index/end-point instruction ordering recovered; `scripts/inspect-front-message-release.py --explain`; **NON-EXACT** |
| Gameplay setup worker | 0x0041AF2D | 1,689 target/candidate; 172 / 171 operand fields; 155 aligned identities, 13 field-difference spans; 78 / 77 direct blocks | Owner/reuse-base lifetime, flags cursor and switch graph; `scripts/inspect-gameplay-setup.py --summary --field-diff`, **NON-EXACT** |
| GameManager OnUpdate | 0x0041AA5F | 1,230 bytes, 87 fields, 105 differences | Input snapshot and entry-zero schedule |
| Player movement | 0x0041C170 | 1,835 authored / 1,900 physical; 66 fields | Position clamp and register allocation |
| Supervisor network service | 0x00430AA0 | 1,633 target / 1,632 candidate, 108 source fields | Now shares the exact Supervisor queue +0x47C/+0x56C physical storage projection; 1,395 linked differences and call scheduling remain; **NON-EXACT** |
| Supervisor OnUpdate | 0x00431110 | 844 authored / 996 physical target; **952-byte** candidate (800 code + 152 switch data), **92/92 external fields, 27/27 calls** | RegisterStateObject physical registration tail merged from 3 source calls to **1**, matching target ECX=0/1, but 791 overlapping differences + 44 absent bytes, **NON-EXACT**; `scripts/inspect-supervisor-update.py` |

Other unresolved owners (including PauseMenu, Replay, network and remaining
Title helpers) live in `config/functions.csv`. The exact short-function subset
is maintained separately in [`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md).

## Closed codegen owners and accepted receipts

- **`PlayerShotDrawCallbackType1 @ 0x00442220` — newly canonical exact in this checkpoint:** pinned VC7.1 `/O2 /Ob1` emits **204/204 complete bytes, all 6 independently bound original COFF fields** and 67/67 decoded instructions. Restoring alpha and returning on the invalid-history sentinel, independently from the normal loop exit and the zero-trip tail, naturally retains the target's two separate 14-byte return sequences. Canonical `player-shot-draw-callback-type1` match unit proves the result. Historical stand-alone early-return probes had been compiler-tail-merged; no inserted padding or fake side effects. The exact ledger increases from 932 to **933**. Factory `strict-live-v1` forced-cold replay of committed `495e944d62fcd5bc9e30f27d447002453e4a589f` returned **pass/accepted**: `job:f936b51a5ae4487eac9882e99f128b4e`, `receipt:7f20349cb55183e13f66e4b1ecabc24133fd379d2f9c2b91a27256929bef1927`. The receipt is **source-commit scoped**; whole-product/runtime closure remains open.

- **Etama/Supervisor Factory cold regressions on committed `4f97ccb` (existing exact only):** `EtamaController::OnUpdate @ 0x004146F0` **2,216/2,216**, 88 fields, `pass/accepted` receipt `receipt:76e9b270bf6efed193d2d371960ab4c0879ac5b687bf052b70b8a8440accc323`; `SupervisorFrameQueueView::AreFrameQueuesSynchronized @ 0x0042ECF0` **49/49**, `pass/accepted` receipt `receipt:56ef7eb17b8cdc9c35778a2e10ab222a46069b1fa17afb6f663c9960caf59df6`. These prove unchanged canonical owners, NOT new exactness. `Supervisor::OnUpdate` with recovered target single title-registration call remains **952/996 physical NON-EXACT**, and `RunEcl` remains **30 nonexact bytes**.

- **ExAttack Type3/Type4 32-history family (already exact members):** Type3 init **454/454**, Type4 init **368/368**, Type4 update **739/739** under shared `src/ExAttackHistory32ExtraView.hpp`. Factory forced-cold `pass/accepted` on committed `76575a8`: `receipt:531b331a35d89b3385d7e13ea9803bc1dc080ca91c883f60625cf311b5135f67` (Type3 init), `receipt:401073f5b594639577acecc39900127a3af08a51e21dc1111b8dda8a5baa1922` (Type4 init) and `receipt:ffd106aaa2a1d6fe22a0c4c78f50bdf0c9cca93be31fce430663197b174b3833` (Type4 update). Type3 Update remains **NON-EXACT 780/38/15**. Unchanged accepted exact functions do not increase exact count.

- **Supervisor frame-queue family (existing exact)**: `InsertPredictedFrame @ 0x0042EAC0` **191/191**, `PopFrame @ 0x0042EB80` **119/119**, and Mode4 queue `ResetTitleMode4Supervisor @ 0x0042EC30` **177/177**, after shared queue record/view corrections. Factory forced-cold `pass/accepted` receipts respectively `receipt:9d49fc892a1cb0c86ae8b17a1611c146fb8d86187cb4c8193fe27daa8d6b1d7a` and `receipt:e0a9041d000117a4304faa47bffcb476ecbb67d20c20f07f1f903a9b7de48760` for committed `f3bec14`, plus `receipt:b757b79ef7616bf001af8317cd073f0edc8356910290b4c44fd025ff3442ac30` for committed `2323c23`. The separate `AreFrameQueuesSynchronized` helper also remains exact at 49/49. **The 213-byte `InsertReceivedFrame` is still NON-EXACT (35 differences); these are regressions, not newly exact functions.**

- **`EnemyManagerDrawImpl @ 0x00411670`**: **1,758 / 1,758 complete bytes,
  53 actual relocations**; pinned match unit `enemy-manager-draw-impl`.
  Its two draw wrappers and interpolation leaf also remain exact. A real
  `float` interpolation result, `long double` delta, and explicit float
  argument conversion resolved the final x87 FSUBR/PUSH scheduling. Factory
  **pass/accepted** receipt `receipt:44cacda296eb6f3a51916daa49d7a31f9fba8d6c1b8400983bfa3c93d1fead8f`
  is bound to source commit `480f2028f82b790a080be6f09994736286d0bc95`.
- **`ExAttackUpdateCallbackType21 @ 0x0044B500` and shared `ExAttackDrawCallbackType19_21 @ 0x0044B940`**: already canonical exact 1,074 and 318 bytes, respectively. After the Type19/21 shared-layout source changes, Factory forced-cold `windows.msvc71.function-exact` accepted both exact claims on committed `9932230` (`receipt:b02ffddd42f7f419e72c64b6952d37440a5737b356e86172cbb1ec34c270db57` and `receipt:46d21ebe6fc8047fce2134ce5810349aad3f93b2b65608509f00a88fad9fde28`). Accepted replay of existing exact units does not promote either initializer, nor change the then-current 932/980 ledger.
- **`DrawSingleBullet @ 0x00413AF0`**: 215/215 authored, 232/232 complete
  physical bytes with all 16 relocations. The refreshed same-TU compiler-private
  switch-table `$L1914` is independently at target +0xD8; the original jump
  operand remains `0x00413BC8`. Factory `pass/accepted` cold receipt
  `receipt:cf9834542cf3bf1116ac4e83a7599ad103611eec1df388a0ae1bc18bdf7d0260` binds the clean
  source commit `fd63e67c16d1a0e822af4f88ed8e544665a42969` and confirms
  this **existing** exact replay, without adding an exact function or promoting
  nonexact OnDraw.
- **`EtamaController::OnUpdate`**, **`TitleScreenView::OnUpdateMusicRoom`**,
  **`TitleScreenView::TitleSetupThread`** and **`ExAttackUpdateCallbackType21`**
  are *already* canonical exact. Four accepted cold replays of these existing
  owners belong to source commit `f6aa0e960900e90b314b0fb0d77aa3d820b5ffe5`;
  they are not four new matches.
- **`EffectManager::OnUpdate`**: **475 authored bytes exact**, with a separate
  Factory pass/accepted receipt
  `receipt:9b01a390ab05d73f20cdde28aeed7c67123343bafefd730b004e74db52eb34dd`
  bound to source commit `5dcd7f2`. The adjacent **Effect AddedCallback is
  still non-exact**; the receipt does not apply to it.

Factory receipts are **source-commit scoped**. An old accepted job cannot
prove exactness for a changed compiler-input tree, a different owner or the
whole product. The canonical exact ledger is local and must always be
replayable independently.

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
python3 scripts/build-match-unit.py --unit enemy-manager-draw-impl
python3 scripts/compare-coff-function.py --unit enemy-manager-draw-impl --json
python3 scripts/build-match-unit.py --unit exattack-type21-update
python3 scripts/compare-coff-function.py --unit exattack-type21-update --json
python3 scripts/build-match-unit.py --unit ecl-pop-context
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
python3 scripts/report-ecl-handler-shapes.py --explain all build/matching/EclManager.obj
python3 scripts/inspect-ecl-biased-movement.py build/matching/EclManager.obj --explain
python3 scripts/validate-docs.py
python3 scripts/progress.py --check
python3 scripts/ci.py
git diff --check
```

The ignored `build/` tree is deliberately **absent after handoff cleanup**.
The build commands above regenerate only the objects they need. Never run an
inspector against a missing, older or unverified `.obj`; first compile its
current `src/` owner under that match unit's pinned profile.

The **RunEcl** complete inspector returns **1 for known differences**; always
read the full result and ensure identity/relocation/CFG checks passed. Other
near-match diagnostics also return nonzero until truly exact. Ignored
`build/` and `.analysis/` objects are not proof. Do not run the whole
historical cohort unnecessarily. Preserve codegen exact
units when cleaning, and never alter source just to manipulate comparisons.

## Evidence and cleanup boundaries

This is the **2026-10-13 clean handoff**. The canonical source/ledger and
original target remain authoritative; local compiled objects do not. Current
counts are the verified **980 source-present / 933 exact / 47 non-exact**,
not the historical 979/876/103 estimate. The whole Windows i386 build and
runtime/semantic phases remain **open**. All screen/Etama/Title/ExAttack
exactness statements in older dated experiments and Factory receipts are
**source-commit scoped**; a successful old replay never promotes a sibling.

`docs/KNOWLEDGE_BASE.md` contains a *historical hypothesis and negative-result
archive*. Old "current" and "exact" prose inside a dated packet, including
older 931/49 counts and deprecated compiler-source variants, is **not live
status**. Consult the ledger and a fresh cold comparison. The full previous
handoff is recoverable from Git (for example `git show a02528d:docs/RE_HANDOFF.md`).

Use `python3 scripts/clean-generated-artifacts.py` for a **dry run** or add
`--apply` to remove ignored generated artifacts. The first 2026-10-13 cleanup sweep removed the ignored `build/` tree and
only `.obj`, `.pdb`, `.pyc`,
`.pch`, `.idb`, `.ilk` compiler binaries under ignored `.analysis/`;
**10,375 files and 726,040,253 measured bytes**, without touching tracked
code, the pinned compiler, IDA databases or the original game. The ignored
local audit is `.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`.
**Retained deliberately:** reproducible `.analysis` probe scripts, candidate
C/C++ source and copied source trees, JSON/TXT/disassembly evidence and
private analysis inputs. These remaining source/result archives were **not**
bulk-pruned; their stale experiments are non-authoritative. The older October
12 selective cleanup remains historical and is superseded by this inventory.
After a build/ cleanup, follow the restart commands above to regenerate
necessary objects; there is no missing canonical evidence simply because a
cached `.obj` or `.pdb` is absent. A clean-room rebuild **after** this removal
replayed `screen-effect-register-chain` **480/480 bytes, 55 relocations** and
`etama-controller-update` **2,216/2,216 bytes, 88 relocations** as exact;
`RunEcl` independently reproduced its complete **15,564-byte / 598-field
non-exact owner with 30 ordinary differences**. The temporary smoke-build
outputs were cleared again. These checks confirm the documented restart path,
not new exact credit or full game build closure. The ignored `.analysis` still
contains substantial historical source copies and JSON artifacts (about 1.1
GiB after compiled-binary cleanup); they are retained pending separate,
source-aware reproducibility review, **not endorsed as current conclusions**.
The cleaner preserves its first cleanup audit across later no-op calls and
appends any later removals instead of overwriting that audit. Note that
`python3 scripts/ci.py` can regenerate `build/pycache` even when no C++ file
is compiled; run the cleanup tool **after all final validation** to hand off
with no build outputs. The final post-CI sweep removed another **234** generated
Python files (**4,210,602 bytes**). The local audit now records **two sweeps,
10,609 generated files and 730,250,855 measured bytes** in total; 121 other
short-lived `scripts/`/`tests/` bytecode cache files were also cleared and are
not included in that audit. At the checkpoint, `build/` is absent and no
`.analysis/` compiler binaries remain. The next dry run should report zero.

# TH09 reconstruction handoff

> **Current-state document, not a session journal.** Refresh the live ledgers
> and object files before new reconstruction work. Historical hypotheses,
> rejected trials and previous snapshots remain in `docs/KNOWLEDGE_BASE.md`
> and Git history (including `git show a02528d:docs/RE_HANDOFF.md`). Do not
> mistake a previous commit's byte counts for the present working tree.

## Current verified handoff checkpoint

- **Repository:** `th09`, `main`; latest original-bound gameplay-source improvement is the Front weighted random window/table scheduling, building on the `88d8e40` and `a28272e` source checkpoints. The current commit must be read from live Git status; all previous strict Factory receipts remain **bound to their respective source commits**, not to these new non-crediting diagnostics. Never avoid dirty/untracked files or claim remote push from a local commit (Factory Bash cannot push).
- **Attested target:** original Japanese TH09 v1.50a, SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`. Current ledger remains **2,192 candidates / 980 authored source-present / 938 exact / 42 non-exact**; **226,285 / 275,770 authored bytes exact (82.0557%)**. The native Windows product/link/runtime gate is **still open**.
- **Last verified regression sweep:** `python3 scripts/check-boss-frontiers.py --owner all` cold-replayed **28 non-crediting boss/leaf owner diagnostics and 54 distinct canonical exact siblings**, all exact checks passing. `python3 scripts/ci.py` passed **343 tests (27 skipped)**, together with target SHA-256, tracking, docs and progress checks. The `--owner front` source-cold PE/COFF audit binds the original 693-byte weighted RNG with all 15 address operands, 6 calls and 40 CFG blocks; retained source advances one genuine five-entry group cursor and now emits **681 bytes / 201 instructions / 569 overlapping differences plus 12 missing target bytes** (previously 572+12), so **581 total nonidentical physical bytes rather than 584**. Target and source CFGs remain 40/41 blocks; 118 normalized aligned instructions. `--owner front` protects exact `FrontCalcCallback` (1,491) and three other exact Front units. Expanded `--owner player` independently audits 862-byte nonexact `CheckBulletCollision` and protects seven related exact functions. RunEcl, ExAttack, Enemy, Etama, Title, ANM, Replay, Screen, Effect and parser gates remain active. Neither nonexact callback is accepted exact; rerun all replays after future edits.
- **Most recent independently Factory-accepted improvements:** `GameManager::OnUpdate` (1,230 bytes; source commit `b8fdbde`), `PlayerCollisionQueryStateView::FindCollision` (602 bytes; source commit `86f5305`), and the relocated **already-exact** `CalcItemBoxCollision` (121 bytes; same source commit). Exact receipts and the separate receipt-only commits are detailed below. Do not double-count the moved helper or count nonexact partial improvements.
- **Factory forced-cold roadmap regressions on source commit `d1ec934` (strict-live-v1, `pass / accepted`):** `EtamaController::OnUpdate` (2,195 authored / 2,216 physical bytes), `receipt:98a21d4ff54063bf84a18084f8e8ece248fe392b03d97937cbaa5b8f5494f583`; `TitleScreenView::TitleSetupThread` (534), `receipt:fee5f86b07cf2f14def601762364ec6e4f03173f2d80d9fa314e93aa7d75fa39`; `TitleScreenView::OnUpdateMusicRoom` (2,258), `receipt:53c8446c0c314a275dc716e544222294aa353b82d88b7a2e00aa6f02a8230b3b`; `ExAttackUpdateCallbackType21` (1,074), `receipt:bd1d14e7c4cd791f917a2880f237b9ab235f4979012bda060fa01da22f5c3cac`. **Four already-exact functions independently reaccepted**, not four new exact matches or acceptance of adjacent nonexact callbacks. These receipts bind to the exact source commit, not the later documentation checkpoint.
- **Factory strict forced-cold existing-exact regressions on gameplay-source commit `8570973448fa68b2e4388a9eda2f8a7c8cade1ff` (`pass / accepted`):** `FrontCalcCallback @ 0x00417630` (**1,491 bytes**) `receipt:de0fe5d2be69f984e90317634e767f5576eba86e94ccbcf706bed97faf731e56`; `PlayerLifecycleView::CheckGrazeCollision @ 0x0041E350` (**204 bytes**) `receipt:fdeaa3e877620fb6e7131497177bb365d7c9cae340879a2fee8c17b162f80f7c`. Both are **previously exact** functions independently reaccepted against the clean committed source. These attest neither nonexact `FrontMessageRandomView::SetupRandomForSide` (682/693) nor nonexact `CheckBulletCollision` (849/862), nor a whole Windows executable or any later docs-only commit.
- **Factory strict replay of the newest retained Front cursor source checkpoint `e0f77b263108baa298faa8758ddb3e8879813e72` (`pass / accepted`):** `FrontCalcCallback @ 0x00417630` (1,491 already-exact bytes), `receipt:a735b7421be0f4572babd1581a3242151d47792cd61cb217f571c48e664ff5d9`. This is a fresh forced-cold receipt bound to the source commit with the 681-byte fallback-cursor refinement. It **does not** attest `SetupRandomForSide` as exact; that 693-original/681-candidate nonexact owner retains 572 overlapping differences, 12 missing original bytes, 40-vs-41 CFG blocks and no function/partial-byte exact credit. Subsequent documentation-only commits are **not** separately accepted source builds.
- **Most recent strict Factory forced-cold already-exact Player regression (source commit `f951317c6ae7c8b9fd8bbb822b5b210beabd38f3`, `pass / accepted`):** `PlayerLifecycleView::OnUpdate @0x0041E900`, **522 bytes**, receipt `b58119d92a0c8eec924d4c68f99c174c9219d57081caf28c177d614202c0718d`. The receipt checks the existing exact caller after the independently nonexact 1,900-byte movement source improvement and the complete owner guard; it does **not** certify UpdateMovementAndOptions as exact or later source commits.
- **Current-source Factory exact regression receipts (source commit `49da692fd7cc35a57906824a971ffbbae2c4d3aa`, strict-live-v1 forced-cold `pass / accepted`):** `EtamaController::OnUpdate` (**2,195 authored / 2,216 physical**), `receipt:877843d4d2295382acd595a7298930802c6d525b6d4683ba23354d39ac597f84`; `TitleScreenView::TitleSetupThread` (**534**), `receipt:ea3220bb4f053c296769bfd2c2b91093cc6b81c21663bce2810eb15f4b0c345e`; `TitleScreenView::OnUpdateMusicRoom` (**2,258**), `receipt:801ce70ced6d66b39b970d3620c04e01ad9070f989da7c1ac14f37b600bc39e0`; `ExAttackUpdateCallbackType21` (**1,074**), `receipt:9c608ab754af70ee605faca889a684838dbbb94c57b4f5c37bbf67eafc4e179c`. **All four functions were already exact**; these receipts do not accept neighboring nonexact Type19/21 initializers, RunEcl or native whole-image closure. Source-commit binding stays `49da692` after later note-only commits.
- **Factory strict forced-cold Title regressions at `4fe7cb06125601cf4dd583c7ab785c045e786ae5` (`pass / accepted`):** Existing exact `TitleScreenView::TitleSetupThread @0x004249E1` (534 bytes), `receipt:afc85df61447369ef8ba8860939608f140b621e327086d1f459cebe957cd80b8`, and existing exact `TitleScreenView::OnUpdateMusicRoom @0x00426E05` (2,258 bytes), `receipt:acb48c0b18a6861b4288c38a27a2d5e7244f2e54be79cd325daef6aecf6bd637`, both returned `receipt_verdict=pass` and `acceptance_decision=accepted` with actual `source_binding.git_commit=4fe7cb0` and `coldness=forced-recompile`. These source-commit-specific receipts protect already-exact Title functions after the real Title Result 484-byte source improvement and the reviewed start-menu label rebinding; they **do not** grant exactness to Title Result, cursor or Options. The whole Windows image remains open.
- **Factory forced-cold exact-regression receipts at the newest `09bb2a6ac47e3b6af1ec6fe9029428fb56f31c06` Front weighted-picker source checkpoint (strict-live-v1 `pass / accepted`):** `FrontCalcCallback` 1,491 bytes — `receipt:74198fed08b459dc9bcdef27c6d5e939afd2e1011b74527e82492ef64df2b8a2`; `EtamaController::OnUpdate` 2,195 authored / 2,216 physical bytes — `receipt:19f65147eaaaea7836cb2d705459c5486d6a4be987d4c8acd27b29babebf17c9`; `TitleScreenView::TitleSetupThread` 534 bytes — `receipt:5831f8f466f042ca5854d6c161a40bba37151c885872f3326623c3cb29f93b9e`; `TitleScreenView::OnUpdateMusicRoom` 2,258 bytes — `receipt:ec836857bcf5bdcaa7eea080398dbd623306205ea1b12091fbacf2c07b48d7f2`; `ExAttackUpdateCallbackType21` 1,074 bytes — `receipt:f722291fa5089d2088b87b74a978f34cd493aad5a49496fafe0a89b3639c5c79`. All **five already exact functions** were *independently accepted under forced cold replay* against this exact clean source commit; none is a new exact function and none is evidence that adjacent `RunEcl`, `EtamaController::OnDraw`, Front weighted picker, Title Result or ExAttack Type19/21 initializers match. The receipts do not bind later documentation-only commits.
- **Next attack order:** `RunEcl` opcode 4/155; ExAttack Type19/21 initialization; `EnemyManager::OnUpdate`, `EtamaController::OnDraw`; then Title Result/Options/Cursors and near-exact leaves. `EtamaController::OnUpdate`, TitleSetup and MusicRoom are already exact, and should be protected, not reclassified as outstanding. Previous source probes under `.analysis/` are **counterexamples**, not authoritative original C++.
- **Generated-artifact hygiene:** all reproducible validation runs precede cleanup. `python3 scripts/clean-generated-artifacts.py --apply` removes only ignored `build/` and `.analysis/` compiler binary caches (`.obj/.pdb/.pyc/.pch/.idb/.ilk`) and writes a local deletion manifest under `.analysis/handoff-cleanup-20261013/`. It **deliberately preserves** the 1.3 GiB-scale original/experimental analysis corpus (probe C++ and headers, JSON/disassembly/patches) because those may be the only reproduction records. This archive is *not* a build product. After cleanup, regenerate each COFF object by `scripts/build-match-unit.py --unit NAME` before comparison. Keep `resources/th09.exe` and `.tools/` untouched.

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
| Canonical exact functions | 938 |
| Source-present non-exact functions | 42 |
| Source-present non-exact bytes | 49,485 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 226,285 |

**226,285 / 275,770 reviewed-authored bytes = 82.0557% exact**.
To reach 95% of this currently classified denominator, another **35,697
bytes** must be exact. The 35 origin-unresolved functions are excluded from
that denominator and could alter it after review. Complete faithful Windows
i386 product/link/runtime closure is **open**; semantic reconstruction and
portability have **not started**. Source presence does not imply exact codegen;
exact codegen does not imply a runnable game.

## Active frontiers

These are **selected** source-present non-exact functions, **not** a complete
42-item inventory or a ranking by difficulty. Size is the target logical
extent unless marked *physical*. Measurements are tied to the maintained
candidate/profile and require fresh compilation after source edits. Consult
`config/functions.csv` and the relevant knowledge-base evidence first.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| RunEcl | 0x004086C0 | **15,564 physical** = 14,792 code + 772 table; 598 fields; **30** ordinary byte differences, zero field/table differences | 146/148 handler shapes; opcode 4 (timer load/push schedule, 7 bytes; actual `operator=` / `SetCurrent` body visibility tested and byte-neutral) and opcode 155 (XOR/AND/XOR now modeled, byte-load/register schedule still 23 bytes); `scripts/inspect-ecl-complete.py`; `--owner runecl` now rejects any new differing opcode, >30 ordinary differing bytes, changed target identity/fields/table, inconsistent 148-handler shape or equality status, and cold-protects exact 147-byte PopContext; `scripts/probe-ecl-variant.py --state/--control ... --output .analysis/...` preserves all seven first-declaration lexical carriers during candidate compilation |
| Front weighted message RNG | 0x004162D0 | **693 original / 681 source physical bytes, 206/201 instructions**, 15 independently bound address fields, six matching ordered calls; 40 original / **41** candidate direct blocks, **569** linked overlapping byte differences + 12 absent original bytes (**581 total**, previously 572+12 = 584); **118** normalized aligned instructions | The maintained source computes the opponent-shot window limit earlier, initializes selection to `primary`, chooses `secondary` on `sideIndex == 1` and carries the actual fallback five-entry cursor across the two groups rather than rederiving `table[selected]`. Original SHA, COFF fields, six callee identities, CFG and byte boundaries verified through `scripts/inspect-front-message-random.py` / `--owner front`, with exact 1,491-byte `FrontCalcCallback` and three related exact Front units preserved. **NON-EXACT**; source/target CFG still differs and whole runtime/port remain open. |
| Player CheckBulletCollision boss | 0x0041DFF0 | **862 original / 849 candidate physical bytes**, 277/266 complete decoded instructions, **26 original/COFF address fields**, 12 matching ordered calls, original 33 vs candidate 32 blocks, **804** overlapping linked differences and 13 missing original bytes | Newly SHA-pinned `scripts/inspect-player-bullet-collision.py` validates the entire original, every original literal/callee/data operand, all x86-encoded source fields, three `ret 12` exits, call order and direct CFG. `--owner player` now cold-builds this nonexact true Player TU while protecting **seven** already exact collision-related functions. Window/difficulty/loop rewrites of 849→865 or 863 bytes still miss target CFG/codegen; no gameplay source promotion, **NON-EXACT**. |
| ECL MoveRandomBiased leaf | 0x00407E30 | **516 target / 517 candidate**, 28 independently bound fields, 14 ordered calls, 24/24 blocks; **148 overlapping differences and 1 extra byte** | Original `LEA ECX,[EAX+4]` / twice `[ECX]` versus candidate `MOV ECX,EAX` / twice `[ECX+4]`; new strict `scripts/inspect-ecl-biased-movement.py --explain`, **NON-EXACT** |
| EnemyManager OnUpdate | 0x00410730 | **3,900 physical**, 1,026 instructions, 97 bound fields, 69 ordered calls, 170 direct blocks, **39** differences | The `--owner enemy` cold gate independently pins original SHA-256 `a7f01c2f…56f1bc75`, protects four reviewed residual windows (+0x19A..0x1A7, +0x251..0x2C0, +0x843..0x853, +0x9BC..0xA82) with 2/11/10/16 respective maximum differing bytes, checks every original field/call/table/CFG and rejects conflicting total/equality indicators. A 21-form draw-head pointer/index panel and 15-form trail-hitbox/lifetime panel were neutral or regressed. The **82-byte** collision leaf now separately SHA-pins `e2be7998…2ff9c0` and protects its +0x1A..0x3F residual/call/ABI gate; 18 true caller/receiver/scaled-size variants failed to improve its **31** differences. Exact siblings: attached-effect update (**151**) and shared draw (**1,758**); four calc register/copy windows remain. |
| ExAttack Type19/21 initializers | 0x0044A030 / 0x0044B220 | 763 / 727 bytes; 57 / 53 fields; **6 / 9** differences | Type19/21 share their typed init/update layout and Type21 exact Update is protected. Independent standalone-with-real-helper TU reproduces six/nine residuals. Typed u-field cursor/loop-reference experiments are identical when scoped per iteration, but declaring a persistent `float* u` cursor changes code extent to 775/784 (Type19) or 735/732 (Type21) and often breaks full CFG. The original one-instruction vertex/history setup reorder remains. Independent original SHA-256 hashes `fc7b630c…563cf67` / `b5271179…a523ca47` and complete linked differing-offset windows Type19 +0x1BB..+0x1C0 / Type21 +0x1C2..+0x1CA are now strictly gated (not merely an aggregate 6/9 count). `scripts/inspect-exattack-trail-init.py` and cold `--owner exattack` protect all 57/53 fields, 27/25 calls, 9/10 blocks and matching exact Type21 update; **NON-EXACT**. |
| ExAttack Type14/22 Draw | 0x00444130 | 959 target / 974 candidate; 53 fields, 20 direct + 12 indirect calls; **9/9 verified jumps, 43 aligned change spans** | All Type14/22 init/update/draw callbacks now share 0xF48 Extra and the original 0x38 record prefix in `src/ExAttackType1422ExtraView.hpp`; Type22 alone owns parameter +0x38; Draw register/loop scheduling still open; `scripts/inspect-exattack-type14-22-draw.py --explain`, **NON-EXACT** |
| ExAttack Type19 Update | 0x0044A330 | **821/821 bytes, 254/254 instructions, 35 independently bound fields, 11 ordered calls, 30/30 matching blocks**, **198** byte differences | **Three adjacent source fields at target field positions 23–25 are reordered**, although the complete 35-field inventory agrees. The original carries the Player-slot pointer across the `Float3` subtract call; current source does not. Cold `scripts/check-boss-frontiers.py --owner type19` independently protects the exact **318-byte Type19/21 Draw** consuming the same Extra. Fresh C++ register/owner/slot layouts did not improve complete equality; **NON-EXACT**. |
| Player position Type4/Type8 | 0x00443B10 / 0x00447620 | **249 target / 250 candidate** per callback; 8 independently bound fields, 6 ordered calls, same 6-block CFG | Original positive branch carries the FromAngleMagnitude receiver in ECX and uses `MOV EDX,ECX`; VC7.1 candidate rematerializes it with `LEA EDX,[EBP-28h]`. Real exact 32-byte `Float3::FromAngleMagnitude` body visible before/after each callback still emits 250 bytes; helper visibility alone is insufficient. `scripts/inspect-player-position-type48.py --type 4/8`; both **NON-EXACT**. |
| ExAttack Type3 Update | 0x00442750 | **780/780 bytes**, 38 independently ordered fields, **14** differences (previously 15) | Shared `ExAttackHistory32ExtraView.hpp` with Type3/Type4 init/update; target writes middle Z **after** middle-Y call and **before** tail-X load, but candidate emits the Z store **before** middle-Y call. All 24 calls / 17 direct blocks still agree; SHA-pinned `scripts/inspect-exattack-type3-schedule.py` and strict `--owner type3` now bound all 14 differing bytes to original +0x18E..0x19C, protect the exact original Y-call/Z-store/tail-X chronology and exact 454-byte Type3 initializer. Eighteen true zero-write/lifetime variants did not close this schedule. **NON-EXACT** |
| Etama OnDraw | 0x00413BE0 | **600/600** physical bytes, 27 fields, 13 ordered calls; **17/17 matching direct blocks**, **268** differing bytes (previously 594/600, 465 overlapping + 6 absent, 18/17 blocks; interim dual-cursor 271) | Review-backed Laser and body VM position dual cursor restores `+0x208` anchor and loop graph; same valid body owner now supplies `inUse`, `angle` and position. Still **NON-EXACT**; target alpha publication uses a `MOV BYTE [laser+0x497], 0xFF`, but changing only the actual color high-byte source shrinks the whole candidate 600→594 and regresses 17-block CFG. Combined owner/field alias matrices also regress or remain neutral. Keep true byte behavior uncredited pending whole-owner identity; 13 fresh complete Laser/body-position loop-cursor variants shrink or expand the owner or break the matching 17-block graph (retained under ignored `.analysis/gpt-web-20261015-etama-loop/`). Protect 600 bytes/27 fields/13 calls/17 blocks with strict `--owner etama`; `scripts/inspect-etama-draw.py --json`. |
| Title OnUpdateResult | 0x004266B5 | **485 original / 484 source physical bytes**, **147/147 instructions**, 34 independently bound fields, 9 ordered calls and matching 40-block CFG; **218 overlapping unequal bytes + 1 absent original byte** (previously 483 source bytes, 146 instructions, 224+2) | Preserved a pre-call character snapshot while using a real `char *` to the selected `g_TitleResultCharacterOrder` slot for the cursor argument; VC7.1 emits one character load, naturally delays it beyond the `PUSH 2`, and recovers the 147th instruction. The remaining difference is still a real ECX low-byte vs EAX pointer/register-scheduling issue, **NON-EXACT**. Cold full original SHA/field/call/CFG gate now allows only 484–485 bytes, 147 candidate instructions, <=218 unequal overlapping bytes and <=1 missing byte. The `TitleScreen.cpp` local-label renumbering is independently original-address audited: 9 changed names with unchanged targets/sections, followed by full **37/37** same-profile exact unit COFF+byte comparisons including OnUpdateStartMenu 1,592/1,592; no partial or new exact credit. |
| Title DrawResult | 0x00423D16 | **1,939/1,939** bytes, **577/577 instructions, 82 independently decoded/bound fields**, 16 ordered calls, 92 matching blocks, **22** differences | The `--owner title` cold replay now fails closed if any full owner length/field/call/92-block identity regresses, 574/577 aligned instructions drop, or 22 ordinary differences increase. Original score-bank pointer computation precedes `currentScreen` load; pinned VC7.1 hoists that read before the store. One-time volatile store and currentScreen-read models remain 1,939/22, 1,939/38 or 1,941–1,949 bytes; no codegen credit. `scripts/inspect-title-result-draw.py --json`. |
| Title character cursor Active/Reverse/Inactive | 0x00424EDD / 0x00424FB0 / 0x0042505F | **211/215**, **175/175**, **175/175** target/candidate bytes; 6/2/2 independently decoded fields, 4/2/2 ordered calls | Active now has 4 extra bytes and 185 overlapping differences (previously 217/189+6); its rebuilt natural dual VM cursor and post-SetSprite VM reload retain the five-block target graph. Reverse/Inactive remain at **38** physical differences each, with **5-target/4-candidate blocks** despite equal extents. Source direct-array, prechecked-loop and count-stack-slot controls are rejected; `scripts/inspect-title-character-cursors.py --variant active/reverse/inactive` is **NON-CREDITING**. |
| Title DrawReplaySave | 0x004239F6 | **800 target / 793 candidate**, 41 original fields, 4 ordered calls, 28/28 blocks with unresolved edges | Renderer source remains NON-EXACT; `scripts/inspect-title-replay-save-draw.py --json` now shares full decoded field-identity gate |
| AnmManager camera-facing projection | 0x00439820 | **622/622 physical bytes, 201 instructions, 40 independently bound fields, 11 ordered calls, 8 matching blocks, 24 ordinary differing bytes** | The actual maintained method now lexically follows exact `TranslateRotation` in the Render TU; its true `Float3` delta assignment and later origin lifetime naturally recover the 622-byte target extent. `scripts/inspect-anm-camera-facing.py` remains **NON-CREDITING**; `--owner anm` now protects exact `TranslateRotation` (49), `Draw2D` (491), `DrawCameraFacingQuad` (88) and `Project3DQuad` (595). The historical retail TU grouping and 24 residual stack home/scheduling differences are still unresolved. |
| Replay BeginPlaybackStage | 0x00420840 | **540 target / 538 candidate physical bytes, 152/151 decoded instructions, 32 target/source address fields, 31 aligned field identities, five matching ordered calls, 12/12 identical direct control blocks; 49 overlapping differences and two missing target bytes** | New strict SHA-256-pinned `scripts/inspect-replay-begin-playback.py` independently validates all original fields and target `ret` ABI; `--owner replay` cold-builds the real no-EH ReplayInput TU and replays exact `BeginRecordingStage` (1,323) and `Create` (361). The live source hoists the SideState0 global read before writing `g_ReplayHeaderValue1E7` and folds the final seed zero-extension into `MOVZX`; the original has an extra `XOR EDX; MOV DX`. Genuine 15-form output/reference/qualification and 14-form terminal inputDelay/seed controls remain unchanged or regress, so no source change/exact credit was made. |
| ScreenEffect CalcShakeEnvelope | 0x004227A0 | **336/336** bytes, 132 instructions, 20 reviewed fields, 8 ordered calls, 26/26 direct blocks; **28** linked byte differences | `scripts/check-boss-frontiers.py --owner screen` now builds the real ScreenEffect TU cold, checks every original operand and graph through `scripts/inspect-screen-shake-envelope.py`, rejects byte/codegen regressions, and protects **six canonical exact** same-TU callbacks (316, 103, 123, 186, 163, 13 physical bytes). Genuine independent hold/fade reads and endpoint sum are preserved; ESI/EDI receiver/timer coloring remains **NON-EXACT**, with no fractional credit. |
| Title Options | 0x004276EB | **2,045 original / 2,048 candidate bytes**, **551/554 instructions**, **135 independently original-decoded and COFF-bound address operands**, **51 ordered calls**, **121/121 identical blocks**, three extra source bytes | `scripts/inspect-title-options.py --json` now SHA-pins the original complete PE owner and redecodes every source field, call and `ret` ABI. Target tail reuses EBX zero before the 0x80 scroll check; current source generates late EBX zero and separate switch-case register coloring. Rewriting case 6/7/8 as if/else can incidentally reach target *length*, but breaks graph/field scheduling. Cold `--owner title` now also rejects unreviewed complete source byte/instruction length, >610 overlapping differences, changed 540/551 aligned instruction baseline or field/call/121-block regression; remains **NON-EXACT**. |
| Controller input | 0x0042B410 | 808 target / 796 candidate; 32 fields | WinMM X/Y register and read ordering; `scripts/inspect-controller-input.py` |
| Supervisor InsertReceivedFrame | 0x0042E9E0 | **213/213 bytes, 67/67 instructions, 9/9 blocks, zero relocations/calls**, **35** different bytes | Shares `SupervisorFrameQueueStorageView.hpp` with exact InsertPredictedFrame (191) / PopFrame (119). Its complete original SHA-256 and `ret 16` ABI are now checked independently under cold `scripts/check-boss-frontiers.py --owner network`. Prefix 16-bit field scheduling and side-base register lifetime remain unresolved; near-match is **NON-EXACT**. |
| Network ParseNetworkConfigValue | 0x00432240 | **243/243 bytes; 116/116 decoded instructions; one independent `strncmp` call at 0x0047CC30; 33 original versus 34 candidate CFG blocks; 76 differing bytes** | SHA-pinned `scripts/inspect-network-config-parser.py` binds the exact target and sole COFF field/callee/RET8 ABI and rejects wrong call, hash, sizes or invented equality. `--owner parser` cold-builds its true TU and replays exact `PrepareTitleMode4Network` (679 bytes, 22 fields). The original has an inline newline-return before the general value-copy scanner; VC7.1 lowers the same case after the scanner. Switch, conditional and goto natural forms are byte-neutral; next work must restore the original code layout. **NON-EXACT**. |
| Effect AddedCallback | 0x0040D1C0 | **51/51 bytes, 19/19 instructions, 4 independently bound fields, 2 ordered calls and 3/3 blocks**, **8** differing bytes | Original loads global ANM manager directly into EDI before binding the owner to ESI. Current compiler reverses that scheduling; branch/early-return/re-entrant pointer views compile identically or regress. SHA-pinned `scripts/inspect-effect-added.py`; `--owner effect` protects complete exact **OnUpdate 475, OnDraw 183, OnDrawShared 146** in the same TU. **NON-EXACT**. |
| Enemy collision-box leaf | 0x00410250 | 82 bytes, 2 direct calls, **31** differing bytes | Shares exact receiver declaration with 3,900-byte EnemyManager OnUpdate; Float3 return-copy schedule still open; SHA-pinned `scripts/inspect-enemy-collision-box.py` and strict full 82-byte `--owner enemy` leaf guard reject new differences beyond the reviewed register/copy window |
| FileSystem TryDecryptFromTable | 0x0042C290 | **220/220 bytes, 77/77 instructions, 12 physical fields, 2 ordered calls, 11/11 matching control blocks**, **38 linked byte differences** | Original SHA-256, three signature bytes and **8×12-byte original parameter table** checked independently. The genuine `FileSystem::Decrypt` (259-byte exact) and `Encrypt` (267-byte exact) are now cold-replayed alongside this source-present **NON-EXACT** table selector with `scripts/check-boss-frontiers.py --owner crypto`; wrong table/addend/call/ABI or false exact attribution is rejected. |
| GameManager CleanupGameplayState | 0x0041B5C6 | **425-byte original / 406-byte current source** (previously 399), **117/114 instructions**, **43 original / 41 candidate COFF fields**, **12 ordered calls**, **40 aligned operand identities**, **104 aligned instructions** (was 100); **24/25 CFG blocks**, 209 bound overlapping byte differences (was 225) | Natural local final-limit clamp restores the real mode-2 conditional `JGE` / min-limit assignment at +0x9C. Exact GameManager OnUpdate 1,230 bytes and all nine same-TU exact siblings remain intact. SHA-pinned `scripts/inspect-game-manager-cleanup.py` and `scripts/check-boss-frontiers.py --owner gamemgr` protect this **NON-EXACT** owner. |
| Gameplay setup worker | 0x0041AF2D | **1,689/1,689 bytes**, original **413** vs candidate **416** instructions, original **172** vs candidate **171** independently decoded address fields, **35 matching ordered calls**, **155 aligned identities / 13 differing field spans**; **78/77** direct blocks | Must compile its real `GameManagerSetup.cpp` owner with **`/Os`**, as in exact GameManager OnUpdate; the distinct exact Supervisor loading-VM sibling uses **no `/Os`** and yields a misleading **1,812-byte** setup worker. Original SHA-256 and profile are now gated by `scripts/inspect-gameplay-setup.py --json` and cold `scripts/check-boss-frontiers.py --owner gamemgr`. Divergent mode-0/1 timed-state and pointer-reg lifetimes remain **NON-EXACT**. |
| Player UpdateMovementAndOptions | 0x0041C170 | **1,835 code / 1,900 complete physical bytes** (1 NOP + 64 bytes of two 8-entry switch tables); target/candidate **489 instructions, 66 fully bound fields, 23 ordered calls**, 129 different linked owner bytes (previously 144), 433 normalized aligned instructions (previously 422) | The real `/O2 /Ob1 /Oy- /Gr` Player TU now snapshots only the first `SpawnEffectInFixedSlot` call's side index before evaluating its manager receiver, while still reloading the side owner for the second call. Cold `scripts/check-boss-frontiers.py --owner movement` now SHA-pins the entire 1,900-byte original/COFF owner, fail-closes above 129 differences and separately replays exact **522-byte** `player-on-update` caller. The standalone `scripts/inspect-player-movement.py` still exposes the complete difference offsets. Other input-focus, bound-clamping and second effect-call rewrite controls are byte-neutral or regress; **NON-EXACT**. |
| Supervisor network service | 0x00430AA0 | 1,633 target / 1,632 candidate, 108 source fields | Now shares the exact Supervisor queue +0x47C/+0x56C physical storage projection; 1,395 linked differences and call scheduling remain; **NON-EXACT** |
| Supervisor OnUpdate | 0x00431110 | 844 authored / 996 physical target; **952-byte** candidate (800 code + 152 switch data), **92/92 external fields, 27/27 calls** | RegisterStateObject physical registration tail merged from 3 source calls to **1**, matching target ECX=0/1, but 791 overlapping differences + 44 absent bytes, **NON-EXACT**; `scripts/inspect-supervisor-update.py` |

Other unresolved owners (including PauseMenu, Replay, network and remaining
Title helpers) live in `config/functions.csv`. The exact short-function subset
is maintained separately in [`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md).

## New Factory-accepted Player collision boss and moved exact callee

`PlayerCollisionQueryStateView::FindCollision @ 0x0041D810`, **602/602 complete owner bytes**, **221 original instructions, nine original-bound COFF external fields, seven ordered calls, and the original `ret 0x0C` ABI**. The real `CalcItemBoxCollision` exact 121-byte source body is now visible in the **same** `src/PlayerCollisionQuery.cpp` translation unit. Together with a correctly scoped early `PlayerCollisionQueryRecordView *record` cursor, the pinned `/O2 /Ob1 /Oy- /Gr` VC7.1 compiler preserves ECX across the item-box success arm, eliminating the source-only `MOV ECX,[ESI]` and its two excess bytes. Strict independently cold canonical comparison reports `exact` on 602/602 complete bytes, original SHA-256 `6efe45e4437bf346cc8616739b1b135f65be92bc235032e442c73b4341845745`. The moved `CalcItemBoxCollision` remains **121/121 exact**; all five other exact source-unit collision helpers remain unchanged and independently exact. No duplicate definition, padding, inline assembler or arbitrary register forcing was used. Canonical `player-find-collision` manifest binds every original call/data field by actual offsets and destination; `scripts/check-boss-frontiers.py --owner player` cold-replays **seven** exact owners. Factory **`strict-live-v1` forced-cold** replay on the exact clean-source commit `86f5305bda32a141bd8c4801af5ed793258fdcbf` completed **`pass / accepted`** for both: `FindCollision` (receipt:41858ea3caf4fd74a5ddf37c7b685132fd744c61fb1adce1d9beb2b9d7424db5; job `job:a37fb3ae1c394d9fb289b51159c40940`), and relocated existing `CalcItemBoxCollision` (receipt:eca3dc5e1f98536fbe918ad2de2186c9b6b7bc2d407a967784bca2b03ee9989e; job `job:a7943c5ac1f64d19ba7f076491d9c338`). These are separately verified function-level source-bound codegen claims, not whole-game runtime/product acceptance. This is source reconstruction exactness, not game runtime/product closure.

## Newly accepted GameManager boss

**New Factory-accepted big-boss exact:** `GameManagerSetupLayout::OnUpdate @ 0x0041AA5F–0x0041AF2C`, **1,230/1,230 complete bytes**, original **337 instructions, 87 actual address fields, 16 target-ordered direct calls**. Pinned VC7.1 `/O2 /Os /Oi /Ob1 /Oy- /Gr` source exactness uses five real `const unsigned short&` aliases of distinct `g_ReplayInputStates[2]` members, then ten unchanged state0/state1 publications. This restores original AX/CX/DX/DI/SI data capture and defers original `XOR ESI,ESI` to target +0xB5. Full canonical `game-manager-on-update` manifest contains every original PE field identity/COFF addend, and all nine GameManagerSetup exact siblings have been revalidated across their two actual compiler profiles. Factory strict-live-v1 forced-cold replay on the clean original-source commit `b8fdbdea16ae5fe1f9a58c1166f534147e9c60ba` returned **`pass / accepted`** (receipt:6355df202f338b7403e1bafc11bc23d59a2b94857987983f87cef7f67d7591f8; job `job:1382e08cff9a496c89d7900639c148c2`). This is function codegen exactness, not Windows product/runtime closure.

## Closed codegen landmarks and accepted receipts

**New Factory-accepted exact:** `TitleDrawCallback @ 0x00424898`, **329/329 bytes, 20 original relocation identities, 12 calls and 26 complete blocks**, canonical `title-draw-callback` strict-live forced-cold replay on source commit `3bb7693`, **`pass / accepted`**, `receipt:42a46abe45f4902fb86aaa94b10235c26ece9be45578de2920fa6cd3db208d35`. The natural low-screen switch cases 3/4/5/6 recover the original two consecutive `DEC EAX` without additional effects. This is an accepted *function* receipt, not product or runtime closure.

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
# Etama, ExAttack Type3/19/21, Title updater/renderer and character-cursor owners.
# Includes the Enemy collision leaf, camera-facing projection, received network
# frame insertion, EffectManager/Type19/21 Draw, one additional GameplaySetupThread
# boss diagnostic, GameManager Cleanup, crypto-table selector, and thirty-one exact sibling regressions including GameManager and FileSystem.
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

This handoff was checked on **2026-10-14** against the original executable,
the ledgers and fresh pinned VC7.1 comparisons. The current source count is
**980 source-present / 938 exact / 42 non-exact**, *not* the older
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

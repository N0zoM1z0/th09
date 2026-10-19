# TH09 reconstruction — active handoff

> **Current-state routing, not a history log.** Read the live Git tree and
> original-bound ledgers before trusting any number here. Historical experiments,
> previous handoff revisions and older Factory receipts remain in
> [the historical checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md),
> [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md) and Git history. Do not treat a source-present or
> near-exact owner as accepted exact.

## Current verified handoff checkpoint

**Use the ledgers and the source tree as the authority, not prior session
estimates.** The original target is Japanese TH09 v1.50a SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The current maintained source ledger is **980 authored/source-present, 942
canonical exact, 38 nonexact**. The reviewed-authored-byte objective is
**95%**; whole executable linkage, native gameplay and portability remain
separate **unproven** milestones.

- **New local canonical exact owners:** ExAttack Type19/21 initialization
  at `0x0044A030` / `0x0044B220`, **763/727 complete bytes and 57/53 original
  fields**, now pass `exattack-type19-init` / `exattack-type21-init`. Natural
  `history3E8[sample + 1]` indexing retains the separate vertex cursor and
  closes both former preheader gaps under the unchanged VC7.1 profile.
  This adds **1,490 exact authored bytes**; no Factory receipt is claimed.
- **Latest bounded source improvement:** Type19 update's history ring now uses
  `history3E8[sample + 1]` while retaining the vertex cursor. Its complete
  **821-byte /35-field /254-instruction** owner improves **198 to 190**
  differing bytes, recovering **eight bytes without any newly differing byte**.
  The vertex-cursor save now precedes history-address materialization, as in
  the original. Full original-SHA/residual-offset protection preserves all
  **631 matching bytes**. This remains a nonexact owner with no count credit.
- **Latest C++ data-owner correction:** the already exact StartMenu's
  **0xCC-byte** snapshot now reads the genuine `g_Supervisor.config388`.
  Its original `+0x47` address operand stays **0x004B3488**, represented by
  root **0x004B3100 +0x388**, replacing one independent private global
  declaration. The complete **1,560 code +32 table bytes /107 fields**
  remain exact; nine private labels retain independently checked original
  destinations. This adds no exact-count credit.
- **Previous Title focused regression:** `--owner title` passed **eight original-bound
  diagnostics and 46 distinct canonical exact function checks**. Both actual
  `TitleScreen.cpp` compiler carriers retain **38 exact owners /18,411 physical
  bytes /943 fields**. CI passes **663 tests, 28 skipped, zero failures**.
  Source-bound witnesses and recipes are below
  `.analysis/gpt-61sol-20261019-options/`. After validation, removed
  **41 generated files /2,855,840 bytes**; compact evidence remains.
- **Current focused regression:** `--owner type19` passes the complete
  original-bound update and separately cold-replays the **318-byte exact**
  shared Type19/21 draw. The unchanged ECL core was also cold-built: **20
  existing exact owners /10,205 physical bytes** pass, with full RunEcl still
  **15,564 bytes /598 fields /30 differences**. Six ordinary timer-forwarding
  controls are neutral; signed timeout-bitfield controls disturb original
  fields/tables and are rejected. Type19 vertex-index and magnitude/step-pointer
  controls regress; history indexing alone provides the accepted eight-byte
  improvement. Recipes and complete results remain below
  `.analysis/gpt-61sol-20261019-{ecl-forward,type19-history}/`. CI passes
  **666 tests /28 skipped**; post-validation cleanup removes **127 generated
  files /2,629,856 bytes**, retaining source-bound recipes and comparisons.
- **Most recent Factory-accepted source checkpoint:** `7324b4e` (`gpt-web: restore native ANM
  and Supervisor ABI across MusicRoom callbacks`). It preserves the **2,258
  byte exact** MusicRoom update and **209 byte exact** drawing function while
  correcting 29 original-bound C++ symbol identities. Factory separately
  accepted those two functions and the real **139-byte exact**
  `AnmManager::DrawTextLeft`, all bound to `7324b4e`. Receipts are in
  [`RE_ACCEPTED_RECEIPTS.md`](RE_ACCEPTED_RECEIPTS.md). Earlier accepted
  receipts belong **only to their named source commits**.
- **Previous full regression, before indexed-history source:** the source-bound `--owner all`
  report passed **35 noncrediting large/leaf diagnostics and 257 distinct
  complete existing exact function checks**. CI passed **659 tests, 28
  skipped, zero failures**. Both witnesses are retained under ignored
  `.analysis/handoff-cleanup-20261019/`; these are not 257 new acceptances.
- **Largest unresolved bosses:** `RunEcl` **15,564 bytes/598 fields, 30
  residual bytes** (opcode 4: seven; opcode 155: 23); Etama `OnDraw`
  **600 bytes, 268 unlike** (`OnUpdate` already exact **2,216/2,216**);
  Title `DrawResult` **1,939 bytes, 22 unlike**; Enemy `OnUpdate`
  **3,900 physical bytes, 29 unlike**. Keep big functions
  and small leaf functions in scope; focused commands are below.
- **Other open work:** Replay `BeginPlaybackStage` **540 original/538 source**,
  Effect `AddedCallback` **51 bytes/eight unlike**, PauseMenu **1,776
  original/1,732 source**. No near match contributes exact credit.
- **Proven cross-TU source identities:** genuine `AnmManager`, `AnmLoaded`,
  `SoundPlayer`, `ZunMemory`, `ZunTimer` and global runtime math are reused
  across Etama, ECL, Title, Replay, ExAttack and other callers. Changes to
  shared headers can rename original-VA-equivalent compiler-private `$L`
  symbols in *other* TUs: never auto-rewrite those without cold PE/COFF
  owner-relative relocation checks. Full chronology and failed hypotheses
  are in [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md).
- **Operator contract:** preserve and investigate *all* dirty changes;
  never reset them just to find a clean path. Test complete function bytes,
  address operands, original compiler profile and exact siblings; use
  `gpt-6.1-sol: ...` for every substantive local Git commit. A Git checkpoint
  or local replay is **not** a Factory acceptance. Do not assume remote
  Git has been pushed from this shell.
- **Verified handoff cleanup:** 1,570 proven disposable cache
  files / **131,018,261 bytes** were removed *after* the 35/257 original
  owner and 659-test CI witness. `build/` and Python bytecode caches were
  removed at that checkpoint and may be regenerated by current work; private PE/toolchain, tracked source, unique probe
  C++/INL and original-bound JSON remain. See
  [`RE_ARTIFACT_CLEANUP_HISTORY.md`](RE_ARTIFACT_CLEANUP_HISTORY.md) for
  file-by-file audit, and cold-build a match unit before any COFF inspection.

## Next worker: first three work packages

1. **Large ECL/Etama:** attack **`RunEcl` opcode 4 (7 bytes) and opcode 155
   (23 bytes)** against the immutable 15,564-byte/598-field owner. The ECL
   compiler's first-include/switch table context is essential; individual
   five-byte snippets are not acceptance. For Etama, **`OnUpdate` is already
   2,195 authored / 2,216 physical bytes exact**; the open boss is
   **`EtamaController::OnDraw`**, 600/600 physical with 268 differences and
   two interdependent Laser cursor roles. Guard `--owner runecl` and
   `--owner etama` plus exact sibling units, respectively.
2. **TitleScreen and ExAttack:** protect exact **MusicRoom Update** and
   **TitleSetup** while investigating **`DrawResult`'s 22 byte offsets**,
   ReplayMenu's **1,280/1,258** and **72/70** CFG split, and options/tail
   callbacks. ExAttack Type19/21 initializers are now **763/727 bytes exact**;
   Type3 update **15 bytes**, Type14/22 Draw **959/974**, and Type19 update
   are still open. `--owner title`, `--owner exattack` and `--owner type3`
   run the original-bound comparisons.
3. **Leaf and independent cross-check:** keep **Effect `AddedCallback`
   (51 bytes, 8 differences)**, Supervisor received-frame insertion
   (213 bytes, 35 differences), and ECL `MoveRandomBiased` (516 vs 517)
   in scope. The separately accepted 220-byte FileSystem table-decrypt
   selector is already exact; do not revert its real original one-past-row
   access to an unverified safe-looking rewrite. See
   [`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md) and exact
   manifests before selecting further leaves.

**Local Player visibility countercheck (source `b761e2d`, October 19):**
`python3 -B scripts/probe-player-position-visibility.py --output
.analysis/player-position-visibility` cold-compiles Type4/8 in standalone and
genuine ECL contexts. Both remain **250 versus 249 bytes**, with **132
overlapping differences plus one extra byte** and all eight original fields
bound. The exposed real `Float3::FromAngleMagnitude` itself is **32/32 exact**;
merely making its definition visible leaves both caller bodies and fields
unchanged. This falsifies that narrow proposed fix, not all possible source
contexts. The tool preserves the void ABI/profile and removes its temporary
compiler files. No gameplay source, exact ledger or acceptance changed.
For this local campaign use direct IDA Pro MCP/Bash and `gpt-6.1-sol: ...`
commit subjects. The bounded experiment cleanup removed **38 generated files /
2,214,529 bytes**, retaining compact recipes/results under
`.analysis/gpt-61sol-20261019-exact/`; the 659-test CI passed (28 skipped).

**Rule:** keep experimental C++ in ignored `.analysis/` until a complete
original-linked COFF comparison passes; do not edit `config/matches.csv` or
advance 942 exact merely because normalized, partial, or source-level metrics
improve. Always inspect existing dirty edits (there may be only this worker),
then commit verified source/guard work as `gpt-6.1-sol: ...`. The currently clean
handoff deliberately removes generated OBJ/PDB/build caches; the next run
**must rebuild** affected match units before invoking inspectors.

## Live ledger snapshot

The canonical state is `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv`, and `config/match-units.toml`, checked by
`python3 scripts/validate-tracking.py --require-target` and
`python3 scripts/validate-docs.py`. The current restart summary is
**980 source-present / 942 exact / 38 non-exact**; earlier operator notes
mentioning 979/876/103 are obsolete for this checkout.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,192 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 980 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 980 |
| Canonical exact functions | 942 |
| Source-present non-exact functions | 38 |
| Source-present non-exact bytes | 47,693 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 228,077 |

**228,077 / 275,770 reviewed-authored bytes = 82.7055% exact**. To reach
95% of the *currently classified* denominator, another **33,905 bytes**
must become complete original-bound exact owners. The 35 origin-unresolved
functions are excluded and could change the denominator. Similarity fractions
and locally matching subsequences never count as accepted exact.

## Active frontiers

Prioritize **large bosses as well as small leaves**. Each row refers to a
maintained, authored, **non-exact** function in `config/functions.csv`.
Comparisons are current-source/original PE cold baselines; lengths are logical
unless explicitly marked physical. The ledger, not this selected table, lists
all 38 open functions. For the complete <=256-byte cohort see
[`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md).

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode 4: 7 bytes; opcode 155: 23 bytes, 146/148 handler shapes. Opcode 4 now directly calls the separately exact real `ZunTimer::operator=(int) @0x00401500` on the proven 12-byte context timer, rather than fake second-class timer ABI; existing genuine `RngRuntimeView`/global `g_Rng` remains. **15,564 complete instruction/table bytes and 598 source-linked original fields are unchanged**, with all **26 exact ECL/ExAttack functions across the declared carriers** (12,097 physical bytes) protected. `--owner runecl`; exact credit remains open. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**, 97 fields, 69 calls, 170 blocks; **29** differing bytes | Genuine `Float3::operator/` and actual `AppendBoxRecord` compiler visibility close the ten-byte `+0x845..+0x850` copy window. The special-attack dispatch now also invokes **real `EtamaController::SpawnBulletPatternPrimary @0x004130F0`** rather than a fake private receiver, with the actual 0x214-byte descriptor's target-backed field layout. Original-linked byte/field/call/graph identity and 29 differences are unchanged. `--owner enemy` now cold-protects **five existing Enemy/Float3/collision/Draw exact siblings plus the separately exact 203-byte Etama callee and 28-byte descriptor constructor** (seven total). |
| **EtamaController::OnDraw** | 0x00413BE0 | **600/600 physical**, 27 fields, 13 calls, 17 blocks; **268** differences | Laser body-VM position cursor ESI still open. The final original call is now typed as `EffectManager::DrawBulletLayerEffects` with its separately **169/169 exact** real-member source at `0x0040D110` and side owner at `+0x0C`; all original linked bytes remain unchanged. Complete SHA/offset guard preserves the 332 equal bytes; `--owner etama` protects **both exact same-carrier helpers plus the separately exact OnUpdate (3 checks)**. |
| **PauseMenu::OnUpdate** | 0x00434740 | **1,776 original physical / 1,732 candidate**; 46 calls; 10-way switch; **1,453** overlapping differences + 44 missing | First seven-VM cursor improvement committed in `7f1d56e`; remaining branch/VM scheduling. `--owner pause` independently checks 11 already-exact Ascii/ANM functions and real original table/ABI. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | Now uses genuine shared `g_Supervisor.framerateMultiplier5B8 @0x004B36B8` and the four **real `GameManagerResetView::coordinate360/364/368/36C` fields** (original `0x004A80F0..FC`), not fake external playfield globals. The independent **79-byte exact GameManager coordinate initializer** physically writes those fields; 1,900-byte linked movement boss and 66 operand identities stay unchanged, with 129 ordinary differences still open. All 129 genuine current original-bound mismatch offsets are now pinned, protecting the other **1,771** already matching bytes (including both switch tables and after-return alignment); future improvements must not introduce mismatches elsewhere. `--owner movement` protects exact 522-byte caller. |
| **Front weighted message RNG** | 0x004162D0 | **693 target / 681 candidate**, 15 fields, 6 calls, **569** overlap + 12 missing | Primary-first table selection refined in `09bb2a6`; target/candidate CFG 40/41 blocks. `--owner front` protects exact FrontCalc and siblings. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions, 34 fields, 9 calls; **218** overlap + 1 missing | Pointer-backed character snapshot in `4fe7cb0`; original CL/EAX codegen still differs. Protect all 37 existing exact `TitleScreen.cpp` units, including privately relabeled StartMenu. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280 target / 1,258 source bytes**, 376/368 instructions, **57/57 original decoded/COFF operands**, **13/13 ordered calls**, **72/70 CFG blocks** | Complete original-SHA-bound source-cold diagnostics now run via `--owner title`; the candidate is still **22 bytes short and NON-EXACT**. The shared replay record is exact-sized but does not imply the function is exact. |
| **TitleScreen::DrawReplaySave** | 0x004239F6 | **800 target / 799 source**, 245/245 instructions, 41 fields, 4 calls, 28/28 original-direct blocks | Shares the ReplayInput-proven 0x1EC record with DrawReplayMenu. Real validity-shared text pointers and frame factor improve **793→799** physical bytes, **241→245** instructions and recover the complete original CFG (formerly divergent). SHA-bound noncrediting 799/245/41/4/28 guard plus `--owner title`; still missing one byte and NON-EXACT. |
| **TitleScreen::DrawResult** | 0x00423D16 | **1,939/1,939**, 82 fields, 16 calls, 92 blocks; **22** differences | All 22 SHA-pinned linked differences are in the entry: 21 at `+0x0E..+0x24`, one at `+0x39` (bank-pointer store vs `currentScreen` prefetch). The entire remaining owner is byte-exact; the strengthened guard now pins **all 22 exact residual positions**, including preserving previously matching `+0x13` and `+0x18` inside the old entry window. `--owner title` protects the original full owner. |
| **TitleScreen::OnUpdateOptions** | 0x004276EB | **2,045 / 2,048**, 135 fields, 51 calls, 121 blocks | The 2,048-byte boss now uses genuine `AnmManager`/`AnmLoaded` and `SoundPlayer` receivers for **29 reviewed original-linked fields** rather than private C++ fake managers. Full codegen/call/CFG identity unchanged; 3 extra candidate bytes in input/confirmation menu remain NON-EXACT. Exact same-TU `PlayMenuSound` 34/34 and `--owner title` are required regressions. |
| **Title character cursors** | 0x00424EDD / 0x00424FB0 / 0x0042505F | Active target/candidate **211/215**; Reverse/Inactive **175/175** | Active reduced 217→215 in `69165b2`, but none is exact. Loop pre-check and VM register/sprite reload schedules; `--owner title`. |
| **GameManager CleanupGameplayState** | 0x0041B5C6 | **425 / 410**, 43 / 42 fields, 12 calls, **210** overlap; 24/24 blocks | **Recovered two real limitB4 memory writes** with first store before signed lower-bound `JGE` and conditional second store of 1. Previous 406/41 candidate only stored once and had 25 blocks. Still 15 bytes short and NON-EXACT; `--owner gamemgr` checks original/source publication traces and ten exact siblings. |
| **ExAttack Type3 update** | 0x00442750 | **780/780**, 38 fields, 24 calls, 17 blocks; **15** differences | Source now places middle-Z assignment after the middle-Y call; VC7.1 still hoists the tail-X load before the store (non-exact). The old 14-difference candidate wrongly wrote Z before the Y call. Strict chronology/offset guard; `--owner type3`. |
| **ExAttack Type19 update** | 0x0044A330 | **821/821**, 35 fields, 11 calls, 30 blocks; **190** differences (198→190) | History indexing restores the vertex-cursor save before history-address materialization, recovering eight bytes with no regressed byte. All 631 matching bytes are protected by full original SHA/residual offsets. Record/extra register roles and Player-slot lifetime remain open; `--owner type19` or `--owner exattack`. |
| **ExAttack Type14/22 draw** | 0x00444130 | **959 / 974**, 53 full PE/COFF fields, 20 direct + 12 indirect calls, 9 jumps, 905 overlapping differences and 15 extra source bytes | SHA-bound `--owner exattack` **now cold-replays this real owner**, individually binds all 53 fields and both call kinds, and prevents any new mismatch in its 54 already matching original bytes; three additional exact Type14/22 initializer/update controls. Remaining draw register/loop schedule stays NON-EXACT. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689/1,689**, 172/171 fields, 35 calls, 78/77 blocks; **911 differing bytes** (down from 921) | Real `setupMode==0` non-mode-2 early exit recovers ten original-linked bytes and raises field identity alignment 155→157. The remaining 172-original / 171-source field gap is now independently decoded by **referent and multiplicity**: original-only `g_GameManager+0x00` (3), `+0x34` (1), `+0xA4` (1); candidate-only `+0x28` (1), `+0x98` (1), `+0xB4` (2). The target's side-registration loop anchors ESI to `side.flags (+0x34)` while VC7.1 prefers `side.selector (+0x28)`; 28 source-cold cursor/alias models were neutral or regressed. Still NON-EXACT; full 911-residual-byte and original/COFF-referent guards plus ten exact siblings run via `--owner gamemgr`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **804** overlap + 13 missing | Three return exits, gameplay/state collision branches; `--owner player` protects seven exact helpers. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Scoped volatile read of `g_ReplaySideState0` after header 1E7 publication restores all 32 original field referents in the correct order and all **first 30 physical field positions**, instead of the previous one reversed field pair. Nine register-scheduling residual positions `+0x152,+0x156,+0x15C,+0x162,+0x168,+0x16C,+0x172,+0x178,+0x17A` and 30 seed/tail bytes `+0x1FB..+0x1FE,+0x200..+0x219` remain guarded. All **499 already matching** candidate bytes are protected; tail fields are still shifted -1/-2 bytes. `--owner replay` source-cold replays all **12 exact members across both pinned compiler profiles**; still NON-EXACT. |
| **ANM camera-facing quad** | 0x00439820 | **622/622**, 201 instructions, 40 fields, 11 calls, 8 blocks; **4** differences (13→6→4) | Typed three-vector scratch plus complete three-word copy keeps source and original semantics and now reproduces **618 original physical bytes**. The final two real local stores are scheduled Z-before-Y instead of retail Y-before-Z, leaving exact residual offsets `+0xF6,+0xF7,+0xF9,+0xFA`. This improves whole-owner physical equality from the earlier six-residual baseline, deliberately moving two mismatches within the same copy; all 618 currently matching original bytes are now strictly protected. All **15 same-Render-carrier exact functions** and separately compiled exact `Project3DQuad` (**16 strict checks**): `--owner anm`. No new exact function credit. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | Authentic VC7.1 order of two real volatile references (fade, then hold; same hold + fade value) recovers original physical **+0x5C and +0x5F** with no regressed byte. Full original-SHA/26-offset guard protects **all 310 already matching bytes**; all **18 ScreenEffect exact owners** (2,562 bytes) pass across the genuine three profiles. Remaining EDI/ESI owner allocation is NON-EXACT. |
| **Controller input** | 0x0042B410 | **808 / 796**, 32 fields; **102** overlap differences /12 missing bytes | First **674 complete instruction bytes** match. WinMM axis midpoint/range CSE and register chronology remain open; split helpers, POD bounds, SDK value/reference types and loop controls have not closed the owner. Use `scripts/inspect-controller-input.py`. |
| **Supervisor network service** | 0x00430AA0 | **1,633 / 1,632**, 108 source fields | Real queue ownership and 1,395 overlap differences still open; `--owner network`. |
| **Supervisor::OnUpdate** | 0x00431110 | **844 authored / 996 target physical**, 952 candidate physical | 92 external fields, 27 calls but 44 missing physical bytes; `--owner network`. |
| **ECL MoveRandomBiased leaf** | 0x00407E30 | **516 / 517**, 28 fields, 14 calls, 24 blocks; 148 overlap + 1 extra | Genuine shared RNG method/global owner symbols now independently original-bound; Y-pointer target `LEA [EAX+4]` versus candidate `MOV EAX` remains unresolved. `--owner runecl`. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Uses genuine `AnmManager`/`AnmLoaded*` types. The SHA-pinned full-owner guard permits narrowing only the target-proven `+0x03..0x09,+0x0D` residuals, protecting the other 43 matching bytes and three same-TU exact sibling checks; `--owner effect`. |
| **Supervisor received-frame leaf** | 0x0042E9E0 | **213/213**, 67 instructions, 9 blocks; **35** differences | Target packed-input/seed 16-bit store register order; `--owner network`. |
| **Network-config parser leaf** | 0x00432240 | **243/243**, 116 instructions, one strncmp call; **76** differences, 167 equal bytes | Original newline-clear block occurs before token scanning; candidate puts it after. Full original SHA/76-residual-offset guard now protects all 167 matching bytes, one call/field, 33/34 blocks; `--owner parser` protects the exact 679-byte caller. |


The independent original PE and `th09-ida` CODE-xref census at `0x004343D0`
verifies all 27 folded `Float3` helper code callsites: 18 calls from 17
exact functions, nine calls from seven nonexact functions. These are callsite
counts, not 27 distinct caller functions. Run `python3 scripts/audit-float3-conversion-calls.py
--json` for target-hash/callsite/owner evidence; the folded physical callee
cannot be assigned unique C++ authorship from calls alone. The source-present
`MoveRandomBiased` 517/516-byte difference remains open.

Additional non-exact owners (e.g. Player charge/update/added state,
replay save, title replay drawing and network updates) remain in the **38-row
live source-present non-exact ledger**, not in a historic handoff tally.
Review `docs/KNOWLEDGE_BASE.md` and current source before rerunning old negative
experiments; unsupported guesses there are not target evidence.

## Exact landmark routing

The canonical manifest and accepted original-bound comparison remain the
**sole authority** for the **942 exact** functions. The independently accepted
Enemy collision wrapper at 0x00410250 is **82/82 exact**; the 3,900-byte
Enemy OnUpdate caller is **still 29 bytes nonexact**. The former wrapper's
original acceptance and several representative source-commit-bound exact
receipts, including Float3 division, are preserved in
[the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md).
No sibling receipt upgrades a source-present non-exact large owner.

The two ExAttack initializers are now canonical exact through
`exattack-type19-init` and `exattack-type21-init`. The original 33-slot history
covers the center at zero plus ring samples 1..32. Their complete PE owner
hashes, all **110** address operands and unchanged real helper ABI are bound
in the manifests; both diagnostic guards now reject **any** byte difference.
This is a local canonical reconstruction claim, separate from the archived
Factory acceptance plane.

## Active shared Etama/Effect ANM resource owner

The complete original Japanese TH09 v1.50a still requires **RunEcl**,
Etama `OnDraw`, remaining ExAttack update/draw callbacks and leaf owners.
The Type19/21 initializers are independently canonical exact; no adjacent
nonexact owner inherits their credit.

- `EtamaController::AddedCallback` and `EffectManager::AddedCallback` both
  read the **same target-proven `AnmLoaded` pointer slot `+0x04`** after
  `GetAnm(8)` before deciding whether to load `"etama.anm"`. The source now
  includes one `AnmLoadedResidentView.hpp` with independently asserted
  **eight-byte prefix and +0x04 pointer** in both original callbacks; the
  Etama-private duplicate and Effect's untyped `void **` cast are gone.
- Source-cold entire original comparison leaves exact `DrawSingleBullet`
  **232/232 physical bytes, 16 COFF fields** and exact Etama
  `AddedCallback` **832/832 physical bytes, 30 fields**. The shared-header
  change renumbered **15** compiler-private `$L` labels across these two
  functions; every single label has been independently verified against its
  **unchanged original PE target address** and strict exact-replayed before
  publishing the renamed COFF manifest. The Etama **600-byte** large
  `OnDraw` remains **600/600, 27 fields, 13 original calls, 17 matching
  direct blocks and 268 unlike bytes**, with separately exact **2,216-byte
  OnUpdate** protected in `--owner etama`.
- Effect `AddedCallback @0x0040D1C0` stays **51 original/candidate bytes**, four
  authentic fields, preserved call/graph and **8 unlike bytes**. The new
  0x04 view is byte-neutral here; it is not a new exact claim. The Windows
  whole-build/runtime gates remain open.
  Independent Factory forced-cold source-commit-bound replay at `74b973e`
  returned **pass / accepted** for four previously exact functions:
  Etama `AddedCallback` (832), `DrawSingleBullet` (232), Etama `OnUpdate`
  (2,216), and Effect `OnUpdate` (475). Full receipt IDs and immutable
  source binding are archived in `docs/RE_ACCEPTED_RECEIPTS.md`. The
  adjacent **nonexact** Etama/Effect callbacks do not inherit acceptance.

## Restart and verification

Read [`AGENTS.md`](../AGENTS.md), [`RE_WORKFLOW.md`](RE_WORKFLOW.md), and
[`ORACLES.md`](ORACLES.md) before mutations. Factory/Web calls the attested
`th09-ida.get_metadata({})`; local `scripts/check-ida-mcp.py` requires a direct
IDA setup and is not a Factory identity check.

```bash
git status --short --branch
git diff && git diff --cached
python3 scripts/verify-target.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
# For a focused boss/leaf source change, choose --owner runecl, exattack,
# enemy, etama, title, pause, movement, player, etc. before the broad sweep.
python3 scripts/check-boss-frontiers.py --owner all
python3 scripts/ci.py
python3 scripts/validate-docs.py
python3 scripts/progress.py --check
git diff --check
# `build/` is generated and may have been removed by handoff cleanup.
python3 scripts/build-match-unit.py --unit title-screen-setup-thread
python3 scripts/compare-coff-function.py --unit title-screen-setup-thread --json
# Preview disposable output. Use --apply only after all codegen/CI checks.
python3 scripts/clean-generated-artifacts.py
```

Local canonical exactness requires a cold build and complete original-byte/
relocation replay of its declared match unit. Factory acceptance separately
requires a source-commit-bound **accepted** receipt; local `--owner`, CI and
Git commands do not establish that acceptance.

## Evidence and cleanup boundaries

`docs/KNOWLEDGE_BASE.md` holds older experiments, negative hypotheses and
original-bound observations; the current Git tree plus the ledger is
normative. Earlier session details are frozen in
[the historical checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md)
(and in Git history), never a second competing active-state table.

The original target under `resources/`, pinned toolchain under `.tools/`,
tracked source, exact manifests and non-reproducible evidence are **not**
cleanup targets. The ignored `build/` tree and compiler `.obj/.pdb/.pyc/.pch`
artifacts inside `.analysis/` may be regenerated by cold scripts. The existing
`python3 scripts/clean-generated-artifacts.py --apply` writes its individual
path/byte audit to `.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`.
Old ignored experiment C++/INL, JSON provenance, hypotheses, scripts and
unknown legacy data are not automatically disposable; they require an
individual producer/reproducibility/reference review. **Never bulk-delete
`.analysis/`**. After cleanup, rebuild match units before asking for a fresh
COFF comparison; never mistake a missing disposable `.obj` for lost source.


### Most recent generated-cache cleanup (2026-10-19)

The last **full** reviewed cleanup is documented in
[`RE_ARTIFACT_CLEANUP_HISTORY.md`](RE_ARTIFACT_CLEANUP_HISTORY.md) with exact
ignored per-path proofs under `.analysis/handoff-cleanup-20261019/`.
The cleaner's cumulative historical manifest lives at
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`;
**only its last `runs` entry** identifies the current deletion.

| Verified removed, October 19 | Files | Logical bytes |
| --- | ---: | ---: |
| Ignored `build/` | 523 | 18,442,237 |
| Compiler binaries/bytecode in `.analysis/` | 997 | 111,683,705 |
| Two separately SHA-256-reviewed Python caches | 50 | 892,319 |
| **Total removed** | **1,570** | **131,018,261 (~125.0 MiB)** |

**Post-clean original target SHA and Git source checks passed**, and the
cleanup dry-run found **zero remaining selected artifacts**. Deliberately
preserved around **1.6 GiB** of historic ignored C++/INL/JSON evidence,
the actual `resources/th09.exe`, pinned toolchains, and every tracked file.
A missing `build/` is intentional. Recreate only the match unit required by
an investigation; do not run `git clean -X` or delete `.analysis/` wholesale.

**Validation after cleanup:** restrict checks to documentation, ledger and
Git commands with `PYTHONDONTWRITEBYTECODE=1 python3 -B`; running the full
CI or a new `--owner all` rebuilds the exact cache that was intentionally
removed. The original-bound **35/257** complete-source and **659-test CI**
proofs were executed and archived *before* deletion, not inferred from a
Git commit or post-clean similarity report.

# TH09 reconstruction — active handoff

> **Current-state routing, not a history log.** Read the live Git tree and
> original-bound ledgers before trusting any number here. Historical experiments,
> previous handoff revisions and older Factory receipts remain in
> [the historical checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md),
> [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md) and Git history. Do not treat a source-present or
> near-exact owner as accepted exact.

## Current verified handoff checkpoint

Read the live ledgers first. The original Japanese v1.50a target SHA-256 is
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Current state: **1,035 authored, 1,035 source-present, 1,000 canonical exact**;
**35 source-present nonexact; no confirmed authored owner lacks maintained source**.
**243,428/291,883 reviewed-authored bytes = 83.3992%**.
The **95%** milestone still needs **33,861** complete exact authored bytes.
The last accepted addition is Player initialization: **552 authored bytes**,
independently replayed over its complete572-byte physical owner in `8ff6ce1`.
This checkpoint makes Type3 replay selective and fail closed on stale reports,
and records bounded original-owner countermodels, **no exact credit**.
Native product/runtime, complete inventory and later gates remain open.

- **Type3 update @0x00442750:** maintained source stays780 bytes/38 fields/15
  differences. A private actual middle-Z qualified store plus tail-X word read
  restores original Y-call -> Z-store -> tail-X-load chronology,at52 differences.
  A fully defined inline utility forwarding all three real collision queries
  reduces this to **four** differences,with240 instructions,24 original calls
  and17 matching direct blocks. Every comparison covers the complete owner.
- **Four-byte candidate is not adopted:** offsets+1B2,+1B7,+247,+249 were all
  previously equal. Target uses ECX versus candidate EAX for the tail-Y float
  argument,and EDX versus ECX for the motion-reference argument. The unchanged
  production guard correctly rejects these regressions. No residual window is
  widened,no maintained C++ or ABI is changed,and no partial credit is assigned.
- **Bounded controls:**48 private full-TU Type3 builds distinguish actual Y/Z
  stores,word/float transport,query interfaces,native vectors and real callee
  visibility. The latest eight retain four/seven differences or emit798 bytes
  with37 fields. The real SDK += body is36/36 original-equal but its caller
  loses the required motion-call boundary; it is rejected. All eight received-
  frame field-publication combinations retain213/0/35 with identical raw bytes.
- **Reproduction:** `nice -n 10 python3 -B scripts/probe-exattack-type3-word-order.py
  --case forwarded-collision --output .analysis/type3-word-order` cold-builds
  only the four-difference candidate. Default `--case all` retains the three
  baseline15/ordered-word52/forwarded-query4 models. All three generated-source
  hashes are unchanged; the selected cold replay reproduces every original-bound
  raw byte/actual COFF row. A failed build cannot leave a stale success report.
  Wrong targets preserve prior evidence and stop before compiling. Three pure
  tests check those failures,one-build scope and unchanged guard rejection.
- **Evidence:** the four .analysis/gpt-61sol-20261020 packets named
  type3-argument-transport,ecl-context-publication,result-paired-accesses and
  queue-paired-stores retain24 frozen models and complete raw/actual-COFF pairs.
  They share21/66/9/10 unique inputs respectively. Original disk/direct IDA/
  entry and distributed mapped-byte checks pass. All1000 canonical proofs
  retain unchanged source/profile bindings; broad replay is unnecessary.
- **Include routing:** staging all63 ECL inputs preserves every raw byte but
  renumbers two private table labels. Actual COFF coordinates still prove
  original40C088/40C0A0. A frozen replay using the original eight-file routing,
  with the other55 hashes verified,now reproduces every actual COFF row and
  the full15564/598/30 original comparison. No manifest spelling is changed.
- **Cleanup:** after all producers stop,a TH09-cwd-scoped check permits removing
 244 reproducible files/4716726 bytes. After terminal CI,362 cache files/6140146
  bytes are removed. Build/ and own OBJ/PDB are absent; unrelated evidence,
  target/toolchain/IDA inputs and the frozen sources remain.
- **Validation:** CI791 tests/28 skips,target-required tracking,documentation,
  generated progress and whitespace pass.728/729 previous engineering inputs
  retain parent hashes; only the Type3 producer changes and one test file is
  added. All485 game sources,1026 units and1000 exact rows are unchanged.
  Local commit and independent remote verification are recorded separately.
- **Open large frontier:** RunEcl remains15564/598/30: opcode4 seven residual
  bytes,opcode155 twenty-three. Both current and target opcode155 use masked
  XOR; byte zero-extension and old/result register allocation remain unresolved.
  Qualifying the actual context-slot read increases differences to33; a genuine
  inline byte-member timeout update stays30,while its word form emits15560 and
  fails the whole-owner gate. DrawResult pointer publication gives1939/38;
  one screen snapshot,alone/paired,gives1941 and is rejected. No model adopted.
- **Operator contract:** direct local IDA Pro MCP and Bash;
  **`gpt-6.1-sol: ...`** commits followed by push and independent remote
  verification. Compile serially at lowered priority, replay affected cohorts,
  and clean only own reproducible outputs after producers stop.


## Next worker: first three work packages

1. **Large Effect/ECL/Etama:** **draw29@0x00448750 /2,220 bytes** now has
   maintained nonexact source in src/EffectDrawType29.inl. Start from its2205-byte
   repeated-expression model, full-byte guard and bounded fidelity evidence.
   Resolve local-center ECX reloads, stack homes and branch layout before
   promoting exactness. The prefix has no missing catalog entry; full
   executable inventory remains unproven. Alongside this recovery,
   keep the existing large-code goal: attack **`RunEcl` opcode 4 (7 bytes) and opcode 155
   (23 bytes)** against the immutable 15,564-byte/598-field owner. The ECL
   compiler's first-include/switch table context is essential; individual
   five-byte snippets are not acceptance. For Etama, **`OnUpdate` is already
   2,195 authored / 2,216 physical bytes exact**; the open boss is
   **`EtamaController::OnDraw`**, 600/600 physical with 268 differences and
   two interdependent Laser cursor roles. Guard `--owner runecl` and
   `--owner etama` plus exact sibling units, respectively.
2. **TitleScreen and ExAttack:** protect exact **MusicRoom Update** and
   **TitleSetup** while investigating **`DrawResult`'s 22 byte offsets**,
   ReplayMenu's newly restored full **1,280/1,280** owner and **29 selected-row
   residual bytes**, plus options/tail callbacks. ExAttack Type19/21 initializers are now **763/727 bytes exact**;
   Type3 update **15 maintained differences / four-byte private candidate**,
   Type14/22 Draw **959/974**, and Type19 update
   are still open. `--owner title`, `--owner exattack` and `--owner type3`
   run the original-bound comparisons.
   Reverse/Inactive's **175/18** cursor candidates remain unadopted: they damage
   ten previously equal positions. `scripts/probe-title-cursor-bound.py` reproduces
   that result; entry register roles still need resolution against the 175/38 source.
3. **Leaf and independent cross-check:** keep **Effect `AddedCallback`
   (51 bytes, 8 differences)**, Supervisor received-frame insertion
   (213 bytes, 35 differences), and ECL `MoveRandomBiased` (516 vs 517)
   in scope. The separately accepted 220-byte FileSystem table-decrypt
   selector is already exact; do not revert its real original one-past-row
   access to an unverified safe-looking rewrite. See
   [`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md) and exact
   manifests before selecting further leaves.

**Current Player context countercheck:**
`python3 -B scripts/probe-player-position-visibility.py --output
.analysis/player-position-visibility` compares the same maintained reference
fragment in isolation and through the actual ECL include. Standalone remains
**250/249 bytes with 132 overlap differences plus one extra byte**; real ECL
is **249/249 exact** for both callers, with all eight fields bound. The exposed
void angle body itself remains **32/32 exact**. This tests the complete current
context, not an isolated attribution to one callee. Pre-reference value-source
results under `c5f2c31` changed two raw byte positions between contexts but had
identical size/difference counts. The earlier `b761e2d` “unchanged” report is
source-scoped history, not a universal negative. Temporary compiler files are
removed automatically; the compact snapshot includes untracked new source and
excludes deleted old paths while hashing actual bytes, including line endings.

**Rule:** keep experimental C++ in ignored `.analysis/` until a complete
original-linked COFF comparison passes; do not edit `config/matches.csv` or
advance the exact count merely because normalized, partial, or source-level metrics
improve. Always inspect existing dirty edits (there may be only this worker),
then commit verified source/guard work as `gpt-6.1-sol: ...`. The currently clean
handoff deliberately removes generated OBJ/PDB/build caches; the next run
**must rebuild** affected match units before invoking inspectors.

## Live ledger snapshot

The canonical state is `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv`, and `config/match-units.toml`, checked by
`python3 scripts/validate-tracking.py --require-target` and
`python3 scripts/validate-docs.py`. The current restart summary is
**1035 source-present / 1000 exact / 35 non-exact**; earlier operator notes
mentioning 979/876/103 are obsolete for this checkout.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,248 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 37 |
| Confirmed authored | 1,035 |
| Classified exclusions | 1,176 |
| Source-present authored mappings | 1,035 |
| Canonical exact functions | 1,000 |
| Source-present non-exact functions | 35 |
| Source-present non-exact bytes | 48,455 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 243,428 |

**243,428 / 291,883 reviewed-authored bytes = 83.3992% exact**. To reach
95% of the *currently classified* denominator, another **33,861 bytes**
must become complete original-bound exact owners. The 37 origin-unresolved
functions are excluded and could change the denominator. Similarity fractions
and locally matching subsequences never count as accepted exact.

The catalog is not proof of complete executable-code inventory. The formerly
uncatalogued **0x0040D677 /0x00444A78** Timer references now belong to the exact
Effect 1/2 and Effect 34 updates. All **203** captured assignment callsites have
catalogued owners; that bounded census proves neither complete executable
coverage nor a return ABI. The Effect 34 **178-byte initializer at 0x00444960** is now exact; its
**946-byte draw at 0x00444AF0** is a maintained, explicitly nonexact frontier.
Do not silently treat uncatalogued code as excluded.

## Active frontiers

Prioritize **large bosses as well as small leaves**. Each row refers to a
maintained, authored, **non-exact** function in `config/functions.csv`.
Comparisons are current-source/original PE cold baselines; lengths are logical
unless explicitly marked physical. The ledger, not this selected table, lists
all35 maintained nonexact functions, including the2,220-byte authored Type29
draw whose maintained candidate remains nonexact.
For the complete <=256-byte cohort see
[`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md).

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode 4: 7 bytes; opcode 155: 23 bytes, 146/148 handler shapes. Opcode 4 now directly calls the separately exact real `ZunTimer::operator=(int) @0x00401500` on the proven 12-byte context timer, rather than fake second-class timer ABI; existing genuine `RngRuntimeView`/global `g_Rng` remains. **15,564 complete instruction/table bytes and 598 source-linked original fields are unchanged**, with all **77 exact ECL/Enemy/position/math functions across two profiles** (24,374 physical bytes) protected. `--owner runecl`; exact credit remains open. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**, 97 fields, 69 calls, 170 blocks; **29** differing bytes | Genuine `Float3::operator/` and actual `AppendBoxRecord` compiler visibility close the ten-byte `+0x845..+0x850` copy window. The special-attack dispatch invokes **real `EtamaController::SpawnBulletPatternPrimary @0x004130F0`** with its target-backed 0x214-byte descriptor. Fourteen Timer/descriptor calls now use seven shared native symbols; the corresponding private aliases are removed. Original-linked byte/field/call/graph identity and 29 differences are unchanged. `--owner enemy` now cold-protects **77 exact shared-TU functions, the separate draw and seven genuine external callees** (85 exact checks). |
| **Effect Type29 draw** | 0x00448750 | **2,220 /2,205 bytes**,129 original referents,72 ordered calls,41/41 physical blocks; **1170 overlap differences +15 missing** | Maintained src/EffectDrawType29.inl preserves flag-scoped main plus six satellite fans. Bounded original/native-helper emulator comparison covers all672 reachable original instructions across26 cases; it is not byte-exact or whole-product validation. The full-byte guard protects1035 equal positions. Source presence is independent of exactness; `--owner effect29` protects77 shared exact functions across two profiles. |
| **Effect 34 draw** | 0x00444AF0 | **946/959 physical**, 53 source-linked original referents, 20 direct +12 COM calls, target/candidate 16/15 blocks; **593 overlap differences +13 extra bytes** | Exact native initialization (178) and update (208) protect the original 0xEF8 buffer: fan33 at +0x54, four line32 groups at +0x2E8, four random33 rows at +0xCE8. The genuine ECL/math context removes two redundant receiver reload bytes from the 961-byte standalone candidate. All 353 equal original bytes and exact D3D arguments are protected by `--owner effect34`; byte-register alpha/cursor homes and loop layout stay open. |
| **EtamaController::OnDraw** | 0x00413BE0 | **600/600 physical**, 27 fields, 13 calls, 17 blocks; **268** differences | Laser body-VM position cursor ESI still open. The final original call is now typed as `EffectManager::DrawBulletLayerEffects` with its separately **169/169 exact** real-member source at `0x0040D110` and side owner at `+0x0C`; all original linked bytes remain unchanged. Complete SHA/offset guard preserves the 332 equal bytes; `--owner etama` protects **both exact same-carrier helpers plus the separately exact OnUpdate (3 checks)**. |
| **PauseMenu::OnUpdate** | 0x00434740 | **1,776 original physical / 1,732 candidate**; 46 calls; 10-way switch; **1,453** overlapping differences + 44 missing | First seven-VM cursor improvement committed in `7f1d56e`; remaining branch/VM scheduling. `--owner pause` independently checks 11 already-exact Ascii/ANM functions and real original table/ABI. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | Now uses genuine shared `g_Supervisor.framerateMultiplier5B8 @0x004B36B8` and the four **real `GameManagerResetView::coordinate360/364/368/36C` fields** (original `0x004A80F0..FC`), not fake external playfield globals. The independent **79-byte exact GameManager coordinate initializer** physically writes those fields; 1,900-byte linked movement boss and 66 operand identities stay unchanged, with 129 ordinary differences still open. All 129 genuine current original-bound mismatch offsets are now pinned, protecting the other **1,771** already matching bytes (including both switch tables and after-return alignment); future improvements must not introduce mismatches elsewhere. `--owner movement` protects exact 522-byte caller. |
| **Front weighted message RNG** | 0x004162D0 | **693 target / 681 candidate**, 15 fields, 6 calls, **569** overlap + 12 missing | Primary-first table selection refined in `09bb2a6`; target/candidate CFG 40/41 blocks. `--owner front` protects exact FrontCalc and siblings. |
| **Front added initialization** | 0x00418480 | **706/706**, 37 original fields, 21 calls, 19 equal blocks; **222 differences** (236→222) | Explicit Z stores and the actual source-vector reference restore 14 physical positions while preserving all 470 previously equal bytes. All 484 equal bytes are guarded. Two private method aliases are replaced by real Reset/Load calls; a shared runtime prefix preserves 0x1D70 and +0/+4/+8/+0C. `--owner frontadded` protects the whole callback plus four exact methods /832 bytes /41 fields. Receiver and remaining panel-store scheduling stay open. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions, 34 fields, 9 calls; **218** overlap + 1 missing | Pointer-backed character snapshot in `4fe7cb0`; original CL/EAX codegen still differs. Protect all 37 existing exact `TitleScreen.cpp` units, including privately relabeled StartMenu. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280/1,280 bytes**,376/376 instructions,57 original-bound fields,13 ordered calls,72 matching direct blocks; **29 differences** | Native indexed list reads, date/name-before-character evaluation, record-before-index acquisition and later timer snapshot recover1,180 more equal bytes. All1,251 matching positions and complete geometry/CFG are now protected. Remaining +185..+321 differences are selected-row ECX/EDX choices and two local-store order. `--owner replaymenu` cold-checks this isolated TU; still NON-EXACT. |
| **TitleScreen::DrawReplaySave** | 0x004239F6 | **800/800 physical**, 245 instructions, 41 fields, 4 calls, 28/28 matching blocks; **96 linked differences** | Raw replay magic, genuine SDK subtraction/two scales/addition and two-byte character construction restore the full extent and nine more loop-counter/FILD home bytes. All **704** equal positions are SHA-bound and protected; no old matching byte regresses from the 105-residual source. Remaining vector/text homes, replay-pointer and keyboard schedules stay NON-EXACT. No enlarged buffer, extra storage or new target helper claim. |
| **TitleScreen::DrawResult** | 0x00423D16 | **1,939/1,939**, 82 fields, 16 calls, 92 blocks; **22** differences | All 22 SHA-pinned linked differences are in the entry: 21 at `+0x0E..+0x24`, one at `+0x39` (bank-pointer store vs `currentScreen` prefetch). The entire remaining owner is byte-exact; the strengthened guard now pins **all 22 exact residual positions**, including preserving previously matching `+0x13` and `+0x18` inside the old entry window. The shared native GameManager field at+0x3F now has its actual COFF symbol/root/addend identity guarded; superseded mode/name-index aliases are rejected. `--owner title` protects the original full owner. |
| **TitleScreen::OnUpdateOptions** | 0x004276EB | **2,045 / 2,048**, 135 fields, 51 calls, 121 blocks | Uses genuine `AnmManager`/`AnmLoaded` and `SoundPlayer` receivers for29 fields, plus **eight native input array references**: two receivers at `g_ReplayInputStates[2]` and six16-bit word06 reads. Strict root/addend checking rejects fake standalone globals even at equal effective addresses. All2,048 linked candidate bytes and135 referents are preserved; 610 overlap differences+3 extra bytes remain NON-EXACT. Exact same-TU `PlayMenuSound`34/34 and `--owner title` protect this frontier. |
| **Title character cursors** | 0x00424EDD / 0x00424FB0 / 0x0042505F | Active target/candidate **211/215**; Reverse/Inactive **175/175** | Active reduced 217→215 in `69165b2`, but none is exact. Loop pre-check and VM register/sprite reload schedules; `--owner title`. |
| **GameManager CleanupGameplayState** | 0x0041B5C6 | **425 / 410**, 43 / 42 fields, 12 calls, **210** overlap; 24/24 blocks | **Recovered two real limitB4 memory writes** with first store before signed lower-bound `JGE` and conditional second store of 1. Previous 406/41 candidate only stored once and had 25 blocks. Still 15 bytes short and NON-EXACT; `--owner gamemgr` checks original/source publication traces and ten exact siblings. |
| **ExAttack Type3 update** | 0x00442750 | **780/780**, 38 fields, 24 calls, 17 blocks; **15 maintained differences** | New private word-read plus real collision-forward model restores original middle-Z chronology and has **four differences** at +1B2,+1B7,+247,+249. These are previously equal bytes, so it is not adopted and the production guard stays unchanged. Reproduce with `probe-exattack-type3-word-order.py`. Real shared `AnmVm` and five canonical interrupts remain; `--owner type3` protects seven exact functions. |
| **ExAttack Type19 update** | 0x0044A330 | **821/821**, 35 fields, 11 calls, 30 blocks; **44** differences (198→190→44) | ECL's genuine ordinary Float3 callee visibility and the retained native GameManager Player slot recover 146 more bytes, with all fields in original order and no newly differing byte. All 777 matching bytes are protected. Remaining Record/Extra and magnitude-cursor register roles stay open; `--owner type19` protects both shared ECL compiler profiles and the separate draw. |
| **ExAttack Type14/22 draw** | 0x00444130 | **959 / 974**, 53 full PE/COFF fields, 20 direct + 12 indirect calls, 9 jumps, 905 overlapping differences and 15 extra source bytes | Twelve device fields use the genuine `Supervisor g_Supervisor +8`; the old fictitious global type is rejected. Original-bound bytes, fields and calls remain unchanged. `--owner exattack` protects all 54 equal original positions and the three exact Type14/22 initializer/update controls. Remaining draw register/loop schedule stays NON-EXACT. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689/1,689**, 172/171 fields, 35 calls, 78/77 blocks; **911 differing bytes** (down from 921) | Real `setupMode==0` non-mode-2 early exit recovers ten original-linked bytes and raises field identity alignment 155→157. The remaining 172-original / 171-source field gap is now independently decoded by **referent and multiplicity**: original-only `g_GameManager+0x00` (3), `+0x34` (1), `+0xA4` (1); candidate-only `+0x28` (1), `+0x98` (1), `+0xB4` (2). The target's side-registration loop anchors ESI to `side.flags (+0x34)` while VC7.1 prefers `side.selector (+0x28)`; 28 source-cold cursor/alias models were neutral or regressed. Still NON-EXACT; full 911-residual-byte and original/COFF-referent guards plus ten exact siblings run via `--owner gamemgr`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **804** overlap + 13 missing | Three return exits, gameplay/state collision branches; `--owner player` protects seven exact helpers. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Scoped volatile read of `g_ReplaySideState0` after header 1E7 publication restores all 32 original field referents in the correct order and all **first 30 physical field positions**, instead of the previous one reversed field pair. Nine register-scheduling residual positions `+0x152,+0x156,+0x15C,+0x162,+0x168,+0x16C,+0x172,+0x178,+0x17A` and 30 seed/tail bytes `+0x1FB..+0x1FE,+0x200..+0x219` remain guarded. All **499 already matching** candidate bytes are protected; tail fields are still shifted -1/-2 bytes. `--owner replay` source-cold replays all **12 exact members across both pinned compiler profiles**; still NON-EXACT. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | Authentic VC7.1 order of two real volatile references (fade, then hold; same hold + fade value) recovers original physical **+0x5C and +0x5F** with no regressed byte. `probe-screen-shake-context.py --variant timer-float` reproduces the measured 332-byte rejection: the actual four-byte conversion body stays exact, but the caller retains an x87 value across that call instead of spilling it. Full original-SHA/26-offset guard protects **all 310 already matching bytes**; the unchanged **18 ScreenEffect exact owners** (2,562 bytes across three profiles) reuse their source-bound proof. Remaining EDI/ESI owner allocation is NON-EXACT. |
| **Controller input** | 0x0042B410 | **808 / 796**, 32 fields; **102** overlap differences /12 missing bytes | First **674 complete instruction bytes** match. WinMM axis midpoint/range CSE and register chronology remain open; split helpers, POD bounds, SDK value/reference types and loop controls have not closed the owner. Use `scripts/inspect-controller-input.py`. |
| **Supervisor network service** | 0x00430AA0 | **1,633 / 1,632**, 108 source fields | Real queue ownership and 1,395 overlap differences still open; `--owner network`. |
| **Supervisor::OnUpdate** | 0x00431110 | **844 authored / 996 target physical**, 952 candidate physical | 92 external fields, 27 calls but 44 missing physical bytes; `--owner network`. |
| **ECL MoveRandomBiased leaf** | 0x00407E30 | **516 / 517**, 28 fields, 14 calls, 24 blocks; 148 overlap + 1 extra | Genuine shared RNG method/global owner symbols now independently original-bound; Y-pointer target `LEA [EAX+4]` versus candidate `MOV EAX` remains unresolved. `--owner runecl`. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Uses genuine `AnmManager`/`AnmLoaded*` types. The SHA-pinned full-owner guard permits narrowing only the target-proven `+0x03..0x09,+0x0D` residuals, protecting the other 43 matching bytes and three same-TU exact sibling checks; `--owner effect`. |
| **Supervisor received-frame leaf** | 0x0042E9E0 | **213/213**, 67 instructions, 9 blocks; **35** differences | Target packed-input/seed 16-bit store register order. All **178** equal physical positions are protected; `--owner queue` (alias of the existing network scope) also checks the two exact queue members. |
| **Network-config parser leaf** | 0x00432240 | **243/243**, 116 instructions, one strncmp call; **76** differences, 167 equal bytes | Original newline-clear block occurs before token scanning; candidate puts it after. Full original SHA/76-residual-offset guard now protects all 167 matching bytes, one call/field, 33/34 blocks; `--owner parser` protects the exact 679-byte caller. |


The independent original PE and `th09-ida` CODE-xref census at `0x004343D0`
verifies all 27 folded `Float3` helper code callsites: 18 calls from 17
exact functions, nine calls from seven nonexact functions. These are callsite
counts, not 27 distinct caller functions. Run `python3 scripts/audit-float3-conversion-calls.py
--json` for target-hash/callsite/owner evidence; the folded physical callee
cannot be assigned unique C++ authorship from calls alone. The source-present
`MoveRandomBiased` 517/516-byte difference remains open.

Additional non-exact owners (e.g. Player charge/update/added state,
replay save, title replay drawing and network updates) remain in the **35-row
live source-present non-exact ledger**, not in a historic handoff tally.
Review `docs/KNOWLEDGE_BASE.md` and current source before rerunning old negative
experiments; unsupported guesses there are not target evidence.

## Exact landmark routing

The canonical manifest and accepted original-bound comparison remain the
**sole authority** for the **1000 exact** functions. The independently accepted
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

The camera-facing projection at **0x00439820** is now **622/622 exact** through
`anm-manager-project-camera-facing-quad`. Both real SDK helper COMDAT sections
also independently reproduce matrix identity **57 bytes** and vector length
**40 bytes/one field**; they receive no new authored credit. The 57-byte SDK
identity overlap with the older authored matrix-helper model needs a separate
origin/ownership review before any source attribution is promoted. The original
TU and unique C++ spelling remain unknown. `--owner anm` cold-protects all
**17** current complete exact functions across the two affected objects.

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
python3 scripts/check-boss-frontiers.py --owner etama
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
| Compiler binaries/bytecode in `.analysis/` | 999 | 111,683,705 |
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

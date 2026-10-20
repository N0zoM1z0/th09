# TH09 reconstruction — active handoff

> Current routing only. Use the live Git tree and ledgers before historical
> numbers or hypotheses. Earlier details remain in
> [the historical checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md),
> [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md) and Git history.

## Current verified handoff checkpoint

The operator requested documentation/artifact cleanup and handoff. The earlier
long-running goal was cleared; **95% has not been achieved**. This checkpoint
changes no game C++, canonical unit or exact ledger row. The last accepted
addition remains Player initialization in `8ff6ce1`:552 authored bytes,
complete 572-byte physical owner and 25 fields. It is exact, not an open frontier.

- Retain the recovered complete Charge auditor and five pure test groups.
  It checks 1,210 original bytes,70 operands,58 calls,actual float definitions
  and 53 original direct blocks. An object alone does not attest its source/build.
- Freeze 24 Charge and four Effect models with full raw/actual-COFF pairs and
  shared hashed inputs. No candidate is adopted. Selected cold replays repeat
  every captured byte/COFF row and complete original comparison.
- Type3 maintained780/38/15 remains unchanged. The private four-difference
  candidate regresses four formerly equal positions and the guard rejects it.
  `probe-exattack-type3-word-order.py --case forwarded-collision` rebuilds only
  that candidate; default `all` builds three models.
- The cleaner now preserves ambiguous `.idb` files, which may be IDA databases.
  **796 tests/28 skips** pass before final cache deletion. Cleanup removes
  **874 files/11,153,946 bytes**; [cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md)
  records scope and preservation. Missing `build/` at handoff is deliberate.
- Use direct local IDA Pro MCP and Bash, serial `nice -n 10` compiles,
  `gpt-6.1-sol: ...` commits, push and independent remote verification.
  Historical Factory-only prefix/transport instructions are stale.

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
| **Player charge update** | 0x0041F810 | **1,210 target /1,197 candidate bytes**, 70 fields, 58 calls, 350/342 instructions; **1,160 overlap differences +13 missing** | Complete diagnostic `scripts/inspect-player-charge-update.py` checks all original bytes, actual COFF operands, literal definitions and direct graph. Seven older region checks pass despite this whole-owner mismatch. A private 1,210-byte mode-local candidate still differs at 561 positions, damages 20 previously equal positions and fails five regions; it is not adopted. |
| **Front weighted message RNG** | 0x004162D0 | **693 target / 681 candidate**, 15 fields, 6 calls, **569** overlap + 12 missing | Primary-first table selection refined in `09bb2a6`; target/candidate CFG 40/41 blocks. `--owner front` protects exact FrontCalc and siblings. |
| **Front added initialization** | 0x00418480 | **706/706**, 37 original fields, 21 calls, 19 equal blocks; **222 differences** (236→222) | Explicit Z stores and the actual source-vector reference restore 14 physical positions while preserving all 470 previously equal bytes. All 484 equal bytes are guarded. Two private method aliases are replaced by real Reset/Load calls; a shared runtime prefix preserves 0x1D70 and +0/+4/+8/+0C. `--owner frontadded` protects the whole callback plus four exact methods /832 bytes /41 fields. Receiver and remaining panel-store scheduling stay open. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions, 34 fields, 9 calls; **218** overlap + 1 missing | Pointer-backed character snapshot in `4fe7cb0`; original CL/EAX codegen still differs. Protect all 37 existing exact `TitleScreen.cpp` units, including privately relabeled StartMenu. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280/1,280 bytes**,376/376 instructions,57 original-bound fields,13 ordered calls,72 matching direct blocks; **29 differences** | Native indexed list reads, date/name-before-character evaluation, record-before-index acquisition and later timer snapshot recover1,180 more equal bytes. All1,251 matching positions and complete geometry/CFG are now protected. Remaining +185..+321 differences are selected-row ECX/EDX choices and two local-store order. `--owner replaymenu` cold-checks this isolated TU; still NON-EXACT. |
| **TitleScreen::DrawReplaySave** | 0x004239F6 | **800/800 physical**, 245 instructions, 41 fields, 4 calls, 28/28 matching blocks; **96 linked differences** | Raw replay magic, genuine SDK subtraction/two scales/addition and two-byte character construction restore the full extent and nine more loop-counter/FILD home bytes. All **704** equal positions are SHA-bound and protected; no old matching byte regresses from the 105-residual source. Remaining vector/text homes, replay-pointer and keyboard schedules stay NON-EXACT. No enlarged buffer, extra storage or new target helper claim. |
| **TitleScreen::DrawResult** | 0x00423D16 | **1,939/1,939**, 82 fields, 16 calls, 92 blocks; **22** differences | All 22 SHA-pinned linked differences are in the entry: 21 at `+0x0E..+0x24`, one at `+0x39` (bank-pointer store vs `currentScreen` prefetch). The entire remaining owner is byte-exact; the strengthened guard now pins **all 22 exact residual positions**, including preserving previously matching `+0x13` and `+0x18` inside the old entry window. The shared native GameManager field at+0x3F now has its actual COFF symbol/root/addend identity guarded; superseded mode/name-index aliases are rejected. `--owner title` protects the original full owner. |
| **TitleScreen::OnUpdateOptions** | 0x004276EB | **2,045 / 2,048**, 135 fields, 51 calls, 121 blocks | Uses genuine `AnmManager`/`AnmLoaded` and `SoundPlayer` receivers for29 fields, plus **eight native input array references**: two receivers at `g_ReplayInputStates[2]` and six16-bit word06 reads. Strict root/addend checking rejects fake standalone globals even at equal effective addresses. All2,048 linked candidate bytes and 135 referents are preserved; 610 overlap differences+3 extra bytes remain NON-EXACT. Exact same-TU `PlayMenuSound`34/34 and `--owner title` protect this frontier. |
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


## Exact landmarks and evidence routing

The manifests govern **1000 exact** functions. Historical Factory receipts
remain in [the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md), with
immutable source-commit bindings; they do not accept a later HEAD.

- Etama `OnUpdate` is2,195 authored/2,216 physical bytes exact. Its distinct
 600-byte `OnDraw` remains nonexact. TitleSetup and MusicRoom update/draw,
 ExAttack Type19/21 initializers and Player initialization are exact.
- Effect/Etama added callbacks share the target-proven `AnmLoaded` pointer at
 +0x04. The shared eight-byte view does not make Effect's51-byte callback exact.
- `audit-float3-conversion-calls.py --json` bounds 27 physical callsites;
  folded helper identity does not prove unique C++ authorship or full inventory.
- `probe-player-position-visibility.py` demonstrates real ECL 249/249 exact
  versus standalone 250 bytes. Preserve actual include routing in a replay.
- Staging all 63 ECL files preserves bytes but renumbers two private labels.
  Frozen replay stages the original eight files and hashes the other 55;
  all 598 literal COFF rows reproduce. The KB retains that routing evidence.

## Restart and verification

Read [`AGENTS.md`](../AGENTS.md), [`RE_WORKFLOW.md`](RE_WORKFLOW.md), and
[`ORACLES.md`](ORACLES.md). Inspect staged, unstaged and untracked work first.
Call direct IDA Pro MCP `get_metadata` with exactly `{}`, then check mapped
bytes; metadata hashes alone are insufficient.

```bash
git status --short --branch
git diff
git diff --cached
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/check-ida-mcp.py
# Choose one affected owner; reuse unchanged source/profile-bound proofs.
nice -n 10 python3 -B scripts/check-boss-frontiers.py --owner runecl
# Complete Charge diagnostic with the maintained recorded profile:
nice -n 10 scripts/compile-probe.sh src/PlayerChargeAttackUpdate.cpp build/diagnostics/charge.obj /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src
python3 -B scripts/inspect-player-charge-update.py build/diagnostics/charge.obj
# Stage reviewed work before CI. Run validation before deleting artifacts.
python3 -B scripts/validate-tracking.py
python3 -B scripts/validate-docs.py
python3 -B scripts/progress.py --check
python3 -B scripts/ci.py
git diff --check
python3 -B scripts/clean-generated-artifacts.py
```

The Charge CLI exits zero for a valid **nonexact diagnostic**; inspect
`complete_bytes_equal`, differences and size deficits. It adds no exact credit.
`inspect-player-charge-text.py` checks only seven regions.
`check-boss-frontiers.py --owner player` checks collision, **not Charge**.

The frozen controls replay with
`python3 -B .analysis/gpt-61sol-20261020-charge-receiver-owner/replay.py
--case volatile-mode-local` or `--owner effect --case member-result`.
Temporary sources/OBJ/PDB self-clean. These ignored packets are local evidence;
public restart commands above use tracked maintained inputs. Rebuild a canonical
match unit before asking for a fresh exact COFF comparison.

## Evidence and cleanup boundaries

The source/ledger baseline for the final 28 controls is `085343b`; this checkpoint
adds diagnostics and documentation, not gameplay changes. All 485 game sources,
1,026 manifest units and 1,000 exact rows retain that source/profile binding.
Native product/runtime, complete inventory, semantic and port gates stay open.

The KB holds bounded observations and unresolved hypotheses. Old packet numbers,
source shapes and current-state claims are dated history, not acceptance.
The charge-receiver-owner and effect-added-inline-owner packets under
`.analysis/gpt-61sol-20261020-*` retain 28 compressed comparisons,36/18 unique
shared inputs,profiles,tool hashes and selected cold replays. Duplicate staged
copies and compiler output are removed only after verifying this frozen evidence.

Never delete `resources/th09.exe`, toolchains, Wine prefixes, IDA databases,
tracked source or unclassified evidence. Never bulk-delete `.analysis/` or run
`git clean -X`. Around 1.6 GiB of older evidence remains pending individual
producer/source/reference review; age alone does not make it disposable.

`clean-generated-artifacts.py` previews by default. Once producers and CI finish,
review its manifest before `--apply`; it clears ignored `build/` and unambiguous
compiler caches in `.analysis/`. Its per-path history lives in
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`; use the last
`runs` entry, not the cumulative total. See
[artifact cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) for actual deletions.
Post-clean checks use `PYTHONDONTWRITEBYTECODE=1 python3 -B`; rerunning CI would
recreate caches. Missing OBJ/PDB is intentional and must be rebuilt.

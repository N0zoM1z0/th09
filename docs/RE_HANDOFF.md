# TH09 reconstruction — active handoff

> **Current-state routing, not a history log.** Read the live Git tree and
> original-bound ledgers before trusting any number here. Historical experiments,
> previous handoff revisions and older Factory receipts remain in
> [the historical checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md),
> [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md) and Git history. Do not treat a source-present or
> near-exact owner as accepted exact.

## Current verified handoff checkpoint

This is a **replace-in-place next-session routing page**, not a daily
experiment journal. Older detailed checkpoint entries have been preserved in
[the historical checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md);
target-specific experiments and negative controls remain in
[`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md). Source-commit-bound Factory proof
belongs in [the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md).

- **Retail target and ledgers:** Japanese TH09 v1.50a, SHA-256
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  The current canonical ledger contains **980 source-present / 940 exact /
  40 non-exact** functions. The 95% exact reviewed-authored-byte goal is open;
  run the commands below for current numbers, not old 979/876/103 snapshots.
- **Most recent proven cold baseline:** the independent, original-linked
  `check-boss-frontiers.py --owner all` replay passed **35 noncrediting owner
  diagnostics** and **204 distinct previously exact complete-function
  comparisons**. The corresponding pre-cleanup evidence remains under ignored
  `.analysis/`. These replay counts are NOT the 940-function ledger and do
  not create new exact credit.
- **Highest-priority bosses:** `RunEcl` (15,564 physical/598 fields, 30
  unlike ordinary bytes at opcode 4/155), `EtamaController::OnDraw`
  (600/600, 268 unlike bytes; `OnUpdate` already exact 2,216 physical),
  TitleScreen DrawResult (1,939/1,939, 22 differences), and ExAttack
  Type19/21 initialization (763/727, 6/9 differences). Secondary original-
  backed open functions and smaller leaf opportunities are routed below.
- **Recent source-bound interface work:** shared genuine `ZunTimer`,
  `AsciiGameManagerView`, `AsciiBossMarkerVmView`, `FrontMessageOwnerView`,
  `TitleScoreRecordView`, and `SoundPlayer` declarations now span the
  ECL, Etama, Enemy, Title/MusicRoom, ExAttack and ScreenEffect carriers.
  Original-PE/COFF complete-owner checks preserve existing bytes and exact
  siblings; this improves C++ linker provenance, **not** native product
  closure or remaining nonexact register/instruction schedules.
- **Next agent contract:** inspect `git status` and preserve dirty changes;
  prioritize a big boss **and** small leaves, challenge prior assumptions,
  compile with the exact original VC7.1 profile, run focused complete-function
  comparisons and exact siblings, and commit substantive verified source work
  with **`gpt-web: ...`**. Do not claim exactness until a complete original-
  bound canonical replay is accepted. Factory-shell Git commits are local;
  remote push must be checked separately.
- **After artifact cleanup:** generated `build/` objects may be absent by
  design. Use a fresh `scripts/build-match-unit.py --unit NAME` before any
  compare/inspector, never assume the last ignored `.obj` is current.
  The original executable, verified source/ledgers, toolchains, receipts and
  unique `.analysis/` experiments must be preserved.

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
   callbacks. ExAttack Type19/21 initializer **6/9 bytes** remain nonexact;
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

**Rule:** keep experimental C++ in ignored `.analysis/` until a complete
original-linked COFF comparison passes; do not edit `config/matches.csv` or
advance 940 exact merely because normalized, partial, or source-level metrics
improve. Always inspect existing dirty edits (there may be only this worker),
then commit verified source/guard work as `gpt-web: ...`. The currently clean
handoff deliberately removes generated OBJ/PDB/build caches; the next run
**must rebuild** affected match units before invoking inspectors.

## Live ledger snapshot

The canonical state is `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv`, and `config/match-units.toml`, checked by
`python3 scripts/validate-tracking.py --require-target` and
`python3 scripts/validate-docs.py`. The current restart summary is
**980 source-present / 940 exact / 40 non-exact**; earlier operator notes
mentioning 979/876/103 are obsolete for this checkout.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,192 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 980 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 980 |
| Canonical exact functions | 940 |
| Source-present non-exact functions | 40 |
| Source-present non-exact bytes | 49,183 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 226,587 |

**226,587 / 275,770 reviewed-authored bytes = 82.1652% exact**. To reach
95% of the *currently classified* denominator, another **35,395 bytes**
must become complete original-bound exact owners. The 35 origin-unresolved
functions are excluded and could change the denominator. Similarity fractions
and locally matching subsequences never count as accepted exact.

## Active frontiers

Prioritize **large bosses as well as small leaves**. Each row refers to a
maintained, authored, **non-exact** function in `config/functions.csv`.
Comparisons are current-source/original PE cold baselines; lengths are logical
unless explicitly marked physical. The ledger, not this selected table, lists
all 40 open functions. For the complete <=256-byte cohort see
[`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md).

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode 4: 7 bytes; opcode 155: 23 bytes, 146/148 handler shapes. Opcode 4 now directly calls the separately exact real `ZunTimer::operator=(int) @0x00401500` on the proven 12-byte context timer, rather than fake second-class timer ABI; existing genuine `RngRuntimeView`/global `g_Rng` remains. **15,564 complete instruction/table bytes and 598 source-linked original fields are unchanged**, with all **24 same-carrier exact ECL/ExAttack functions** (10,607 physical bytes) protected. `--owner runecl`; exact credit remains open. |
| **ExAttack Type19/21 init** | 0x0044A030 / 0x0044B220 | **763 / 727 physical**, 57 / 53 fields, **6 / 9** byte differences | Real `ExAttackAllocateRecordView` and `ExAttackRecordSideView` exact callees, shared original 0x38 record/side with `EffectManager +0x0C` and `effectAnm +0x2D4` now replace fake per-type receiver classes. Remaining preheader `vertex`/history instruction-order residuals remain NON-EXACT; `--owner exattack` protects exact Type21 update plus true allocator/side leaf. |
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
| **ExAttack Type19 update** | 0x0044A330 | **821/821**, 35 fields, 11 calls, 30 blocks; **198** differences | Target EDI/ESI record–extra ownership and Player-slot lifetime; `--owner exattack`. |
| **ExAttack Type14/22 draw** | 0x00444130 | **959 / 974**, 53 full PE/COFF fields, 20 direct + 12 indirect calls, 9 jumps, 905 overlapping differences and 15 extra source bytes | SHA-bound `--owner exattack` **now cold-replays this real owner**, individually binds all 53 fields and both call kinds, and prevents any new mismatch in its 54 already matching original bytes; three additional exact Type14/22 initializer/update controls. Remaining draw register/loop schedule stays NON-EXACT. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689/1,689**, 172/171 fields, 35 calls, 78/77 blocks; **911 differing bytes** (down from 921) | Real `setupMode==0` non-mode-2 early exit recovers ten original-linked bytes and raises field identity alignment 155→157. The remaining 172-original / 171-source field gap is now independently decoded by **referent and multiplicity**: original-only `g_GameManager+0x00` (3), `+0x34` (1), `+0xA4` (1); candidate-only `+0x28` (1), `+0x98` (1), `+0xB4` (2). The target's side-registration loop anchors ESI to `side.flags (+0x34)` while VC7.1 prefers `side.selector (+0x28)`; 28 source-cold cursor/alias models were neutral or regressed. Still NON-EXACT; full 911-residual-byte and original/COFF-referent guards plus ten exact siblings run via `--owner gamemgr`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **804** overlap + 13 missing | Three return exits, gameplay/state collision branches; `--owner player` protects seven exact helpers. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Scoped volatile read of `g_ReplaySideState0` after header 1E7 publication restores all 32 original field referents in the correct order and all **first 30 physical field positions**, instead of the previous one reversed field pair. Nine register-scheduling residual positions `+0x152,+0x156,+0x15C,+0x162,+0x168,+0x16C,+0x172,+0x178,+0x17A` and 30 seed/tail bytes `+0x1FB..+0x1FE,+0x200..+0x219` remain guarded. All **499 already matching** candidate bytes are protected; tail fields are still shifted -1/-2 bytes. `--owner replay` source-cold replays all **12 exact members across both pinned compiler profiles**; still NON-EXACT. |
| **ANM camera-facing quad** | 0x00439820 | **622/622**, 201 instructions, 40 fields, 11 calls, 8 blocks; **4** differences (13→6→4) | Typed three-vector scratch plus complete three-word copy keeps source and original semantics and now reproduces **618 original physical bytes**. The final two real local stores are scheduled Z-before-Y instead of retail Y-before-Z, leaving exact residual offsets `+0xF6,+0xF7,+0xF9,+0xFA`. This improves whole-owner physical equality from the earlier six-residual baseline, deliberately moving two mismatches within the same copy; all 618 currently matching original bytes are now strictly protected. All **15 same-Render-carrier exact functions** and separately compiled exact `Project3DQuad` (**16 strict checks**): `--owner anm`. No new exact function credit. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | Authentic VC7.1 order of two real volatile references (fade, then hold; same hold + fade value) recovers original physical **+0x5C and +0x5F** with no regressed byte. Full original-SHA/26-offset guard protects **all 310 already matching bytes**; all **18 ScreenEffect exact owners** (2,562 bytes) pass across the genuine three profiles. Remaining EDI/ESI owner allocation is NON-EXACT. |
| **Controller input** | 0x0042B410 | **808 / 796**, 32 fields | WinMM X/Y read and register chronology; use `scripts/inspect-controller-input.py`. |
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
replay save, title replay drawing and network updates) remain in the **40-row
live source-present non-exact ledger**, not in a historic handoff tally.
Review `docs/KNOWLEDGE_BASE.md` and current source before rerunning old negative
experiments; unsupported guesses there are not target evidence.

## Exact landmark routing

The canonical manifest and accepted original-bound comparison remain the
**sole authority** for the **940 exact** functions. The independently accepted
Enemy collision wrapper at 0x00410250 is **82/82 exact**; the 3,900-byte
Enemy OnUpdate caller is **still 29 bytes nonexact**. The former wrapper's
original acceptance and several representative source-commit-bound exact
receipts, including Float3 division, are preserved in
[the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md).
No sibling receipt upgrades a source-present non-exact large owner.

## Active shared Etama/Effect ANM resource owner

The complete original Japanese TH09 v1.50a still requires **RunEcl**,
Etama `OnDraw`, ExAttack Type19/21 initializers and the remaining leaf owners.
This checkpoint does **not** change 980 source / 940 exact / 40 open.

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
  0x04 view is byte-neutral here; it is not a new exact claim. Thirty-six
  full VC7.1 field-read/owner-write variants and 27 complete ExAttack
  Type19/21 preheader/loop/lifetime variants were separately tested in
  ignored reproducible `.analysis/` paths; none improves their complete
  original-bound full-owner residual, so no forced register hints, source
  padding, profile substitutions or fake ABI changes were adopted. The full
  **31/31 owner** and **159/159 distinct strict exact sibling** cold replays
  pass; CI has **473 tests (28 skipped, zero failures)**, with target, ledger,
  documentation, progress and source whitespace validated. No new exact
  function is claimed and the Windows whole-build/runtime gates remain open.
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

Do not infer a successful Factory forced-cold replay from local `--owner all`,
CI or the commit command. To claim new exactness, submit a complete,
source-commit-bound canonical replay and verify an actual **accepted** receipt.

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


### Most recent generated-cache cleanup (2026-10-18)

The current cleanup record is in
[`RE_ARTIFACT_CLEANUP_HISTORY.md`](RE_ARTIFACT_CLEANUP_HISTORY.md) and the
ignored per-file audit `.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`
(**last `runs` entry**, never the cumulative historical total). Cleanup is
scoped to the wholly ignored `build/` tree and compiler-object/bytecode
suffixes inside `.analysis/`, plus separately verified bytecode-only
`__pycache__` directories. Reproducibility scripts, candidate source files,
PE/COFF disassembly, unique JSON evidence and unreviewed historical data
remain untouched, even if large. `resources/th09.exe`, `.tools/`, IDA
state and all tracked files are outside the deletion scope.

**Verified completed cleanup, not merely a proposal:** after original-target
SHA attestation, all **35 complete noncrediting boss/leaf diagnostics**, all
**204 distinct existing canonical exact function comparisons**, and the full
**584-test CI (28 skipped, zero failed)** passed, a fail-closed artifact audit
approved and removed the following ignored generated files:

| Disposable category | Files removed | Logical bytes |
| --- | ---: | ---: |
| Entire generated `build/` tree | 486 | 15,757,511 |
| Compiler binaries and bytecode in `.analysis/` | 2,326 | 197,796,698 |
| SHA-256-rechecked bytecode-only `scripts/tests __pycache__` | 68 | 1,069,658 |
| **Total** | **2,880** | **214,623,867 (~204.7 MiB)** |

The post-deletion cleanup dry run returned **zero files / zero bytes**, both
selected Python cache roots are absent, `build/` no longer exists, and the
private original executable still has its canonical SHA-256. Ignored
`.analysis/` retains **104,476 evidence/source files, ~1.5 GiB allocated**;
its unknown historical sources and provenance were **not** bulk-deleted.
The specific verified cleanup audit and original-bound pre-deletion replay/
CI witness are in `.analysis/handoff-cleanup-20261018/` (`reviewed-preflight.json`,
`verified-deletion.json`, `precleanup-full-boss.json`, `precleanup-ci.log`).
No production code or canonical exact ledger entry changed in this handoff.

**Handoff verification order:** run complete cold boss/leaf comparisons and CI
**before** cleanup; after cleanup run the preview-only cleaner to ensure zero
selected files. Do not rerun CI merely to validate deletion: that recreates
`build/` and bytecode caches. On the next session, rebuild before inspecting:

```bash
python3 scripts/build-match-unit.py --unit ecl-pop-context
python3 scripts/check-boss-frontiers.py --owner runecl
```

For docs-only checks after deletion, set `PYTHONDONTWRITEBYTECODE=1` and use
`python3 -B` so validators and child Python processes do not recreate
bytecode. Details of the prior October 17 cleanup remain preserved in
[`RE_ARTIFACT_CLEANUP_HISTORY.md`](RE_ARTIFACT_CLEANUP_HISTORY.md), rather
than competing as a second active cleanup status here.

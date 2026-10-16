# TH09 reconstruction — active handoff

> **Current-state routing, not a history log.** Read the live Git tree and
> original-bound ledgers before trusting any number here. Historical experiments,
> previous handoff revisions and older Factory receipts remain in Git (for the
> detailed previous handoff, `git show 7f1d56e:docs/RE_HANDOFF.md`) and
> [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md). Do not treat a source-present or
> near-exact owner as accepted exact.

## Current verified handoff checkpoint

- **Git state:** TH09 `main`; derive the actual HEAD and dirty work from
  `git status --short --branch` and `git log` at restart. Complete,
  review, and commit **every tracked/untracked change**; do not reset or
  sidestep it. New checkpoints use `gpt-web: ...`; local commits are
  **not** pushed automatically.
- **Original target:** Japanese TH09 **v1.50a** `resources/th09.exe`,
  immutable SHA-256
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  It is ignored, private, read-only, and never a cleanup candidate.
  Factory's registered IDA source is `th09-ida`; mapping metadata is not
  canonical codegen acceptance.
- **Current code milestones:** Source commit `c30a35a` narrows
  ScreenEffect CalcShakeEnvelope from 28 to **26 complete unlike bytes**
  while preserving all exact siblings; source commit `d21c98d` shares the
  genuinely proven 0x1EC replay record between four large translation
  units. `RunEcl` remains nonexact at **15,564 physical bytes/598
  original bound operands/30 remaining bytes**.
- **Cold regression scope:** after adding the original-SHA-bound
  DrawReplayMenu diagnostic, the full `--owner all` suite verifies
  **30 non-crediting complete boss/leaf functions plus 68 complete
  already-exact siblings**. This includes the Etama OnDraw 600-byte guard,
  Enemy OnUpdate 3,900-byte guard, the 1,280-byte ReplayMenu original and
  Title DrawResult's 1,939-byte owner. These checks are *not*
  source-commit-bound accepted Factory receipts.
- **Phase:** original exact-reconstruction only. The 95% reviewed-authored
  byte milestone is open; whole Windows i386 build/link/resources/runtime,
  dual-Oracle semantic reconstruction and portability are still open.

## Live ledger snapshot

The canonical state is `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv`, and `config/match-units.toml`, checked by
`python3 scripts/validate-tracking.py --require-target` and
`python3 scripts/validate-docs.py`. The current restart summary is
**980 source-present / 939 exact / 41 non-exact**; earlier operator notes
mentioning 979/876/103 are obsolete for this checkout.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,192 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 980 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 980 |
| Canonical exact functions | 939 |
| Source-present non-exact functions | 41 |
| Source-present non-exact bytes | 49,403 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 226,367 |

**226,367 / 275,770 reviewed-authored bytes = 82.0854% exact**. To reach
95% of the *currently classified* denominator, another **35,615 bytes**
must become complete original-bound exact owners. The 35 origin-unresolved
functions are excluded and could change the denominator. Similarity fractions
and locally matching subsequences never count as accepted exact.

## Active frontiers

Prioritize **large bosses as well as small leaves**. Each row refers to a
maintained, authored, **non-exact** function in `config/functions.csv`.
Comparisons are current-source/original PE cold baselines; lengths are logical
unless explicitly marked physical. The ledger, not this selected table, lists
all 41 open functions. For the complete <=256-byte cohort see
[`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md).

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode 4: 7 bytes; opcode 155: 23 bytes, 146/148 handler shapes. Opcode 4 now directly calls the separately exact real `ZunTimer::operator=(int) @0x00401500` on the proven 12-byte context timer, rather than fake second-class timer ABI; existing genuine `RngRuntimeView`/global `g_Rng` remains. **15,564 complete instruction/table bytes and 598 source-linked original fields are unchanged**, with all **24 same-carrier exact ECL/ExAttack functions** (10,607 physical bytes) protected. `--owner runecl`; exact credit remains open. |
| **ExAttack Type19/21 init** | 0x0044A030 / 0x0044B220 | **763 / 727 physical**, 57 / 53 fields, **6 / 9** byte differences | Real `ExAttackAllocateRecordView` and `ExAttackRecordSideView` exact callees, shared original 0x38 record/side with `EffectManager +0x0C` and `effectAnm +0x2D4` now replace fake per-type receiver classes. Remaining preheader `vertex`/history instruction-order residuals remain NON-EXACT; `--owner exattack` protects exact Type21 update plus true allocator/side leaf. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**, 97 fields, 69 calls, 170 blocks; **29** differing bytes | Genuine `Float3::operator/` plus real `AppendBoxRecord` compiler visibility closes the entire ten-byte `+0x845..+0x850` three-dword copy window. Remaining 2/11/16 bytes stay open; `--owner enemy` cold-protects all four same-TU exact helpers. |
| **EtamaController::OnDraw** | 0x00413BE0 | **600/600 physical**, 27 fields, 13 calls, 17 blocks; **268** differences | Laser body-VM position cursor ESI still open. The final original call is now typed as `EffectManager::DrawBulletLayerEffects` with its separately **169/169 exact** real-member source at `0x0040D110` and side owner at `+0x0C`; all original linked bytes remain unchanged. Complete SHA/offset guard preserves the 332 equal bytes; `--owner etama` protects exact OnUpdate. |
| **PauseMenu::OnUpdate** | 0x00434740 | **1,776 original physical / 1,732 candidate**; 46 calls; 10-way switch; **1,453** overlapping differences + 44 missing | First seven-VM cursor improvement committed in `7f1d56e`; remaining branch/VM scheduling. `--owner pause` independently checks 11 already-exact Ascii/ANM functions and real original table/ABI. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | Now uses genuine shared `g_Supervisor.framerateMultiplier5B8 @0x004B36B8` and the four **real `GameManagerResetView::coordinate360/364/368/36C` fields** (original `0x004A80F0..FC`), not fake external playfield globals. The independent **79-byte exact GameManager coordinate initializer** physically writes those fields; 1,900-byte linked movement boss and 66 operand identities stay unchanged, with 129 ordinary differences still open. `--owner movement` protects exact 522-byte caller. |
| **Front weighted message RNG** | 0x004162D0 | **693 target / 681 candidate**, 15 fields, 6 calls, **569** overlap + 12 missing | Primary-first table selection refined in `09bb2a6`; target/candidate CFG 40/41 blocks. `--owner front` protects exact FrontCalc and siblings. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions, 34 fields, 9 calls; **218** overlap + 1 missing | Pointer-backed character snapshot in `4fe7cb0`; original CL/EAX codegen still differs. Protect all 37 existing exact `TitleScreen.cpp` units, including privately relabeled StartMenu. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280 target / 1,258 source bytes**, 376/368 instructions, **57/57 original decoded/COFF operands**, **13/13 ordered calls**, **72/70 CFG blocks** | Complete original-SHA-bound source-cold diagnostics now run via `--owner title`; the candidate is still **22 bytes short and NON-EXACT**. The shared replay record is exact-sized but does not imply the function is exact. |
| **TitleScreen::DrawReplaySave** | 0x004239F6 | **800 target / 799 source**, 245/245 instructions, 41 fields, 4 calls, 28/28 original-direct blocks | Shares the ReplayInput-proven 0x1EC record with DrawReplayMenu. Real validity-shared text pointers and frame factor improve **793→799** physical bytes, **241→245** instructions and recover the complete original CFG (formerly divergent). SHA-bound noncrediting 799/245/41/4/28 guard plus `--owner title`; still missing one byte and NON-EXACT. |
| **TitleScreen::DrawResult** | 0x00423D16 | **1,939/1,939**, 82 fields, 16 calls, 92 blocks; **22** differences | All 22 SHA-pinned linked differences are in the entry: 21 at `+0x0E..+0x24`, one at `+0x39` (bank-pointer store vs `currentScreen` prefetch). The entire remaining owner is byte-exact; full physical-window guard plus `--owner title` protects it. |
| **TitleScreen::OnUpdateOptions** | 0x004276EB | **2,045 / 2,048**, 135 fields, 51 calls, 121 blocks | The 2,048-byte boss now uses genuine `AnmManager`/`AnmLoaded` and `SoundPlayer` receivers for **29 reviewed original-linked fields** rather than private C++ fake managers. Full codegen/call/CFG identity unchanged; 3 extra candidate bytes in input/confirmation menu remain NON-EXACT. Exact same-TU `PlayMenuSound` 34/34 and `--owner title` are required regressions. |
| **Title character cursors** | 0x00424EDD / 0x00424FB0 / 0x0042505F | Active target/candidate **211/215**; Reverse/Inactive **175/175** | Active reduced 217→215 in `69165b2`, but none is exact. Loop pre-check and VM register/sprite reload schedules; `--owner title`. |
| **GameManager CleanupGameplayState** | 0x0041B5C6 | **425 / 406**, 43 / 41 fields, 12 calls, **209** overlap | Preserve actual stage clamp/return contract; naive switch/if permutations regress. `--owner gamemgr`. |
| **ExAttack Type3 update** | 0x00442750 | **780/780**, 38 fields, 24 calls, 17 blocks; **15** differences | Source now places middle-Z assignment after the middle-Y call; VC7.1 still hoists the tail-X load before the store (non-exact). The old 14-difference candidate wrongly wrote Z before the Y call. Strict chronology/offset guard; `--owner type3`. |
| **ExAttack Type19 update** | 0x0044A330 | **821/821**, 35 fields, 11 calls, 30 blocks; **198** differences | Target EDI/ESI record–extra ownership and Player-slot lifetime; `--owner exattack`. |
| **ExAttack Type14/22 draw** | 0x00444130 | **959 / 974**, 53 fields, 20 direct calls | Draw register/loop schedule after shared Type14/22 extra layout; `--owner exattack`. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689/1,689**, 172/171 fields, 35 calls, 78/77 blocks; **911 differing bytes** (down from 921) | Real `setupMode==0` non-mode-2 early exit recovers ten original-linked bytes and raises field identity alignment 155→157. Still NON-EXACT; the full residual-bitmask guard and ten existing exact siblings run via `--owner gamemgr`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **804** overlap + 13 missing | Three return exits, gameplay/state collision branches; `--owner player` protects seven exact helpers. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 32 fields, 5 calls, 12 blocks; **49** overlap + 2 missing | One hoisted SideState0 header field and seed zero-extension; `--owner replay`. |
| **ANM camera-facing quad** | 0x00439820 | **622/622**, 40 fields, 11 calls, 8 blocks; **13** differences | Typed three-vector scratch closes all prior local-frame differences; only the target's load-all-before-store-all return-copy scheduling at `+0xE9..+0xF7` differs. Full original-byte-window gate and four exact neighbors: `--owner anm`. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | Authentic VC7.1 order of two real volatile references (fade, then hold; same hold + fade value) recovers original physical **+0x5C and +0x5F** with no regressed byte. Full original-SHA/26-offset guard protects **all 310 already matching bytes**; all **18 ScreenEffect exact owners** (2,562 bytes) pass across the genuine three profiles. Remaining EDI/ESI owner allocation is NON-EXACT. |
| **Controller input** | 0x0042B410 | **808 / 796**, 32 fields | WinMM X/Y read and register chronology; use `scripts/inspect-controller-input.py`. |
| **Supervisor network service** | 0x00430AA0 | **1,633 / 1,632**, 108 source fields | Real queue ownership and 1,395 overlap differences still open; `--owner network`. |
| **Supervisor::OnUpdate** | 0x00431110 | **844 authored / 996 target physical**, 952 candidate physical | 92 external fields, 27 calls but 44 missing physical bytes; `--owner network`. |
| **ECL MoveRandomBiased leaf** | 0x00407E30 | **516 / 517**, 28 fields, 14 calls, 24 blocks; 148 overlap + 1 extra | Genuine shared RNG method/global owner symbols now independently original-bound; Y-pointer target `LEA [EAX+4]` versus candidate `MOV EAX` remains unresolved. `--owner runecl`. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Uses genuine `AnmManager`/`AnmLoaded*` types. The SHA-pinned full-owner guard permits narrowing only the target-proven `+0x03..0x09,+0x0D` residuals, protecting the other 43 matching bytes and three same-TU exact sibling checks; `--owner effect`. |
| **FileSystem decrypt-table leaf** | 0x0042C290 | **220/220**, 12 fields, 2 calls, 11 blocks; **38** differences | Scan counter/key-bias lifetime; `--owner crypto`, exact Decrypt/Encrypt siblings. |
| **Supervisor received-frame leaf** | 0x0042E9E0 | **213/213**, 67 instructions, 9 blocks; **35** differences | Target packed-input/seed 16-bit store register order; `--owner network`. |
| **Network-config parser leaf** | 0x00432240 | **243/243**, 116 instructions, one strncmp call; **76** differences | Newline-clear early-return block order; `--owner parser`. |


The independent original PE and `th09-ida` CODE-xref census at `0x004343D0`
verifies all 27 folded `Float3` helper code callsites: 18 calls from 17
exact functions, nine calls from seven nonexact functions. These are callsite
counts, not 27 distinct caller functions. Run `python3 scripts/audit-float3-conversion-calls.py
--json` for target-hash/callsite/owner evidence; the folded physical callee
cannot be assigned unique C++ authorship from calls alone. The source-present
`MoveRandomBiased` 517/516-byte difference remains open.

Additional non-exact owners (e.g. Player charge/update/added state,
replay save, title replay drawing and network updates) remain in the **41-row
live source-present non-exact ledger**, not in a historic handoff tally.
Review `docs/KNOWLEDGE_BASE.md` and current source before rerunning old negative
experiments; unsupported guesses there are not target evidence.

## Exact landmark routing

The canonical manifest and accepted original-bound comparison remain the
**sole authority** for the **939 exact** functions. The independently accepted
Enemy collision wrapper at 0x00410250 is **82/82 exact**; the 3,900-byte
Enemy OnUpdate caller is **still 29 bytes nonexact**. The former wrapper's
original acceptance and several representative source-commit-bound exact
receipts, including Float3 division, are preserved in
[the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md).
No sibling receipt upgrades a source-present non-exact large owner.

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
normative. The detailed previous handoff remains available via
`git show 7f1d56e:docs/RE_HANDOFF.md`, rather than retaining a second stale
current-state table.

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


### Current cleanup and evidence boundaries

Preview with `python3 scripts/clean-generated-artifacts.py`. Only after cold
codegen/CI checks use its `--apply` option. Its strict scope is wholly ignored
generated `build/` plus compiler .obj/.pdb/.pyc/.pch/.idb/.ilk caches
inside `.analysis/`; **no tracked source, experiment C++, scripts, JSON
evidence, original executable, IDA database, or toolchain may be deleted**.
The ignored per-path audit is
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`.

The [cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) preserves the
2026-10-15 exact deletion inventory. A new scoped cleanup and the remaining
evidence inventory will be recorded here after verifying its exact scope.
An absent `build/` after handoff is intentional: cold-rebuild a match unit
before comparing, rather than treating deleted .obj as missing source.

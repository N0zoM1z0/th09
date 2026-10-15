# TH09 reconstruction — active handoff

> **Current-state routing, not a history log.** Read the live Git tree and
> original-bound ledgers before trusting any number here. Historical experiments,
> previous handoff revisions and older Factory receipts remain in Git (for the
> detailed previous handoff, `git show 7f1d56e:docs/RE_HANDOFF.md`) and
> [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md). Do not treat a source-present or
> near-exact owner as accepted exact.

## Current verified handoff checkpoint

- **Working branch / code baseline:** `th09` on `main`. Determine the source
  commit from the live `git log` and `git status --short --branch`, never from
  a historical handoff ID. The preceding completed operator checkpoint was
  `3115305` (large-function negative-frontier evidence), following `86f722c`
  (Type3 original-order source repair); this checkpoint promotes the separate
  Enemy collision wrapper by restoring authentic callee code visibility.
  **Review and finish dirty or untracked work**, never reset or sidestep it.
  New commits use **`gpt-web: ...`**; local Git commits are not remote pushes.
- **Original target:** Japanese TH09 **v1.50a** (`resources/th09.exe`), original
  SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  This is an ignored private, read-only user input, **never delete or commit**.
  Factory's IDA provider is `th09-ida`; its attestation is not a codegen Oracle.
- **Regression scope:** Before this Enemy collision closure, the prior
  complete `--owner all` cold sweep guarded **29 non-crediting boss/leaf
  owners** and **65 exact sibling checks**. The new `--owner enemy` scope
  cold-rebuilds the **3,900-byte non-exact Enemy update**, its two exact
  same-TU helpers and the independently canonical exact **205-byte
  `AppendBoxRecord` / 82-byte Enemy collision wrapper** from one shared
  source object. Re-run focused Oracles for changed owners and CI; never treat
  old full-sweep numbers as fresh checks on a new source commit.
- **Phase:** exact reconstruction only. Faithful Windows i386 whole-product
  build/link/resource/runtime closure is **open**; dual-Oracle semantics and
  portability **have not started**. The 95%-reviewed-authored-byte goal is open.

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
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode 4: 7 bytes (timer register/push order); opcode 155: 23 bytes (wire-byte read and masked flags). 146/148 handler shapes; `--owner runecl`, exact PopContext sibling. |
| **ExAttack Type19/21 init** | 0x0044A030 / 0x0044B220 | **763 / 727 physical**, 57 / 53 fields, **6 / 9** byte differences | Two instruction-order residuals around vertex increment/history setup; reject candidates changing full source/code extent. `--owner exattack` protects exact Type21 update. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**, 97 fields, 69 calls, 170 blocks; **39** differing bytes | Four register/Float3 copy windows remain; `--owner enemy` separately proves the caller and newly exact 82-byte collision leaf plus its exact 205-byte callee. |
| **EtamaController::OnDraw** | 0x00413BE0 | **600/600 physical**, 27 fields, 13 calls, 17 blocks; **268** differences | Laser body-VM position cursor ESI ownership/register residency. `--owner etama` protects separately exact `EtamaController::OnUpdate`. |
| **PauseMenu::OnUpdate** | 0x00434740 | **1,776 original physical / 1,732 candidate**; 46 calls; 10-way switch; **1,453** overlapping differences + 44 missing | First seven-VM cursor improvement committed in `7f1d56e`; remaining branch/VM scheduling. `--owner pause` independently checks 11 already-exact Ascii/ANM functions and real original table/ABI. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | First effect-spawn side snapshot improved 144→129 in `7dfa553`; `--owner movement` protects exact 522-byte `Player::OnUpdate` caller. |
| **Front weighted message RNG** | 0x004162D0 | **693 target / 681 candidate**, 15 fields, 6 calls, **569** overlap + 12 missing | Primary-first table selection refined in `09bb2a6`; target/candidate CFG 40/41 blocks. `--owner front` protects exact FrontCalc and siblings. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions, 34 fields, 9 calls; **218** overlap + 1 missing | Pointer-backed character snapshot in `4fe7cb0`; original CL/EAX codegen still differs. Protect all 37 existing exact `TitleScreen.cpp` units, including privately relabeled StartMenu. |
| **TitleScreen::DrawResult** | 0x00423D16 | **1,939/1,939**, 82 fields, 16 calls, 92 blocks; **22** differences | Entry score-bank pointer calculation vs `currentScreen` load schedule; `--owner title`. |
| **TitleScreen::OnUpdateOptions** | 0x004276EB | **2,045 / 2,048**, 135 fields, 51 calls, 121 blocks | Three extra candidate bytes; score/input option tails remain open, `--owner title`. |
| **Title character cursors** | 0x00424EDD / 0x00424FB0 / 0x0042505F | Active target/candidate **211/215**; Reverse/Inactive **175/175** | Active reduced 217→215 in `69165b2`, but none is exact. Loop pre-check and VM register/sprite reload schedules; `--owner title`. |
| **GameManager CleanupGameplayState** | 0x0041B5C6 | **425 / 406**, 43 / 41 fields, 12 calls, **209** overlap | Preserve actual stage clamp/return contract; naive switch/if permutations regress. `--owner gamemgr`. |
| **ExAttack Type3 update** | 0x00442750 | **780/780**, 38 fields, 24 calls, 17 blocks; **15** differences | Source now places middle-Z assignment after the middle-Y call; VC7.1 still hoists the tail-X load before the store (non-exact). The old 14-difference candidate wrongly wrote Z before the Y call. Strict chronology/offset guard; `--owner type3`. |
| **ExAttack Type19 update** | 0x0044A330 | **821/821**, 35 fields, 11 calls, 30 blocks; **198** differences | Target EDI/ESI record–extra ownership and Player-slot lifetime; `--owner exattack`. |
| **ExAttack Type14/22 draw** | 0x00444130 | **959 / 974**, 53 fields, 20 direct calls | Draw register/loop schedule after shared Type14/22 extra layout; `--owner exattack`. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689/1,689**, 172/171 fields, 35 calls; 78/77 blocks | Exact GameManager O2/Os profile matters; `--owner gamemgr`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **804** overlap + 13 missing | Three return exits, gameplay/state collision branches; `--owner player` protects seven exact helpers. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 32 fields, 5 calls, 12 blocks; **49** overlap + 2 missing | One hoisted SideState0 header field and seed zero-extension; `--owner replay`. |
| **ANM camera-facing quad** | 0x00439820 | **622/622**, 40 fields, 11 calls, 8 blocks; **24** differences | ESI/EDI receiver/register lives; `--owner anm`. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 20 fields, 8 calls, 26 blocks; **28** differences | Preserve exact timer/float ABI, change only true receiver register lifetime; `--owner screen`. |
| **Controller input** | 0x0042B410 | **808 / 796**, 32 fields | WinMM X/Y read and register chronology; use `scripts/inspect-controller-input.py`. |
| **Supervisor network service** | 0x00430AA0 | **1,633 / 1,632**, 108 source fields | Real queue ownership and 1,395 overlap differences still open; `--owner network`. |
| **Supervisor::OnUpdate** | 0x00431110 | **844 authored / 996 target physical**, 952 candidate physical | 92 external fields, 27 calls but 44 missing physical bytes; `--owner network`. |
| **ECL MoveRandomBiased leaf** | 0x00407E30 | **516 / 517**, 28 fields, 14 calls, 24 blocks; 148 overlap + 1 extra | Target `LEA [EAX+4]` versus candidate `MOV EAX`/offset reads. Exact family remains protected; `--owner runecl`. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | EDI ANM pointer vs ESI owner load order; `--owner effect`. |
| **FileSystem decrypt-table leaf** | 0x0042C290 | **220/220**, 12 fields, 2 calls, 11 blocks; **38** differences | Scan counter/key-bias lifetime; `--owner crypto`, exact Decrypt/Encrypt siblings. |
| **Supervisor received-frame leaf** | 0x0042E9E0 | **213/213**, 67 instructions, 9 blocks; **35** differences | Target packed-input/seed 16-bit store register order; `--owner network`. |
| **Network-config parser leaf** | 0x00432240 | **243/243**, 116 instructions, one strncmp call; **76** differences | Newline-clear early-return block order; `--owner parser`. |

Additional non-exact owners (e.g. Player charge/update/added state,
replay save, title replay drawing and network updates) remain in the **41-row
live source-present non-exact ledger**, not in a historic handoff tally.
Review `docs/KNOWLEDGE_BASE.md` and current source before rerunning old negative
experiments; unsupported guesses there are not target evidence.

### New exact closure: Enemy collision wrapper

`EnemyAppendCollisionView::AppendPlayerCollisionBox @ 0x00410250` has a
canonical **82/82 full-byte** exact source-cold match with both independent
`REL32` calls (to `Float3::operator/` and `AppendBoxRecord`) and 30 decoded
instructions/`ret 8`. A genuine shared `src/PlayerCollisionAppendBox.cpp`
translation unit exposes the unchanged previously exact `AppendBoxRecord`
body to the optimizer. It changes the formerly 31 differing register/copy
bytes into the original grouped return copy, without replacing either
algorithm. Both canonical units independently cold replay from the shared
object: **82/82** for the new wrapper and **205/205** for the existing callee.
The original C++ translation-unit partition and other compile-unit identities remain
unproven; this is a codegen/visibility finding, not whole-image or runtime
closure. A **new, committed-source Factory acceptance receipt is required**
before describing this as Factory accepted. All 3,900 bytes of the large Enemy
update remain NON-EXACT (39 differences); there is no partial credit.

## Closed codegen landmarks and accepted receipts

The exact-function ledger is the authority for **939 locally canonical exact functions** (the newly promoted collision wrapper still requires a source-commit-bound Factory receipt).
These are three representative independently accepted owners whose current
identities are validated against the canonical match-unit manifest:

| Exact owner | Target/bytes | Canonical unit | Accepted source commit and receipt |
| --- | --- | --- | --- |
| `FrontMessageRuntimeView::LoadForSideCount` | `0x004182E0`, **405/405**, 18 fields | `front-message-load-for-side-count` | `4d921a2`; `receipt:8ea1d24dda7aab61c63e8e1fb288727adb1125dff033a86fecc1c81cd8658e98` |
| `FrontMessageRuntimeView::ReleaseForSideCount` | `0x00417520`, **271/271**, 14 fields | `front-message-release-for-side-count` | `99ea00b`; `receipt:3ad8649181efbc34d2b08b5d1ead5d96ddd77d7fec074b9a0508219dfdf567a1` |
| `PlayerShotDrawCallbackType1` | `0x00442220`, **204/204**, 6 fields | `player-shot-draw-callback-type1` | `495e944`; `receipt:7f20349cb55183e13f66e4b1ecabc24133fd379d2f9c2b91a27256929bef1927` |

**Source-commit-bound Factory `strict-live-v1` forced-cold regression receipts
(`pass / accepted`) for *already-exact* functions:**

- Gameplay-source `09bb2a6`: `EtamaController::OnUpdate` (2,195 authored / 2,216 physical), `receipt:19f65147eaaaea7836cb2d705459c5486d6a4be987d4c8acd27b29babebf17c9`; `TitleSetupThread` (534), `receipt:5831f8f466f042ca5854d6c161a40bba37151c885872f3326623c3cb29f93b9e`; MusicRoom Update (2,258), `receipt:ec836857bcf5bdcaa7eea080398dbd623306205ea1b12091fbacf2c07b48d7f2`; ExAttack Type21 Update (1,074), `receipt:f722291fa5089d2088b87b74a978f34cd493aad5a49496fafe0a89b3639c5c79`; FrontCalc (1,491), `receipt:74198fed08b459dc9bcdef27c6d5e939afd2e1011b74527e82492ef64df2b8a2`.
- Gameplay-source `4fe7cb0`: TitleSetup (534), `receipt:afc85df61447369ef8ba8860939608f140b621e327086d1f459cebe957cd80b8`; MusicRoom Update (2,258), `receipt:acb48c0b18a6861b4288c38a27a2d5e7244f2e54be79cd325daef6aecf6bd637`.
- Gameplay-source `f951317`: exact `PlayerLifecycleView::OnUpdate` (522), `receipt:b58119d92a0c8eec924d4c68f99c174c9219d57081caf28c177d614202c0718d`.
- Additional exact `GameManager::OnUpdate` (1,230) and `PlayerCollisionQueryStateView::FindCollision` (602) have source-commit-bound acceptance in Git history and the indexed knowledge base.

**Do not count these regressions as new exact matches.** Receipts do not bind
later source commits, diagnose adjacent non-exact owners, establish a native
Windows image or attest runtime semantics. Preserve current source-specific
compiler-private labels only after verifying their real owner-relative targets;
never transfer `$L...` spellings from older snapshots.

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

### 2026-10-15 cleanup execution and preservation audit

The clean baseline was checked **before deletion**: target and tracking passed,
all **29** original-bound non-exact boss/leaf cold diagnostics passed, all
**65** related existing exact unit comparisons passed, and `scripts/ci.py`
passed **347 tests (27 skipped)**. Then the existing reviewed cleanup script
removed exactly **4,825** ignored generated files totaling **366,037,887
bytes** (about **349 MiB**): **388 `build/` files** / 9,498,720 bytes and
**4,437 `.analysis/` compiler artifacts** (`.obj/.pdb/.pyc/.pch/.idb/.ilk`)
/ 356,539,167 bytes. Separately, **67** ignored Python bytecode cache files
under `scripts/__pycache__` and `tests/__pycache__` were removed (847,340
bytes). The follow-up `clean-generated-artifacts.py` dry run reported **zero
remaining selected cache files**. The per-path local deletion audit is in
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`; only its
**latest run**, not its cumulative historical total, describes this sweep.

`build/` is intentionally **absent** at handoff. Rebuild the necessary
match-unit or diagnostic owner fresh before calling its inspector; running
those commands again will recreate disposable files. **Do not rerun CI solely
to confirm deletion** (it has already passed, and a new run may regenerate
bytecode). The ignored `.analysis/` still occupies roughly **1.2 GiB on disk**:
mainly old `.cpp/.inl/.hpp` experiment copies and JSON evidence across
previous GPT-web/GPT-dots campaigns. They are **retained pending individual
reproducer/consumer review**, not advertised as fresh or accepted evidence;
the remaining size is not a failed build-cache cleanup. Preserve
`resources/th09.exe`, `.tools/`, tracked C++ and manifests, and any analysis
material whose unique source binding is not established. Before further
archival pruning, audit a specific campaign against its `probe.py`,
`results.json`, source commit and tracked knowledge; never delete it by age or
extension alone.

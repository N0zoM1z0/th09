# TH09 reconstruction — active handoff

> **Current-state routing, not a history log.** Read the live Git tree and
> original-bound ledgers before trusting any number here. Historical experiments,
> previous handoff revisions and older Factory receipts remain in Git (for the
> detailed previous handoff, `git show 7f1d56e:docs/RE_HANDOFF.md`) and
> [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md). Do not treat a source-present or
> near-exact owner as accepted exact.

## Current verified handoff checkpoint

- **RunEcl opcode-4 original-machine timer/redispatch safety (October 18):**
  The previously unfinished source-cold `scripts/audit-ecl-timer-jump.py` is
  now retained and executed by `--owner runecl` / `--owner all`, alongside
  the existing opcode-155 masked-XOR behavior gate. From the verified
  **15,564-byte /598-field** real PE/VC7.1 owner it separately decodes the
  **29-byte / eight-instruction opcode-4** sequence, tests all six raw signed
  and unsigned time operands x five positive/negative relative offsets x three
  context addresses (**90 original/candidate transitions**) and verifies the
  actual `ZunTimer::operator=(int) @0x00401500` thiscall's
  `activeContext+0x08` receiver, unmodified argument, one call, sole
  original ECL pointer advance and direct redispatch @`0x00408750`.
  Unknown instructions, memory/width changes, wrong call/branch/offset
  and false exactness are rejected by ten pure-input tests. The retained
  source still emits the same **seven opcode-4 different bytes** (the
  `MOV ECX; PUSH ECX; MOV ECX` vs `MOV EAX; MOV ECX; PUSH EAX` register
  allocation), while opcode 155 still differs by 23 ordinary bytes.
  **146/148** handlers and all **24 same-source exact siblings** still pass;
  neither semantic gate grants codegen exactness. Old 979/876/103 counts
  remain superseded by **980/940/40**. The original target and native-product
  state are unchanged; focused codegen proof and CI stay mandatory.

- **Title actual-added callback's genuine ASCII global/callee identity (October 18):**
  `TitleScreen.cpp` previously declared the physical `g_AsciiManager`
  `@0x004CE458` as a separate `TitleAsciiManagerView`, producing a linker
  symbol different from the canonical `AsciiManager` global used by other
  exact functions. The same exact `TitleScreenView::ActualAddedCallback`
  `@0x00424BF7` called the original **exact** `AsciiManager::Reset
  @0x004343E0` under a fabricated `TitleAsciiManagerView::Reset` method name.
  Both now use the genuine shared `AsciiManager` type/method, whose target
  address and ABI were independently proven. The next original **one-byte
  folded** call `@0x0043D2B0` still has `origin unknown/review` and remains
  behind a separate *declaration-only* adapter; no `AsciiManager::InitializeVms`
  origin is invented. Source-cold `ActualAddedCallback` is still **335/335
  full original bytes, 27 authentic fields**, with only the two reviewed
  assembler/COFF external names changed. The genuine `OnUpdateStartMenu`
  1,560/1,592 physical owner changes only **nine** compiler-private `$L`
  names, individually verified to the *same* original VAs/field offsets;
  all **41 Title exact siblings** still replay. The separate 1,939-byte
  DrawResult boss keeps its own **22** original-bound residual bytes.
  `--owner title`; this is a native source-reference repair, **not**
  newly exact codegen or a closed native product. 980/940/40 unchanged.

- **Title score-record producer/renderer unification (October 18):**
  `TitleScreen.cpp`'s exact `TitleScoreRecordView::InsertIntoTable` and the
  still-open **1,939-byte `DrawResult` boss** now share the *same* typed
  0x2C-byte record, singleton and 16×5×5 global table declarations in
  `src/TitleScoreRecordView.hpp`. The real target/VC7.1 C++ table symbol
  `?g_TitleScoreTable@@3PAY144UTitleScoreRecordView@@A` binds both consumers
  to **0x004A8380**, instead of the old renderer-only
  `ResultDrawScoreRecord` type with a *different* mangled linker symbol.
  Original TH09 independently fixes score `+0x0C`, category/rank, name
  `+0x18` and signed digit `+0x2B`; no data definition, allocation or native
  link/runtime acceptance is invented. On fresh source-cold compilation,
  `DrawResult` preserves **1,939/1,939 physical, 82 original address fields,
  16 ordered calls, 92 direct blocks and the identical 22 non-exact offsets**.
  Including the shared declaration shifts only nine compiler-private `$L`
  names in exact `OnUpdateStartMenu`. Each new COFF local-label section offset
  independently yields the SAME original VA at all nine immutable manifest
  field offsets; only the private symbol spelling was updated. The full
  **1,592-byte physical StartMenu remains exact** with 107 relocations,
  and **all 41 Title exact siblings** cold-replay. Independent Factory
  forced-cold `pass / accepted` replays on clean source commit `6b47d06`
  reconfirm the already-exact 216-byte `InsertIntoTable`, 1,592-byte
  physical `OnUpdateStartMenu`, and 534-byte `TitleSetupThread`;
  immutable source-bound receipts are in `RE_ACCEPTED_RECEIPTS.md`.
  This is a genuine source-owner/linker identity correction, NOT a newly
  exact function: 980/940/40.

- **RunEcl opcode-155 original-machine semantic guard (October 18):** A new
  source-cold `scripts/audit-ecl-timeout-bit.py`, invoked by the full
  `--owner runecl` / `--owner all` frontier, independently links the current
  real VC7.1 PE/COFF candidate to TH09's SHA-verified 15,564-byte original.
  It decodes the 45-byte opcode-155 handler at `0x0040B770`, interprets the
  eight real flag-update instructions on both sides, and checks **all 256
  wire-byte values across 12 whole-word flag patterns (3,072 transitions)**.
  Both implementations update **only bit 24** and agree for every checked
  case. Unknown instructions, operand widths and extra or displaced memory
  writes are rejected, with eleven pure-input semantic and orchestration tests. This is
  **semantic protection, NOT codegen exactness**: the full RunEcl remains
  **15,564/15,564 physical bytes, 598 bound fields, 30 unequal ordinary
  bytes**, divided as opcode 4 = 7 and 155 = 23; 24 same-carrier/alternate-
  profile exact siblings remain independently cold-protected. Additional
  source-cold variants in `.analysis/gpt-web-20261018/` (ignored diagnostic
  artifacts) were rejected when whole-owner relocations, byte positions or
  handler extents regressed rather than trusting local instruction similarity.
- **Current-source Factory regressions of three established exact big owners
  (October 18):** On the clean source commit `83a94cfb204a17518d4efd9db7f8ec8d2e87a4fe`,
  fresh Factory `strict-live-v1` *forced-cold*, original-bound receipts
  returned **`pass / accepted`** for Etama `OnUpdate` **2,216** physical
  bytes, `TitleSetupThread` **534** bytes and MusicRoom `OnUpdate` **2,258**
  bytes. Exact receipt IDs and their immutable pre-audit-change source binding
  are archived in `docs/RE_ACCEPTED_RECEIPTS.md`. No new exact match unit is
  claimed: the canonical ledger remains **940 / 980**, with **40** open.

- **Original-PE-attested ExAttack special members (October 18):** The
  `ExAttackRecord` and `ExAttackController` constructors are now checked by
  a fresh, *non-crediting*, full-owner diagnostic on **every** `--owner
  exattack` and `--owner all`. The original **15/15-byte record constructor**
  binds its real `ZunTimer::ZunTimer @0x00403390` REL32; the independent
  **28/28-byte controller constructor** under `/O2 /Os` binds the record
  constructor's real `0x004157A0` DIR32 and the original VC7 EH vector
  constructor at `0x00401470` REL32, including the exact **257 × 0x4C**
  array arguments. The functions' original SHA hashes, sizes, instruction
  decodings, every COFF field, and the *separate* `/Ob1` versus `/Os` real
  compiler profiles all agree. The original target does **not** reveal
  whether these empty special members were explicitly authored or implicitly
  compiler-generated, so **both remain `origin unknown/review`, exactness
  credit `none`**. No fake authored exact ledger rows or source-profile
  rewrites. `--owner exattack` also cold-protects the **four original-exact
  ExAttackController.cpp functions** sharing the normal `/Ob1` carrier, with
  codegen and origin fail-closed tests. **Spawn is separately already exact
  (321/321)**; only the two special-member origin classifications and the
  unrelated ECL/Etama/Title/ExAttack big-boss residuals remain open.
- **ExAttack controller full native-carrier and special-member check (October 18):**
  Source `ExAttackController.cpp` shares an authentic VC7.1 `/O2 /Ob1 /EHsc`
  optimization context among **four already exact** functions (`ReleaseExAttackRecords`,
  `Release`, `Register` and `Spawn`). `--owner exattack` now cold-compiles
  and independently strict-original PE/COFF replays **all four**, rather than
  omitting this lifecycle family when checking the hard Type19/21 big callbacks.
  Adjacent constructors `ExAttackRecord::ExAttackRecord @0x004157A0` and
  `ExAttackController::ExAttackController @0x004157B0` independently compile
  byte-for-byte to the entire original **15/15 (one field)** and **28/28
  (two fields)** physical extents under the true separately pinned `/Ob1`
  and `/Os` VC7.1 profiles. The controller's 28-byte constructor uses the
  actual 257-record vector-construction runtime target `@0x00401470`;
  the record's true Timer constructor is `@0x00403390`. The SHA-pinned new
  `inspect-exattack-constructors.py` checks **every original physical byte,
  all three independent call/address operands and complete decoded RETs**,
  but both constructor origins remain explicitly `unknown/review` because
  the retail binary cannot distinguish explicit empty source definitions
  from compiler-generated special members. Neither is counted as new exact;
  the authoritative `config/functions.csv` and exact ledger remain unchanged.
  The fresh full `--owner all` verifies **33 independent full owners**
  (including these two noncrediting provenance checks) plus **184/184
  DISTINCT already exact full-source functions**, up from 31 and 180.
  `scripts/ci.py` passes **489 target-independent tests (28 skipped,
  zero failures)**, plus tracking, documentation, build graph, generated
  progress, immutable target PE identity and whitespace checks.
  Four existing exact ExAttackController functions independently returned
  Factory `pass / accepted` on these source commits: `ReleaseExAttackRecords`
  at `704e6fe`; `Register`, `Spawn` (321 bytes) and `Release` at the
  documentation-corrected `7528a7e`. Their distinct source bindings and
  immutable receipts are archived in `docs/RE_ACCEPTED_RECEIPTS.md`. The
  two unknown-origin constructors still have **no accepted authored exact**.
- **Current Replay big-function machine-code improvement (October 18):**
  Full SHA-bound original Japanese `ReplayManagerView::BeginPlaybackStage
  @0x00420840` is **540 physical bytes**; pinned VC7.1 candidate remains
  **538 bytes**, but source now explicitly performs its FIRST original
  `g_ReplaySideState0` pointer read **after publishing header 1E7 and
  before publishing header 1E8**. Recompilation recovers the target's
  independently decoded order of **all 32/32 call/data fields**, up from
  31/32; the **first 30 real PE/COFF field offsets** now match literally,
  with only the final RNG field/call shifted -1/-2 bytes by the remaining
  two-byte code-length gap. Complete overlapping original-byte differences
  fell from **49 to 39**, protecting **499/538** already matching physical
  positions through an explicit 39-offset fail-closed residual guard;
  original/compiled calls remain **5/5**, blocks **12/12**, no partial
  mask/exact credit. The nine remaining header-window differences are solely
  the target EDX/ESI register allocation, with 30 in the seed/tail. A late
  scoped volatile pointer *read* preserves the original source-visible
  distinct owner load; remaining subsequent global SideState accesses are
  intentionally NOT cached. Twenty-four isolated header load/store variants
  and earlier SideState variants were checked; most are byte-neutral or
  change the original read multiplicity (32 to 29 fields), so were rejected.
  This is **a net ten-byte full-owner improvement**, not a newly exact
  function. The complete **12 exact ReplayInput siblings**, both real EH/noEH
  compiler profiles and separate GameManager callee guards remain mandatory.
  Fresh source-cold `--owner all` passes **31 complete boss/leaf original
  diagnostics and 180/180 unique exact sibling comparisons**; CI passes
  **482 tests (28 skipped, 0 failed)**, and immutable target, ledger,
  documentation, progress, build-graph and whitespace checks all pass.
- **Replay large-boss full exact-source profile guard (October 18):**
  Original `ReplayManagerView::BeginPlaybackStage @0x00420840` remains an
  original-PE-bound **540-byte** owner against **538-byte** source, 32 linked
  operands, five ordered calls, twelve direct blocks, **39** unlike overlapping
  bytes (down from 49) and two missing source bytes; it is **NON-EXACT**. Its native
  `ReplayInput.cpp` source contains **12** already exact functions in TWO
  *different* original VC7.1 profiles: **two in the no-`/EHsc`
  `ReplayInputNoEh.obj`** (BeginRecordingStage, Create) and **ten in the
  standard `/EHsc` `ReplayInput.obj`** (Update, Release, frame sync,
  record/playback input and related members). `--owner replay` now guards
  **all 12** complete exact siblings after a fresh cold compile of both
  pinned carrier/profile combinations, up from two. The full `--owner all`
  runs Replay before the still-open GameManager gameplay setup and therefore
  does **not** repeat the already-verified exact `ReplayManager::Create`
  **or** `ReplayManager::Release`; focused `--owner gamemgr` still
  individually rebuilds/replays those real callee ABIs. Fail-closed tests
  reject missing source units, compiler profile or output swaps, duplicated
  exact functions and stale/false prebuilt carriers. The source/target
  canonical function ledger remains **unchanged**; the subsequent scoped
  first-side-state-load correction restores all 32 linked original field
  identities/order and narrows the open boss from 49 to 39 bytes without
  claiming exactness. This is full-owner regression protection and genuine
  bounded codegen progress, not a new exact closure. Fresh cold
  `--owner all` passes **31 full physical boss/leaf diagnostics and 180/180
  UNIQUE existing exact member checks** (up from 171), with both Replay
  callee identities counted just once. `scripts/ci.py` passes **481 tests
  (28 skipped, zero failures)**, along with authentic PE SHA, tracking,
  documentation, progress, build graph and source whitespace checks.
- **Current EffectManager original-codegen guard (October 18):** The still-
  nonexact **51-byte** `EffectManager::AddedCallback @0x0040D1C0` is embedded
  in the SAME pinned `/O2 /Ob1` original VC7.1 compilation carrier as
  **15 separately exact `EffectManager.cpp` functions**. Previously
  `--owner effect` rebuilt that real carrier but rechecked only three
  exact update/draw methods. The original-bound owner now checks the entire
  **15/15** complete PE/COFF codegen family, including pool lifecycle,
  spawning, update, initialization and drawing; the 51-byte open callback
  remains **4 authentic fields, 3 blocks, 2 calls, 8 unlike original bytes**.
  Missing exact units, duplicate symbols, mixed compiler profiles, changed
  carrier and skipped comparisons fail closed. More than thirty complete
  ExAttack Type19/21 first-vertex/iterator source-shape experiments and
  seventeen Title cursor stack-home variants were independently compiled
  this session against the original; they did not improve the authentic
  **6/9** ExAttack or **38-byte** cursor frontiers and were **not** adopted.
  A fresh source-cold `--owner all` sweep passes **31 whole owner
  diagnostics** and **171/171 distinct exact functions**, up from 159;
  `scripts/ci.py` passes **477 tests, 28 skipped, zero failures** along
  with target SHA, ledger, docs, open build graph, progress and whitespace.
  Independent Factory strict-live forced-cold `pass / accepted` regressions
  on gameplay-source **`7d3f89b`** separately reconfirm `InitializeEffect`
  (407 bytes), `Create` (327 bytes), `SpawnEffectWithVelocity` (175 bytes)
  with exact physical bytes and 17/27/1 real field identities respectively.
  Their immutable source-bound receipts are in `RE_ACCEPTED_RECEIPTS.md`.
  This is original-byte regression protection, NOT newly accepted exactness.
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
- **Newly canonical exact:** `FileSystem::TryDecryptFromTable @ 0x0042C290` cold-replays **220/220 entire code bytes and all 12 original-bound COFF fields**. Source-cold VC7.1 recovers the original row-offset/key-bias schedule without changed call/branch/return behavior. `--owner crypto` strictly rebuilds this exact selector as well as independently exact Decrypt/Encrypt siblings. Historical 38-byte nonexact experiments are superseded. Independent Factory `strict-live-v1` forced-cold `pass / accepted` receipt `e5fbed2e35363191833752be9a9daef1968ab0ec01a83b06088188cad80ea5a9` binds exactness to gameplay-source commit `9590206`; it does not prove whole-program link/runtime closure.
- **Latest code checkpoints (pre-handoff HEAD `944d95c`):**
  `e96bc16` narrows **ANM `ProjectCameraFacingQuad` from 13 to six**
  independently linked ordinary byte differences, with all 40 fields, 11
  calls and four exact siblings unchanged; `3223caf` restores the target's
  **two observable GameManager `limitB4` publications** (function remains
  nonexact); `ea5242d` pins 172-versus-171 `GameplaySetupThread` source/PE
  address-referent identities; `944d95c` protects **489 already exact bytes**
  within the still-nonexact 540/538-byte `Replay BeginPlaybackStage`.
  Earlier CalcShakeEnvelope (28→26) and 0x1EC Replay record corrections are
  retained. `RunEcl` remains **15,564 physical bytes / 598 authentic operand
  fields / 30 ordinary differences**, no table/field drift. None of these
  nonexact-owner improvements adds canonical exact function credit.
- **Cold regression scope:** the live full `--owner all` suite now verifies
  **30 open non-crediting boss/leaf original-bound functions plus one
  separately canonical-exact 220-byte crypt selector diagnostic and 146
  strict already-exact sibling comparisons**, all passing at the current
  checkpoint. The new, separately SHA-bound **959-byte ExAttack Type14/22
  Draw** original owner is checked on every `--owner exattack` and full replay;
  it emits 974 source bytes and remains NON-EXACT. The suite also protects
  the 15,564-byte RunEcl, 600-byte Etama OnDraw, 3,900-byte Enemy OnUpdate,
  1,280-byte ReplayMenu and 1,939-byte DrawResult owners. These diagnostics
  are not source-commit-bound accepted Factory receipts.
- **Pre-cleanup verification (2026-10-17):** a **fresh cold** original-PE/COFF
  `--owner all` sweep passed **31 owner reports (30 noncrediting plus one
  already-exact crypto selector diagnostic)** and **72/72 separate strict
  exact sibling units**. `scripts/ci.py`: **437 tests, 28 skipped, success**;
  target SHA, ledger, progress and docs passed. Evidence is retained under
  `.analysis/handoff-cleanup-20261017/`; this is not a new Factory
  source-commit-bound exactness receipt.
- **Latest live source-cold checkpoint (October 17):** The common
  `BulletSideStateView` now declares the target-backed `EffectManager *` at
  **+0x0C**. `EtamaController::OnDraw` calls the actual exact
  `EffectManager::DrawBulletLayerEffects` through that shared owner, replacing
  a duplicate private view without changing its physical **600/600**, 27
  address operands, 13 real calls, 17 blocks or **268 nonexact bytes**.
  The affected **`EtamaController::OnUpdate` 2,216/2,216**,
  `DrawSingleBullet` **232/232** and `EnemyPostEclUpdateMovement` **992/992**
  original physical bytes remain exact. VC7.1's source-label numbering changed
  across Etama/ECL carrier functions; the refreshed exact manifests were
  checked against original label destinations before a full replay. The live
  full `--owner all` now cold-checks **31 owner diagnostics** and **133
  independently exact sibling comparisons**, including every exact match in
  the 24-unit ECL and 38-unit TitleScreen source families, plus the separate
  MusicRoom and DrawCallback checks. These are noncrediting regressions, not
  a new canonical exact function. The 980 / 940 / 40 scoreboard is unchanged.
  The committed checkpoint passed `scripts/ci.py` with **446 tests, 28 skipped**,
  plus target SHA, tracking, documentation, progress and whitespace checks.
- **Latest big-owner source improvement (October 17):**
  `ProjectCameraFacingQuad @0x00439820` now cold-compiles **622/622**
  bytes with **only four full-owner original-linked ordinary differences**,
  down from six. The natural unsigned-word copy maintains the actual
  three-word data flow and all **40 real target fields, 11 calls, eight
  blocks, 201 instructions**. The four residual offsets are
  `+0xF6,+0xF7,+0xF9,+0xFA`, reflecting only the final Y/Z store order;
  the previous six-offset copy model is now superseded. The focused original
  replay protects **all four already exact ANM sibling functions**, with
  a new four-offset fail-closed frontier; exactness of the big owner remains
  unclaimed.
- **Enemy large-owner genuine Etama dispatch (October 17):** The
  **3,900-byte** `EnemyManager::OnUpdate @0x00410730` now calls the
  separately original-exact **203-byte** `EtamaController::SpawnBulletPatternPrimary`
  through the *real* `EtamaController` and `BulletSpawnDescriptor` interface,
  removing `EnemyCoreEtamaControllerView`'s speculative callback type.
  The source retains the independently exact **28-byte** descriptor-constructor
  call and target-checked **0x214-byte layout**, with C++ offset assertions
  for the consumed physical fields. The source-cold whole-owner audit remains
  **3900/3900, 97 bound fields, 69 calls, 170 matching direct blocks and
  29 ordinary unlike bytes**, and its exact same-TU owners are unchanged.
  `--owner enemy` now adds both genuine callee cold replays to the five
  already-exact protections, increasing the complete `--owner all` sibling
  comparison scope from **131 to 133**. The current fresh-cold `--owner all`
  sweep passes **31 physical owner reports and 133/133 original-bound exact
  siblings**; `scripts/ci.py` passes **448 tests, 28 skipped** plus target,
  tracking, docs and progress gates. No new exact credit is claimed.
- **Current complete native-carrier exact protection (October 17):** The
  still-open **622-byte ANM `ProjectCameraFacingQuad`** compiles alongside
  **15 exact** `AnmManagerRender.cpp` functions (including `Draw2D`,
  `TranslateRotation`, flush/set-state helpers and both drawing paths). All
  **15/15** are now independently original-PE/COFF replayed from the freshly
  built **same** real VC7.1 Render object, rather than checking just three
  nearby members. The separately compiled **595-byte exact `Project3DQuad`**
  is also replayed (ANM total **16**). The still-open **600-byte Etama
  `OnDraw`** now strictly protects both exact same-carrier functions,
  `DrawSingleBullet` **232/232** and `EtamaController::AddedCallback`, plus
  separately built **2,216-byte exact `OnUpdate`** (Etama total **3**).
  Inventory cardinalities, compile profiles and output carrier identities
  fail closed, including synthetic missing-member regression cases. The
  freshly source-cold `--owner all` passes **31 full original-bound owner
  diagnostics** and **146/146 strict existing exact unit checks**, up from
  **133** at the preceding checkpoint. `scripts/ci.py` passes **452 tests
  (28 skipped)** with target, tracking, documentation, build-graph and
  progress checks. No source function is newly exact;
  canonical progress remains **980 source-present / 940 exact / 40 open**.
- **ECL helper full-byte no-regression guard (October 17):** Reopened
  the independently SHA-attested retail `MoveRandomBiased @0x00407E30`
  in the same freshly compiled 15,564-byte `RunEcl` translation unit.
  The helper remains **516 original / 517 candidate physical bytes**;
  all **28 real COFF fields, 14 ordered calls, 24 CFG blocks and 145
  instructions** agree in identity/count, but **148 overlapping bytes**
  differ (and one source byte is extra). Full original-linked comparison now
  pins all 14 exact residual runs and rejects any new wrong byte among the
  already matching **368 original positions**. A second guard checks the
  only two differing normalized instructions: the repeated post-`Float3`
  Y-position comparisons. Independent source-cold 20-form Y-alias probes
  either retain 517 bytes or regress to **518/526 bytes**; no source edit
  masquerades as an exact solution. `--owner runecl` still independently
  protects all 24 exact ECL siblings; `RunEcl` itself remains 30 bytes open.
  The fresh full `--owner all` passes **31 diagnostics and 146/146 exact
  siblings**; `scripts/ci.py` passes **458 tests (28 skipped)** with tracking,
  docs, build-graph, progress and whitespace validation.
- **ReplayManager true owner shared across three source families (October 17):**
  The still-open **1,689-byte** `GameplaySetupThread @0x0041AF2D`
  now calls the *genuine* independently exact **318-byte**
  `ReplayManagerView::Release @0x00420AD0`, not the speculative
  `ReplayManagerSetupReleaseView` method alias. A common reviewed
  `ReplayManagerRuntimeView.hpp` holds the real **0x164-byte** ReplayManager
  layout and 0x18-byte ReplayBufferLink, with compile-time field offsets,
  for `ReplayInput.cpp`, `GameManagerSetup.cpp` and `TitleScreen.cpp`.
  The separate **42-byte** `ReleaseStageObject @0x00420C10` remains a distinct
  teardown contract. The original-linked GameplaySetupThread retains
  **1,689/1,689 physical bytes, 171/172 source/original fields, 35 ordered
  calls, 77/78 source/original blocks and 911 unequal bytes**; this is a
  real call/type correctness gain, **not** a newly exact boss. Independently
  cold compiled all **60/60** existing exact source functions across their
  **six real compiler carrier/profile groups** without a byte/field failure:
  12 ReplayInput, 10 GameManagerSetup and 38 TitleScreen. Nine compiler-local
  `OnUpdateStartMenu` `$L` labels changed only in spelling and were checked
  individually against their unchanged real original target addresses before
  refreshing their manifest names. Focused `--owner gamemgr` now additionally
  protects the true 318-byte Release and 361-byte Create exact callees;
  full `--owner all` de-duplicates Create because the replay owner already
  checked that no-EH source TU. The complete fresh `--owner all` passes
  **31 original-bound diagnostics and 147/147 DISTINCT strict exact sibling
  reports** (no duplicate Create credit); full `scripts/ci.py` passes
  **465 tests, 28 skipped**, plus tracking, docs, build graph, progress and
  whitespace validation. The canonical scoreboard remains
  **980 source-present / 940 exact / 40 nonexact**.
- **Latest whole-carrier ScreenEffect guard (October 17):** Independently
  re-opened the **336-byte `ScreenEffect::CalcShakeEnvelope`** and its real
  source compiler profiles, rather than trusting the old six-function
  dependency sample. Its maintained body still has **336 physical bytes,
  132 instructions, 20 authentic bound fields, 8 calls, 26 matched direct
  blocks** and **26 genuine different original bytes**; no exact credit.
  The exact-source `ScreenEffect.cpp` manifest contains **18** functions:
  **16** ordinary `/O2 /Ob1 /EHsc`, **one `RegisterChain` without `/EHsc`**
  and **one `DrawSquare` with `/Ob0`**. `--owner screen` now cold-compiles
  all **three** physical carriers, strict original-PE/COFF replays **18/18**
  existing exact bodies (previously 6), and rejects any missing member,
  changed compiler profile, changed object, duplicated function or false
  reused build. Complete `--owner all` now passes **31 owner diagnostics
  and 159/159 distinct original-byte exact sibling reports**. CI
  passes **469 tests (28 skipped, zero failures)**; target, tracking,
  documentation, progress and source whitespace gates pass. Separately
  cold-compiled nineteen legal ScreenEffect owner/timer/reference-source
  variants; neutral forms remain at 26 differences, early timer aliases
  degrade to 337 bytes /317 mismatches. The actual target is unchanged.
- **Independent Factory acceptance, ScreenEffect three source profiles:**
  Original-bound strict-live forced-cold jobs bound to exact gameplay-source
  commit **`cbfae2185e4ab8bacbc8cf95b1b5bc77c80f5c5f`** each return
  **`receipt_verdict=pass`, `acceptance_decision=accepted`**:
  standard `/O2 /Ob1 /EHsc` exact `ScreenEffect::CalcShake @0x00422660`
  (316 bytes; `receipt:4a636fe2e0141d41075ae7b69ced9123ba98a66aae7bff3002bb0f8f6fff4d83`),
  separately compiled no-EH exact `ScreenEffect::RegisterChain @0x00422D20`
  (447 authored/480 physical bytes; `receipt:ec759b50d81d19c2095b023d53e46cc94bb4bbe42aee92e8bcccdb958e2ea8dd`),
  and `/Ob0` exact `ScreenEffect::DrawSquare @0x00422960`
  (527 bytes; `receipt:a076aa0b462bca701b2b8f33e61081f9cdd5e0d36b8374f01382f2656f673379`).
  These confirm **three already exact functions under the new code commit**,
  not a previously open `CalcShakeEnvelope` match. Native whole-product
  link/gameplay, semantic and port stages are still open.
- **Phase:** original exact-reconstruction only. The 95% reviewed-authored
  byte milestone is open; whole Windows i386 build/link/resources/runtime,
  dual-Oracle semantic reconstruction and portability are still open.

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


### Current cleanup and evidence boundaries (2026-10-17)

**Fresh, tested handoff cache cleanup completed.** At gameplay-source HEAD
`944d95c`, the SHA-pinned original, 31 complete owner checks, 72/72 strict
exact sibling checks, public CI **437 tests (28 skipped)**, tracking, docs
and progress passed **before deletion**. The repository was clean; no
uncommitted user source was reset or overwritten. Following the reviewed,
preflighted `scripts/clean-generated-artifacts.py --apply` whitelist:

| Proven disposable category | Files deleted | Logical bytes released |
| --- | ---: | ---: |
| Entire ignored generated `build/` tree | 376 | 9,539,846 |
| Ignored `.analysis/` compiler OBJ/PDB/PCH/PYC/ILK/IDB | 4,858 | 339,950,751 |
| Reviewed Python-only `scripts/__pycache__/` and `tests/__pycache__/` | 39 | 656,008 |
| **Total** | **5,273** | **350,146,605 (334.0 MiB)** |

The immediate **post-cleanup dry-run reports zero** `build/` or `.analysis/`
compiler cache files and `build/` is absent. No tracked C++ source, manifest,
unit test, document, target bytes, pinned tools, Factory receipt, or original
replay evidence was removed. The target remains SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Exact per-file deletion manifests are ignored but **retained** in:

- `.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`,
  **last `runs` entry only** (the top-level summary is cumulative), for
  `build/` and compiler binaries.
- `.analysis/handoff-cleanup-20261017/pycache-cleanup-audit.json` for the
  extra 39 bytecode files, with original SHA-256 for each.
- `.analysis/handoff-cleanup-20261017/precleanup-full-boss.json` and
  `precleanup-ci.log` for the complete **original-bound pre-deletion** tests.

**Intentionally retained:** approximately **1.4 GiB allocated** under
`.analysis/` remains, chiefly historical experiment C++/INL/header copies,
original-bound JSON outputs, compiler-probe scripts and unknown legacy
files. These are **not** classified as disposable merely by extension or
age. Preserve the candidate/producer/input relationship until independently
reviewed, and see
[`RE_ARTIFACT_CLEANUP_HISTORY.md`](RE_ARTIFACT_CLEANUP_HISTORY.md)
for earlier sweeps and the complete preservation policy. In particular,
**never bulk-delete `.analysis/`, IDA/provenance data, `.tools/` or
`resources/th09.exe`.**

**Restart after clean handoff:** run `git status --short --branch`, verify
original SHA and ledgers, then cold compile a relevant match unit, e.g.
`python3 scripts/build-match-unit.py --unit title-screen-setup-thread`.
Only then may `compare-coff-function.py` or its original-bound inspector
consume the freshly generated `build/matching/...obj`. A new full owner
sweep or CI **recreates ignored build outputs**; this is expected. At the
next final handoff, run CI/boss checks *before* repeating the reviewed
cache-cleaner dry-run/apply. For post-deletion docs-only verification, set
`PYTHONDONTWRITEBYTECODE=1` **and** use `python3 -B ...` so that both the
main Python interpreter **and child Python processes** skip bytecode; such
a check is not an original-byte accepted receipt.

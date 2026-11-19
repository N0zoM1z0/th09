# TH09 reconstruction — current agent handoff (2026-11-19)

This is the restart guide for the live TH09 repository. It records current
status and the next useful work, not a session diary. Read
[AGENTS.md](../AGENTS.md) first, then this file. Use
[PROGRESS.md](PROGRESS.md) for generated totals and
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) for historical target/compiler findings.
The canonical ledgers and current source always outrank dated prose.

## 2026-11-19 checkpoint: native ExAttack, score, MIDI, Enemy and Player sources

Latest local source checkpoint `9f672b4b` retains the preceding seven
natural C++ source commits and verifies their affected exact owners. The
canonical ledger is **1,026 exact / 1,037 authored** with **11 remaining
source-present nonexact** owners. None of the source repairs below earns
additional exact-function credit; no full cold replay was performed.

- `ba29fb09`: the **1,436-byte** ExAttack type-18/24 update now holds the
  real four-element `PlayerAnmVmView` array through its native record,
  eliminating 15 repeated `void*` conversions. The bounded trail-index
  arithmetic and state increment are natural C++; complete owner and **44
  relocation fields** pass. Breaking its original chained trail/rotation
  publications into separate statements changes three bytes and was rejected.
- `01cda8f4`: `ScoreFileView::OpenScore`, `LoadScoreRecords`, `LoadLastName`
  and `LoadPlayStats` traverse the actual score file's byte buffer instead
  of repeatedly converting addresses to DWORDs. The seven selected exact
  owners replay **2,572 bytes and 169 relocation fields**.
- `a72af11f`: **1,507-byte `SampleNetworkInput`** reads both controller mode
  bytes through the genuine `GameConfiguration` object representation,
  without inventing a second array or caching mutable mode values.
  Selected source replays **1,507 bytes / 13 relocations**.
- `746df0bd`: restore real MIDI/SMF header, MTrk and event byte cursors,
  plain bounded counters, and the original independent event reads. The
  affected `/Ob1` Midi carrier replays **32 exact units / 4,270 bytes /
  132 relocations**. All 23 compiler-private local-label renames in
  `ProcessMsg` were independently checked against their original physical
  same-section target addresses before rebinding manifest symbols.
- `f1b5a9e3`: native `EnemyManagerDrawImpl` samples the real enemy trail
  array, bounded neighbor indices and vertex count without integer-address
  scaffolding. Selected source replay passes **4 units / 1,922 bytes /
  61 relocations**. The original integer-expression UV step must remain:
  simplification moved its floating-point evaluation and caused 596 unlike
  bytes in the 1,758-byte owner.
- `c7e6636b`: the exact **273-byte** `UpdatePlayerCollisionRegions` scans,
  compacts and appends through the genuine active/released 514-slot pointer
  arrays, with deliberate explicit DWORD wrapping only for potentially
  unbounded signed lifetimes. Scoped `PlayerRuntime.cpp` replay:
  **21 exact owners / 5,391 bytes / 288 relocations**. Eleven private
  `player-update-before-state` labels had unchanged physical destinations.
- `c2159930` and follow-up `9f672b4b`: `LoadPlayerShtFile` rebases its
  genuine file-relative shot-descriptor offset through the file buffer's
  byte pointer instead of aliasing pointer storage as `unsigned int&`.
  Its **172-byte / 6-relocation** owner and the same **21 selected exact
  PlayerRuntime units / 5,391 bytes / 288 fields** replay. Private-label
  names were verified and rebound at unchanged PE addresses.

Unretained: fifteen real-array C++ variants of `AnmManager::ReleaseAnm`
changed **73 bytes** in its 163-byte exact owner, even though the separate
87-byte preload-service owner remained exact. The unusual projection is
still unresolved; do not substitute an unproven C++ array type. The separate
`fsincos` helper uses the retail x87 `fsincos` instruction (33 bytes), while
ordinary C++ `sinf`/`cosf` yields 36; no ASM workaround is admissible.

The unresolved nonexact frontiers have not been softened:
`EnemyManager::OnUpdate` remains 3,900 bytes / 2 residuals;
`EffectManager::AddedCallback` 51 bytes / 4; `UpdateChargeAttack` 1,210
bytes / 183; `CheckBulletCollision` 849/862 bytes / 539 and
`UpdateNetworkService` 1,650/1,633 bytes / 914. The last two remain
**strictly REGRESSED** against older ratchets despite lower raw differences.
No ASM, padding, register/liveness coaxing, or target/ledger fabrication.

## 2026-11-19 checkpoint: Front, ANM loading and Enemy pool source

Latest local code checkpoint `e56dfb9b` contains **seven source commits**,
all based on original Japanese TH09 v1.50a and the pinned VC7.1 profiles.
There are still **1,026 canonical exact functions** and **11 source-present
nonexact authors**. These commits make existing exact C/C++ more faithful;
they are not new exact credit or a whole-product Windows link/runtime claim.

- `a718ba48`: `FrontSide::OnUpdate` reads GameManager's adjacent
  +0x108/+0x10C native marker-count DWORDs through one C++ word cursor
  rather than converting the base address to an integer. All **15 same-source
  exact owners / 4,087 bytes / 176 relocation fields** replayed.
- `6f1ae443`: both Front panel timer and color macros now index the real
  per-side native arrays. The two affected exact source owners total
  **2,244 bytes / 145 relocations**, including `FrontCalcCallback` and
  Player's death callback.
- `ea54fbfe`: Front's panel-vertex macro directly indexes its true `[2][5]`
  `FrontPanelVertexView` grid instead of scaling integer addresses. All
  **3 affected exact owners / 3,671 bytes / 271 relocations** pass. The
  overlaps with the previous commit are not additional function credit.
- `a158b532`: the **1,491-byte `FrontCalcCallback`** accesses the real
  two `short` neutral-message selections at file-record offsets +8/+10,
  naturally decrements the native positive panel timer, and increments
  the transition frame. Complete **1,491/1,491 bytes, 107 relocations**
  are exact, including the old callback order and separate live reads.
- `f8be09ce`: ANM loader `ReadAnmEntries` and `PostloadAnmEntry` advance
  through actual variable-length file data with byte pointers; native
  counts use ordinary increments and surface tables use direct indexing.
  The deliberately unusual +0x24-crossing `AnmLoadSlot` projection was
  **not** replaced by a false array representation. All **17 source exact
  units / 3,258 bytes / 133 relocations**, including the `/Ob0` constructor
  and `/Ob1` loaders, pass.
- `35ca5c84`: both `EnemyManagerView::SpawnEnemy` and the native card-attack
  spawn helper iterate the real bounded 128-entry 0x5430-byte Enemy pool by
  `++enemy`/`++enemyIndex` instead of integer address words. Both exact
  original functions pass (**373 + 364 bytes, 8 relocations**).
- `e56dfb9b`: ExAttack limits/difficulty scales and ASCII boss-marker slots
  use their genuine DWORD tables directly. Three already exact leaf owners
  replay (**174 bytes, 11 relocations**).

A focused **12-variant** `CheckBulletCollision` attempt used the adjacent
Cirno-Labs TH09 hit-dispatch source as a hypothesis: circle success fell
through to the `hit` block and rotated/AABB success jumped backward.
With genuine native types it instead emitted 807-810-byte bodies and
716-730 residuals, losing the correct EBX/ESI/EDI prologue. Every failed
variant was restored. Keep the current 849/862, 539-position candidate's
real 0x34-byte stack frame; the old strict frontier remains REGRESSED.
`UpdateNetworkService` also remains 1650/1633, 914-position REGRESSED.
Neither residual baseline was edited. No full cold replay, ASM, padding,
fake relocations, dummy register lifetimes or target modification occurred.

## 2026-11-19 checkpoint: native background/ANM bosses and Supervisor leaves

Newest local source checkpoint `18d308d3` includes **five reviewed code
commits**. All target comparisons use original Japanese v1.50a, its verified
hash and pinned VC7.1 profiles; no broad cold replay or fabricated output.
No new canonical exact function was awarded for maintaining existing exact
sources.

- `fdb7709f`: `Background::RenderObjects` now uses the actual stage-object,
  ANM VM and special-effect arrays and the encoded signed byte step for
  variable-length stage quads. Its complete **3,274/3,274 bytes and 113
  relocations** pass a source-scoped replay. Replacing the native 12-byte
  effect point copy with three scalar float writes instead emitted a
  3,298-byte candidate / 175 residuals; that experiment was reverted.
- `3defefe9`: the **9,358-byte `AnmManager::ExecuteScript` interpreter** uses
  the script's signed integer byte offsets for three jump instructions and
  directly increments the real manager's per-frame script counter. Its
  **9,776-byte physical owner** (including the 418-byte tail/switch data)
  remains completely exact with 368 relocations. Both canonical compiler
  profiles `/Ob0` and `/Ob1` were scoped-replayed: **5 exact units / 10,496
  physical bytes / 421 relocation fields**. All 147 renamed compiler-local
  symbols were separately checked to resolve to unchanged PE target
  addresses in their owning COFF sections before updating the manifest.
- `0de883bf`: `BackgroundRunStageScriptPhase` now uses the real instruction
  and camera viewport arrays, simple DWORD color assignments, and the next
  typed 0x14-byte wire record. The complete **2,652-byte physical owner**
  (2,472-byte body + 180-byte attachment) is exact. The scoped carrier passes
  **5 exact units / 3,231 bytes / 160 relocation fields**. All 54 changed
  interpolation/stage-script local labels resolved to the same original
  same-section targets before their manifest spellings were rebound.
- `cb27a128`: `SupervisorCheckFps` uses the real 30-float sample array and
  native frame count increments; `SupervisorDrawLoading` uses the genuine
  three VM array. Both complete owners remain exact: **517 + 71 bytes** and
  **34 + 5 relocation fields**. No invented bounds checks or stale buffers.
- `18d308d3`: the Ending callback accesses native 16-element Ending VM and
  global script path tables rather than scaling their base pointers through
  integers. The shared VM macro now uses an actual C++ array element; the
  selected callback replays **800/800 bytes with 85 relocations**. The wider
  Ending interpreter was not edited or reclassified.

The actual nonexact work remains: **11 source-present nonexact owners**,
notably `EnemyManager::OnUpdate` (3,900 bytes, two unlike register bytes),
`PlayerLifecycleView::CheckBulletCollision` (849/862 bytes; strict frontier
REGRESSED), `Supervisor::UpdateNetworkService` (1,650/1,633 bytes, strict
REGRESSED) and `PlayerLifecycleView::UpdateChargeAttack` (1,210 bytes,
183 unlike positions). This checkpoint changes neither the original 1,026
exact-function ledger nor any nonexact frontier ratchet. No ASM, padding,
register scheduling tricks, target patches or full cold replay were used.

## 2026-11-19 checkpoint: native ECL interpreter and front-message tables

Latest code checkpoint `1cb188d8` includes **four natural C++ source commits**.
All are target-bound by Japanese TH09 v1.50a and the pinned VC7.1 compiler;
no owner or relocation is promoted merely because source text looks closer.

- `09f887fc`: in the 187-way **`EclManager::RunEcl` interpreter**,
  recover the actual eight 0x30-byte interpolation-slot array, the real
  interpolation callback table, native byte-pointer movement through
  variable-length instructions and their two call/subroutine advances.
  Complete **15,564-byte physical owner** (14,792 executable bytes plus
  its 772-byte switch table) matches exactly. All **84 exact functions /
  45,420 physical bytes / 1,987 relocation fields** in `EclManager.cpp`
  replayed. 203 changed local COFF symbol *names*, including the named
  `th09_ecl_advance_instruction` branch destination, were independently
  resolved against the same original same-section target addresses before
  refreshing `config/match-units.toml`. The separate Enemy OnUpdate
  candidate remains **3,900 bytes with 2 unlike positions**.
- `e7bcf686`: restore the **`FrontMessageRuntimeView::Update`** variable-
  length text/command payload to native byte pointers; use direct indices
  for the genuine eight portrait ANM VMs. The complete owner including its
  attached switch table is **2,968/2,968** with all 167 relocations.
  Thirty local label names were resolved against the original physical
  jump destinations. Both pinned-source profiles pass **2 exact units /
  3,007 bytes / 167 relocation fields**, including the separately compiled
  39-byte message-string decoder.
- `76b45a16`: remove the integer-address three-dimensional Title name
  helper and index the true `g_TitleNameTable[bank][row][record]` directly
  at all seven call sites. The **966-byte result-name entry remains exact**;
  nine local `$L` names were independently target-checked. Full affected
  Title replay: **41 exact functions / 18,972 bytes / 953 relocations**.
- `1cb188d8`: timeline event opcodes 13 and 14 read their encoded argument
  through the actual file-backed instruction byte cursor rather than
  converting its pointer to an integer and back. Complete
  **1,048-byte physical `EnemyScheduleRuntimeView::Run` owner** and
  **42 relocations** are exact; scoped replay covers **2 exact units /
  1,068 bytes / 42 relocations**.

The maintained ledger is still **1,026 exact functions / 11 source-present
nonexact authors**. None of these exact-source repairs is newly awarded
exactness credit; Windows build/link and runtime validation remain open.
Remaining top targets are the `CheckBulletCollision` hit-region CFG,
`Supervisor::UpdateNetworkService` receiver lifetime, Player charge side
register scheduling, and the two-byte Enemy/Replay frontiers. Do not weaken
strict nonexact ratchets or use ASM, padding, register/liveness tricks, or
full cold replay to force acceptance.

## 2026-11-19 checkpoint: key config, snapshot, popup and music source

The latest source checkpoint is `056bb831`. Four new natural C++ edits were
validated against the pinned VC7.1 and the verified original Japanese TH09
v1.50a target, strictly within their affected exact-owner sources:

- `15888a7f`: `TitleScreenView::OnUpdateKeyConfig` directly indexes the native
  `sideFocusMode[keyConfigSide]` rather than repeatedly casting an original
  two-side selector. The **2,803-byte exact boss** and its same-source owner
  replay as **2 functions / 2,909 bytes / 118 relocation fields**.
  An honest alternative `GameConfiguration::sideDeviceMode[2]` union for the
  adjacent `valueB7/valueB8` bytes remains **nonexact by four different ModRM
  index/base bytes** at `+0x6B6`, `+0x6CC`, `+0x82E` and `+0x843`; it was not
  committed or adopted as a shared header change. Removing the original
  unsigned word semantics from the opposite-side selector likewise causes
  four differences and was rejected.
- `5c3afa5d`: `Supervisor::TakeSnapshot` now walks the actual Direct3D locked
  pixel buffer and allocated BMP output through byte pointers, with no
  integer-to-pointer round trips. The **543-byte exact function and 23
  relocations** all replayed; channel-copy ordering is unchanged.
- `b7a50520`: `AsciiManager::OnDrawHighPrioImpl` and `OnUpdate` use real
  two-by-100 score-popup arrays, independently reread live player sides, and
  traverse full `0x38`-byte popup records as C++ pointers. All **10 exact
  owners / 4,206 bytes / 264 relocations** passed. Nineteen compiler-private
  label names in the same carrier were independently checked against exact
  original same-section addresses before the manifest names were rebound.
- `056bb831`: removed raw integer-address music descriptor and unlock-byte
  macros in `SupervisorAudio.cpp`, replacing them with live native table
  elements and an unsigned DWORD unlock index. All **8 exact functions /
  1,054 bytes / 82 relocations** passed. No new bounds check or cross-callback
  field cache was introduced.

Additional target-bound negative experiments were restored: a direct network
queue-synchronization success-first branch rearrangement regressed the large
1,633-byte `UpdateNetworkService` owner (1,644-byte candidate, 917 differences);
separating its wait status also regressed. Fifteen natural address/reference/
byte-index expressions in `EnemyManager::OnUpdate` all retain the same two
EAX/ECX register byte differences in the 3,900-byte candidate. Twelve natural
assignment, reference and inline-publisher variants of `EffectManager::AddedCallback`
retained 4 register differences or regressed to 7 (a volatile owner alias
expanded the body to 60 bytes). None was retained or used to rebaseline.

The authoritative ledger remains **1,026 exact functions**, with **11
source-present nonexact authors**. Source refactoring does not earn new exact
credit. The full Windows i386 build and runtime validation remain open. No
full cold replay, ASM, register tricks, byte padding or target patching was
introduced.

## 2026-11-19 checkpoint: native PLST rows and network frame queues

Latest local code checkpoint `bfc3125f`; all six source commits in this pass
used the pinned VC7.1 compiler and original Japanese TH09 1.50a. No whole-game
cold replay was run. Source/header changes do **not** create new exact credit.

- `7cb92568`: rewrote **all seven** PlayStats leaves to use the real
  `characterStats[character][index]` and `characterUnlocked[row][character]`
  arrays. Importantly, the five independent `HasAnyPrimaryCharacterStat`
  short-circuit reads must be repeated in source; caching the row changes the
  75-byte target body to 62 bytes. Scoped exact replay passes **7 functions /
  213 bytes / 0 relocation fields**. No new bounds gate was introduced.
- `5d527568` and `7c4bdc1c`: recovered direct Supervisor
  `entries47C[index]`, `lastReceived56C[side]` and ReplayInputState
  indexing, pointer `+1`/`-1` shift steps, and actual byte-pointer traversal
  of the two side queue windows. The original field-copy ordering and the
  final native-address store idiom remain. Scope: **2 exact functions /
  310 bytes / 1 relocation** (`InsertPredictedFrame`, `PopFrame`).
- `8b277b15`: restored incoming queue writes/reads and queue-sync head
  tests to the original typed Supervisor array elements, preserving the
  native flat `unsigned(side)*10+index` address calculation. Scope:
  **2 exact functions / 262 bytes / 0 relocations** (`InsertReceivedFrame`,
  `AreFrameQueuesSynchronized`). Together the queue and PLST edits prove
  11 complete existing exact units / 785 bytes / 1 relocation.
- `618317ac`: restored the column selection and animation interpolation
  spelling from the adjacent TH09 menu source in the nonexact
  `TitleScreenView::DrawReplayMenu`. Whole **1,280 candidate bytes and all
  57 relocations are unchanged**. Retail has the same 1,280-byte extent but
  retains **29** differing register-use bytes.
- `bfc3125f`: recovered ordinary original-source `++frameCounter`,
  `60/syncRate - 1 + frameCounter` and `1 - side` formulas in the large
  `Supervisor::UpdateNetworkService`, preserving its real overlapped 20-byte
  stack home, native compiler output **1,650 bytes** and **106 unchanged
  relocations**. Retail is 1,633 bytes; the candidate still has **914 raw
  differences** and **fails** the older strict 1,629-byte frontier ratchet.

Target-backed negative boss experiments (restored): moving the collision
null-region exit to an explicit end label generated the **same 849/862,
539-difference** function; the target instead has the circular hit dispatcher
at `+0x8E` and a long null-region branch to the tail. Reversing the network
queue-sync success/wait blocks yielded **1,644/1,633 and 917 differences**;
introducing a separate natural network wait status yielded **1,651/1,633 and
988 differences**, or **1,639/1,633 and 984 differences** when combined with
success-first. A direct native queue-record assignment at the end of
`InsertPredictedFrame` shortened that exact owner from 191 to 185 bytes,
so its original separately ordered field stores were preserved. Twelve
natural replay-menu register-lifetime alternatives preserved the 29-difference
frontier or regressed. No rejected variant was retained or used to weaken a
frontier.

Current canonical ledger: **1,026 exact authored functions / 278,578 bytes**,
11 source-present nonexact authors. Important next work remains the collision
hit/reward CFG, network EDI receiver and IAT clock lifetime, Player charge
side/input EBX-EDI allocation and near-exact Enemy/Replay leaves. Native
Windows build and runtime proof remain separate open gates.

## 2026-11-19 latest checkpoint: typed Title, network and shake owners

Earlier local code checkpoint `43a5f322` extended the commits below. That
pass made **four natural C++ source commits**, retaining all existing
exact units without claiming a new exact owner:

- `bddf9707`: removed all three redundant Title replay-pool access helpers,
  replacing **25 uses** with direct member arrays `replays[]`, `replayPaths[]`
  and `replayAllocations[]`. The 1,387-byte replay-menu update stays exact.
  Nine compiler-private labels in `title-screen-start-menu` changed names;
  each was independently checked to resolve to the unchanged native target
  address. `TitleScreen.cpp` scoped replay passes **41 exact functions /
  18,972 physical bytes / 953 relocation fields**.
- `9c7789b7`: replaced the redundant network service `union` for two names
  of the same return DWORD with a single `int returnValue`. The complete
  candidate remains **1,650 bytes and 106 unchanged relocations**; retail is
  1,633 bytes and the current 914 raw differences still **fail** its strict
  nonexact frontier. The 0x14-byte authentic stack layout is retained.
- `aceff639`: removed the Effect VM indexing macro entirely; `InitializeEffect`
  and `OnUpdate` now use direct `effect->vms[index]` members. The
  `EffectManager.cpp` scoped replay passes **15 exact functions / 2,530 bytes /
  100 relocation fields**. The 51-byte AddedCallback remains at 4 residuals.
- `43a5f322`: used the real signed `contextIndex` to index GameManager sides
  and Supervisor viewports in the exact 316-byte `ScreenEffect::CalcShake`.
  No bounds gate or cross-RNG receiver cache was introduced. Nine renamed
  local COFF labels in `screen-effect-register-chain` were verified against
  their same-section target addresses. `ScreenEffect.cpp` scoped replay passes
  **18 exact functions / 2,562 bytes / 164 relocation fields**; the distinct
  `CalcShakeEnvelope` remains nonexact at 336/336 with 26 differences.

The authoritative authored ledger still contains **1,026 exact functions**.
These commits restore original data ownership and natural C++ access, not
additional byte-exact owners. Collision/charge/network boss CFG and register
residuals, and small register-allocation leaf residuals, remain active. No
full cold replay, ASM, dummy padding, target patching or frontier rebaseline
was used.

## 2026-11-19 current checkpoint: typed Effect/Title bosses and collision reward

Earlier source checkpoint `b0dcea11` (local commit, not independent remote
acceptance) included five natural-C++ changes:

- `916c714b` replaces five single-float `memcpy` calls in the native 87-byte
  `EffectUpdateFromVm` with typed Effect float members. Complete code remains
  **87/87** exact; this does not add a new match-ledger owner.
- `a1e1fa5a` replaces hand-scaled ANM VM pointer words with genuine
  `effect->vms[index]` access and typed pointer increment. Exact
  `InitializeEffect` (407 bytes), `OnUpdate` (475), `OnDraw` (183) and
  `OnDrawShared` (146) retain their complete original bytes and relocations.
- `43d6d30c` removes the integer-reinterpret template helper in the Effect
  initialize path; repeated real `g_EffectTemplates[unsigned effectId]` reads
  preserve its original native 0x18-byte stride and all callback order.
  The complete selected-source Effect replay passes **15 exact units,
  2,530 physical bytes and 100 relocations**.
- `0e589575` restores playable-side arithmetic and the naturally expressed
  two-way reward difficulty in the **862-byte collision boss**. Valid Player
  side indices are 0/1; the machine code remains the same **849-byte**
  candidate with **539 differing positions**. The **five exact same-source
  units / 1,220 bytes / 32 relocations** pass. The old strict collision
  frontier still says **REGRESSED**; the shared `hit:` CFG and target branch
  destination remain unresolved, and no frontier ratchet was altered.
- `b0dcea11` restores the real Title `replays[]`, `replayPaths[]` and
  `replayAllocations[]` arrays instead of converting addresses through
  integers. The **1,387-byte `OnUpdateReplayMenu` remains exact** with its
  76 relocation fields; all **41 exact Title units / 18,972 bytes /
  953 relocations** replay. Nine `title-screen-start-menu` private label
  names were resolved to unchanged same-section target destinations before
  updating `config/match-units.toml`.

Negative target/compiler probes were restored: in the 1,210-byte
`UpdateChargeAttack`, 36 combinations of 32-bit side width, direct input
record access, and volatile charge-mode lifetime did not improve the 183
residuals; using plain full-width side without reproducing EBX/EDI lifetimes
regressed. In 51-byte `EffectManager::AddedCallback`, typed receiver / branch
variants kept 4 or worsened to 7 register differences. In 336-byte
`CalcShakeEnvelope`, the reference source's rise/hold/fall expression produced
319 bytes / 201 differences, whereas the retained source remains 336/336
with 26 register differences. All exploratory rewrites were reverted.

Ledger totals remain **1,026 exact authored functions / 278,578 authored
bytes**. There are 11 source-present nonexact authors; full native build/link
and runtime proof remain open. The next useful target is the collision
shared hit-dispatch layout at `+0x8E`, plus charge side/input EDI-EBX
lifetimes, network receiver reloads, and the 2-byte Enemy OnUpdate residual.
Do not claim a new exact function from source-only refactoring.

## 2026-11-19: native replay tables and current boss/leaf handoff

The following five local checkpoints were completed with focused, pinned
VC7.1 source/owner checks, not a project-wide cold replay. The original TH09
v1.50a target and **1,026 authored-exact** ledger entries remain unchanged.

- `e26918b5`: in the **1,323-byte exact BeginRecordingStage**, replaced all
  twelve manually scaled integer pointer slots with real `frameStart[row][stage]`
  and `fpsStart[stage]` entries. Independent reads across free calls remain.
  The entire owner and 82 relocations are exact; affected `ReplayInput.cpp`
  replay passed **12 exact functions / 4,093 bytes / 232 relocations**.
- `a6a5ce83`: in the **3,900-byte Enemy OnUpdate boss**, linked the native
  `EnemyView` directly into its draw head instead of an unnecessary object
  reference. The candidate preserves **3,900/3,900 bytes, 2 unlike positions**.
  Four changed internal COFF label names were checked against actual
  same-section destinations. Two-profile `EclManager.cpp` replay passed
  **84 exact owners / 45,420 bytes / 1,987 relocations**.
- `2927ea8a`: original byte-stream reading through `fpsCursor[1]` and
  real bounded frame/FPS array indices restored in replay playback and release.
  Their **299-byte** and **318-byte** owners and relocations remain exact.
- `1befbc2a`: removed a redundant packed-DWORD alias and restored the
  ordinary eight-byte network input structure inside the real overlapping
  frame scratch. Complete compiled candidate and all **106 COFF fields**
  are unchanged; network service remains **1,650/1,633 with 914 raw differences**
  and fails its older strict frontier.
- `d627aada`: removed the replay-menu name-table wrapper; use the actual
  Difficulty, Mode and Character pointer arrays. Whole menu remains
  **1,280/1,280 with 29 differences** and all **57 relocations** intact.

Additional bounded negative experiments, all reverted: Cirno-Labs TH09's
`CheckHitAreas` confirms the circle-success `hit:` fallthrough and backward
rectangle jumps, but its direct `for`-scan adaptation lost native EBX/ESI/EDI
lifetimes (810/862 bytes and about 729 differences). Relocating the shared
label inside the original scan remained around 849/862, 540-541 differences,
not better than the retained **849/862, 539-difference** collision candidate.
`EffectManager::AddedCallback` typed-callback trials still gave a 51-byte
owner with four or seven unlike register bytes. The previous source and
strict frontiers were preserved; no padding, ASM or register forcing was used.

The independent rotation leaf also replays exact at 62 bytes with both
original trig symbols. The currently maintained EffectManager source's
**15 exact owners / 2,530 bytes / 100 relocations** and the four exact
PlayerShortLeaves owners (**51 bytes**) were checked. All product build and
runtime milestones remain open. Continue with target-backed full-body CFG and
register-lifetime work on collision/charge/network/Enemy rather than rewriting
small locals solely to shift one opcode.

## 2026-11-19 current checkpoint: natural boss and replay ownership

Source checkpoints through `136dc400` are locally committed. These are
**local Git checkpoints**, not independent evidence of a remote push. Exact
credit remains **1,026 authored functions / 278,578 bytes**. This pass makes
natural C++ ownership and type corrections in four additional functions;
none was promoted to exact and no full cold replay was performed.

- `6b349119`: `TitleScreenView::OnUpdateResult` reads real
  `PlayStatsRecordView::characterUnlocked[1/2]` arrays, preserving the
  wrapped native cursor traversal. Its complete 484/485-byte nonexact frontier
  remains at 167 differing offsets. All **41 exact same-source units** pass
  over **18,972 bytes / 953 relocations**. Nine compiler-private COFF labels
  were verified against their true owner-section destinations before updating
  the maintained manifest.
- `6746af16`: `SaveReplay` walks its true header and compressed-payload bytes
  through `u8*`/`const u8*` rather than an integer address cast. Its complete
  1,238-byte owner still differs at only two positions. The **12 exact Replay
  siblings / 4,093 bytes / 232 relocations** pass. `BeginPlaybackStage` is
  still **540/540 with nine raw register differences**, but fails the old
  strict 13-offset frontier because the mismatches have moved.
- `e331b598`: network prefill uses the actual signed 32-bit frame counter
  beginning at -9998. The complete **1,650-byte network service code and all
  106 COFF relocation records are identical** before/after the source change.
  `UpdateNetworkService` remains nonexact (914 raw differences, size 1,650 vs
  target 1,633), still failing the recorded 1,629-byte strict baseline.
- `136dc400`: `UpdateChargeAttack` binds the actual per-side
  `ReplayInputState` as a C++ reference. Its **1,210 code bytes and all 70
  relocation records** are unchanged; the 183-register-byte frontier remains
  open. Changing the side index to full `int` without recovering the receiver
  lifetime worsens the full owner to 1,209 bytes / 717 differences.

The preceding checkpoints (`baeb73c6`, `9b33689b`, `9b4277ce`) restored
**existing** exact `RotatePlayerCollisionVector` (62 bytes, both real `sinf`
/ `cosf` relocations), the genuine collision-boss `0x34` local frame, and a
fast check against contradictory exact/nonexact ledgers. In the collision
boss the first 0x24 bytes of the prologue/header now match the target, but
its branch destination and later hit/rectangle blocks do not: complete
**849/862 bytes, 539 raw differences**. Both collision and network strict
baselines stay unchanged; never rebaseline only to conceal their regressions.

Focus next on the target's circle-hit dispatcher at collision `+0x8E`, the
side/input EDI-EBX lifetime in charge, global-receiver reloads in network,
and the near-exact replay save / enemy manager register residuals. Use only
`replay-scoped-exact.py --source` for affected exact cohorts and
`check-frontier.py` for the selected nonexact owners. Do not run a full
project-wide cold replay without a genuinely broad impact.

## 2026-11-19 collision rotation exact-source repair

`RotatePlayerCollisionVector @ 0x0042AF40` had **already** been entered into
`config/matches.csv` as a canonical exact 62-byte owner, but recent source
rewrites replaced the original `sinf`/`cosf` call identities with two 24-byte
custom CRT wrappers and broke that reproducible claim. This session restores
real C/fastcall `sinf(float)` and `cosf(float)` declarations in
`src/PlayerCollisionRotate.cpp`; without a math-header intrinsic expansion,
VC7.1 generates the **exact 62 bytes and both original REL32 call symbols**
`@sinf@4 -> 0x00401070` / `@cosf@4 -> 0x00401060`. The natural C++ Y-product
intermediate preserves the retail x87 stack order. The existing canonical
`player-collision-rotate` unit and selected-source replay pass; the stale
66-byte diagnostic frontier is removed. This **repairs** earlier exact credit,
not a newly exact authored function or a reason to change ledger totals.

## 2026-11-19 live continuation

The earlier source checkpoint before the rotation/collision repairs was
`dac78c38`. The working branch is ahead of `origin/main`; these are **local**
checkpoints, not verified remote pushes. The authored exact ledger remains
1,026 functions (278,578 bytes); no nonexact owner was promoted in this pass.

- `94c8961d`: recovered an already-dirty natural array-element address in
  `EnemyManagerCore.inl`, and validated ten shifted compiler-private COFF
  labels by their actual same-section target offsets. The selected two-profile
  `EclManager.cpp` replay passes all **84 exact units / 45,420 physical bytes**;
  the 3,900-byte `EnemyManager::OnUpdate` still has its two register differences.
- `cb8579e9`: removed a redundant volatile reference to the private collision
  stack overlay. `CheckBulletCollision` is **849/862 bytes, 493 raw differences**
  (496 previously), and the **five exact same-source units / 1,220 bytes** pass.
  It **still fails the older strict frontier ratchet** (842-byte baseline and
  newly differing offsets). Do not `--update` it merely to accept a lower raw
  count; resolve the geometry/dispatcher CFG and stack lifetime naturally.
- `dac78c38`: replaced integer-address type punning of the movement focus
  effect ID table with a genuine unsigned indexed array access. The complete
  1,900-byte `UpdateMovementAndOptions` emitted **identical code bytes** and
  still has four residuals. Its 66 COFF relocation fields retain the same
  offsets, kinds, addends and physical targets; 18 names are only renumbered
  owner-local `$L...` labels. `PlayerMovement.cpp` has no canonical exact
  siblings, so use its focused frontier check rather than selected-source
  exact replay.

The immediately preceding local commits (`f5818f9e`, `e269c4ec`, `74f11366`,
`1e6e82b5`) recovered typed network input storage, shared collision cancel
handling, a typed volatile replay runtime pointer and native replay payload
byte indexing. Their affected exact cohorts were checked locally; the network,
collision and replay playback owners remain nonexact. No whole-product cold
replay or native build/runtime claim was made.

Target-backed recent negative probes: initial `SupervisorNetworkState *`
sharing can improve a register at the beginning of the network service but
would retain a stale receiver across session callbacks; reject it. Nine
`EffectManager::AddedCallback` two-argument inline-publisher variants all
reproduced the same four register differences or regressed. Keep the existing
natural helper until a target-backed lifetime model improves the complete
51-byte owner. Further C++ ordering probes for the rotation leaf and replay
save copy also failed to move their x87/register residuals.

## First five minutes

```bash
git status --short --branch
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/ci.py
```

Recover dirty/untracked work before editing. Do not assume a clean worktree or
silently discard another agent's changes. For source work, inspect the exact
owner and shared consumers, change natural C/C++, then run only the affected
complete frontier/exact cohort. Do not run a full cold replay for routine work.
Commit coherent advances as `gpt-web: <English summary>`; a commit is a local
checkpoint, not proof of exactness.

## Target and phase

- **Only target:** original Japanese TH09 v1.50a, ignored
  `resources/th09.exe`, 685,056 bytes, SHA-256
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`,
  MD5 `cf634df46e05552e104fa97a971aaac0`, PE32 i386, image base
  `0x00400000`, entry `0x0047D45F`.
- **Current ledger:** 2,248 function candidates; boundary/origin review
  pending 0; 35 reviewed but origin-unresolved; 1,037 confirmed authored
  functions / 291,931 authored bytes; 1,026 exact / 278,578 exact bytes;
  11 source-present nonexact owners / 13,353 bytes. Exact share of reviewed
  authored bytes is 95.4260%. These are authored-byte statistics, not a
  whole-image exactness percentage. The complete candidate inventory remains
  unproven.
- **Phase:** exact reconstruction. `config/build.toml` is still a skeleton;
  whole Windows i386 build/link and runtime validation are open. Semantic
  dual-Oracle work and portability have not started.

Regenerate status from the ledgers with
`python3 -B scripts/report-reconstruction-status.py`; do not hand-edit the
counts in `PROGRESS.md`.

## Current nonexact owner queue

These are the eleven authored source-present nonexact owners in
`config/frontiers.toml`. Counts below are relocation-masked differing byte
positions, not differing instructions. The frontier file is authoritative
for exact residual offsets, profiles, sizes and target hashes.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| `Supervisor::UpdateNetworkService` (`SupervisorNetwork.cpp`) | 0x00430AA0 | 1,629 / 1,633 bytes; 966 residual positions | Large host/network state-machine CFG, call order, stack homes and receiver lifetimes |
| `CheckBulletCollision` (`PlayerCollisionHelpers.cpp`) | 0x0041DFF0 | 842 / 862 bytes; 705 residual positions | Three nested retry levels, geometry/hit dispatch, cursor lifetime and native block order |
| `UpdateChargeAttack` (`PlayerChargeAttackUpdate.cpp`) | 0x0041F810 | 1,210 / 1,210 bytes; 183 residual positions | Native 32-bit side-index vs current 16-bit local; EDI/EBX input-pointer lifetime |
| `OnUpdateResult` (`TitleScreen.cpp`) | 0x004266B5 | 484 / 485 bytes; 167 residual positions | Result-state branches and selected-character load/call/store lifetime |
| `BeginPlaybackStage` (`ReplayInput.cpp`) | 0x00420840 | 540 / 540 bytes; 13 residual positions | Replay byte store, value load and side-0 runtime-pointer read order |
| `CalcShakeEnvelope` (`ScreenEffect.cpp`) | 0x004227A0 | 336 / 336 bytes; 26 residual positions | Receiver/timer register assignment and X87 value lifetimes |
| `DrawReplayMenu` (`TitleScreenReplayMenuDraw.cpp`) | 0x004234F6 | 1,280 / 1,280 bytes; 29 residual positions | Replay-record/menu-selection register and stack-home allocation |
| `UpdateMovementAndOptions` (`PlayerMovement.cpp`) | 0x0041C170 | 1,900 / 1,900 bytes; 4 residual positions | Config-pointer / live side-index load order at `+0x127..+0x12A` |
| `AddedCallback` (`EffectManager.cpp`) | 0x0040D1C0 | 51 / 51 bytes; 4 residual positions | `g_AnmManager` EDI load vs callback receiver ESI setup |
| `SaveReplay` (`ReplayInput.cpp`) | 0x00420C60 | 1,238 / 1,238 bytes; 2 residual positions | Header-copy source/destination register at `+0xB7/+0xBB` |
| `OnUpdate` (`EclManager.cpp`) | 0x00410730 | 3,900 / 3,900 bytes; 2 residual positions | Draw-group index uses ECX instead of retail EAX; residual bytes `+0x19C/+0x1A3` |

**Separate unclassified frontier:** `fsincos` in `EtamaControllerDraw.cpp`,
target `0x00412780`, candidate 36 / target 33 bytes, 25 differing positions.
It is not one of the eleven authored nonexact functions. Its origin remains
unclassified. Keep the maintained natural `cosf`/`sinf` C++; do not restore
inline assembly or an old exact claim.

## Latest source facts to preserve

- **Supervisor network PopFrame scratch:** commits `1c4cb7e7` and
  `bd4b8b07` recover the eight-byte native output window as
  `NetworkPopFrameScratch`. The prefill at `0x00430E66`/`0x00430E77` writes
  two seeds at frame offsets 0/2 and prediction at 4; the final frame at
  `0x0043108B`/`0x004310AC` reverses this to prediction at 0 and seeds at 4/6.
  All three typed union views have compile-time offset assertions. Commit
  `719acb53` also recovers the final frame's two consecutive 16-bit input
  results as a typed `NetworkFrameInputPair` with offsets 0/2 and size 4;
  the seed/prediction output pointers remain independent scalars pending
  VC7.1 lifetime recovery. Commit `32e1524f` additionally makes the
  target-backed connection phases 0/1/2/3, frame phases 0/1/2, and message
  kinds 0/2 explicit C++ enums; original identifier spellings are unknown.
  Commit `b79c9cf2` also names target-observed network timing thresholds
  (100-ms host poll, 4000-ms jitter, 4000/6000-ms retry bases, 20/60-second
  frame deadlines), 60-Hz frame conversion and -9998/+9998 prefill origin.
  Commits `8eb542ff`, `95006098`, `7394f9a5` and `67071267` separate
  frame-completion from wait/reset statuses, prioritize synchronized frames
  over prediction fallback, recover counted prefill with DWORD wrap, and
  restore the join-retry and packet-wait branches as natural positive C++
  conditions. Full-function VC7.1 comparison still holds the original
  1,629/1,633-byte, 966-difference frontier.
  This follows the independently observed TH09 state transitions (the
  adjacent GPL-3.0 App::UpdateNetPlay source remains a hypothesis).
  These are not new retry or safety policies. Six further C++ seed/prediction
  sharing experiments contracted the stack frame from 0x14 to 0x10 and
  introduced a new mismatch at +0x05, so the distinct scratch phases remain
  maintained pending better lifetime evidence.
  More than 18 source-local ordering/scope experiments and eight ordinary
  global-receiver early-read alternatives produced the same frontier, while
  a positive synchronized-frame body before the wait branch moved physical
  code blocks and introduced 51 new differences. Preserve the current
  target-backed call order, not a partial prologue or register resemblance.
  The complete frontier remains 1,629/1,633 bytes, 966 residual positions;
  a shared function-scope output-object probe changed the local-frame size
  without yielding a ratchetable improvement. Common epilogue and global
  receiver-reload experiments also regressed; do not promote them on partial
  instruction similarity.
- **Replay capture/save native semantics:** commit `fb66d164` restores
  native 32-bit wrapping frame-counter increments in `RecordInputAndFps`
  without signed-overflow assumptions. Commit `9ad8f335` uses a complete
  typed `ReplayFrameDataStartView` aggregate assignment in `SaveReplay`,
  naturally emitting the retail eight-DWORD REP MOVSD. Commit `19e3e09f`
  recovers the native little-endian USER chunk magic as a typed C++ character
  constant (the original tag is still 0x52455355), instead of treating a
  wire-format signature as an opaque integer. A focused replay of
  `ReplayInput.cpp` still proves 12 exact units over 4,093 physical bytes
  and 232 relocation fields. The nonexact `SaveReplay` and
  `BeginPlaybackStage` remain 1,238/1,238 with 2 residual positions and
  540/540 with 13, respectively. Neither commit claims an additional exact
  function.
- **Title result unlock inputs:** commit `6259d68b` recovers the
  ten-key D,D,D,S,S,S,S,Q,Reset,Reset sequence from the actual controller
  bit masks (0x2000/0x0400/0x0200/0x4000), plus bomb/menu cancel mask
  0x000A. Scoped C preprocessor names preserve VC7.1's private COFF labels;
  a seemingly equivalent local C++ enum changed an unrelated exact unit.
  `TitleScreen.cpp` passes 41 exact same-source units over 18,972 physical
  bytes (two compiler profiles), while OnUpdateResult still measures
  484/485 bytes with 167 residual positions. Do not claim a new exact owner.
- **Screen shake axis choices:** commit `1c7a403d` documents the shared
  three-value RNG choice for each of the independent X/Y shake axes in both
  `CalcShake` and `CalcShakeEnvelope`: zero, positive and negative amplitude.
  Scoped C names compile to the original branch literals and preserve all
  18 exact same-source units across three profiles / 2,562 physical bytes.
  `CalcShakeEnvelope` remains 336/336 bytes with 26 residual positions.
  Earlier local-enum probes broke exact COFF private-label evidence even
  though the named nonexact frontier was unchanged; keep the scoped macro
  representation unless a fully verified natural alternative supersedes it.
- **Effect ANM publication:** commit `2ebbf0cd` keeps the natural
  `ResolveEffectAnm(AnmManager *, AnmLoaded *&)` helper. It resolves the
  resident ANM, optionally preloads it, then publishes the final handle to
  `effectAnm`. This preserves the delayed member-write boundary; a direct
  member-reference store before the preload was an incorrect earlier probe.
  The complete 51-byte callback still has four residual positions; this is
  not an exactness claim.
- **Enemy draw heads:** commit `09660746` removed the one-use synthetic
  draw-head helper. Both append sites use typed pointer arithmetic on the
  real four-entry `drawGroupHeads2AC410` array. Commit `bcdc5040` restores
  the actual EnemyView object at the common draw-chain append as a C++
  reference, after a direct pointer-elimination experiment changed a private
  `CompareOperands` relocation label. The maintained reference passes the
  scoped 84 exact units: 45,420 bytes and 1,987 relocation fields.
  The 3,900-byte Enemy manager boss remains at two residual bytes; do not
  introduce assembly, byte offsets or padding to force EAX allocation.
- **Player collision cursor and retry loop:** commits `4a6e16cb` and
  `8db39334` assert the true 514-entry `activeB100` extent. Commits
  `8613e62f` and `1bc46c9b` replace opaque collision/reward stack prefixes
  with typed 52-byte vector/cursor overlays (reward vectors at offsets 24/36,
  cursor pointer at 48) and spell out the original null-region return.
  Commits `aa841a4e` and `aa59e47b` restore a distinct inner type/delay
  retry and make the shared hit/cancel dispatch explicit in source.
  Commit `f8bd4309` further recovers the circle, rotated-rectangle and AABB
  checks as one natural three-way C++ dispatch feeding that common hit path.
  Commit `a4290c3c` snapshots the hit-region kind once before testing the
  two cancellation categories, matching the target's single kind load.
  Commit `8f8adf56` places the circle's common `hit:` label inside the
  circle branch and moves the cancellation reward outside the active-region
  scan, aligning natural C++ control flow with the observed target and the
  GPL-3.0 adjacent-source hypothesis. Commit `60c2fa0d` also names the three
  TH09-attested region kinds (direct hit 1, bullet cancellation 2 and 3)
  without implying that original C++ enum spellings were recovered.
  VC7.1 still emits the maintained
  842/862-byte, 705-difference body.
  Its five exact same-source functions pass a focused replay over 1,220
  physical bytes and 32 relocation fields. Target evidence carries
  Player in EDI, Bullet in EBX, the cursor in ESI, and region in ECX; the
  cursor home precedes the first region dereference. A complete three-nested-
  loop rewrite and a reordered rectangle branch fixed some bytes but also
  introduced new differences. The best angle-branch ordering probe reached
  845/862 bytes and 477 residual positions, but introduced 28 new positions
  (notably `+0xC9..+0xCD`, `+0x11A..+0x11C`, `+0x1B6..+0x1BF`); the original
  target executes rotated-rectangle geometry before AABB geometry and joins
  circle hits at the same dispatcher. Do not promote the diagnostic probe or
  widen the frontier based solely on its lower total mismatch count.
- **Player movement and charge input:** commit `cf3ed195` names the native
  four cardinal and four combined diagonal movement masks plus focus input.
  Commit `ba55d762` names the two observed charge configuration states:
  charge from held shot input (0) or held focus input (1). Commits `34315d01`
  and `f6d85416` recover the actual 100/200/300/400 charge tiers, resource
  costs, and native frame deadlines. Commit `897d149c` names the distinct
  shoot bit `0x0001` and focus bit `0x0004`, grounded in TH09's keyboard and
  controller sampler; they must not be confused with configuration mode 0/1.
  Both full-function frontiers remain unchanged: movement 1,900/1,900 with
  4 residual bytes and charge 1,210/1,210 with 183. The current `short side`
  and volatile charge-mode snapshot are not claimed as final semantics.
  Widening/removing the side local, direct indexing and pointer/reference
  alternatives regressed charge, while twelve additional ordinary movement
  config-mode accessors compiled to the same four residual bytes. Preserve
  the complete owner measurements; do not ratchet on a local instruction win.

- **New bounded compiler probes (2026-11-18):** ten `PlayerMovement`
  source-only inline focus-mode accessor variants all reproduced the same
  complete 1,900-byte function and four residual positions. Reversing the
  `ResolveEffectAnm` argument order and changing pointer/reference forms
  did not solve the 51-byte Effect callback's four-register-byte residual.
  A type-safe rewrite of the native Title increment macro and an explicit
  endpoint cast in `ScreenEffect` preserved their named nonexact frontiers,
  but changed compiler-private COFF labels in same-source exact units; both
  experiments were restored, with no ledger or private-label rewriting.
  A local `ShakeDirection` enum in the nonexact envelope also changed the
  original same-source `RegisterChain` private COFF relocation labels;
  the enum experiment was restored despite matching the 336-byte envelope.
  Four additional clean C++ amplitude-helper extractions emitted ~175-byte
  `CalcShakeEnvelope` candidates instead of the target's 336 bytes; preserve
  the native timer/x87 calculation inside its owner. Do not bypass private
  label or full function verification to promote those probes.
  Keep the scoped exact cohorts mandatory for future shared-source edits.

Historical negative results are clues, not proof that every natural source
form is exhausted. Never use ASM, inert padding, fake returns, register hints,
compiler roulette or a widened frontier to manufacture progress.

## Next investigations

1. **Large boss — `Supervisor::UpdateNetworkService`:** continue from the
   original `0x00430AA0` body and current `src/SupervisorNetwork.cpp`. This is
   a reconstruction-facing member name: IDA identifies a `__thiscall` receiver
   and the sole caller is `Supervisor::OnUpdate`; the original spelling is
   unknown. Commits
   `8eb542ff`, `95006098`, `7394f9a5` and `67071267` already separate frame
   result from wait/reset status, restore synchronized-frame-first dispatch,
   recover the counted prefill loop, and make join retry / packet wait gates
   explicit. Do not repeat those branch rewrites: they all preserve the
   1,629/1,633-byte, 966-position frontier. Focus next on the remaining
   `PopFrame` output homes, receiver reloads, stack lifetime and call order.
2. **Large boss — `CheckBulletCollision`:** the circle `hit:` entry now sits
   inside the circle branch, and cancellation rewards execute after leaving
   the region scan (`8f8adf56`). Continue with the distinct nested filter /
   geometry / hit retry levels, cursor advances and rotated/AABB block order.
   Keep all exact Player siblings protected; never promote a saved diagnostic
   merely because its raw mismatch count is lower.
3. **Small, bounded blockers:** `EnemyManagerView::OnUpdate` (two draw-index
   bytes), `SaveReplay` (two header-copy bytes), then Effect callback,
   ScreenEffect, Replay playback and Title owners. Test complete owners, not
   just isolated expressions.
4. Resolve the 35 origin-unresolved candidates separately. Whole-product
   translation-unit partition, link order, data/resources and runtime closure
   remain later work; exact functions do not imply a runnable executable.

## Focused commands

```bash
# Recheck all eleven authored nonexact owners against their pinned profiles:
python3 -B scripts/check-frontier.py \
  supervisor-network-service player-bullet-collision player-charge-attack \
  title-update-result replay-begin-playback screen-shake-envelope \
  title-draw-replay-menu player-movement effect-added-callback \
  replay-save enemy-on-update --show
python3 -B scripts/check-frontier.py etama-fsincos --show

# Inspect exact consumers before changing shared source/header inputs:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EnemyManagerCore.inl --dry-run
python3 -B scripts/replay-scoped-exact.py --source src/PlayerCollisionHelpers.cpp
python3 -B scripts/replay-scoped-exact.py --source src/EffectManager.cpp

# Target, tracking, docs and fast repository checks:
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
python3 -B scripts/ci.py

git diff --check
```

A nonexact frontier pass means only that its recorded monotone baseline was
not worsened; it is not exact credit. Exact units require complete physical
extent and relocation proof with the pinned VC7.1 profile. For shared
`.hpp`/`.inl` edits, inspect the actual affected source dependents before
choosing scoped replay. Do not run a full cold replay for routine iteration.

## Private inputs and storage hygiene

Preserve these inputs; they are not disposable build output:

- `resources/th09.exe` (the original target; never patch, delete or commit it).
- `.tools/` (shared Ghidra/JDK/MSVC selectors and this game's Wine prefix).
- `_reference/` (reference checkout).
- `.analysis/ghidra-direct/243bffea8dcbe99a/` (saved Ghidra project) and
  `.analysis/factory-runtime/` (runtime lock state).
- `.analysis/c2-heap.bin`, `FUN_107844a4.decomp.txt`, the four
  `gdb-c2-*.cmd` recipes, and exactly these three natural-C++ diagnostics:
  `SupervisorNetwork.natural-ebx-reload.cpp`,
  `PlayerCollisionHelpers.natural-preheader-842.cpp`, and
  `PlayerCollisionHelpers.nested-three-loop-849.cpp`.
- `c2-analysis/c2.dll`, `c2-analysis/project/`,
  `c2-analysis/allocator-project/`, `c2-analysis/scripts/` and the retained
  top-level C2 register/allocator/codegen reports. These support the still
  unresolved Enemy/Supervisor compiler-allocation questions.

The top-level `.analysis/` compiler scratch corpus accumulated hundreds of
source variants and generated OBJ/PDB pairs after the previous cleanup. Those
variants are reproducible from maintained source and the notes above; they are
not prerequisites for exactness. The cleanup for this handoff removes those
root-level trials, generated objects/PDBs, old rename/export snapshots,
`.analysis/factory-native-ida/` call-result captures, and `build/`. It retains
only the three C++ diagnostics listed above, the four GDB recipes, the heap /
allocator decompilation inputs, the active Ghidra project, and runtime lock
state. The two C2 project roots, C2 DLL/scripts and retained top-level reports
remain intact. Recreate only the selected compiler carrier when needed.
Missing `build/frontier/*.obj` or `build/matching/*.obj` after cleanup is
expected. Do not run `git clean -fdx`: it would also target protected ignored
inputs. Never delete `.tools/`, `resources/th09.exe`, `_reference/`, either C2
project, or the saved Ghidra project while cleaning caches.

## 2026-11-18 GPT-web continuation checkpoint

Current HEAD is the two-commit network leaf cleanup from this workstream.
The playback-stage source now has a four-byte `ReplayPlaybackScratch` union
for the disjoint stage-index / side-0-runtime-pointer lifetime, while its
complete owner remains at 13 residual positions. The near-exact SaveReplay
and EnemyManager update owners still have two register-scheduling residuals
each. The live Supervisor network service trial now uses a compile-time-checked
20-byte aggregate: frame/input union at offset 0, PopFrame overlay at offset 8,
and result/status union at offset 16. Its emitted frame matches the target
(`sub esp, 0x14`, result at EBP-4, PopFrame homes at EBP-0x0C), and the focused
residual count falls from 966 to 914. It remains nonexact (1650 candidate bytes
versus 1633 target bytes); do not rebaseline. The collision nested-loop
experiment remains rejected (612 total differences but 93 new positions).
Continue with the two large owners and near-exact leaves; keep the natural C++
stack overlay and commit after focused verification.


## 2026-11-18 GPT-web follow-up checkpoint

The near-exact leaves were probed again after the network stack overlay commit.
No speculative register alias was retained: PlayerMovement still has one
four-byte config/side load-order residual; AddedCallback has two register
setup differences; CalcShakeEnvelope has the ESI/EDI timer-receiver swap;
SaveReplay has the two destination-base register differences; and the replay
menu declaration reorder was a large regression. The detailed variants and
measured results are in `docs/KNOWLEDGE_BASE.md`. All rejected source trials
were restored. The best source state remains HEAD `107ff7b1` with the verified
20-byte network stack overlay (914 differing positions) and playback-stage
lifetime union (13 residual positions). Continue with the network boss CFG /
register lifetimes and the large collision/charge owners; run only the
frontiers involved in each source change.


## 2026-11-18 GPT-web collision checkpoint

The first nested-scan checkpoint (`428f4079`) measured 638 differing
positions. The follow-up nested C++ scan measures 848/862 bytes with 606 differing
positions (705 at the original baseline). It uses a shared direct-hit/cancel
tail, reloads the rotated-box region from the live slot after the helper, and
uses the target's double-precision zero constants for both x87 radius/angle
comparisons. This source change is checkpointed
separately from the earlier network stack work. It is still nonexact and must
not be used to update the frontier baseline. Next compare the null-return
branch target, type/delay filter branch polarity, x87 unordered flags, and the
bullet/slot register lifetimes.


## 2026-11-18 GPT-web collision continuation

The current collision scan checkpoint is the volatile hit-region reload
variant. It preserves the nested scan preheader and the target ESI cursor
prologue, sends invalid/delayed/geometric misses through a shared C++ advance,
and separately advances after a cancel-kind hit whose bullet is not canceled.
The hit path reloads `*slot` through a volatile pointer local; the rotated
path reloads it after the vector-rotation call. The focused frontier is
849/862 bytes with 533 differing positions (baseline 842/862, 705 positions).
Do not rebaseline: the circle hit still branches to a late common block rather
than retail's fallthrough hit block, and the angle/axis and reward tails remain
nonexact. Continue with source-level control-flow/lifetime experiments only.


## 2026-11-18 GPT-web collision rotation leaf

A new focused frontier tracks `RotatePlayerCollisionVector` at `0x42AF40`
(target 62 bytes). The source now calls explicit natural-C++ float-argument,
double-return wrappers around CRT `sin` and `cos`; this avoids VC7.1 folding
`sinf`/`cosf` into `fsin`/`fcos`. The focused object has the retail save/prologue,
argument home reuse, trig call ordering, and x87 expression family. It is
currently 66 bytes with 24 differing offsets (`1D`, `1F`, `2A-2B`, `2D-36`,
`38-41`). Continue with x87 operand evaluation order and removal of the
leftover `fstp st(0)` through source-level lifetime changes. No baseline for
existing frontiers was altered.

### Collision frontier ratchet note

`CheckBulletCollision` currently measures 849/862 bytes and 533 differing
positions, but `check-frontier.py` reports `REGRESSED` because new byte offsets
differ and the candidate grew by seven bytes from the original 842-byte
baseline. Do not rebaseline; the count reduction alone is not a monotonic
improvement. Keep resolving those new offsets in natural C++.

## 2026-11-18 GPT-web: boss residual register research

- `EnemyManagerView::OnUpdate`: 3900/3900 bytes, only two differing bytes at
  `+0x19A/+0x1A1`; target reuses EAX for the draw-group byte and scaled pointer,
  candidate uses ECX for the index. Several typed-pointer, array, split-increment,
  local-type and inline-accessor variants did not change codegen. Keep natural
  pointer arithmetic; no assembly/padding.
- `UpdateChargeAttack`: retain the 1210-byte / 183-difference baseline. Widening
  the local side index or changing input pointer to reference caused a 648-diff
  regression and was reverted. Target side/input registers are EDI/EBX versus
  candidate EBX/EDI; continue source lifetime experiments.
- `RotatePlayerCollisionVector`: retain the 66-byte double-wrapper checkpoint;
  `long double` variants did not change the 24-position residual. Remaining
  work is x87 operand order and final `fstp st(0)` cleanup.

## 2026-11-18 GPT-web: replay and movement residuals

- `BeginPlaybackStage` checkpoint `30febe15` uses a volatile read of the
  side-0 runtime pointer to stop hoisting it before the replay-byte read. Raw
  differences fall 13 -> 9, but the monotonic checker reports `REGRESSED` due
  to seven new offsets (`156`, `162`, `168`, `16C`, `172`, `178`, `17A`). The
  target uses EDX for replay-data and ESI for runtime; the candidate reverses
  those registers. Keep the recorded frontier baseline unchanged.
- `UpdateMovementAndOptions` is still 1900 bytes / 4 differences. The only
  residual is the global configuration pointer vs side-index load order at
  `+0x126`; direct C++ expression/reference variants did not alter codegen,
  volatile variants regressed the large body and were reverted.
- `EnemyManagerView::OnUpdate` is still 3900 bytes / 2 differences. Multiple
  natural C++ local, union, volatile-byte and scope variants did not change the
  EAX/ECX draw-group allocation. No assembly, padding or baseline update.

## 2026-11-18 GPT-web: collision hit-layout trial

- `CheckBulletCollision` was restructured as natural C++ with a shared
  `process_hit` block before the rectangle tests. The candidate's raw mismatch
  count dropped from 705 to 531, but function size grew 842 -> 849 and
  `check-frontier.py` reports `REGRESSED` due to newly differing positions.
  Retail falls through from the circle hit test into its dispatcher at `+0x8E`;
  this build emits a jump to `+0x17C`. The rotated-rectangle/AABB branches still
  target the common block. Keep the experiment as a source checkpoint, but do
  not alter the recorded baseline or claim exactness.
- A second outer-`for`/inner-retry rewrite, a duplicated hit-handler rewrite,
  and an explicit 32-bit draw-group pointer rewrite did not improve the
  measured object and were discarded. `EnemyManager::OnUpdate` remains 2
  residual bytes; `UpdateChargeAttack` remains 183 residual bytes.

## 2026-11-18 GPT-web: collision rotation leaf

- `RotatePlayerCollisionVector` uses a volatile reference to the native point
  record and retains ordinary double-precision C++ arithmetic. Candidate size
  is now 62/62 bytes and raw differences fall 24 -> 10. The checker still
  reports `REGRESSED` because x87 operand order differs at `0x21-0x22` and
  `0x24-0x25`: retail duplicates cosine before the point-X memory multiply;
  candidate loads point X/Y first. Do not update the frontier baseline.

## 2026-11-18 GPT-web: boss/charge register experiments

- `EnemyManager::OnUpdate`: moving `drawHead` to the active path, explicit
  DWORD byte addressing for draw-group indexing, and `register` on the head
  pointer all retained the exact same two EAX/ECX residual bytes. Restored the
  baseline source.
- `UpdateChargeAttack`: full-width signed/unsigned side locals, direct use of
  the member side index, reference binding for `ReplayInputState`, and pointer
  declaration-order permutations did not improve the 183-byte residual; the
  full-width forms regressed hundreds of bytes. Restored the baseline source.

## 2026-11-18 GPT-web: second boss/charge allocation pass

- `EnemyManager::OnUpdate`: unsigned local index, split base/index pointer
  arithmetic, array subscript, and `register` byte/integer locals all retained
  the same two EAX/ECX residual instructions. Directly rereading the no-sprite
  flag instead of using the existing native flag reference caused 645
  differences; reverted.
- `UpdateChargeAttack`: full-width side locals plus pointer, reference,
  const-pointer, array-base arithmetic, declaration-order and volatile field
  variants consistently moved the candidate to EBX=side / EDI=input, opposite
  retail's EDI=side / EBX=input, and regressed 183 -> 717 differences. Reverted
  all trials; the original 183-difference source remains.

## 2026-11-18 GPT-web: collision and leaf follow-up

- `CheckBulletCollision`: positive-hit predicate, switch dispatch and direct
  cancel-to-reward goto measured 580-585 differences (845-853 bytes), all
  `REGRESSED`; dispatcher still lands at `+0x17C` rather than target `+0x8E`.
  The committed 531-difference shared-hit source remains the best candidate.
- `RotatePlayerCollisionVector`: direct cosine-times-coordinate accumulator
  initialization retained 10 differences / 62 bytes; no new gain.
- `CalcShakeEnvelope`: register callback alias retained 26 differences;
  register timer pointer caused 242 differences. Reverted.
- `UpdateMovementAndOptions`: volatile configuration pointer alone caused
  1109 differences. `EffectManager::AddedCallback`: volatile global manager
  read changed 4 differences to 7. Both reverted.

## 2026-11-18 GPT-web: structured CheckBulletCollision follow-up

- Target-attested IDA decompilation confirms the scan order and reward gate:
  valid kinds 1/2/3 with delay <= 0; circle when radius != 0; rotated rectangle
  when radius == 0 and angle != 0; otherwise AABB. Shared hit kind 1 increments
  and returns 2; cancel kinds 2/3 reward only for a non-null bullet whose word
  at +0x10C0 is zero.
- The current natural-C++ structured geometry/shared-hit candidate is
  849/862 bytes and 496 raw differences (prior best 531, original baseline
  705). It remains `REGRESSED` because size grew 842 -> 849 and new positions
  differ. Keep the original baseline. The dispatcher still lays out at
  candidate +0x17C instead of retail +0x8E; stack frame remains 0x38 vs 0x34.
  Commit is an exploratory source checkpoint, not an exactness claim.

## 2026-11-18 GPT-web: rejected register-allocation probes

The follow-up pass restored all non-improving source variants. Enemy update
append-site references regressed 3,005/1,816 bytes; side-category scope did
not change its 2-byte residual. Full-width/direct/reference side-index forms
in `UpdateChargeAttack` regressed to 717-873 differences, so retain 183. The
`SaveReplay` reference, register-cursor, and memcpy forms kept the same two
`+0xB6/+0xB9` differences; volatile destination cursor regressed to 856. The
callback helper/parameter/register variants retained 4 differences or
regressed; retain the 51-byte source. Screen shake receiver/timer aliases
retained 26 differences or swapped ESI/EDI. Direct rotation expressions and
volatile cosine were worse than the committed 62-byte/10-difference leaf.
No baseline was changed.

## 2026-11-18 GPT-web: title-result and network-boss follow-up

- `TitleScreenView::OnUpdateResult` now takes the selected byte through the
  native array element pointer. The cursor walk guarantees an index in
  `[0, FullCharacterSlots)`. Focused result remains 484/485 bytes / 167 recorded
  differences (`OK`); this improves natural C++ expression only, not binary
  matching. Commit: `2f78701f`.
- `Supervisor::UpdateNetworkService` current scratch-overlay source remains
  `REGRESSED` at 1650/1633 bytes / 914 differences. Splitting the 20-byte
  overlay into separate locals reached 1629 bytes but had 967 differences and
  a new `+0x05` mismatch because the frame shrank to 0x10 rather than 0x14.
  Shared timer, receiver aliases, and volatile scratch probes did not improve
  it. Restore all trials; keep the baseline unchanged.
- Enemy-update combined predicate and switch experiments were discarded; the
  two-byte residual remains. `EtamaController::fsincos` remains the 36-byte
  natural C++ implementation against the 33-byte x87 target; no ASM.

## 2026-11-18 GPT-web: rotation, Etama and boss register-lifetime probes

- New source checkpoints: `ec7d138f` simplifies `RotatePlayerCollisionVector`
  to direct C++ vector equations and removes the volatile point alias; focused
  frontier is back at 66/62 bytes, 24 recorded differences (`OK`).
  `c2431adc` orders the Etama sine/cosine computations and output stores like
  the native helper; frontier remains 36/33 bytes, 25 recorded differences
  (`OK`). Neither commit claims a byte-match improvement.
- Collision boss: the historical `aa59e47b` source compiles against the current
  target at its recorded 842/705 baseline. Current 849/496 candidate is still
  `REGRESSED` due new offsets; all this session's loop experiments were
  restored.
- Charge boss: full-width side index, reordered pointer construction,
  volatile side, and aggregate context probes did not solve the EBX/EDI swap;
  source restored to the 183-difference baseline.
- Enemy boss: removing the side-category local and materializing the draw-group
  index did not solve the two-byte EAX/ECX residual; source restored.
- Network boss: separate stack locals with a volatile return word and native
  declaration order reproduced the retail 0x14 prologue and `[ebp-4]` zero
  store, but the body still has 926 differences and a receiver-register
  mismatch. All network probes restored. The nine-offset replay playback
  EDX/ESI swap also remains unresolved and its probes were restored.
- Worktree should be clean at `c2431adc`; continue from this checkpoint and
  keep the `gpt-web: ...` commit subject format.

## 2026-11-18 GPT-web: collision layout and replay/charge register probes

- New checkpoint `4738abd3 gpt-web: map collision boss scratch and retry flow`
  records the full target `CheckBulletCollision` stack/CFG findings in the
  knowledge base. Do not treat the temporary 832-byte / 435-difference
  collision candidate as accepted: it introduced new offsets and was restored.
- Collision target frame: 0x34 local bytes, with a 48-byte geometry/reward
  overlay and a separate cursor home at `[ebp-4]`. Target registers: EBX bullet,
  ESI cursor, EDI owner. Target's two retry paths and shared hit dispatcher are
  described in `docs/KNOWLEDGE_BASE.md`.
- Remaining collision task: find a natural C++ source shape that keeps the
  circle check's x87 condition branch (`test ah,0x41` / `jne next`) and places
  the shared hit dispatcher at target `+0x8E`, before rectangle blocks. Current
  compiler layout moves the dispatcher after rectangle tests. No asm/padding.
- Replay playback: attempts to widen/promote the `value1E8` byte, reverse
  register-local declaration order, or force a volatile stack home did not
  resolve the target EDX-byte / ESI-runtime-pointer allocation. Restore
  `src/ReplayInput.cpp` to HEAD.
- Charge boss: side-index and input-pointer register hints/aggregate/volatile
  variants still fail to put the side in EDI and input pointer in EBX. Restore
  `src/PlayerChargeAttackUpdate.cpp` to HEAD.
- Enemy boss: byte register local and explicit DWORD address arithmetic do
  not change the EAX/ECX two-instruction residual. Restore
  `src/EnemyManagerCore.inl` to HEAD.
- Title result and collision rotation: direct character-array/volatile-local
  and distinct/mutable cosine variants were non-improving; restore both files
  to HEAD.
- All experimental source files are restored. Only docs changed in this
  session. Documentation validation passed. Continue from `4738abd3` and keep
  `gpt-web: ...` commit subjects; run only touched frontiers, never full cold
  replay unless specifically needed.

- Additional collision-loop experiments after `4738abd3`: the 48+4 stack
  overlay and three nested loops compiled at 862 bytes / 640 differences;
  a nonvolatile position-pointer variant reached 848 / 569. Both remain
  `REGRESSED` because the compiler schedules the shared hit dispatcher after
  the rectangle blocks instead of at target `+0x8E`. Separate cancellation
  retry and volatile region-pointer variants were worse. Source restored.

## 2026-11-18 GPT-web: collision x87/dispatcher layout follow-up

- Re-examined the live dirty collision source and ran only the focused
  `player-bullet-collision` frontier plus target/candidate disassembly. The
  48-byte geometry/reward union and separate cursor home are structurally
  correct, but candidate stack lifetimes can shift the frame to 0x30/0x38.
- A scoped volatile radius reference used in both the radius-zero test and
  circle product reproduced target x87 load order; direct member and volatile
  cast variants did not. Splitting type and positive-delay rejections can
  restore the target `JG` sense and common cursor advance.
- Main unresolved CFG mismatch: target lays out circle test, hit dispatcher,
  then rectangle geometry at +0x5D/+0x8E/+0xBB. Compiler continues to place
  rectangle geometry before the dispatcher. The best candidate in this pass
  is still `REGRESSED` (848/862, 607 raw differences); no source candidate
  retained. Canonical collision source restored. Continue with natural C++
  basic-block shaping, not asm or padding.

## 2026-11-18 GPT-web: boss register allocation follow-up

- `EnemyManager::OnUpdate`: exact two-byte residual is register choice only.
  At `+0x19A/+0x1A1`, retail uses EAX for the byte draw-group index and then
  for the computed head pointer; source emits ECX for the index and EAX for
  the pointer. Removing/recomputing the head local or making either pointer
  volatile caused 696-3005 differences; volatile byte reference did not move
  the residual. Canonical source restored at 3900/3900, 2 differences.
- `UpdateChargeAttack`: target loads `sideIndex` as a full DWORD into EDI and
  derives the input pointer in EBX using stride 0x8E. Current source narrows
  the side local to short, putting sign-extended side in EBX and input pointer
  in EDI. Full-width and volatile side locals, register pointer, reversed
  declaration order, and split full/short index experiments regressed to
  648-895 differences. Canonical source restored at 1210/1210, 183 differences.
- Continue from this handoff; do not change the recorded frontier baselines.

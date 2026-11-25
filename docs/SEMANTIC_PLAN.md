# TH09 semantic and readability plan

Branch: `semantic/readability`
Policy: [SEMANTIC_RECONSTRUCTION.md](SEMANTIC_RECONSTRUCTION.md)
Baseline: `1,036 / 1,037` confirmed authored exact functions (`99.3714%`),
working position-independent Windows i386 trial links (`trial.exe`,
`trial-reversed.exe`, `trial-playable.exe`), verified Wine replays and Windows
playthrough.

## Baseline debt snapshot

Measured before Batch 1 via `scripts/report-semantic-debt.py` and workspace
inspection:

| Metric | Baseline | Current (`3.2`) | Notes |
| --- | ---: | ---: | --- |
| Source files in `src/` | 550 | 553 | Added 5 protocol headers, retired `PlayerState4OpsView.hpp` |
| Numeric `switch` cases | 717 | 297 | `-420` (`EclOperands`, `BackgroundStageScript`, `FrontMessageUpdate`, `EnemySchedule`, `SupervisorUpdate`, `TitleCalc/Draw` closed & guarded) |
| Numeric `PlaySound*` / `PlayMenuSound` literals | 147 (89 + 58) | 0 | Closed & CI-guarded (`SoundIdx` 0..53) |
| Numeric `SpawnEffect*` literals | 10 | 0 | Closed & CI-guarded (`Th09EffectId`) |
| `*View` struct/class definitions | 362 | 299 | `-63` net (`-66` duplicate views retired; Batch 3.3 consolidates shared header declarations) |
| Link `/ALTERNATENAME` aliases | 139 | 37 | `-102` (`-73.4%`) aliases retired across Batches 3.1–3.2 (`verify-trial-layouts.py` green) |
| `reinterpret_cast` sites | 1,930 | 1,929 | Batch 3.3/3.4 targets canonical header & cast cleanup |
| Anonymous `unknown`/`pad`/`raw` hex names | 913 | 848 | `-65` removed alongside duplicate view structs |
| `ProductData.asm` generated labels | 628 | 612 | `-16` duplicate view-typed global data labels retired |

## Phased execution plan

Each batch is committed cleanly with the `gemini-4-neon: ...` prefix after
passing its required exact/CI/layout checks.

### Phase 1: Infrastructure and planning

- [x] **1.1 Plan & policy:** add `docs/SEMANTIC_PLAN.md` and
  `docs/SEMANTIC_RECONSTRUCTION.md`.
- [x] **1.2 Semantic debt scanner:** add `scripts/report-semantic-debt.py`
  (read-only work selector modeled on TH08's scanner, tuned for TH09 views,
  numeric protocols, stable IDs, offset suffixes, and anonymous fields).

### Phase 2: Protocol and stable-ID closure (highest readability gain, zero ABI risk)

Close each observed numeric domain with explicit TH09-evidenced enums and verify
every affected TU with `scripts/replay-scoped-exact.py`:

- [x] **2.1 ECL operand selectors (`EclOperands.cpp`, 269 numeric cases):**
  define `Th09EclOperandId` in `src/EclOperandIds.hpp` for all `0x2710+`
  integer/float variable selectors and replace raw hex cases in
  `ResolveInt`, `ResolveIntLValue`, `ResolveFloatLValue`, and
  `EnemyView::ResolveFloat`.
- [x] **2.2 Background stage-script & camera protocols (`BackgroundStageScript.cpp`, 51 numeric cases):**
  define `BackgroundStageOpcode`, `BackgroundInterpolationMode`, and
  `BackgroundCameraMotionMode` in `src/Background.hpp` and replace numeric
  `switch` cases and raw mode checks in `BackgroundStageScript.cpp`,
  `BackgroundPendingStageLabel.cpp`, and `Background.cpp`.
- [x] **2.3 FrontMessage dialogue script protocol (`FrontMessageUpdate.cpp`, 24 numeric cases):**
  define `FrontMessageOpcode` in `src/FrontMessageTableView.hpp` from
  TH09-observed handlers and replace raw opcode numbers in
  `FrontMessageUpdate.cpp`.
- [x] **2.4 Stable audio IDs (`SoundIds.hpp` / `SoundPlayer.hpp`, 147 numeric calls):**
  inventoried TH09 `g_SfxList[39]` (`0x004A2130`) and `g_SoundBufferIdxVol[54]`
  (`0x004A1F80`), defined `SoundIdx` (`0..53`) in `src/SoundIds.hpp`, and
  replaced all numeric `PlaySoundByIdx`, `PlaySoundPositionedByIdx`, and
  `PlayMenuSound` literals across 23 files (`207/207` exact units replayed).
- [x] **2.5 Enemy timeline, Etama draw, Supervisor, Title & Effect protocols:**
  named `EnemyTimelineOpcode` (`EnemyScheduleRuntimeAdvance.cpp`),
  `SupervisorState` (`SupervisorStates.hpp`), `TitleScreenId` /
  `TitleMatchSubmode` (`TitleScreenStates.hpp`), `BulletRenderState`
  (`EtamaControllerDraw.cpp`), and `Th09EffectId` (`EffectIds.hpp`, 10/10 fixed
  `SpawnEffect*` calls across 8 files).
- [x] **2.6 Semantic protocol CI guard:** added
  `scripts/check-semantic-protocols.py` and `tests/test_check_semantic_protocols.py`
  and wired the guard into `scripts/ci.py`.

### Phase 3: Core `*View` consolidation, `/ALTERNATENAME` reduction & field cleanup

Consolidate redundant layout views onto canonical owners in bounded batches,
preserving exact VC7.1 bytes and verifying PI layouts after symbol changes:

- [x] **3.1 ExAttack duplicate view & alias reduction (first wave):**
  replaced `ExAttack*InterruptView`, `ExAttackType*AllocateView`,
  `ExAttackType*SideLookupView`, `ExAttackType*SideView`,
  `ExAttackType*AnmOwner`, `ExAttackType*GameManagerView`,
  `ExAttackType*PlayerView`, and `ExAttackType7BulletDescriptorStorage` across
  15 files with canonical `AnmVm`, `ExAttackRecordRuntimeView`,
  `AsciiGameManagerView`, `PlayerLifecycleView`, and
  `BulletSpawnDescriptorCtorView`; retired **46 `*View` structs** and
  **32 `/ALTERNATENAME` aliases** (`139 -> 107`), verified with scoped exact
  replay + full normal/reversed/playable PI links (`verify-trial-layouts.py`).
- [x] **3.1b Shared descriptor, Supervisor, Replay, Title & Player view consolidation:**
  consolidated all remaining 8 `BulletSpawnDescriptor` views (`ExAttackUpdateType{8Type9,11Type12,15,20,21,25,26}`, `BulletAdvanceTransformProgram`), `SupervisorStartup`/`SupervisorDeletedCallback`/`SupervisorDrawLoading`/`SupervisorCalculateFps`/`SupervisorDrawMain`/`SupervisorVersion`, `GameWindowRender`, `TitleScreenFactoryView` (`AnmVm` + `ReplayDataView`), `ReplayStageDispatch`, `EnemyManagerRelease`, `PlayerLifecycle`, and `PlayerAddedState` onto canonical `AnmManager`, `AnmLoaded`, `AnmVm`, `AsciiManager`, `AsciiGameManagerView`, `PbgArchive`, `MidiOutput`, `SoundPlayer`, `ReplayManagerView`, `RngRuntimeView`, `TextHelper`, and `SupervisorThreadStartView`; retired **20 more `*View` definitions**, **41 `/ALTERNATENAME` aliases** (`107 -> 66`), and **10 duplicate `ProductData.asm` labels** (`624 -> 614`), verified with scoped exact replay (`128/128` exact units) + full normal/reversed/playable PI links (`verify-trial-layouts.py`).
- [x] **3.2 `SupervisorAudio`, `Player`/`Enemy` reward, `Effect` fixed-slot, `Title`, `Replay` & math helper alias closure:**
  canonicalized `SupervisorAudio` (`MidiOutput`, `MidiPlaybackPathView`, `AsciiGameManagerView`, `Supervisor`), `PlayerDeath`/`PlayerRuntime` (`ConfigureState3Effect13/14/15`, `CreateCircleType1/4`; retired `PlayerState4OpsView.hpp`), `PlayerShotSpawnCallbacks`/`6` (`EffectManager::SpawnEffectInFixedSlot`, `EffectFixedSlotManagerView::GetFixedSlotEffect`), `EnemyManagerUpdate`/`EnemyManagerRelease`/`EclPostRuntime` (`PlayerOwnerStateView`, `PlayerLifecycleView`, `PlayerPositionCallbackView`, `EnemyEclManagerReleaseView`), `TitleScreenRegister`/`TitleScreen` (`TitleScreenView::AddedCallback`/`DeletedCallback`, `RngRuntimeView::GetRandomU32`, `InputView::IsPressedScrolling`), `ReplayInput` (`SupervisorVersionView::CheckVersion`), `AnmManagerDraw3D`/`EnemyManagerDraw` (`AnmProjectionAbs`), and `PlayerDamage` (`RotatePlayerCollisionVector`); retired **29 more `/ALTERNATENAME` aliases** (`66 -> 37`, `-73.4%` from baseline) and **2 more `ProductData.asm` labels** (`614 -> 612`), verified with scoped exact replay + `check-frontier.py player-movement` + full normal/reversed/playable PI links (`verify-trial-layouts.py`).
- [ ] **3.3 `AnmManager` and `FrontSide` / `FrontMessage` views:**
  consolidate the 12 `AnmManager*View` overlays and `Front*View` types.
- [ ] **3.4 `EnemyManager` / `Player` / `ExAttack` view reduction:**
  consolidate repeated `EnemyEcl*View`, `Player*View`, and high-duplication
  `ExAttack*View` headers where types are structurally identical overlays of
  the same runtime object; retire unnecessary `reinterpret_cast` chains.

### Phase 4: Maintainer navigation documentation

Modeled on TH08's `readability/maintainer-navigation` pass:

- [x] **4.1 `docs/PROJECT_GUIDE.md`:** engine frame loop (Mermaid diagram),
  dual-side PvP architecture (P1/P2 + shared EffectManager), ECL/ANM/Replay
  overview, and glossary.
- [x] **4.2 `docs/SOURCE_MAP.md`:** map the 383 `.cpp` files and `.inl`
  lexical fragments by subsystem, compiler profile, and verification command.
- [x] **4.3 `docs/SEMANTIC_INDEX.md` & resource/storage contracts:**
  subsystem index plus TH09 ANM resource namespaces and Effect/ExAttack storage
  contracts.
- [x] **4.4 Streamline `README.md`:** link the new navigation and semantic docs
  cleanly and verify with `scripts/validate-docs.py`.

### Phase 5: Optional follow-on (native data ownership closure)

- [ ] **5.1 Global data migration:** replace selected `ProductData.asm` tables
  and globals with natural C++ definitions where exact comparison and PI link
  checks remain green, and advance `config/build.toml` beyond `skeleton`.

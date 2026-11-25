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

| Metric | Baseline | Current (`3.4b`) | Notes |
| --- | ---: | ---: | --- |
| Source files in `src/` | 550 | 558 | Added 7 protocol headers + 3 shared view headers (`AsciiInputView`, `SupervisorDrawMainViews`, `PlayerPositionCallbackView`), retired `PlayerState4OpsView.hpp` |
| Numeric `switch` cases | 717 | 110 | `-607` (`-84.7%`); 20 core protocol & state-machine files closed & CI-guarded |
| Numeric `PlaySound*` / `PlayMenuSound` literals | 147 (89 + 58) | 0 | Closed & CI-guarded (`SoundIdx` 0..53) |
| Numeric `SpawnEffect*` literals | 10 | 0 | Closed & CI-guarded (`Th09EffectId`) |
| `*View` struct/class definitions | 362 | 256 | `-106` (`-29.3%`) duplicate view definitions eliminated across Batches 3.1–3.4b |
| Link `/ALTERNATENAME` aliases | 139 | 37 | `-102` (`-73.4%`) aliases retired across Batches 3.1–3.2 (`verify-trial-layouts.py` green) |
| `reinterpret_cast` sites | 1,930 | 1,929 | Preserved exact VC7.1 bytes across all 1,036 exact functions |
| Anonymous `unknown`/`pad`/`raw` hex names | 913 | 807 | `-106` removed alongside duplicate view structs |
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
- [x] **3.3 Secondary menu, difficulty, direction & movement state-machine closure:**
  extracted TH09-local menu help strings (`g_TitleOptionsHelpText @ 0x4A1DDC`, `g_TitleKeyConfigHelpText @ 0x4A1E04`, `g_TitleHelpText @ 0x4A1E3C`) from `resources/th09.exe`; added `src/GameDifficulty.hpp` (`GameDifficulty`), `src/PlayerDirection.hpp` (`PlayerMoveDirection`), and extended `src/TitleScreenStates.hpp` (`TitleStartMenuItem`, `TitleOptionsScreenState`/`MenuItem`, `TitleKeyConfigScreenState`/`MenuItem`); named `EnemyPostEclMovementMode`/`Easing`/`HorizontalFacing` (`EclPostRuntime.inl`), `PlayerRewardTailType` (`PlayerRuntime.cpp`), and `AsciiRetryMenuState` (`AsciiManagerMenu.cpp`); eliminated **111 numeric `switch` cases** (`297 -> 186`, `-74.1%` from baseline) across 9 files and extended `scripts/check-semantic-protocols.py` to guard all 9 files in CI (`139/139` exact units replayed + `player-movement` frontier OK).
- [x] **3.4a `TitleScreen`, `AsciiMenuState5`, `PlayerOwnerStateReward`, `SampleNetworkInput` & `AnmManagerRuntime` state-machine closure:**
  extended `TitleScreenStates.hpp` (`TitleRegistrationContext`, `TitleSubscreenState`, `TitleReplayMenuState`, `TitleReplaySaveState`) and named `AsciiGameOverMenuState`, `InputDeviceMode`, `AnmDrawMode`, and `GameDifficulty` dispatches across all 5 files, eliminating **76 more numeric `switch` cases** (`186 -> 110`, `-84.7%` from baseline) and guarding all 5 files in `scripts/check-semantic-protocols.py` (`51/51` exact units replayed).
- [x] **3.4b Shared `*View` overlay deduplication:**
  deduplicated 43 copy-pasted `*View` definitions (and 9 duplicate `ExAttackInitAnmOwner` structs) across 33 files onto single canonical headers (`ExAttackRecordRuntimeView.hpp` for 9 `ExAttackInitType*.cpp` files, `AsciiInputView.hpp`, `SupervisorDrawMainViews.hpp`, `PlayerPositionCallbackView.hpp`, `EffectManager.hpp`, `EnemyEclRawFileView.hpp`, `Midi.hpp`, `AnmManager.hpp`, `SoundPlayer.hpp`, `Supervisor.hpp`, and `AnmCameraProjectionViews.hpp` for `BackgroundRender.cpp`); reduced `*View` definitions `299 -> 256` (`-106` from baseline) and anonymous identifiers `848 -> 807` (`-106` from baseline) with zero symbol changes (`34` compiler jobs, `154/154` exact units, `69,214` bytes replayed 100% exact).

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

### Phase 5: Native C++ global data ownership closure (matching TH08)

Baseline verified at `aa659141`: full PI builds (`trial.exe`, `trial-reversed.exe`,
`trial-playable.exe`, `385/385` carriers), `verify-trial-layouts.py` green
(`1,085/1,085` code units moved, `611/612` data labels moved), and 300-frame
Wine replay (`demorpy0.rpy`) bit-for-bit identical between retail and
`trial-playable.exe`.

Current `build/product/ProductData.asm` inventory: `479` blocks / `612` labels
(`0x4002d` bytes):
- `CONST` (`.rdata`): `364` blocks (`8,397` B) — `128` named blocks + `236` anonymous `th09_rdata_*` strings referenced by `_DATA` pointer tables
- `_DATA` (`.data`): `64` blocks (`13,372` B) — `44` single-label tables + `6` multi-label tables + `14` `g_OpponentLists` sub-arrays
- `_BSS` (`.bss`): `51` blocks (`240,420` B) — `35` standalone globals + `11` small 2–3 label pairs + `5` multi-label owner objects (`g_GameManager`, `g_Supervisor`, `g_ReplayInputStates`, `g_PlayStatsRecord`, `g_GameWindow`)

Goal: migrate all `479` blocks / `612` labels into natural C++ definitions so
`ProductData.asm` reaches `0` blocks / `0` labels while keeping `1,036 / 1,037`
exact codegen, `player-movement` frontier unchanged, and full PI + Wine replay
verification green.

- [x] **5.1 Single-label `_DATA` tables & string-pointer tables:**
  migrated 32 standalone `_DATA` tables (2,822 bytes, 153 string pointers) across
  18 owning `.cpp` files (`PlayerGameplayState`, `EndingAddedCallback`,
  `BulletManager`, `ExAttackAddedCallback`, `Background`, `GameManagerSelectOpponent`,
  `FrontSide`, `PlayerRuntime`, `TitleScreenRegister`, `TitleScreenResultDraw`,
  `TitleScreenMusicRoom`, `TitleScreenOptions`, `TitleScreenKeyConfig`, `PbgFile`,
  `TextHelper`, `SoundPlayerProcessQueues`, `SupervisorAudio`, `FrontMessageLoad`),
  retiring **172 blocks & 172 labels** from `ProductData.asm` (`479 -> 307` blocks,
  `612 -> 440` labels, `552 -> 399` data relocations); verified `176/176` exact
  units, bit-for-bit retail parity on all 32 tables + 153 strings across both
  `trial.exe` and `trial-reversed.exe`, `verify-trial-layouts.py`, and `ci.py`.
- [x] **5.2 Callback/template `_DATA` tables, multi-label `_DATA` tables & `g_OpponentLists`:**
  migrated all remaining 32 `_DATA` tables (`g_CardAttackCallbackRows`, `g_EffectTemplates`,
  `g_ExAttackTemplates`, all 7 `g_PlayerCallback*` / `g_PlayerShot*Callbacks` tables,
  `g_PlayerPatternConfigs` + `PlayerPatternConfig.hpp`, `g_PlayerResourcePaths[16][6]`,
  `g_OpponentEntries[17]`, `g_OpponentLists[14]` + all 14 static opponent arrays,
  `g_OpponentParameterTable[225]`, `g_BulletSpriteOffsetSmall[16]`,
  `FileSystem::g_DecryptParams` + `g_CryptSignature`, and `g_TitleFadeReleaseEffect`),
  retiring all 6 multi-label `_DATA` aliases and all 14 `th09_data_*` / 96 `th09_rdata_*`
  blocks; **`_DATA` in `ProductData.asm` is now `0` blocks (`0` data relocations)**
  (`307 -> 179` blocks, `440 -> 304` labels); verified `73/73` exact units,
  bit-for-bit retail parity across `trial.exe` and `trial-reversed.exe`,
  `verify-trial-layouts.py`, and `ci.py`.
- [ ] **5.3 Named `CONST` (`.rdata`) strings & lookup tables:**
  migrate all remaining `128` named `CONST` blocks (error/log/format/DirectInput/DirectPlay
  strings and `const` arrays) to natural C++ `const` definitions. Target: `CONST = 0` blocks.
- [ ] **5.4 Standalone `_BSS` globals & small 2–3 label `_BSS` pairs:**
  define all `35` standalone `_BSS` globals and unify the `11` small 2–3 label
  `_BSS` pairs onto single owning C++ definitions.
- [ ] **5.5 Multi-label `_BSS` owner objects (`g_GameWindow`, `g_PlayStatsRecord`, `g_ReplayInputStates`, `g_Supervisor`, `g_GameManager`):**
  retire all interior-offset global aliases in `src/`, define all `5` canonical
  global objects in natural C++, and bring `ProductData.asm` to `0` blocks / `0` labels.
- [ ] **5.6 Final closure & Wine end-to-end replay verification:**
  clean up remaining product link glue (`ProductStaticInit.cpp` / `config/build.toml` /
  docs), verify full normal/reversed/playable/base-relocatable PI links, Wine
  replay parity, and `scripts/ci.py --full`.

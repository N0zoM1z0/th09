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

| Metric | Baseline | Notes |
| --- | ---: | --- |
| Source files in `src/` | 550 | 383 `.cpp`, 128 `.hpp`, 38 `.inl`, 1 `.md` |
| Numeric `switch` cases | 717 | Across 53 files (`EclOperands.cpp` 269, `TitleScreen.cpp` 54, `BackgroundStageScript.cpp` 51, ...) |
| Numeric `PlaySound*` literals | 89 | `SoundPlayer.hpp` has no `SoundIdx` enum yet |
| Numeric `SpawnEffect*` literals | 10 | `EffectManager.hpp` has no `EffectId` enum yet |
| `*View` struct/class definitions | 362 | 56 `*View.hpp` files (`ExAttack*` alone has 108) |
| Link `/ALTERNATENAME` aliases | 139 | Recorded in `build/link-probe/aliases.json` |
| `reinterpret_cast` sites | 1,930 | Across 234 files (many view-to-view casts) |
| Offset-suffixed identifiers | ~26,785 | E.g. `config388`, `position1B88`, `activeContext2CE0` |
| Anonymous `unknown`/`pad`/`raw` hex names | 913 | 289 unique names across `src/` |
| Hardcoded `0x004xxxxx` literals | 77 | Across 34 files |
| `ProductData.asm` generated globals | 628 | `PUBLIC` symbols extracted from `th09.exe` |

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

- [ ] **3.1 Shared vector / primitive / timer / ANM VM views:**
  eliminate duplicate `AnmVm*View` / `Float3` / `BulletSpawnDescriptor*Storage`
  overlays and retire their `/ALTERNATENAME` aliases where exactness allows.
- [ ] **3.2 `GameManager` and `Supervisor` canonical layouts:**
  unify `AsciiGameManagerView` / `GameManagerSetupView` /
  `GameManagerRuntimeView` / `GameManagerResetView` and `Supervisor*View`
  headers onto canonical `GameManager` and `Supervisor` definitions; clean
  evidenced offset-suffixed fields.
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

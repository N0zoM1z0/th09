# TH09 source and build ownership map

TH09 currently contains 383 `.cpp` files, 128 `.hpp` headers, and 38 `.inl`
fragments under [src/](../src/), plus whole-image startup glue in
[product/](../product/). During exact reconstruction, functions were grouped
into compiler carriers that preserve VC7.1 optimization and inlining context;
the file layout therefore reflects verified emission carriers rather than
claimed original translation units.

For runtime architecture, see [PROJECT_GUIDE.md](PROJECT_GUIDE.md). For
protocols and declarations, see [SEMANTIC_INDEX.md](SEMANTIC_INDEX.md).

## Authoritative selectors

| Question | Authoritative file / tool |
| --- | --- |
| Which `(source, profile)` compiles an exact function? | [config/match-units.toml](../config/match-units.toml) |
| Which functions are accepted as byte-exact? | [config/matches.csv](../config/matches.csv) |
| Which source-present functions remain non-exact? | [config/frontiers.toml](../config/frontiers.toml) (`scripts/check-frontier.py`) |
| How are carriers selected and linked for PI / playable builds? | [scripts/trial-link.py](../scripts/trial-link.py) and [config/product-link.toml](../config/product-link.toml) |
| Where do unmigrated retail globals and static constructors come from? | [scripts/product-data.py](../scripts/product-data.py) and [product/ProductStaticInit.cpp](../product/ProductStaticInit.cpp) |

## Subsystem file families

| Subsystem | `.cpp` count | Primary entries | Notes |
| --- | ---: | --- | --- |
| Player & shots | 59 | [PlayerLifecycleView.hpp](../src/PlayerLifecycleView.hpp), [PlayerRuntime.cpp](../src/PlayerRuntime.cpp), [PlayerMovement.cpp](../src/PlayerMovement.cpp), [PlayerGameplayState.cpp](../src/PlayerGameplayState.cpp) | `PlayerMovement.cpp` hosts the single open non-exact frontier (`0x0041C170`, 4 masked byte diffs). |
| Supervisor & netplay | 51 | [Supervisor.hpp](../src/Supervisor.hpp), [SupervisorUpdate.cpp](../src/SupervisorUpdate.cpp), [SupervisorStartup.cpp](../src/SupervisorStartup.cpp), [SupervisorVersion.cpp](../src/SupervisorVersion.cpp) | `SupervisorVersion.cpp` is recompiled with `-DTH09_PLAYABLE` for playable links. |
| ExAttack | 43 | [ExAttackController.hpp](../src/ExAttackController.hpp), [ExAttackController.cpp](../src/ExAttackController.cpp), `src/ExAttackInitType*.cpp`, `src/ExAttackUpdateType*.cpp` | Types 7/8/9/11/12/14/19/21/22 have lexical `.inl` fragments included by `EclManager.cpp`. |
| ANM | 24 | [AnmManager.hpp](../src/AnmManager.hpp), [AnmManagerExecuteScript.cpp](../src/AnmManagerExecuteScript.cpp), [AnmManagerLoad.cpp](../src/AnmManagerLoad.cpp), [AnmManagerRender.cpp](../src/AnmManagerRender.cpp) | `TitleDrawCallback.cpp` has explicit COMDAT precedence over `AnmManagerRuntime.cpp` for `SpriteHasTexture`. |
| Front HUD & dialogue | 24 | [FrontSide.hpp](../src/FrontSide.hpp), [FrontSideDraw.cpp](../src/FrontSideDraw.cpp), [FrontMessageUpdate.cpp](../src/FrontMessageUpdate.cpp), [FrontCalcCallback.cpp](../src/FrontCalcCallback.cpp) | `FrontMessageUpdate.cpp` implements the 24-opcode dialogue interpreter. |
| Enemy & schedule | 23 | [EnemyManager.hpp](../src/EnemyManager.hpp), [EnemyManagerCore.inl](../src/EnemyManagerCore.inl), [EnemyManagerUpdate.cpp](../src/EnemyManagerUpdate.cpp), [EnemyScheduleRuntimeAdvance.cpp](../src/EnemyScheduleRuntimeAdvance.cpp) | `EnemyManagerCore.inl` is lexically included by `EclManager.cpp`. |
| GameManager & config | 21 | [GameManagerSetup.cpp](../src/GameManagerSetup.cpp), [GameManagerSideState.hpp](../src/GameManagerSideState.hpp), [GameConfiguration.cpp](../src/GameConfiguration.cpp), [GameErrorContext.cpp](../src/GameErrorContext.cpp) | Owns the two playable `0x38`-byte side records and gameplay setup thread. |
| AsciiManager & menus | 14 | [AsciiManager.hpp](../src/AsciiManager.hpp), [AsciiManagerMenu.cpp](../src/AsciiManagerMenu.cpp), [AsciiManagerScorePopup.cpp](../src/AsciiManagerScorePopup.cpp) | Pause/retry menus, boss markers, and score popups. |
| Effects | 13 | [EffectManager.hpp](../src/EffectManager.hpp), [EffectManager.cpp](../src/EffectManager.cpp), [EffectIds.hpp](../src/EffectIds.hpp) | Several ring/fan callback families are `.inl` fragments included by `EclManager.cpp`. |
| Title screen | 12 | [TitleScreenReplayView.hpp](../src/TitleScreenReplayView.hpp), [TitleScreen.cpp](../src/TitleScreen.cpp), [TitleCalcCallback.cpp](../src/TitleCalcCallback.cpp), [TitleDrawCallback.cpp](../src/TitleDrawCallback.cpp) | Includes Options, KeyConfig, MusicRoom, ReplayMenu/Save, and ResultDraw. |
| Bullets & Etama | 15 | [BulletManager.hpp](../src/BulletManager.hpp), [BulletManager.cpp](../src/BulletManager.cpp), [EtamaControllerDraw.cpp](../src/EtamaControllerDraw.cpp) | Bullet transform programs, laser updates, and 6-bucket rendering. |
| PBG archives & FileSystem | 10+ | [PbgArchive.hpp](../src/PbgArchive.hpp), [PbgArchiveLoad.cpp](../src/PbgArchiveLoad.cpp), [FileSystem.hpp](../src/FileSystem.hpp) | `product/PbgArchiveCtor.cpp` supplies the folded D3DX8-matching constructor glue. |
| Background | 7 | [Background.hpp](../src/Background.hpp), [Background.cpp](../src/Background.cpp), [BackgroundStageScript.cpp](../src/BackgroundStageScript.cpp), [BackgroundRender.cpp](../src/BackgroundRender.cpp) | Stage `.std` camera/fog script and 3D quad rendering. |
| Replay | 7 | [ReplayInput.cpp](../src/ReplayInput.cpp), [ReplayDataConstructors.cpp](../src/ReplayDataConstructors.cpp) | Input recording/playback, sync state, and `.rpy` serialization. |
| CardAttack | 6 | [CardAttack.cpp](../src/CardAttack.cpp), [CardAttackStart.cpp](../src/CardAttackStart.cpp) | Boss/spell card attack dispatch and presentation callbacks. |
| Chain, Score, Sound, Ending, MIDI, WinMain | 22 | [Chain.hpp](../src/Chain.hpp), [SoundPlayerProcessQueues.cpp](../src/SoundPlayerProcessQueues.cpp), [ScoreData.cpp](../src/ScoreData.cpp), [Ending.cpp](../src/Ending.cpp), [Midi.cpp](../src/Midi.cpp), [WinMain.cpp](../src/WinMain.cpp) | Core scheduler, DirectSound/MIDI audio, persistence, ending script, and entry point. |
| ECL core | 4 | [EclManager.cpp](../src/EclManager.cpp), [EclOperands.cpp](../src/EclOperands.cpp), [EclHelpers.cpp](../src/EclHelpers.cpp), [EclInitializeSubroutine.cpp](../src/EclInitializeSubroutine.cpp) | See below for `EclManager.cpp`'s lexical include protocol. |

## Reading `EclManager.cpp` and `.inl` fragments

[EclManager.cpp](../src/EclManager.cpp) is a large shared VC7.1 carrier:

- Inside `EclManager::RunEcl`, it lexically includes six handler fragments
  (`EclRunControl.inl`, `EclRunMovement.inl`, `EclRunRemote.inl`,
  `EclRunBullet.inl`, `EclRunState.inl`, `EclRunLate.inl`) so all 187 opcodes
  share one stack frame, local variables, and jump labels.
- At the bottom of `EclManager.cpp`, it also includes `EclPostRuntime.inl`,
  `EnemyManagerCore.inl`, and selected `ExAttack*` / `Effect*` / `Player*`
  `.inl` fragments whose exact register allocation and inlining depend on
  co-visibility in the same VC7.1 translation unit.

Do **not** split these `.inl` files into standalone `.cpp` files during
cleanup without verifying every affected exact unit.

## Validation recipes

```bash
# Single source file exact replay
python3 -B scripts/replay-scoped-exact.py --source src/EclOperands.cpp

# Shared header impact check before editing
python3 -B scripts/replay-scoped-exact.py --changed-file src/Background.hpp --dry-run

# Whole-image PI rebuild and verification (after symbol/alias/layout changes)
python3 -B scripts/trial-link.py build
python3 -B scripts/trial-link.py link
python3 -B scripts/trial-link.py link --reverse
python3 -B scripts/verify-trial-layouts.py

# Fast public CI (includes ledgers, links, progress, source purity)
python3 -B scripts/ci.py
```

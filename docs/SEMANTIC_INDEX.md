# TH09 semantic index

Choose a subsystem to jump directly to its canonical declarations, recovered
protocols, and implementation owners. For engine-level overview see
[PROJECT_GUIDE.md](PROJECT_GUIDE.md); for compiler/carrier ownership see
[SOURCE_MAP.md](SOURCE_MAP.md); for the active cleanup roadmap see
[SEMANTIC_PLAN.md](SEMANTIC_PLAN.md).

## Subsystems and recovered protocols

| Subsystem | Declarations / protocols | Implementation landmarks | Key distinction / contract |
| --- | --- | --- | --- |
| ECL opcodes & operands | [EclOpcodes.hpp](../src/EclOpcodes.hpp) (`Th09EclOpcode` 1..187), [EclOperandIds.hpp](../src/EclOperandIds.hpp) (`Th09EclOperandId` `0x2710`..`0x2775`), [EclEnemyPrimaryFlags.hpp](../src/EclEnemyPrimaryFlags.hpp) | [EclManager.cpp](../src/EclManager.cpp), [EclOperands.cpp](../src/EclOperands.cpp), `src/EclRun*.inl` | 20 opcode slots dispatch to the retail default tail (`UNHANDLED_*`); TH09 adds opcodes 185..187 and operand `0x2775` for dual-side PvP state. |
| Enemy & stage timeline | [EnemyManager.hpp](../src/EnemyManager.hpp), `EnemyTimelineOpcode` in [EnemyScheduleRuntimeAdvance.cpp](../src/EnemyScheduleRuntimeAdvance.cpp) | [EnemyManagerCore.inl](../src/EnemyManagerCore.inl), [EnemyManagerUpdate.cpp](../src/EnemyManagerUpdate.cpp), [EnemyScheduleRuntimeAdvance.cpp](../src/EnemyScheduleRuntimeAdvance.cpp) | Each playable side owns an independent `0x2AC450`-byte `EnemyManager` with remote slots (`remoteSlots2AC388`) and 4 timeline event slots (`timelineEventSlots2AC430`). |
| Background & camera | [Background.hpp](../src/Background.hpp) (`BackgroundStageOpcode` 0..34, `BackgroundInterpolationMode` 0..7, `BackgroundCameraMotionMode` 0..4) | [Background.cpp](../src/Background.cpp), [BackgroundStageScript.cpp](../src/BackgroundStageScript.cpp), [BackgroundPendingStageLabel.cpp](../src/BackgroundPendingStageLabel.cpp), [BackgroundRender.cpp](../src/BackgroundRender.cpp) | TH09 adds camera motion mode 4 (`ORBIT_SWAY_AND_ROLL`, 2048-frame cycle) beyond TH08's modes 0..3. |
| ANM scripts & rendering | [AnmManager.hpp](../src/AnmManager.hpp), [AnmVmLifecycle.hpp](../src/AnmVmLifecycle.hpp), [EtamaAnmResourceSlot.hpp](../src/EtamaAnmResourceSlot.hpp) | [AnmManagerExecuteScript.cpp](../src/AnmManagerExecuteScript.cpp), [AnmManagerLoad.cpp](../src/AnmManagerLoad.cpp), [AnmManagerRender.cpp](../src/AnmManagerRender.cpp) | Keep ANM file slots, resource-local script IDs, sprite IDs, and runtime `AnmVm` array indices separate. |
| FrontSide HUD & dialogue | [FrontSide.hpp](../src/FrontSide.hpp), [FrontMessageTableView.hpp](../src/FrontMessageTableView.hpp) (`FrontMessageOpcode`), [FrontMessageRuntimeView.hpp](../src/FrontMessageRuntimeView.hpp) | [FrontMessageUpdate.cpp](../src/FrontMessageUpdate.cpp), [FrontSideDraw.cpp](../src/FrontSideDraw.cpp), [FrontCalcCallback.cpp](../src/FrontCalcCallback.cpp) | Dialogue strings are XOR-obfuscated (`key=0x77`, `step+=0x10`); portrait pairs occupy 8 contiguous `AnmVm` slots (`messageVms14[8]`). |
| Audio (SFX & BGM) | [SoundPlayer.hpp](../src/SoundPlayer.hpp), [SoundIds.hpp](../src/SoundIds.hpp) (`SoundIdx` 0..53), [SoundManager.hpp](../src/SoundManager.hpp) | [SoundPlayerProcessQueues.cpp](../src/SoundPlayerProcessQueues.cpp), [SoundManager.cpp](../src/SoundManager.cpp) | 39 loaded `.wav` buffers map through `g_SoundBufferIdxVol[54]` with per-slot attenuation; TH09 indices 39..53 diverge from TH08. |
| Effects | [EffectManager.hpp](../src/EffectManager.hpp), [EffectIds.hpp](../src/EffectIds.hpp) (`Th09EffectId`) | [EffectManager.cpp](../src/EffectManager.cpp), `src/Effect*.cpp`, `src/Effect*.inl` | `Effect` (`0xD8` bytes) overlays variant payloads at `+0xA0` (`reward`, `cancel`, `playerReward`) and draws via 3 linked sentinel buckets. |
| Bullets & lasers | [BulletManager.hpp](../src/BulletManager.hpp) | [BulletManager.cpp](../src/BulletManager.cpp), [EtamaControllerDraw.cpp](../src/EtamaControllerDraw.cpp) | Bullets draw across 6 size-sorted buckets; `EtamaControllerDraw.cpp` defines `BulletRenderState` (spawn fast/normal/slow, despawn). |
| Players, shots & ExAttack | [PlayerLifecycleView.hpp](../src/PlayerLifecycleView.hpp), [ExAttackController.hpp](../src/ExAttackController.hpp) | [PlayerRuntime.cpp](../src/PlayerRuntime.cpp), [PlayerMovement.cpp](../src/PlayerMovement.cpp), [ExAttackController.cpp](../src/ExAttackController.cpp) | One `0x30F70`-byte player object per side; ExAttack dispatches 27 character/attack-type callbacks against the opposing side. |
| Supervisor & Title state machines | [Supervisor.hpp](../src/Supervisor.hpp), [SupervisorStates.hpp](../src/SupervisorStates.hpp) (`SupervisorState`), [TitleScreenReplayView.hpp](../src/TitleScreenReplayView.hpp), [TitleScreenStates.hpp](../src/TitleScreenStates.hpp) (`TitleScreenId`, `TitleMatchSubmode`) | [SupervisorUpdate.cpp](../src/SupervisorUpdate.cpp), [TitleCalcCallback.cpp](../src/TitleCalcCallback.cpp), [TitleDrawCallback.cpp](../src/TitleDrawCallback.cpp), [TitleScreen.cpp](../src/TitleScreen.cpp) | `Supervisor::OnUpdate` coordinates state transitions, netplay retries, and periodic version checks; Title dispatches 16 screens plus 5 match submodes. |
| Callback scheduler | [Chain.hpp](../src/Chain.hpp) (`ChainCallbackResult`) | [Chain.cpp](../src/Chain.cpp), [ChainRun.cpp](../src/ChainRun.cpp) | Separate priority-ordered calculation and drawing chains. |
| Replay & score persistence | [ReplayDataOriginalView.hpp](../src/ReplayDataOriginalView.hpp), [TitleScoreRecordView.hpp](../src/TitleScoreRecordView.hpp) | [ReplayInput.cpp](../src/ReplayInput.cpp), [ScoreData.cpp](../src/ScoreData.cpp) | Three `0x8E`-byte `ReplayInputState` records cover P1, P2, and shared UI/dialogue input (`index 2`). |

## Address and debt lookup

```bash
# Locate a target address or symbol across source, manifests, and docs
rg -n "0x0041C170|FrontMessageRuntimeView" src config docs

# Inspect remaining heuristic semantic/readability debt
python3 -B scripts/report-semantic-debt.py --top 25
```

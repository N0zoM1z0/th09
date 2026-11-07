# TH09 native boss and leaf work — 2026-11-07

This is a **source-and-diagnostic checkpoint**, not a new exactness receipt.
The target remains the original Japanese TH09 v1.50a:
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
All experiments used the registered live `th09` worktree and the retained
VC7.1 profiles. No full cold replay, target-byte patch, register pinning,
compiler-option sweep, or other-session worktree assumption was used.

## Source commits and protected original extents

Four independently completed `gpt-web:` source checkpoints:

| Local commit | Actual C++ work | Original-bound checks |
| --- | --- | --- |
| `a815c6c0` | `EnemyManagerView::OnUpdate` now reads the native Player tracking record and focus effect; `RunEcl` writes `AnmVm.rotation.z`, removing the late-stage rotation overlay | Enemy 3,900/97/13; RunEcl 15,564/598/30; complete scoped original-bound siblings |
| `9b905a85` | The Enemy scheduling freeze gate reads `PlayerSharedRuntimeView::updateBlock1095C` instead of an independent 0x1095C-byte shadow | Enemy 3,900/97/13; 91 exact sibling comparisons |
| `3cab7f1e` | `DrawReplayMenu` and `DrawReplaySave` share `TitleScreenReplayView.hpp`; both access a single checked 50-entry replay record and path layout, including the two state aliases at +0x28 | Menu 1,280/57/29; save renderer exactly **800/800**; 23 related `unittest` tests pass |
| `ca415d42` | Effect initialization, VM transfer, script execution and draw callbacks use the already native `AnmVm*` / `AnmVm` members directly; ten redundant same-type casts removed | Effect callback 51/4/8; 16 exact same-owner comparisons / 2,571 bytes; 72 related `unittest` tests pass |

The complete RunEcl-focused comparison checked **99** exact related units and
30,461 physical bytes when the first native rotation change was committed.
The 20 manifest changes in `a815c6c0`, and two in `9b905a85`, only update
compiler-generated `$L...` COFF-local label spellings; all addresses, field
positions, relocation types and independently original-bound physical bytes
were held fixed and re-compared. These do **not** promote a nonexact function.

Source layout assertions in `TitleScreenReplayView.hpp` explicitly check
`+0x28` state, `+0x8C` name, `+0x28C` paths (0x200 bytes each), `+0x68E4`
replays (0x1EC bytes each), `+0xC908` selected slot and `+0x1B230` timer.
The main `TitleScreen.cpp` still contains an independent fuller view; this
checkpoint unifies **the two separate draw carriers only**. Complete class
ownership and original nominal source unit remain provisional.

## Fresh negative controls: real source was restored after each probe

All results below are full target-bound diagnostics, not partial bytes credited
as complete matches. Experiments were serial and left the compiled source
unchanged unless explicitly committed above.

- **Effect AddedCallback** (`.analysis/gpt-web-effect-models-20261107/`):
  eight natural receiver/owner, scope and branch C++ expressions compiled.
  Their complete callback byte distance was 11, worse than the maintained
  51-byte/four-field/eight-difference body. None was adopted. Original ECX,
  ESI and EDI allocation remains unresolved.
- **Replay SaveReplay** (`.analysis/gpt-web-replaysave-models-20261107/`):
  the natural structure copy and seven other buffer-copy expressions were
  tested against the complete 1,238-byte/53-field owner. The ordinary
  candidates retained the two byte differences at `+0xB7/+0xBB` without
  improvement; explicit word-loop copying changed much more. The existing
  `memcpy` remains. No widened alignment/lifetime assumption was adopted.
- **Replay BeginPlaybackStage**
  (`.analysis/gpt-web-playback-stage-address-20261107/`): nine natural C++
  array-row, address-of, pointer-decay and flat-pointer expressions all
  produced the same 538-byte/32-field/39-overlap-plus-two-missing owner.
  The actual conditional checks the **table address**, not the stored frame
  pointer. Its existing bounded byte-representation expression avoids
  introducing potentially out-of-bounds `frameStart[stage]` array semantics.
- **PlayerGameplayState anonymous type trap**: replacing the local
  `PlayerSharedRuntimeView` type with the shared public header changed the
  decorated type name and member lookup of exact Player functions. This
  experiment was **reverted** before the second commit; the independent
  Enemy owner safely uses the shared native type without changing that
  original-proven Player symbol ABI.

No claim is made that earlier observations are universally correct. The
real original, compiler object, and canonical exact unit comparisons are the
controlling evidence for the specific current source/profile.

## Reproduction — only changed or directly related scopes

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/check-boss-frontiers.py --owner enemy
python3 -B scripts/check-boss-frontiers.py --owner runecl
python3 -B scripts/check-boss-frontiers.py --owner replaymenu
python3 -B scripts/build-match-unit.py --unit title-screen-draw-replay-save
python3 -B scripts/compare-coff-function.py --unit title-screen-draw-replay-save
python3 -B scripts/check-boss-frontiers.py --owner effect
python3 -B -m unittest discover -s tests -p 'test_title_replay_*.py' -q
python3 -B -m unittest discover -s tests -p 'test_effect*.py' -q
python3 -B scripts/validate-docs.py
```

`--owner replaysave` and `--owner replay` are available to recheck the
unadopted Replay hypotheses. Do not use `--owner all` for these edits.
The probes in ignored `.analysis/` are local research material, not tracked
source or an accepted Factory receipt; recreate them if absent.

At this checkpoint the live ledger is unchanged: **2,248** candidates,
**1,034** authored/source-present mappings, **1,021** exact authored functions,
**13** still nonexact functions, and **29,834** still-nonexact authored bytes.
The Windows product link, whole-image identity, runtime behavior, semantic
port and Factory acceptance gates remain unproven. A local Git commit is a
checkpoint, not an Oracle acceptance decision.

## Follow-up: true Supervisor camera owners and Enemy positional transport

The following are **additional source commits** after the four listed above,
not newly promoted exact ledger functions. Original target identity and the
pinned VC7.1 profiles were rechecked; the prior negative source notes are not
assumed correct merely because they are written in this document.

| Source checkpoint | What actually changed | Fresh narrow source/target proof |
| --- | --- | --- |
| `126bc837` | Enemy OnUpdate's temporary Etama descriptor assigns its `Float3 position` directly from the native Enemy vector, rather than using an untyped `memcpy` for the 12-byte copy | Complete 3,900-byte owner, 97 fields, 13 unlike bytes; 91 exact related functions, 41,868 physical bytes |
| `52d2ede5` | The same boss borrows the native `EnemySideStateView` for the Etama receiver and character selector. Its position assignment remains typed. The source-only scalar-context probe and tests follow the new native receiver | Complete 3,900/97/13 unchanged; 69 calls, 170 blocks and switch graph preserved; 91 exact related functions; 91 Enemy tests pass |
| `e6d42e9c` | Exact `InitializeD3DRendering` uses `SupervisorCtorArrayElement (&viewports)[3] = g_Supervisor.ctorArray078` instead of a duplicated 0x2D0-byte camera-array overlay. A separate 4-byte flag receiver remains because it is codegen-sensitive | Complete 1,561/1,561 bytes exact; the separate 74- and 809-byte D3D capability leaves also exact; dedicated layout tests pass |
| `2798a84b` | Exact `BackgroundStageScript` now reads/writes the actual `g_Supervisor.ctorArray078` and verified native member names, removing the fake `BackgroundStageSupervisorView` global and its macro. A compile-time guard connects the background camera record to the native 0xF0 viewport layout | Stage script 2,472/2,472 code and **2,652/2,652 physical** bytes, 133 fields; interpolation 423/423 code, 448/448 physical; three 20/20, 20/20 and 91/91 same-TU leaves exact |

The Background change replaced **19** `?g_Supervisor@@3UBackgroundStageSupervisorView@@A`
relocation symbols with `?g_Supervisor@@3VSupervisor@@A` at the same independently
reviewed base `0x004B3100`. The original field offsets, relocation kinds,
addends and target addresses are unchanged, checked against fresh actual
COFF rows. Another 55 fields merely acquired refreshed compiler-local `$L...`
spellings. All five complete same-source original PE comparisons pass after
fresh targeted compilation; none was granted new authored exactness credit.

Additional retained **negative natural-source controls** (never committed as
production game code): `.analysis/gpt-web-enemy-side-models-20261107/` contains
ten versions of the Enemy draw-group index, Side lifetime, periodic timer,
threshold and position-copy idioms. The accepted-safe candidates stayed at the
same 3,900/97/13; the early Side and position-pointer variations changed
literal field positions and failed the strict oracle. The local scratch
`./.analysis/gpt-web-effect-register-schedule-20261107/` contains nine
`EffectManager::AddedCallback` receiver/owner declaration-order models. All
retain a complete 51-byte callback and four fields; none lowers its eight
remaining register-scheduling bytes, and some increase the distance to eleven.
All trial game sources were restored before the accepted changes above.

**Use these focused commands**, not `--owner all` or whole-project cold replay:

```bash
python3 -B scripts/check-boss-frontiers.py --owner enemy
python3 -B scripts/build-match-unit.py --unit initialize-d3d-rendering
python3 -B scripts/compare-coff-function.py --unit initialize-d3d-rendering
python3 -B scripts/build-match-unit.py --unit background-run-stage-script
python3 -B scripts/compare-coff-function.py --unit background-run-stage-script
python3 -B scripts/compare-coff-function.py --unit background-interpolate-camera-vector
python3 -B -m unittest discover -s tests -p 'test_enemy*.py' -q
python3 -B -m unittest discover -s tests -p 'test_d3d_init_native_supervisor_viewports.py' -q
python3 -B -m unittest discover -s tests -p 'test_background_native_supervisor_viewports.py' -q
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
```

**Open frontiers stay open:** Enemy OnUpdate remains 13 physical byte differences;
RunEcl remains 15,564/598/30 (opcodes 4 and 155); Effect AddedCallback stays
51/4/8. The current 1,034 source-present / 1,021 exact / 13 nonexact ledger
is unchanged. Full Windows product linking, semantic/runtime/port tests and
Factory acceptance are not established by these local source commits.

## Continued original-bound HUD and camera reconstruction (2026-11-07)

These seven further `gpt-web:` code checkpoints are **real source edits**, not
promotions of existing exact-function credit. The original Japanese 1.50a PE,
the existing pinned VC7.1 compiler profiles, independently decoded relocation
fields, complete physical extents and true native side/VM/camera/timer storage
controlled adoption. Old source hypotheses and archived reports are not treated
as proof of the current tree.

| Checkpoint | Accepted native C++ work | Complete local original-bound evidence |
| --- | --- | --- |
| `9fded1d1` | Background's four `+0x834..+0xB04` camera snapshots reuse `SupervisorCtorArrayElement` as their checked 0xF0-byte data type, without changing Background ownership | Stage script 2,652/2,652 physical; camera interpolation 448/448; three same-TU leaves 20/20, 20/20 and 91/91 exact |
| `1abea221` | HUD draw screen-shake offsets read `g_Supervisor.ctorArray078` rather than a private 0xF0-byte viewport projection | Entire `FrontSide::OnDraw` 3,094/3,094 exact |
| `b567a713` | HUD draw uses the true `PlayerSideStateView` and `PlayerLifecycleView::position1B88` instead of two smaller shadow structures | Entire `FrontSide::OnDraw` 3,094/3,094 exact |
| `a2a70373` | HUD update calls the real `Supervisor::SelectSide @0x401440`, not the independent fake `FrontSideSupervisorView` receiver; both relocation names bind their previous original targets | 15/15 complete FrontSide source functions, 4,087 physical bytes exact |
| `21d6e36d` | HUD's score/phase reads use the native `AsciiGameSideRuntimeView` record rather than `FrontSideSideRuntimeView`; its distinct GameManager method ABI remains unchanged | 15/15 functions / 4,087 bytes exact |
| `1f3eccc0` | HUD's color, scale, position and flags fields all use native `AnmVm` and `AnmVmScriptColor`; removes duplicate 0x2A4 VM and 4-byte color projections and the VM cast macro | 15/15 functions / 4,087 bytes exact |
| `d3853707` | HUD's integer current-time and `%2` checks now use real `ZunTimer` methods; five original-bound call fields retain their positions/target addresses and use the exact native getter/modulo symbols | 15/15 functions / 4,087 bytes exact; 40 focused FrontSide tests pass |

The source changes are protected by offset/layout assertions and new focused
`tests/test_background_native_supervisor_viewports.py`,
`tests/test_front_draw_native_supervisor_shake.py`,
`tests/test_front_draw_native_player_side.py`,
`tests/test_front_side_native_supervisor_owner.py`,
`tests/test_front_side_native_side_runtime.py`,
`tests/test_front_side_native_anm_vm.py` and
`tests/test_front_side_real_zun_timer.py`. Each successful COFF comparison
covers the **whole real function**, not a selected instruction span. No full
cold replay was run.

The separate `.analysis/gpt-web-save-register-lifetime-20261107/` packet
contains **15 natural SaveReplay C++ probes** (loop/link lifetimes, mutable
headers, typed and byte destination pointers, alternative offset expressions,
and integer-width forms). All are nonadopted: some keep exactly the current
1,238-byte/53-field owner with two residual bytes at `+0xB7/+0xBB`; others
regress previously equal positions. The input source was restored after each
trial. The original wants the destination base in EDI before the first stage
header `rep movsd`; these controls do not recover it. Do not claim byte-exact
SaveReplay on the strength of any of these observations.

Narrow reproducibility:

```bash
python3 -B scripts/build-match-unit.py --unit background-run-stage-script
python3 -B scripts/compare-coff-function.py --unit background-run-stage-script
python3 -B scripts/compare-coff-function.py --unit background-interpolate-camera-vector
python3 -B scripts/build-match-unit.py --unit front-side-draw
python3 -B scripts/compare-coff-function.py --unit front-side-draw
python3 -B scripts/build-match-unit.py --unit front-side-on-update
python3 -B scripts/compare-coff-function.py --unit front-side-on-update
python3 -B scripts/compare-coff-function.py --unit front-side-added-callback
python3 -B -m unittest discover -s tests -p 'test_front*.py' -q
python3 -B scripts/validate-tracking.py --require-target
```

The ledger remains **2,248 candidates / 1,034 source-present / 1,021
exact / 13 nonexact (29,834 bytes)**. `EnemyManager::OnUpdate` is still
3,900 bytes with 13 nonmatching bytes; `RunEcl` still has 30; SaveReplay
still has two; AddedCallback still has eight. Function-level equality here
does not prove a complete Windows link, runtime equivalence, original TU
partition, semantic port or remote Factory acceptance. Git commits are
checkpoints, not Oracle acceptance receipts.

## CardAttack boss and leaf ownership continuation — 2026-11-07

Seven further code checkpoints recover more **natural native C++** from the
pinned Japanese TH09 v1.50a owner, without claiming original class spelling,
source TU boundaries, or *new* exact-function credit. Every adopted source
change was rebuilt with its reviewed VC7.1 profile. Complete original PE bytes,
all COFF relocation fields and actual call targets were compared independently.
No all-owner/full cold replay was run.

| Committed checkpoint | Source recovered | Real original-bound evidence |
| --- | --- | --- |
| `5c187810` | `CardAttack::OnDraw` calls real `Supervisor::ConfigureScreenViewport` rather than a duplicate `CardAttackSupervisorView` fake method | Target method `0x00403D90`, original draw 334/334 bytes, 17 fields; eight CardAttack source functions 1,302 bytes exact |
| `893cf5a9` | `CardAttack::Create/OnUpdate/OnDraw` share native `AsciiGameManagerView`, its transition bits and exact popup-coordinate methods, eliminating `CardAttackGameManagerView` | 13 field identities changed to genuine global/callees at the **same original field positions/targets**; all eight source functions remain exact |
| `d9fc12b2` | `CardAttack` side pointers reuse the single native 0x38-byte `AsciiGameManagerSideView` and Create no longer casts from another unrelated side record | Eight same-source functions and eleven additional directly dependent CardAttack leaves exact; nonexact GameManager setup's 1,682/1,689-byte, 172-field scope unchanged |
| `2365bd79` | Recovers the 0x1348..0x1364 CardAttack primary/secondary values and extra pointers inside the real 0x1368-byte record, removes private `CardAttackBeginTailView`/`CardAttackEndTailView` | 17 affected exact functions, 2,757 physical bytes, including 923-byte StartAttack, 536-byte update, 334-byte draw and eight Begin/End callbacks |
| `d1bb8c16` | `CardAttackStartView : CardAttack` preserves the proven `StartAttack` source ABI but inherits real owner storage; retained specialized side-pointer aliases; implicit upcasts replace unnecessary `reinterpret_cast<CardAttack *>(this)` | StartAttack 923/923 exact with all 29 relocations; 18 other related exact units; nonexact charge boss still 1,210 original/1,197 candidate, 70 fields and 58 calls |
| `dfc3e2b7` | Update/Draw use genuine `AnmVm` scalar, color, flags, position and sprite arrays directly, discarding the redundant identity helper, VM alias and unused Float2/Float3 shadows | **All 19** CardAttack source/dependent exact functions, 2,804 physical bytes, all original-bound; 11 focused CardAttack tests |
| `baee11df` | Exact `StartAttack` dereferences real `FrontSide` and its `FrontSideAuxView` VMs at +0xA678 instead of two fake HUD/auxiliary allocations | StartAttack still 923/923 exact; nonexact charge caller keeps 70 fields, 58 calls and 13 missing bytes; 13 focused CardAttack tests |

The 19 distinct original functions checked across the changed CardAttack
sources have 2,804 physical bytes. Separate original `Supervisor` and
`AsciiGameManagerView::TransformPopupX/Y` leaves anchor their actual call
addresses `0x00403D90`, `0x00401680`, and `0x004016B0`.
The StartAttack independent target remains `0x00403E50` with the
`?StartAttack@CardAttackStartView@@QAEXHHHPBD@Z` symbol, **not** a fabricated
change to a new method identity. The inherited class and specialized side
aliases are source-facing representations of independently observed storage,
not proof of the game's original class hierarchy.

### Nonexact boss experiments — rejected source restored

- `.analysis/gpt-web-charge-mode-20261107/` records **14** natural C++
  `PlayerChargeAttackView::UpdateChargeAttack` experiments: unsigned/signed/short,
  bool, structured local, branching/comma forms, initialized/captured locals,
  direct ReplayInput references. The target is 1,210 bytes. The 1,197-byte
  candidate still lacks 13 bytes and the independently observed stack byte
  `sideMode` at EBP−1. Two 1,201-byte variants have only nine missing bytes
  but do **not** restore the original stack access or control-flow graph.
  No such variant was adopted or credited.
- `.analysis/gpt-web-movement-aux-20261107/` preserves eight direct native
  `ReplayInputState::auxiliary2A` candidate forms for the **1,900-byte**
  Player movement boss. Those compiled, but the strict inspector rejected
  mismatched independent COFF operand shapes. Its previous 66 fields,
  23 calls and 122 differing bytes remain the verified baseline.
- `.analysis/gpt-web-shake-natural-20261107/` preserves **12** ordinary C++
  expressions/lifetime alternatives for 336-byte ScreenEffect shake envelope.
  The best remains 336 bytes/20 fields/8 calls/**26** different bytes.
  Several alternatives regressed to 249–317 differences; none was adopted.
  Original ScreenEffect/Timer register lifetimes (EDI/ESI respectively) still
  differ from the current ESI/EDI allocation. No asm, register pinning, target
  patching, speculative compiler switch or partial-byte exactness claim used.

**Focused reproduction** (not `--owner all`, not full cold replay):

```bash
python3 -B scripts/build-match-unit.py --unit card-attack-start
python3 -B scripts/compare-coff-function.py --unit card-attack-start
python3 -B scripts/build-match-unit.py --unit card-attack-on-update
python3 -B scripts/compare-coff-function.py --unit card-attack-on-update
python3 -B scripts/build-match-unit.py --unit card-attack-on-draw
python3 -B scripts/compare-coff-function.py --unit card-attack-on-draw
python3 -B scripts/build-match-unit.py --unit card-attack-begin-pl00-type0
python3 -B scripts/compare-coff-function.py --unit card-attack-begin-pl00-type0
python3 -B scripts/build-match-unit.py --unit card-attack-end-free1360
python3 -B scripts/compare-coff-function.py --unit card-attack-end-free1360
python3 -B scripts/check-boss-frontiers.py --owner charge --skip-exact-siblings
python3 -B scripts/check-boss-frontiers.py --owner gamemgr --skip-exact-siblings
python3 -B -m unittest discover -s tests -p 'test_card_attack*.py' -q
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
```

Relevant new guards:
`tests/test_card_attack_real_supervisor_viewport.py`,
`tests/test_card_attack_real_game_manager_abi.py`,
`tests/test_card_attack_native_tail_leaves.py`,
`tests/test_card_attack_start_native_inheritance.py`,
`tests/test_card_attack_start_native_front_side.py` and the existing
`tests/test_card_attack_native_vm_and_manager.py` (updated to the real VM).

The status ledger remains **2,248 candidate / 1,034 source-present /
1,021 exact / 13 nonexact**, with **29,834** bytes of nonexact authored
functions. These C++ improvements are valuable source reconstruction but **do
not** by themselves establish any new byte-exact function count, an entire
Windows image rebuild, whole-program semantics, runtime or Factory acceptance.

## Native CardAttack → Player → ChargeAttack chain (continued 2026-11-07)

This continuation starts from the real clean 1.50a repository target and carries
**nine additional `gpt-web:` source commits**. These are source-fidelity gains,
not promotions of new authored-exact functions. Each original-bound byte
comparison uses the pinned existing VC7.1 profile and the original PE at its
recorded address. A Git checkpoint is not acceptance by the Factory, runtime
semantic equivalence, or a proven original source translation-unit partition.

| Checkpoint | Real native C/C++ reconstruction | Full scoped original-bound result |
| --- | --- | --- |
| `8008049a` | The exact `CardAttackStartView::StartAttack` uses genuine `Background::transitionMode` and `transitionFrames` (+0x1C/+0x20), removing the private Background state layout | StartAttack **923/923 exact** |
| `65d4d96c` | Removes `CardAttackStartSideView`'s entire duplicate 0x38-byte record by sharing `AsciiGameManagerSideView`; resolves a genuine VC7.1 old `struct` forward-declaration conflict, not an executable mismatch | **19 CardAttack exact functions / 2,804 physical bytes**; nonexact ChargeAttack and setup envelopes preserved |
| `9271ddbe` | StartAttack reads Player `header24.state7C/state80/state8C/state90/state94` and `tableValue303F0` from native `PlayerLifecycleView` (+0xA0/+0xA4/+0xB0/+0xB4/+0xB8/+0x303F0), deleting its entire private Player view and temporary Side pointer union | StartAttack **923/923 exact**, 19 related exact functions and exact Player `OnUpdate` **522/522** |
| `2b9be035` | The resource clamp method view derives from native `PlayerLifecycleView`; its 0x30388 resource is the real `scalar30388` | `SubtractResourceClamped` **51/51 exact** |
| `336b1c3c` | `PlayerChargeAttackView` borrows `PlayerResourceSubtractView`; ChargeAttack's three resource calls become inherited method calls instead of sibling overlay casts | Exact resource **51/51**, Player update **522/522**; nonexact ChargeAttack stays **1,197/1,210**, 70 fields/58 calls/13 missing |
| `11f0f432` | Three state-3 effect methods derive from the native Player; use its real Side, position and `timer303C8` instead of a private ~0x303D4-byte prefix | `ConfigureState3Effect13/14/15` **91/91, 91/91, 100/100 exact** |
| `98fa6e4c` | A zero-extra-storage view chain lets ChargeAttack call the exact State3 methods directly without three overlay casts | Whole ChargeAttack 70 fields/58 calls/13 missing unchanged; exact state3/resource/Player witnesses pass |
| `b69b0880` | The SpawnShots method view derives from native Player, using real shot and SHT allocation offsets; retained callback-specific shot/descriptors; Effect35 initialization now uses native Player ANM field +0xBC | `SpawnShots` **190/190**, `EffectType35::Initialize/Update` **72/72, 38/38 exact** |
| `0fbe2f17` | Native `PlayerChargeAttackView → PlayerSpawnShotsView → PlayerStateEffectView → PlayerResourceSubtractView → PlayerLifecycleView` source-facing hierarchy; two SpawnShots calls use the inherited method directly | ChargeAttack complete 1,197 candidate / 1,210 original, 70 real fields, 58 ordered calls, 13 missing unchanged. Eight representative exact caller/leaf owners **1,155 complete bytes** all pass |

The class inheritance above is a **zero-extra-storage source-facing view** of
observed Player ownership, **not** a claim about the game's original nominal
C++ inheritance. Methods retain their independent original C++ symbol names,
and no compiler flags, calling convention, target bytes, or canonical exact
credit were altered. The three specialist shot callback views remain distinct
because their exact callback ABI ownership has not yet been proved.

The main remaining ChargeAttack discrepancy is still the original **stack
`sideMode` byte at EBP−1** and associated register/control-flow lifetimes; the
candidate still emits 1,197 bytes versus 1,210 original. Earlier
`.analysis/gpt-web-charge-mode-20261107/` natural-source controls were
rejected, including the candidates that grew to 1,201 bytes without recovering
the real stack-byte dataflow. The new native inheritance does not close this
frontier. Likewise RunEcl, Enemy OnUpdate, Replay SaveReplay and the other
12 remaining nonexact functions were **not** marked exact here.

Additional scoped unit regression tests include:
`test_card_attack_start_native_background_transition.py`,
`test_card_attack_start_native_side_record.py`,
`test_card_attack_start_native_player_header.py`,
`test_player_native_resource_subtract.py`,
`test_player_charge_native_resource_chain.py`,
`test_player_charge_native_state3_chain.py`,
`test_player_state3_effect_native_storage.py`,
`test_player_spawn_shots_native_owner.py` and
`test_player_charge_native_shots_chain.py`.

To reproduce only the affected functions (do **not** invoke all-owner cold
replay):

```bash
python3 -B scripts/build-match-unit.py --unit card-attack-start
python3 -B scripts/compare-coff-function.py --unit card-attack-start
python3 -B scripts/build-match-unit.py --unit player-state3-effect13
python3 -B scripts/compare-coff-function.py --unit player-state3-effect13
python3 -B scripts/compare-coff-function.py --unit player-state3-effect14
python3 -B scripts/compare-coff-function.py --unit player-state3-effect15
python3 -B scripts/build-match-unit.py --unit player-spawn-shots
python3 -B scripts/compare-coff-function.py --unit player-spawn-shots
python3 -B scripts/build-match-unit.py --unit effect-initialize-type35
python3 -B scripts/compare-coff-function.py --unit effect-initialize-type35
python3 -B scripts/compare-coff-function.py --unit effect-update-type35
python3 -B scripts/check-boss-frontiers.py --owner charge --skip-exact-siblings
python3 -B -m unittest discover -s tests -p 'test_card_attack*.py' -q
python3 -B -m unittest discover -s tests -p 'test_player_charge_native*.py' -q
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
```

The final source ledger for this continuation is **2,248 candidates / 1,034
source-present / 1,021 exact / 13 nonexact functions**, with 29,834 bytes
in nonexact authored functions. Whole Windows product linking, complete
function inventory, runtime behavior and Factory acceptance remain open.

## Title native full input record and options snapshot (later 2026-11-07)

The live worktree was verified against the **Japanese TH09 1.50a original**
(SHA256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`)
before editing. The following five incremental `gpt-web:` commits are genuine
source-fidelity changes and **do not add new exact functions**:

| Local checkpoint | Source change | Scoped full-function proof |
| --- | --- | --- |
| `e7432aa6` | Share the original-bound eight-byte input method prefix between exact `InputView::IsPressedScrolling` and the key-config update owner | **42/42** leaf, **2,803/2,803** key boss, **106/106** auxiliary leaf; only two verified callee symbol names changed |
| `f5eb2d17` | Replace the temporary eight-byte class with a full `InputView : ReplayInputState` at native **0x8E-byte** stride. The original input array starts at **0x4ACE18**; its third slot is **0x4ACF34**. Bind the key-config global to the true `?g_TitleInput@@3UInputView@@A` name | Key boss **2,803/2,803**, scrolling helper **42/42**; three original-bound global symbols updated without target/offset/addend changes |
| `d57b7418` | Remove the main TitleScreen TU's duplicate tiny `InputView` and include `TitleInputView.hpp`, using the same full native input allocation | **41 complete exact Title functions, 18,972 physical bytes**, including StartMenu **1,560 code / 1,592 physical**; nine private VC7.1 `$L...` labels refreshed without altering original bounds |
| `15cd17b2` | Replace the Title three-slot input-array's independent `TitleInputStatePhysicalView` layout with the same native `InputView` type; both cursor helpers now call the actual scrolling leaf | Two **167/167-byte** cursor bosses exact; **8** exact native call symbols, Title source **41/18,972**, nine compiler-only labels refreshed |
| `3bdd198d` | Replace the options page's independent `TitleConfigSnapshotView` record with native `GameConfiguration` (size **0xCC**, fields `valueAD` +0xAD and `windowed` +0xB1) | OnUpdateOptions **2,045/2,045 exact**, PlayMenuSound **34/34 exact**, 157 focused Title tests |

The original-bound `0x423158` input scrolling method retains its exact ABI and
body. The three-slot input table's global identity stays distinct from the
last-slot alias; a correct physical address is not proof of a distinct original
source-level owner. Any remaining `TitleScreenOptions.cpp` local `InputView`
representation has deliberately **not** been overwritten: substituting the
out-of-line native full class into the 2,045-byte options owner yielded a
**2,042-byte candidate**, not exact, even under the original compiler flags.
Five natural receiver/ref/cast variations did not repair that codegen drift;
the source was restored. This is a deliberate negative compiler observation,
not a rationale for declaring the eight-byte window the whole native object.

The independently nonexact **3,900-byte Enemy OnUpdate** was reexamined at its
original 13 difference bytes:
`0x19C,0x1A3,0x252,0x263,0x26D,0x272,0x294,0x29A,0x29C,0x2A5,0x2A9,0x2B7,0x2BD`.
A further **16 natural C++ models** for `Float3` descriptor copying, pointer
versus reference lifetimes, Side record scheduling and draw group indices were
independently rebuilt. All valid unchanged-width candidates retained **3,900
bytes / 97 bound fields / 69 calls / 13 unlike bytes**; component-wise
rewrites moved or changed independently original-bound literal positions and
were rejected. No candidate was committed as an exact improvement. The related
82-byte collision leaf remains exact. An additional 51-byte effect callback
check remained **51/51 bytes with four fields and eight unlike bytes**.

Relevant ignored experiment evidence is under
`.analysis/gpt-web-enemy-descriptor-models-20261107/`,
`.analysis/gpt-web-options-native-input-probes-20261107/`, and
`.analysis/gpt-web-title-input-triple-20261107/`. The finished source is clean;
no failed candidate was merged. The following **scoped** tests and checks
avoid the forbidden whole-owner cold replay:

```bash
python3 -B scripts/build-match-unit.py --unit input-is-pressed-scrolling
python3 -B scripts/compare-coff-function.py --unit input-is-pressed-scrolling
python3 -B scripts/build-match-unit.py --unit title-screen-update-key-config
python3 -B scripts/compare-coff-function.py --unit title-screen-update-key-config
python3 -B scripts/build-match-unit.py --unit title-screen-start-menu
python3 -B scripts/compare-coff-function.py --unit title-screen-start-menu
python3 -B scripts/compare-coff-function.py --unit title-screen-move-character-cursor-horizontal
python3 -B scripts/compare-coff-function.py --unit title-screen-move-character-cursor-horizontal-for-input
python3 -B scripts/build-match-unit.py --unit title-screen-options-update
python3 -B scripts/compare-coff-function.py --unit title-screen-options-update
python3 -B scripts/check-boss-frontiers.py --owner title --skip-exact-siblings
python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
python3 -B -m unittest discover -s tests -p 'test_title*.py' -q
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
```

The ledger is still **2,248 candidates; 1,034 source-present; 1,021 exact;
13 nonexact covering 29,834 bytes**. In particular, Enemy OnUpdate, RunEcl,
Replay SaveReplay, Title OnUpdateResult and DrawReplayMenu are not newly exact.
Whole Windows linking, semantic/run-time validation and Factory acceptance
remain unproven. This source is a local Git checkpoint, not a remote push.

## Exact native options input and one 20-byte D3D vertex across gameplay (later 2026-11-07)

The live original remains Japanese TH09 v1.50a, SHA256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Five further `gpt-web:` source checkpoints preserved original-bound function
bytes, relocation types/offsets/addends/targets and unaltered VC7.1 profiles:

| Source checkpoint | Evidence-backed source reconstruction | Exact original-bound owner checks |
| --- | --- | --- |
| `3bc1eaf9` | Replace the options page's private **8-byte** `InputView` prefix with a **0x8E-byte** `InputView : ReplayInputState` source-facing receiver, retaining its native +0x06/+0x04 scrolling words and original **inlined** implementation | `TitleScreen::OnUpdateOptions` **2,045/2,045 bytes**, `PlayMenuSound` **34/34**; the two original `InputView::IsPressedScrolling` relocations stay bound to the exact 0x423158 leaf |
| `f5ee2aa5` | Introduce **one `VertexDiffuseXyzrhw` concrete definition** with native `Float3 pos` +0x00, aliased `w`/`rhw` +0x0C, `diffuse` +0x10 and its exact constructor identity 0x4343D0. ScreenEffect and two Effect strip callbacks use it | `ScreenEffect::DrawSquare` **527/527**, strip Type27 **491/491**, Type36 **506/506**. Scoped ScreenEffect source: **18 exact siblings / 2,562 physical bytes**. Nine VC7.1 `$L...` names in 447-code/480-physical-byte RegisterChain were refreshed; all 55 original-bound fields including addends/targets frozen |
| `20f1eff2` | The same native vertex now underlies `EffectType34VerticesView::fan54[33]` and `lines2E8[4][32]`, preserving the exact **0xEF8-byte** allocation; it remains `Alloc`/`memset`-initialized, not constructed as a C++ object | Effect34 draw **946/946**, Type34 init **178/178**, Type29 init **152/152** exact. Same-ECL focused nonexact guards: **RunEcl 15,564 bytes, 30 residual** and **Enemy OnUpdate 3,900 bytes, 13 residual** unchanged |
| `07cf3266` | Remove ANM primitive draw's independent nominal vertex definition; use native vertex for triangle fan/strip and all other primitive source methods | **12 complete exact functions / 1,763 physical bytes**, including triangle strip **318**, triangle fan **318**, vertex-buffer setup **472** |
| `d3fd84d0` | ANM 3D projection and 3D draw borrow that same native vertex; eliminate the remaining duplicated concrete `VertexDiffuseXyzrhw` declarations in `.cpp` sources | Project3DQuad **595/595** and Draw3D **811/811** exact |

An important corrected **negative** finding: a prior attempt to replace the
Options page's inline 8-byte helper by an **out-of-line** native method shortened
the 2,045-byte source owner to **2,042**, so it was rightly rejected. Five
separate natural C++ **inlined** full-`ReplayInputState` alternatives now produce
identical 2,045-byte code. Changing the source-facing class's nominal name to
`OptionsInputView` left the bytes intact but changed two authentic
`InputView::IsPressedScrolling` COFF call symbols, so that form was rejected.
Nominal original class definitions across source TUs and full product link
compatibility remain unproven; the original-bound method identity is deliberately
preserved instead of silently changing the manifest to a fake alias.

Additional *real* nonexact frontier work, not just a source inspection:

- `ScreenEffect::CalcShakeEnvelope`: newly compiled **17** natural receiver,
  declaration, timer, magnitude, RNG and local-lifetime variants. All admissible
  candidates with the original **336 bytes / 20 relocation fields / 8 calls**
  retain **26 unlike bytes**. Timer pointer/reference controls expanded to
  **337 bytes** with hundreds of differences, and were rejected. The target
  retains EDI for ScreenEffect and ESI for its timer; the current candidate
  allocates the two registers oppositely. No byte patches or register-fixing
  compiler extensions were used.
- `EffectManager::AddedCallback`: newly compiled **16** natural preload index,
  condition, output owner, reference and dataflow variants. All accepted
  **51-byte / 4-relocation** variants retain the **eight different bytes** at
  `+0x03..+0x09` and `+0x0D`. One form using two independently loaded global
  manager pointers failed the four-field audit; it was rejected. In the
  original, `mov edi,[g_AnmManager]` precedes `mov esi,ecx`, unlike the
  current receiver/global load schedule. Neither function was promoted exact.

Ignored and preserved focused experiment receipts:
`.analysis/gpt-web-options-inline-full-input-20261107/`,
`.analysis/gpt-web-native-diffuse-vertex-20261107/`,
`.analysis/gpt-web-shake-reg-lifetime-20261107/`, and
`.analysis/gpt-web-effect-new-branch-schedule-20261107/`.

Use **only changed-owner scopes**, not an all-owner cold replay:

```bash
python3 -B scripts/build-match-unit.py --unit title-screen-options-update
python3 -B scripts/compare-coff-function.py --unit title-screen-options-update
python3 -B scripts/build-match-unit.py --unit screen-effect-draw-square
python3 -B scripts/compare-coff-function.py --unit screen-effect-draw-square
python3 -B scripts/build-match-unit.py --unit effect-draw-callback-type34
python3 -B scripts/compare-coff-function.py --unit effect-draw-callback-type34
python3 -B scripts/build-match-unit.py --unit anm-manager-draw-triangle-strip
python3 -B scripts/compare-coff-function.py --unit anm-manager-draw-triangle-strip
python3 -B scripts/build-match-unit.py --unit anm-manager-project-3d-quad
python3 -B scripts/compare-coff-function.py --unit anm-manager-project-3d-quad
python3 -B scripts/check-boss-frontiers.py --owner screen --skip-exact-siblings
python3 -B scripts/check-boss-frontiers.py --owner effect --skip-exact-siblings
python3 -B scripts/check-boss-frontiers.py --owner runecl --skip-exact-siblings
python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
python3 -B -m unittest discover -s tests -p 'test_screen*.py' -q
python3 -B -m unittest discover -s tests -p 'test_effect*.py' -q
python3 -B -m unittest discover -s tests -p 'test_anm*.py' -q
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
```

The exact-function ledger did **not** change: **2,248 candidates / 1,034
source-present / 1,021 exact / 13 nonexact over 29,834 bytes**. This work
addresses both substantial exact boss implementations and the real remaining
nonexact codegen frontiers. Complete Windows linking, gameplay semantics and
Factory acceptance were **not** asserted or performed.

## Shared native ANM quad cache and renewed Player movement work (2026-11-07)

Starting from the clean pinned Japanese TH09 1.50a executable (SHA256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`),
this continuation made **seven further incremental `gpt-web:` source/test
commits**, each validated only against the changed original-bound units.
These are real C++ reconstruction/ownership changes, **not** seven newly exact
function credits.

| Checkpoint | Natural source reconstruction and evidence | Scope verified |
| --- | --- | --- |
| `1bb9fa95` | `PlayerLifecycleView::UpdateMovementAndOptions` snapshots the native ReplayInputState side index before deriving the real **0x8E-byte stride** and input slot; the typed input and focus access refer to the same original allocation, not a private input prefix | **1,900/1,900 physical bytes**, **66 fields**, **23 calls**; still **122 unlike bytes**. The independent Player OnUpdate (522 bytes) and ANM SetInterrupt (18 bytes) remain exact |
| `fe472a13` | Remove the private eight-byte `SupervisorPrimitiveView` D3D projection. Original ANM primitive code now consumes **`Supervisor::d3dDevice08` at +0x08** directly | **12 complete exact functions, 1,763 bytes**, including 238-byte DrawVertices, 318-byte TriangleStrip/Fan and 472-byte vertex-buffer setup |
| `eb133673` | Remove `SupervisorDraw3DView`, use the same native Supervisor device in the large Draw3D owner | Draw3D **811/811 exact**, original VA **0x43B1A0** |
| `c637e297` | Add the native **+0x12024 `D3DMATRIX` world cache** in `AnmManagerRenderCacheView`'s raw-byte-cover union. Replace Project3DQuad's otherwise duplicated 0x128E4-byte prefix with a derived view, retaining four +0x12894 software vertices | All four relevant ANM source profiles: **30 complete exact functions, 8,983 physical bytes**, including 595-byte Project3DQuad |
| `27ad499c` | Introduce `AnmManagerQuadCacheView` with one real +0x1288C sprite pointer, +0x12890 VB pointer, +0x12894 `VertexTex1Xyz[4]`, +0x128E4 sprite count. Primitive TU borrows this type and deletes its private tail | All 12 primitive methods exact (1,763 physical bytes) |
| `daec4bfd` | Make Draw3D and Project3DQuad typedef the shared zero-offset quad cache; Render inherits it and supplies only the later vertex-buffer pointers at +0x2B28E8/+0x2B28EC | **30 complete exact ANM functions, 8,983 bytes** under the original four VC7.1 profiles, with unchanged original-bound relocation targets, kinds and addends |
| `76a140c5` | Correct four stale structural tests that still required the now-removed private `AnmManagerPrimitiveView`, `AnmManagerProjectionView` and `AnmManagerRenderView` layouts; assert the *new* actual shared base/derived relationship, original field offsets and native array instead | **61 focused ANM tests pass**; independent full-function byte checks already recorded for all 30 |

The C++ software vertex type is source-facing. C++03 prohibits forming a union
of the nontrivially constructed `VertexTex1Xyz` and `VertexDiffuseXyzrhw`
classes. The source therefore uses the **same physical 20-byte prefix** under a
single typed ANM quad cache; Project3DQuad reads the XYZ part, while primitive
rendering observes the remaining texture UV words. **Do not** infer the
original game's nominal class inheritance or literal source-file partition
from this tested code-reconstruction view. Original PE/COFF field and function
extent matches, rather than a guessed class name, remain authoritative.

### Renewed Player movement boss research (not yet exact)

The 1,900-byte original movement code and its 66 COFF-bound fields were
independently rechecked, including both jump tables. New targeted C++
experiments under `.analysis/gpt-web-player-movement-input-20261107/` and
`.analysis/gpt-web-player-movement-focus-mode-20261107/` explored over thirty
input-array indexing, configuration pointer/reference, byte-stride, scoped
`auxiliary2A`, temporary lifetime and conditional-writing models. The valid
original-width candidates retained **1,900 bytes / 66 fields / 23 calls /
122 differing bytes / 435 normalized aligned instructions**; none recovered
a previously mismatched byte without breaking another independently matching
byte. Invalid unbraced branch declarations were corrected and **three legal
scoped** forms were tested as well. `volatile` on the configuration receiver
expanded the differences to well over a thousand and was rejected. All
experimental source edits were restored, and no synthetic register binding,
inline assembly, byte patch or compiler-profile change was accepted.

A particularly useful independently observed instruction difference occurs at
**original movement +0x126**: the target loads `g_GameConfiguration` **before**
loading the Player side index; the candidate loads the side index first. At
**+0x139** the original compares the input history word through an
array-base-plus-index absolute address (0x4ACE42 plus a byte offset); the
candidate uses an already computed input pointer plus +0x2A. Several natural
code formulations preserve the original-width candidate but do not alter
these VC7.1 register/operand scheduling choices. This is an **open frontier**,
not evidence that previous source code is the exact original.

For a subsequent pass, rebuild **only the relevant scope**, never all-owner
cold replay:

```bash
python3 -B scripts/check-boss-frontiers.py --owner movement
python3 -B scripts/build-match-unit.py --unit anm-manager-project-3d-quad
python3 -B scripts/compare-coff-function.py --unit anm-manager-project-3d-quad
python3 -B scripts/build-match-unit.py --unit anm-manager-draw-3d
python3 -B scripts/compare-coff-function.py --unit anm-manager-draw-3d
python3 -B scripts/build-match-unit.py --unit anm-manager-draw-triangle-strip
python3 -B scripts/compare-coff-function.py --unit anm-manager-draw-triangle-strip
python3 -B scripts/build-match-unit.py --unit anm-manager-flush-vertex-buffer
python3 -B scripts/compare-coff-function.py --unit anm-manager-flush-vertex-buffer
python3 -B -m unittest discover -s tests -p 'test_anm*.py' -q
python3 -B -m unittest discover -s tests -p 'test_player_movement_native_input_snapshot.py' -q
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
```

The final source ledger **remains 2,248 function candidates, 1,034
source-present, 1,021 exact, 13 nonexact spanning 29,834 bytes**. All commits
are local checkpoints only. A complete Windows link, runtime semantic check,
full inventory and Factory acceptance have **not** been claimed or performed.

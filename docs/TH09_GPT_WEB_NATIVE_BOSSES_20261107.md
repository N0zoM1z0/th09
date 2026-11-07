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

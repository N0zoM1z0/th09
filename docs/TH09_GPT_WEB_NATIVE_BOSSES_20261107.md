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

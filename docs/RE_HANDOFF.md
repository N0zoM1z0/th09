# TH09 reconstruction — current agent handoff (2026-11-18)

This is the restart guide for the live TH09 repository. It records current
status and the next useful work, not a session diary. Read
[AGENTS.md](../AGENTS.md) first, then this file. Use
[PROGRESS.md](PROGRESS.md) for generated totals and
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) for historical target/compiler findings.
The canonical ledgers and current source always outrank dated prose.

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

# TH09 reconstruction — current agent handoff (2026-11-19)

This is the single current restart guide. Read AGENTS.md first. The canonical
target, ledgers, current source and comparison scripts outrank prose. This
handoff is deliberately short; durable layout/compiler findings live in
KNOWLEDGE_BASE.md and generated counts live in PROGRESS.md.

## Verified project state

- Target: original Japanese TH09 v1.50a, 685,056 bytes,
  SHA-256 10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822.
- Compiler: pinned VC7.1 with each owner's profile in config/match-units.toml
  or config/frontiers.toml. Never change profiles to chase register choices.
- Stage: exact reconstruction. Latest source checkpoint: 7adfc808 (native EnemyManager draw-dispatch CFG). Local main has commits beyond origin/main; do not assume
  local commits have been pushed. Inspect git status and git log before work.
- Worktree was clean at handoff start. No interrupted source edits were found.
- Ledger: 2,248 tracked function candidates; origin/boundary review pending 0;
  reviewed but origin-unresolved 35; authored 1,037; excluded 1,176;
  1,037 authored mappings have source; 1,026 canonical exact functions;
  11 source-present authored functions remain nonexact (13,353 bytes).
- Complete function inventory is still unproven; callback tables need a
  separate audit. Whole Windows i386 product closure remains open. Semantic
  runtime validation and portability have not started.
- Source-naturalization commits do not increase exact-function credit.
  The exact count remains 1,026.

## Current nonexact owners

The table is the validator-backed list of all 11 source-present authored
nonexact owners. Run check-frontier.py NAME --show for a fresh local
comparison. An OK result means only that the recorded frontier baseline was
not worsened; it does not mean exact. Do not rebaseline REGRESSED candidates.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| EnemyManagerView::OnUpdate | 0x00410730 | 3,900/3,900 bytes; 2 differences (OK) | Source now mirrors retail short-circuit CFG: side-state bit is tested before the category read, and the early draw path falls into the shared append. Only the +0x19A/+0x1A1 index-register choice remains (retail EAX, candidate ECX). |
| ReplayManagerView::SaveReplay | 0x00420C60 | 1,238/1,238 bytes; 2 differences (OK) | Stage-header REP MOVSD destination setup at +0xB6/+0xB9: retail loads the destination base into EDI; candidate uses ECX. Preserve the native POD copy and caller proofs. |
| ReplayManagerView::BeginPlaybackStage | 0x00420840 | 540/540 bytes; 9 differences (REGRESSED) | EDX/ESI register lifetime around the playback-stage byte and side-0 runtime pointer. Raw count improved from 13, but new residual positions violate the monotone frontier; do not update it. |
| PlayerLifecycleView::UpdateMovementAndOptions | 0x0041C170 | 1,900/1,900 bytes; 4 differences (OK) | Focus-mode load order at +0x127..+0x12A. Retail loads g_GameConfiguration before player sideIndex; current source emits the reverse order. |
| PlayerLifecycleView::UpdateChargeAttack | 0x0041F810 | 1,210/1,210 bytes; 183 differences (OK) | Boss register allocation: retail loads the full DWORD sideIndex into EDI and forms the ReplayInputState pointer in EBX (0x8E stride). Current short-side/input locals allocate the opposite way; widening/removing locals regressed hundreds of bytes. |
| PlayerLifecycleView::CheckBulletCollision | 0x0041DFF0 | 849/862 bytes; 539 differences (REGRESSED) | Preserve the 0x34-byte target frame and recover the target CFG: circle check and shared hit dispatcher at +0x8E precede rotated-rectangle/AABB paths. Valid region types are 1/2/3 with delay <= 0; kinds 2/3 reward only for a non-null bullet with word +0x10C0 equal to zero. Candidate grew from the 842-byte frontier baseline; do not rebaseline. |
| EffectManager::AddedCallback | 0x0040D1C0 | 51/51 bytes; 4 differences (OK) | Four setup bytes concern the ANM-manager/context receiver order. Retail loads the global manager before preserving the callback context; simple helper, pointer and reference rewrites did not reproduce it. |
| ScreenEffect::CalcShakeEnvelope | 0x004227A0 | 336/336 bytes; 26 differences (OK) | Retain signed timer comparisons, DWORD endpoint arithmetic and X87 conversion order; receiver/timer alias probes did not improve the owner. |
| TitleScreenView::DrawReplayMenu | 0x004234F6 | 1,280/1,280 bytes; 29 differences (OK) | Continue from the native replay-row/VM layout and exact caller evidence; preserve all 29 recorded positions. |
| TitleScreenView::OnUpdateResult | 0x004266B5 | 484/485 bytes; 167 differences (OK) | Source is one byte shorter than the target owner. Character-array cursor is natural, but the complete owner remains nonexact. |
| Supervisor::UpdateNetworkService | 0x00430AA0 | 1,645/1,633 bytes; 695 differences (REGRESSED) | Large service boss: retain the real Supervisor receiver and 20-byte scratch/PopFrame lifetime evidence. The current natural-C++ candidate improves the raw count from 717 to 695 by sharing the frame/prefill local and scoping the timer callback pointer; it still exceeds the 1,629-byte ratchet and reloads the frame counter after GetSeed instead of keeping native EBX live. Do not rebaseline. |

### CheckBulletCollision target CFG (verified against 0x0041DFF0)

The target instruction order is more specific than the high-level geometry
summary in the table above:

- Region filtering starts at function offset +0x20. The X87 radius comparison
  is at +0x3B..+0x4B; its zero-radius edge enters the rectangle branch at
  +0xAB, while the nonzero-radius circle calculation occupies +0x4D..+0x7D.
- The shared hit-type dispatcher begins at +0x7E (absolute 0x0041E07E),
  before the zero-radius geometry. It reloads the region from the active-slot
  cursor, distinguishes kinds 2/3, and checks the bullet word at +0x10C0.
- The rotated-rectangle path starts at +0xBD; the AABB path starts at +0x13B.
  Successful rectangle/AABB tests return to the +0x7E dispatcher. Misses
  advance the active-slot cursor at +0x19E.
- The cancellation exit begins at +0x1A6; the direct-hit count/return path is
  at +0x326. Keep the target 0x34-byte local frame and X87 status-word branch
  sense; do not replace these comparisons with inline assembly.

A nested-loop source experiment matching the decompiler shape produced an
845/862-byte candidate with 581 differences, but the compiler still placed
dispatch at +0x17C and the candidate violated the 842-byte monotone baseline.
That experiment was discarded. The maintained source remains 849/862 bytes
with 539 differences (REGRESSED); do not record either experimental size as
a new frontier.

Since the previous handoff, commit 9f223ed4 preserves native DWORD wrapping
in the collision reward-side arithmetic. The changed carrier passed its
scoped replay: five exact units, 1,220 physical bytes, 32 relocation fields.
This does not grant exactness to CheckBulletCollision itself.

Commit 7adfc808 restructures EnemyManager::OnUpdate to match the original
short-circuit branch order: the side-state bit is tested first, the category
byte is read only when required, and the early draw path joins the shared
append. The function remains 3,900/3,900 bytes with the same two EAX/ECX
register-selection differences; no frontier was rebaselined. The changed
carrier passed `scripts/replay-scoped-exact.py --changed-file
src/EnemyManagerCore.inl`: two EclManager compiler profiles, 84 exact units,
45,420 physical bytes and 1,987 relocation fields. This is a scoped carrier
proof, not a whole-product cold replay.

The separate Etama fsincos helper is 36/33 bytes with 25 differences; it is
not one of the 11 source-present authored owners. Natural sinf/cosf remains
nonexact. Do not add assembly or padding.

## Recent local source checkpoints

The 12 local code/checkpoint commits preceding this documentation-only refresh, oldest first, are:

- a2751ec4 — natural network retry return and frame-side read; frontier is
  currently REGRESSED as recorded above.
- c231fc16 — native ExAttack draw VM array access.
- cadfc828 — native Type4 attack history and VM indices.
- d1b29b50 — typed Type4 source extra pointer ownership.
- ecd40217 — native 2D polar views in Player Type9 callback.
- bac91bf6 — native 4x4 ANM matrices in 3D draw.
- c8d57624 — signed native volume bytes in Title options.
- 38a9f1e0 — executable path indexing and Win32 window metrics in WinMain.
- ddcd49e1 — DirectSound LPVOID locked buffers and DWORD lengths.
- 96568947 — previous handoff/knowledge checkpoint; superseded by this file.
- 285e2339 — native DirectInput capability-record array.
- 57d3f852 — native Supervisor D3D device in viewport methods.
- 7adfc808 — mirror the native EnemyManager draw-dispatch short-circuit CFG.

Earlier local commits already contained the Player collision-region arrays,
SHT descriptor offset rebasing, Background VM/STD pointer recovery, ANM
preload slot arrays, MIDI/score/Enemy/Front source recovery and related
scoped proofs. Consult Git history when a specific change needs its original
commit detail. None of these naturalization commits changes the 1,026 exact
ledger count.

## Latest source work and boss probes (2026-11-19)

Two source commits restored `BeginPlaybackStage` to its recorded strict
frontier: `bb6c3c82` removes the `volatile` runtime-pointer alias, and
`fee10d43` simplifies it to the direct typed member read. It is now 540/540
bytes with the original 13 differences; it is not exact-function completion.
The remaining failed boss and leaf experiments are recorded in
`KNOWLEDGE_BASE.md` and were reverted. The worktree is clean.

- CheckBulletCollision remains the primary large target. Explicit-label and
  `do`-loop variants still let VC7.1 move rectangle geometry before the shared
  dispatcher. Maintained source remains 849/862 bytes with 539 differences;
  do not replace it with the 831/862-byte nested-loop experiment or rebaseline.
  Required CFG: circle test falls through to dispatcher at +0x7E; rotated
  rectangle/AABB successes branch back to it.
- UpdateNetworkService remains 1,645/1,633 bytes with 695 differences and is
  REGRESSED against the 1,629-byte/966-difference ratchet. The current best
  natural-C++ probe shares the frame/prefill local and scopes the early timer
  callback pointer; it still reloads frameCounter after GetSeed instead of
  retaining the native EBX value. Do not use volatile/register tricks or
  rebaseline.
- EnemyManager::OnUpdate remains at its two EAX/ECX draw-group index residuals.
  Scoped category temporaries, a shared helper and local index variables did
  not change the generated code.
- UpdateChargeAttack remains 1,210 bytes with 183 differences. Do not repeat
  naive `short`-to-`int` or pointer/reference conversions.
- UpdateMovementAndOptions remains 1,900 bytes with four residual bytes at
  +0x127..+0x12A. Local configuration aliases did not change the load order.
- ReplaySave and EffectManager::AddedCallback remain at two and four residuals
  respectively; destination-pointer sequencing and callback manager/context
  aliases did not improve them. EnemyManager::OnUpdate remains at its two
  EAX/ECX draw-group index residuals; const locals and typed pointer addition
  did not change the generated code.

## Additional IDA boss captures (2026-11-19)

`UpdateNetworkService` is native `sub_430AA0` (1,633 bytes); `UpdateChargeAttack`
is native `sub_41F810` (1,210 bytes). The new register-lifetime evidence and
all failed source variants are recorded in `KNOWLEDGE_BASE.md`, including a
full-function side-local candidate at 1,200/1,210 bytes and 648 differences.
Keep the network service's existing regressed frontier and the charge
attack's 1,210-byte/183-difference frontier unchanged. Do not use `register`,
volatile aliases, inline assembly or padding to force the original registers.

## Next work

Prioritize boss control flow and then close near-exact leaves:

1. CheckBulletCollision: follow the newly recovered IDA CFG: the inner
   region-filter loop owns rectangle/AABB tests; nonzero radius breaks to the
   circle test; both successful paths reach one dispatcher; direct hits branch
   to the shared counter tail. Preserve the 0x34 frame and EBX=Bullet,
   ESI=slot, EDI=Player roles. A three-loop dispatcher-before-circle probe
   reached 831/862 bytes with 694 differences but reversed the entry
   allocation and retained the wrong `jp` radius edge; a loop-local-region
   variant reached 835/862 with 693 differences and the same prologue issue.
   Both were reverted. Preserve the current source and solve the argument
   lifetime without new locals or register hints.
2. UpdateNetworkService: reconstruct the 20-byte local overlay, EDI state
   pointer lifetime, connection callbacks, prediction path and common return.
   Initial-only pointer aliases and register-hinted pointer probes did not
   help. The current best candidate is 1,645/1,633 bytes with 695 differences;
   continue from the natural timer/frame lifetime experiment and solve the
   first predicted-frame EBX lifetime without rebaselining.
3. UpdateChargeAttack: solve the side-index/input-pointer lifetime without
   forcing register allocation.
4. EnemyManager::OnUpdate: revisit the remaining EAX/ECX draw-group index only
   after the two large owners have advanced.
5. Close the near-exact ReplaySave, Player movement and Effect callback leaves.

All previous failed variants are restored. Do not copy diagnostic variants
from .analysis into maintained source. Keep the strict frontier ratchets.

## Rebuild and validation commands

For a nonexact owner: python3 -B scripts/check-frontier.py NAME --show

For a changed .cpp carrier: python3 -B scripts/replay-scoped-exact.py --source src/FILE.cpp

For a shared .hpp/.inl: first run
python3 -B scripts/replay-scoped-exact.py --changed-file src/FILE.hpp --dry-run,
inspect every selected dependent, then run the bounded selected replay.

Before target-dependent work:
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

For documentation and ordinary checkpoint maintenance:
python3 -B scripts/validate-docs.py
python3 -B scripts/ci.py
git diff --check

Do not run a project-wide cold replay for routine work. A frontier pass is not
an exact comparison; source edits require all affected exact owners, physical
tails and relocation identities to be checked. Rebind private $L labels only
after comparing actual COFF same-section definitions and original PE targets.

## Private inputs and storage

Preserve:
- resources/th09.exe and the original game data.
- .tools/ (shared Ghidra/JDK/MSVC selectors and this game's Wine prefix).
- _reference/.
- .analysis/ghidra-direct/243bffea8dcbe99a/ (active saved Ghidra project).
- .analysis/factory-runtime/ (including Wine lock and Ghidra runtime state).
- .analysis/c2-heap.bin, FUN_107844a4.decomp.txt, the four gdb-c2-*.cmd
  recipes, and exactly these three source diagnostics:
  SupervisorNetwork.natural-ebx-reload.cpp,
  PlayerCollisionHelpers.natural-preheader-842.cpp,
  PlayerCollisionHelpers.nested-three-loop-849.cpp.
- c2-analysis/c2.dll, both project roots (project/ and allocator-project/),
  scripts/ and retained top-level register/allocator/codegen reports.

The 2026-11-19 storage cleanup removed build/ (469 generated OBJ/PDB files),
one-shot factory-native-ida call captures, three Ghidra export snapshots,
Ghidra tmp output, and obsolete root-level replay/Cirno/recording probe files.
These are reproducible or superseded; recreate only the selected build object.
No c2-analysis probe project copies existed. The two C2 project roots and
their reports were left intact. .tools/, the target, reference checkout,
active Ghidra project and runtime state were not touched.

Never use git clean -fdx: it can delete protected ignored inputs. Check
git status and inspect dirty/untracked files before every resumed session.


## UpdateNetworkService: recovered entry and session CFG (2026-11-19)

IDA identifies `Supervisor::UpdateNetworkService` as `sub_430AA0` (native
extent `0x661`). The current source reconstruction now matches the first
`0x103` bytes byte-for-byte and restores the native `0x14` stack frame. The
local frontier is 1,645/1,633 bytes with 717 differing bytes, down from the
previous 966-difference baseline. This is a real partial recovery, not an
accepted exact match.

The source now follows the native receiver and branch lifetimes:

- Capture `g_SupervisorNetworkState` immediately after entering critical
  section 4. Use that receiver for the initial state/join reads, the join-side
  `localSide` store, the searching/host-wait state checks, and
  `GetRemotePeerId`.
- Keep methods whose native receiver is reloaded from the global as explicit
  `g_SupervisorNetworkState->...` calls. Reload the cached receiver after the
  searching text draw, after host-session setup, and after the successful
  remote-peer check before frame preparation.
- Move `HOST_SESSION` out of the initial idle branch. The initial idle-host
  path and the retry path jump to it; the join and non-idle paths pass through
  the searching check; a non-searching state skips the host body and reaches
  the host-wait check. The retry predicate now preserves the native
  short-circuit order: elapsed time below the jittered threshold OR join mode
  equal to one goes to wait; otherwise reset and retry hosting.
- Widen the service return local from `unsigned short` to the native 32-bit
  `SupervisorNetworkServiceResult`. This restores the `0x14` stack frame and
  lets the ordinary scratch return value remain distinct from reset/wait
  status.

Remaining work: the service status still spills to a stack home instead of
reusing EDI, the network-wait branch is 14 bytes later than the native label,
and the compiled body is 12 bytes longer than the native extent. Do not
rebaseline this frontier. Continue from the current natural-C++ control flow;
no register hints, inline assembly, padding, or synthetic bytes were added.

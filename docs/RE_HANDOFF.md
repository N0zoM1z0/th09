# TH09 reconstruction — current agent handoff (2026-11-20)

This is the single current restart guide. Read AGENTS.md first. The canonical
target, ledgers, current source and comparison scripts outrank prose. This
handoff is deliberately short; durable layout/compiler findings live in
KNOWLEDGE_BASE.md and generated counts live in PROGRESS.md.

## Verified project state

- Target: original Japanese TH09 v1.50a, 685,056 bytes,
  SHA-256 10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822.
- Compiler: pinned VC7.1 with each owner's profile in config/match-units.toml
  or config/frontiers.toml. Never change profiles to chase register choices.
- Stage: exact reconstruction. Latest source checkpoint: 8b4a1c60 (Effect +0xC9 draw flag recovered across the callback family); subsequent boss-frontier probes are recorded below. Local main has commits beyond origin/main; do not assume
  local commits have been pushed. Inspect git status and git log before work.
- Worktree was clean at this handoff update. No interrupted source edits were found.
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
| ReplayManagerView::SaveReplay | 0x00420C60 | 1,238/1,238 bytes; 2 differences (OK) | Stage-header REP MOVSD destination setup at +0xB6/+0xB9: retail `mov edi,[ebp-0x10]; lea edi,[edi+ebx-0xC0]`; candidate uses ECX as the LEA base. Direct typed assignment, C++ reference, mutable/typed cursor and direct source-header pointer probes did not change the two bytes. A saved stageRoot plus a sizing-only cursor matched +0x36 but left 6 differences (two stack-home stores reversed plus the EDI/ECX pair); reverted. Preserve the native POD copy and caller proofs. |
| ReplayManagerView::BeginPlaybackStage | 0x00420840 | 540/540 bytes; 9 differences (REGRESSED) | EDX/ESI register lifetime around the playback-stage byte and side-0 runtime pointer. Raw count improved from 13, but new residual positions violate the monotone frontier; do not update it. |
| PlayerLifecycleView::UpdateMovementAndOptions | 0x0041C170 | 1,900/1,900 bytes; 4 differences (OK) | Focus-mode load order at +0x127..+0x12A. Retail loads g_GameConfiguration before player sideIndex; current source emits the reverse order. |
| PlayerLifecycleView::UpdateChargeAttack | 0x0041F810 | 1,210/1,210 bytes; 183 differences (OK) | Boss register allocation: retail loads the full DWORD sideIndex into EDI and forms the ReplayInputState pointer in EBX (0x8E stride). Current short-side/input locals allocate the opposite way; widening/removing locals regressed hundreds of bytes. |
| PlayerLifecycleView::CheckBulletCollision | 0x0041DFF0 | 849/862 bytes; 539 differences (REGRESSED) | Preserve the 0x34-byte target frame and recover the target CFG: circle check and shared hit dispatcher at +0x8E precede rotated-rectangle/AABB paths. Valid region types are 1/2/3 with delay <= 0; kinds 2/3 reward only for a non-null bullet with word +0x10C0 equal to zero. Candidate grew from the 842-byte frontier baseline; do not rebaseline. |
| EffectManager::AddedCallback | 0x0040D1C0 | 51/51 bytes; 4 differences (OK) | Four setup bytes concern the ANM-manager/context receiver order. Retail loads the global manager before preserving the callback context; simple helper, pointer and reference rewrites did not reproduce it. |
| ScreenEffect::CalcShakeEnvelope | 0x004227A0 | 336/336 bytes; 26 differences (OK) | Retain signed timer comparisons, DWORD endpoint arithmetic and X87 conversion order; receiver/timer alias probes did not improve the owner. |
| TitleScreenView::DrawReplayMenu | 0x004234F6 | 1,280/1,280 bytes; 29 differences (OK) | Continue from the native replay-row/VM layout and exact caller evidence; preserve all 29 recorded positions. |
| TitleScreenView::OnUpdateResult | 0x004266B5 | 484/485 bytes; 167 differences (OK) | Source is one byte shorter than the target owner. Character-array cursor is natural, but the complete owner remains nonexact. |
| Supervisor::UpdateNetworkService | 0x00430AA0 | 1,633/1,633 bytes; 307 differences (REGRESSED) | The current natural-C++ candidate restores the 0x14-byte frame and EBX's mutually exclusive prediction-frame / prefill-counter lifetime. It retains the handshake and frame-packet receivers across RNG calls and uses live side/frame reloads in the second prediction call. Still four bytes above the 1,629-byte ratchet; do not rebaseline. |

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

The 15 local code/checkpoint commits preceding this documentation-only refresh, oldest first, are:

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
- 230e3f4c — restore native network prediction frame lifetime.
- 61fbf69e — recover network frame and packet flow (current 1,633/307 candidate).

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

- CheckBulletCollision remains the primary large target. The fresh IDA review
  confirms a nested region-filter loop: zero-radius rectangle/AABB tests stay
  inside the scan, while nonzero radius exits to the circle test; both hit paths
  feed the dispatcher at +0x7E. A literal nested-loop rewrite changed the entry
  registers and reached 828/862 bytes with 697 differences. A source-ordered
  circle/dispatcher/rectangle label graph preserved the entry roles but VC7.1
  still moved the rectangle body ahead of the dispatcher (825/862 bytes, 583
  differences). Both probes were reverted. Maintained source remains 849/862
  bytes with 539 differences; preserve the 0x34 frame, EDI=Player, ESI=slot,
  EBX=Bullet and do not rebaseline. A fresh three-loop rewrite retaining a
  region local produced 813/862 bytes and 722 differences but changed the
  entry to EBX=Player, EDI=slot, ESI=region. Removing the region local changed
  EDI to the position argument, grew the frame to 0x38, and produced 798/862
  bytes with 675 differences. Both are reverted; keep the 0x34 frame and
  native X87 operand order.
- UpdateNetworkService is now 1,633/1,633 bytes with 307 differences and is
  still REGRESSED against the 1,629-byte/966-difference ratchet. Commits
  `230e3f4c` and `61fbf69e` restore the native EBX frame/prefill lifetime,
  second-prediction live-side/frame reloads, handshake sender and frame-packet
  sender lifetimes, plus the final packed-input locals. The stack remains
  0x14. Continue on the packet-ready/shared-wait layout and remaining packet
  queue code; do not rebaseline or use register hints/volatile aliases.
- EnemyManager::OnUpdate remains at its two EAX/ECX draw-group index residuals.
  Scoped category temporaries, a shared helper and local index variables did
  not change the generated code.
- UpdateChargeAttack remains 1,210 bytes with 183 differences. A full-DWORD
  side local plus direct input-row calls can reproduce the target entry
  (EDI=side, EBX=input pointer), but keeping that local alive for later sound
  pan blocks EDI reuse for `timer303A4`; the best such probe is 1,208/1,210
  bytes with 526 differences. Shortening the side lifetime restores the old
  reversed registers; cached-pan and mode-byte variants are worse. Continue
  from the side/input/timer register lifetime, not naive type/pointer rewrites.
- UpdateMovementAndOptions remains 1,900 bytes with four residual bytes at
  +0x127..+0x12A. A sequenced local `GameConfiguration *` snapshot still lets
  VC7.1 emit the player-side load before the global configuration load.
- ReplaySave and EffectManager::AddedCallback remain at two and four residuals
  respectively. Reusing one stage-buffer base across both payload loops grew
  ReplaySave from two to six differences; direct assignment/cursor/memcpy
  variants also retained the two-byte EDI/ECX destination-base residual.
  Callback manager/context aliases did not improve AddedCallback. EnemyManager::OnUpdate remains at its two
  EAX/ECX draw-group index residuals; const locals and typed pointer addition
  did not change the generated code.

## Additional IDA boss captures (2026-11-19)

`UpdateNetworkService` is native `sub_430AA0` (1,633 bytes); `UpdateChargeAttack`
is native `sub_41F810` (1,210 bytes). The current network source is 1,633/1,633
bytes with 307 differences and remains REGRESSED against its 1,629-byte ratchet.
The charge attack remains 1,210/1,210 bytes with 183 differences. New register
lifetime evidence and failed variants are recorded in `KNOWLEDGE_BASE.md`,
including a full-function side-local candidate at 1,200/1,210 bytes and 648
differences. Do not use `register`, volatile aliases, inline assembly or
padding to force the original registers.

## Latest continuation results (2026-11-19)

The two network-service source commits are maintained. All failed ChargeAttack,
collision and SaveReplay experiments from this continuation were reverted.
Detailed measurements are in `KNOWLEDGE_BASE.md`.

- `UpdateChargeAttack`: a full-DWORD side local plus direct input-row calls
  and a post-cooldown reload of `sideIndex` did not retain the target entry
  registers without a lexical phase scope (1,211/1,210 bytes, 816 differences;
  EBX=side, EDI=input). A scoped phase restored EDI=side, EBX=input, then
  reused EBX/EDI for `timer303B0`/`timer303A4` and reloaded `this+8` for the
  later focus check. However, VC7.1 reordered the later attack CFG and reached
  1,202/1,210 bytes with 503 differences. It is not an improvement over the
  maintained 1,210/183 frontier and was reverted. Do not copy it wholesale.
- `CheckBulletCollision`: sharing the direct-hit and cancellation reward
  counter tail while removing the volatile dispatcher pointer produced
  818/862 bytes with 598 differences; retaining the volatile pointer produced
  825/862 with 583 differences. Both preserve the 0x34 frame and entry roles
  but are worse than the maintained 849/862, 539-difference source. The scoped
  carrier replay exercised five other exact units, not this boss. Both were
  reverted.
- `SaveReplay`: expressing the POD destination as a C++ reference leaves the
  two +0xB6/+0xB9 EDI/ECX differences unchanged. The reference form was
  reverted.
- `UpdateNetworkService`: the branch-local unsigned frame plus a separate
  preparation-loop counter now reproduces EBX across both mutually exclusive
  paths. The second prediction reloads the live side and frame counter after
  `GetSeed`; handshake/frame packet receiver lifetimes are restored. The
  current candidate is 1,633/1,633 bytes, 307 differences, 0x14-byte frame.
  It remains REGRESSED because the ratchet is 1,629 bytes. Separate snapshot
  variants at 720/728 differences and 0x18 frame were reverted.
- `CheckBulletCollision`: an explicit circle/dispatcher/rectangle rewrite with
  a shared hit-count tail fell to 811/862 bytes but regressed to 726 differences
  and changed the entry register roles. Reverted; preserve 849/862, 539.
- `SaveReplay`: mutable byte-cursor, typed-cursor and direct source-header
  pointer variants retained the two +0xB6/+0xB9 ECX/EDI residuals. Reverted;
  preserve 1,238/1,238 bytes and two differences.

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
2. UpdateNetworkService: preserve the recovered 0x14-byte frame and EBX
   lifetime: the unsigned frame counter is live across the first prediction
   seed call, while the preparation arm reuses EBX for its signed prefill
   counter. The second prediction reloads the live network side and frame
   counter after the second seed. The handshake and frame packet senders are
   retained across their seed calls. Current candidate: 1,633/1,633 bytes,
   307 differences, still REGRESSED against the 1,629-byte ratchet. Continue
   on the packet-ready/shared-wait block layout and remaining frame queue code;
   do not rebaseline or disturb the EBX/EDI lifetimes.
3. UpdateChargeAttack: solve the side-index/input-pointer lifetime without
   forcing register allocation. The target uses EDI=DWORD sideIndex,
   EBX=input record, then EDI/EBX as timer303A4/timer303B0 and reloads this+8
   for the post-cooldown focus check. A scoped natural-C++ phase recovered all
   those roles but changed the later attack CFG to 1,202/503. Preserve the
   1,210/183 frontier while finding the same lifetimes without changing label
   layout or the later attack branches.
4. EnemyManager::OnUpdate: revisit the remaining EAX/ECX draw-group index only
   after the two large owners have advanced.
5. Close the near-exact ReplaySave, Player movement and Effect callback leaves.

All failed variants outside the committed network-service recovery are
restored. Do not copy diagnostic variants from .analysis into maintained source.
Keep the strict frontier ratchets.

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


## Latest source work and boss probes (2026-11-20)

Eight additional natural-C++ source checkpoints are maintained on local `main`:

- `c7e8a8ac` — recover typed ASCII menu transition-flag reads. The changed
  carrier passed 10 exact units, 4,206 physical bytes and 264 relocation
  fields. Two compiler-private same-section label names changed; both were
  rebound only after checking the actual COFF definitions and their unchanged
  section-relative destinations in the original PE.
- `ff31849d` — replace the HUD enemy-indicator byte-stride wrappers with direct
  indexing of the native `AnmVm` and GameManager side arrays. The changed
  carrier passed its 213-byte owner and all 10 relocation fields.
- `d66e8533` — use direct typed members for the one-off `vm3CBC` and `vm59C8`
  HUD VMs in `FrontSide::OnDraw`.
- `2f753505` — use the direct player-relative `vm7430.pos2` members and native
  VM receiver for its HUD draw, retaining the original `Float3` cursor for
  position writes.
- `1c403728` — remove two now-dead HUD VM cursor assignments.
- `953efe02` — name the native 0x0C-byte `ScreenShakeEnvelopeView` and
  assert its three DWORD member offsets. The changed header passed 21 compiler
  carriers, 100 exact units, 40,016 physical bytes and 2,487 relocation fields;
  79 local COFF labels were rebound only after original-PE destination and
  same-section owner-relative checks.
- `3b391a01` — recover `PlayerShotCtorView::trailSegmentCount46A` at +0x46A,
  shared by the spawn, update and draw callbacks. The affected source scope
  passed three exact units, 1,103 physical bytes and 25 relocations.
- `b22cc6ba` — remove the duplicate sparse shot-draw layout and use the real
  0x484-byte `PlayerShotCtorView`/embedded `PlayerAnmVmView` in
  `PlayerShotDrawCallbackType1`, retaining its original opaque callback ABI.
  The 204-byte owner and all six relocations remain exact.

The complete `FrontSide::OnDraw` owner remains 3,094/3,094 bytes with all 209
relocation fields exact. Its changed-source replay passed one exact unit and
3,094 physical bytes. The `FrontEnemyIndicator` changed-source replay passed
one exact unit, 213 bytes and 10 relocations. The ASCII menu changed-source
replay passed the 10-unit cohort above. No whole-product cold replay was run;
the canonical authored exact-function count remains 1,026.

Focused boss experiments were also continued, but none justified a frontier
update or a source change:

- `CheckBulletCollision`: a nested scan/rectangle/circle structure with the
  dispatcher label inside the radius branch preserved the 0x34-byte frame
  and entry registers, but the best tested candidate was 845/862 bytes with
  584 differences, worse than the maintained 849/862-byte, 539-difference
  candidate. Other label layouts were worse. All probes were reverted.
- `UpdateNetworkService`: splitting the packet-ready wait return from the
  later queue wait, varying the two return expressions, and reusing the
  existing frame-network pointer did not improve the maintained 1,633-byte,
  307-difference candidate. The 1,629-byte ratchet remains untouched.
- `UpdateChargeAttack`: unsigned DWORD side-index variants still produced
  1,209/1,210-byte candidates with 717 differences. The maintained
  1,210-byte, 183-difference source remains unchanged.
- `FrontSide::OnDraw`: converting its shared `Float3` cursor wholesale to an
  `AnmVm *` or introducing a non-inlined owner-recovery helper regressed the
  complete owner. Those variants were discarded; only the direct typed
  single-VM accesses listed above remain.
- `EnemyManager::OnUpdate`: additional array-reference, pointer-to-array,
  pointer-base and inline append-helper forms retained the same two-byte
  EAX/ECX draw-group index residual. All probes were reverted; the maintained
  3,900-byte source remains unchanged.
- `CheckBulletCollision`: target-polarity dispatch, dispatcher-in-circle,
  single-inner-loop, and radius-condition inversion forms were compiled. The
  best new label/loop trial was 848/862 bytes with 569 differences, worse than
  the maintained 849/862-byte, 539-difference candidate. All trials were
  reverted; no frontier was rebaselined.
- `UpdateChargeAttack`: full-DWORD side with scoped pointer/reference aliases
  did not improve the 1,210-byte, 183-difference baseline. A timed-out probe
  left a partial local edit and compiler temporaries; those were explicitly
  restored/removed, and the worktree was verified clean before continuing.

No ASM, inline-assembly, padding or synthetic-byte changes were introduced.

## Continuation after 134cf5e0 (2026-11-20)

The following source checkpoints are now on local `main`; all use the
`gpt-web:` commit prefix and preserve the original callback ABI tags:

- `3b391a01` names the shot +0x46A trail segment count, shared by the exact
  Type1 spawn/update/draw callbacks.
- `b22cc6ba` and `2e9756b5` remove the duplicate sparse shot layouts from both
  shot draw callbacks. Both now use the complete 0x484-byte
  `PlayerShotCtorView` and its embedded `PlayerAnmVmView`; the 204-byte and
  293-byte owners remain exact.
- `c7593bb3` shares the native shot/SHT records across spawn Type2/4/5/6 and
  update Type5/6/7; all seven callbacks remain exact.
- `55992dee` removes the duplicate 0x484-byte shot and 0x38-byte descriptor
  definitions from `PlayerShotInitializeView.hpp`, retaining only the
  original forward-declared ABI tags. Its selected replay passed seven
  carriers, nine units, 1,025 bytes and 22 relocations.
- `3897723b` names shot +0x470 as `mode470` and +0x472 as `flag472`; the
  selected replay passed 14 carriers, 17 units, 2,886 bytes and 68 relocations.
- `93027eb2` recovers Player +0x30354 as `shotCollisionCount30354` and removes
  the callback-only sparse Player prefix. Both shot collision callbacks
  remain exact; selected replay passed 19 bytes.
- `fa62f431` removes the duplicate sparse Player/side views from spawn Type4
  and Type5; both use `PlayerLifecycleView` and its native side state. The
  three callbacks in that source remain exact; selected replay passed 184
  bytes and four relocations.
- `eb752fdd` also moves spawn Type6 to the full native Player owner and
  replaces its sparse effect-position view with `Effect::position`, while
  retaining the callback-specific manager/return ABI aliases. `f8767b8f` and
  `07918611` type the fixed-slot effect pools as native `Effect *` and recover
  the standalone accessor's native storage. The five affected exact functions
  passed 329 bytes and six relocation fields across three carriers.

The authored exact-function count remains **1,026**. No whole-product cold
replay was run. Basic CI passes and the worktree is clean; local `main` remains
ahead of `origin/main`. No push was performed.

### Boss continuation results

- `EnemyManager::OnUpdate` remains 3,900/3,900 bytes with two EAX/ECX index
  register differences. Function-scope index types, pointer/reference
  aliases, array-reference forms and append helpers did not improve it.
- `CheckBulletCollision` remains 849/862 bytes with 539 differing positions
  and is still REGRESSED against the strict 842-byte/705-difference ratchet.
  A new IDA-CFG reconstruction tried the circle-first, shared-dispatch,
  rectangle-after layout, including the native double-zero radius comparison
  and loop-back labels. The best new candidate was 804/862 bytes with 712
  differences; other variants were worse. All probes were reverted. The
  maintained source and frontier manifest are unchanged.
- `UpdateNetworkService` remains 1,633/1,633 bytes with 307 differences and
  REGRESSED against the 1,629-byte ratchet. Splitting the packet-ready return
  from the later frame-queue wait did not improve its code layout; all trials
  were reverted.
- `DrawReplayMenu` remains 1,280/1,280 bytes with 29 differences. Reordering
  selected-index/replay-view locals regressed to 1,264 bytes and 694
  differences; those variants were reverted.
- `UpdateChargeAttack` remains 1,210/1,210 bytes with 183 differences. The
  widened side-index/pointer variants still regress the function and were
  reverted.

Do not promote any of these boss candidates or rebaseline their frontiers.
Continue to work from the current natural C++ source and target CFG; no ASM,
inline assembly, register hints, padding or synthetic bytes were introduced.


## Continuation after 5e79811e (2026-11-20)

- 5e79811e removes the sparse Player and Side records from
  EffectFollowPlayerUpdate; the 38-byte callback remains exact and its
  changed-source replay passed.
- The following ECL helper checkpoint replaces the unused private
  EclHelperEnemyView layout with compile-time checks against the shared
  EnemyView. InterpolateLinear and InterpolateHermite remain exact at
  74 and 188 bytes; their changed-source replay passed 262 physical bytes
  and 12 relocations.
- EffectUpdateCallbackType1Type2 already reads the random-axes side's
  character through AsciiGameManagerSideView; it remains exact at 775 bytes
  and 33 relocation fields.
- Boss priorities remain EnemyManager::OnUpdate (3,900 bytes, two
  register-selection differences), CheckBulletCollision (maintained source
  is still REGRESSED against the 842-byte ratchet), and
  UpdateNetworkService (1,633-byte candidate, REGRESSED against 1,629).
  Continue from target CFG and natural C++; do not rebaseline or use ASM,
  register forcing, padding or synthetic bytes.


## Continuation after 3738f9cc (2026-11-20)

- The Effect byte at +0xC9 is now named drawFlag0C9 across the shared Effect
  owner and all ring/fan/strip initializers, update callbacks and draw
  callbacks. Its offset is asserted; its set/test/clear behavior is supported
  by independent callback consumers.
- The changed EclManager.cpp object regenerated 202 compiler-private local
  COFF label names across CompareOperands, EnemyPostEclUpdateMovement and
  RunEcl. Each replacement was verified as an owner-local symbol at the same
  original target address; the canonical relocation checker then passed.
- Selected replay of EffectManager.cpp, EclManager.cpp, EffectDrawStrips.cpp
  and EffectPlayerStripFamily.cpp passed five carriers, 104 exact units,
  49,253 bytes and 2,148 relocations. Basic CI passed; no cold replay ran.
- Exact authored function credit remains 1,026. Continue to prioritize the
  three nonexact gameplay bosses in the table above; do not promote or
  rebaseline REGRESSED candidates.


## Continuation after 8b4a1c60 (2026-11-20)

- EnemyManager::OnUpdate was rebuilt after the Effect header changes and
  remains 3,900/3,900 bytes with the same two draw-group register differences
  at +0x19A/+0x1A1. Branch-local pointer/reference/array aliases and explicit
  index casts did not alter the residual.
- CheckBulletCollision cursor-home experiments were reverted. Removing the
  redundant entry home store yielded 837/862 bytes with 706 differences and
  105 new offsets; making the scratch union the only scan cursor yielded
  830/862 bytes with 711 differences and 113 new offsets. The maintained
  849/862, 539-difference source remains REGRESSED against the 842-byte
  ratchet. Do not rebaseline.
- UpdateNetworkService remains 1,633/1,633 bytes with 307 differences and
  REGRESSED against the 1,629-byte ratchet.
- All experiments above were restored. The three boss frontiers were checked
  individually; no full cold replay ran. The Effect draw-flag commit remains
  the latest source change and Basic CI passed.

## Continuation after collision-region shared-owner work (2026-11-20)

- `PlayerCollisionRegionCreateRegionView::collisionDelay40` now names the
  allocator's +0x40 delay field used by both bullet-collision and
  shot-to-enemy-damage filtering. All three consumers use the same field.
- `PlayerShortLeaves.cpp` removed its duplicate sparse 0x44-byte collision
  region layout. `Deactivate` is now a direct member of the complete
  `PlayerCollisionRegionCreateRegionView`; its exact-unit symbol binding was
  updated to that canonical class. The five-byte leaf remains exact.
- Selected replay passed 19 exact units, six compiler jobs, 2,799 bytes and
  68 relocations. No full cold replay ran. `CheckBulletCollision` remains a
  separate non-exact boss frontier; do not count it as covered by that replay
  or rebaseline its strict ratchet.

## Boss continuation notes (2026-11-20)

- CheckBulletCollision was re-decompiled at 0x41DFF0. Preserve its native
  0x34 frame and EDI/ESI/EBX roles; the next source attempt should target the
  nested filter loop and the circle -> shared dispatcher -> rectangle CFG.
  The best fresh natural-C++ candidate was 845/862 bytes / 581 differences,
  worse than the maintained 849/862 / 539 frontier, so all experiments were
  reverted. Do not rebaseline.
- EnemyManager::OnUpdate remains 3,900/3,900 bytes with only the two
  +0x19C/+0x1A3 MOVZX/LEA register choices remaining. Several natural pointer,
  index, helper and lifetime variants were tried and reverted.
- The collision-region owner and Deactivate leaf changes are committed
  separately and their 19-unit selected replay passed. Current exact count
  remains 1,026; no full cold replay ran.

## Player collision owner continuation (2026-11-20)

- UpdatePlayerCollisionRegions now uses PlayerLifecycleView's actual embedded
  region, active/released arrays and counters, all typed as the shared complete
  PlayerCollisionRegionCreateRegionView. Removed both sparse local overlays.
- Its old geometry slots are now named radius/radiusGrowth/halfWidth/
  halfHeight/widthGrowth/heightGrowth, matching the allocator and collision
  consumers. Delay, lifetime and active are the same +0x40/+0x24/+0x3C fields.
- The updater remains exact at 273 bytes. Selected replay passed 21 exact units,
  five compiler jobs, 5,391 bytes and 288 relocations. Eleven regenerated
  PlayerRuntime.cpp local-label spellings were checked against identical
  owner-relative addresses before updating the manifest.
- No full cold replay ran; the separate CheckBulletCollision and
  UpdateNetworkService boss frontiers remain non-exact and are not covered by
  this replay.

## Player input owner continuation (2026-11-20)

- UpdatePlayerBombState now uses ReplayInputState::IsHeld(2) and
  g_ReplayInputStates instead of the duplicate PlayerInputRuntimeView /
  g_PlayerInputStates overlay. Both target relocations bind to the canonical
  ReplayInputState names at the same original addresses.
- The 11 regenerated PlayerRuntime.cpp local COFF labels were checked against
  unchanged owner-relative target addresses and rebound in the manifest.
- Five selected compiler carriers passed 21 exact units, 5,391 bytes and 288
  relocation fields. No full cold replay ran.

## Boss probe checkpoint after shared Player owners (2026-11-20)

- UpdateNetworkService remains at its maintained 1,633-byte candidate with
  307 differing bytes (strict 1,629-byte ratchet). Natural-C++ probes that
  split the packet-ready early return from the later frame-queue wait, and
  that reuse the long-lived frameNetwork pointer at the packet-ready edge,
  rebuilt to the same 1,633/307 result. The target still branches from the
  +0x3F8 packetReady test to +0x422 input handling, while the candidate's
  return block remains out of line. All probe source was restored.
- UpdateChargeAttack remains 1,210 bytes / 183 differing bytes. Widening the
  side local, moving the ReplayInputState pointer to sideIndex, changing the
  pointer to a reference, consolidating the mode snapshot and narrowing its
  scope were all tested as natural C++ variants. Candidates ranged from
  1,209/717 to 1,215/900 and did not improve the maintained source; all were
  restored. The field sideIndex itself is a signed 32-bit member at +0x08.
- The committed PlayerRuntime owner cleanup and ReplayInputState reuse are
  independent exact improvements: 21 selected exact units passed after the
  PlayerRuntime rebuild. Do not confuse those exact consumer results with
  the two still-nonexact boss frontiers above.

## Player bomb-state owner consolidation (2026-11-20)

- Removed PlayerBombTargetRuntimeView and PlayerBombOwnerRuntimeView.
  UpdatePlayerBombState now calls the typed side-state EnemyManagerView's
  FindActiveEnemyBySideCategory(3) and CardAttack::StartAttack directly.
  This matches the same canonical call owners already used by charge,
  death and reward code.
- Updated the five method relocation bindings in player-update-bomb-state
  to the canonical exact symbols; both destinations are unchanged. The
  335-byte function remains exact.
- Selected PlayerRuntime.cpp replay passed 21 exact units, five compiler
  carriers, 5,391 bytes and 288 relocations. Eleven regenerated local-label
  spellings in UpdateBeforeState were address-checked before rebinding.

- Additional CheckBulletCollision probe: replacing the rotated-rectangle
  half-size reads through (*slot) with the already loaded region pointer
  changed the candidate to 856/862 bytes and 708 differing positions
  (110 new, 107 closed relative to the recorded 705-byte residual). This
  worsened the maintained 849/862 / 539-difference frontier and was reverted.
  The repeated slot dereference is retained because it participates in the
  native register/lifetime shape; do not simplify it without a better CFG fit.

## Player collision timer owner consolidation (2026-11-20)

- Removed PlayerCollisionTimerView from PlayerCollisionQuery.cpp. The native
  timer field is already PlayerTimerCtorView/ZunTimer; FindCollision now uses
  its exact operator> comparison directly.
- Rebound the call at 0x00403DE0 to the canonical decorated ZunTimer operator>
  symbol. The existing 20-byte ZunTimer::operator> leaf remains independently
  exact.
- Selected PlayerCollisionQuery.cpp replay passed FindCollision (602 bytes)
  and CalcItemBoxCollision (121 bytes), 723 bytes / nine relocations total.
  No full cold replay ran.

## Player timer getter consolidation (2026-11-20)

- Removed PlayerState4TimerCurrentView from PlayerRuntime.cpp. Its six getter
  calls now use the existing ZunTimer conversion operator on timer303C8.
- Updated six relocations across PlayerCheckState4, PlayerTransitionSpecial
  and PlayerUpdateCommon to the canonical exact ZunTimer::operator int symbol
  at 0x00435F00. The function bytes remain exact.
- Five selected PlayerRuntime carriers passed 21 exact units, 5,391 bytes
  and 288 relocations. Eleven regenerated local labels in UpdateBeforeState
  were verified by unchanged owner-relative addresses before rebinding.
  No full cold replay ran.

## Player FrontSide owner consolidation (2026-11-20)

- Removed PlayerFrontSideRuntimeView. Player runtime now calls the real
  FrontSide owner through sideState->frontSide18 for ClearState,
  UpdateMeterHundreds and SetPatternTiming.
- The old SetTimingValue call at 0x0041A3E0 is the already exact
  int-returning FrontSide::SetPatternTiming function, not a separate wrapper.
- Selected PlayerRuntime replay passed 21 exact units, five compiler
  carriers, 5,391 bytes and 288 relocations. Eleven regenerated local labels
  were verified at unchanged owner-relative destinations. No full cold replay.

## Player timing metric owner consolidation (2026-11-20)

- Removed PlayerMetricRuntimeView. The opponent's header24.timer5C is already
  the complete ZunTimer member at Player header +0x5C / Player +0x80.
  Player::OnUpdate now reads it through a typed ZunTimer* and its existing
  int conversion operator.
- Updated the +0x18A call relocation to canonical ZunTimer::operator int.
  The first direct-member spelling altered three caller bytes; a natural
  ZunTimer* local restores exact 522/522 Player::OnUpdate code.
- Selected PlayerRuntime replay passed 21 exact units, five carriers,
  5,391 bytes and 288 relocations. Eleven regenerated local labels were
  address-verified before rebinding. No full cold replay ran.

## Player ANM blend owner consolidation (2026-11-20)

- Removed PlayerRespawnBlendView, unused PlayerRespawnVmView and the separate
  sparse PlayerAnmVmBlendModeView layout. The blend methods are now members
  of the canonical PlayerAnmVmView / AnmVm storage and operate on flagsWord
  at +0x1F8.
- PlayerTransitionSpecial calls mainVm directly. The two exact leaf symbols
  and both caller relocations now name PlayerAnmVmView.
- Selected replay of PlayerRuntime.cpp and PlayerShortLeaves.cpp passed 25
  exact units, six compiler carriers, 5,442 bytes and 289 relocations.
  Eleven regenerated local labels were verified at unchanged destinations.
  No full cold replay ran.

## Player GameManager owner consolidation (2026-11-20)

- Removed PlayerGameManagerRuntimeView and the duplicate
  g_PlayerGameManagerRuntime global. PlayerRuntime now uses the existing
  g_GameManager / AsciiGameManagerView for IsGameMode1, transforms and
  IsWithinPlayfield.
- The former CheckTimingState is the exact IsGameMode1 leaf at 0x00404970.
  The suppressPlayerDraw13D byte aliases the existing +0x13D GameManager
  field, now with an explicit offset assertion.
- Rebound all PlayerRuntime references to canonical global/method symbols.
  Selected replay passed 21 exact units, five carriers, 5,391 bytes and
  288 relocations. Eleven local labels were verified at unchanged targets.
  No full cold replay ran.

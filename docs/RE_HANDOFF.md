# TH09 reconstruction — next-agent handoff

**Start here.** Read [AGENTS.md](../AGENTS.md), the
[bounded workflow](RE_WORKFLOW.md) and [Oracle rules](ORACLES.md).
The [knowledge base](KNOWLEDGE_BASE.md) records target layouts and useful
failed approaches; old trial results are history, not current codegen proof.
Git retains earlier source revisions.

## Verified baseline and boundaries

- **Target:** original Japanese TH09 **v1.50a**, Windows i386, base
  `0x00400000`, ignored `resources/th09.exe`, SHA-256
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  Never patch or delete it.
- **Last verified 2026-11-11:** **1,025 / 1,037** source-present functions
  byte-exact; **12 nonexact / 28,145 original bytes**. Another 35 candidate
  origins remain reviewed but unresolved; inventory completeness is unproven.
  These are *snapshots*, not a new agent's live proof: regenerate totals with
  `python3 -B scripts/report-reconstruction-status.py`. Canonical authority
  is `config/matches.csv`, `config/functions.csv` and
  `config/match-units.toml`, not manually copied status prose.
- **Closed large owners:** `GameplaySetupThread @ 0x0041AF2D` is exact
  (1,689 bytes / 172 relocations). `Title DrawResult` is also exact
  (1,939 bytes); do **not** confuse it with nonexact `OnUpdateResult`.
- **Phase:** source exactness remains open. No complete Windows i386 link
  and runtime closure, dual-Oracle semantic baseline or port is proven.
- **Current source:** native Win32 32-bit address math and word-wrap
  semantics have been recovered across Enemy/ECL, Player/collision, Replay,
  Title, Effect, Sound, ANM, ExAttack, and Supervisor frame queues.
  Replay BeginRecordingStage now also uses native signed-stage strides for
  three frame-pointer rows, the FPS row and inline 0x18-byte buffer links;
  the two Replay focus loops now address their original 0x8E-byte side-state
  records by native DWORD side strides. EffectManager's nine indexed
  template fields now use the real table's native 0x18-byte DWORD stride;
  its 15 exact owners (2,530 bytes / 100 fields) remain cold-verified.
  ScreenEffect's shake side/camera indices now use native 0x38/0xF0-byte
  strides; all 18 same-source exact owners (2,562 bytes / 164 fields) pass,
  with nine verified VC7.1 private-label name rebindings in RegisterChain.
  The 1,323-byte Replay stage initializer and all 12 ReplayInput exact units
  (4,093 bytes / 232 fields) also remain verified. The exact 151-byte Enemy
  attached-effect updater now computes the real 24 pointer slots by native
  DWORD addressing; both affected ECL compiler carriers protect 83 exact
  units (29,856 bytes / 1,389 fields) and preserve both large Enemy/ECL
  nonexact frontiers.
  These patches preserved exact siblings but **did not** promote any of the twelve
  open functions. Use `git log --oneline` for the actual commits; do not
  restore scratch files as if they were accepted changes.

## Remaining source-present nonexact owners

Each row describes the **complete original PE owner**, not partial exact
credit. These are the last reviewed source frontiers; recompile the chosen
owner after editing it. `--owner` routes refer to
`scripts/check-boss-frontiers.py` unless another inspector is specified.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | 15,564/15,564 bytes, 598 fields; 30 unlike | Opcode 4 timer register scheduling (7), opcode 155 flag write (23). Original owned tables intact. `--owner runecl`. |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900 bytes, 97 fields, 69 calls; 13 unlike | Draw-index and bullet-descriptor register lifetime. `--owner enemy`. |
| **Player movement** | 0x0041C170 | 1,900/1,900 bytes, 66 fields, 23 calls; 122 unlike | Option/index scheduling and Float3/history transport. `--owner movement`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210 bytes, 70 fields, 58 calls; 235 unlike | Mode byte and input receiver; 53-block CFG retained. `--owner charge`. |
| **Title OnUpdateResult** | 0x004266B5 | 484/485 bytes, 34 fields, 9 calls; 218 unlike overlaps, 1 missing | Character byte load/sign extension. **Use `inspect-title-update-result.py`**, not `--owner resultdraw` (which checks the already-exact DrawResult). |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280 bytes, 57 fields, 13 calls; 29 unlike | Selected record EDX/ECX lifetime. `--owner replaymenu`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | 849/862 bytes, 26 fields, 12 calls; 799 unlike overlaps, 13 missing | Original 33 CFG blocks vs candidate 32; needs a real branch. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238 bytes, 53 fields, 23 calls; 2 unlike | First frame-header copy uses EDI vs ECX at +0xB7/+0xBB. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | 538/540 bytes, 32 fields, 5 calls; 39 unlike overlaps, 2 missing | Seed-load/input-delay tail. `--owner replay`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336 bytes, 20 fields, 8 calls; 26 unlike | ESI/EDI timer/owner lifetime. `--owner screen`. |
| **Supervisor network service** | 0x00430AA0 | 1,627/1,633 bytes, 106 fields; 1,381 unlike overlaps, 6 missing | Waiting/prediction block layout. Compile `SupervisorNetwork.cpp` and use `inspect-supervisor-service.py`; `--owner network` tests other owners. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51 bytes, 4 fields, 2 calls; 8 unlike | ANM manager/owner EDI/ESI schedule. Keep exact Create caller. `--owner effect`. |

## Resume with only affected compiler carriers

```bash
git status --short --branch         # recover all staged/dirty/untracked work
git log -6 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

# One original-bound nonexact owner (not a whole-game replay):
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner runecl --skip-exact-siblings

# Only exact siblings affected by an edited .cpp/.inl/.hpp:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl --dry-run
nice -n 12 python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl

# One canonical exact owner, only when relevant:
nice -n 12 python3 -B scripts/build-match-unit.py --unit ecl-pop-context
python3 -B scripts/compare-coff-function.py --unit ecl-pop-context --json

# Correct Title OnUpdateResult path (resultdraw is DIFFERENT):
nice -n 12 python3 -B scripts/build-match-unit.py --unit title-screen-setup-thread
python3 -B scripts/inspect-title-update-result.py build/matching/TitleScreen.obj

# Network service: standalone source, no exact siblings in that .cpp:
# The generated build/matching directory may be absent after cleanup.
mkdir -p build/matching
nice -n 12 scripts/compile-probe.sh src/SupervisorNetwork.cpp \
  build/matching/SupervisorNetwork.obj \
  /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src
python3 -B scripts/inspect-supervisor-service.py build/matching/SupervisorNetwork.obj

# Documentation/ledger checks (no game compiler required):
python3 -B scripts/validate-docs.py
python3 -B scripts/ci.py
git diff --check
```

The scoped exact replay **does not** accept a nonexact boss; run its
complete original-bound inspector separately. Some standalone nonexact
sources have no canonical exact unit and correctly cannot run scoped
exact replay. Exact acceptance covers **complete physical code, tails,
owned switch tables and all original-bound relocations**, not a partial
code slice or a similarity score. Do not use `--owner all`, an all-source
cold replay or `ci.py --full` for routine iteration. Do not change the
pinned VC7.1 profile or add inert register hints to force a match.

## Best next questions; avoid repeating failed experiments

Work on large bosses **and** small leaves. Previous changes to natural
32-bit addresses and signed wrap are already included in maintained source.

- **RunEcl:** opcode 4 has seven register bytes; opcode 155's byte-to-bit-24
  XOR/write has 23. Fields/tables are already right. Pointer aliases,
  bitfields, union operands, timer receivers and source-order variations
  generally preserved 30 differences or broke table targets. See the
  [opcode notes](KNOWLEDGE_BASE.md); don't replay cosmetic rewrites.
- **Enemy:** 13 specific register bytes remain in draw-group and
  spawn-descriptor construction. Avoid improving a local register at
  the expense of original literal relocations.
- **SaveReplay:** only +0xB7/+0xBB differ; frame-header constness,
  natural dest arithmetic, typed copies and moving link initialization
  have not solved the EDI/ECX choice. Preserve the 23 calls.
- **Player collision:** one genuine original CFG block and 13 bytes
  are missing; manufactured `goto`, `ret` or padding is not recovery.
- **Effect callback:** eight receiver/manager register bytes remain;
  pointer/ref/inline wrappers seldom change them. Its Create caller
  remains exact.
- **Supervisor network service:** the retail owner has 1,633 bytes versus
  the 1,627-byte candidate, with 106 relocations and 41 direct calls but a
  different wait/success block schedule. Reversing branches or sharing an
  exit did not restore the original layout; do not mistake fewer overlapping
  differences in a longer candidate for a strict improvement.
- **Title OnUpdateResult / DrawReplayMenu:** distinguish them from the exact
  DrawResult leaf. The result updater needs its actual signed character-byte
  read and one missing byte; replay menu has 29 register-lifetime differences.
  Pure pointer casts and cached source-index snapshots have not fixed either.

The pinned neighboring `_reference/Cirno-Labs-th09`, TH08 and TH095
code are hypothesis sources, **not** original TH09 evidence.

## Protected inputs and scratch lifecycle

The obsolete ignored `build/` cache was removed for this handoff,
including `build/gpt-web/` trial copies/PDBs/diagnostics and
`build/matching/` compiled objects. **The absence of an object is expected**:
use `scripts/build-match-unit.py --unit <unit-id>` for a selected exact
owner or `scripts/replay-scoped-exact.py --changed-file <source>` for its
*affected* exact cohorts. For a standalone nonexact probe, recreate
`build/matching/` and use `scripts/compile-probe.sh` with the pinned profile
above. Do not run project-wide `--owner all` or cold replay to rebuild caches.
Removing generated objects does not revoke existing ledger matches, nor does
an old receipt prove the current source without recompilation.

**Preserve** ignored `resources/th09.exe`, `.tools/wine/` (the game-specific
VC7.1/Wine runtime and prefix), `.analysis/ghidra-direct/` (saved Ghidra
projects), `.analysis/factory-runtime/`, shared Factory tools and
`_reference/`. The six small ignored
`.analysis/gpt-web/20261111-prove-*.py` scripts are retained *only in this
live worktree* as optional private-label same-section PE/COFF proof helpers;
they are not canonical evidence and require fresh selected objects first.
Do not delete these protected inputs to save space, or commit originals,
SDKs, executable assets or analysis databases. No remote push was requested.
See [tool routing](TOOLS.md) and [shared runtimes](SHARED_TOOLS.md).

Commit real improvements as `gpt-web: ...`. Always inspect and recover
dirty/untracked work instead of deleting it. Check live Git ahead/behind
status; do not carry forward an old count.

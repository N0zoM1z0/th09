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
- **At this handoff:** **1,025 / 1,037** source-present authored functions
  byte-exact; **12 nonexact / 28,145 original bytes**. Another 35 candidate
  origins remain reviewed but unresolved; inventory completeness is unproven.
  Regenerate totals with `python3 -B scripts/report-reconstruction-status.py`.
  Canonical authority is `config/matches.csv`, `config/functions.csv` and
  `config/match-units.toml`, not manually copied status prose.
- **Closed large owners:** `GameplaySetupThread @ 0x0041AF2D` is exact
  (1,689 bytes / 172 relocations). `Title DrawResult` is also exact
  (1,939 bytes); do **not** confuse it with nonexact `OnUpdateResult`.
- **Phase:** source exactness remains open. No complete Windows i386 link
  and runtime closure, dual-Oracle semantic baseline or port is proven.
- **Recent source:** native Win32 32-bit address math and word-wrap semantics
  have been recovered across Enemy/ECL, Player/collision, Replay, Title,
  Effect, Sound and ANM. They preserve exact siblings but have **not**
  promoted any of the twelve open functions. See Git for individual changes.

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

The immutable neighboring `_reference/Cirno-Labs-th09`, TH08 and TH095
code are hypothesis sources, **not** original TH09 evidence.

## Protected inputs and scratch lifecycle

Ignored `build/` is rebuildable. Historical `build/gpt-web/` source copies,
PDBs, objects and diagnostics and the old `build/matching/` cache were
cleared for this handoff. Rebuild selected exact owners from the maintained
source and canonical manifests, not old object receipts.

**Preserve** ignored `resources/th09.exe`, `.tools/wine/` (private compiler
and Wine prefix), `.analysis/ghidra-direct/` (saved Ghidra projects),
`.analysis/factory-runtime/`, shared Factory tools, and `_reference/`.
Six small, ignored `.analysis/gpt-web/20261111-prove-*.py` PE/COFF
private-label proof helpers were retained **in this live worktree only**.
They require rebuilding their selected `build/matching/` objects first;
they are not canonical evidence or part of the tracked source.
Do not commit originals, SDKs, executable assets or analysis databases,
and do not push without a request. See [tool routing](TOOLS.md) and
[shared runtimes](SHARED_TOOLS.md).

Commit real improvements as `gpt-web: ...`. Always inspect and recover
dirty/untracked work instead of deleting it. Check live Git ahead/behind
status; do not carry forward an old count.

# TH09 reconstruction — current agent handoff (2026-11-17)

**Read first:** [AGENTS.md](../AGENTS.md) for operator rules,
[RE_WORKFLOW.md](RE_WORKFLOW.md) for the bounded reconstruction loop,
[ORACLES.md](ORACLES.md) for source/target evidence, and
[PROGRESS.md](PROGRESS.md) for generated statistics. This handoff describes
**current maintained source**; dated entries in
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) are historical observations, not
instructions to restore previous source or old scratch objects.

## First five minutes

~~~sh
git status --short --branch                # inspect and continue any dirty work
git log -6 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/ci.py                   # syntax, ledgers, docs, progress
~~~

Do not discard uncommitted changes or run an all-owner cold replay. Use the
registered live `th09` repository/worktree, not copies of source from old
experiments. Commit coherent changes as `gpt-web: ...`; never push silently.

## Authoritative target and phase

- **Only target:** original Japanese TH09 v1.50a,
  `resources/th09.exe`, Windows PE32 i386, image base `0x00400000`,
  entry `0x0047D45F`. SHA-256:
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  This ignored binary is protected: never patch, rename, commit or delete it.
- **Canonical status on 2026-11-17:** 2,248 provisional function candidates;
  35 reviewed but origin-unresolved; 1,037 confirmed authored mappings
  and source-present owners; **1,026 exact, 11 source-present nonexact
  (13,353 logical authored bytes)**. The authored-byte measure is
  **278,578 / 291,931 = 95.4260%**. It is *not* a whole-executable
  exactness percentage or proof of complete original-code inventory.
  Re-read `scripts/report-reconstruction-status.py` after any ledger change.
- **Native product:** `config/build.toml` remains a skeleton with
  unknown translation-unit and link order. Whole Windows i386 build and
  runtime validation are **open**; semantic dual-Oracle work and port
  stages have not started.
- **Exactly measured evidence:** `config/matches.csv` lists accepted
  authored exact functions; `config/match-units.toml` defines their pinned
  VC7.1 source/profile, full owned physical extent and symbolic relocation
  identities; `config/frontiers.toml` sets monotone nonexact baselines.
  An exact byte count, a matching Ghidra decompilation, or an old object
  alone is not enough to promote a function.

## 2026-11-17 GPT-web source checkpoints (native C++, no new exact claims)

Five local source commits, all `gpt-web: ...`, have been saved after
strictly scoped VC7.1 compilation/relocation checks:

- `bdd34f42`: `FrontSide::OnDraw` traverses its first 11 genuine HUD
  `AnmVm` records with an `AnmVm *`, not a `Float3 *` interior-offset
  cursor. The full 3,094-byte boss remains exact (209 relocations).
- `ba48f475`: all five `EffectManager` pool cursor advances now use
  `++effect`, covering reset, spawn, velocity-spawn, update and release.
- `a4dc57a1`: the three `EffectManager` spawn owners use their real
  `effects[index]` array instead of manually scaling integer addresses.
  Across both Effect commits, all 15 exact same-source owners remain exact
  (2,530 physical bytes; 100 relocations), and the `AddedCallback`
  frontier remains at four differing bytes.
- `4d977749`: the three fog-blend branches in the 3,274-byte
  `Background::RenderObjects` boss read/write the existing packed BGRA
  word through named `.b/.g/.r/.a` lanes, retaining all 113 relocations.
- `78abe86d`: the 2,803-byte Title key-config boss directly increments
  its three native counters (`++screenFrameCounter`, `++stateTimer`,
  `++stateTimer2`). The 106-byte co-resident leaf remains exact;
  118 relocations were checked.

These four selected sources passed a combined **19 exact owners, 11,807
physical bytes and 540 relocation fields**. `scripts/ci.py` also passed.
The ledger still reports **1,026 exact**; the work improves original-source
structure without claiming newly exact functions or whole-image closure.

**Targeted negative experiments:** ten `SaveReplay` frame-header copy forms
preserved the same two EDI/ECX bytes; the tested Effect added-callback and
Player movement expression variants did not reduce their register residuals.
Replacing `AnmManager::ExecuteScript` wire-payload casts or its signed
counter increment shifted compiler-private jump-table relocation identities,
so none were retained. In the `FrontMessageRuntimeView::Update` boss,
native side-index expressions and a direct frame-counter increment similarly
shifted compiler-label identities. Converting the remaining Front HUD
position cursors to independent `AnmVm *` loop variables changed the exact
boss's code/relocation schedule; only the verified first group was kept.
All temporary experiments were restored, without ASM, NOP padding,
manifest-label rebinding, artificial frontier updates, full cold replay,
or changes to protected original binaries.

## Current nonexact work queue

The numbers below were rebuilt with the owner's recorded compiler profile
and `scripts/check-frontier.py` on 2026-11-17. They are **relocation-masked
differing byte positions**, not counts of differing instructions. The
instruction starting address can precede the differing byte offset.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **enemy-on-update (EclManager.cpp)** | 0x00410730 | 3,900/3,900, **2 masked bytes** | At `+0x19C` and `+0x1A3`, VC7.1 uses ECX where retail uses EAX to load/index the early draw-head group. Typed schedule/pool/group access is already in source. The 84 co-resident ECL exact functions must remain intact. |
| **replay-save (ReplayInput.cpp)** | 0x00420C60 | 1,238/1,238, **2 masked bytes** | `+0xB7` and `+0xBB`: header-copy destination register ECX instead of retail EDI. |
| **replay-begin-playback (ReplayInput.cpp)** | 0x00420840 | 540/540, **13 masked bytes** | Replay game-side restoration load order around `+0x14B–0x15F`. |
| **player-movement (PlayerMovement.cpp)** | 0x0041C170 | 1,900/1,900, **4 masked bytes** | Config/side load order at `+0x127–0x12A`. |
| **player-charge-attack (PlayerChargeAttackUpdate.cpp)** | 0x0041F810 | 1,210/1,210, **183 masked bytes** | Side/input/short state lifetimes and EDI/EBX allocation; keep real ReplayInputState and configuration ownership. |
| **player-bullet-collision (PlayerCollisionHelpers.cpp)** | 0x0041DFF0 | 842/862, **705 masked bytes** | Collision/loop CFG, entry preheader and four distinct backedges; no ASM or fake alignment. |
| **effect-added-callback (EffectManager.cpp)** | 0x0040D1C0 | 51/51, **4 masked bytes** | EDI global ANM manager vs ESI callback receiver setup order. |
| **screen-shake-envelope (ScreenEffect.cpp)** | 0x004227A0 | 336/336, **26 masked bytes** | Volatile timer/receiver lifetimes and ESI/EDI selection. |
| **title-update-result (TitleScreen.cpp)** | 0x004266B5 | 484/485, **167 masked bytes** | Control flow / signed character load at result state transition; note the candidate is one byte shorter. |
| **title-draw-replay-menu (TitleScreenReplayMenuDraw.cpp)** | 0x004234F6 | 1,280/1,280, **29 masked bytes** | Register assignment around replay-record and menu selection paths. |
| **supervisor-network-service (SupervisorNetwork.cpp)** | 0x00430AA0 | 1,629/1,633, **966 masked bytes** | Larger host/network branch CFG, 20-byte stack frame and network receiver lifetime. The smaller scratch CFG is *not* ratchet-safe. |

**Additional frontier, not one of the eleven authored nonexact functions:**
`etama-fsincos` at `0x00412780`, `src/EtamaControllerDraw.cpp`:
maintained **36 / 33 bytes, 25 differing positions**.
Original authored-versus-library provenance remains *unclassified*.
The previous x87/FSINCOS inline-assembly source once produced exact bytes,
but it was deliberately replaced with natural `cosf/sinf` C++.
Its obsolete standalone match-unit was removed on 2026-11-17;
do not restore that exact claim, add inline ASM, or assume numerically
identical last-bit rounding.

The frontier file is authoritative for addresses, profile flags, byte
positions, and SHA-256 of each original extent. `check-frontier.py`
checks the monotone ratchet; it **does not award exact credit**.

## High-value next investigations

1. **Real boss progress:** restore `CheckBulletCollision`'s original
   early circle-hit dispatch/backedges in natural C++ and compare every
   exact Player sibling. A previous scratch top-tested-loop variant
   fixed some old offsets but introduced new ones; do not promote it.
   `SupervisorNetworkService` likewise needs the correct first
   network-pointer EDI lifetime and host-session branch placement.
2. **Two-byte blockers:** trace C2's early Enemy draw-group virtual
   register choice (vreg 7; equal-score descriptors 1/2) rather than
   repeating register hints. For `SaveReplay`, analyze the typed
   frame-header copy and temp-buffer lifetime. Both remained at two
   differences after earlier simple pointer/declaration sweeps.
3. **Small leaves / related bosses:** `EffectManager::AddedCallback`,
   `ScreenEffect::CalcShakeEnvelope`, Replay playback, Player movement,
   Title result update and replay menu all have bounded frontiers.
   Recheck both source siblings and the protected residual before
   keeping a variant.
4. **Origin and product closure:** resolve the 35 outstanding origin
   cases separately; establish real translation units, link order,
   resources, build/runtime tests. Do not equate per-function Exact
   with a runnable native product.

Recent natural-C++ refactoring has already restored real
GameManager/Player/Enemy/ANM/Supervisor object ownership, indexed
arrays and file-backed byte pointers in large exact bosses. Check
**maintained source and Git history** before repeating this work;
a private symbol spelling change without PE/COFF proof is not progress.

## Fast, scoped verification

~~~sh
# One exact owner and all its relocations / complete physical tail:
python3 -B scripts/build-match-unit.py --unit ecl-run-ecl
python3 -B scripts/compare-coff-function.py --unit ecl-run-ecl --json

# One affected source, all its exact owners and distinct pinned profiles:
python3 -B scripts/replay-scoped-exact.py --source src/ReplayInput.cpp

# For a changed shared .hpp/.inl, see actual source dependents first:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EnemyManagerCore.inl --dry-run

# Recompile and protect only the relevant nonexact boss:
python3 -B scripts/check-frontier.py enemy-on-update
python3 -B scripts/check-frontier.py --show replay-save

git diff --check
python3 -B scripts/ci.py
# Target-independent tool tests, when editing tooling/config:
python3 -B scripts/ci.py --full
~~~

`ReplayInput.cpp` has **both /EHsc and no-/EHsc** exact carrier
profiles; Title uses `/O1` without `/Oi` and some Effect owners omit
`/EHsc`. Always use the profile recorded for the target unit.
For `EclManager.cpp`, changing the included
`EnemyManagerCore.inl` can renumber private `$L...` labels across
84 exact functions. Before editing any manifest spelling, prove
nonrelocation byte equality, relocation offset/type, PE destination
and same-section COFF symbol definition. Do not pretend changed
label numbers establish new Exact credit.

Build products and compiler PDBs are **disposable ignored caches**;
missing `build/matching/*.obj` or `build/frontier/*.obj` after storage
cleanup is expected. Regenerate only the relevant carrier(s).
A missing historical `.analysis` probe is not missing proof: rebuild
from `src/` with the current target/profile.

## Protected inputs and storage policy

**Never delete:** the verified executable
`resources/th09.exe`, `.tools/` pinned runtime/toolchain selectors,
`_reference/`, saved Ghidra project
`.analysis/ghidra-direct/`, or the
`c2-analysis/` C2 investigation / Ghidra projects.
The native allocator investigation also retains
`.analysis/c2-heap.bin`,
`.analysis/FUN_107844a4.decomp.txt`, and four
`.analysis/gdb-c2-*.cmd` recipes. Two compact
natural-C++ diagnostic *sources* remain useful:
`.analysis/PlayerCollisionHelpers.natural-preheader-842.cpp`
and `.analysis/SupervisorNetwork.natural-ebx-reload.cpp`.
They are **not maintained source and not exact proof**.

Superseded compiler objects, PDBs, textual variant corpora,
captured logs and `build/` caches have been pruned to save disk.
Do not restore them en masse. Don't commit original game files,
toolchains, Ghidra databases, C2 DLLs, decompiler dumps or
generated comparison results. Do not touch the protected
target while cleaning cache state.

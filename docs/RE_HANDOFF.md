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

## Current source-native reconstruction checkpoints (2026-11-17)

The maintained source includes several **natural C/C++ ownership recoveries**;
none of these changes adds an exact-function ledger claim. All were checked
against the original Japanese 1.50a executable with pinned VC7.1 profiles,
complete compared code/tails and independently verified COFF relocations.

### Latest six scoped commits

- `e7c7f5ee`, `34098751` — The **2,852-byte** `FrontMessageRuntimeView::Update`
  boss reads short/byte script operands from their actual native typed payload
  pointers (20 short + 2 byte sites) and addresses the authentic
  `g_GameManager.sides[index]` allocation. Its complete physical owner and
  same-source decode leaf pass **2 exact / 3,007 bytes / 167 relocations**.
- `9058a27f` — The **2,472-byte** background stage-script interpreter reads
  its 22 integer, 4 float and 20 vector wire payloads from
  `RawStageInstr::args`, retaining the native 20-byte wire record layout.
  The script, camera interpolation, Hermite and timer owners pass
  **5 exact / 3,231 bytes / 160 relocations**.
- `49f2af51` — The **9,358-byte** `AnmManager::ExecuteScript` interpreter reads
  integer/float operands directly from its variable-length `AnmRawInstr`
  payload (27 integer + 3 float cursor forms), with readable opcode access
  macros. The whole instruction-switch tail and sibling ANM leaves pass
  **5 exact / 10,496 bytes / 421 relocations** across the two pinned profiles.
- `25c271d0` — The **14,792-byte** `EclManager::RunEcl` boss uses native
  `activeCallStack2CE4[depth]` context records at all three sites, rather
  than synthesizing integer-address byte strides. The full ECL source cohort
  passes **84 exact / 45,420 bytes / 1,987 relocations** across two profiles.
- `7bcee027` — Enemy's nonexact 3,900-byte `OnUpdate` now indexes the actual
  `trailSamples33E8` and `attachedEffectState53B4.effects00` arrays. The same
  **84 ECL exact** siblings pass their scoped replay after this edit; the
  Enemy calc frontier remains **3,900/3,900 with 2 differing bytes**.

Across the four distinct exact source cohorts above, the current verified
comparison totals **96 existing exact functions, 62,154 full physical bytes
and 2,735 relocation fields**. These are selected-source proofs, not a
whole-EXE replay, newly promoted functions or a successful native game build.
The compiler-private `$L...` symbols changed when natural source was restored;
**every** recorded rebinding was checked against its actual same-section COFF
definition and the unchanged linked original PE destination (as well as the
relocation offset, type and full linked bytes). No handwritten ASM, NOP/padding,
register forcing, altered target bytes or relaxed frontier ratchets were used.

### Earlier source-native checkpoints in this worktree

`bdd34f42`, `ba48f475`, `a4dc57a1`, `4d977749`, `78abe86d` restore
native HUD VM traversal, Effect pool iterators and allocation indexing,
background fog BGRA lanes, and Title key-config counters. Scoped replay covers
all associated exact owners. `40a561ee`, `79fecb41`, `5118ecad`, `c283bb54`,
`0f2b2902`, `8e872bff` additionally recover typed Player selector collision
vectors, genuine `ZunTimer` conversions and Player collision/angle receivers,
plus native Laser pool traversal. Their linked COFF symbols were validated
before rebindings; exact siblings remain unchanged. See the Git commits for
individual source edits and `docs/KNOWLEDGE_BASE.md` for older experiments.

### Unresolved work and useful negative results

The **11 source-present nonexact owners** remain nonexact. In particular,
Enemy `OnUpdate` still differs at just two register-encoding bytes in the
restricted-side draw-head selection. Eleven more natural array, helper,
reference and category-expression forms were compiled without improving this
frontier; preserve the current code and investigate the earlier value lifetime.
`CheckBulletCollision` still needs its original loop backedges and early hit
routing, while `SupervisorServiceUpdate` has a larger control-flow mismatch.
Prior short-index-to-int substitutions in `UpdateChargeAttack` increased its
residual; do not promote unproved variants or fabricate padding/ASM.

Fast checks for the maintained work:

~~~sh
python3 -B scripts/replay-scoped-exact.py --source src/FrontMessageUpdate.cpp
python3 -B scripts/replay-scoped-exact.py --source src/BackgroundStageScript.cpp
python3 -B scripts/replay-scoped-exact.py --source src/AnmManagerExecuteScript.cpp
python3 -B scripts/replay-scoped-exact.py --changed-file src/EnemyManagerCore.inl
python3 -B scripts/check-frontier.py enemy-on-update player-bullet-collision supervisor-network-service
python3 -B scripts/ci.py
~~~

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

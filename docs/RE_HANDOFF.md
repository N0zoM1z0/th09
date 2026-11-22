# TH09 reconstruction handoff (2026-11-21)

[Progress](PROGRESS.md)

## Start the session

Read [AGENTS.md](../AGENTS.md). Inspect staged, dirty and untracked work
before changing it. Run these commands before target-dependent work.

```bash
git status --short --branch
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
```

Pick one source-present owner and rebuild its full frontier with the
profile in `config/frontiers.toml`. Keep that owner as the focus through
strict acceptance. Follow its real callees, same-carrier siblings and shared
data when the evidence points beyond its body. Use the direct Ghidra runner
for new analysis. It checks the disk target and the saved Program's
architecture, entry and every file-backed mapped byte before export.
[Comparing compiler contexts](RE_WORKFLOW.md#trace-a-difference-through-its-compiler-context)

```bash
python3 -B scripts/check-frontier.py NAME --show
python3 -B scripts/ghidra-analysis.py --function ADDRESS
```

## Whole-image link and position independence

The operator selected Windows i386 product closure, aiming at position
independence (PI) as in TH08. PI here means code and data can move at link
time while every reference still reaches its intended function, object and
field. The TH08 checkout's `docs/POSITION_INDEPENDENCE.md` lists the
acceptance checks.
The first step is a link of every canonical exact carrier with the pinned
VC7.1 linker and libraries. It lists the remaining closure work.

```bash
python3 -B scripts/trial-link.py build    # about 9 minutes, serial, nice
python3 -B scripts/trial-link.py link     # classify LNK2005/LNK2001/LNK2019
python3 -B scripts/trial-link.py verify   # units vs trial carriers and profiles
```

The trial carriers reproduce all 1,084 canonical units. After the
checkpoints below, the link reports this work:

| Work | Count | Next action |
| --- | ---: | --- |
| Duplicate definitions from nine sources compiled under two profiles | about 150 symbols | Recover the original translation units. Use address neighbourhoods; see below |
| Data symbols with no definition | 379 | Define each global once, with its initializer for `.data` and `.rdata` |
| Calls through view-type aliases of exact functions | 136 symbols, 81 functions | Use the real type. Identical bodies folded by the linker need their own definitions instead |
| Functions without exact source | 10 | Movement owner, 5 unknown constructors, 4 compiler-generated helpers |

Nine sources have no single profile that keeps all their units exact.
Most minority units lie in another module's address range. Moving
`TitleScoreRecordView::InsertIntoTable` into the ScoreData carrier it
precedes resolved TitleScreen. `AnmManager::SpriteHasTexture` and
`AsciiManager::SetScale` sit unaligned inside the `/O1` Title block. They
are probably header inline bodies that only the `/O1` unit emitted, so
check each caller's call or inline expansion before moving them. Constructors
needing `/Ob0` inside an `/Ob1` range, such as AnmManager, CardAttack and
Player, point to a callee that the original did not inline. `EclManager.cpp`
spans `0x004010B0..0x0044C790` and combines many original units.

The D3DX8 library in TH09 likely predates the pinned SDK. The pinned
`D3DXIMAGE_INFO` is 0x1C bytes, while TH09's surface table uses 0x14-byte
records, the DirectX 8.0 layout. Library provenance for libcmt and d3dx8
is still open. The trial link uses the VC7.1 copies. It drops `libci`,
`libc` and `amdmath`, which d3dx8.lib requests.

## Player movement handoff

The single-function exact reconstruction is paused for handoff. The
latest cold build reproduced the recorded frontier. [PlayerMovement.cpp](../src/PlayerMovement.cpp) · [Frontier manifest](../config/frontiers.toml)

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| PlayerLifecycleView::UpdateMovementAndOptions | 0x0041C170 | 1,900/1,900 bytes, 4 differences with relocation bytes masked, 8 after relocation replay | Recover the original order of the two loads at 0x0041C296. [Compiler evidence](KNOWLEDGE_BASE.md#2026-11-20-player-movement-focus-loads) |

The original loads configuration into ECX, then the live Player side into
EAX. The maintained build emits the same two loads in reverse order. The
configuration DIR32 field moves from `+0x128` to `+0x12B`. The other 65
fields retain their positions. The 16 focus-effect IDs already have a
checked 64-byte COFF `.data` definition at the original table reference
`0x004A1A10`. [Extent and data evidence](KNOWLEDGE_BASE.md#extent-and-owned-data)

A cold frontier build checks the recorded four masked differences.
Complete acceptance also covers unmasked bytes, all relocations and
their destinations, the two switch tables, the focus-effect initializer,
and affected exact units.

```bash
python3 -B scripts/check-frontier.py player-movement --show
```

The local scratch control
`.analysis/movement-focus/focus-order-control.cpp` reproduces the original
lookup and all relocation positions. It leaves seven differences in the
flag update and seven in the first effect call. Its early loads get
registers globally. The maintained build assigns both loads locally,
advancing the scratch cursor to EDX before the flag update. A full match
needs both the retail lookup and the later scratch choices. The retained
control and debugger recipes live in this checkout's ignored analysis
directory. [Other measured controls](KNOWLEDGE_BASE.md#2026-11-21-player-movement-direction-join-and-scratch-allocation)

```bash
cp src/PlayerMovement.cpp .analysis/movement-focus/focus-current.cpp
python3 -B .analysis/movement-focus/trace-run.py cursor focus-current focus-order-control
python3 -B .analysis/movement-focus/trace-run.py flag-liveness focus-current focus-order-control
```

The latest slot-access controls retained the known load-order problem or
put the pointer in EAX. The combined ReplayInput build hit conflicting
`g_Supervisor` declarations. The paired-copy and helper batch also failed
to complete. [Interrupted build notes](KNOWLEDGE_BASE.md#configuration-slot-access-and-carrier-boundary)

## Recent exact checkpoints

- `fsincos` at 0x00412780 matches all 33 bytes with zero relocations.
  The restored x87 primitive and its exact Etama draw caller replay from
  the same compiler carrier. Original provenance stays open.
  [Etama x87 proof](KNOWLEDGE_BASE.md#2026-11-21-etama-paired-trigonometric-helper)
- `Supervisor::UpdateNetworkService` matches 1,633 bytes and 106 relocations.
  Block-local seed and prediction outputs let VC7.1 reuse the original
  stack slots and keep the input loads in EDI and EBX.
  [Network service proof](KNOWLEDGE_BASE.md#2026-11-21-network-service-retry-and-input-context)
- `PlayerLifecycleView::CheckBulletCollision` matches 862 bytes and 26
  relocations. The inner scan and common hit-count exit recover the original
  dispatcher layout and cursor lifetime.
  [Collision proof](KNOWLEDGE_BASE.md#2026-11-21-checkbulletcollision-reward-storage-and-ordered-bounds)
- `PlayerLifecycleView::UpdateChargeAttack` matches 1,210 bytes and 70
  relocations. Two row-end lookups recover the early base/index encodings.
  [Charge input proof](KNOWLEDGE_BASE.md#2026-11-21-chargeattack-input-and-timer-allocation)
- `TitleScreenView::OnUpdateResult` reproduces the original byte load and
  sign extension when VC7.1 merges the two cursor-direction calls.
  [Result update proof](KNOWLEDGE_BASE.md#2026-11-21-title-result-cursor-direction)
- `RunEcl` recovered its final register choices by restoring natural source
  in other handlers. [RunEcl context](KNOWLEDGE_BASE.md#runecl-register-coupling-across-handlers)
- `EnemyManagerView::OnUpdate` needed the exact timer definition visible in
  its carrier and the authentic draw-list control flow.
  [Enemy update proof](KNOWLEDGE_BASE.md#2026-11-21-enemy-update-exact)
- `ReplayManagerView::BeginPlaybackStage` needed the visible, byte-identical
  `RngRuntimeView::SetSeed` body to recover scheduling across the restore
  block. [Replay seed proof](KNOWLEDGE_BASE.md#2026-11-21-co-visible-replay-seed-and-exact-playback)
- `DrawReplayMenu` needed separate caption-table ownership and a shared
  selected-slot reference. `SaveReplay` retains its own Shift-JIS table
  proof. [Title replay proof](KNOWLEDGE_BASE.md#2026-11-21-title-replay-caption-tables)
  [SaveReplay proof](KNOWLEDGE_BASE.md#2026-11-20-savereplay-payload-copy-and-user-tables)

## Finish the selected owner

Require a cold pinned VC7.1 replay of the complete physical extent and all
fields, plus checked tables, literal data and associated unwind contributions
where applicable. Rebuild the changed carrier and its affected exact
consumers. Use `--changed-file` with `--dry-run` first for a shared header or
inline file to check the dependent scope. Track the chosen owner's promotion
in the exact ledger and regenerate progress. Use basic CI and review the
diff before the local checkpoint. [Oracle policy](ORACLES.md)

```bash
python3 -B scripts/replay-scoped-exact.py --source src/FILE.cpp
python3 -B scripts/replay-scoped-exact.py --changed-file src/FILE.hpp --dry-run
python3 -B scripts/progress.py
python3 -B scripts/ci.py
git diff --check
```

## Private inputs and storage

Preserve the original `resources/th09.exe` and game data, `.tools/`, the
`_reference/` checkout, the active Ghidra project
`.analysis/ghidra-direct/243bffea8dcbe99a/`, and `.analysis/factory-runtime/`.
These include this game's toolchain, runtime selectors and Wine state.

Keep the existing C2 project roots `c2-analysis/project/` and
`c2-analysis/allocator-project/`, `c2-analysis/c2.dll`, its scripts and
retained top-level allocator reports. The small compiler context comprises
`.analysis/c2-heap.bin`, `.analysis/FUN_107844a4.decomp.txt`, the four
`.analysis/gdb-c2-*.cmd` recipes, and the
`SupervisorNetwork.natural-ebx-reload.cpp`,
`PlayerCollisionHelpers.natural-preheader-842.cpp`, and
`PlayerCollisionHelpers.nested-three-loop-849.cpp` controls under
`.analysis/`. They retain allocator traces and CFG controls for unresolved
work. Rebuild selected objects when `build/` is absent. Keep cleanup scoped
to identified disposable outputs. [Shared tools](SHARED_TOOLS.md)

# TH09 reconstruction handoff (2026-11-23)

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

The 2026-11-23 origin review confirmed all 35 candidate boundaries and
classified the implicit DummyMidiTimer destructor. The remaining origins
require source or provider evidence that distinguishes byte-identical
producers. [Origin review](KNOWLEDGE_BASE.md#2026-11-23-review-of-unresolved-function-origins)

## Whole-image link and position independence

The operator selected Windows i386 product closure aiming at position
independence (PI): code and data move at link time while every reference
still reaches its function, object and field. The TH08 checkout's
`docs/POSITION_INDEPENDENCE.md` lists the acceptance checks.

The normal and reordered trials both run under Wine from the title into the
attract-mode match. All tracked code units and all but one generated data
label move between the two links. The linked comparison checks each unit
against its selected COFF object and checks the generated data and its pointer
fields. The generated pointers and link aliases also have a retail binding
check. A single link precedence constraint keeps the reviewed
`SpriteHasTexture` COMDAT provider active in both layouts.
[Linked layout evidence](KNOWLEDGE_BASE.md#2026-11-23-position-independence-and-product-checks)
· [Retail binding check](KNOWLEDGE_BASE.md#2026-11-23-retail-binding-for-generated-pointers-and-link-aliases)

```bash
python3 -B scripts/trial-link.py build              # serial, lowered priority
python3 -B scripts/product-data.py                  # labelled data + resources
python3 -B scripts/product-data.py --reverse
python3 -B scripts/trial-link.py link               # build/link-probe/trial.exe
python3 -B scripts/trial-link.py link --reverse     # trial-reversed.exe
python3 -B scripts/verify-trial-layouts.py
```

The [disjoint image-base check](KNOWLEDGE_BASE.md#2026-11-23-disjoint-image-base-check)
uses the same inputs and separate relocatable outputs. It checks section
attributes and uses Capstone to check absolute memory operands independently
of the HIGHLOW directory.

```bash
(
    set -e
    for base in 0x400000 0x600000; do
        python3 -B scripts/trial-link.py link --base "$base"
        python3 -B scripts/trial-link.py link --reverse --base "$base"
    done
    python3 -B scripts/verify-trial-layouts.py --verify-bases
)
```

The game data is `resources/th09.dat` (ignored, copied from the operator's
install, SHA-256 `73a344a2...5ea0`). A run directory needs the exe,
`th09.dat` and `th09.cfg`; use `xvfb-run` with a separate Wine prefix such
as `.analysis/game-prefix`.

Check executable identity before a product-length replay. The command
checks the archive digest, then reports each PE's size and the checksum
used by the game. The current trial layouts fail the stock
`th09_0150a.ver` record.

```bash
python3 -B scripts/build.py --runtime-version \
    resources/th09.exe build/link-probe/trial.exe build/link-probe/trial-reversed.exe
```

What the product still borrows or approximates:

- `scripts/product-data.py` emits the original globals that no source
  defines yet as labelled MASM data, extracted from the attested original
  at build time. Pointer words are symbolic. `config/product-link.toml`
  binds DirectInput formats to the library and records reviewed object
  sizes. The linked checks cover section access flags, controller and
  loading buffers, and the source-defined enemy schedule.
  [Storage evidence](KNOWLEDGE_BASE.md#2026-11-23-linked-data-permissions-and-bulk-storage)
  Natural C++ definitions for generated tables remain open.
- `product/` holds link glue: the global construction sequence for those
  objects and the PbgArchive constructor, whose body folds with a D3DX8
  routine at `0x00433040`. The typed auxiliary PbgArchive array now has
  matching retail initializer and terminator wrappers. The GameErrorContext
  cleanup completes the ten observed retail exit registrations. All six
  layouts pass checks of the registration order and zero-fill storage.
  Console links with the same startup objects confirmed the ten live CRT
  registrations and exited normally.
  [Startup and exit evidence](KNOWLEDGE_BASE.md#2026-11-23-linked-product-startup-and-exit-callbacks)
- 139 references resolve through `/ALTERNATENAME` to the definition
  bound to the same original address. They are view types or bodies that
  the original linker folded. Replacing the views with real types is open.
- EclManager uses one exact `/Ob1` carrier for comparison and trial linking.
  Function-scoped inline depth preserves the five bodies that previously
  differed in the product while keeping `SetPolar2D` visible to its callers.
  RunEcl and the ECL callback tables pass in the linked layouts.
  [Carrier split evidence](KNOWLEDGE_BASE.md#2026-11-23-ecl-carrier-split-and-polar-call-context)
- The movement owner is linked under its reviewed `/Ob1` frontier profile.
  The linked verifier checks the recorded byte differences, complete extent,
  relocations and focus ID table. An earlier local 1P versus CPU run matched
  retail for ordinary and focused movement in both layouts.
  [Live input evidence](KNOWLEDGE_BASE.md#2026-11-23-live-keyboard-movement-in-local-match-mode)
  · [Selected carriers and right-side focus](KNOWLEDGE_BASE.md#2026-11-23-selected-carrier-comparisons-and-right-side-focus-mode)
- External playback of the bundled `demorpy0.rpy` supplies a
  repeatable per-frame check. It exposed a split CPU difficulty table
  that changed the reordered build's decisions from frame 516. After
  the fix, both layouts agree with retail until the periodic executable
  version check shuts them down. The retail capture covers all 7,996
  input records. [Replay evidence and command](KNOWLEDGE_BASE.md#2026-11-23-replay-layout-divergence-and-cpu-table)
  The replay probe can select a relocated trial PE and reports a live
  version rejection as a failed capture with the observed version arguments.
  [Version rejection evidence](KNOWLEDGE_BASE.md#2026-11-23-live-replay-version-rejection)
- Retail and both trials write short stage-9 replays through the results
  menu. Both trials play the retail recording with matching traces through
  480 frames. Trial recordings carry their writer's executable size and
  checksum, which the stock version record rejects.
  [Replay save and playback evidence](KNOWLEDGE_BASE.md#2026-11-23-linked-replay-captions-and-short-replay-saves)

The trial pins the Platform SDK D3DX8 archive used for the reviewed
game-facing entry points. The retail provider and both trial providers
write 0x1C bytes into metadata slots spaced 0x14 bytes apart.
The linked verifier checks the SIMD constants, six math dispatch thunks
and all 112 initial function pointers in their shared table.
[D3DX8 provider and runtime evidence](KNOWLEDGE_BASE.md#2026-11-23-d3dx8-provider-and-surface-output)
· [SIMD constants and math dispatch table](KNOWLEDGE_BASE.md#2026-11-23-d3dx8-simd-constant-section)

The trial pins VC7.1 `libcmt.lib` and explicitly orders its four
`.CRT$XIC` inputs so SSE2 detection precedes multibyte initialization
as in retail. The linked verifier checks the C and C++ startup tables.
[CRT startup evidence](KNOWLEDGE_BASE.md#2026-11-23-crt-startup-provider-and-initializer-order)
It also checks the PE GUI entry, `WinMainCRTStartup` and its SEH scope
against the pinned startup object. [PE entry evidence](KNOWLEDGE_BASE.md#2026-11-23-pe-entry-and-crt-scope-bindings)

The linked GUID checks cover DirectPlay and the other tracked COM identifiers.
The selected Wine runtime returns `E_FAIL` from its DirectPlay `Host` stub.
Retail and both trials follow the same wait and cancel path at that boundary.
[DirectPlay evidence](KNOWLEDGE_BASE.md#2026-11-23-directplay-guid-bindings-and-host-entry)
The native Windows provider passed a standalone join. Retail and both
tested trial images also reached the native title with the same provider
binding. Windows startup exposed a path-buffer adjacency requirement
that the trial data layout now preserves.
[Native startup evidence](KNOWLEDGE_BASE.md#2026-11-23-native-directplay-startup-and-path-storage)
Retail subsequently reached native Match Mode host wait with a local
DirectPlay player ID. Native trial hosting and two-game synchronization
remain open. [Retail host evidence](KNOWLEDGE_BASE.md#2026-11-23-native-retail-match-mode-host)

The reordered link exposed a split in the opponent parameter table used by
Story setup and the front transition callback. Both layouts now preserve
the two interior views. The 14 opponent lists have separate link labels
and move relative to the neighboring BGM table.
[Opponent parameter table](KNOWLEDGE_BASE.md#2026-11-23-opponent-and-transition-parameter-table-storage)
· [Opponent lists](KNOWLEDGE_BASE.md#2026-11-23-independently-linked-opponent-lists)

Retail and both trial layouts exchanged live score files, and retail
reloaded an Options change from the reordered build.
[Persistence evidence](KNOWLEDGE_BASE.md#2026-11-23-score-and-configuration-round-trip)

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

The completed paired-reference and array variants kept the load-order
differences or also changed later x87 instructions. Making the ReplayInput
definitions co-visible kept the eight-byte residual.
[Slot and carrier controls](KNOWLEDGE_BASE.md#configuration-slot-access-and-carrier-boundary)

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

Preserve the original `resources/th09.exe` and game data (`resources/th09.dat`,
`resources/th09.cfg`), `.tools/`, the
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
work. Rebuild selected objects when `build/` is absent. The game-run Wine prefix
(`.analysis/game-prefix`) is recreated on first use. Keep cleanup scoped
to identified disposable outputs. [Shared tools](SHARED_TOOLS.md)

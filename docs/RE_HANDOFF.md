# TH09 exact reconstruction — agent handoff (verified 2026-11-16)

**Start:** [AGENTS.md](../AGENTS.md) · [workflow](RE_WORKFLOW.md) ·
[original/VC7.1 Oracles](ORACLES.md) ·
[knowledge base](KNOWLEDGE_BASE.md).
This is the **current restart checklist**, not a session log; older session
notes live in Git history and the knowledge base. **Start with `git status`,
recover any dirty changes, then regenerate ledger counts and only the
compiler objects you need.**

## Verified target, counts and evidence boundaries

- **Target:** original Japanese TH09 **v1.50a**, Windows i386, PE image base
  `0x00400000`, entry `0x0047D45F`. Ignored, protected
  `resources/th09.exe` SHA-256:
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  Do not patch, rename, commit or delete the target.
- **Ledger (2026-11-14):** 2,248 candidates, **1,037 source-present
  authored mappings**, **1,026 exact**, **11 nonexact / 13,353 logical
  bytes**, 35 reviewed unresolved origins. Exact authored bytes
  **278,578 / 291,931 = 95.4260%**. Complete original-code inventory is
  **not** proven. Use `scripts/report-reconstruction-status.py` and the
  generated [progress](PROGRESS.md) rather than copying totals.
- **Phase:** exact reconstruction is unfinished. A whole Windows i386
  product, dual-Oracle semantic tests and port are **not** proven.
- **Proof hierarchy:** original PE + attested Ghidra for analysis; pinned
  VC7.1 COFF for candidates; `config/matches.csv`, `config/functions.csv`
  and `config/match-units.toml` for exact claims. Partial similarity is
  NONEXACT. Rebind private `$L…`/`$name$…` labels only after checking the
  same-section COFF definition, manifest target and original PE operand.

## What changed in the 2026-11-14/15 sessions

- **RunEcl is exact** (unit `ecl-run-ecl`, 14,792 logical / 15,564
  physical bytes). Plain statements replaced mutually compensating
  `goto` merges and snapshot locals; see the knowledge base section
  "VC7.1 register choice is coupled to neighbouring source".
- **BeginPlaybackStage** is plain C++ and now 540/540 bytes (was 538/540).
- **SaveReplay** and **Player movement** cast/address scaffolding was
  replaced by plain indexing with byte-identical codegen.
- No further owner became exact. The pinned `c2.dll` path is mapped through
  `FUN_107821a7`, `FUN_1078368e`, `FUN_107833f2` (75 weighted register-score
  slots), and `FUN_107831f7`. `FUN_1077c10f` creates 0x48-byte live-range records
  that retain a pointer to their source IR node; `FUN_1077ff3b` walks the
  live-set/bitset structures. The first observed color pick was an EBX-only
  value (register ID 4), not the draw-group byte; do not use that score as an
  explanation for EAX/ECX. A raw-register inline-assembly helper emits the
  desired EAX/EAX pair but moves five local stack homes and four Float3 call
  argument bytes, producing 22 other differences. Twenty-four declaration
  permutations, sixteen local-scope combinations, and six `register` index
  forms did not improve it. All experiments were reverted; the maintained
  two-byte carrier is intact. A live GDB trace of the pinned C2 backend
  captured 897 class-0 weighted-score calls while compiling `EclManager.cpp`.
  The actual restricted-side draw-head expression is now mapped: C2's
  flattened line 68814 corresponds to physical line 208785 of the preprocessed
  `EclManager.i`, which is `EnemyManagerCore.inl:439`. Its score node is vreg 7
  (`IR 0x00b05d04`, opcode `0x200a0805`, AST `0x00b08b8c`) and selects descriptor
  ID 1. The live-set mask is `0x19E`; candidate IDs 1 and 2 are both eligible,
  both have zero score and the candidate order starts with ID 1, so the C2
  tie-break selects ID 1 (the emitted ECX byte load). The later normal draw-head
  expression maps to preprocessed line 69373 / `EnemyManagerCore.inl:998`, but
  has no separate score event in this trace. The earlier statement that no
  node mapped to source line 434 compared flattened C2 line numbers with
  `.inl` line numbers and is superseded. Descriptor-to-register mapping for
  the second candidate and the original EAX preference still need proof; see
  the 2026-11-16 draw-group live-set entry. Do not infer this choice from the
  first unrelated EBX-only color pick. A scoped volatile side-state pointer
  produces the target EAX/EAX branch pair but shifts four Float3 call-argument
  bytes (+0x1BA/+0x1BF/+0x1C1/+0x1C3); narrowing its scope, using const-volatile
  or a reference, and adding `register` to the pointer/flags do not recover
  those bytes. A byte-width side-category local regresses 463 bytes, and a
  volatile pointer to the flags field regresses 653. All probes were reverted.
- `SupervisorServiceUpdate` remains 1,629/1,633 bytes / 1,038 differences.
  Attested Ghidra decomp confirms the result local is initialized to zero, set
  to one on the retry path, and returned from both retry and synchronized-frame
  exits. A common result-return label reduces the residual to 980 but homes the
  result at `[ebp-0xc]` in a `0x18` frame. A 20-byte aggregate for both `PopFrame`
  scratch sets, inputs and result reduces it to 931 differences, but grows the
  owner to 1,656 bytes and still homes the result at `[ebp-8]`. A function-wide
  `SupervisorNetworkState` snapshot restores the `0x14` frame and `[ebp-4]`
  result home, but changes the owner to 1,525/1,633 with 1,168 differences;
  segmented snapshots also spill and regress. All probes were reverted. Next,
  identify the extra `[ebp-4]` local in the aggregate carrier and its true
  lifetime before trying another stack overlay. See the 2026-11-16 result/scratch
  lifetime entry in `KNOWLEDGE_BASE.md`.
- `PlayerLifecycleView::UpdateChargeAttack` remains 1,210/1,210 with 183
  differences. Separate side/input-side locals, typed-array indexing,
  reference input receivers, and a `register` input-pointer hint did not
  recover retail's EDI side / EBX input-pointer allocation.
- A `ScreenEffect::CalcShakeEnvelope` duration-reference probe regressed to
  340/336 bytes / 252 differences and was reverted; its 336-byte/26-offset
  baseline remains intact.
- **Player CheckBulletCollision** has a protected 853/862-byte, 692-difference
  carrier. The best byte-overlap diagnostic remains **840/862 / 632
  differences** with the retail circle-first order, but it has five backedges
  versus the target's four. A new four-latch source reached **835/862 / 683
  differences** and exactly four header backedges; its type/cancel block is
  still scheduled after geometry, opposite retail's circle-hit fallthrough.
  A fallthrough spelling reached 853/862 / 670 differences but retained the
  wrong block order and added a radius test. A separate six-latch form reached
  845/862 / 661 differences. None was promoted. The five exact same-TU
  siblings pass scoped replay (1,220 bytes / 32 fields). Maintained source and
  frontier remain 853/862. See the 2026-11-15 collision-latch and boss
  allocation entries.

## Remaining source-present nonexact owners

Measured 2026-11-14 with each owner's pinned profile. "Masked" counts
exclude relocation fields; dedicated inspectors may report slightly higher
numbers.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900, **2 bytes** (+0x19A/+0x1A1) (EclManager `/O2 /Ob1`) | Early draw-group vreg 7 maps to `EnemyManagerCore.inl:439`; C2 mask `0x19E` leaves descriptors 1/2 tied at zero and selects 1/ECX first. Identify why the original chooses EAX and how to make ECX unavailable without regressing adjacent Float3 setup. `check-frontier.py enemy-on-update`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238, **2 bytes** (+0xB7/+0xBB) (`/O2 /Ob1`) | Header-copy buffer load ECX vs EDI; source is now plain. 128 spelling combinations are byte-neutral. `replay-save`. |
| **Player movement** | 0x0041C170 | 1,900/1,900, 8 bytes (4 masked) at +0x127 (`/O2 /Ob1`) | `g_GameConfiguration`/`sideIndex` load order; every subscript spelling compiles side-first. `player-movement`. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51, **4 bytes** in two windows (`/O2 /Ob1`, no `/EHsc`) | Retail uses EDI for the global ANM manager and ESI for callback context; current source reverses the setup order. Direct-global-for-both and local-snapshot-for-both probes regressed to 25 and 7 differences respectively. `effect-added-callback`. |
| **Title OnUpdateResult** | 0x004266B5 | **484/485**, one site at +0xFD (Title `/O1 /Ob1`, no `/Oi`) | Retail keeps the selected character in CL and sign-extends separately; VC7.1 folds ours (only a real intervening store unfolds it). That single site causes the 1-byte shift behind ~166 masked differences. `title-update-result`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336, 26 bytes (`/O2 /Ob1`) | ESI/EDI `screenEffect`/`&timer` swap. `screen-shake-envelope`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280, 29 bytes (Title `/O1 /Ob1`) | Selected-record EDX/ECX. `title-draw-replay-menu`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540/540**, 13 masked / 19 raw at +0x14B–0x15F (`/O2 /Ob1`) | Plain C++ (no `volatile` alias). VC7.1 hoists the `sides[0].runtime` load above the +0x37C store; retail issues it after the +0x1E8 load. Statement moves/swaps, array and side-pointer forms do not move it. `replay-begin-playback`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210, 183 masked (`/O2 /Ob1`) | Retail: int side in EDI, input in EBX, `sideMode` spilled to `[ebp-1]`. Separate side/input aliases, typed indexing, reference input and register hints did not change the allocation. `player-charge-attack`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **853/862**, 692 differing offsets / 26 relocation fields (`/O2 /Ob1`) | Protected carrier. Best byte-overlap diagnostic is 840/862 / 632 differences with five backedges; a separate 835/862 / 683 candidate has four backedges but wrong hit/cancel block order. Both break protected matches; do not update frontier metadata. Five exact siblings pass. `player-bullet-collision`. |
| **Supervisor network service** | 0x00430AA0 | 1,629/1,633, **1,038 differing bytes** (`/O2 /Ob1`) | Retail keeps the network pointer in EDI and result in `[ebp-4]`; ours CSEs constant `1` into EDI and folds the result home. Volatile pointer/result probes regressed to 1,676/1,633 and 1,692/1,633; keep the protected baseline. `supervisor-network-service`. |

## Reproduce *only* the changed compiler carriers

`build/` objects, PDBs and comparison JSON are **ignored disposable build
caches**, removed in the storage cleanup. Missing objects are expected, not
missing sources or original target data; each selected command regenerates
only the necessary carriers.
Do **not** start with `--owner all`, a project-wide cold replay, or
`ci.py --full` just to repopulate caches.

```bash
git status --short --branch             # inspect AND recover dirty work
git diff --check
git log -7 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

# Nonexact functions: rebuild and fail if any matching byte regresses
# (diagnostic only; names and residuals live in config/frontiers.toml).
nice -n 12 python3 -B scripts/check-frontier.py                 # all, ~80 s
nice -n 12 python3 -B scripts/check-frontier.py replay-save --show
python3 -B scripts/check-frontier.py replay-save --update       # record an improvement

# Preserve all genuinely affected exact siblings after modifying a source:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl --dry-run
nice -n 12 python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl
# Or use --source src/ReplayInput.cpp for a single source-owned cohort.

# One exact function when needed; builds the selected original-bound carrier:
nice -n 12 python3 -B scripts/build-match-unit.py --unit ecl-pop-context
python3 -B scripts/compare-coff-function.py --unit ecl-pop-context --json

# For documentation/ledger edits (no game compiler necessary):
python3 -B scripts/validate-docs.py
python3 -B scripts/ci.py
```

`replay-scoped-exact.py` only checks previously **exact** canonical functions,
not the nonexact boss in the same TU. Editing a shared `.inl` can change
private symbol names in several VC7.1 carriers. Compile and check every
affected physical owner *before* changing manifest symbol spellings. Avoid
compiler register-forcing hints, artificial padding and target-patching.

For new target analysis use attested `scripts/ghidra-analysis.py`, for example
`python3 -B scripts/ghidra-analysis.py --function 0x004086C0`. This validates
the imported Program and original mapped PE bytes. The earlier Ghidra
pseudocode under ignored `.analysis/ghidra-direct/` is a **hypothesis**, not
a byte-exact source receipt; a fresh import can be necessary after a Ghidra
tool upgrade.

## Next steps (ranked)

1. **Player CheckBulletCollision (large boss).** Keep the maintained
   `853/862`-byte, 692-difference carrier protected. Attested target analysis
   fixes the required order: circle test, shared hit/cancel block, then rotated
   and AABB geometry; circle success falls through to hit, while square success
   jumps backward. Retail has four increment/backedge sites at `+0x0a3`,
   `+0x133`, `+0x194`, and `+0x19e`. The saved `840/862`, 632-difference
   diagnostic has five backedges and new offsets; the `835/862`, 683-difference
   candidate has four but C2 still schedules hit/cancel after geometry. The
   common-invalid/circle-latch plus pre-square-hit attempt also collapses to
   that 835-byte shape. Radius-first and comparison-inversion probes alter the
   x87 schedule or add new mismatches. Next, influence C2 block layout without
   changing x87 operand order or adding a latch. Never update the frontier for
   a candidate with new differences.
2. **SupervisorServiceUpdate (large boss).** Continue from the confirmed
   `[ebp-4]` result-home mismatch. Recover the original local lifetime and the
   `PopFrame` scratch layout without forcing an enlarged volatile/address-taken
   frame. The Ghidra export confirms the result semantics; the remaining
   question is the source-scope/stack-slot assignment that keeps the result
   home live while the two seed outputs occupy the lower slots.
3. **Enemy OnUpdate (two-byte frontier).** The maintained carrier is
   `3900/3900` bytes with only `+0x19a/+0x1a1` differing. C2 line 68814 / vreg 7
   has live-set mask `0x19E`; class-0 candidates 1 and 2 tie at zero score and
   ordered scan chooses descriptor 1, which the candidate emits as ECX. Retail
   uses EAX only at the early restricted-side draw-group site; the ordinary
   draw-group site already uses EDX in both builds. Descriptor 2's physical
   mapping is not yet independently proved. Next, trace vreg 7 through
   `FUN_10770294`/`FUN_10770248` (live-range overlap and kind-`0x0b` bitset
   interference), with `FUN_10759f2c`/`FUN_1075a28a` as the bitset insert/subtract
   helpers. Do not repeat pointer, helper, parameter-width, same-local reuse,
   or register-hint sweeps. The scoped volatile side-state pointer fixes the
   EAX/EAX pair but regresses four following Float3 argument bytes; do not
   reinstate it.
4. **Smaller owners.** `EffectManager::AddedCallback` remains 51/51 bytes with
   4 differences; `ScreenEffect::CalcShakeEnvelope` remains 336/336 with 26.
   The ANM-manager receiver-order and ScreenEffect/timer register-hint probes
   did not improve either owner. Only reopen them with a new, specific
   live-range hypothesis. Do not repeat the failed ScreenEffect aliases,
   TitleResult table-index forms, or the documented BeginPlaybackStage and
   SaveReplay variants.

## Practical pitfalls

- Compile experiment copies **inside the repository** (e.g. a directory
  under `.analysis/`); compiling from `/tmp` under Wine is ~8x slower.
  Ghidra refuses project paths containing a `.`-prefixed element, so put
  Ghidra scratch projects outside `.analysis/`.
- Title code uses `/O1` without `/Oi`; EffectManager omits `/EHsc`. Check
  `config/match-units.toml` for the owner's unit profile before measuring.
- An automated source-variant search (the Cirno-Labs permuter's rewrite
  functions driven by a local compile/compare harness) was used locally
  and is **not** committed (GPL-3.0 reference code); its results are
  recorded in the knowledge base.
- Repository `.cpp/.hpp` files are CRLF in the working tree
  (`.gitattributes`); edit them with CRLF-aware tools.
- `c2-analysis/` is a local scratch bundle containing the pinned C2 backend,
  Ghidra project/dumps and source probes. Keep it available for the weighted
  allocator investigation, but do not stage its DLL, analyzer database or
  decompiler dumps in source commits.
- A quoted `#include` resolves next to the including file first. To test an
  edited shared `.inl`/`.hpp`, copy the including `.cpp` next to it; `/I`
  order alone does not override `src/`.
- Comparing private `$L` labels is part of exactness: byte-identical edits to
  `EnemyManagerCore.inl` still renumber labels pinned by exact RunEcl and
  CompareOperands units.
- Parallel Wine compiles can leave `_CL_*` compiler scratch files in the
  repository root; delete them.
- `scripts/ci.py --full` runs the 82 target-independent tooling tests in a
  few seconds and must stay green. Source-spelling snapshot tests and
  per-owner inspectors were removed on 2026-11-15; exactness evidence is the
  canonical comparison, and nonexact progress is `check-frontier.py`.

## Storage / protected inputs / commit discipline

Keep `resources/th09.exe`, the `.tools/` Wine/VC7.1 toolchain selectors,
`.analysis/ghidra-direct/`, `_reference/`, and `.analysis/gpt-web/`.
`build/` and Python caches are disposable. Do not commit executables,
analyzer databases, toolchains or decompiler output. Use the commit prefix
the operator requests (`claude-opus-5.5-high:` in the last session,
`gpt-web:` per AGENTS.md otherwise), commit coherent checkpoints, and never
push silently.

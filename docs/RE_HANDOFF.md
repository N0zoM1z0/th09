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
- **Ledger (verified 2026-11-16):** 2,248 candidates, **1,037 source-present
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

## What changed in the 2026-11-16 handoff

- **RunEcl** remains exact (unit `ecl-run-ecl`, 14,792 logical / 15,564
  physical bytes). Its plain-statement cleanup and compiler-label constraints
  are documented in the knowledge base.
- **GameplaySetupThread** is exact at 1,689 bytes and is no longer an open
  owner. **BeginPlaybackStage** remains nonexact at 540/540 bytes with 13
  masked / 19 raw differences; its plain-C++ source is retained.
- **Enemy OnUpdate** remains 3,900/3,900 bytes with two differences
  (`+0x19A/+0x1A1`). The early draw-group C2 node is mapped to vreg 7 and
  source `EnemyManagerCore.inl:434`; C2 selects descriptor 1/ECX on an
  equal-score tie, while the original's EAX preference and descriptor-2
  physical mapping remain unresolved. The scoped volatile side-state pointer
  fixes the pair but shifts four Float3 call-argument bytes; do not restore it.
  The maintained restricted-side fallback uses the shared native `+0x34`
  `EnemySideFlagBits34::bit0` C++ field and a no-sprite early `continue`,
  preserving byte-identical codegen across the 3,900-byte owner and 84 exact
  same-source siblings.
- **SupervisorServiceUpdate** now has a ratchet-safe maintained natural C++
  baseline of **1,629/1,633 bytes / 966 differences** (72 prior mismatches repaired). The 20-byte stack map and `[ebp-4]` result home are understood.
  A separate natural C++ diagnostic (no ASM) is `.analysis/SupervisorNetwork.natural-ebx-reload.cpp`
  (1,591/1,633 / 976 differences): it recovers the branch-scoped EBX clock
  pointer, EDI `connectNow`, and final `0x430fb0` IAT reload, but remains 42
  bytes short. It is not promoted. The maintained improvement makes the
  predicted-frame sides typed reads after their seed callbacks: the first
  reads through the captured `frameNetwork` pointer, the second through the
  refreshed global pointer, preserving retail call-argument order. Next resolve the initial receiver
  (candidate EAX vs retail EDI) and final side-inversion register order.
- **Player CheckBulletCollision** now uses natural C++ iteration, with its
  protected baseline at **842/862 bytes / 705 masked differences**. The old
  853/862 carrier included an artificial `_emit` loop-entry jump/NOP and is
  invalid as natural-source progress. A separate genuine C++ diagnostic
  `.analysis/PlayerCollisionHelpers.natural-preheader-842.cpp` restores the
  original entry jump/alignment through +0x25 and reaches 842/862 / 662
  differences, but adds 102 newly mismatching positions, so it cannot pass
  the monotone frontier ratchet. C2 still places circle-hit dispatch after
  rectangle/AABB geometry instead of retail's early fallthrough; the four
  independent native cursor advances remain to be recovered. Do not promote
  that diagnostic or reintroduce ASM/padding.
- **SaveReplay** remains 1,238/1,238 bytes with two differences; **Player
  movement** remains 1,900/1,900 with eight bytes (four masked); **ChargeAttack**
  remains 1,210/1,210 with 183 masked differences. **ScreenEffect shake** is
  336/336 with 26 differences, and **Effect AddedCallback** is 51/51 with four
  differences. Their failed register/alias probes are recorded in the
  knowledge base; do not repeat simple spelling sweeps.

All open-owner figures above are diagnostic/frontier status, not exactness
claims. The live report on 2026-11-16 confirms **11 nonexact owners / 13,353
logical bytes**, **1,026 exact functions**, and **278,578 / 291,931 authored
bytes exact (95.4260%)**. The complete original-code inventory and native
Windows i386 product remain unproven.

## Remaining source-present nonexact owners

Measured 2026-11-14 with each owner's pinned profile. "Masked" counts
exclude relocation fields; dedicated inspectors may report slightly higher
numbers.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900, **2 bytes** (+0x19A/+0x1A1) (EclManager `/O2 /Ob1`) | Early draw-group vreg 7 maps to `EnemyManagerCore.inl:439`; C2 mask `0x19E` leaves descriptors 1/2 tied at zero and selects 1/ECX first. Identify why the original chooses EAX and how to make ECX unavailable without regressing adjacent Float3 setup. `check-frontier.py enemy-on-update`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238, **2 bytes** (+0xB7/+0xBB) (`/O2 /Ob1`) | Header-copy buffer load ECX vs EDI. The temporary allocation is a genuine `u8 *`, and the first 0x20-byte frame header is copied by natural `ReplayFrameDataStartView` aggregate assignment (`rep movsd` emitted by VC7.1). The 1,238-byte/2-difference owner and twelve same-source exact siblings are unchanged. `replay-save`. |
| **Player movement** | 0x0041C170 | 1,900/1,900, 8 bytes (4 masked) at +0x127 (`/O2 /Ob1`) | Explicit live `GameConfiguration *` followed by freshly read `sideIndex` models retail's intended data access, but VC7.1 still loads the side first; the four masked differences remain. `player-movement`. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51, **4 bytes** in two windows (`/O2 /Ob1`, no `/EHsc`) | Retail uses EDI for the global ANM manager and ESI for callback context; current source reverses the setup order. The shared ANM slot is now resolved through a natural `static inline` helper using one manager receiver across Get/Preload; the 51-byte owner still has four differences, and 15 exact Effect siblings replayed unchanged. `effect-added-callback`. |
| **Title OnUpdateResult** | 0x004266B5 | **484/485**, one site at +0xFD (Title `/O1 /Ob1`, no `/Oi`) | The selected character now uses native `g_TitleResultCharacterOrder[keyboardSelection]` indexing and is retained across the cursor callback; retail still separately loads CL/sign-extends after a `push 2`, whereas VC7.1 folds our read. The 484/485 and 167-difference frontier is unchanged; 41 exact Title siblings replay. `title-update-result`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336, 26 bytes (`/O2 /Ob1`) | Natural direct `screenEffect->timer` member calls replace the old clock alias; ESI/EDI receiver/timer register swap remains. Same-source 18 exact owners replayed; `screen-shake-envelope`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280, 29 bytes (Title `/O1 /Ob1`) | Native `replays[]`, `replayPaths[]`, `frameStart[][]` and replay-name pointer-table indexing replace four integer-address stride helpers; same 29-byte register-allocation residual. The shared replay-record macro's exact 800-byte SaveDraw sibling still replays. `title-draw-replay-menu`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540/540**, 13 masked / 19 raw at +0x14B–0x15F (`/O2 /Ob1`) | Plain C++ (no `volatile` alias). VC7.1 hoists the `sides[0].runtime` load above the +0x37C store; retail issues it after the +0x1E8 load. Statement moves/swaps, array and side-pointer forms do not move it. Native file-to-replay, signed byte/float and frame-stream pointer conversions are typed C++; contiguous playback character/side/score/runtime restoration now uses one actual `AsciiGameManagerView &game` owner without extra loads. The 540/13 carrier and 12 exact siblings are unchanged. `replay-begin-playback`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210, 183 masked (`/O2 /Ob1`) | Retail: int side in EDI, input in EBX, `sideMode` spilled to `[ebp-1]`. Maintained source now uses genuine `ReplayInputState[side]` and `GameConfiguration::sideFocusMode[side]` indexing instead of the old raw-pointer stride and macro; 1,210/183 codegen unchanged. The EDI/EBX allocation remains open. `player-charge-attack`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **842/862**, 705 differing offsets / 26 relocation fields (`/O2 /Ob1`, maintained natural C++) | A separate nested-loop diagnostic reaches 840/862 / 693 and four backedges without assembly, but is 22 bytes short and has wrong entry register homes. The former 853/862 / 692 baseline included a 9-byte `_emit` pad and is retired. `player-bullet-collision`. |
| **Supervisor network service** | 0x00430AA0 | 1,629/1,633 bytes, **966 differences** (maintained); separate diagnostic **1,591/1,633 / 976 differences** (`/O2 /Ob1`) | The 20-byte frame and `[ebp-4]` result home are mapped. Maintained C++ models the early `PopFrame` output overlap with a checked 8-byte union and a second mutually exclusive `ServiceReturnState` union for frame/retry versus wait/reset enum outcomes. These leave the exact byte residual set and extent unchanged. Native packet/seed/input conversions are typed. A saved natural-C++ diagnostic (no inline assembly) now reproduces EBX clock-pointer / EDI `connectNow` allocation and the final `0x430fb0` IAT reload, but is still 42 bytes short and is not promoted. Remaining: initial state receiver EAX vs retail EDI, plus final side-inversion register order. `.analysis/SupervisorNetwork.natural-ebx-reload.cpp`; `supervisor-network-service`. |

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

1. **Player CheckBulletCollision (large boss).** The maintained natural-C++
   carrier is now `842/862` bytes with 705 differences. A 9-byte inline
   `_emit` jump/NOP pad was removed from the loop entry; the old 853/862,
   692-difference result is invalid as progress and must not be restored. Attested target analysis
   fixes the required order: circle test, small hit dispatcher, then rotated
   and AABB geometry; circle success falls through to the dispatcher, while
   square success jumps backward. The separate cancel-reward body is later in
   the target. Retail has four increment/backedge sites at `+0x0a3`,
   `+0x133`, `+0x194`, and `+0x19e`. The previous 840/862, 835/862, 849/862
   and 853/862 measurements were made with the artificial loop-entry pad in
   place; do not use those byte scores as valid natural-C++ progress. The maintained source now uses a natural top-tested `for` loop with
   `continue` rather than the two artificial advance/backedge labels; its
   compiled 842/862 carrier is byte-identical to the post-depadding baseline. The repeated
   `position` home and overlapping collision scratch cursor now use actual
   typed `volatile` references instead of repeated reinterpret-casts; this
   is byte-neutral and all five exact siblings replayed. Next, recover the target preheader and successor order through
   genuine C++ control flow while preserving x87 operand order and four
   distinct backedges. No `_emit`, inline assembly, or byte padding.
   Ignored natural-C++ diagnostic `.analysis/PlayerCollisionHelpers.natural-preheader-842.cpp`
   reproduces the target preheader through +0x25 and reaches 842/862 with
   662 differences, but adds 102 new differing positions (145 repairs).
   It is **not** a ratchet-safe source promotion; next fix C2 circle-hit
   dispatcher ordering before replacing the maintained 842/705 owner.
2. **SupervisorServiceUpdate (large boss).** The retail 20-byte frame is now
   mapped: inputs `-0x14/-0x12`, predicted `-0x0c`, seed outputs `-0x08/-0x06`,
   and result `-0x04`. The early second `PopFrame` intentionally overlaps seed
   `-0x0a` with predicted `-0x08`. The latest saved natural-C++ diagnostic (no inline assembly) is
   **1,591/1,633 bytes / 976 differences** (maintained baseline is now
   1,629/1,633 / 966); it reproduces branch-scoped EBX clock loads, EDI
   `connectNow`, and the final `0x430fb0` EBX reload, but remains 42 bytes short
   and must not be promoted. It is saved at
   `.analysis/SupervisorNetwork.natural-ebx-reload.cpp` with object
   `.analysis/supervisor-natural-ebx-reload.obj`. Direct-return and initial
   reference-alias variants did not improve it. Merging the early and later network aliases into
   one reassigned pointer, or materializing `initialConnectionState`, is
   byte-neutral and must not be repeated. This turn recovered 72
   exact offsets in the predicted-frame `InsertPredictedFrame` side/seed path
   with no new mismatches, using an explicit native unsigned subtraction and
   a live volatile reference to the network side. The remaining high-value mismatch is
   the initial network receiver (candidate EAX vs retail EDI) and the final
   side-inversion order (candidate reads into EDI, retail EAX then ECX). Continue from
   `.analysis/SupervisorNetwork.natural-ebx-reload.cpp` and preserve the exact
   20-byte scratch layout. Do not repeat simple `register` hints, volatile
   receiver qualification, volatile-result or declaration-order probes,
   function-wide global-pointer snapshots, or the initial reference-alias
   variant.
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
   4 differences; `ScreenEffect::CalcShakeEnvelope` remains 336/336 with 26;
   `PlayerLifecycleView::UpdateMovementAndOptions` remains 1,900/1,900 with 4;
   `TitleScreenView::OnUpdateResult` remains 484/485 with 167; `ReplaySave`
   remains 1,238/1,238 with 2. The ANM-manager receiver-order, ScreenEffect/timer,
   PlayerMovement configuration-pointer order, TitleResult direct-index/register,
   and ReplaySave temp-buffer register/alias probes did not improve their
   owners. Only reopen them with a new, specific live-range hypothesis. Do not
   repeat the failed aliases, TitleResult table-index forms, or the documented
   BeginPlaybackStage and SaveReplay variants.

## Natural-C++ source debt (not exactness credit)

The previous artificial `CheckBulletCollision` loop-entry `_emit` bytes have
been removed and rebaselined at 842/862. The unclassified `fsincos` source in
`EtamaControllerDraw.cpp` is also now ordinary `cosf`/`sinf` C++ with
**36/33** bytes and 25 masked differences. Its old x87 exact-codegen unit is
historical evidence only; use `check-frontier.py etama-fsincos` for current
comparison. A source-scoped replay of the other three Etama exact units passes
1,664 compared bytes and 73 relocations. Internal `$L...` COFF label names
were updated only after confirming identical same-section targets.

CI now forbids new inline assembly, raw `_emit` bytes or naked functions.
Other sources still contain genuine x87 inline-assembly helpers
(`AnmManagerCameraFacing.inl`, `AnmManagerRender.cpp`,
`EffectPolarScale.inl`, `ExAttackInitType8.inl`). Retire these through actual
C/C++ semantic reconstruction and scoped owner checks, not byte padding or
manual instruction insertion. Natural trig may differ from the retail x87
combined result in final rounding; do not assert runtime equivalence without
evidence.

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

Preserve `resources/th09.exe`, `.tools/` selectors/toolchain, the saved
`.analysis/ghidra-direct/` project, `c2-analysis/`, and `_reference/`. The old
`.analysis/gpt-web/` proof templates and variant corpora were pruned after
their conclusions were recorded; recreate a narrowly scoped helper only if a
new question needs it. Keep only the current best diagnostic sources explicitly
referenced by this handoff/knowledge base. For the open C2 allocator question,
retain `.analysis/c2-heap.bin`,
`.analysis/FUN_107844a4.decomp.txt`, and the selected GDB recipes
`gdb-c2-member-register-flow.cmd`, `gdb-c2-conflict-predicate.cmd`,
`gdb-c2-source-less-vreg7.cmd`, and `gdb-c2-offset-proof.cmd`. Their captured
stdout/log duplicates and the volatile heap variant are disposable.
`build/`, Python caches, Ghidra packed-db cache/logs, old variant directories,
compiler objects/PDBs, preprocessed files, duplicate debugger logs and stale
scratch snapshots are disposable and may be removed. Do not delete the saved
Ghidra/C2 analysis projects or protected target. Do not commit executables,
analyzer databases, toolchains or decompiler output. Use `gpt-web: ...` for
coherent local checkpoints and never push silently.

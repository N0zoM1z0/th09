# TH09 exact reconstruction — agent handoff (verified 2026-11-13)

**Start:** [AGENTS.md](../AGENTS.md) · [workflow](RE_WORKFLOW.md) ·
[original/VC7.1 Oracles](ORACLES.md) ·
[historical experiments and target layouts](KNOWLEDGE_BASE.md).
This is the **current restart checklist**, not an append-only session log.
Old source/commit histories remain accessible through Git, but ignored scratch
reports and compiled objects are disposable and must not be used as current
exactness evidence. **Start with `git status`, recover any dirty changes,
then regenerate the current ledger counts and needed compiler objects.**

## Verified target, counts and evidence boundaries

- **Target:** original Japanese TH09 **v1.50a**, Windows i386, PE image base
  `0x00400000`, entry `0x0047D45F`. Ignored, protected
  `resources/th09.exe` SHA-256:
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  Do not patch, rename, commit or delete the target.
- **Checked 2026-11-13 against the canonical ledgers:**
  `config/functions.csv` has 2,248 candidates,
  **1,037 source-present authored mappings**, **1,025 exact**,
  **12 nonexact / 28,145 canonical function bytes**, and
  **35 reviewed unresolved origins**. The 1,025/1,037 figure is a
  **98.84% function count**, NOT overall reconstruction completeness:
  exact authored bytes are **263,786 / 291,931 = 90.3590%**.
  Complete original-code inventory is **not** proven. Generated
  [progress](PROGRESS.md) and the command below reflect ledgers;
  counts quoted in old experiment notes are historical. The branch may be
  synchronized with the remote; do not assume an ahead/behind count or a
  particular HEAD from a previous agent message.
- **Important size distinction:** the 28,145-byte nonexact total is the sum of
  `functions.csv` **logical sizes**, *not* the sum of the larger physical
  original-bound comparison extents. In particular:
  RunEcl = **14,792 logical / 15,564 physical** (includes owned tables/tails);
  Enemy OnUpdate = **3,883 / 3,900**; Player movement = **1,835 / 1,900**.
  Those physical extensions add **854** bytes across the three owners.
  Neither number is a new partial-exactness credit.
- **Phase:** exact source reconstruction is **unfinished**. A whole Windows
  i386 compile/link/runtime product, dual-Oracle semantic tests and port are
  **not** proven. `GameplaySetupThread` (1,689 bytes) and **Title DrawResult**
  (1,939 bytes) *are exact*; do not confuse DrawResult with the nonexact
  Title `OnUpdateResult` below.
- **Proof hierarchy:** original disk PE + attested local Ghidra for target
  analysis; pinned VC7.1 COFF for candidate source; canonical
  `config/matches.csv`, `config/functions.csv` and
  `config/match-units.toml` for exact claims. A diagnostic that preserves
  bytes, fields or CFG but has residual differences is **NONEXACT**.
  Verify complete owned code, tails, switch tables and every relocation.
  Private `$L` label names may be rebound only after proving *actual*
  same-section COFF definitions and original PE destinations, not by
  renaming strings blindly.

## Remaining source-present nonexact owners

Rows below are **full original PE comparison extents**, not logical ledger
sizes or partial exact claims. The `--owner` options refer to
`scripts/check-boss-frontiers.py` except where a dedicated inspector is given.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | 15,564/15,564 physical bytes; 598 fields; **30 unlike** | Opcode 4 timer registers (7), opcode 155 timeout flag (23). Owned switch tables intact. `--owner runecl`; `src/EclManager.cpp` with `EclRun*.inl`. |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900; 97 fields; 69 calls; **13 unlike** | Draw-group / bullet-descriptor register lifetimes. `--owner enemy`, `src/EnemyManagerCore.inl` included by EclManager. |
| **Player movement** | 0x0041C170 | 1,900/1,900; 66 fields; 23 calls; **122 unlike** | Option/history, input and Float3 scheduling. `--owner movement`, `src/PlayerMovement.cpp`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210; 70 fields; 58 calls; **235 unlike** | Mode byte / input receiver scheduling; 53-block CFG retained. `--owner charge`, `src/PlayerChargeAttackUpdate.cpp`. |
| **Title OnUpdateResult** | 0x004266B5 | 484 candidate / 485 original; 34 fields; 9 calls; **218 unlike** and 1 missing | Character byte load/sign extension. Dedicated `inspect-title-update-result.py` after building `title-screen-setup-thread`; **not** `--owner resultdraw`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280; 57 fields; 13 calls; **29 unlike** | Selected-record EDX/ECX lifetime. `--owner replaymenu`, `src/TitleScreenReplayMenuDraw.cpp`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | 849 candidate / 862 original; 26 fields; 12 calls; **799 unlike** and 13 missing | Original 33 blocks vs candidate 32; needs genuine branch structure. `--owner player`, `src/PlayerCollisionHelpers.cpp`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238; 53 fields; 23 calls; **2 unlike** | Header-copy EDI/ECX register encoding at +0xB7/+0xBB. `--owner replaysave`, `src/ReplayInput.cpp`. |
| **Replay BeginPlaybackStage** | 0x00420840 | 538 candidate / 540 original; 32 fields; 5 calls; **39 unlike** and 2 missing | RNG seed-load and input-delay tail. `--owner replay`, `src/ReplayInput.cpp`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336; 20 fields; 8 calls; **26 unlike** | ESI/EDI timer/receiver lifetimes. `--owner screen`, `src/ScreenEffect.cpp`. |
| **Supervisor network service** | 0x00430AA0 | 1,627 candidate / 1,633 original; 106 fields; **1,381 unlike** and 6 missing | Waiting/prediction block layout. Compile `src/SupervisorNetwork.cpp` with pinned profile and run `inspect-supervisor-service.py`. `--owner network` checks *different* functions. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51; 4 fields; 2 calls; **8 unlike** | ANM owner/manager EDI/ESI register selection. `--owner effect`, `src/EffectManager.cpp`; protect its exact Create caller. |

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

# Choose ONE open boss. Diagnostic-only: it cannot claim exactness.
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner runecl --skip-exact-siblings

# Preserve all genuinely affected exact siblings after modifying the owner:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl --dry-run
nice -n 12 python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl
# Or use --source src/ReplayInput.cpp for a single source-owned cohort.

# One exact function when needed; builds the selected original-bound carrier:
nice -n 12 python3 -B scripts/build-match-unit.py --unit ecl-pop-context
python3 -B scripts/compare-coff-function.py --unit ecl-pop-context --json

# A separate Title owner (do not substitute exact DrawResult):
nice -n 12 python3 -B scripts/build-match-unit.py --unit title-screen-setup-thread
python3 -B scripts/inspect-title-update-result.py build/matching/TitleScreen.obj

# The standalone, nonexact network service has NO canonical exact sibling:
mkdir -p build/matching
nice -n 12 scripts/compile-probe.sh src/SupervisorNetwork.cpp \
  build/matching/SupervisorNetwork.obj \
  /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src
python3 -B scripts/inspect-supervisor-service.py build/matching/SupervisorNetwork.obj

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

## Immediate next-agent steps

1. Verify the original SHA-256, ledger status and clean/dirty worktree via the
   commands above. A clean tree is **not** a prerequisite for continuing:
   inspect and integrate any interrupted edits before starting new ones.
2. Pick **one nonexact boss** from the table. SaveReplay has only two
   ordinary register bytes left; RunEcl and Enemy OnUpdate have larger
   exact-sibling impact. Build its carrier and inspect its **whole physical
   function**; a `--skip-exact-siblings` diagnostic alone does not certify
   its existing exact neighbors after an edit.
3. Keep a natural source improvement only after comparing original code,
   relocation destinations/addends, calls and relevant exact siblings. Commit
   coherent advances with `gpt-web: ...`; never turn partial similarity
   into a canonical match. The live `docs/PROGRESS.md` must be regenerated
   from ledgers, not edited as a hand-counted score.

## Suggested attack order / experiments already rejected

1. **Replay SaveReplay:** only two instruction register bytes remain.
   Natural `memcpy` casts, constness, buffer pointer locals, `register`,
   `DWORD`/`size_t`, copy and loop-order variants have not changed +0xB7/+0xBB.
   Investigate real value lifetime and original optimization constraints, not
   artificial register directives.
2. **Enemy OnUpdate / RunEcl:** keep the full 3,900-/15,564-byte graph,
   literals, relocations and original jump tables. Draw-descriptor forms still
   differ by 13 register bytes; opcode 4/155 by 7+23. Many bitfield/XOR,
   timer, pointer-alias and source-order variants have failed or worsened
   unrelated exact siblings. Detailed hypotheses in [knowledge base](KNOWLEDGE_BASE.md).
3. **Player movement/charge/collision:** preserve original ABI, calls and
   NaN-aware float control flow. Collision lacks an actual original CFG block;
   padding, cosmetic gotos and changed comparison signs are not a solution.
4. **Other nonexact callbacks:** Title Result vs DrawResult, ReplayMenu,
   ScreenEffect, Effect AddedCallback and network-service timing all have
   separate inspectors. Consult the experiments before repeating rejected
   variants. Research large bosses **and** leaf functions.

Adjacent TH08/TH095 and `_reference/Cirno-Labs-th09` source are read-only
hypothesis material, **never** proof about TH09 original bytes.

## Storage / protected inputs / commit discipline

**Keep** `resources/th09.exe`, the ignored `.tools/wine/` VC7.1/Wine
prefix, the factory-provisioned tools, matched `.analysis/ghidra-direct/`
database and target-attested Ghidra exports, original manifests, `_reference/`,
and the small `.analysis/gpt-web/` private proof scripts and experiment
summaries. Some old scratch output belongs to failed probes, so review it
rather than treating it as current evidence. `build/`, Python bytecode
caches and superseded Ghidra tool-version projects are re-creatable;
they are not canonical source proof. A fresh build may recreate `build/`.

Check `git status` on entry and **do not skip dirty changes**; only this
worktree's current contents are authoritative. Commit actual improvements
as `gpt-web: <what changed>`, save multiple coherent checkpoints when earned,
and never silently push. Do not infer a fixed ahead/behind count from old notes.

## 2026-11-13 continuation checkpoint

At the earlier continuation checkpoint, the live `th09` branch was `main`,
clean, and three commits ahead of `origin/main`. The newest code checkpoint
at that time was `fe6ee2c7`:

- `gpt-web: model replay scratch allocation as void`
- `ReplayManagerView::SaveReplay` now keeps the `Alloc` result as `void *`
  until the LZSS byte-input boundary. Its 1,238-byte frontier remains exactly
  two register-encoding differences at `+0xB7/+0xBB`; all 12 same-TU exact
  siblings passed `check-boss-frontiers.py --owner replaysave`.

Target verification and `validate-tracking.py --require-target` passed before
analysis. Focused non-crediting checks currently report:

- Enemy `OnUpdate`: 3,900/3,900 bytes, 97 fields, 69 calls, graph/switches
  agree, 13 ordinary bytes remain. The 82-byte collision append leaf is
  byte-equal; its two original calls and fields are bound.
- Effect `AddedCallback`: 51/51 bytes, four fields and two calls agree, eight
  register-scheduling bytes remain. Target-attested IDA analysis confirms
  GetAnm slot 8, `etama.anm` fallback and owner offset `+0x2D4`.
- Replay `SaveReplay`: two register bytes remain; no exactness credit.

The full `--owner enemy` attempt stopped during the canonical same-source
cohort phase inside `compare-coff-function.py` while enumerating local COFF
symbols. The diagnostic-only Enemy check still passes. No experimental Enemy
source changes remain. Investigate that specific canonical replay failure
before relying on a full Enemy-owner regression result; do not replace it with
an unbounded project replay.

Continue with both a large boss and a small leaf. Do not repeat the rejected
Enemy draw-group cast, direct four-element subscript, descriptor field-copy,
Effect AddedCallback local-receiver, or SaveReplay pointer-arithmetic
experiments; their outcomes are recorded in `KNOWLEDGE_BASE.md`. Preserve the
`gpt-web: ...` commit format and create further checkpoints only for coherent
source or evidence advances.

## 2026-11-13 Player collision boss frontier

`PlayerLifecycleView::CheckBulletCollision @ 0x0041DFF0` is still open; do not
promote any diagnostic variant. The clean maintained baseline is 849/862
physical bytes, 266/277 instructions, 32/33 CFG blocks, 26/26 relocation
fields and 12/12 ordered calls, with 799 overlapping byte differences and
13 missing target bytes. `CheckGrazeCollision` remains a separate exact
204-byte leaf.

The original loads the `Bullet *` stack argument into EBX at entry, keeps the
active collision-region slot in ESI and spills that slot to `[ebp-4]` before
jumping to the aligned loop header. The baseline instead retains `position`
in EBX and hoists the first region null-check into its prologue. The
2026-11-13 source experiments established that top-level volatile pointer
qualifiers can change these register choices, but the best such candidates
still alter the stack frame, overwrite/reuse the caller argument slot, or
change the required `ret 12` exits. They were rejected and restored; do not
reintroduce them as codegen fixes.

Focused next steps:

1. Reproduce the retail region-loop header and `[ebp-4]` slot spill without
   volatile qualifiers, register hints, or writes to caller-owned argument
   slots.
2. Keep the Bullet/position live ranges and the three `ret 12` exits under
   original-bound comparison.
3. Build only `player-check-graze-collision`, run
   `inspect-player-bullet-collision.py` and compare the exact sibling unit;
   no full cold replay is needed.

## 2026-11-13 Player collision parameter-home progress

The current `CheckBulletCollision` source experiment preserves the original
`PlayerPositionView *` decorated ABI and uses volatile-qualified aliases to
the two incoming pointer parameter homes. This keeps the original argument
stack slots readable while the compiler carries `Bullet *` in EBX. The
candidate now has EDI=`this`, ESI=active region slot, EBX=`Bullet *`, a
0x34-byte stack frame, and three `ret 12` exits. It does not overwrite either
caller-owned argument slot.

Focused pinned-VC7.1 `/O2 /Ob1 /Oy-` evidence from the same source:

- `CheckBulletCollision`: 864/862 bytes, 279/277 instructions, 33/33 CFG
  blocks, 26/26 fields, 12/12 ordered calls, three `ret 12` exits. This is a
  diagnostic candidate only; no exactness credit.
- `CheckGrazeCollision`: structural-exact 204-byte extent; all 176
  non-relocation bytes match and all 28 relocation bytes solve to the reviewed
  destinations. The leaf remains intact while the boss changes.

The remaining boss mismatch is now concentrated in the loop-entry schedule
and one local-pointer spill. Retail first forms ESI=`this+0xB100`, stores ESI
at `[ebp-4]`, jumps to the aligned loop header, then loads `[esi]` into ECX.
The candidate still hoists the first region load/null test into the prologue
and later writes EDX (the region pointer) over `[ebp-4]`; retail retains the
slot pointer there. The next experiment should remove the local region
reference and return to the direct `(*slot)->hitCount2C` update, then test a
volatile active-slot dereference while retaining the parameter-home aliases.
Keep the three exits and both argument homes under every focused build.

Use `scripts/compile-probe.sh` with the `player-check-graze-collision`
profile, `inspect-player-bullet-collision.py`, and the explicit
`compare-coff-function.py` leaf comparison. Do not run a full cold replay.

## 2026-11-13 Player collision slot/exit checkpoint

Committed `2a0306ca` (`gpt-web: preserve collision slot exit paths`). This
supersedes the earlier 864-byte parameter-home candidate described above.
The current `PlayerLifecycleView::CheckBulletCollision @ 0x0041DFF0`
source uses explicit geometry rejection edges and a volatile reference for
the ordinary-hit counter. That counter distinction prevents the compiler from
merging the ordinary-hit and cancel-hit success exits.

Focused pinned-VC7.1 `/O2 /Ob1 /Oy-` result from
`build/matching/PlayerCollisionHelpers.obj`:

- Boss candidate: 878/862 bytes, 278/277 instructions, 32/33 CFG blocks,
  26/26 relocation fields, 12/12 ordered calls, and three `ret 12` exits.
  `inspect-player-bullet-collision.py` remains diagnostic-only: 818
  overlapping linked bytes differ and exactness credit is none.
- The earlier bad `[ebp-4] = EDX` region-pointer spill is gone. The candidate
  retains the slot pointer in the `[ebp-4]` home used by the cancellation
  path, while the ordinary path uses ESI. This is a structural improvement,
  not an exact match.
- `CheckGrazeCollision` remains exact in the same object: 204/204 bytes,
  all seven relocation records and their targets match. Verified with
  `compare-coff-function.py --unit player-check-graze-collision`.

The remaining front-end mismatch is still the loop-entry schedule. Retail
loads Bullet into EBX first, forms ESI=`this+0xB100`, saves ESI at `[ebp-4]`,
then jumps to the aligned loop header that loads `[esi]`. The candidate
currently hoists the first `[edi+0xB100]` region load/null test ahead of the
ESI address formation. It also remains one instruction and 16 bytes larger
than retail. Continue exploring loop-header scheduling without volatile
qualifying the slot cursor itself: that experiment expanded the function to
1,017 bytes / 330 instructions and was rejected.

Do not rerun a full cold replay. Rebuild this translation unit only, retain
the exact `CheckGrazeCollision` sibling check, and keep the boss diagnostic
non-crediting until its full byte/CFG/ABI comparison is exact.

## 2026-11-13 Collision/reward scratch overlay checkpoint

Committed `e1402e15` (`gpt-web: overlay collision and reward scratch`). The
`CheckBulletCollision` source now expresses the four geometry vectors and the
cancel reward's two `EffectFloat3` values as overlapping members of a local
union. This models the retail function's reuse of stack storage after the
geometry phase and prevents the compiler from keeping both lifetimes live at
once.

Focused pinned-VC7.1 `/O2 /Ob1 /Oy-` result from
`build/matching/PlayerCollisionHelpers.obj`:

- Boss candidate: 850/862 bytes, 275/277 instructions, 31/33 CFG blocks,
  26/26 relocation fields, 12/12 ordered calls, and three `ret 12` exits.
  The diagnostic reports 485 differing overlapping bytes, 12 missing target
  bytes, and no extra candidate bytes. This remains a non-exact candidate.
- Stack frame is back to the retail `0x34` bytes. The slot home is
  `[ebp-4]`; `delta` occupies `[ebp-1c..-18]` and `rotated` occupies
  `[ebp-10..-0c]`, matching the target's key geometry temporaries.
- `CheckGrazeCollision` remains exact at 204/204 bytes in the same object.
  Verified with `compare-coff-function.py --unit player-check-graze-collision`.

This is a better ABI/local-lifetime frontier than the preceding 878-byte
candidate, but the loop-entry schedule and two CFG blocks still differ. Do
not promote the boss to exactness. Continue focused experiments on the loop
header and remaining geometry/reward instruction ordering; do not run a full
cold replay.

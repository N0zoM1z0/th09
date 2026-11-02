# TH09 next-agent handoff — 2026-11-02

## 2026-11-02 native Enemy trail and ECL state proof (newer checkpoint)

Independently compared original v1.50a PE and current pinned VC7.1 COFF
for three further native Enemy integration steps. `EnemyView` now owns
callback thresholds/IDs and four child-context pointers at +0x33C0..0x33E7,
rank/life/damage/drop fields at +0x2E38..0x2E63 and +0x3360..0x3367,
then 96 original-size `0x1C` trail samples at +0x33E8, 192 `0x1C`
strip vertices at +0x3E68, and timer/attached-effect fields through
+0x542F. ECL script handlers use the native fields rather than the private
`EnemyCoreView` projection; both share exactly the same trail sample/vertex
C++ types. The complete 15,564-byte `RunEcl` still differs in 30 ordinary
bytes; 96 exact siblings pass. The 3,900-byte Enemy calc boss still differs
in 13 bytes; 88 exact siblings pass. Independently exact ECL operand owners
1980+726+968+2044 bytes, nine Enemy leaf functions and four Enemy draw
owners (including the full 1,758-byte draw callback) all replay exact.
Only 1,005 canonical exact functions are currently credited. These are
source-layout advances, **not** new byte-exact owner credits or whole-program
runtime tests. The private compiler labels were rebound only after auditing
actual COFF-local destinations against unchanged original PE addresses.

## Current original-backed improvement: Player movement

A November 2 local `gpt-web:` source checkpoint updated the complete
`PlayerLifecycleView::UpdateMovementAndOptions` boss at `0x0041C170`.
Direct native side-state accesses remove seven previously different physical
bytes at offsets `0x186/0x187/0x188/0x18A/0x18C/0x18D/0x18E` without
changing any previously equal byte. The fresh full-source comparison has
**1,900 physical bytes, 66 fields, 23 calls and 122 remaining differences**,
not the 129 at the earlier handoff. Both exact dependent owners and the
focused gate pass. `scripts/check-boss-frontiers.py` now rejects regression
at those seven offsets. The source tree and target remain authoritative;
this is not a new exact owner or accepted Factory replay. No full all-owner
cold replay was run; target-independent public CI passed after related source
assertions were updated.

**Read this first**, then inspect the actual Git worktree. The authoritative
project is the original Japanese TH09 **v1.50a**, with ignored local
`resources/th09.exe` SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a **local source checkpoint**, not a pushed branch, a Factory-accepted
current-source receipt, a completed Windows executable, or a runtime proof.

## 2026-11-02 newest exact-source Enemy/ECL integration (live verified)

The following five `gpt-web:` source checkpoints succeeded in **the real TH09
worktree** after a fresh Japanese v1.50a SHA-pinned target check. Read the
actual current Git tree before applying this snapshot: private compiler label
names, local branch ahead/behind state, and dirty state change between builds.

- `9bc9f45b`: The exact ECL operand resolvers now share the real
  `EnemyRewardPlayerView` for both Player position and the `initialStateA0`
  two-word array populated by `PlayerAddedState`; removed their second
  `EclOperandPlayerView` record.
- `24967f6e`: All remote ECL opcode slots use the real
  `EnemyManagerView::remoteSlots2AC388[index]`, instead of a raw byte offset,
  cast, and duplicated remote-table constant. Repeated effectful resolver
  calls and original call order are retained.
- `15de80ee`: Added the genuinely observed per-Enemy +0x2CE4 context-stack
  pointer and +0x2CE8/+0x2D08 integer/float banks to `EnemyView`; 48 operand
  reads/writes now address the real native object.
- `d2688ee1`: The remaining independently observed Enemy motion scalars,
  vector components, +0x2E54 value and +0x2E6C boss timer value now have
  native fields; 48 more exact-resolver reads/writes moved to them.
- `b734c68f`: Removed the **entire duplicate** `EclOperandEnemyView` layout.
  ECL's signed +0x335C item-drop interpretation and the original unsigned
  reward token share a typed union; +0x336B boss slot, +0x3380 flags, +0x33AC
  damage and +0x33B0 life thresholds share their original native EnemyView.
  The two Float3 differences retain explicitly reviewed 12-byte geometry
  reinterpretation without replacing the original TH09 vector call ABI.

Independent source-cold PE/COFF evidence **after the final source change**:

| Actual source original-bound unit | Complete physical bytes | Fields | Verdict |
| --- | ---: | ---: | --- |
| `ecl-resolve-int` | 1,980 | 165 | exact |
| `ecl-resolve-int-lvalue` | 726 | 39 | exact |
| `ecl-resolve-float-lvalue` | 968 | 49 | exact |
| `ecl-resolve-float` | 2,044 | 121 | exact |

All four are **previously exact functions**, not four new ledger matches.
The 15,564-byte RunEcl remains 30 ordinary differences (7 at opcode 4,
23 at opcode 155), 598 bound fields, no table drift, **96 exact dependency
replays passed**. The 3,900-byte Enemy OnUpdate remains 13 differences,
97 fields and 69 calls, **88 exact dependency replays passed**. Source-local
compiler-generated label renamings in the four leaf units and EclManager
carriers were admitted only after checking the actual COFF same-owner label
position against **the unchanged original PE destination**. No field
retargeting, relocation offsets/kinds, or masked code bytes were allowed.

Focused checks: `python3 -B scripts/check-boss-frontiers.py --owner runecl`,
`--owner enemy`, `python3 -B scripts/build-match-unit.py --unit ecl-resolve-int`,
and `python3 -B scripts/compare-coff-function.py --unit ecl-resolve-int --json`
(and the other three resolver units). Targeted native storage and related ECL
tests: **32 passed** after the final edit; `validate-tracking.py
--require-target` reports **1,005/1,035** authored exact. No all-owner cold
replay, whole-image link, accepted factory receipt, runtime validation, or
push was attempted in this checkpoint.

**Rejected hypotheses, not adopted:** eleven bounded opcode-155 timeout flag
source expressions either left the same 30-byte whole-RunEcl residual, or
changed 2,290 physical bytes. The current maintained source was restored and
rechecked; local ignored evidence resides under
`.analysis/20261102-runEcl-opcode155-results.json`. The next difficulty is
**new original-informed codegen evidence**, not repeating old compiler hints
or assuming these historical negative trials settle every possible model.

## 2026-11-02 live continuation: four new local commits (read before dated notes)

These results were checked against a newly verified original v1.50a PE and
fresh source-compiled VC7.1 COFF objects, **not** assumed from this handoff's
older notes. The worktree was clean on entry; the following four changes were
committed in order, all using the required `gpt-web:` prefix:

1. `b9695ed8` — `PlayerLifecycleView::EnterDeathState` now accesses actual
   `movementFields1CCC`, `focusEffect364` and `secondaryEffect368` instead of
   raw pointer arithmetic. The **complete 753-byte** original comparison
   remains exact with **38 original relocation fields**.
2. `0c445c52` — the 15,564-byte `RunEcl` bullet/movement families now use
   native `EnemyManagerView` ANM pointers, the actual side `player04`, and
   the actual `etama08` controller. The source-facing `EnemySideStateView`
   gained a typed, offset-asserted +0x08 controller field. Rebound 20 VC7.1
   compiler-private labels, verifying **each actual COFF-local destination**
   equals the unchanged original PE destination, never editing target VAs.
3. `19bde0be` — restored **107** generated label bindings for the separate
   `AnmManager::ExecuteScript` original-bound /Ob0 object. All 107 independently
   checked actual same-owner COFF label definitions, unchanged 368-field
   identities and original target VAs. Its entire **9,776 / 9,776-byte**
   physical range is still exact. `--owner anm` cold-replayed **17** exact
   siblings and `--owner enemy` replayed **88**, all passing.
4. `73870b62` — finished native late-`RunEcl` source integration: original
   `EnemyManagerView` now exposes +0x2AC3DC protocol and +0x2AC440 timeline
   values; the shared +0x38 side record exposes background00, owner14,
   flags34, and the native Player window exposes position1B88/tableValue303F4.
   **Eleven** late-ECL accesses moved from the duplicate Core manager to the
   shared native owner; compile-time asserts check native-vs-Core offsets.
   Reverified 20 newly generated private labels by target-correct COFF-local
   definitions before changing only their manifest spellings.

**Final focused source-cold gates after these changes:**

```sh
python3 -B scripts/check-boss-frontiers.py --owner runecl   # 96 exact siblings
python3 -B scripts/check-boss-frontiers.py --owner enemy    # 88 exact siblings
python3 -B scripts/check-boss-frontiers.py --owner anm      # 17 exact siblings
python3 -B scripts/build-match-unit.py --unit player-enter-death-state
python3 -B scripts/compare-coff-function.py --unit player-enter-death-state --json
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
```

`RunEcl` still has **30 ordinary mismatch bytes**, zero field/table mismatch
bytes and 146/148 matching opcode shapes; `EnemyManager::OnUpdate` still has
**13 mismatch bytes**, with all 97 fields/69 calls and full block graph intact.
These structural source changes and exact sibling proofs earn **no new exact
function count**: the canonical ledger remains **1,005 / 1,035** authored
functions exact. No all-owner cold replay, link/whole-image replay, or `git
push` was attempted; accepted Factory receipts and runtime validation remain
separate requirements.

**Negative counterchecks, restored rather than adopted:** the complete
1,238-byte `SaveReplay` still has the same 3 mismatches at +0xB7/+0xBB/+0x3F6
after eight bounded frame-copy/checksum C++ variants. `TitleScreenView::DrawResult`
was tested in six more natural screen-snapshot/alias/switch forms: explicit
member syntax remains 1,939 bytes with the same 3 register-byte differences;
three snapshot variants and the boolean-local variant emitted 1,941 bytes,
and the switch experiment emitted 1,964 bytes with a changed CFG. Recompiling
the unchanged source restored the original **1,939-byte/82-field/16-call**
guard result. Trial inputs/reports are ignored under `.analysis/20261102-*`;
none is an accepted original-source hypothesis or a Git-tracked source edit.

The local `main` and `origin/main` relationship should always be read freshly
via `git status --short --branch`; old counts and literal private `$L` names
in earlier handoffs are snapshot-specific and can become stale after shared
header changes. Do not discard dirty incoming changes, overwrite matching
manifest targets, or treat byte-neutral typed-source work as a new exact match.

## Git and ledger at the handoff snapshot

The last **source-changing** commit before this handoff was `e4ea703b`
(`gpt-web: unify both Supervisor ThreadClose receivers with native runtime`).
The local `main` was clean and **52 commits ahead of `origin/main`** at that
instant. Subsequent handoff/cleanup commits advance HEAD; **always check live
`git log` and `git status`**, never hard-code these hashes as latest.

Authoritative ledgers: `config/functions.csv`, `config/matches.csv`,
`config/function-origins.csv` and `config/match-units.toml`. The original-bound
checks at this snapshot report **2,248** candidate functions,
**1,035** authored/source-present, **1,005 canonical exact**,
**30 source-present nonexact** (44,286 bytes), and **37 origin-unresolved**.
Exact authored coverage is **247,597 / 291,883 = 84.8275%**. The
98% milestone, native product link, runtime semantics and ports remain **open**.

## Real current source ownership (historical overlays are obsolete)

- **ANM**: One shared `AnmLoaded` **0x1C** resource record, at offsets
  +0x04/+0x08/+0x0C/+0x10/+0x14/+0x18; the former
  `AnmLoadedResidentView.hpp` 8-byte-only view has **been deleted**.
  `AnmManager::ExecuteScript` is **exact for the full 9,776-byte compared
  extent with 368 original relocations**, not just its 9,358 authored code
  bytes. Resource/load/runtime commits include `182071b3`, `707e9616`,
  `1bcbf9d1` and `4e9b0ca3`.
- **GameManager**: One native `AsciiGameManagerView` with two **0x38** side
  records, each sharing the **0xA0** score/phase allocation and seven
  gameplay subsystem pointers. Replay/Setup use the same +0xB0..+0xE8
  fields. The **1,230-byte/87-field `OnUpdate` is exact**; the **1,689-byte
  `GameplaySetupThread` remains nonexact**, with 172 target vs. 171
  candidate fields, 35 ordered calls, 78/77 direct blocks and **904
  differing bytes**. Important checkpoints include `de83c0d4`, `74145f9e`,
  `ced1017f`, `eeb035e6`, `adeac064`, `a835cef3` and `5dd8be98`.
- **Supervisor**: The actual **0x7B0** owner now includes startup, DInput,
  MIDI, 0xF0 ctor-array frames, queues, deletion, and thread state.
  `StartupThread` **882 bytes /106 fields exact**, `DeletedCallback`
  **405/44 exact**, `ThreadClose` **21/0 exact**, DInput enum callback sizes
  **93/94/94 exact**; frame queue pop **119** and predicted insert
  **191** exact. **`Supervisor::OnUpdate` remains NONEXACT:** target physical
  extent **996**, candidate **952**, 92 fields/27 calls, 791 overlapping
  differing bytes and 44 absent bytes. Newer commits include `f359c201`,
  `e4ea703b`, `0b84231f`, `79612f34`, `f60b1f61`, `2fa230b7`,
  `22cf89a8`, `5235f666` and `73bf0ed2`. Do not resurrect
  `SupervisorUpdateLayout` or any old full-size startup/deletion overlays.
- **Other unsolved major owners**: `RunEcl` **15,564 bytes /598 fields,
  30 ordinary differences** (opcode 4 has 7; opcode 155 has 23);
  `EnemyManager::OnUpdate` **3,900 bytes /97 fields/69 calls/13 different
  bytes**; `ReplayManager::SaveReplay` **1,238 bytes/53 fields/23 calls,
  three differing bytes** at +0xB7/+0xBB/+0x3F6; Etama `OnDraw`
  **600 bytes/268 differences**; Title `DrawResult` **1,939 bytes/three
  differences**. Similarity, unchanged exact sibling replays and authored
  local source files **are not new exact matches**.

## Where to attack, and what already failed

The complete `RunEcl` function has valid original-bound calls, relocations
and switch tables; the remaining two instruction families are register/codegen
problems. Natural opcode-4 raw-int direct-access variants widened differences
from 30 to 42, and opcode-155 byte/word lifetime rewrites disrupted subsequent
switch tables. All failed trial source changes were reverted.

`EnemyManager::OnUpdate` has all 1,026 instructions, 69 call identities,
97 relocations and 170 CFG blocks aligned; 13 nonfield byte differences
are two draw-group register choices plus 11 differences in the effect/stack
descriptor path. Reordering side-state or descriptor lifetimes either made no
change or moved original literal fields. Failed trials were reverted.

`GameplaySetupThread` has a real register-base mismatch: the target indexes
side records using `+0x34` (flags) while the candidate prefers `+0x28`
(selector); dozens of pointer declarations/register/flags-anchor rewrites
failed to improve its 904-byte residual. Preserve the authentic 35 calls,
not a forced target-byte patch. The historical Title barrier is a narrow
source-specific no-op ordering constraint, **not** a generic compiler trick.

## Fast safe restart and scoped verification

From the registered live `th09` repository, with no pending other agent:

```sh
git status --short --branch
git log -5 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/validate-docs.py
```

Build **only the touched source/carrier** before comparison. For example:

```sh
python3 -B scripts/build-match-unit.py --unit supervisor-startup-thread
python3 -B scripts/compare-coff-function.py --unit supervisor-startup-thread --json
python3 -B scripts/check-boss-frontiers.py --owner supervisor
python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
python3 -B scripts/check-boss-frontiers.py --owner runecl --skip-exact-siblings
```

The `--skip-exact-siblings` option is for quick boss exploration, **not**
verification of a source change's affected exact siblings. Check them locally
before commits; `docs/TH09_SCOPED_EXACT_REPLAY.md` documents selective
carrier/profile replays. Never run a full `--owner all` cold replay for
ordinary iterations. After artifact cleanup `build/matching/*.obj` can be
absent: **regenerate them**; historical .analysis objects are not a fresh
current-source proof. Treat VC7.1 compiler-private `$L` names as generated:
only rename after proving their COFF-local destinations match the unchanged
original PE addresses, never alter original destination/relocation kinds to
make a test pass.

The exactness ledger remains **1,005** until a genuinely new complete,
original-bound proof is reviewed. Use `gpt-web: ...` for coherent local Git
commits, **preserve incoming dirty work**, and do not push without explicit
instruction. Historical KB and 2026-10-31 reports are evidence for their
specific snapshots, not current-source specifications.

## Storage / preserved proof

`.analysis/` holds ignored experiment scripts, candidate sources and
original-bound JSON results. `build/` and analysis .obj/.pdb/.pyc are
generated compiler caches. This handoff removed **1,395 generated files /
175,672,705 deleted bytes** (~167.5 MiB), including 528 hash-reviewed
old variant objects; the net logical reduction after writing audit records
was ~167.1 MiB. **755** unreviewed analysis objects remain deliberately.
For a new preview use `python3 -B scripts/clean-generated-artifacts.py`
without the already-consumed `--reviewed-objects` manifest. The dated cleanup summary is in
`docs/RE_ARTIFACT_CLEANUP_HISTORY.md`, and the actual reviewed-path audit
is under ignored `.analysis/handoff-cleanup-20261013/`. The original target,
Git-tracked source/ledgers, unreproducible analysis evidence, native Ghidra
projects and the installed toolchain were preserved. This work never called
`git push`. The local `origin/main` tracking ref was observed equal to
the docs HEAD after the prior commit; tracking refs are **not independent
evidence** that a remote branch was published. Do **not** delete the
remaining JSON source/provenance packets or `.analysis/` en masse merely
because earlier diagnostics were nonexact. Some historical paths named by
old reports were already cleaned and must be rebuilt or recovered via their
documented source/hash inputs.

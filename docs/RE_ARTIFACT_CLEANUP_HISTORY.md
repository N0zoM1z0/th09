# TH09 artifact cleanup history and preservation review

> For current cache state, use the [active handoff](RE_HANDOFF.md).
> Historical cleanup counts are not presently reclaimable amounts.
> **Never delete kept candidate sources or unique evidence simply by age.**

### 2026-10-20: SaveReplay source progress and bounded cache removal

From pushed source parent `26e0fd3`, maintained SaveReplay improves from 152 to
3 complete linked byte differences with no new mismatching position. It is
still NON-EXACT. The focused Oracle passes all 12 exact same-source owners
across two compiler profiles; unrelated source/profile proofs are reused.
Pre-clean CI passes **809 tests /28 skips**, plus tracking, documentation,
progress and whitespace. Native product/runtime gates remain open.

The current session's 93 source/report/helper files are first frozen in
`.analysis/gpt-61sol-20261020-parser-replay/exploratory-carriers.zip`. Full
readback hashes and ZIP CRC verification cover every member. Its manifest
binds 41 source/report cases, profiles, transitive local headers and compiler
pins. The selected stage-indexed candidate is cold-replayed from that archive,
reproducing all 1,238 raw bytes and all 53 actual COFF records. It earns no
exact credit. Only verified duplicate loose members are replaced by this
archive; the failed counter-scope source is also preserved without a result.

After CI, a TH09-cwd-scoped `/proc` check finds no active compiler/CI producer.
The cleaner selects only this manifest's `build/diagnostics/sol-<case>.obj/pdb`
regular ignored files, confirms none is tracked, and rechecks each size/SHA
immediately before unlinking. The exact 83-file deletion manifest is
`.analysis/gpt-61sol-20261020-parser-replay/cleanup.json`.

| Actually removed/replaced | Files | Logical bytes |
| --- | ---: | ---: |
| Loose source/report duplicates, replaced by verified ZIP | 93 | 2,054,858 |
| Session-owned reproducible OBJ/PDB caches | 83 | 4,924,968 |
| Retained verified ZIP | 1 | 370,685 |
| **Net reduction** | | **6,609,141 (~6.30 MiB)** |

Canonical matching objects are retained for unchanged source/profile reuse.
The private target, pinned tools, IDA databases and older unclassified analysis
are untouched. CI precedes final cache deletion; post-clean original-bound
SaveReplay/target/ledger/docs/progress/Git checks follow. No whole build tree
or age-based cleanup is performed.

### 2026-10-20: local handoff, complete Charge diagnostic and frozen controls

The operator requested cleanup/handoff after clearing the long-running goal;
the 95% milestone remains unfinished. Baseline `085343b` and this checkpoint
have identical 485 game-source files,1,026 manifest units and 1,000 exact rows:
243,428/291,883 reviewed-authored bytes, 83.3992%. Recoverable dirty work was
the complete Charge auditor and its five pure test groups; both were reviewed
and retained. The cleaner now preserves `.idb`, whose suffix is ambiguous with
IDA databases, with explicit `.idb`/`.i64` preservation checks.

**Proof before deletion:** direct local IDA metadata and distributed mapped
bytes agree with the verified Japanese v1.50a target. All 24 Charge models pass
the new complete 1,210-byte/70-field/58-call input checks; none is exact.
Four genuine inline Effect callback models remain51/4/8. Selected Charge and
Effect cold replays reproduce every captured raw byte/actual COFF row and full
original comparison. CI passes **796 tests, 28 skipped**, with tracking,docs,
progress,open build graph and whitespace checks. No new runtime/product claim.

Before removing duplicate stages,28 full comparisons and all candidate inputs
were frozen into36 Charge/18 Effect unique content-addressed inputs,with
profiles,tool/SDK hashes and a one-case replay. Per-path deletion records live
in `.analysis/gpt-61sol-20261020-charge-receiver-owner/`:
`cleanup-result.json` and `cache-cleanup-result.json`. Both deletion phases
require a TH09-cwd-scoped `/proc` check with no active compiler/CI producers;
selected paths are ignored,regular files and rechecked by SHA before deletion.

| Actually deleted | Files | Logical bytes |
| --- | ---: | ---: |
| Frozen/reproducible duplicate candidate stages, including OBJ/PDB | 468 | 4,138,775 |
| Generated CI `build/` | 364 | 6,169,370 |
| Remaining `.analysis/` Python cache | 1 | 6,342 |
| Reviewed `scripts/` and `tests/` bytecode caches | 41 | 839,459 |
| **Total** | **874** | **11,153,946 (~10.64 MiB)** |

Postconditions: `build/` and both top-level Python caches are absent; cleanup
dry-run selects zero files; original target SHA remains
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
All tracked input and source/profile bindings are retained. Around 1.6 GiB of
older `.analysis/` evidence remains deliberately unclassified or still useful;
no age-based deletion or provider/toolchain/IDA cleanup was performed.
CI ran before final cache deletion; only cache-free target/docs/ledger/Git
checks follow it. A later reconstruction must rebuild its affected object.
Commit and independently verified remote push are recorded separately.

### 2026-10-19: 257-exact original-bound MusicRoom handoff and generated-cache removal

**Proof first, cleanup second.** On current gameplay-source commit
`7324b4ecc5b0eb77b2245751b6b2c4732f794e9a`, the SHA-attested original
Japanese TH09 1.50a target passed `check-boss-frontiers.py --owner all`:
**35 complete noncrediting original-linked boss/leaf groups and 257 DISTINCT
complete already-exact function checks**. The three independently accepted
Factory MusicRoom/DrawTextLeft receipts are archived in
[`RE_ACCEPTED_RECEIPTS.md`](RE_ACCEPTED_RECEIPTS.md) and bound only to this
gameplay-source commit. The fresh pre-clean `scripts/ci.py` passed **659 tests,
28 skipped, zero failed**, plus exact-ledger, documentation, original-target,
progress and build-graph checks. This does not create any new exact ledger
credit: **940/980** remains the canonical count. Ignored reproducible
witnesses: `.analysis/handoff-cleanup-20261019/precleanup-full-boss.json`,
`precleanup-ci.log`, `target-verification.txt`, `source-commit.txt`.

**Explicit generated-cache preflight:** Git's tracked path inventory for
`build/` and `.analysis/` was empty, the worktree was clean and neither root
was a symlink. The immutable original target SHA remained
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Every extra `scripts/__pycache__` and `tests/__pycache__` file was an untracked,
regular **`.pyc` only**, checked by SHA-256 before and again during deletion.
The reviewed dry-run and individual bytecode hashes live in
`.analysis/handoff-cleanup-20261019/reviewed-preflight.json`.

| Approved, actually deleted category | Files | Logical bytes |
| --- | ---: | ---: |
| Entire ignored generated `build/` tree | 523 | 18,442,237 |
| Compiler `.obj/.pdb/.pyc/.pch/.idb/.ilk` inside `.analysis/` | 997 | 111,683,705 |
| Separately SHA-256-reviewed Python caches in `scripts/` and `tests/` | 50 | 892,319 |
| **Verified total** | **1,570** | **131,018,261 (~125.0 MiB)** |

**Postconditions checked after actual deletion:** `build/` and both top-level
`__pycache__` roots no longer exist; a second
`PYTHONDONTWRITEBYTECODE=1 python3 -B scripts/clean-generated-artifacts.py`
dry-run selected **zero files and zero bytes**; Git tracked source/ledgers and
the private original SHA remained unchanged. Precise removed compiler paths
are the **latest `runs` entry**, not the lifetime cumulative count, in
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`. Reviewed
per-file bytecode SHA and exact post-clean results are recorded in
`.analysis/handoff-cleanup-20261019/verified-deletion.json`. That audit
records the current docs-only HEAD before cleanup as `7239806` and preserves
the distinct source-commit binding to `7324b4e`.

**Intentionally preserved:** private `resources/th09.exe`, pinned `.tools/`,
tracked source/config/docs/tests, and roughly **1.6 GiB allocated** of
ignored `.analysis/` original PE/COFF JSON, source/INL variants, old experiment
scripts and unclassified research provenance (**106,690 retained files** at
post-clean review). These historical materials are not automatically
reproducible solely by extension, and were **not** bulk-deleted. Do not
mistake retained, currently unused `.analysis/` study sources for safe cache.
Before deleting a specific old campaign, review its producer, unique target
observations, source-commit binding and tracked documentation references.

**Handoff:** no CI or compiler replay is rerun *after* the cache deletion,
since it would recreate ignored binaries and erase the cache-free result.
Use `PYTHONDONTWRITEBYTECODE=1` plus `python3 -B` for documentation-only
post-clean checks. The next operator starts with the clean Git worktree,
attests target/ledgers, and runs `python3 scripts/build-match-unit.py --unit NAME`
before any COFF inspector. A missing `build/matching/*.obj` is intentional,
not a missing original function or proof.

### 2026-10-15 cleanup execution and preservation audit

The clean baseline was checked **before deletion**: target and tracking passed,
all **29** original-bound non-exact boss/leaf cold diagnostics passed, all
**65** related existing exact unit comparisons passed, and `scripts/ci.py`
passed **347 tests (27 skipped)**. Then the existing reviewed cleanup script
removed exactly **4,825** ignored generated files totaling **366,037,887
bytes** (about **349 MiB**): **388 `build/` files** / 9,498,720 bytes and
**4,437 `.analysis/` compiler artifacts** (`.obj/.pdb/.pyc/.pch/.idb/.ilk`)
/ 356,539,167 bytes. Separately, **67** ignored Python bytecode cache files
under `scripts/__pycache__` and `tests/__pycache__` were removed (847,340
bytes). The follow-up `clean-generated-artifacts.py` dry run reported **zero
remaining selected cache files**. The per-path local deletion audit is in
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`; only its
**latest run**, not its cumulative historical total, describes this sweep.

`build/` is intentionally **absent** at handoff. Rebuild the necessary
match-unit or diagnostic owner fresh before calling its inspector; running
those commands again will recreate disposable files. **Do not rerun CI solely
to confirm deletion** (it has already passed, and a new run may regenerate
bytecode). The ignored `.analysis/` still occupies roughly **1.2 GiB on disk**:
mainly old `.cpp/.inl/.hpp` experiment copies and JSON evidence across
previous GPT-web/GPT-dots campaigns. They are **retained pending individual
reproducer/consumer review**, not advertised as fresh or accepted evidence;
the remaining size is not a failed build-cache cleanup. Preserve
`resources/th09.exe`, `.tools/`, tracked C++ and manifests, and any analysis
material whose unique source binding is not established. Before further
archival pruning, audit a specific campaign against its `probe.py`,
`results.json`, source commit and tracked knowledge; never delete it by age or
extension alone.


### 2026-10-16: independently reviewed cache cleanup and preservation

**Preconditions:** At code/document checkpoint `88f0df8` on `main`, the
private Japanese TH09 v1.50a original SHA matched the canonical
10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822.
The formerly untracked replay-menu inspector was adopted and committed
with original physical SHA, 57-field/13-call/72-block tracking and four
negative tests. The large handoff was split into an active summary plus
[historical accepted receipts](RE_ACCEPTED_RECEIPTS.md); no accepted
receipt's original source-commit binding was changed.

A complete cold `check-boss-frontiers.py --owner all` verified **30
noncrediting original-bound function owners and 68 already-exact sibling
units**, including the 1,280/1,258-byte DrawReplayMenu. Public CI
passed **416 tests, 27 skipped**; original PE/tracking, docs,
progress and diff checks passed. Cleanup was run *after* these checks,
not as a substitute for them. The initial scoped preflight retained in
`.analysis/handoff-cleanup-20261016/reviewed-preflight.json` checked
`git ls-files` for all cache roots, refused symlink roots, and rehashed
the original target before deletion.

**Actual independently counted deletion:**

| Reviewed reproducible artifact | Files | Bytes |
| --- | ---: | ---: |
| Wholly ignored generated `build/` | 415 | 12,570,896 |
| Ignored `.analysis/` compiler OBJ/PDB/PCH/ILK/IDB/PYC | 3,070 | 196,077,550 |
| `scripts/__pycache__/`, verified only .pyc | 33 | 486,249 |
| `tests/__pycache__/`, verified only .pyc | 21 | 266,253 |
| **Total** | **3,539** | **209,400,948 (199.7 MiB)** |

`python3 -B scripts/clean-generated-artifacts.py --apply` removed
the first two categories; the two exact Python cache roots were removed
separately only after checking their bytes/file counts and that **every
item was ignored, untracked and .pyc-only**. The precise local per-file
audit for the generated subset is in
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`
(its **latest runs entry**, not the aggregate total). The separate
ignored full outcome is
`.analysis/handoff-cleanup-20261016/verified-deletion.json`;
the original-bound source-cold witness is
`.analysis/handoff-cleanup-20261016/precleanup-full-boss.json`.
A new dry run showed **all zeroes**: neither the generated build tree
nor compiler objects remained. No non-cache file was erased.

**Preserved deliberately:** original `resources/th09.exe`, `.tools/`,
tracked source/config/docs/tests/scripts, IDA/provider state, all ignored
compiler-experiment C++/INL/HPP and reproducibility Python scripts,
SHA-bound JSON, source-diff outputs and legacy-unknown roots. After
cleanup, retained `.analysis/` has **93,469 files, 6,271 subdirectories,
1,058,738,306 logical bytes (1,009.7 MiB)** and about 1.3 GiB
allocated, dominated by **65,151 .cpp (450.6 MiB)**,
**5,143 .json (377.9 MiB)**, **7,800 .inl (100.5 MiB)**,
**75 .toml (24.5 MiB)** and **8,056 .hpp (22.0 MiB)**. Not all
historical experiments have independent producer/source-commit/reference
classification, so they are explicitly **retained legacy-unknown or
still-useful unresolved probe evidence**, not approved for blind pruning.
In particular, source probes for RunEcl, ExAttack Type19/21, ANM
CameraFacing, ScreenEffect shake and Title replay remain reproducible
without their removed .obj/.pdb files.

**Future handoff lifecycle:** the absence of `build/` is normal.
Each comparison must rebuild its actual code carrier. CI sets
`PYTHONPYCACHEPREFIX=build/pycache` itself, so rerunning CI after
cleanup recreates disposable files even if the caller disables Python
bytecode. For a cache-free handoff run CI and cold Oracles **before**
deleting generated caches; verify deletion via a post-run dry-run.
Never interpret historical Factory receipts as exactness acceptance
for a newer gameplay-source commit.

### 2026-10-17: 31-owner/72-exact full replay and reviewed generated-cache cleanup

**Clean baseline:** `main` at gameplay-source HEAD `944d95c`, original
Japanese TH09 v1.50a executable SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`,
**980 authored source functions / 940 complete exact / 40 still nonexact**.
`python3 scripts/check-boss-frontiers.py --owner all` recompiled and passed
**31 original-linked owner diagnostics (30 open plus one separate already
exact crypto selector diagnostic)** and all **72 existing exact sibling
replays**, with no new exact function credited. `python3 scripts/ci.py`
passed **437 tests (28 skipped)**; target/tracking/docs/progress and
diff checks passed. The original-bound pre-deletion replay, stdout/stderr,
CI and target reports are in ignored
`.analysis/handoff-cleanup-20261017/`.

**Scope and actual deletion:** `python3 -B scripts/clean-generated-artifacts.py`
dry-run identified **5,234** wholly generated files; `--apply` removed exactly
**349,490,597 logical bytes**. This comprised 376 ignored `build/` files
(9,539,846 bytes) and 4,858 ignored `.analysis/` compiler-suffix files
(339,950,751 bytes), after checking that neither root was a symlink and
that neither contained Git-tracked files. A separate fail-closed audit
verified that both `scripts/__pycache__/` and `tests/__pycache__/` contained
**only** 39 ignored, untracked, regular `.pyc` files; these contributed
656,008 more cleaned bytes, with per-path SHA-256 records retained. The
aggregate deleted set was **5,273 files / 350,146,605 logical bytes
(334.0 MiB)**. The approved cache cleaner's exact path manifest is the
**most recent `runs` entry** of
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`;
`.analysis/handoff-cleanup-20261017/pycache-cleanup-audit.json` covers the
other 39. A second dry-run reported zero remaining generated cache files.

**Preservation and postconditions:** source/config/docs/tests, all ignored
experiment sources, original-linked PE/COFF JSON, scripts and earlier handoff
receipts are retained. No user-supplied original exe, `.tools`, provider/IDA
input or third-party game data was deleted. Git worktree remained clean;
original target SHA remained unchanged. The remaining `.analysis/`
occupied approximately **1.4 GiB allocated** and contains ~100,000
retained evidence/source files. In particular, historical experiment C++
source variants and ~400 MiB of JSON evidence were **not** removed because
not all have individually verified reproduction/source-commit histories.
Preserve them or separately audit and archive specific campaigns; do not
bulk-delete `.analysis/` by age/extension. The absence of `build/` is normal
and a future owner comparison must rebuild its matching object first.
**No CI was rerun after deletion**: doing so would regenerate caches; the
original-bound full pre-cleanup CI and replay above remain the source proofs.
Python `-B` alone does not propagate to Python launched as a child process
by a validator; for final cache-free document checks also export
`PYTHONDONTWRITEBYTECODE=1`. One such validator-created bytecode file was
noticed during final audit and re-cleaned within the same approved `.pyc`
whitelist; this did not affect the 5,273 **distinct cache paths** cleared.


### 2026-10-18: 35-owner/204-exact clean handoff with historical-checkpoint archival

**Verified evidence before deletion:** original Japanese TH09 v1.50a
`resources/th09.exe` retained SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The tracked source/ledger baseline was `0df4c1d` on `main` (**980
source-present / 940 canonical exact / 40 nonexact**, 49,183 open physical
bytes). The target-bound `python3 -B scripts/check-boss-frontiers.py --owner
all` freshly rebuilt and passed **35 noncrediting original-linked owner
checks and 204 *distinct* already-exact full-function comparisons**.
`python3 -B scripts/ci.py` passed **584 tests (28 skipped, zero failed)**,
including the new documentation archive/anti-journal tests, plus tracking,
original/build graph, docs, progress and whitespace checks. These witnesses
were saved under ignored `.analysis/handoff-cleanup-20261018/` **before**
removing any compiled object. The 204 checked units are a regression-suite
cardinality, not 204 new codegen acceptances.

**Explicit, fail-closed scope:** `clean-generated-artifacts.py` previews and
removes only the wholly ignored `build/` cache and compiler binaries/bytecode
(`.obj/.pdb/.pch/.idb/.ilk/.pyc`) under the ignored `.analysis/` tree.
The extra `scripts/__pycache__` and `tests/__pycache__` directories were
individually checked for ignored, untracked, regular **`.pyc` files only**,
with each file's bytes and SHA-256 recorded in `reviewed-preflight.json`
*before* unlinking. Every bytecode hash was checked again before deletion.
No symlink cache roots, tracked cache files, unexpected Python cache entries,
or unstaged source edits were allowed into the deletion set.

| Approved, actually deleted artifact | Files | Logical bytes |
| --- | ---: | ---: |
| Wholly ignored generated `build/` | 486 | 15,757,511 |
| `.analysis/` compiler binaries and bytecode only | 2,326 | 197,796,698 |
| Separately SHA-256-reviewed Python bytecode caches | 68 | 1,069,658 |
| **Total** | **2,880** | **214,623,867 (~204.7 MiB)** |

**After deletion:** `build/`, `scripts/__pycache__` and `tests/__pycache__`
are absent. A post-apply `python3 -B scripts/clean-generated-artifacts.py`
returned **zero candidate files and zero bytes**. SHA-256 of the original
game remained unchanged; Git-tracked code, manifests and docs were retained.
The cleanup script's append-only precise path inventory is kept in
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`:
use the **last `runs` record**, not the cumulative top-level summary, for
this deletion. The separate Python bytecode file hashes, preflight approval,
postconditions and original/CI witnesses are in:

- `.analysis/handoff-cleanup-20261018/reviewed-preflight.json`
- `.analysis/handoff-cleanup-20261018/verified-deletion.json`
- `.analysis/handoff-cleanup-20261018/precleanup-full-boss.json`
- `.analysis/handoff-cleanup-20261018/precleanup-ci.log`

**Deliberate preservation:** roughly **1.5 GiB allocated** remains in
ignored `.analysis/`: **104,476 files / 1,263,262,173 logical bytes** of
legacy and recent C++/INL/HPP probe sources, JSON original-PE/COFF evidence,
reproducers, analysis snapshots and uniquely sourced or still-unclassified
artifacts. These were not declared disposable by extension or age. Preserve
operator-input `resources/th09.exe`, installed `.tools/`, IDA databases,
real source/ledgers/receipts and all unknown legacy experiments. This is a
bounded **cache** cleanup, not historical evidence deletion; never run
`git clean -X` or `rm -rf .analysis` as a shortcut.

**Documentation handoff:** the former **683 detailed chronological checkpoint
lines** were frozen (not thrown away) in
[`RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md`](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md).
The active [`RE_HANDOFF.md`](RE_HANDOFF.md) shrank from **968 lines to a
concise current routing document**, with live ledger, priority bosses and
small leaves, exact validation commands, and the artifact boundary.
`validate-docs.py` and tests now fail on dated checkpoint bullets or an
append-only multi-hundred-line active checkpoint section. Accepted original
Factory receipts remain source-commit-scoped in `RE_ACCEPTED_RECEIPTS.md`.

**Restart:** missing `build/matching/*.obj` is intentional. Start with
`git status`, target+ledger verification, then cold-build the exact source
unit before a COFF comparison. Full CI/`--owner all` **will regenerate
ignored build caches**. No full CI was rerun *after* cleanup, since doing so
would undo the cache-free handoff; post-deletion doc-only verification must
use `PYTHONDONTWRITEBYTECODE=1` **plus** `python3 -B` to avoid recreating
Python bytecode. The original-bound pre-deletion results remain the proof.

# TH09 artifact cleanup history and preservation review

> For current cache state, use the [active handoff](RE_HANDOFF.md).
> Historical cleanup counts are not presently reclaimable amounts.
> **Never delete kept candidate sources or unique evidence simply by age.**

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

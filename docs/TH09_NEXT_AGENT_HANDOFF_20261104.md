# TH09 — current next-agent handoff (2026-11-04)

**Read this first.** This is a **routing and audit snapshot**, not a Factory
acceptance receipt. The Git worktree, `config/functions.csv`,
`config/matches.csv`, `config/match-units.toml`, the SHA-pinned Japanese original
and **fresh** source-profile/PE/COFF evidence supersede every dated note here.
The [2026-11-03 handoff](TH09_NEXT_AGENT_HANDOFF_20261103.md),
[historical handoff](RE_HANDOFF.md) and
[knowledge base](KNOWLEDGE_BASE.md) intentionally retain older experiments:
any old "current" claim, duplicate view or negative result may be obsolete.

## First 60 seconds — recover the one real worktree

```bash
git status --short --branch
git diff --check
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
```

Treat all staged, tracked, untracked and relevant ignored experiments as **the
same agent's in-progress work**. Recover them, do not avoid or silently reset
them. The last source commit before this handoff preparation was
`f2e66154 gpt-web: restore native CardAttack VM and 923-byte StartAttack
GameManager fields`; **it is not a promise about a later HEAD**. Commit each
coherent verified improvement as `gpt-web: <short English summary>` and make
multiple commits when there are multiple results. Do not push without an
explicit instruction. A local commit is not a Factory accepted replay or proof
of remote publication.

Original input: **Japanese TH09 v1.50a** at ignored `resources/th09.exe`,
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Never replace it with another distribution/version or commit it. Use the pinned
VC7.1 compiler to test complete function **and** owned tables/tails, actual
COFF symbols, every relocation kind/addend/destination, and the original PE
operand word. Ghidra, when useful, must be reattested to the original mapped
bytes and is not by itself an exactness oracle.

## Canonical progress — rechecked from live ledgers on 2026-11-04

| Fact | State |
| --- | ---: |
| Candidate navigation inventory (not proven complete) | 2,248 |
| Confirmed authored, source-present functions | **1,035** |
| Locally exact authored functions | **1,005** |
| Source-present nonexact functions | **30** |
| Exact / reviewed authored bytes | 247,597 / 291,883 (**84.8275%**) |
| Source-present nonexact authored bytes | 44,286 |
| Reviewed but unresolved origin/boundary | 37 |
| Authored without maintained source | 0 |

**84.8275% is not a whole-game completion percentage.** Target inventory
closure, faithful Windows i386 full-product linking, runtime equivalence,
semantic reconstruction and ports remain independent open gates. Historical
[accepted receipts](RE_ACCEPTED_RECEIPTS.md) are bound to their source commit,
not acceptance of this HEAD. Two locally PE-exact source constructors at
`0x0041F010` (**23 bytes**) and `0x0041F030` (**44 bytes**) remain explicitly
**unclassified** as developer-authored versus compiler-generated and are
**not counted** in the 1,005; see `scripts/inspect-player-{option,shot}-constructor.py`.

## Current important nonexact *whole-owner* frontiers

These are diagnostic baselines from focused original-bound local comparisons;
recheck the affected owner before claiming improvement. They are **not** new
exactness credit.

| Function / scope | Target / natural candidate | Residual and review scope |
| --- | --- | --- |
| `RunEcl` — `--owner runecl` | 15,564 / 15,564 | **30** unlike bytes, opcode 4 = 7, opcode 155 = 23; 598 address fields; **96 exact siblings** |
| EnemyManager `OnUpdate` — `--owner enemy` | 3,900 / 3,900 | **13** unlike bytes, 97 fields, 69 calls; **88 exact siblings** |
| Player `UpdateMovementAndOptions` — `--owner movement` | 1,900 / 1,900 | **122** unlike bytes, 66 fields, 23 calls; **2 exact siblings**; **129 is obsolete** |
| `ReplayManagerView::SaveReplay` — `--owner replaysave` | 1,238 / 1,238 | **3** unlike bytes at `+0xB7`, `+0xBB`, `+0x3F6`; 53 fields, 23 calls |
| Title `DrawResult` — `--owner resultdraw` | 1,939 / 1,939 | **3** unlike bytes at `+0x1D`, `+0x23`, `+0x39`; original/VC7.1 register allocation differs |
| Supervisor `OnUpdate` — `--owner supervisor` | 996 **owned** / 952 owned | **44 absent** original bytes and 791 overlapping differences; 92 fields / 27 calls; **10 exact siblings**; 844 is authored code, not the complete owned extent |
| Replay `BeginPlaybackStage` — `--owner replay` | 540 / 538 | **39** overlapping differences, 2 missing; 32 field targets; **15 exact replay siblings** |
| Player `CheckBulletCollision` — `--owner player` | 862 / 849 | 799 overlapping differences, 13 absent; 26 fields, 12 calls, 7 exact siblings |
| Controller `GetControllerInput` — `--owner controller` | 808 / 810 | **84** unlike overlap bytes; 2 excess bytes; 32 fields, 2 exact helper siblings |
| Etama `OnDraw` — `--owner etama` | 600 / 600 | **268** unlike bytes; separate exact Etama update/laser owners |
| Supervisor frame `InsertReceivedFrame` — `--owner queue` | 213 / 213 | **35** unlike bytes; 191/119/49-byte related queue leaves exact |
| Effect `AddedCallback` — `--owner effect` | 51 / 51 | **8** unlike bytes; 16 exact effect siblings |

For leaf-sized nonexact owners, refresh the filter in
[SMALL_FUNCTION_FRONTIER.md](SMALL_FUNCTION_FRONTIER.md) from the actual ledgers.
If a report disagrees with any row above, **prefer the fresh original-bound
report**, not the prose. Failed source-equivalent experiments are bounded
counterexamples, never proof that a new hypothesis cannot work.

## Structural progress since the **historical** 2026-11-03 handoff

- Native `AsciiGameManagerView` is **0x390 bytes**, sides `[2]` are **0x38**
  each. Replay reads **actual** `g_GameManager.sides[].runtime` pointers, not
  fabricated `g_ReplaySideState` globals. A distinct `GameConfiguration *`
  is original-backed at manager +0xE8. Real coordinate bounds include
  +0x350..+0x36C, original ECL mask +0x120 and opcode148 counter +0x380.
- Native `Supervisor` is **0x7B0 bytes**: DirectInput slots, network frame
  queues and version data length/pointer +0x7A8/+0x7AC now share one allocation.
  Network and version member source roles preserve their original decorated
  methods without pretending to prove the retail nominal class hierarchy.
- Native `AnmVm` is **0x2A4 bytes**. Player main/option/shot VMs, Enemy draw
  VMs, and now CardAttack main/secondary VMs use this one physical layout.
  The original 23-byte option and 44-byte shot C++ constructors are **locally
  byte exact but origin-unclassified**. The 44-byte body needs its original
  `PlayerRuntime.cpp` translation-unit context to emit the `__ehvec_ctor` call;
  an isolated compiler probe generated a nonexact 51-byte loop.
- Native `EnemyView` is **0x5430 bytes**, `EnemyManagerView` **0x2AC450**.
  Enemy timeline's 974 logical / **1,048 physical** bytes are exact with 42
  fields, true Enemy pool/event slots and original switch tables. `Effect`
  type3/4 trail callback is **464 bytes exact**, writing native Enemy
  acceleration +0x2DF8 and category bits 6..8 at +0x3380.
- `ReplayInputState` has one **0x8E-byte** native stride but **two separate
  16-counter windows**: network +0x0A, ordinary Replay +0x38. Do not merge
  these fields. Replay's 1,323-byte recording owner is exact with 82 fields.
- Exact CardAttack `StartAttack` **923 bytes/29 fields**, `OnUpdate`
  **536 bytes/22 fields**, `OnDraw` **334 bytes/17 fields**, `Create`
  **264 bytes/23 fields** use native ANM VMs and real GameManager +0xB0/+0x134.
  The six character-specific begin callbacks and 364/38-byte Enemy spawn
  callees remain exact. **Open caller ABI discrepancy:** the exact 1,048-byte
  timeline pushes one spawn argument as a 32-bit int, but the separately exact
  `SpawnEnemyRecord` source declares `char`; changing the timeline caller to
  `char` grew its code by eight bytes. Keep the original exact caller source
  until independently resolved; don't force a nominal signature match.

These are **source-layout and ABI recoveries, not newly promoted functions**;
current exact tally remains 1,005. Never use an old private padded class
from the long Knowledge Base to overwrite the current shared owners.

## Fast, correctly scoped validation

```bash
# Explore one large owner without rebuilding exact siblings yet.
python3 -B scripts/check-boss-frontiers.py --owner runecl --skip-exact-siblings
# Before committing a source change, verify its touched owner *and* exact cohort.
python3 -B scripts/check-boss-frontiers.py --owner runecl
python3 -B scripts/check-boss-frontiers.py --owner enemy
python3 -B scripts/check-boss-frontiers.py --owner supervisor
python3 -B scripts/check-boss-frontiers.py --owner replay

# A selected leaf: recorded flags/profile and exact PE/COFF, no other cold replay.
python3 -B scripts/build-match-unit.py --unit card-attack-start
python3 -B scripts/compare-coff-function.py --unit card-attack-start --json

# Ledger/document-only maintenance:
python3 -B scripts/progress.py --check
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
```

**Use only the subset touched by the actual diff; the commands above are
examples, not a batch to run every time.** Do **not** run `--owner all`, a
full cold replay, whole-product build or all tests for routine iteration.
Where VC7.1 re-numbers private `$L` labels, compare each **actual defined
COFF label's section-relative VA and the original PE operand** before changing
**only** the manifest symbol spelling. Never move a target VA or suppress an
ordinary byte difference to make a comparator pass. Source/diagnostic success
is distinct from accepted Factory replay.

## Storage state and protected evidence

The ignored `.analysis/` holds research source/probes, original-target
comparisons, direct Ghidra projects and historical archives. Its large
uncompressed JSON/log caches are **not automatically disposable**; inspect
referenced paths first. Keep the SHA-pinned `resources/th09.exe`, tracked
manifests/source, local tool symlinks `.tools/`, original evidence and the two
`.analysis/ghidra-direct/` projects. The existing
`.analysis/handoff-archive-20261103/legacy-unreferenced-experiments.tar.zst`
is a verified local-only evidence archive; do not delete it or mistake it for
a tracked artifact. `build/` and disposable compiler/debug output are ignored
and **reproducible**, so they may be absent after reviewed cleanup. Rebuild
needed objects before inspecting them. A historical note saying "build/
absent" is not a property of every newer checkout.

### Actual 2026-11-04 local cleanup (not merely a proposal)

The worktree's historical generated JSON was compacted **losslessly**: 1,258
large old files / **620,291,028** original bytes were packed into
`.analysis/handoff-archive-20261104/historical-generated-json.tar.zst`
(**22,176,904 bytes**; net **598,114,124 bytes** saved). **Every** archived
member was verified against its original SHA-256 and again checked against its
original on-disk bytes before removal. Its ignored manifest and receipt are
retained alongside the archive. The local original `resources/th09.exe`,
**both Ghidra databases**, original older archive and source/negative-test
scripts were not selected. Recent/small JSON observations remain unpacked.

The guarded cache cleaner removed **252 build files / 15,319,883 bytes**;
`build/` may now be **absent**. Recompile only the exact source/profile you
need. Local Git objects were safely repacked with `git gc --prune=never`:
all branch/tag refs stayed identical; nothing was deliberately pruned.
Total allocated repository size went from about **1.5 GiB** to approximately
**533 MiB** at the cleanup checkpoint. These are local-only storage actions,
**not** a new PE/COFF match, push or Factory acceptance.

Recover one archived historical JSON, safely and on demand:

```bash
# Preview one original path from the archived SHA-256 manifest:
python3 -B scripts/restore-analysis-archive.py \
  --path .analysis/EclManager.c774-base.obj.full.json
# Restore only that path, after SHA-256 verification and without overwrites:
python3 -B scripts/restore-analysis-archive.py \
  --path .analysis/EclManager.c774-base.obj.full.json --apply
```

The restore helper checks the archive/manifest receipt and the selected
member SHA, refuses overwrites/unsafe paths and does not automatically unpack
all 591.56 MiB. On a *different clone*, ignored local-only archives may be
absent; regenerate evidence from maintained source/target or arrange an
approved archive transfer. See [artifact cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md)
for the measured audit, full SHA-256 strings and original-path manifest.

### Post-cleanup validation (2026-11-04)

The full **target-independent** public CI was rerun after correcting historical
Etama GameManager symbol names and a handful of old source-spelling assertions.
It passed **1,318 unittest cases (29 skipped), no failures**, including the
script compilation, ledger, documentation, progress and whitespace stages.
This is **not** a full VC7.1 cold replay or native Windows game build.
The post-CI cache cleaner removed its newly generated **503 files /
7,642,130 bytes** from `build/`, and the final clean-cache preview again
reports zero. Protected original, Ghidra databases and the per-file verified
JSON archive were untouched. The repository occupies approximately **534 MiB
of allocated disk blocks** (current checkout) versus about **1.5 GiB** before
this cleanup; exact logical bytes and SHA-256 totals are retained in the
[2026-11-04 artifact cleanup audit](RE_ARTIFACT_CLEANUP_HISTORY.md).

The archived JSON package and its SHA-256 receipt live under the **ignored**
`.analysis/handoff-archive-20261104/` directory; they are local only and
**do not accompany a normal Git clone**. The tracked restore tool is still
useful in the same worktree, and the manifest lists the original paths. If
those ignored archives are missing in another checkout, rebuild historical
evidence or ask the operator for an explicit archive transfer. The next agent
should start with `git status` and only rebuild a touched original-bound unit,
not all compiled objects merely because `build/` is absent.

**Final public CI:** `python3 -B scripts/ci.py` passed **1,318 tests (29
skipped)** plus the validation and generated-progress checks after correcting
three obsolete Etama diagnostic referents and five stale native-source test
assertions. These tests do **not** assert a whole-game runtime or exact-source
acceptance receipt. The public CI's 505 temporary build/pycache files were
cleaned after the successful run; the final cache-cleaner preview was zero.

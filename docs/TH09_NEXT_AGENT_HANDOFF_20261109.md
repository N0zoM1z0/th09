# TH09 — next-agent handoff (2026-11-09)

**Start from the real worktree, not historical experiment snapshots.** Read
[AGENTS.md](../AGENTS.md), [live frontier table](RE_HANDOFF.md),
[original-bound validation rules](ORACLES.md) and [workflow](RE_WORKFLOW.md).
The November 8 and earlier `TH09_NEXT_AGENT_HANDOFF_*.md` guides document
their own dates; they are not current source or storage inventories.
The latest source-affecting checkpoint in this handoff is
`e7964962 gpt-web: reconstruct full native EnemyView constructor and trail extent`.
Git `HEAD` may advance with maintenance commits; always run `git status`
and `git log` again.

## Verified original and ledger (unchanged by constructor experiments)

- Target: original Japanese TH09 v1.50a, ignored `resources/th09.exe`;
  SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
- Source compiler: pinned VC7.1 under ignored `.tools/wine/`. No alternate
  profile/source-image substitution, target patch, or wholesale replay.
- Canonical status: **2,248 reviewed candidates; 1,037 authored/source-present;
  1,024 canonical exact; 13 source-present nonexact (29,834 original bytes)**.
  Authored exact bytes: **262,097 / 291,931 = 89.7805%**. Confirm from
  `scripts/report-reconstruction-status.py` and the canonical ledgers.
- There remain **35 origin-unresolved cases**. Even byte-perfect native
  constructor special members remain unclassified until authorship is proven.
  Exact bytes are not a Windows product link or runtime-semantic proof.

## Newly recovered native C++ layout and constructor chain

The 2026-11-09 work reconstructs six linked construction interfaces using
ordinary C++ types and constructors under the existing `/O2 /Ob0` unit
profile. `scripts/inspect-enemy-ecl-constructors.py` binds every original
relocation and checks original SHA-256 values:

| Source-facing constructor | Original VA | Entire body | Fields | Status |
| --- | --- | ---: | ---: | --- |
| `EnemyEclTimelineStateView` | 0x0040FCA0 | 28/28 | 2 | Exact bytes; origin unknown |
| `EnemyEclManagerView` | 0x00412090 | 15/15 | 1 | Exact bytes; origin unknown |
| `Th09EclInterpolationSlotView` | 0x004120C0 | 15/15 | 1 | Exact bytes; origin unknown |
| `Th09EclContextView` | 0x004120D0 | 47/47 | 4 | Exact bytes; origin unknown |
| `EnemyBulletDescriptorView` alias | 0x0040D500 | 28/28 | 0 | Existing canonical ctor; no new unique owner |
| `EnemyView` | 0x00412100 | 207/207 | 18 | Exact bytes; origin unknown |

The physical structure facts behind these owners are now in
`src/EnemyManager.hpp` and `src/EclOpcodes.hpp`:

- Timeline +0x100: 8 constructed `Float3` objects, 0x0C bytes each;
  entire timeline subobject remains 0x180 bytes.
- ECL interpolation slot: 0x30 bytes with `ZunTimer` at +0x04.
  Context: 0x22C bytes with timers at +0x08/+0x94 and eight slots at +0xA0.
- Enemy's two 0x214-byte bullet/laser descriptors at +0x2E74/+0x30C4
  receive exact native constructor calls; another real timer is at +0x3370.
- Enemy history: 96 x 0x1C samples at +0x33E8. **194 x 0x1C vertices**
  are *constructed* at +0x3E68 and end at +0x53A0. The known renderer
  uses only **192** of them (two per history sample); the last two
  elements' gameplay role is **unknown**. Old documents describing
  192 constructed vertices plus an opaque +0x5368–+0x53A0 gap are wrong.

Existing exact siblings remain intact: `EnemyManagerView` constructor
101/101, `EnemyManagerView::Create` 328/328, and
`EnemyManager::DrawImpl` 1,758/1,758. No new authored match was inserted
into `config/matches.csv` merely because a previously unclassified
compiler-generated-looking constructor produced exact bytes.

## Current high-value large boss frontiers (all nonexact)

Consult [the full 13-owner table](RE_HANDOFF.md#reconstruction-frontiers).

| Owner | Current source-cold physical comparison | Main next step |
| --- | --- | --- |
| `EnemyManager::OnUpdate` (0x00410730) | 3,900 bytes, 97 fields, 69 calls; **13** unlike | Decode grouped draw-head/descriptor register allocation without shifting the whole owner; `--owner enemy` |
| `EclManager::RunEcl` (0x004086C0) | 15,564 physical bytes (code + switch tables), 598 fields, **30** unlike, 0 field/table drift | Opcode 4 JUMP (7 bytes), opcode 155 timeout-spell bit (23); `--owner runecl` |
| `PlayerLifecycleView::UpdateChargeAttack` (0x0041F810) | **1,210/1,210**, 70 fields, 58 calls, 350 instructions, 53 blocks, direct CFG equal, **235** unlike | Mode stack byte / input register schedule; `--owner charge` |
| `ReplayManagerView::SaveReplay` (0x00420C60) | 1,238/1,238, 53 fields, **2** unlike at +0xB7/+0xBB | Destination `memcpy` pointer ECX/EDI scheduling; `--owner replaysave` |
| `ReplayManagerView::BeginPlaybackStage` (0x00420840) | 538/540, 32 fields, **39** overlap unlike + 2 missing | Byte snapshot, side-0 receiver/seed codegen; `--owner replay` |
| `ScreenEffect::CalcShakeEnvelope` (0x004227A0) | 336/336, 20 fields, **26** unlike | Receiver and fade arithmetic register allocation; `--owner screen` |
| `EffectManager::AddedCallback` (0x0040D1C0) | 51/51, 4 fields, **8** unlike | Side effect/manager register lifetime; `--owner effect` |

**Important corrected history:**
- ChargeAttack's **old 1,197/1,210** and **1,195/1,210** frontiers are
  superseded by the fully sized 1,210-byte candidate. The remaining 235
  differences are not exactness credit.
- `TitleScreenView::DrawResult` (0x00423D16) is now proven **1,939/1,939
  strict exact** with 82 bound fields and 577 instructions. An old
  three-byte diagnostic allowance was stale and has been removed. Do not
  confuse this exact *draw* with still-nonexact `OnUpdateResult`.
- The timeout-spell argument at ECL opcode 155 is a **single byte** at
  instruction +0x0C. The old four-byte union snapshot source hypothesis
  was replaced. The remaining discrepancy is emitted register scheduling.
- The GameManager side +0x0C/+0x10 Player state previously described as
  a handle-release interface is actually the exact `AddScore` API.
  See [corrected Player reset](TH09_PLAYER_NATIVE_OWNER_RESET_IS_SCORE_20261108.md).

## Minimal restart / bounded verification

```bash
git status --short --branch
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

# Recreate only the needed object after the handoff cache clean:
nice -n 12 python3 -B scripts/build-match-unit.py --unit enemy-manager-constructor
python3 -B scripts/inspect-enemy-ecl-constructors.py build/matching/EnemyManagerLifecycleCtor.obj

# One complete noncrediting boss oracle:
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
```

Use the selected owner's own pinned flags from `config/match-units.toml`.
Compare the **whole original physical owner**, relocations and any owned
switch tables. Preserve inherited dirty work; commit new substantive changes
as `gpt-web: <brief English summary>`. **No full cold replay** for these
bounded tasks.

## Storage state and how to restore older experiments

2026-11-09 cleanup losslessly archived 140 old source-shape/failed-probe
directories (24,862 files; **383,162,789 original logical bytes**) into:

`.analysis/handoff-archive-20261109/source-probes.tar.zst`
(**24,643,109 compressed bytes**).

Every archived file was individually SHA-256 verified against the
`source-probe-manifest.json`; independent receipt, verification and deletion
results are beside the archive. To list or selectively restore, from the
repository root:

```bash
tar -I zstd -tf .analysis/handoff-archive-20261109/source-probes.tar.zst | head
tar -I zstd -xf .analysis/handoff-archive-20261109/source-probes.tar.zst \
  .analysis/gpt-web-th09-boss-leaf-20261108/onupdate-scope-register-v25b
```

The baseline root, `runecl-probes/`, `enemy-probes/` and Ghidra
playback probe folder remain unpacked, as do saved Ghidra projects,
historical archives, the original game and `.tools/wine/`. The
guarded cache cleaner separately removed 158 regenerated `build/` files
(12,328,681 bytes); the next focused compile recreates those as needed.
Manifest/receipt files and analysis archives are **ignored local state**,
not available in a plain Git clone. See
[artifact cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) for precise
scope and current reclaim totals.

Do not delete `.analysis/` wholesale or confuse old experiment object
files with immutable target evidence. Never delete `resources/th09.exe`,
`.tools/wine/`, `.analysis/ghidra-direct/` or
`.analysis/factory-native-ida/`. No target file was patched; its SHA
remains exactly the one above.

# TH09 reconstruction handoff

This is the live restart document. Historical packet-by-packet notes through
Packet 208 remain available in Git at
`e0da09e:docs/RE_HANDOFF.md`; they are not duplicated here.

## Current state

Only the original Japanese TH09 v1.50a executable is supported:
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.

| Measure | Current value |
| --- | ---: |
| Function candidates | 2,191 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 34 |
| Confirmed authored | 990 |
| Classified exclusions | 1,167 |
| Source-present authored mappings | 645 |
| Canonical exact functions | 518 |

Boundary/origin inventory is reviewed, but exact reconstruction is not
complete. The faithful Windows i386 product graph remains open. Runtime
semantic reconstruction and portability have not started.

## Restart checklist

From the repository root:

```bash
git status --short --branch
git diff --check
python3 scripts/verify-target.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
```

Attest the active IDA database independently and require the same SHA-256,
image base `0x00400000`, entry `0x0047D45F`, and image size `0x000E7000`.
Use the direct registered IDA provider from local Codex. Factory attestation is
only required when working through a Factory/Web client.

Before editing, inspect all tracked and untracked work. The ignored target,
IDA database, toolchain, compiler outputs, and analysis artifacts must never be
committed.

## Boundary and origin closure

The full 2,191-candidate ledger has been reviewed against the pinned target.
Current dispositions are 990 authored, 1,167 excluded, and 34 intentionally
unknown. The 34-entry unknown set is frozen by SHA-256
`17bb6cebc7577183c2b339fca631bb1b41c754d3239916995324d9670eb67ef0`.

- Thirty-one entries are construction/destruction-only special members. Their
  target bodies are compatible with both explicit out-of-line source and
  implicit VC7.1 generation.
- `0x0042F3F0`, `0x0043D2B0`, and `0x00435EC0` are shared, folded, or otherwise
  owner-ambiguous physical bodies.
- Four former IDA `nullsub` candidates in the transition band are proven data.
- D3DX8, CRT, import, compiler-helper, static-initializer, and DXErr8 residuals
  have separate target-bound reviews.

Replay the closure checks with:

```bash
python3 scripts/review-transition-data.py
python3 scripts/review-runtime-residuals.py
python3 scripts/apply-game-origin-review.py --group compiler
python3 scripts/apply-game-origin-review.py --group authored
python3 scripts/apply-game-origin-review.py --group ambiguous
```

These commands are read-only without `--apply` and must report every selected
candidate as already applied.

## Exact reconstruction state

`config/implemented.csv` contains 645 source-present authored mappings. Of
these, 518 are canonical exact and 127 retain honest non-exact compiler results.
`config/matches.csv` contains 518 complete target-bound VC7.1 matches totaling
75,146 exact authored bytes. Source presence, origin, exactness, product
closure, and runtime behavior remain independent claims.

Canonical units are replayed with their recorded commands, for example:

```bash
python3 scripts/build-match-unit.py --unit <unit>
python3 scripts/compare-coff-function.py --unit <unit> --json
```

Do not promote near matches, adjacent-game similarities, IDA names, maintained
source, or successful compilation into `config/matches.csv`.

## Product gate

`config/build.toml` deliberately records an open product graph. Exact functions
do not yet establish all translation units, static data owners, libraries,
resources, initializers, linker order, or exercised Windows i386 runtime paths.
Do not open semantic reconstruction or portability until this gate closes.

## Next Web priorities

Phase state is `active-incomplete` exact reconstruction. Do not spend a new
session re-reviewing the frozen 34-entry origin-unknown set unless genuinely new
target evidence can distinguish explicit source, implicit special-member
generation, or folded ownership.

1. **The smallest exact-frontier packet is now exhausted.** `PopContext`,
   `FrontSide::ResetTransitionState`, and both Bullet transform installers are
   canonical exact. `CaptureFrameSyncState` and `AsciiManager::OnUpdate` have
   bounded natural-source negative receipts for their one-byte residuals. Do
   not reopen these without genuinely new ABI/type/TU evidence; rotate to the
   authored/no-source seams below.
2. **Recover authored source in bounded subsystem seams.** There are 345
   authored functions / 100,700 bytes without maintained source. The routed
   Bullet `0x00413AF0-0x00414095` packet is now closed: `DrawSingleBullet` and
   `EtamaController::AddedCallback` are canonical exact, while
   `EtamaController::OnDraw @ 0x00413BE0` remains independently maintained
   NON-EXACT at 568/600. The routed ANM surface-load seam
   `0x0043CAE0-0x0043CD35` is now closed with exact LoadSurface/PreloadSurface.
   In Ascii/Title `0x004234F6-0x00424579`, exact `DrawMusicRoom @ 0x004244A9`
   closes the screen-12 leaf and `DrawReplaySave @ 0x004239F6` is now maintained
   NON-EXACT at 783/800 after bounded natural compiler-shape probes. The remaining
   no-source owners are `0x004234F6` (ReplayMenu) and `0x00423D16`
   (Result/NameEntry). Do not reopen DrawReplaySave without new Float3/local/TU evidence.
   revisit the two larger Player collision owners `0x0041DC20/0x0041DFF0`
   after their shared layouts/helpers are recovered. These ranges remain routing
   hypotheses; establish every owner, ABI, boundary, and source mapping from
   TH09 evidence.
3. **Then attack coherent large non-exact families, not isolated giant owners.**
   The largest maintained frontiers are TitleScreen (21 functions / 19,899
   bytes), ECL (15 / 19,142), EnemyManager (5 / 7,709), Player (11 / 6,999),
   BulletManager (14 / 6,125), and AnmManager (10 / 5,920). Prefer helper leaves
   and shared ABI/layout discoveries before `RunEcl @ 0x004086C0` or another
   multi-kilobyte dispatcher.
4. **Prepare product closure only after the exact/source frontier becomes a
   documented plateau.** `config/build.toml` is still a skeleton: translation
   units, compile profiles, static-data owners, libraries, resources, linker
   switches/order, and Windows i386 runtime scenarios are empty or unknown.
   Populate these from evidence; a clean link alone does not close the gate.

Run `python3 scripts/report-reconstruction-status.py` after each checkpoint; it
reports the live non-exact and no-source frontier rather than relying on prose.
Semantic reconstruction and portability remain out of scope until the native
Windows i386 product has been compiled, linked, owner-audited, and exercised.

## Durable evidence map

- `config/functions.csv` and `config/function-origins.csv`: candidate boundary
  and origin ledger.
- `config/implemented.csv`: source-present authored mappings.
- `config/matches.csv` and `config/match-units.toml`: canonical exact replay
  units and commands.
- `docs/KNOWLEDGE_BASE.md`: durable TH09-only facts and negative results.
- `docs/PROGRESS.md`: generated current totals.
- `docs/RE_WORKFLOW.md` and `docs/ORACLES.md`: phase and acceptance rules.

The detailed historical handoff is intentionally not a second knowledge base.
Use Git history when investigating an old packet, then promote any still-useful
fact into the maintained ledger or knowledge base.

## Latest checkpoints

Repository preparation and source-ledger repair end at:

- `57ef025 gpt-5.6-sol: reconcile source presence ledger`
- `9f132e6 gpt-5.6-sol: make static init review durable`
- `88a8916 gpt-5.6-sol: enforce ledger consistency`
- `6f12e9f gpt-5.6-sol: compact live reconstruction docs`

The preceding boundary/origin closure is the eleven-commit series ending at:

- `e0da09e gpt-5.6-sol: close boundary and origin review`
- `e41119d gpt-5.6-sol: classify runtime origin residuals`
- `b988d52 gpt-5.6-sol: remove transition data pseudo-functions`
- `b5e8ac4 gpt-5.6-sol: classify remaining game origins`
- `7d1edf1 gpt-5.6-sol: add pinned runtime-origin audit`

No checkpoint in this series was pushed by the reconstruction agent.

## Finish checklist

Run the focused Oracle for the changed unit, then:

```bash
python3 scripts/validate-tracking.py --require-target
python3 scripts/progress.py
python3 scripts/ci.py
git diff --check
git status --short --branch
```

Keep only evidence required to reproduce unresolved claims. Delete superseded
dumps, duplicate decompilations, stale logs, generated builds, and Python cache
files after confirming they are not the sole record of an open result.

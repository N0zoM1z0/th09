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
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 979 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 971 |
| Canonical exact functions | 776 |

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
Current dispositions are 979 authored, 1,177 excluded, and 35 intentionally
unknown. The 35-entry unknown set is frozen by SHA-256
`126885e1a6a78ac42b0d81852253714cc1c9eb99141066d495b0029a16ca5695`.

- Thirty-one entries are construction/destruction-only special members. Their
  target bodies are compatible with both explicit out-of-line source and
  implicit VC7.1 generation.
- `0x0042F3F0`, `0x004343D0`, `0x00435EC0`, and `0x0043D2B0` are shared,
  folded, or otherwise owner-ambiguous physical bodies.
- Four former IDA `nullsub` candidates in the transition band are proven data.
- D3DX8, CRT, import, compiler-helper, static-initializer, and DXErr8 residuals
  have separate target-bound reviews. The original game-origin sweep cohort is now
  frozen explicitly in `config/game-origin-review-cohort.txt` so later exact/source
  evidence IDs cannot silently invalidate the read-only closure replay.
- Three former authored-sweep entries, `0x00401060/70/80`, are now explicitly
  reclassified as VC7.1 compiler-generated `@cosf@4/@sinf@4/@sqrtf@4` COMDAT
  helpers after two cold pinned-compiler reproductions of all 12 bytes each.
- Three more broad-sweep entries, `0x00405710` and `0x00436AA0/AB0`, are
  reclassified as `MathRuntime` library helpers `@fmodf@8`, `@acosf@4`, and
  `@atanf@4`. Pinned VC7.1 <math.h> emits the same COMDATs naturally; two
  cold target-bound comparisons reproduce every ordinary byte and the
  `__CIfmod` / `__CIacos` relocation destinations.
- Four D3DX8 SDK inline bodies, `0x00401290/0x004012C0/0x004012E0` and
  `0x0042E920`, are likewise removed from the authored denominator. The pinned
  PlatformSDK `d3dx8math.inl` defines `D3DXVec3Length`, `D3DXVec3LengthSq`,
  `D3DXVec3Dot`, and `D3DXVec3Cross`; natural target-backed `/Gr /O2` inline
  compilation emits the target COMDATs byte-for-byte in two cold rounds.
  Length resolves to `@sqrtf@4`, while Cross is 83/83 exact with no relocations.

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

`config/implemented.csv` contains 971 source-present authored mappings. Of
these, 776 are canonical exact and 195 retain honest non-exact compiler results.
`config/matches.csv` contains 776 complete target-bound VC7.1 matches totaling
121,112 exact authored bytes. Source presence, origin, exactness, product
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

Phase state is `active-incomplete` exact reconstruction. The live frontier is
**8 authored functions / 4,144 bytes without maintained source**, plus 195
source-present functions that retain honest non-exact compiler results.

1. **Preserve frozen and durable-negative frontiers.** The 35 origin-unknown
   entries remain frozen. Do not revisit them without new evidence that can
   distinguish explicit source, implicit special-member generation, or folded
   ownership. Likewise, do not churn known natural-codegen plateaus such as
   `CaptureFrameSyncState @ 0x00420190`, `AsciiManager::OnUpdate @ 0x00435B00`,
   `PlayerLifecycleView::CheckBulletCollision @ 0x0041DFF0`, or
   `Float3::FromAngleMagnitude @ 0x00441890`. The knowledge base is the
   authoritative list of their tested hypotheses and negative results.

2. **Treat source-present plateaus as closed until evidence changes.**
   `EnemyManagerView::SpawnEnemy @ 0x0040F340` is cold-stable at 361/373;
   collapsing its primary/opposing ECL paths over-optimizes to 322, while
   explicit duplicated failure tails inflate to 403. `ExAttackUpdateCallbackType20
   @ 0x0044ADE0` is exact-sized 405/405 with all 11 relocations solved and
   299/361 ordinary comparable bytes; bounded field-order/type probes were
   worse. `ExAttackUpdateCallbackType6 @ 0x00446060` is exact-sized 698/698,
   535/578 comparable; rows 11/12 are exact-sized 713/710 with 549/593 and
   546/590 comparable bytes. Their bounded natural source-shape probes are
   exhausted; these frontiers do not justify register, padding, volatile,
   assembly, or profile roulette.

3. **Continue authored/no-source work from target-proven owners, not address
   adjacency.** The separate EnemyManager spawn owner `0x0040F1D0` is now
   canonical exact: TH09 proves its sixth argument controls mirror bit 15 plus
   X reflection, while its seventh selects primary versus opposing ECL. ExAttack
   type-20 init `0x0044AC20` is now also canonical exact from its template-table
   owner and natural local-value lifetime. The `IP.txt` network loader
   `0x004324C0` is now canonical exact, while its same-TU parser `0x00432240`
   is source-present/non-exact at a documented natural code-layout plateau.
   `MixRenderColor @ 0x00401090` is now canonical exact under its target-proven
   file-local visibility; the same natural ABI correction also promotes
   `SetRenderStateForVm3D`, `DrawInner`, and `DrawInnerFlippedX` to exact.
   `RotatePlayerCollisionVector @ 0x0042AF40` is also now exact: the target's
   external `@sinf@4/@cosf@4` calls select the already-evidenced `/Ob0`
   lowering, while `/Ob1` remains the documented 42-byte intrinsic negative.
   `FileSystem::TryDecryptFromTable @ 0x0042C290` is now maintained source-present
   at an honest 220-byte register-allocation plateau; its TH09 crypt data remain
   extern/unowned and must not be copied from TH08.
   `ExAttackUpdateCallbackType6 @ 0x00446060` is now maintained source-present
   from the row-6 template update slot and is exact-sized at 698 bytes; the
   remaining register/store scheduling residual stays non-exact after a bounded
   authentic-family same-TU visibility probe.
   `FrontMessageRuntimeView::Update @ 0x00416590` and its same-TU
   `DecodeFrontMessageString @ 0x00415C60` are now maintained source-present.
   TH09 independently fixes the 29-way message opcode owner and complete external
   call surface; two cold natural builds keep Update at 2724/2852 and Decode at
   36/39. Their residuals are distributed global-address/register scheduling and
   private helper ABI, so they are honest non-exact frontiers rather than
   candidates for register forcing or padding.
   Rows 11 and 12 are now covered as well: `0x00446EE0` / `0x004471B0`
   are exact-sized 713/710-byte maintained updates from their template-table
   slots, with a shared 0x5C layout and a documented 44-byte scheduling residual
   after the target-visible chained `FromAngleMagnitude` expression closes size.
   Very small bodies such as `0x0044AB20` should not be claimed merely because
   they are short; first rule out folded/shared ownership with TH09-local xrefs.

   PlayerLifecycleView::UpdateMovementAndOptions @ 0x0041C170 is now
   maintained source-present from the exact OnUpdate owner and TH09-local
   input/SHT/effect/option/history evidence. Natural VC7.1 settles at 1864
   bytes versus the 1835-byte target after bounded type/lifetime/source-order
   probes; the exact caller remains 522/522 under the corrected semantic name.

   TitleScreenView::OnUpdateKeyConfig @ 0x00427EE8 is also now maintained.
   TH09 screen dispatch, config offsets and exact helper leaves close the full
   2,803-byte behavior; two cold builds are exact-sized at 2,803/2,803 with
   2,323/2,331 comparable bytes. The remaining eight bytes are two SoundPlayer
   scheduling triples plus two equivalent SIB encodings, so this is a durable
   natural-codegen plateau rather than a register/padding target. The same work
   corrects 0x00422F67 from an explicit-stdcall body model to the target-proven
   TitleScreenView member ABI while preserving 106/106 canonical exactness.

   FrontSide::OnDraw @ 0x004193E0 is now maintained as the 3,094-byte
   draw-chain owner proven by exact FrontSide::Create. The complete HUD prefix
   and 103-call surface are target-backed; a target-observed side-state lifetime
   closes the prefix through transition offset 0x582. Two cold natural builds
   remain 3,106/3,094 with a 12-byte transition-overlay constructor/branch
   scheduling residual after bounded profile/type/lifetime probes, so it stays
   source-present/non-exact without compiler steering.

4. **Keep using shared-helper leverage before large callbacks.** Exact ExAttack
   init/update rows, Player leaves, Front helpers, Bullet descriptor transforms,
   and EnemyManager boundaries have repeatedly unlocked larger code without
   attacking multi-kilobyte dispatchers directly. Preserve that pattern before
   revisiting `RunEcl @ 0x004086C0` or another large owner.

5. **Keep product closure separate.** `config/build.toml` remains an honest
   skeleton. Do not start semantic/port work until translation units, data
   owners, libraries, resources, link order, compiler profiles, and exercised
   Windows i386 runtime paths are evidenced and the native product gate closes.

Run `python3 scripts/report-reconstruction-status.py` after each checkpoint; it
is authoritative for live totals. Use `docs/KNOWLEDGE_BASE.md` for accepted
local facts and negative results, not old chat summaries.

## Workspace hygiene

`.analysis/` is disposable and is intentionally empty at this checkpoint.
The remaining Player collision codegen plateau, including its preserved-register
negative result and tested source shapes, is already durable in
`docs/KNOWLEDGE_BASE.md` and `config/functions.csv`; duplicate disassembly
receipts are not retained. Future agents should follow `docs/RE_WORKFLOW.md`:
after a checkpoint, delete rebuildable probe artifacts instead of using
`.analysis/` as a journal.

Private target executables, IDA databases, compiler/toolchain files, and game
assets that are not explicitly tracked resources must never be added to Git.

## Durable evidence map

- `config/functions.csv` and `config/function-origins.csv`: candidate boundary
  and origin ledger.
- `config/implemented.csv`: source-present authored mappings.
- `config/matches.csv` and `config/match-units.toml`: canonical exact replay
  units and commands.
- `docs/KNOWLEDGE_BASE.md`: durable TH09-only facts and negative results.
- `docs/PROGRESS.md`: generated current totals.
- `docs/RE_WORKFLOW.md` and `docs/ORACLES.md`: phase and acceptance rules.

The handoff is intentionally not a second knowledge base. Use Git history when
investigating an old packet, then promote any still-useful fact into the
maintained ledger or knowledge base.

## Recent checkpoints

Use `git log -12 --oneline --decorate` as the authoritative current history.
Recent substantive checkpoints include:

- `7c61d94 gpt-web: prune stale reconstruction notes`
- `da37007 gpt-web: clean Enemy spawn boundary`
- `5e96a77 gpt-web: recover ExAttack type20 update`
- `9293645 gpt-web: close ExAttack type16 type23 update`
- `b617bc4 gpt-web: close ExAttack type15 update`
- `4aa6702 gpt-web: close ExAttack type25 update`
- `675da3a gpt-web: close ExAttack type26 update`
- `c6b8eaa gpt-web: close ExAttack type13 update`
- `be28d0a gpt-web: close ExAttack type13 init`
- `59af14d gpt-web: close ExAttack type17 update`
- `2bb1722 gpt-web: close Front message setup`
- `3eb4786 gpt-web: close Front line strip`

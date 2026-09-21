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
| Source-present authored mappings | 897 |
| Canonical exact functions | 738 |

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

`config/implemented.csv` contains 897 source-present authored mappings. Of
these, 738 are canonical exact and 159 retain honest non-exact compiler results.
`config/matches.csv` contains 738 complete target-bound VC7.1 matches totaling
100,424 exact authored bytes. Source presence, origin, exactness, product
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
**82 authored functions / 59,897 bytes without maintained source**, plus 159
source-present functions that retain honest non-exact compiler results.

1. **Do not churn closed or frozen frontiers.** The 35 origin-unknown entries
   remain frozen. Keep the durable natural-codegen negatives closed unless new
   ABI/type/TU evidence appears, especially `CaptureFrameSyncState`,
   `AsciiManager::OnUpdate`, replay-input register-allocation leaves,
   Player/shot preserved-register residuals, and
   `ChainReleaseView::ReleaseSingleChain @ 0x0042CAE0`, `AppendD3DCapabilitySupportLine @ 0x0042CD40`, `FrontMessageOwnerView::InitializeMessageRuntime @ 0x004181E0`, `DecodeFrontMessageString @ 0x00415C60`, `EnemyView::CleanupAfterDeactivation @ 0x00410110`, and `EnemyView::IntegrateMotion @ 0x0040F9B0`, and `FrontMessageRuntimeView::ReleaseForSideCount @ 0x00417520`, `FrontMessageRuntimeView::LoadForSideCount @ 0x004182E0`, `TitleDrawCallback @ 0x00424898`, `ExAttackUpdateCallbackType01 @ 0x00441100`, `ExAttackInitializeCallbackType4 @ 0x00442A60`, `ExAttackInitializeCallbackType3 @ 0x00442580`, `Float3::FromAngleMagnitude @ 0x00441890`, `ExAttackInitializeCallbackType6 @ 0x00445EC0`, `PlayerPositionCallback30404Type4 @ 0x00443B10`, `PlayerPositionCallback30404Type8 @ 0x00447620`, `PlayerShotDrawCallbackType2 @ 0x00443300`. The Chain core is
   source-present at a cold-stable 268/241; `/GX-` collapses it to 218, so
   placement-new/local-order tricks are not justified.

2. **Continue bounded authored/no-source seams with target-proven owners.**
   Prefer short leaves whose receiver, table slot, or sole caller already fixes
   ownership. The recent Supervisor startup/network run is a good model:
   `SupervisorNetworkMessageThunk @ 0x00432EA0`,
   `SupervisorNetworkState::SupervisorNetworkState @ 0x0042CC10`, and
   `SupervisorInitializeD3D @ 0x0042CD10` all closed naturally, while the
   768-byte DirectPlay instance handler `0x00432B70` remains independent.
   Nearby candidates such as `0x0042CC90` / `0x0042CD40` should only be
   pursued after their owner/caller ABI is established; do not infer ownership
   from adjacency.

3. **Use shared-helper leverage before large callbacks.** Player, CardAttack,
   ExAttack, ECL, Chain, input/replay, and Supervisor work has repeatedly paid
   off by recovering a shared helper first and then reusing its exact ABI/layout.
   Keep that pattern. Avoid spending a session on a multi-kilobyte dispatcher
   when a smaller helper or table column can establish the same layout.

4. **Attack coherent large non-exact families only after their helper seams are
   mature.** The largest maintained families are still TitleScreen, ECL,
   EnemyManager, Player, BulletManager, and AnmManager. Prefer leaves and shared
   ABI/layout discoveries before `RunEcl @ 0x004086C0` or another giant owner.

5. **Keep product closure separate.** `config/build.toml` remains an honest
   skeleton. Do not start semantic/port work until translation units, data
   owners, libraries, resources, link order, compiler profiles, and exercised
   Windows i386 runtime paths are evidenced and the native product gate closes.

Run `python3 scripts/report-reconstruction-status.py` after each checkpoint;
it is authoritative for the live frontier. The knowledge base is authoritative
for accepted local facts and durable negative results.

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

Recent Web reconstruction checkpoints, newest first:

- `4555076 gpt-web: close ExAttack type9 init`
- `670c681 gpt-web: close ExAttack type10 init`
- `a0f3288 gpt-web: close ExAttack type22 init`
- `28be3b0 gpt-web: recover Player shot draw type2`
- `6770a3c gpt-web: recover Player position type8 callback`
- `c4a5a97 gpt-web: recover Player position type4 callback`
- `e14563b gpt-web: close ExAttack type5 update`
- `fd5a418 gpt-web: close Ascii input leaves`
- `a5bee85 gpt-web: recover ExAttack type6 init`
- `824eea4 gpt-web: close ExAttack type7 init`
- `9661228 gpt-web: close ExAttack type8 same TU`
- `b6316d0 gpt-web: close ExAttack type5 init`
- `b662ecd gpt-web: recover ExAttack type3 init`
- `7222f90 gpt-web: recover ExAttack type4 init`
- `b4ceb86 gpt-web: recover ExAttack type01 update`
- `4deb83d gpt-web: close ExAttack type0 type1 init`
- `e059a8d gpt-web: close ExAttack type2 init`
- `90534dd gpt-web: recover Title draw callback`
- `da69999 gpt-web: recover Front message loader`
- `f11b2f0 gpt-web: close Front death transition`
- `aaff33e gpt-web: freeze Front message release codegen`
- `190a497 gpt-web: close controller state sampler`
- `22ce901 gpt-web: close render state reset`
- `0b88980 gpt-web: close GameWindow startup check`
- `3379539 gpt-web: close GameWindow shortcut resolver`
- `b113121 gpt-web: close GameWindow render loop`
- `d016f33 gpt-web: close game window present`
- `e026ad0 gpt-web: close ANM texture strip pair`
- `733b934 gpt-web: close TitleScreen constructor`
- `21b8dd8 gpt-web: close TitleScreen registration`
- `ff574b8 gpt-web: close Front gameplay factory`
- `6af49e7 gpt-web: recover Enemy motion integration`
- `cec0779 gpt-web: close Background stage VM update`
- `86895e6 gpt-web: close executable checksum`
- `d93e699 gpt-web: recover game window creation`
- `c2657a1 gpt-web: close game window procedure`
- `5a0f44c gpt-web: close ANM capture service`
- `8bef301 gpt-web: close Front enemy indicator`
- `f98c0be gpt-web: close DirectPlay peer initialization`
- `613f56c gpt-web: close DirectPlay session host connect`
- `94e2fb8 gpt-web: close DirectPlay host address`
- `afc0ee6 gpt-web: close DirectPlay device address`
- `c7ed1d2 gpt-web: freeze Front message init ABI`
- `74545ae gpt-web: close Enemy ECL loader`
- `0a59eaf gpt-web: close Enemy attached effect update`
- `8f7e15c gpt-web: close Enemy movement clamp`
- `c068f07 gpt-web: close DirectPlay session reset`
- `2edff85 gpt-web: recover DirectPlay provider check`
- `ecc96de gpt-web: close DirectPlay error reporter`
- `77c9ebb gpt-web: close game timestamp helper`
- `c73ea66 gpt-web: close Player state3 effect15`
- `4f89531 gpt-web: close DirectPlay resource release`
- `63a80b9 gpt-web: close DirectPlay packet send`
- `1cad749 gpt-web: close Background pending label`
- `7deafa0 gpt-web: close scalar Hermite helper`
- `adf4959 gpt-web: close game window activation`
- `31fad54 gpt-web: close Player state3 effect pair`
- `8b1d6e4 gpt-web: recover Player type11 geometry seam`
- `f842e5e gpt-web: classify D3DX cross inline`
- `9aac6d1 gpt-web: close Player reward callbacks type8-15`
- `a3dc6ae gpt-web: close Player reward callbacks type5-7`
- `359357b gpt-web: close Player reward callbacks type2-4`
- `8854492 gpt-web: close Player reward callback type1`
- `b11f041 gpt-web: close ExAttack Hermite helper`
- `5abce23 gpt-web: close ExAttack type2 update`
- `557089d gpt-web: close EnemyManager immediate spawn`
- `e096446 gpt-web: close GameManager inverse coordinates`
- `24ef850 gpt-web: close RNG u16 range`
- `2eaaaaa gpt-web: classify D3DX inline helpers`
- `81de220 gpt-web: close Supervisor Direct3D bootstrap`
- `d966a1f gpt-web: close Supervisor network state constructor`
- `3b77490 gpt-web: recover Chain release seam`
- `98a1fc0 gpt-web: close Supervisor network message thunk`
- `1da3897 gpt-web: close ZunTimer tick interval`
- `1d82b38 gpt-web: close GameManager valueCC clamp`

Earlier boundary/origin closure and source-ledger preparation remain available in
Git history; the live handoff intentionally does not duplicate their packet
chronology. None of the six checkpoints above has been pushed by this agent.

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

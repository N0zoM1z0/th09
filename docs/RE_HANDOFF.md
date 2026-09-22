# TH09 reconstruction handoff

This is the live restart document. It intentionally contains current state and
current priorities only. Historical packet-by-packet investigation belongs in
Git history and docs/KNOWLEDGE_BASE.md.

## Current state

Supported target: original Japanese TH09 v1.50a.
SHA-256: 10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822.

| Measure | Current value |
| --- | ---: |
| Function candidates | 2,191 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 979 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 979 |
| Canonical exact functions | 779 |
| Source-present non-exact functions | 200 |
| Source-present non-exact bytes | 151,149 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 124,520 |

The source-presence frontier is closed. Exact reconstruction is not complete.
The faithful Windows i386 product graph remains open. Semantic reconstruction
and portability have not started.

## Restart checklist

From the repository root:

    git status --short --branch
    git diff --check
    python3 scripts/verify-target.py
    python3 scripts/validate-tracking.py --require-target
    python3 scripts/report-reconstruction-status.py

Attest the active IDA database independently before using semantic evidence.
Require the same target hash, image base 0x00400000, entry 0x0047D45F and image
size 0x000E7000. IDA names and types are provisional evidence, not exactness
credit.

Before editing, inspect all tracked and untracked work. Private target binaries,
IDA databases, toolchains, compiler outputs and analysis artifacts must not be
committed.

## Boundary and origin closure

The complete 2,191-candidate inventory has boundary/origin review. Current
dispositions are 979 authored, 1,177 excluded and 35 intentionally unresolved.
The unresolved set is frozen by SHA-256
126885e1a6a78ac42b0d81852253714cc1c9eb99141066d495b0029a16ca5695.

Do not revisit the 35 unknown entries without genuinely new evidence that can
separate explicit source, implicit compiler-generated special members or
folded/shared ownership. The read-only closure scripts remain:

    python3 scripts/review-transition-data.py
    python3 scripts/review-runtime-residuals.py
    python3 scripts/apply-game-origin-review.py --group compiler
    python3 scripts/apply-game-origin-review.py --group authored
    python3 scripts/apply-game-origin-review.py --group ambiguous

Without --apply they must report the selected dispositions as already applied.

## Exact reconstruction state

The authoritative live totals come from report-reconstruction-status.py and the
tracking ledgers. At this checkpoint there are 979 source-present authored
functions: 779 canonical exact and 200 honest non-exact.

Canonical exactness requires a target-bound match unit and relocation-aware
replay. Maintained source, exact size, adjacent-game similarity, IDA naming or
successful compilation do not by themselves justify a match.

The authored/no-source roadmap is obsolete and must not be restarted. All
confirmed authored functions now have maintained source.

## Active large frontier: EclManager::RunEcl

RunEcl at 0x004086C0 remains the main actively investigated large owner.
Target logical size is 14,792 bytes, followed by 772 bytes of compiler-owned
tables: 6 easing entries and 187 opcode entries.

A fresh pinned VC7.1 build at this checkpoint reports:

| RunEcl measure | Target | Current candidate |
| --- | ---: | ---: |
| Logical bytes | 14,792 | 14,244 |
| Logical gap | 0 | 548 |
| Stack frame | 0x168 | 0x13C |
| Immediate direct calls | 375 | 375 |
| Indirect calls | 4 | 4 |
| Relocations | target-owned | 598 |
| Integer resolver calls | 131 | 131 |
| Float resolver calls | 100 | 100 |
| Integer-lvalue resolver calls | 17 | 17 |
| Float-lvalue resolver calls | 24 | 24 |
| Compiler table entries | 193 | 193 |

Important current facts:

- Call structure is closed. Candidate and target both have 379 calls: 375
  immediate direct plus four indirect. Do not resurrect the older 376+3 count.
- Direct EnemyView::ResolveFloat is target-correct. The older wrapper-based
  14,696-byte near plateau was false because it created target-absent temporary
  homes and a 0x2C0 frame.
- Recent target-backed corrections include child-context scan lifetime, the
  fixed 0x20 laser-slot clear loop, coalesced interaction flags, repeated
  laser-slot relookup, child-ECL slot relookup, polar operand reevaluation,
  ordinary timer assignment, direct destination staging removal, late laser
  angle relookup and repeated side-owner lookup.
- Th09EclRunMovement::SetExtraAnmScript @ 0x00407B20 is now canonical
  exact at 261/261 bytes. Moving it from the artificial external helper TU
  into the RunEcl TU as a natural static function reproduces the target's
  private EDI/ESI entry transport and both opcode 57/61 callsites without
  register forcing. The corresponding RunEcl candidate is 14,244 bytes.
- Th09EclRunMovement::SetPrimaryAnmScripts @ 0x00406740 is also
  canonical exact at 80/80 bytes after moving it into the RunEcl TU as a
  natural static helper. VC7.1 eliminates the unused instruction transport
  and selects the target-private EAX Enemy receiver. This removes two
  target-absent caller moves, so the locally more-correct RunEcl candidate is
  14,240 bytes even though the aggregate gap grows to 552.
- Th09EclRunMovement::ConfigurePolarMotion @ 0x004070A0 is now
  canonical exact at 346/346 bytes. Its sole RunEcl caller establishes
  compiler-private EDI=Enemy / ESI=instruction transport. The target also
  reevaluates speed and duration for the x and y products and reads duration
  again for the timer write; natural cos/sin * speed * duration source gives
  VC7.1 the target evaluation order. The live RunEcl candidate is 14,244
  bytes, gap 548.
- Th09EclRunMovement::ConfigureRelativeMotion @ 0x00407320 has been
  corrected from stale source assumptions and moved into the RunEcl TU. TH09
  fixes target - worldPosition, origin from position, and zeroing velocity
  +0x2D8C. The natural same-TU candidate is 295/296 bytes with the target
  0x18 frame; the remaining byte is a final flag-register scheduling plateau.
  TH08/TH095 bitfield source shapes shrink TH09 to 289 and are rejected.
- The latest ANM ownership review proves 0x00439CF0 and 0x00439DC0 are
  AnmManager member helpers. RunEcl opcode 157 uses trail render vertices at
  Enemy +0x3E68, not the trail sample buffer at +0x33E8, and prepares
  g_AnmManager in ECX at the callsite.
- Aggregate function size is diagnostic only. Prefer a locally target-correct
  handler even when it makes the whole function temporarily shorter. Do not
  reintroduce disproven staging temporaries just to approach 14,792.
- No register forcing, var_order, volatile steering, padding, assembly or
  profile roulette is allowed.

Use docs/KNOWLEDGE_BASE.md Packets 466 through 484 only as chronological
investigation history. The current functions.csv row plus a fresh
report-ecl-codegen.py run are the live baseline.

## Other durable non-exact plateaus

Do not churn these without new evidence:

- ReplayManagerView::CaptureFrameSyncState 0x00420190: exact-sized 257 bytes,
  one commutative SIB byte remains.
- AsciiManager::OnUpdate 0x00435B00: exact-sized 253 bytes, one signedness
  branch byte remains; explicit signed source is worse.
- PlayerLifecycleView::CheckBulletCollision 0x0041DFF0: best natural 849
  versus target 862; remaining frontier is preserved-register allocation.
- Float3::FromAngleMagnitude 0x00441890: natural 30/32 FCOS/FSIN versus target
  FSINCOS; do not import the TH095 inline-assembly workaround.
- TitleScreenView::OnUpdateKeyConfig 0x00427EE8: exact-sized 2803/2803 with
  eight ordinary comparable bytes of scheduler/SIB residual.
- FrontSide::OnDraw 0x004193E0: 3106/3094; remaining 12 bytes are transition
  overlay constructor/branch scheduling after bounded probes.
- PlayerLifecycleView::UpdateMovementAndOptions 0x0041C170: 1864/1835 with
  complete external call surface and bounded lifetime/type probes.
- EnemyManagerView::SpawnEnemy 0x0040F340: cold-stable 361/373; merge and
  duplicated-failure-tail alternatives are both worse.

These are not forbidden forever; they require evidence that invalidates a
previous negative result.

## Non-exact family snapshot

Current source-present/non-exact bytes are concentrated in these modules. This
is a snapshot, not a permanent ranking; recompute before choosing work.

| Module | Functions | Non-exact bytes |
| --- | ---: | ---: |
| TitleScreen | 26 | 27,050 |
| EclManager | 14 | 18,995 |
| Player | 26 | 17,687 |
| ExAttackController | 21 | 14,283 |
| EnemyManager | 11 | 9,902 |
| Front | 10 | 8,138 |
| BulletManager | 14 | 7,084 |
| FrontSide | 4 | 6,254 |
| Background | 8 | 5,306 |
| GameManager | 5 | 4,259 |
| ReplayManager | 5 | 3,949 |
| AnmManager | 7 | 3,375 |

When RunEcl reaches an honest local plateau, prefer target-proven helper,
ownership, ABI and layout leverage in these families before broad profile or
register experiments.

## Product gate

config/build.toml deliberately remains an open native-product graph. Exact
functions do not establish all translation units, data owners, libraries,
resources, static initializers, linker order or exercised Windows i386 runtime
paths.

Do not begin semantic reconstruction or portability until exact/source work has
reached an honest plateau and the native product gate has the required
translation-unit and runtime evidence.

## Workspace hygiene

.analysis/ is disposable working state, not a journal. At this checkpoint its
contents have been cleared after promoting all durable facts to tracked source,
ledgers, knowledge-base packets or Git commits.

After each checkpoint:

1. Move durable facts into tracked source/config/docs.
2. Keep only compact unresolved evidence when it is the sole record.
3. Delete rebuildable OBJ/PDB/probe/source copies, duplicate disassembly dumps,
   stale inventories and superseded comparison logs.
4. Never use .analysis/ as long-term memory.

If recovering a dirty session, explain an artifact before deleting it. For a
clean checkpoint with no tracked references to its receipts, prefer deletion.

## Durable evidence map

- config/functions.csv and config/function-origins.csv: candidate and origin
  ledger.
- config/implemented.csv: source-presence mapping.
- config/matches.csv and config/match-units.toml: canonical exact units.
- docs/KNOWLEDGE_BASE.md: durable TH09 facts and negative results.
- docs/PROGRESS.md: generated totals.
- docs/RE_WORKFLOW.md and docs/ORACLES.md: phase and acceptance rules.
- scripts/report-reconstruction-status.py: authoritative live counts.
- scripts/report-ecl-codegen.py: RunEcl compiler-structure diagnostic.

The handoff is intentionally not a second knowledge base. Historical details
belong in Git and the knowledge base.

## Recent checkpoints

Use git log -12 --oneline --decorate as authoritative history. Current recent
substantive checkpoints are:

- dac1e8d gpt-web: correct ANM strip ownership
- 9539880 gpt-web: align RunEcl side owner state
- d09fe4a gpt-web: align RunEcl late laser angle
- 935e766 gpt-web: remove RunEcl staging temporaries
- 30a2476 gpt-web: align RunEcl laser angle relookup
- a7df1d9 gpt-web: align RunEcl timer and laser state
- f8a6621 gpt-web: align RunEcl polar conversion
- 2f858b2 gpt-web: restore RunEcl child ECL relookup
- 5e73983 gpt-web: restore RunEcl laser slot relookup
- 2611823 gpt-web: coalesce RunEcl interaction flags
- 7ea00b6 gpt-web: restore RunEcl laser slot loop
- 39f7c45 gpt-web: align RunEcl child context scan

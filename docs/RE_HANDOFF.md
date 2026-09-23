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
| Canonical exact functions | 802 |
| Source-present non-exact functions | 177 |
| Source-present non-exact bytes | 144,025 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 131,644 |

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
functions: 802 canonical exact and 177 honest non-exact.

Canonical exactness requires a target-bound match unit and relocation-aware
replay. Maintained source, exact size, adjacent-game similarity, IDA naming or
successful compilation do not by themselves justify a match.

The authored/no-source roadmap is obsolete and must not be restarted. All
confirmed authored functions now have maintained source.

The latest registration batch makes `Background::Create @ 0x00403AE0`
canonical exact at 307/307 bytes and 29 solved relocations. TH09 target has
no null guard after `AddToRegistry`; removing the source-only guard recovers
the direct chain-registration path. All four accepted `Background.cpp` units
replay in two cold rounds. `Background::Background` itself remains non-exact;
see Packet 519 for the separate evidence.

The preceding bounded transform batch makes `UpdateBulletBoundaryBounce @
0x004138C0` canonical exact at 295/295 bytes. A reconstruction-only sprite
lookup call was absent from the target; direct access to the Bullet VM's
loaded-sprite field reproduces the complete body and all thirteen relocations.
All fourteen accepted `BulletManager.cpp` units replay in two cold rounds;
Packet 518 records the proof. The at-most-256-byte frontier remains 44 units.

The preceding short-function batch makes `EffectManager::ResetPool @ 0x0040C990`
canonical exact at 194/194 bytes. Its former `int` return was unsupported by
the two target callers, and the natural void source reproduces all nine
relocations; Packet 517 in the knowledge base records the proof. All thirteen
accepted `EffectManager.cpp` units replay in two cold rounds. `Background`
constructor remains non-exact despite a 160-byte `/O1` candidate; its two
vector-helper address registers still differ from target. The short-function
frontier is now 44 source-present non-exact units at at most 256 bytes.

The preceding short-function batch makes `TitleCharacterConfigView::GetOptionState
@ 0x004234A7` canonical exact at 54/54 bytes. Packet 516 also corrects the
target-disproved fixed-input read in `MoveCharacterCursorHorizontal`: both
horizontal cursor helpers are now target-sized at 167 bytes but remain
non-exact. All 24 configured `TitleScreen.cpp` units, across two profiles,
replay exact in two cold rounds.

The preceding small-function batch makes the four remaining short Bullet transform
updates canonical exact: `UpdateBulletVectorAcceleration @ 0x004134D0`
(179 bytes), relative direction change at `0x00413630` (202), absolute
direction change at `0x00413700` (199), and aimed direction change at
`0x004137D0` (236). Packet 514 records the shared math-call correction and
target-backed interval snapshot/owner accesses. All 13 configured
`src/BulletManager.cpp` units replay exact in two cold rounds. Other Bullet
functions remain independent non-exact candidates.

The preceding small-function batch makes `ScreenEffect::CalcFadeHold @ 0x004224F0`
(186 bytes), `UpdateBulletDeceleration @ 0x00413460` (110), and
`UpdateBulletPolarAcceleration @ 0x00413590` (158) canonical exact. Packet 513
records a real FadeHold release-decay correction (`255/8`, not the stale
`128/8`), the target's folded deceleration coefficient, and target-backed
Bullet-relative delta accesses. All 15 ScreenEffect and nine BulletManager
configured units replay exact in two cold same-TU rounds. Other members of
either family remain independent non-exact candidates.

Packet 512's preceding short batch closed `ScreenEffect::CalcFadeIn`,
`CalcFadeOut`, and `CalcArcadePulse` (389 authored bytes) and corrected the old
fade/x87 residual diagnosis. Five earlier ScreenEffect-related non-exact
functions remain outside `config/matches.csv`.

`Ending::RunEndingScript @ 0x0040E8A0` is the preceding large exact unit: 1,499
logical bytes, 1,727 code-plus-compiler-table owned bytes, and 88 target-solved
relocations. The earlier 1,695-versus-1,499 size claim compared an entire COFF
section with only target logical code and is withdrawn. The independent
`Ending::Ending @ 0x0040EF60` starts after one unowned `CC` byte at
`0x0040EF5F`. Packet 511 records the source and boundary evidence. The original
translation-unit partition remains unknown; this is a function-level exactness
claim, not Windows i386 product closure.

## Short-function routing frontier

The bounded snapshot in `docs/SMALL_FUNCTION_FRONTIER.md` lists all 45 current
source-present non-exact authored functions whose target logical bodies are at
most 256 bytes; 13 are at most 128 bytes. This is a size filter, **not** a
verified call-graph leaf set or an ease-of-matching ranking. The live
`config/functions.csv` row and a fresh target/compiler check override that
snapshot after later checkpoints. The four short Bullet transform-update
siblings are now exact; the TitleScreen character-cursor, Supervisor frame
queue and ECL interpolation families remain bounded short-function groups.
Packet 515 removed one stale unused ECL helper that had made the configured
`ecl-interpolate-linear` Oracle fail at compile time; the linear unit still
replays exact, while `InterpolateHermite` remains 182/188 non-exact.

## Active large frontier: EclManager::RunEcl

RunEcl at 0x004086C0 remains the main actively investigated large owner.
Target logical size is 14,792 bytes, followed by 772 bytes of compiler-owned
tables: 6 easing entries and 187 opcode entries.

A fresh pinned VC7.1 build at this checkpoint reports:

| RunEcl measure | Target | Current candidate |
| --- | ---: | ---: |
| Logical bytes | 14,792 | 14,872 |
| Candidate-target delta | 0 | +80 |
| Stack frame | 0x168 | 0x154 |
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
  register forcing.
- Th09EclRunMovement::SetPrimaryAnmScripts @ 0x00406740 is also
  canonical exact at 80/80 bytes after moving it into the RunEcl TU as a
  natural static helper. VC7.1 eliminates the unused instruction transport
  and selects the target-private EAX Enemy receiver.
- Th09EclRunMovement::ConfigurePolarMotion @ 0x004070A0 is now
  canonical exact at 346/346 bytes. Its sole RunEcl caller establishes
  compiler-private EDI=Enemy / ESI=instruction transport. The target also
  reevaluates speed and duration for the x and y products and reads duration
  again for the timer write; natural cos/sin * speed * duration source gives
  VC7.1 the target evaluation order.
- Th09EclRunMovement::ConfigureRelativeMotion @ 0x00407320 has been
  corrected from stale source assumptions and moved into the RunEcl TU. TH09
  fixes target - worldPosition, origin from position, and zeroing velocity
  +0x2D8C. The natural same-TU candidate is 295/296 bytes with the target
  0x18 frame; the remaining byte is a final flag-register scheduling plateau.
  TH08/TH095 bitfield source shapes shrink TH09 to 289 and are rejected.
- StartTimedPolarDisplacement @ 0x00407200 and
  Th09EclRunMovement::BeginBoundaryAwareMove @ 0x00407C30 are now canonical
  exact at 288/288 and 510/510 bytes. StartTimed requires repeated
  speed/duration operand evaluation; BeginBoundary requires the target
  Float3 pointer-identity call @ 0x004343D0 before reusing one pointer for all
  four boundary tests. Their private same-TU transports arise naturally.
- Th09EclRunLate::MoveRandomBiased @ 0x00407E30 is now reconstructed to a
  natural 517/516-byte near match, up from 411 bytes. TH09 target evidence
  requires repeated manager/side/player X lookups, branch-specific angle
  normalization and the Float3 pointer-identity call before the Y-bound tests.
  The remaining one-byte size residual is localized to Y-pointer lowering:
  target uses lea ecx,[eax+4] then two [ecx] comparisons; the best natural
  source keeps the returned base pointer and emits two [ecx+4] comparisons.
  Natural alias variants expand to 518 via mov/add, so the helper remains
  honestly NON-EXACT rather than steering an encoding.
- EnemyPostEclUpdateMovement @ 0x00408180 is now canonical exact over its
  complete physical owner: 967 logical bytes, one alignment NOP and the
  six-entry easing table, 992/992 total. The target-proven flags snapshot and
  same-TU visibility of the ordinary non-inline Float3::FromAngleMagnitude
  definition reproduce the private ESI transport naturally.
- EnemyPostEclUpdateShotAndAnm @ 0x00408560 is canonical exact at 349/349.
  RunEcl naturally supplies Enemy in EDI; its nested shot call uses the
  TU-private EBX=Enemy / ESI=instruction transport. Starting direction=0
  before the mirror test closes the remaining scheduling bytes.
- Th09EclRunBullet::DispatchShotInstruction @ 0x00408040 is now canonical
  exact at 315/315. Same-TU private visibility supplies EBX=Enemy /
  ESI=instruction naturally; target-backed int lifetimes for bulletType and
  color hoist each short sign-extension before its parameter-mask branch,
  closing the former 321-byte plateau without register or encoding steering.
- Earlier TU/owner fixes moved RunEcl from the superseded 14,244-byte
  candidate to a size-near 14,820/14,792 (+28) checkpoint. That checkpoint is
  no longer the structural baseline: its opcode switch was physically about
  0x140 bytes too late even though the aggregate size looked attractive.
- Fresh SET_BOSS review removes a stale cached stateIndex shape. TH09
  reevaluates the ECL integer twice on the positive path, reloads bossSlot336B
  at each UI/slot use, and lays out state >= 0 as fall-through with teardown on
  the taken negative branch. VC7.1 then naturally keeps zero in ESI and reuses
  the Float3 constructor return for the hidden (-999,-999,0) marker. This cuts
  another 12 logical bytes without changing any accepted EclManager exact unit.
- Main-loop CFG is now reconstructed around an inner dispatch loop instead
  of two long gotos. TH09 requires a stable pointer to secondaryTime094,
  secondary/time mismatch to break into the frame tail, activeContext loading
  only after life>0, and the first interpolation-slot address to stay live
  across the per-frame callback. With those natural lifetimes the first opcode
  entry is exactly relative +0x1F2, versus about +0x332 in the old committed
  shape. The current whole owner is 14,872/14,792 (+80) with frame 0x154.
  This larger aggregate is accepted as a more truthful reconstruction because
  block placement and the hot CFG are materially closer to TH09.
- The next RunEcl work is local layout, not blind size trimming. SET_FLOAT is
  already semantically shaped correctly but target places its float temporary
  at [ebp-0xA4] while the current candidate uses a disp8-reachable slot; three
  such accesses account for nine bytes of downstream case drift. Direct
  activeContext rewrites for SET_SECONDARY_TIME were also tested and rejected
  because VC7.1 tail-merged the two Timer::operator= calls, dropping the
  target-correct direct-call multiplicity from 375 to 374.
- The latest ANM ownership review proves 0x00439CF0 and 0x00439DC0 are
  AnmManager member helpers. RunEcl opcode 157 uses trail render vertices at
  Enemy +0x3E68, not the trail sample buffer at +0x33E8, and prepares
  g_AnmManager in ECX at the callsite.
- Aggregate function size is diagnostic only. Prefer a locally target-correct
  handler even when it makes the whole function temporarily shorter. Do not
  reintroduce disproven staging temporaries just to approach 14,792.
- No register forcing, var_order, volatile steering, padding, assembly or
  profile roulette is allowed.

Use docs/KNOWLEDGE_BASE.md Packets 466 through 490 only as chronological
investigation history. The current functions.csv row plus a fresh
report-ecl-codegen.py run are the live baseline.

## Secondary large frontier: EnemyManagerView::OnUpdate

`EnemyManagerView::OnUpdate @ 0x00410730` is an active large-function frontier,
not a frozen size plateau. Fresh TH09 call-graph review invalidated several
stale maintained-source owners rather than merely changing compiler spelling:

- Target logical size is 3,883 bytes with frame `0x2A8` and 69 direct call
  sites. The old natural candidate was 3,156 bytes, frame `0x25C`, with only
  59 direct calls.
- The maintained source now uses the target-proven `Float3` by-value
  constructor/operators, the full 0x214 bullet spawn descriptor and side
  EtamaController path, four static playfield-test sites, three explicit death
  effect spawns, and `AnmProjectionAbs`.
- Target trail-history shifting is member-wise rather than one aggregate
  assignment. Spelling the position/velocity/angle copies separately makes
  VC7.1 naturally emit the target-shaped four-sample unrolled loop plus scalar
  remainder instead of `rep movsd`.
- Target-local owner review then corrected five stale free-call aliases:
  `Supervisor::SelectSide`, `EnemyEclManagerView::GetSubroutine`, all four
  `GameManagerPlayfieldView::IsWithinPlayfield` calls, both
  `EnemyAppendCollisionView::AppendPlayerCollisionBox` calls, and
  `SoundPlayer::PlaySoundPositionedByIdx`. The private ANM VM layout is also
  corrected to color1/color2/flags at `+0x1F0/+0x1F4/+0x1F8`.
- Those locally correct ABIs make the current pinned VC7.1 `/O2 /Ob1` build
  3,824/3,883 bytes with the target-exact frame `0x2A8` and 67/69
  direct calls after also restoring canonical RunEcl / EnemyView member
  receivers. The remaining logical gap is 59 bytes. Do not restore
  the disproven free aliases just to regain aggregate bytes. One target
  normal-path `Float3::operator+` call and one target-distinct
  `CleanupAfterDeactivation` site are currently compiler-merged.
- Target code repeatedly reloads `manager->sideState320` and its player/effect
  members at distant blocks. Keeping `side` and `player` as function-wide
  locals was the main remaining frame/lifetime error; removing them closes
  136 bytes and exactly closes the eight-byte frame gap.
- RunEcl now uses the current EnemyManager / offset-zero primary ECL receiver;
  Clamp/Integrate/Reset/Cleanup use their canonical EnemyView member owners.
  This locally correct owner cleanup shortens the aggregate by four bytes and
  is retained rather than reverting target-disproved aliases.
- `EnemyCoreUpdateAttachedEffects @ 0x0040F4C0`, from the same source file,
  still replays exact at 151/151 in two cold passes after these changes.
- The remaining 59-byte frontier now has bounded negative coverage: labels,
  branch aliases, explicit trail cursors, function-scope integer declarations,
  manager-prefix views and natural damage-pointer spellings do not recover the
  two merged target callsites. A named branch-local movement Float3 gives a
  tempting 3,864-byte candidate but has the wrong post-call copy shape and is
  rejected. Do not resume declaration/register roulette without new target
  evidence.

- Continue this owner by target-backed CFG/type/lifetime recovery. Do not add
  filler, force registers, use volatile steering/assembly, or chase alternate
  profiles for aggregate size.

The durable evidence is in Packets 491 through 498.

## Secondary large frontier: PlayerUpdateSelectorState

`PlayerUpdateSelectorState @ 0x004049A0` is an active Player large-owner frontier.
Fresh TH09 target/cold-compiler review now gives:

- target 3,238 bytes, frame `0x5C`, 33 physical direct callsites;
- current pinned VC7.1 `/O2 /Ob1` candidate 3,237/3,238 bytes, target-exact frame `0x5C`;
- target-backed timer fixes restore three folded current reads, three postfix
  increments, `HasTickedEvery`, and timer `operator==`;
- target-left and target-right pattern branches now keep their distinct
  `ResolvePatternOffset` callsites, closing the physical call count at 33/33;
- the entry construction still differs: target uses
  `??_H @ 0x00401470` over three 0x0C elements while the candidate emits an
  explicit constructor loop;
- same-TU `ResolvePatternOffset @ 0x00404710` still cold-replays exact at
  346/346 bytes and 412/412 owned compare extent.

The selector has target-exact frame `0x5C` and full 33/33 call surface.
Failure-tail Player caching/reload and target-backed 16-bit protocol storage at
`+0x32/+0x34` close the natural body to 3,237/3,238 bytes. This remains
NON-EXACT: target entry construction is `??_H` while the candidate has an
explicit ctor loop, and local branch layout still differs. EH-disabled lowering
does not change this. Do not chase the final aggregate byte by jump/register/
padding/profile tricks; resume only with new TH09-local source evidence.

The durable evidence is in Packets 499 through 505.

## Secondary large frontier: EtamaController::OnUpdate

`EtamaController::OnUpdate @ 0x004146F0` remains an active BulletManager
large-owner frontier. Fresh target-backed corrections give:

- target 2,195 bytes, frame `0x40`, 62 immediate direct calls;
- current pinned VC7.1 `/O2 /Ob1` candidate 2,188 bytes, frame `0x50`,
  with 62 external DISP32 relocations;
- three target-absent sprite helper calls are gone; VM loadedSprite storage is
  read directly;
- bullet cancel records now use canonical `AppendBoxRecord`;
- laser timing now uses the target GetCurrent / integer comparison operators,
  reducing `__ftol2` from eight calls to the target-observed two;
- the target-proven shared spawn completion tail, cached WAIT-flags flow and
  side-freeze queue path are now restored; the latter fixes a real source bug
  where frozen bullets incorrectly ticked two timers;
- the target-disproved 0.7 laser-size branch is removed, and FIRED-before-
  DESPAWNING switch ordering now puts the cold despawn block after laser
  AddNormalizeAngle/SetZRotation as in the target;
- laser state 0/2 now share a natural currentWidth local: state 0 publishes
  it to the Laser member, while state 2 keeps the shrinking value local as the
  target does;
- raw-section Capstone confirms 62 actual candidate calls, resolving the
  previous COFF objdump relocation-boundary ambiguity;
- all seven canonical units from the same TU remain exact in two cold passes.

The honest current frame is still 0x10 too large. Bounded natural lifetime
probes reproduce the target laser scalar homes but cannot make VC7.1 reuse the
four early Float3 return buffers naturally; declaration/type/named-temp variants
are exhausted. Do not force registers, use var_order, or chase aggregate size
alone.

The durable evidence is in Packets 506 through 509.

## Secondary large frontier: EtamaController::SpawnSingleBullet

`EtamaController::SpawnSingleBullet @ 0x00412960` has moved from a large
structural gap to a bounded five-byte frontier:

- target 1,893 bytes, frame `0x14`, 22 direct calls;
- current pinned VC7.1 `/O2 /Ob1` candidate 1,888 bytes, frame `0x14`;
- separate primary/secondary searches naturally reproduce the target 5-way /
  6-way allocator unrolling;
- sentinel resets use embedded pool bases, not cached cursor fields;
- the folded position conversion call and inline despawn SetSprite path are
  restored; loaded-sprite geometry is fixed to target field `+0x30`;
- call offsets are aligned through the second direct SetSprite and the entire
  transform-copy / AdvanceTransformProgram / return tail is structurally closed;
- the remaining five-byte displacement is isolated to VC7.1 transformFlags CSE:
  target memory-tests FAST then reloads the dword for NORMAL/SLOW, while the
  natural candidate keeps one dword value live. Clean TH08 corroborates the
  ordinary 32-bit / if-else-if source family, so do not fabricate a byte alias.

All seven canonical units from the same BulletManager TU remain exact in two
cold passes. Resume only with new TH09-local evidence that explains the flags
lifetime naturally.

The durable evidence is in Packet 510.

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
| BulletManager | 12 | 6,816 |
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

.analysis/ is disposable working state, not a journal. It is empty at this
checkpoint. The recovered probe variants and generated outputs were inspected
and moved to the system trash; no current claim depends on those unbound files.
Use live source, ledgers, this handoff and the knowledge base to resume work.

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
- docs/SMALL_FUNCTION_FRONTIER.md: bounded short-function routing snapshot;
  re-filter the live ledger before selecting a new unit.
- docs/PROGRESS.md: generated totals.
- docs/RE_WORKFLOW.md and docs/ORACLES.md: phase and acceptance rules.
- scripts/report-reconstruction-status.py: authoritative live counts.
- scripts/report-ecl-codegen.py: RunEcl compiler-structure diagnostic.

The handoff is intentionally not a second knowledge base. Historical details
belong in Git and the knowledge base. Use `git log -12 --oneline --decorate`
for recent checkpoints.

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
| Source-present authored mappings | 732 |
| Canonical exact functions | 596 |

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

`config/implemented.csv` contains 732 source-present authored mappings. Of
these, 596 are canonical exact and 136 retain honest non-exact compiler results.
`config/matches.csv` contains 596 complete target-bound VC7.1 matches totaling
81,662 exact authored bytes. Source presence, origin, exactness, product
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
2. **Recover authored source in bounded subsystem seams.** There are 258
   authored functions / 86,756 bytes without maintained source. The routed
   Bullet `0x00413AF0-0x00414095` packet is now closed: `DrawSingleBullet` and
   `EtamaController::AddedCallback` are canonical exact, while
   `EtamaController::OnDraw @ 0x00413BE0` remains independently maintained
   NON-EXACT at 568/600. The routed ANM surface-load seam
   `0x0043CAE0-0x0043CD35` is now closed with exact LoadSurface/PreloadSurface.
   The routed Ascii/Title `0x004234F6-0x00424579` no-source seam is now source-closed:
   `DrawMusicRoom @ 0x004244A9` is exact; `DrawReplaySave @ 0x004239F6` is
   maintained NON-EXACT at 783/800; `DrawReplayMenu @ 0x004234F6` is maintained
   NON-EXACT at 1244/1280; and shared `DrawResult @ 0x00423D16` is maintained
   NON-EXACT at 1941/1939 with the target 0x44 stack frame but different register/slot
   allocation. Do not reopen these without genuinely new local/TU/compiler evidence.
   The shared Player math leaf `Float3::operator/ @ 0x0040F5A0` is now canonical exact;
   The small Player helper packet is now closed: `AddRespawnResource @ 0x0041BC90`
   and `CalcItemCollectionCollision @ 0x0041BEE0` are canonical exact.
   `PlayerLifecycleView::UpdateBeforeState @ 0x0041DC20` is now canonical exact
   after TH09 call-site review corrected `PlayerOwnerStateView::ApplyReward` to
   position + four integer arguments (the previously inferred float was spurious).
   `PlayerLifecycleView::CheckBulletCollision @ 0x0041DFF0` is now source-present
   NON-EXACT with a repeatable 849/862 natural candidate and exact 0x34 stack frame;
   the remaining 13-byte frontier is preserved-register allocation, so do not reopen
   it without new local/TU evidence. Shared `PlayerOwnerStateView::ApplyReward @
   0x0041D150` is now source-present NON-EXACT: TH09 proves its position-plus-four-
   integers ABI and full reward/effect/timer semantics, while the maintained natural
   candidate is 1544/1519 with the target 0x18 stack frame; the remaining 25-byte
   frontier is popup-cadence tail merging/register allocation. Do not reopen without
   genuinely new source/TU evidence. The short `0x0041BD40-0x0041BD94` Player
   leaf packet is now canonical exact: additive/normal blend setters, ANM raw-data
   accessor, and collision-region deactivate total 51 bytes. Next prefer the connected
   `PreloadPlayerAnmResources @ 0x0041CDE0`, `PlayerTransitionSpecial @
   0x0041CB50`, and `PlayerUpdateCommon @ 0x0041CC80` are now canonical
   exact; the latter two preserve exact `Player::OnUpdate` while VC7.1 naturally
   uses the target-private ESI handoff. Continue with another bounded Player-owned
   dependency only after re-establishing its owner/caller evidence; do not jump from
   this closure into a multi-kilobyte dispatcher by adjacency alone. The TH09
   collision-region creation family `0x0041CF30-0x0041D140` is also now
   canonical exact: one 110-byte allocator plus seven 43-61 byte Type0/1/2/4
   geometry wrappers total 477 bytes. Neutral TypeN names remain until local caller
   evidence distinguishes every original Cancel/Damage spelling. Two additional
   Player leaves are canonical exact: same-TU `PlayerUpdateStageD @ 0x0041BA80`
   (121 bytes) and forward `AngleFromPlayerToPoint @ 0x0041BB00` (101 bytes).
   The latter remains separate from `0x0041BB70` because the two physical
   helpers use opposite delta signs despite identical size/dependencies. The enemy-
   damage seam is now source-closed: `PlayerBuildAabb @ 0x0041F2C0` is
   canonical exact at 55 bytes through a natural private four-register ABI, while
   `CalcDamageToEnemy @ 0x0041FCD0` is maintained NON-EXACT at 983/996.
   Two TH09 EnemyManager callers fix its five-argument ABI and full shot/region
   semantics; the remaining 13-byte frontier is target SI=2 plus savedRotation
   spilling versus natural VC7.1 ESI allocation. Do not reopen without new TU/source
   evidence; do not force registers or var-order. Shared `ZunTimer::HasTicked @
   0x0040F810` is now canonical exact at 17 bytes; TH09 fixes current!=previous
   and clean TH08 only corroborates the method family. Keep the established negative
   Rotate frontier `0x0042AF40` closed unless new external-call/TU evidence appears.
   ANM local `AnmProjectionAbs @ 0x0040F1C0` is now canonical exact at 12 bytes;
   exact Project3DQuad already bound the symbol/owner, and plain `fabs` source emits
   the target out-of-line x87 wrapper without asm or intrinsic forcing. Three Enemy
   helper leaves are also canonical exact: `EnemyEclManagerView::GetSubroutineCount`
   at `0x0040F160` (7 bytes), `GetSubroutine` at `0x0040F170`
   (16 bytes), and `EnemyView::ResetBulletRankInfluence @ 0x0040F180`
   (51 bytes). Their raw-ECL table and rank-field offsets are already maintained in
   EnemyManager/ECL source, so no adjacent-game layout was imported. Player-heavy
   caller evidence also closes `ZunTimer::operator-=(int) @ 0x0041F300`
   exactly at 9 bytes as a natural tail wrapper to exact `Decrement(int)`.
   Neutral Player `SubtractResourceClamped @ 0x0041F310` is also canonical
   exact at 51 bytes: three TH09 heavy-state callers subtract 300/200/100 from
   Player `+0x30388` and the natural lower clamp to 1.0f matches first try.
   `InitializeShot @ 0x0041F350` is now source-present NON-EXACT: TH09 fixes
   its fastcall Player/shot/descriptor ABI and complete 0x484-shot/0x38-descriptor
   layout; a repeatable `/Ob0` candidate is target-sized 309/309 with all
   seven relocations solved and 269/281 ordinary bytes. The only residual is Y/Z
   independent-store scheduling. Do not reopen with volatile/pragma/profile fishing.
   Its default frame-match wrapper `0x0041F490` is canonical exact at 37
   bytes; TH09 directly compares timer-current input to descriptor `+0x00`
   before calling InitializeShot, so adjacent TH08 modulo semantics are not imported.
   The connected descriptor dispatcher `SpawnShots @ 0x0041F4C0` is now
   canonical exact at the reviewed 190-byte extent: heavy-state supplies timer current
   in EDX plus power-level 0/1 on stack, and TH09 directly walks the selected 0x38-byte
   descriptor chain over 128 0x484-byte shot slots. Two additional cross-system
   leaves are now canonical exact without layout expansion: `EnemyManagerView::
   FindActiveEnemyBySideCategory @ 0x0040F7D0` (62 bytes) scans the already-
   maintained 128x0x5430 Enemy array, while `AsciiManager::SetBossMarkerState @
   0x0040F890` (20 bytes) writes the target-backed marker-state plane at +0x2254.
   Player code consumes the EnemyManager query but does not own it; EnemyManager
   calls the Ascii setter but does not own that state. Two tiny shared Player
   leaves are also now canonical exact: `GetTransitionBlockFlag @ 0x0040D4F0`
   returns shared-runtime +0x11EA8 used by both reward/death gates, and
   `ApplyRewardWithStateAdvance @ 0x0040F8B0` increments owner-state +0x38
   before a natural tail-jump to maintained `ApplyReward`. The latter target
   independently distinguishes state38 +0x38 from activeFrameCounter +0x3C.
   Player `SetUpdateState @ 0x00403C20` is also canonical exact at 12 bytes;
   non-Player callers still pass Player pointers, while the distinct shared/folded
   `GetUpdateState @ 0x00435EC0` remains frozen origin-unknown and must not be
   reclassified from this setter evidence. PauseMenu now has two more exact leaves:
   GameManager `HasFlagBit0 @ 0x004343B0` (10 bytes, already-authored source
   in the Pause TU) and SoundPlayer-facing `ResumeAfterPause @ 0x00423498`
   (15 bytes), whose exact body queues opcode 7/arg 0/path unpause through exact
   `SoundPlayer::QueueCommand`. Two explicit static-object constructors are
   also now canonical exact: `ZunMemory::ZunMemory @ 0x0042B0D0` (13 bytes)
   and `GameErrorContext::GameErrorContext @ 0x0042B110` (19 bytes), each
   bound by its TH09 static-initializer thunk and pre-existing target-backed fields.
   The third nearby static-init constructor is now closed too: `ReplayInputState::
   ReplayInputState @ 0x0042B010` is 182-byte exact for the 3x0x8E global input
   state array; target codegen proves the +0x58 mapping table uses signed short -1
   sentinels rather than unsigned 0xFFFF. The runtime reset seam adds two more
   canonical exact leaves: opaque `GameWindowView::GameWindowView @ 0x0042D290`
   (18-byte zeroing ctor for the TH09 0x44 global at 0x004B30B0) and
   `AnmManager::ReleaseSurfaces @ 0x0042D2F0` (43 bytes), which releases
   the established 32 primary surface slots at +0x123FC before D3D reset.
   Static-init review also closes three tiny score/archive special members exactly:
   `ScoreFileView::ScoreFileView @ 0x00421C40` (24 bytes), neutral 0x2C
   `ScoreRecordCtorView::ScoreRecordCtorView @ 0x00421C60` (18 bytes, used
   for the 400-entry table and current singleton), and `PbgArchive::~PbgArchive @
   0x004335D0` (5-byte natural tail jump to exact Release).
   Supervisor netplay now also has exact `SupervisorFrameQueueView::AreFrameQueuesSynchronized @ 0x0042ECF0`
   (49 bytes): TH09 proves two 0x78-stride queue heads at +0x480/+0x4F8, and
   only the natural short-circuit conjunction reproduces the target branch shape.
   Fresh target ECX review also corrects queue ownership: the frame queues live on
   the Supervisor callback receiver, while `g_SupervisorNetworkState` is separate
   DirectPlay/session state; correcting maintained `SupervisorNetwork.cpp` moves its
   natural candidate from 1,639 to 1,625 bytes versus the 1,633-byte target.
   Bullet/ECL descriptor setup also gains an exact neutral constructor at
   `0x0040D500` (28 bytes): TH09 fixes the 0x214 physical size and
   `transformSound +0x204 = -1` sentinel, while a neutral ctor view avoids
   overclaiming the current reconstructed Float3 subobject model.
   Connected mode-4 setup helper `ResetTitleMode4Supervisor @ 0x0042EC30`
   is now 177-byte canonical exact: it resets Supervisor timing/queue state,
   seeds both frame queues with -9999 sentinels, activates the separate network
   session object, and clears its connection state. Two fixed-size memset regions
   are the natural source shape that reproduces the target ESI rebasing.
   Two connected ECL/Bullet leaves are now canonical exact as well: `Bullet
   ResetTransformRuntime @ 0x0040C390` (41 bytes) clears the target-backed
   transform runtime plane, and neutral `EnemyEclOpcode1 @ 0x0040C3F0`
   (76 bytes) assigns bit0 in both side-state +0x34 flag dwords and conditionally
   plays sound 0x34. The opcode1 bitfield lowering is target-proven, not imported.
   The same ExAttack seam now also closes Type10/11/12 wrappers and the Type22
   three-shot burst at `0x0040C770/0x0040C7C0/0x0040C810/0x0040C860`,
   totaling 434 exact bytes. Types 10-12 source local position values from the
   active ECL context; Type22 builds three radius-64 temporary parameters at
   pi/2, 0.4*pi and 0.6*pi before exact Spawn calls.
   Etama's side-local EffectManager tail is now closed too: `DrawBulletLayerEffects
   @ 0x0040D110` is 169-byte canonical exact and walks drawSentinel1 at +0x1E8.
   The adjacent polar helper `0x0040D4D0` stays no-source: natural cosf/sinf
   emits 30 bytes versus target 32-byte FSINCOS lowering, so do not force it.
   AnmManager's pending-capture request publisher `0x0042F420` is now exact
   at 101 bytes: +0x08 is the -1/active capture index and +0x2B2914..+0x2B2930
   hold the eight source/destination rectangle integers. Its sole caller is the
   Supervisor loading/capture helper `0x004303C0` is now canonical exact
   too at 214 bytes: it shares the established +0x5D0/+0x740 loading layout, sets
   up scripts 0/1/2 and VM positions once, then queues the full-screen capture
   through the exact request publisher.
   The adjacent loading-state transitions are exact too: Title-side `HideLoadingVms
   @ 0x00430360` sends interrupt 1 and clears +0x740, while gameplay
   `FinishLoading @ 0x004304A0` sends interrupt 2 and sets +0x740=2; both
   fade-release the shared screen effect. Their source contracts are corrected to
   void because target callers ignore EAX and the bodies synthesize no return.
   WinMain's two 5x0x18 lock-array lifecycle leaves at `0x00430530/0x00430560`
   are also exact: the existing +0x6C0 CRITICAL_SECTION layout plus PE IAT identity
   proves InitializeCriticalSection/DeleteCriticalSection without new layout guesses.
   Shared `Float3::operator+= @ 0x00405730` is now 36-byte canonical exact;
   it closes the common three-component add used by Supervisor/Effect/Player/Enemy
   callers. High-reuse `Float3::FromAngleMagnitude @ 0x00441890` remains
   source-absent: natural VC7.1 is 30 bytes FCOS/FSIN versus target 32-byte FSINCOS,
   and adjacent source requires inline asm, which is not accepted here.
   EffectTemplate effectId 7's update callback `0x0040C960` is now 38-byte
   canonical exact: table ownership comes from the +0x10 callback slot at
   `0x004A0C00`, and the body copies side Player +0x1B88 into Effect +0x0C.
   GameManager reset `0x0041A7C3` is now 100-byte canonical exact under the
   already-pinned GameManagerSetup /Os profile. It proves a three-record 0x38 flag
   plane at +0x34/+0x6C/+0xA4 and resets the setup/runtime fields through +0x380.
   Supervisor transition helper `GameManager_CutChain @ 0x0041A827` is now
   98-byte canonical exact too: it settles both setup-state counters, restores
   Supervisor speed to 1.0, then cuts the GameManager calc/draw chains.
   Gameplay setup's adjacent date formatter `0x0041A796` is now 45-byte exact;
   it writes `%y/%m/%d` into shared `g_ReplayPlayTimeText @ 0x004AC879` via
   pinned CRT time/localtime/strftime and replaces the old provisional reset alias.
   Shared GameManager playfield test `0x0041A6EB` is owner/ABI/semantics-closed
   but remains no-source: natural /Os shapes are 110/114 bytes versus target 116,
   with the residual in x87 compare-status/pop control flow; do not force it.
   FrontSide auxiliary transition leaf `0x00415D50` is now 24-byte exact.
   GameplaySetup's exact FrontSide::Create return is stored in side +0x18, and both
   callers load those same pointers before writing FrontSide +0xABCC / timer +0xABD0.
   FrontInf's embedded message-runtime reset `0x00415C90` is now 23-byte exact.
   Front added-callback embeds it at +0xE944 and then loads `plNN.msg` /
   `plNN_match.msg`; the reset clears 0x1D70 bytes and seeds +0x08 to -1.
   ExAttack draw is now closed as a coherent pair too: `OnDraw0 @ 0x00415460`
   is 286-byte exact and `OnDraw1 @ 0x00415580` is 193-byte exact. TH09
   fixes the record VM array/count/list/callback fields and controller draw-list
   heads; exact closure uses ordinary VM byte-offset traversal and natural local
   declaration order, with no register or pragma steering.
   The connected calc callback `ExAttackController::OnUpdate @ 0x00415340`
   is now canonical exact at 279 bytes as well, closing the update/draw callback
   trio installed by exact Register. It rebuilds the three draw lists and side
   counts from the 256-record pool each ungated frame, releases completed records,
   executes per-record VMs, and advances record timers through exact helpers.
   Supervisor frame-queue insertion is now source-closed but deliberately NON-EXACT:
   `InsertReceivedFrame @ 0x0042E9E0` has the corrected Supervisor receiver,
   two 10x0x0C ordered queues at +0x47C and last-received records at +0x56C;
   the best natural candidate is a stable 211/213. The remaining two-byte
   frontier is register/temporary-slot allocation, so do not force it. Next prefer
   the adjacent queue leaves are now source-closed too: `InsertPredictedFrame @
   0x0042EAC0` is a stable 176/191 natural candidate, while `PopFrame @
   0x0042EB80` is target-sized 119/119 after target-backed u16 return-type
   refinement but retains ten ordinary register-allocation byte differences. All
   three queue methods now share one coherent Supervisor layout; do not force their
   remaining register/temporary-slot frontiers.
   Six thin Enemy ExAttack spawn leaves are now canonical exact too: fixed types
   1/7/8/9/23/24 at `0x0040C3C0/440/470/4A0/4D0/500` total 246 bytes.
   TH09 fixes Enemy position +0x2D74, manager side +0x31C, global ExAttack controller
   0x004A7E3C, and the maintained Spawn ABI; all six match on the first natural build.
   Next inspect the connected `0x0040C390/0x0040C3F0/0x0040C530` Enemy
   helper/owner seam before another large family.
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

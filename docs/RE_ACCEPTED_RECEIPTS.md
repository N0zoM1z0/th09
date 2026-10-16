# TH09 historical Factory acceptance receipts

> **Historical source-commit-bound evidence, NOT the active exactness ledger.**
> These originally accepted exact-sibling regressions belong only to their
> specified gameplay-source commits. Later docs/source commits do not
> automatically inherit them. Consult [the active handoff](RE_HANDOFF.md)
> and the canonical ledgers for current counts.

**Source-commit-bound Factory `strict-live-v1` forced-cold regression receipts
(`pass / accepted`) for *already-exact* functions:**

- Gameplay-source `09bb2a6`: `EtamaController::OnUpdate` (2,195 authored / 2,216 physical), `receipt:19f65147eaaaea7836cb2d705459c5486d6a4be987d4c8acd27b29babebf17c9`; `TitleSetupThread` (534), `receipt:5831f8f466f042ca5854d6c161a40bba37151c885872f3326623c3cb29f93b9e`; MusicRoom Update (2,258), `receipt:ec836857bcf5bdcaa7eea080398dbd623306205ea1b12091fbacf2c07b48d7f2`; ExAttack Type21 Update (1,074), `receipt:f722291fa5089d2088b87b74a978f34cd493aad5a49496fafe0a89b3639c5c79`; FrontCalc (1,491), `receipt:74198fed08b459dc9bcdef27c6d5e939afd2e1011b74527e82492ef64df2b8a2`.
- Gameplay-source `4fe7cb0`: TitleSetup (534), `receipt:afc85df61447369ef8ba8860939608f140b621e327086d1f459cebe957cd80b8`; MusicRoom Update (2,258), `receipt:acb48c0b18a6861b4288c38a27a2d5e7244f2e54be79cd325daef6aecf6bd637`.
- Gameplay-source `f951317`: exact `PlayerLifecycleView::OnUpdate` (522), `receipt:b58119d92a0c8eec924d4c68f99c174c9219d57081caf28c177d614202c0718d`.
- Additional exact `GameManager::OnUpdate` (1,230) and `PlayerCollisionQueryStateView::FindCollision` (602) have source-commit-bound acceptance in Git history and the indexed knowledge base.

**Current 2026-10-16 independent Factory source-bound cold regressions**
(`strict-live-v1`, gameplay-source commit `758d8c94c317f0fbd2afa4af68c19506f87d92f9`):

- Exact `EtamaController::OnUpdate @ 0x004146F0`, 2,195 logical / **2,216 physical** bytes: `receipt:a18133f3faec74e98ed8b9aaa97051e57d2cf772af9b23c767ed6d9aacd26d52`, **pass / accepted**.
- Exact `TitleScreenView::TitleSetupThread @ 0x004249E1`, **534** bytes: `receipt:20ccce3426869f8747bb28724747f8e6aec156c118c2212b2670f4ae4e51e787`, **pass / accepted**.
- Exact `TitleScreenView::OnUpdateMusicRoom @ 0x00426E05`, **2,258** bytes: `receipt:cc6a418bc10479a1267d85b018fc0f456d5f0cd1451d62aba73fc5397874a8f7`, **pass / accepted**.
- Exact `ExAttackUpdateCallbackType21 @ 0x0044B500`, **1,074** bytes: `receipt:6d40b47c803d92801cb8b1b0553ee99ed77fc12b23138d6726c9a93a1cb5417d`, **pass / accepted**.

These are **four already-exact owner regression receipts**, not new functions,
and the immutable source binding is to `758d8c9`; later documentation
commits are not retroactively accepted. Original remaining RunEcl,
Etama::OnDraw, ExAttack Type19/21 initializers and other nonexact owners
still require separate full-byte acceptance.

**After the GameplaySetupThread source-narrowing commit**
`4d9d2e31c530f6bdfd0e018d9f9b5b234115e560`, three separate
`strict-live-v1` forced-cold, source-commit-bound Factory codegen-exact
regressions returned **`pass / accepted`**:

- `GameManagerSetupLayout::OnUpdate @ 0x0041AA5F`, **1,230/1,230** bytes:
  `receipt:82d3cc97d60aa8050d97881619aa625537eeca75ed87cfa56099953a30f78a4e`.
- `GameManagerSetupLayout::AddedCallback @ 0x0041B76F`, **187/187** bytes:
  `receipt:5d136ca4a2776929e0e1843fb9f446b6c5fe57e9d457265d19b96746c197acae`.
- `GameManagerSetupLayout::DeletedCallback @ 0x0041B82A`, **422/422** bytes:
  `receipt:03e026c805c13d8921de69700ff62f7e25c98580f7988c6a098df880d9dd1b5f`.

The canonical GameManager ten-sibling local cold sweep, the 911-byte
noncrediting GameplaySetupThread frontier and **363 target-independent CI tests
(27 skipped)** also passed. These three receipt subjects **were already exact**;
none certifies `GameplaySetupThread`, native linking or runtime behavior.
This subsequent documentation-only checkpoint does not retroactively extend
the source-commit binding.

**New source-commit-bound ANM type-unification receipts** (same physical
Japanese TH09 v1.50a original, registered `strict-live-v1` forced VC7.1 cold
recompile, source commit `8c221cdbfb6ad657697599b541e829f60e71457e`):

- Exact `EffectManager::OnUpdate @ 0x0040CDD0`, **475/475 bytes**:
  `receipt:ad7dc44b3fe3bf56294dce49544434decc6acf38e0157f182094b1a658b6f45b` — **pass / accepted**.
- Exact `EffectManager::OnDraw @ 0x0040CFB0`, **183/183 bytes**:
  `receipt:13766e26949a07b8f1d01f3e16e48b8d36fd472ea221bda54f42526c3034228d` — **pass / accepted**.
- Exact `EffectManager::OnDrawShared @ 0x0040D070`, **146/146 bytes**:
  `receipt:6aa75811bcd29d39bbc31c9754c04eaf20bba60357012c2edb37acd6ced1c56a` — **pass / accepted**.

These are **three existing canonical exact regression confirmations** after
replacing an incorrect ANM source alias with genuine `AnmManager` C++ types.
All 15 EffectManager.cpp exact owners also passed complete same-TU local
comparison (2,530 physical bytes). A new full cold `--owner all` sweep after
the ANM type unification and Effect leaf guard passed **29 nonexact diagnostics
and 68 exact sibling replays**; CI passed **366 tests (27 skipped)**.
`EffectManager::AddedCallback` remains
NON-EXACT at 51 bytes / eight residual differences. Later documentation
commits do not change the receipts' immutable source binding.

**Current native RNG source-bound accepted replays** (`strict-live-v1`
forced VC7.1 rebuild against original TH09 v1.50a, exact source commit
`5a16237e18b1f13ec650b90c6b644d918ab37be4`):

- Existing exact `ScreenEffect::CalcShake @ 0x00422660`, **316/316** bytes,
  `receipt:f8170109ad5fba0372c8be8eab97f6a876db1f0c2cc30fcc6224111d57208f29`, **pass / accepted**.
- Existing exact `ScreenEffect::RegisterChain @ 0x00422D20`, **480/480 physical**
  (447 authored plus 33 true compiler switch bytes),
  `receipt:58489865290e4099e5d35849805f5189ce85f54f68492f0b463ace12d274cbee`, **pass / accepted**.
- Existing exact `ScreenEffect::DrawSquare @ 0x00422960`, **527/527**
  bytes under its separate `/Ob0` profile,
  `receipt:b8663157f0924c4ea888bcd2e5f1d861f66a7db7a0338b6b6380aaf2f097c41f`, **pass / accepted**.

All three are **previously exact owners**, not additions to the 939
canonical exact count. Independently, a post-change true cold
`check-boss-frontiers.py --owner all` passes **29 complete noncrediting owner
diagnostics and 68 full exact sibling replays**. It includes the 336/20/28
`CalcShakeEnvelope` owner, 15,564/598/30 `RunEcl`, 3,900/97/29 Enemy update,
600/268 Etama draw, the 763/6 and 727/9 ExAttack initializers and the
1,689/911 GameplaySetupThread. The cold JSON is ignored under
`.analysis/gpt-web-all-boss-native-rng-20261016.json` and earns no extra
exactness credit. A later documentation commit does not change the Factory
receipts' immutable gameplay-source binding.

**Etama/Effect genuine receiver source-commit-bound Factory cold receipts**
(`strict-live-v1`, original Japanese TH09 v1.50a PE, source commit
`2b216b3c96e29bd55a5af32af478ad23a33f0060`):

- `EffectManager::DrawBulletLayerEffects @ 0x0040D110`, **169/169 bytes**, now
  with actual `EffectManager` thiscall receiver:
  `receipt:270944e49bd65f7c1da1e6824679128d84ca3b8d3bb615b54198513e67a58dd9`, **pass / accepted**.
- `DrawSingleBullet @ 0x00413AF0`, **215 authored / 232 physical** bytes:
  `receipt:296551f9192cdc7d4e5c3b84d86e0bcee3ac858aad33de29c2c565a5df35c82f`, **pass / accepted**.
- `EtamaController::AddedCallback @ 0x00413E40`, **598 authored / 832 physical** bytes:
  `receipt:797c31ef5b39437e6db9ac3e3e8e57e3291f476fc821f16239de47ae8c75233f`, **pass / accepted**.
- `EtamaController::OnUpdate @ 0x004146F0`, **2,195 authored / 2,216 physical** bytes:
  `receipt:3dd9d7a8762fa42cfd72dbe6622f9e7a1d092222a5d377a67eed117b2cc4eba6`, **pass / accepted**.

These are **already exact** source-bound original regressions, not four
new exact functions; `EtamaController::OnDraw` remains 600/600 physical
but NON-EXACT with 268 byte differences. The independent whole-source
cold run retained 29 noncrediting bosses and 68 exact siblings. A later
docs-only checkpoint cannot change the above receipt source commit.

**ExAttack shared true record/side exact source-bound forced-cold receipts**
(`strict-live-v1`, attested original Japanese TH09 v1.50a,
source commit `c25b44b06a589884dc692964526a94d145f7cc40`):

- Existing exact `ExAttackRecordSideView::GetSideState @ 0x00440CB0`,
  **12/12 physical bytes**,
  `receipt:03c3187cbd5ff07b3a0440f35f6b4cb9e107aa3e68ab2a873d479e9725bcf29e`,
  **pass / accepted**.
- Existing exact `ExAttackAllocateRecordView::AllocateDynamicData @ 0x00440D90`,
  **81/81 physical bytes**,
  `receipt:02e13b132d444a337f55edcab5944d5ba1773fe9be38ea780173e4e18e23efe5`,
  **pass / accepted**.
- Existing exact `ExAttackUpdateCallbackType21 @ 0x0044B500`,
  **1,074/1,074 bytes**,
  `receipt:252778d75878abf02f021f3939cef92ee3b021b754802addda95ea5e0987f612`,
  **pass / accepted**.

These are **three previously exact units** after the shared C++ record,
side and EffectManager receiver model was committed. The two nonexact
Type19/21 initializers still differ in **6/9 target code bytes**, not
accepted. The full `--owner all` cold verification independently covered
29 noncrediting owners and 68 complete already exact siblings. These receipts
bind to the above gameplay-source commit; later documentation-only commits
do not extend that source identity.

**RunEcl opcode-4 actual ZunTimer owner source-bound exact regressions**:
registered Factory `strict-live-v1`, forced-pinned-VC7.1 TH09 v1.50a
original-bound replays from source commit
`ab6b6a138895801b19fba5498e662906ceb82776`, all **pass / accepted**:

- Genuine already-exact `ZunTimer::operator=(int) @0x00401500`, **9/9** bytes,
  `receipt:82bbd6e11555c8e87d83d7088a89068ee7fab3bbc9f342635ccc51ff5f55e596`.
- Already-exact `Th09EclRunControl::PopContext @0x00406680`, **147/147** bytes,
  `receipt:ca6a8207f37c24dc3c3c5c0b9cbbdba98b1f65d47c4cdef269522a2572bbf2af`.
- Already-exact `Th09EclRunMovement::BeginBoundaryAwareMove @0x00407C30`,
  **510/510** bytes,
  `receipt:b2383ac9f8554f29ad1f74c5efd15c499237e0915d39cab867567e9b38113e35`.

All are **previously exact**, not three new matches and not acceptance of
`RunEcl` itself. Source-cold `--owner all` also passes **29 complete
noncrediting open-owner diagnostics plus 68 complete exact sibling units**;
`RunEcl` remains 30 original-linked bytes different (opcode 4: 7,
opcode 155: 23). The exact source-commit binding above is immutable and
does not extend to subsequent handoff/documentation changes.

**Independent Title Options real-owner regression receipts** from source commit
`7026be1ba625a1d7831685d9524c1c99fc57cd04` (TH09 v1.50a
`strict-live-v1` forced VC7.1 cold recompile; all **pass / accepted**):

- Existing exact `TitleScreenView::PlayMenuSound @0x00422F17`, **34/34** bytes,
  genuine `SoundPlayer` method/global COFF identity:
  `receipt:a90f090fc29a76b05e404caecb43951833090cec8e80fa7b3b6bbbc8c5f28070`.
- Existing exact `TitleScreenView::TitleSetupThread @0x004249E1`, **534/534**
  bytes, `receipt:88f5c28aaaa6ae5d055acc4b2d00a8c999e556ca2da3778aef3f8b105585d439`.
- Existing exact `TitleScreenView::OnUpdateMusicRoom @0x00426E05`, **2,258/2,258**
  bytes, `receipt:a34d356de5ae62332be86e2543171808c47d3eb1aac37d794aa34bf835a0da31`.

These three are **previously exact** owner checks, not new functions;
`OnUpdateOptions` itself remains **2,045 original /2,048 source** and
NON-EXACT. The same source passes the real Title focused cold comparison,
independent exact 34-byte menu sound, and **389 public CI tests (27 skipped)**.
The immutable receipt scope is precisely the listed source commit, not a later
history-only documentation commit.

**Player GameManager playfield-bounds source-bound receipt:**

- Source commit `e7bb6db5c8b0cbc44f627bd0b5c20b3cd16cfdfe` replaces the four
  misleading scalar playfield globals with the actual `g_GameManager`
  `+0x360/+0x364/+0x368/+0x36C` fields. Its separate exact
  `PlayerLifecycleView::OnUpdate @0x0041E900` **522/522**-byte forced-cold
  `strict-live-v1` Factory replay is **pass / accepted**:
  `receipt:7b03d4eda61a67d80344ac7f636d489ddfa1bd4773c9307cfcf0941862991c38`.
- The immutable receipt covers this **previously exact** caller only. The
  `UpdateMovementAndOptions` full physical owner remains **1,900 bytes,
  66 actual fields, 23 calls and 129 differing original-linked bytes**;
  the independent original-exact **79-byte** coordinate initializer also
  replays exact. A new whole-source cold `--owner all` passes **29 complete
  noncrediting boss/leaf audits and 68 exact sibling units**, without changing
  the 939 canonical exact function count. A later docs-only checkpoint does
  not alter this receipt's source binding.

**Shared replay record, source-commit-bound exact regressions:** Against
Japanese TH09 v1.50a under Factory strict-live-v1, independent forced-cold
VC7.1 source commit d21c98d404cbaf83b12a2bfa6611a5119401df83:

- Previously exact TitleScreen::StartMenu @0x00429E23, 1,560 authored /
  1,592 original physical bytes (107 COFF fields):
  receipt:fa58503f939a3548ed56a852562094dde9d33401d8b521561dd4464825c22622
  — **pass / accepted**, including the nine source-renumbered jump labels.
- Previously exact ReplayManager::RecordInput @0x004202A0, 460 bytes:
  receipt:bee015670a1a64452a0228fe26528d2df78ce1a6b824dfb1f9699c3e5599cc11
  — **pass / accepted**.
- Previously exact ReplayManager::BeginRecordingStage @0x00421140,
  1,323 bytes under its distinct no-/EHsc profile:
  receipt:0181afe7c82836ca826e5a7a3b115ee0868e186f8e1764624a73075488dcd66e
  — **pass / accepted**.

These are **three already-exact** callback regressions, not three new
matches or acceptance of DrawReplayMenu/DrawReplaySave, both still NON-EXACT.
The independent complete cold sweep retains 29 noncrediting owners and
68 exact siblings; all 50 shared-TU canonical code owners were separately
checked byte-for-byte (22,504 physical bytes, 1,175 relocations). The
subsequent docs checkpoint does not extend immutable gameplay-source bindings.

**ScreenEffect 28→26 boss source-commit-bound accepted regressions:**
Original Japanese TH09 v1.50a, genuine Factory strict-live-v1,
forced-cold VC7.1 at source commit
c30a35a920881b1dc3674959f68a9f2bf3406552:

- Previously exact ScreenEffect::CalcShake @0x00422660, **316/316**
  original bytes: receipt:
  5b765d3491b74d87eaaff802770281171a069d07c8d19c7afe0d77f28ed43547
  — **pass / accepted**.
- Previously exact ScreenEffect::RegisterChain @0x00422D20,
  **480/480 physical bytes**, including 33 real compiler-switch data
  bytes and its separate no-EH profile: receipt:
  522d7ae9d73f44f4c10bcc5ee2bcad23315b52c3a33cb07d3ec12fd64e5862f0
  — **pass / accepted**.
- Previously exact ScreenEffect::DrawSquare @0x00422960,
  **527/527** bytes under its distinct /Ob0 profile: receipt:
  c8df10045d0306ef3302592f876c357fe9062a03b4df49fdb85bd9ed5f07a84f
  — **pass / accepted**.

The improved **336-byte CalcShakeEnvelope** is **still nonexact** with
26 unmatched complete original-bound physical bytes. These are three
already-exact sibling regressions and grant no canonical new exact credit.
All **18 exact ScreenEffect.cpp units** (2,562 physical bytes, 164
original fields), the latest **29 complete noncrediting bosses** and
**68 exact siblings** passed separate pinned source-cold comparison.
The documentation-only checkpoint does not extend the immutable source
commit binding. CI: **410 public tests, 27 skipped**, all remaining pass.

**Do not count these regressions as new exact matches.** Receipts do not bind
later source commits, diagnose adjacent non-exact owners, establish a native
Windows image or attest runtime semantics. Preserve current source-specific
compiler-private labels only after verifying their real owner-relative targets;
never transfer `$L...` spellings from older snapshots.


## Earlier exact closure and representative accepted landmark proofs

### New exact closure: Enemy collision wrapper

`EnemyAppendCollisionView::AppendPlayerCollisionBox @ 0x00410250` has a
canonical **82/82 full-byte** exact source-cold match with both independent
`REL32` calls (to `Float3::operator/` and `AppendBoxRecord`) and 30 decoded
instructions/`ret 8`. The preceding accepted `bc93e0d` source grouped the genuine definitions
in `src/PlayerCollisionAppendBox.cpp`; the current source further groups that
exact pair with unchanged `Float3::operator/` in `src/EnemyManagerCore.cpp`,
which also recovers ten additional target-bound bytes in the big Enemy caller. It changes the formerly 31 differing register/copy
bytes into the original grouped return copy, without replacing either
algorithm. All four exact same-TU owners cold-replay independently: **82/82** wrapper,
**205/205** append callee, **56/56** division, and **151/151** attached-effects
helper. The original 3,900-byte Enemy boss is still NON-EXACT (29 bytes).
The original C++ translation-unit partition and other compile-unit identities remain
unproven; this is a codegen/visibility finding, not whole-image or runtime
closure. **Factory strict-live-v1 accepted** a forced-cold replay of the new
82-byte exact wrapper from source commit `bc93e0d`:
`receipt:9ce6b5a1b03cb19de16acecb924700e1a21eb738c71291f7c82c71bf8007ba3c`.
The formerly exact 205-byte `AppendBoxRecord` received a separate pass/accepted
regression receipt from the **same** committed source:
`receipt:7badaa8a8ade77bd6e3824141fcbc1e15cef9a64cf15a8d5765ee4b9b463c9bd`.
These are **one new exact function plus one already-exact regression**, not two
new functions. All 3,900 bytes of the large Enemy update remain NON-EXACT
(29 differences); there is no partial credit.

## Closed codegen landmarks and accepted receipts

The exact-function ledger is the authority for **939 canonical exact functions** (including the newly Factory-accepted collision wrapper).
The following representative independently accepted owners have their
identities validated against the canonical match-unit manifest:

| Exact owner | Target/bytes | Canonical unit | Accepted source commit and receipt |
| --- | --- | --- | --- |
| `EnemyCoreUpdateAttachedEffects` | `0x0040F4C0`, **151/151**, 2 fields | `enemy-attached-effect-update` | `5fdb2c1`; `receipt:02924ae8158481decead09c66f44826d72860951dbf2c1baa97e6698b6be702d` |
| `Float3::operator/` | `0x0040F5A0`, **56/56**, 2 fields | `float3-divide` | `5fdb2c1`; `receipt:d9be0d756d2b9ab4d375d76b0a5d0bbf4d64fa1a52f6c1b594d3461d702bc330` |
| `PlayerCollisionQueryStateView::AppendBoxRecord` | `0x0040F8E0`, **205/205**, 2 fields | `player-collision-append-box` | `5fdb2c1`; `receipt:3775bc5ca1e2bc1bc26e0e8e8be75f0d0473a28d93a585319471559a66cdefa9` |
| `EnemyAppendCollisionView::AppendPlayerCollisionBox` | `0x00410250`, **82/82**, 2 fields | `enemy-collision-append-box` | `5fdb2c1`; `receipt:2e546dbc12acb9274f71d144af5329bdc11c720f3283754a1980f219ecb9864e` |
| `FrontMessageRuntimeView::LoadForSideCount` | `0x004182E0`, **405/405**, 18 fields | `front-message-load-for-side-count` | `4d921a2`; `receipt:8ea1d24dda7aab61c63e8e1fb288727adb1125dff033a86fecc1c81cd8658e98` |
| `FrontMessageRuntimeView::ReleaseForSideCount` | `0x00417520`, **271/271**, 14 fields | `front-message-release-for-side-count` | `99ea00b`; `receipt:3ad8649181efbc34d2b08b5d1ead5d96ddd77d7fec074b9a0508219dfdf567a1` |
| `PlayerShotDrawCallbackType1` | `0x00442220`, **204/204**, 6 fields | `player-shot-draw-callback-type1` | `495e944`; `receipt:7f20349cb55183e13f66e4b1ecabc24133fd379d2f9c2b91a27256929bef1927` |


Historical accepted Factory receipts, SHA references, and their **immutable
gameplay-source commit bindings** are archived verbatim in
[the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md). Important recent
anchors (all are **already exact siblings**, not new exact owners):

- ScreenEffect fade/hold improvement source `c30a35a`: accepted CalcShake,
  RegisterChain (separate no-EH) and DrawSquare (separate /Ob0).
- Shared replay-layout source `d21c98d`: accepted StartMenu, RecordInput
  and BeginRecordingStage (separate no-EH).
- Earlier RunEcl, Etama, TitleSetup, MusicRoom, ExAttack and other accepted
  regressions remain bound to their own earlier gameplay commits.

Neither the nearby non-exact boss nor the whole Windows i386 game product
receives acceptance from these exact sibling receipts.

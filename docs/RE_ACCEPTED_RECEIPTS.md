# TH09 historical Factory acceptance receipts

> **Historical source-commit-bound evidence, NOT the active exactness ledger.**
> These originally accepted exact-sibling regressions belong only to their
> specified gameplay-source commits. Later docs/source commits do not
> automatically inherit them. Consult [the active handoff](RE_HANDOFF.md)
> and the canonical ledgers for current counts.

## RunEcl authentic boss interrupt/SoundPlayer owner: exact ECL carrier regressions (October 18)

After correcting the source-only false `BossUiView::SetBossMarkerState`
reference to the real `AsciiBossMarkerVmView::SetBossMarkerInterrupt @0x4067D0`
(**NOT** distinct `AsciiManager::SetBossMarkerState @0x40F890`), unifying the
boss marker/sound globals across the genuine ECL interpreter and post-ECL
code, and validating unchanged original PE-linked bytes, Factory's independent
`strict-live-v1` service performed **forced-cold VC7.1 recompilation** from
precisely the **clean immutable source commit
`5808950a87a44def2ec708901974192e05f2d945`**, original Japanese TH09
1.50a, on these two already canonical exact functions:

- `EnemyPostEclUpdateMovement @0x00408180`, **967 logical / 992 complete
  physical bytes**, 41 reviewed PE/COFF relocations including real global
  `g_SoundPlayer @0x004DC698` and `SoundPlayer::PlaySoundByIdx @0x0043E2F0`,
  `job:0cabbe1637864891847dfbdee955b787`,
  `receipt:e7ec8a43d6f4274e998f927566ccada240ab9956152378e261a123687f4bea58`.
- `Th09EclRunControl::CompareOperands @0x00407700`, **842 logical / 892
  physical bytes**, 37 original-bound COFF relocations with 13 actual local
  VC7.1 label addresses proven unchanged,
  `job:da3d237c4820414eb9c58c644b1d2602`,
  `receipt:958830223941f4568d7db193fc32cb12c306c0b80bb86e71825daf9ccda1fd24`.

Both jobs independently finished `receipt_verdict=pass` and
`acceptance_decision=accepted`, without rejection reasons.
These were **already exact source functions**, not new exact credits; the
still nonexact **15,564-byte RunEcl 30-byte residual** itself has no accepted
exact claim from these jobs. The unchanged source-linked entire opcode/table
caller, canonical count **940/980**, and open Windows x86 product/resource/
runtime stages remain distinct. These receipts do not apply to future commits.

## Shared Front message exact callee across gameplay and Front lifecycle bosses (October 18)

After unifying `FrontMessageOwnerView` declarations into one checked
`src/FrontMessageOwnerView.hpp` and replacing the two fabricated call-facing
class names in nonexact `CleanupGameplayState` and `FrontAddedCallback`, the
Factory `strict-live-v1` independent replay service performed **forced-cold
VC7.1 recompilation** of these already exact owners from precisely the clean
source commit **`76d283d88f82c0f03c66b50042150c10651fd76d`**, against the
pinned, SHA-verified Japanese TH09 v1.50a original:

- `FrontMessageOwnerView::InitializeMessageRuntime @0x004181E0` — complete
  **133/133 bytes**, all **nine original reviewed relocations**; job
  `job:565ed20eb9494e948e7edef57e4b9a71`,
  `receipt:95e50ca1b479b1e245bac71b485eb76a6ced44594e6a7e5f363f474ccbfba9ab`.
- `GameManagerSetupLayout::OnUpdate @0x0041AA5F` — complete **1,230/1,230
  bytes**, **87 original reviewed relocations**; job
  `job:785a937e7706402f95034643fa61f616`,
  `receipt:981263782f7c7eb65631f68aa940dbf0e82d10f6d8807b7638701c6487c3e401`.

Both finished `receipt_verdict=pass`, `acceptance_decision=accepted`, with no
rejection reasons. They **reconfirm already exact** body ownership and
compilation at that immutable source commit; there are **zero newly exact
functions**. The independent GameManager Cleanup **425 target /410 source**
and FrontAdded **706-byte nonexact** callers were not accepted by either
receipt. Their native C++ external method symbols now agree with the genuine
FrontMessage callee, but whole-image native linking, data/resource closure and
runtime validation remain open. Canonical authored exact stays **940/980**.

## Enemy source-owner unification: Ascii boss-marker exact regressions (October 18)

After committing the real ANM/SoundPlayer/AsciiManager references and the
shared `AsciiBossMarkerVmView.hpp` for EnemyManager's open 3,900-byte boss,
Factory `strict-live-v1` independently **forced VC7.1 cold recompilation**
from the clean, immutable source commit
`6e1f3b43abdf0f8356ab75bc9f214a4e36ac8c9c` against the SHA-verified
Japanese TH09 v1.50a original for two **previously canonical exact** helper
functions in the same real AsciiBossMarkerVm translation unit:

- `AsciiBossMarkerVmView::SetBossMarkerPosition @0x004067F0`, **42/42 physical**
  bytes, `job:90cdb8dd87ee46afb807b3d281eb2e78`,
  `receipt:f6a06e8ac77ebb5041a90bf7b70c2eb1fb8712d88424752343aa9f4b90b0ee32`.
- `AsciiBossMarkerVmView::SetBossMarkerInterrupt @0x004067D0`, **32/32 physical**
  bytes, `job:56f7e51bf7534e4db89859d72dba0e99`,
  `receipt:69a834dc627a84c5c6dc4b8d2f07b9dd27d5d4bd8784ece6aa8eb17ac6bd704c`.

Both independently finished with `receipt_verdict=pass`,
`acceptance_decision=accepted` and zero rejection reasons. These receipts
confirm the reconstructed helper definitions remain exact **after** the
shared-header and real Enemy-callsite change. They **do not** accept the
still-nonexact EnemyManager OnUpdate itself (29 byte differences), create any
new exact function, prove whole i386 linking or settle source ownership for
unreviewed compiler-generated/folded bodies. The canonical total at this
source commit remains **940 of 980**.

## Title real AsciiManager source-bound exact regressions (October 18)

Two independent Factory `strict-live-v1` forced-cold original-bound jobs
were accepted for the **clean** gameplay-source commit
`787583c43aed2ab124bbaafdb17469e3df3d130c` after unifying the exact
Title callback's real `g_AsciiManager` and `AsciiManager::Reset` C++ symbol
identities. The separately unresolved one-byte folded call `@0x0043D2B0`
remains origin `unknown/review` and is **not** newly accepted:

- Existing exact `TitleScreenView::ActualAddedCallback @0x00424BF7`,
  **335/335 bytes**, 27 original reviewed PE/COFF fields, job
  `job:5b55a73af29045f4af49e3b686935439`,
  `receipt:daac95eafa24c694057298a3c7d8dd42df2e4cd07fd644f0da792ed36ef70dac`.
- Existing exact `TitleScreenView::OnUpdateStartMenu @0x00429E23`,
  **1,560 logical / 1,592 full physical bytes**, 107 reviewed fields
  including the independently original-address-attested switch labels, job
  `job:1aedbc266d6049d3b3a3b74e4b8f1fad`,
  `receipt:3dd1a0080f0fac30b53f9751483081dea940c264fe1ff1f1f484fec4c5a17281`.

Both jobs finished with `receipt_verdict=pass` and
`acceptance_decision=accepted`, no rejection reasons. These are **two new
verifications of already exact source**; they do not count two more canonical
matches, do not verify the still-open `DrawResult` 22-byte residual, and do
not imply native whole-product linkage, completed resource/data ownership,
or runtime validation. This immutable receipt is bound to `787583c`, **not**
to the subsequent RunEcl diagnostic/docs commit.

## Shared TitleScoreRecordView source-bound exact regressions (October 18)

After the real shared score-record type/header, producer/renderer global-symbol
correction, and nine separately original-VA-checked private-label updates were
committed, Factory `strict-live-v1` independently forced a **cold VC7.1
recompile** of three distinct already-exact owners. All three jobs bind to
precisely the **clean source commit
`6b47d0642dc231147bf493c07df26f88ecbc194d`**, Japanese original TH09
v1.50a and the `codegen_exact` claim at their own target entry:

- `TitleScoreRecordView::InsertIntoTable @0x00421800`, **216/216** full
  bytes (five real score-table DIR32 referents), job
  `job:8f06d81494274c4d979e4e7c6b1278ab`,
  `receipt:2e67473c8287e2b4878c337564b56565fb4d8deca0c5676f1c46052ebfba13a4`.
- `TitleScreenView::OnUpdateStartMenu @0x00429E23`, **1,560 authored /
  1,592 physical** code/table bytes, all 107 original-bound relocations
  including the nine correctly section-derived private switch labels, job
  `job:ba903cef47ee4239a365a27256d7aca8`,
  `receipt:f05f57a42fa90c857ee2234406222ca0e48a7a3a7dd59a87c303477b222c9c61`.
- `TitleScreenView::TitleSetupThread @0x004249E1`, **534/534** bytes,
  job `job:6e67c32f15404f299b78ab3e802a8428`,
  `receipt:b9e3bf3827501de70cb17c4bfb34a56f552070fdcafe28cbce11eebb1d68cf52`.

Each actual job finished with **`receipt_verdict=pass` and
`acceptance_decision=accepted`**, with zero reported rejection reasons.
These are independent new confirmations of *already canonical exact*
functions, **not three newly exact units**. The nonexact 1,939-byte
`DrawResult` is still 22 original-linked bytes apart. The canonical ledger
remains **940/980**; this archived receipt entry cannot establish native
whole-product linkage, data initializer closure, or acceptance for later
commits with a different source tree.

## Three established exact large owners, independently cold reverified (October 18)

All three Factory `strict-live-v1` jobs below were bound to clean, unchanged
source commit `83a94cfb204a17518d4efd9db7f8ec8d2e87a4fe` and original
Japanese TH09 v1.50a; each was actually forced recompiled and returned both
`receipt_verdict=pass` and `acceptance_decision=accepted`:

- **EtamaController::OnUpdate @0x004146F0**, **2,195 authored / 2,216
  physical** bytes; `job:050a5dfd52064836a29d9d410094c86e`,
  `receipt:4c65923a9a9fd47686ed38da882cbf649200a2b139c6b7ac95ea3567c34e4c75`.
- **TitleScreenView::TitleSetupThread @0x004249E1**, **534** bytes;
  `job:9414ae004ab542ffba434f29f0f35f28`,
  `receipt:ce0716bb454c60f074ac171f2be4545e69306eb6b977a05b7308493692004c14`.
- **TitleScreenView::OnUpdateMusicRoom @0x00426E05**, **2,258** bytes;
  `job:5941e8e7472b4c51af776cd8ce7b56f8`,
  `receipt:f72ac52baac3f14d195a37c191ab49de02da601e7affd4a4b8ea66c5966c8856`.

These are **source-bound new verifications of three *previously* exact
functions**, not three new matches or acceptance of RunEcl, Etama OnDraw,
Title Results, ExAttack Type19/21 initializers, native linking or runtime.
The subsequent opcode-155 semantic-gate commit changes diagnostics/docs/tests;
these receipts do not automatically attach to that later commit.

## ExAttackController genuine four-member exact regression receipts (October 18)

Against SHA-256-verified Japanese TH09 v1.50a, all four independently
accepted Factory `strict-live-v1` jobs performed **source-commit-bound forced
VC7.1 cold compilation** of functions that were *already exact*. The two
commits differ by a **documentation-only** correction that correctly records
`Spawn` as already 321-byte canonical exact; they do NOT alter gameplay C++:

- `ExAttackController::ReleaseExAttackRecords @0x00415650`, **70/70 bytes**,
  job `job:5700c84ded8f4bc69eb727a893a23357`, source commit
  `704e6feca3a0b0b844bbf28e1efcdcba3fcfd4d0`,
  `receipt:04d681de7053b0fbcb36192b3ab4a84bbbbea6510e76822218bc258e75dfb509`.
- `ExAttackController::Register @0x004157D0`, **264/264 bytes**,
  job `job:e4877a1ba4004aae9a164e13b835dd4b`, source commit
  `7528a7e915af69ea1da96b2c8ccfa1c48603d59e`,
  `receipt:8bde05a971cc2fa3b367cd1027b056a5c27e62b772416719404bb1e61c1c11b6`.
- `ExAttackController::Spawn @0x004151F0`, **321/321 bytes**,
  job `job:46e0040084404987a91d046ef2cd9cd2`, source commit
  `7528a7e915af69ea1da96b2c8ccfa1c48603d59e`,
  `receipt:6d7e15e6f0a58fa14c8b1883536da7bb6fd60af9354476ce6398ac3c282cb276`.
- `ExAttackController::Release @0x004156A0`, **66/66 bytes**,
  job `job:5ba4194d3af94226b39dca593d77419c`, source commit
  `7528a7e915af69ea1da96b2c8ccfa1c48603d59e`,
  `receipt:3623171bb3ca97dc1e9d25d314f9c4f5c1d744184ab168ed184bb903c510857c`.

For each: `receipt_verdict=pass` and `acceptance_decision=accepted`, with no
rejection reasons. The native `--owner exattack` review also compiles both
original-profile `ExAttackRecord` and `ExAttackController` special-member
constructors **15/15** and **28/28 physical bytes**, but their authorship
remains `unknown/review`: no claim or authored-exact credit has been made for
those two. The still-open `RunEcl`, Etama `OnDraw` and ExAttack Type19/21
initializers likewise receive **no new acceptance** from these four already
exact siblings. Future documentation-only commits cannot retroactively alter
these immutable source bindings.

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


## 2026-10-17: newly canonical exact TH09 crypt-table selector

The newly authored complete original-bound match for
`FileSystem::TryDecryptFromTable @ 0x0042C290` was source-cold recompiled
by Factory's pinned VC7.1 `windows.msvc71.function-exact` Oracle under
`strict-live-v1` **after** the gameplay-source/manifest commit
`9590206bb00496ea2ed3e9dc8374b97dd5b143be`:

- Full original physical **220/220 code bytes**, 12 exact COFF fields,
  Japanese v1.50a original SHA256
  `c9195eb6365a7e3d6905b3e2cead12ed480c4fd9ad968992db2b397e9b6fd8a7`;
  forced-cold job `job:acd4abf549b14135b8bb7b0ab19cab3c`: **`pass / accepted`**,
  `receipt:e5fbed2e35363191833752be9a9daef1968ab0ec01a83b06088188cad80ea5a9`,
  registry `registry:80579764b4b0ce5563dcdcc3083186a08d7a0335ba08e65a479f2fbef69aca76`.

This is **one genuinely newly exact function**, unlike earlier sibling-only
receipts: canonical **940/980** authored owners after promotion. The former
38-byte frontier and the intermediate two-byte `XOR` register-lifetime residual
were resolved with ordinary C++ offset/declaration scheduling. The exact
`FileSystem::Decrypt` and `FileSystem::Encrypt` bodies remain independent
canonical siblings. This receipt does not prove the Windows i386 whole image,
data ownership, runtime fidelity, nearby bosses, or later portability.

## Current shared Etama/Effect ANM-8 resident-prefix regression receipts

Original Japanese TH09 v1.50a, authenticated SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
All four independent **`strict-live-v1` forced-cold** Factory jobs bind
**exact gameplay source commit `74b973eba7b14c8ee35aea921809c60b88ca8d66`**,
where Etama's earlier private `AnmLoaded +0x04` resource-state view and Effect's
raw `void**` alias were consolidated into one asserted 8-byte prefix. The two
Etama exact carrier methods' 15 compiler-private `$L` label renames were
original-address reviewed before this commit. For each job:
**`receipt_verdict=pass`, `acceptance_decision=accepted`**, with no rejection reasons.

- `EtamaController::AddedCallback @0x00413E40`, **832/832 physical bytes** and
  30 original-bound COFF fields: job `job:0e861f19d06941c8a5878462423c2294`,
  `receipt:bead406a957aa191b580de896f5d42dd8cc24e92a14a93e982c881b7ae583ac4`.
- `DrawSingleBullet @0x00413AF0`, **215 authored /232 physical bytes** and
  16 original COFF fields: job `job:ca7c0b5330bb49378ad6146ddbb6e240`,
  `receipt:5eb1b56b7e59e9076dffed71c861badd2e5ec7a61f02063c02257a0b3781d91d`.
- `EtamaController::OnUpdate @0x004146F0`, **2,195 authored /2,216 physical bytes**:
  job `job:0fe5e33b4e3e455d8c4847bef4abcb0d`,
  `receipt:121530694ccd731bb92a3c71854a1c8b086225db1af221bdd407889b9a7a5270`.
- `EffectManager::OnUpdate @0x0040CDD0`, **475/475 physical bytes**:
  job `job:2d7248df74be4b55a040ca234d4a1f14`,
  `receipt:8bdb90d0e4eb5c03576683f00aeee5ff3ba9801ab1bb17b6fac2456892f93d0b`.

These are **four already exact source-owner regression confirmations**, not
four newly reconstructed exact functions. They do **not** accept the adjacent
600-byte `EtamaController::OnDraw` (268 full-owner differences), the 51-byte
`EffectManager::AddedCallback` (eight differences), RunEcl (30), the
Type19/21 initialization preheaders (six/nine), or the whole Windows i386
program and runtime. The canonical scoreboard remains **940/980 authored
source-present exact**, with 40 unclosed. Later docs-only commits do not
retroactively change these source commit bindings.

## EffectManager full same-carrier exact protection at source `7d3f89b`

The original Japanese TH09 v1.50a target (SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`)
was independently checked by Factory's **strict-live-v1 forced-cold
`windows.msvc71.function-exact`** replay with gameplay-source commit
**`7d3f89b0944d5769c88c2dc5f9dbfa17a5db9f08`**. Three previously
canonical-exact functions newly included in the complete 15-member
EffectManager.cpp regression inventory were independently accepted:

- `EffectManager::InitializeEffect @0x0040CA60`, **407/407 bytes**, 17 real
  relocation fields; job `job:a067dc90d2ce44588fac01c492dedef2`,
  `receipt:405a5181c4342727724719248de371e32b21091d389842753c9a5171b8707288`.
- `EffectManager::Create @0x0040D320`, **327/327 bytes**, 27 real COFF fields;
  job `job:6e6a791b2771475fa9fcda822afa945e`,
  `receipt:ee442c5c6b0929456850fb15c613de0e3bcdc2b5c3abada17e0b9e18ccef3b92`.
- `EffectManager::SpawnEffectWithVelocity @0x0040CCC0`, **175/175 bytes**, 1
  real COFF field; job `job:1999fe38e53f4c428c1a71b23305cb5b`,
  `receipt:a56ed8f92ad5f538f6c9ac8dc60ec5ce7e8570919d8e6722efa4d3c87f6b6e30`.

For each job, **`receipt_verdict=pass`**, **`acceptance_decision=accepted`**,
and no recorded acceptance objections. These three original codegen owners
were already exact in the authoritative canonical 940-function ledger.
They are immutable **source-commit-scoped regression receipts**; the newer
script/protection/checkpoint does not add their sizes to the exact function
count and does not certify the adjacent still-open 51-byte
`EffectManager::AddedCallback` (8 original differences), Etama OnDraw,
RunEcl, the whole Windows native game, or runtime fidelity. The complete
source-cold `--owner all` at `7d3f89b` passed **31 whole original-PE/COFF
owner diagnostics and 171/171 distinct already-exact functions**, with
**477 tests (28 skipped, zero failed)** under `scripts/ci.py`.

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

The exact-function ledger is the authority for **940 canonical exact functions**, including the newly Factory-accepted crypt-table selector and earlier collision wrapper.
The following representative independently accepted owners have their
identities validated against the canonical match-unit manifest:

| Exact owner | Target/bytes | Canonical unit | Accepted source commit and receipt |
| --- | --- | --- | --- |
| `FileSystem::TryDecryptFromTable` | `0x0042C290`, **220/220**, 12 fields | `file-system-try-decrypt-table` | `9590206`; `receipt:e5fbed2e35363191833752be9a9daef1968ab0ec01a83b06088188cad80ea5a9` |
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

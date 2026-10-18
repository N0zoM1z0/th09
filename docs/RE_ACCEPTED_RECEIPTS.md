# TH09 historical Factory acceptance receipts

> **Historical source-commit-bound evidence, NOT the active exactness ledger.**
> These originally accepted exact-sibling regressions belong only to their
> specified gameplay-source commits. Later docs/source commits do not
> automatically inherit them. Consult [the active handoff](RE_HANDOFF.md)
> and the canonical ledgers for current counts.

## October 19: Etama OnUpdate and Bullet DrawSingle native runtime-angle ABI accepted

Four independent Factory `strict-live-v1` original-Japanese-TH09-v1.50a,
SHA-256-attested (`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`),
original VC7.1 compiler cold-replay jobs returned
**`receipt_verdict=pass` / `acceptance_decision=accepted`** and no recorded
acceptance objections. Each binds to the same **immutable clean gameplay-source
commit `6d647f697274d99665f41c000c59ba4e88ac08b1`**:

- **`EtamaController::OnUpdate @0x004146F0`** — **2,195 authored / 2,216
  complete physical exact bytes**, 88 original PE/COFF-bound relocations.
  Its existing real `AddNormalizeAngle` call now uses the genuine global
  fastcall C++ owner rather than a source-only stdcall symbol at the same VA:
  `job:36eab768e0f149dfbf39e941b9c0bde5`,
  `receipt:31596edc7af74a9e64aa9b81c85402446ecfec0cee9e9678a40baa48114ce3e3`.
- **`DrawSingleBullet @0x00413AF0`** — **215 authored / 232 complete physical
  exact bytes**, 16 bound relocations including real math callee at
  `0x0042AED0`: `job:e82dd78c559745078bffddddc6e01d33`,
  `receipt:6c1997827b9ab9024160861dad19d187b90d733ff869709e5edaf83a77a22052`.
- **`EtamaController::AddedCallback @0x00413E40`** — **598 authored / 832
  complete physical exact bytes**, 30 true relocations; all related private
  `$L` spelling changes were original-VA-verified:
  `job:7b362dd0f732485e807da3a6197efef4`,
  `receipt:7d1345ce87888af00d1e44f2a2d2b3d7db35ec367c5b7c70483573ebd2cc0589`.
- **Native global `AddNormalizeAngle @0x0042AED0`** — **111/111 physical
  exact bytes** with authentic `?AddNormalizeAngle@@YIMMM@Z` C++ identity,
  the independent genuine target runtime-math leaf:
  `job:b7029d0910394cf48891b8540f8568a5`,
  `receipt:c0f36716ea35737af0e5dd160941cd9b8ba10e42e8432c977acf45d025728ad1`.

All four functions were **already canonical exact**; these receipts are
source-commit-bound regression acceptance, not four newly exact functions.
The independently compiled real `src/BulletManager.cpp` three compiler
carriers and `src/EtamaControllerDraw.cpp` preserve **20 distinct previously
exact function bodies / 7,775 full original physical bytes**, six genuine
original-VA fastcall call labels and 31 original-VA-preserving compiler-private
names. The clean-source complete `--owner all` cold replay checks **35
noncrediting full boss/leaf diagnostics and 247 distinct already-exact
canonical functions** (no duplicate shared math/sound leaf credit). Public CI
passed **633 tests / 28 skipped / zero failures**. The source-present exact
ledger stays **940/980**; `RunEcl` remains 15,564 physical bytes with 30
ordinary mismatches, Etama `OnDraw` remains 600/600 with 268, Title
`DrawResult` 1,939/1,939 with 22, ExAttack Type19/21 initializers six/nine.
These are not an accepted nonexact boss, whole-game native link, runtime
semantic proof or port result. They do not transfer to this later notes-only
checkpoint or to subsequent modified gameplay source.

## October 19: real SoundPlayer ExAttack Front GameWindow and Supervisor ABI receipts

Six independent Factory `strict-live-v1` forced-cold pinned VC7.1 comparisons
of the genuine Japanese TH09 v1.50a SHA-verified original executable
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`
returned **`receipt_verdict=pass`** and **`acceptance_decision=accepted`**
with no recorded objections. Their exact source binding is the **immutable
clean gameplay-source commit `a328381a7cbb5b8c4fafa58e92c3b7476f89cb6b`**:

- `FrontCalcCallback @0x00417630`, **1,491/1,491 entire exact bytes**,
  107 genuine source/original-PE-bound fields, and native `SoundPlayer`
  global/member references: `job:1f1880ff9bb449b4b79f7198a7f4df05`,
  `receipt:aa3bdce93ac3a33605f7e48812c4f6408e925cfd1f99bb883a919f35694a7c62`.
- `ExAttackUpdateCallbackType14_22 @0x00443DF0`, **805 logical / 824
  complete physical exact bytes**, 44 COFF fields including five
  original-VA-equivalent compiler-private switch labels and true global
  sound call: `job:7904b6fca1d3432b8de87e84e7dc3282`,
  `receipt:11c604ee69c3ecaa380bd90da82faae5d96c71aa09464d75e4b03f0beb75346c`.
- `GameWindow::Render @0x0042DC40`, **326/326 complete exact bytes**,
  30 original-bound fields including native `SoundPlayer::ProcessQueues`:
  `job:9c0abf1c79314aaa80c33766c19a2883`,
  `receipt:c2b959142ce175c9eecdedc18385f5117854f53e65b5c72f825c82308a858749`.
- `SoundPlayer::ProcessQueues @0x0043F1F0`, **1,414 logical / 1,456
  physical exact bytes**, independently rebuilt true sound callee with
  the canonical full original-owned tail:
  `job:3e618f4dd5f2414084140193099f3846`,
  `receipt:138a2d178ab9deca6da84aa3e072a32301789801ca31476114ffe9ac364f5f6e`.
- `SupervisorDeletedCallback @0x00431B70`, **405/405 complete exact
  bytes**, 44 original-bound fields, canonical-source-path anonymous C++
  namespace identity and genuine `QueueCommand` method:
  `job:1b1bbb6809a5472990b2b5482b4d9b37`,
  `receipt:5852845b897c5655bfa65621bc20111521f40e798797600aa52aea1ac88bf561`.
- `Supervisor::PlayMusic @0x00431930`, **254/254 complete exact bytes**,
  26 original-bound fields, with both genuine `SoundPlayer::QueueCommand`
  calls and canonical `src/`-path anonymous namespace symbol identities:
  `job:19839d5b2b6140fc8bba5655984daad5`,
  `receipt:a7a9ed17d7b760da90c1894755c6c8fd6c19b9dd57555816958496b2d0447e21`.

Each function was **already recorded as canonical exact**. These receipts
validate a real C++ linker/source identity repair, not six new matches. The
independent complete `--owner all` replay of that same clean source commit
passed **35 noncrediting original-linked boss/leaf diagnostics plus 247
distinct previously exact full-function comparisons**, with all unchanged
RunEcl, Etama, Title, Pause and ExAttack open residuals preserved. The
canonical count remains **940 exact / 980 source-present**. Staging-path
anonymous namespace compiler fingerprints were explicitly rejected in favor
of actual maintained `src/` source-path cold compilation. No completed native
whole-game executable, runtime semantic test, new nonexact-to-exact owner or
portability claim is implied. **These receipts do not transfer to this later
documentation-only commit or any subsequently modified gameplay source.**

## October 19: authentic Ascii menu audio and SoundPlayer resume C++ ABI accepted

Three separate Factory `strict-live-v1` original-linked, forced-cold pinned
VC7.1 replays completed **`receipt_verdict=pass`** and
**`acceptance_decision=accepted`** with no acceptance objections. The
original is Japanese TH09 1.50a SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
All three are scoped **only to the clean source commit
`e2da5e03907d28b9b7ba83c46cd41223c58cae9d`**:

- `AsciiMenuState4::OnUpdate @0x004355F0`, **1,144 logical / 1,172 full
  physical exact bytes**, 64 original-target-bound COFF fields, with nine
  genuine `g_SoundPlayer @0x004DC698` / native `PlaySoundByIdx @0x0043E2F0`
  pairs: `job:d971ed150f77462c8748516d77a1047b`,
  `receipt:1532d60a5c2c8218e666ecd46fd91bafe748b716cf6d375da19835f21fa58b0e`.
- `AsciiMenuState5::OnUpdate @0x00434E90`, **1,213 logical / 1,236 full
  physical exact bytes**, 86 COFF fields and four native sound pairs.
  This separately demonstrates the canonical exactness of the repaired
  **six previously stale VC7.1 private `$L` manifest symbol spellings**:
  `job:f89e58c4f4654beb93aed8431a6fa7df`,
  `receipt:e7c935c768aa6f4f98dae2772939f4ea9c4b96f66473864d2f904435831a9584`.
- `SoundPlayer::ResumeAfterPause @0x00423498`, authentic C++ method
  replacing the earlier source-only `AsciiSoundPlayerView` name, **15/15
  exact bytes** and independent native `SoundPlayer::QueueCommand` target
  `0x0043EE40`: `job:d31f9a058c954da5a9a8342f326ae468`,
  `receipt:fc96c1eaac06774a1d4c5a9341d29ff597fa62985ec13d0421cbc8a159578e1d`.

These are **revalidations of three previously canonical functions** and a
real cross-TU C++ linker/manifest correction. They are not a rise in the
**940 exact / 980 source-present** ledger. The independent full original
cold `--owner all` from this clean source passed **35 noncrediting whole-boss
and leaf diagnoses plus 236 distinct complete already-exact functions**,
including 122 functions rebuilt because the shared `SoundPlayer.hpp` changed
and 63 private-label VA-preserving renames. `PauseMenu::OnUpdate` is
**still nonexact**, 1,776 original vs 1,732 source physical bytes, with 44
missing bytes. `RunEcl` still has 30 ordinary original-linked differences,
Etama OnDraw 268, Title DrawResult 22, and ExAttack initializer Type19/21
six/nine. The receipts do **not** claim native whole-image linkage, runtime
behavior, a newly exact boss, or acceptance of subsequent commits.

## October 19: true RunEcl/ExAttack shared runtime-angle ABI accepted exact siblings

Five separate Factory `strict-live-v1` forced-cold source-compilation jobs
verified previously exact function bodies against the attested original
Japanese TH09 v1.50a SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`
and the pinned original VC7.1 compiler. All job source bindings are the
**one immutable clean gameplay-source commit
`8e92ce0c09fd8fcb0862fa8f540900275183c684`**. Each job completed with
**`receipt_verdict=pass`** and **`acceptance_decision=accepted`**, without
acceptance objections:

- **Global `AddNormalizeAngle @0x0042AED0`**, genuine fastcall C++ original
  `?AddNormalizeAngle@@YIMMM@Z`, **111/111 full exact bytes** with six
  independently original-bound COFF operands:
  `job:33643bec119242d698ef94a750f61ada`,
  `receipt:8e85a1325d03f2c471758171627781dbefcf5d89ea2b74a058e43e31155e9ed5`.
- **`ExAttackUpdateCallbackType21 @0x0044B500`**, **1,074/1,074 complete
  exact bytes** after shared true C++ math declaration replaced its
  source-only stdcall alias:
  `job:73ed03fd57934e2aa356c79b9e80a472`,
  `receipt:49dd0272e3553c82c6effb210cc640cd91c419235597fd48d17d9e4f1d7e54df`.
- **`EnemyPostEclUpdateMovement @0x00408180`**, **967 logical / 992 complete
  physical exact bytes**, including local compiler-tail data and true angle
  symbol/call addresses:
  `job:98a65ccca92b40e295349829e7ae21fd`,
  `receipt:5758423429365e0eb2a125e1cec3b8e6d1e82ccf2c4ff98eb79993fa34fe9246`.
- **`ExAttackUpdateCallbackType7 @0x004464A0`**, **799/799 physical exact
  bytes**, another genuine large ExAttack callback using the real callee:
  `job:1cf724e72d674cda99f0f385d90825d2`,
  `receipt:380c8900a676fb6f3a4bc59c888a9bbfb230ea6ae518784995ebc29c29eab9ae`.
- **`Th09EclRunControl::CompareOperands @0x00407700`**, **842 logical /
  892 complete physical exact bytes** despite safe renumbering of the
  actual original-target-identical VC7.1 `$L...` switch labels:
  `job:651db76a3e034880bee646dc8037d6d4`,
  `receipt:12d4f5ada3f64a7a13778357122285bb9735e6fd0baf01eb284db53bf03a8ac2`.

These are **revalidated existing exact** functions, NOT five newly exact
functions. The complete cold `--owner all` independently passed **35
noncrediting boss/leaf original-target diagnostics and 234 distinct complete
already-exact function replays** from the same gameplay source commit.
The **15,564-byte, 598-original-field `RunEcl` remains nonexact with 30
ordinary differences** exclusively in opcodes 4 and 155. The eight real
angle call identities and the 24 exact ECL translation-unit siblings are
protected, but that is **not** a Factory acceptance of RunEcl. Etama OnDraw
(268 differing bytes), Title DrawResult (22) and ExAttack Type19/21
initializers (six/nine) remain open. Native game product link, runtime
behavior and later semantic/port claims remain open. The ledger is unchanged
at **940 exact / 980 source-present**; acceptance is bound **only** to
`8e92ce0`, not to this later documentation-only commit.

## October 19: genuine CardAttack sound ABI across two large original callbacks

Factory `strict-live-v1` separately forced cold the SHA-verified original
Japanese TH09 v1.50a and pinned VC7.1 compiler against **clean immutable
CardAttack gameplay-source commit
`6f601f09c516656b6327f6c2d33f209733bd6236`**. This change replaces two
source-only fake `CardAttack*SoundPlayerView` linker types with the one genuine
`SoundPlayer g_SoundPlayer @0x004DC698` receiver and canonical method
`PlaySoundByIdx @0x0043E2F0`, without changing raw machine code. Two
independent completed jobs returned **`receipt_verdict=pass`** and
**`acceptance_decision=accepted`** with no rejection reasons:

- `CardAttack::OnUpdate @0x004041F0`, **536/536 entire physical exact bytes**,
  all **22** original-VA-bound COFF fields and genuine sound global/call
  symbols at original `+0x192/+0x1A9`: job
  `job:13cace2ea8db4f718a949a7c9ea864e1`,
  `receipt:63e7e2458d68eef32d444aa6cce8bf33a72e0fb904a373272ab2c727ea00ef23`.
- `CardAttackStart @0x00403E50`, **923/923 entire physical exact bytes**,
  all **29** source COFF fields and the true singleton/member at original
  `+0x232/+0x237`: job `job:5ff9331a91304b129950ae33bf2de3e7`,
  `receipt:bf4a114e6c80acbd83e279c6b9c9737e863ef663d804d89eb319e7e458b716ad`.

Both were **already canonical exact functions**, not newly exact codegen.
An independent full cold `--owner all` replay of the clean source separately
passes **35 noncrediting whole-boss/leaf diagnostics and 232 DISTINCT complete
previously exact functions**, including nine protected CardAttack source bodies.
The accepted source binding does **not** transfer to this later documentation
commit. Canonical totals stay **940 exact / 980 source-present**; the
RunEcl (30), Etama OnDraw (268), Title DrawResult (22), and Type19/21 ExAttack
init (six/nine) real original-code gaps and native product/runtime remain open.

## October 19: Etama/Bullet genuine audio and RunEcl exact-callee regressions

Factory `strict-live-v1` performed **seven independent forced-cold, original-
PE-bound VC7.1 comparisons** of previously exact TH09 functions from the
**one clean immutable gameplay-source commit**
`0508a96b736eb5a1b78b305eaadbd4e2b12012b0`. The target is the original
Japanese TH09 1.50a SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Each completed job independently returned `receipt_verdict=pass` and
`acceptance_decision=accepted` without acceptance objections:

- **`EtamaController::OnUpdate @0x004146F0`**, 2,195 logical / **2,216
  complete physical exact bytes** including all 88 original COFF fields and
  compiler-owned tail, genuine shared BulletManager source:
  `job:993630a7057b4449acce11ebd0663140`,
  `receipt:be9886642552ee4b6e2cfae07154a0c3edd068efa4987e4e49f019b6965d5c03`.
- **`Bullet::AdvanceTransformProgram @0x004141C0`**, 1,236 logical / **1,320
  complete physical exact bytes**, all 26 real relocated fields including
  the authentic SoundPlayer global and positioned/sound-index methods:
  `job:27f2d945df1b411fa901eee61d2963da`,
  `receipt:3edc646abf5d8b3395f9ae0a27b6356bf152fe298ae847a56cf980f63e0581ff`.
- **`Th09EclRunControl::CompareOperands @0x00407700`**, 842 logical / **892
  complete physical exact bytes**, with 13 rebuilt VC7.1-private `$L` labels
  each independently resolving to the unchanged original VA:
  `job:9bd619f9dc044fd5a698d71933b1ebb0`,
  `receipt:8838b1c4d83e4372fed54f7f25d51f0ab5dc6a58f221908ceabecf1e5399e6a2`.
- **`EtamaController::RemoveAllBullets @0x00412590`**, separately canonical
  true member **3/3 exact bytes**:
  `job:268b64976df446c184adeb573f8f7759`,
  `receipt:de02d6ae715cb13760d806ed248a8a9db59cd537227cec9dad3e0ff301e616f7`.
- **`EtamaController::RemoveBulletsInRadius @0x004125A0`**, separately
  canonical true member **3/3 exact bytes**:
  `job:ab89338745a0468e8ae220499a9137b9`,
  `receipt:0d03e7e1a61b8ed2b444080bace97cb05e501e52c2016826d9cd0805c8387bd3`.
- **`EtamaController::SpawnBulletPatternPrimary @0x004130F0`**, **203/203
  complete original-bound exact bytes** with authentic positioned audio:
  `job:bc5d8added004f3bb90005567fedc355`,
  `receipt:d1e4e0fae72916901fc55aaa9c2e6558ce9395c3c438752b5e433f58fd80e588`.
- **`SoundPlayer::PlaySoundPositionedByIdx @0x0043E380`**, **153/153
  complete exact bytes**, genuine separate method/callee source:
  `job:01743a67bae643989fd01d0d28891a6e`,
  `receipt:f07327207b674ff0634cbab91e0b9c6cf933f1a17d0d77d46767b8d54bc492ae`.

These are **seven revalidations of already canonical exact bodies**, not
seven newly exact functions or credits for the entire RunEcl interpreter.
All receipts bind only to the immutable `0508a96` gameplay source, **not to
this later receipt-archiving/docs commit**. An independent fresh complete
`--owner all` run passed **35 noncrediting owner diagnostics and 223 distinct
original-bound whole exact function regressions**. The still-open bosses
remain RunEcl (**15,564 physical / 598 original fields / 30 differences**),
Etama OnDraw (**600/268 differences**), Title DrawResult (**1,939/22**),
ExAttack Type19/21 init (**763/6**, **727/9**); no native Windows whole
product, exercised runtime, semantic, or portability closure is implied.
The authoritative canonical ledger remains **940 exact / 980 source-present**.

## October 19: ExAttack genuine SoundPlayer linkage accepted from source

The original TH09 v1.50a Japanese PE, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`,
was independently bound to the **clean immutable gameplay-source commit
`fc087e3ef1e0fb33c5e23ae674dae347b56b7407`** through Factory
`strict-live-v1` / forced-cold pinned VC7.1 compilation. Three separately
completed jobs returned **`receipt_verdict=pass`** and
**`acceptance_decision=accepted`** with no rejection reasons:

- **`ExAttackController::Spawn @0x004151F0`**: previously exact **321/321
  complete bytes**, all **seven source COFF original-bound relocation
  fields**, including actual `SoundPlayer g_SoundPlayer @0x004DC698`
  (`+0x8E`, `DIR32`) and genuine method
  `SoundPlayer::PlaySoundByIdx @0x0043E2F0` (`+0x93`, `REL32`) rather
  than the removed `ExAttackSoundPlayerView` alias. Job
  `job:8cfd6de4af32426282037ba11febd242`,
  `receipt:fd674b0860bdaadf7bc9c558c3b1f36b5c1d73ee9bb7ccc70a6494d29151a473`.
- **`SoundPlayer::PlaySoundByIdx @0x0043E2F0`**: real separately compiled
  receiver/callee, previously exact **140/140 complete bytes**,
  `job:e5ebfa2842634cc98adb14fc3aebef60`,
  `receipt:147b0acc95deb385d344b0b24bbe6e3a5346733af64a1b3459080429c9e41ea2`.
- **`ExAttackController::Register @0x004157D0`**: another true
  ExAttack-controller translation-unit sibling, previously exact **264/264
  complete bytes**, `job:c9dd70e6fd764165ba14cf4dc32f6b7a`,
  `receipt:51da231d2d6627d4482d64108edf9adc629fc9990b4229b98463cd040b8df1f1`.

This is newly **reconfirmed real cross-owner C++ native linker identity**, not
three newly reconstructed functions: the canonical ledger remains
**940 exact / 980 source-present**. All three receipts bind only the named
clean source commit, **not this later docs-only commit**. The 35 still-open
boss and leaf diagnostics from `--owner all` are not Exact Oracle receipts.
`RunEcl` (30 residual bytes), `EtamaController::OnDraw` (268), Title
`DrawResult` (22), ExAttack Type19/21 initialization (six/nine), native
whole-game link/runtime, and later semantic/port validation remain open.

## October 19: Etama update and Title exact regression quartet

The independent Factory `strict-live-v1` forced a fresh original-bound VC7.1
function comparison from **clean gameplay-source commit
`7edc4472933707224156aaabc2b8664aa316fa5e`** against the attested
Japanese TH09 v1.50a original SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Each separate completed job returned `receipt_verdict=pass` and
`acceptance_decision=accepted`, with no recorded acceptance objections:

- `EtamaController::OnUpdate @0x004146F0`, **2,195 logical / 2,216
  complete physical exact bytes**, including post-function tail and all
  reviewed COFF/retail fields: `job:1b89c6f17d974e93bafa526043e336be`,
  `receipt:94b633a2b55f91f419e18ba1adea67eb8151d47d91ff0082d55fb3db7b1bf438`.
- `TitleScreenView::OnUpdateMusicRoom @0x00426E05`, **2,258 physical exact
  bytes**: `job:d65f64978eef477bbeabee064adb6147`,
  `receipt:5e21808fb9704988e41968160ac4003f82808c180a1f107aef9792fcd9f0c3bd`.
- `TitleScreenView::TitleSetupThread @0x004249E1`, **534 physical exact
  bytes**: `job:60d5acfe159141d1bca67d2ff5b5c592`,
  `receipt:1372474eaba35ff1510d7f7c527e110dc1dfcef3c92c70f2e8d1f3dd2ea0e1cf`.
- `DrawSingleBullet @0x00413AF0`, **215 logical / 232 complete physical
  exact bytes** (including its carrier tail):
  `job:824b70faba52400aabadb8cb33a56b3a`,
  `receipt:4ed5446dfca2e5cab8ffb2b5a22c802558ce799fe265390e9ffebb7bec298be8`.

These four are **revalidations of already canonical exact functions**, not
new exact credit: the ledger remains **940 / 980**. The four original-bound
receipts are scoped to the identified earlier clean source commit only,
**not** to this later notes-only checkpoint. No replay in this batch accepted
`RunEcl`, `EtamaController::OnDraw`, `TitleScreenView::DrawResult`, the
ExAttack Type19/21 initializers, whole-native-image closure or gameplay runtime.

## ScreenEffect authentic GameManager owner: three independent exact regressions (October 18)

After the original-PE-backed `ScreenEffectGameManagerView` duplicate global
was replaced by the ONE genuine `AsciiGameManagerView` global at
`0x004A7D90` and `int value348` given its original observed byte offset,
Factory `strict-live-v1` independently forced the pinned VC7.1 source cold
build against the SHA-verified TH09 v1.50a Japanese original. All three
jobs are bound to precisely **clean source commit
`868a572bf872bc6f839989b966d224f1cae4decf`**:

- Previously exact `ScreenEffect::CalcShake @0x00422660`, complete
  **316/316 physical bytes** and its genuine **16 original relocation
  fields**, including two canonical `g_GameManager` native symbol fields.
  Job `job:8b39ae93702444b0b547d6c1b099d75f`,
  `receipt:7e7d28fa787028c452ae18972ed4b31e65645d6785d4117150a87dd74c1fa1b7`.
- Previously exact `ScreenEffect::CalcFadeOut @0x00422470`, complete
  **123/123 physical bytes**, eight original fields and two canonical
  global references. Job `job:5841d49e054f44339bf2548afc3c2c0d`,
  `receipt:06e67a387f3904a90b2557588e64ebcd1eeb13a1eaf202596e458dbc807bf3c8`.
- Previously exact `ScreenEffect::RegisterChain @0x00422D20`, **447 logical
  /480 complete physical bytes**, including the source-carrier table tail
  and all **55 original relocations**, including nine verified
  same-original-VA compiler-private `$L` labels. Job
  `job:a0b5baa25c804eb9ad33c3b95255601e`,
  `receipt:33c6a2e05c48b2b60be127be29c99ed0e69b620871d37622f4094a3ec54de8f4`.

All three separately returned `receipt_verdict=pass`,
`acceptance_decision=accepted` with no rejection reasons. These receipts
**reconfirm three previously accepted exact bodies** and DO NOT accept the
still-nonexact 336-byte `ScreenEffect::CalcShakeEnvelope` (26 different
ordinary bytes), much less create three new exact functions. Source-cold
complete `--owner all` checks **35 noncrediting original-bound diagnostics,
204 DISTINCT already canonical exact source functions**. Canonical count
remains **940/980** and whole native link/runtime/data closure are open.
Receipt provenance applies ONLY to the named clean commit; subsequent
commits and partial near matches do not inherit Factory acceptance.

## TitleScreen and MusicRoom genuine SoundPlayer ABI: three strict-live-v1 exact regressions (October 18)

After unifying the fake `SoundPlayerView` globals in both the main
`TitleScreen.cpp` and separately compiled `TitleScreenMusicRoom.cpp` with the
canonical `SoundPlayer g_SoundPlayer @0x004DC698`, real
`PlaySoundByIdx @0x0043E2F0` and `QueueCommand @0x0043EE40`, the independent
Factory `strict-live-v1` source service forced fresh pinned VC7.1 builds
against the SHA-verified Japanese TH09 v1.50a original from **exactly** the
clean immutable source commit **`d867bbcb2e3d8228090cca37545f69ee57cb238e`**:

- Previously exact `TitleScreenView::OnUpdateMusicRoom @0x00426E05`:
  **2,258/2,258 physical bytes, all 74 original-bound COFF fields**,
  including the authentic SoundPlayer global and three real queued-command
  call referents. Job `job:2a8078fe13e04753bceb35ef0013ee8a`,
  `receipt:c98fda03e9e45227b6e6bcb73872d3ea92b13df57f047ae29c4e4e7f34789638`.
- Previously exact `SoundPlayer::QueueCommand @0x0043EE40`:
  **105/105 complete physical bytes**, its independently compiled true
  source method whose original-target CALLs are now shared by MusicRoom.
  Job `job:dd59fb7644b14f09a554150bd096e7fc`,
  `receipt:9e3b9af6d9751fe277c94f28cd2dd202b1ba365b4638c300e52ad666975d7524`.
- Previously exact `TitleScreenView::OnUpdateStartMenu @0x00429E23`:
  **1,560 logical / 1,592 complete physical bytes, all 107 original
  relocation fields**, including nine same-original-VA compiler-local `$L`
  switch labels independently reattested after the shared header changed.
  Job `job:31de3e3e7d174e369d36fbe9c5ba2d49`,
  `receipt:bb7a1ceafe724c039282cf7b43547c398af1c480594105dc07169ad12b5035f8`.

All three jobs independently returned `receipt_verdict=pass`,
`acceptance_decision=accepted`, with no rejection reasons. These **reconfirm
three already canonical exact functions** and do NOT create newly exact
codegen: the open Title `OnUpdateResult` is still 485 target vs 484 source
bytes, `DrawResult` remains 1,939/1,939 with 22 different ordinary bytes,
and MusicRoom's full native Title ANM/data dependencies and whole-product
Windows i386 build/runtime are not closed. The same source-cold `--owner all`
full-PE audit verifies **35 noncrediting owner diagnostics and 204 distinct
already-exact full functions** (including one newly checked, not newly
accepted, SoundPlayer::QueueCommand leaf); the canonical ledger remains
**940 exact among 980 source-present**. Receipts bind only the named clean
Git source commit, not subsequent commits or fresh native product claims.

## ExAttack Type3/Type4 shared original GameManager ABI: source-bound exact regressions (October 18)

After correcting the two non-equivalent Type3 and Type4 source-only
`g_GameManager` C++ types to one genuine `AsciiGameManagerView @0x004A7D90`,
using the independently exact `InverseTransformPopupX/Y` methods at
`0x00442420 / 0x00442450`, Factory `strict-live-v1` performed three
**independent forced-cold VC7.1 compilations** of *already exact* source
functions against the SHA-verified original Japanese TH09 v1.50a, strictly
bound to the **clean immutable source commit
`bc80608010b64d321332761c81a0e3a1e27dec0c`**:

- `ExAttackInitializeCallbackType4 @0x00442A60`: **368/368 complete physical
  bytes, 22 original-PE-linked COFF fields**, including nine canonical
  `AsciiGameManagerView` global/member identities (five global accesses have
  a real +0x04 COFF addend): `job:ae7194437f914602a4af2345042cf480`,
  `receipt:56bb237c5044bcc38f6b1ed50aaafbce7afa0db8b4c589364b6c5ce66f8decbd`.
- `AsciiGameManagerView::InverseTransformPopupX @0x00442420`: **46/46
  physical bytes**, two independently reviewed original COFF fields,
  `job:dedb741604c34e0bad4dd0a68f6939c2`,
  `receipt:6b9eb8acdfdea6fbe80376bba2e6aabd98befdcd12ede4bfd9407e6ff934b946`.
- `AsciiGameManagerView::InverseTransformPopupY @0x00442450`: **46/46
  physical bytes**, two independently reviewed original COFF fields,
  `job:8616c4efb17d45c0888b313ee564c733`,
  `receipt:ba7f80d1b88a36e4a9db1e294d39136d4311ff67a4e7f48243032145e1e21a01`.

All three returned `receipt_verdict=pass`,
`acceptance_decision=accepted`, without rejection reasons. These are
**rechecks of three previously canonical exact functions**, not newly exact
code; Type3 Update itself is **nonexact (780/780 bytes, 38 original fields,
15 residual machine bytes)** and was not accepted by these jobs. The
original-target-bound source-cold `--owner all` suite passes **35
noncrediting owner diagnostics and 203 distinct already-exact functions**,
including four 46-byte GameManagerCoordinates functions as one compilation
carrier and the separately exact 368-byte Type4 initializer. The canonical
accepted count remains **940/980**. Native full-image linking, static data,
resources and deterministic gameplay/runtime comparison remain open and are
not proven by these receipts. Future commits require fresh source binding.

## Shared ECL ZunTimer ABI across exact compiler profiles (October 18)

After the former source-only `Th09EclTimerStorageView` class was replaced
with the already genuine, source-defined `ZunTimer` type and its 0x0C-byte
original-backed memory layout, the independent Factory `strict-live-v1`
service forced a **new cold VC7.1 build** against verified original TH09
v1.50a Japanese code, bound to exactly the clean source commit
**`61dfa79e1a1dab3bc73cb9b99f73a7166d7f9890`**:

- Already exact `EnemyPostEclUpdateMovement @0x00408180`, **967 logical /
  992 complete physical bytes, all 41 true relocation fields**, from the
  main `/O2 /Ob1` EclManager carrier: `job:3fe4c719c3ab492cbb35e882a10fb777`,
  `receipt:7a2bb7354477059d76a4008a87ab2c18df7d9cf675529ef35dd9798eded68cca`.
- Already exact `Th09EclRunControl::CompareOperands @0x00407700`,
  **842 logical / 892 complete physical bytes, all 37 true fields**,
  from its independently pinned original reviewed EclManager compiler profile:
  `job:da38da846af24eeca5d67078a024fbcb`,
  `receipt:b8f61673eb373114b38f08063c09c1aa8fe2f11b5a55f84559d770ba76c47425`.
- Already exact `ZunTimer::operator>=(int) @0x00401520`, **20/20 bytes**
  from the genuine **`/Ob0`** BackgroundStageScript source-carrier profile,
  which must not be silently replaced by the normal `/Ob1` ECL profile:
  `job:ee1c53ebe7984fb8b6df0242ae4e7e7d`,
  `receipt:eb67aa33c6fa96e1d0c4577c79d9f8da8b922fcac54208e2494f7b3a66e33f3e`.

All three jobs completed with `receipt_verdict=pass`,
`acceptance_decision=accepted`, and no rejection reasons. These are
**re-verifications of three previously canonical exact functions**;
**no 941st exact function** is claimed. They do not accept the still-open
15,564-byte RunEcl boss (30 different bytes), the 517-vs-516-byte biased
movement helper, the full native i386 link, or any later source commit.
The `--owner all` original-PE-linked local source-cold gate checks 35
noncrediting diagnostics and **198 different already exact complete owners**;
the separate canonical function count remains **940/980**.

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

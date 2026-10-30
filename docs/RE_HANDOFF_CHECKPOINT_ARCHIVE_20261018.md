# TH09 — archived checkpoint details

> **Historical source/experiment notes, NOT a live handoff and NOT a new
> codegen acceptance receipt.** Frozen from the previous long-form
> `docs/RE_HANDOFF.md` at repository baseline `0df4c1d`. These notes retain
> their original source-commit scope and can include superseded explanations.
> For the authoritative next-session ledger, frontiers and commands, use
> [the active handoff](RE_HANDOFF.md). For actual accepted SHA-bound receipts,
> use [the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md), not an
> experimental narrative. Retained for reproducibility, not instructions.

## Archived checkpoints (historical)

- **ScreenEffect and all-gameplay exact GameManager native owner (October 18):**
  Original `ScreenEffect::CalcShakeEnvelope @0x004227A0` (336-byte
  authored/nonexact boss) and 18 independently exact ScreenEffect source
  owners previously declared the same genuine `g_GameManager @0x004A7D90`
  as a *different native C++ `ScreenEffectGameManagerView` global*, despite
  exact `AsciiGameManagerView` elsewhere. The shared genuine header now
  preserves source-backed `value348` as a 32-bit member at `+0x348`, alongside
  the previously verified flags `+0x134`, two side records (0x38 bytes each),
  side flag low byte `+0x34`, menu `+0x13C/0x13D`, and coordinates
  `+0x350/+0x354`. `ScreenEffect.cpp` uses the real class/global; opaque
  side-flag low-byte access is explicit without claiming unreviewed fields.
  Original-target SHA-bound full source-cold exact proof covers **18 full
  ScreenEffect owners**, split across all three real pinned VC7.1 profiles:
  **2,562 physical bytes and 164 relocation fields**, with only four genuine
  GameManager global C++ names and nine locally renumbered `$L` switch labels
  changed. `CalcShakeEnvelope` still generates **336/336 physical bytes,
  all 20 original relocations, eight call targets and 26 CFG blocks**, with
  precisely its original **26 unlike bytes** (NO new exact credit).
  The shared-header change also renumbered **8** exact ASCII menu-state4,
  **seven** exact PostEclMovement, **13** exact ECL CompareOperands,
  **five** exact Etama DrawSingleBullet and **ten** exact Etama AddedCallback
  private labels. Every one of these 43 symbols was independently checked
  from the current section-relative COFF offset against the unchanged
  Japanese retail VA/encoded DWORD before its *spelling only* was updated;
  the full related owner bytes and field geometry stay exact.
  Whole `--owner all` reports **35 noncrediting original-bound function
  diagnostics / 204 distinct previously exact complete owners**. Native
  Windows link/data/resources and runtime proof remain open; ledger stays
  **980 authored / 940 canonical exact / 40 open**.
  The actual retail GameManager field carries an original `+0x348`
  COFF addend, verified without changing its decoded encoding.
  Three Factory `strict-live-v1` forced-cold `pass / accepted` receipts
  independently recheck the already exact **316-byte CalcShake**,
  **123-byte CalcFadeOut**, and **480-byte RegisterChain** at immutable
  clean source commit `868a572`. See `RE_ACCEPTED_RECEIPTS.md` for
  job/receipt identifiers; none accepts the open 336-byte shake boss.

- **Title/MusicRoom genuine SoundPlayer native owner (October 18):**
  Separate `TitleScreen.cpp` and `TitleScreenMusicRoom.cpp` compiled the
  *same original* `g_SoundPlayer @0x004DC698` as two fake
  `SoundPlayerView` types, hiding real exact callees
  `PlaySoundByIdx @0x0043E2F0` and `QueueCommand @0x0043EE40` behind
  incompatible C++ linker symbols. Both now include genuine
  `SoundPlayer.hpp` and have NO fabricated global, method or stub.
  Independently rebuilt **three real pinned source compiler carriers**:
  38 complete already-exact `TitleScreen.cpp` owners plus two independently
  exact MusicRoom owners — **40 full original-bound functions, 20,878
  verified physical bytes, 1,023 real COFF fields**. All generated raw
  function bytes are identical before/after. Only 35 genuine audio
  C++-symbol references (12 global, 20 PlaySound, three QueueCommand)
  and **nine** compiler-private `$L` StartMenu labels changed names.
  Original PE addresses, relocation offsets/kinds/addends, actual local-label
  section-relative VAs and all code bytes were independently held fixed;
  manifest edits touch only those 44 exact-reviewed symbol spellings.
  `--owner title` now also cold-builds the authentic 140-byte sound-play and
  105-byte queue-command exact leaves in their two real source carriers;
  **43 distinct already exact owners** pass, while `--owner all` avoids
  double-counting sound-play, already checked by RunEcl. Open Title
  OnUpdateResult remains 485 original /484 candidate, 34 original fields;
  DrawResult still 1,939/1,939 with 22 ordinary mismatches; ReplaySave,
  Options, cursor leaves and whole native Title ANM/data closure are open.
  No new canonical exact credit: still **940/980**, not 941.
  Independent Factory `strict-live-v1` forced-cold **pass / accepted**
  receipts for MusicRoom **2,258/2,258**, QueueCommand **105/105** and
  physical StartMenu **1,592/1,592** are archived in
  `RE_ACCEPTED_RECEIPTS.md`, each bound to clean source commit `d867bbc`.
  Those are regressions of existing exact owners, not new exact functions.

- **Real GameManager unified across RunEcl, ExAttack, ASCII/Etama exact owners (October 18):**
  The source-only `ExAttackType8GameManagerView` previously redeclared
  `g_GameManager @0x004A7D90`, `TransformPopupX/Y @0x00401680/0x004016B0`,
  side `+0x04/+0x08/+0x0C` and speed `+0xEC` as a SECOND native C++ owner.
  It now is just a typedef to the already genuine `AsciiGameManagerView`,
  whose physically verified 0x38 side prefix, original player/Etama/ANM
  pointers, speed `+0xEC`, difficulty `+0x11C`, and exact transform callee
  symbols have one source declaration and compile-time field checks.
  Independent pinned whole-source TU before/after VC7.1 builds confirm
  **byte-for-byte identical RunEcl 15,564/598 COFF fields**, Type19/21
  initializers **763/57 and 727/53 fields** and all exact EclManager
  owners. Source-cold full original PE compares confirm **18 main + six
  alternate-profile + two EclHelpers exact** owners, with only verified
  C++-decorated / private `$L` symbol-name changes (immutable field
  positions, original targets/addends and matched physical bytes).
  Shared-header label renumbering also touched the separately exact
  **1,172-byte full ASCII menu state4** and two Etama physical owners
  (**232-byte single bullet / 832-byte AddedCallback**). All 23 renamed
  private labels were independently checked against original PE destination
  bytes and current COFF section-relative offsets; both related cold
  owner families now replay exact. The updated Type7/Type21 and Type19/21
  strict inspectors recognize only the real global name with the same
  target-bound address. Focused RunEcl/ExAttack/Etama/Pause checks and
  `--owner all` verify **35 noncrediting owner diagnostics / 203 distinct
  existing complete exact units**; updated tests reject a second global,
  fake method referents, wrong speed/side offsets or altered jump-table VAs.
  Crucially **NO big-function byte-exact credit**: RunEcl still differs by
  30 ordinary bytes, Type19/21 by 6/9, Etama OnDraw 268; all former
  original identities, dispatch graphs and semantically gated behavior
  remain intact. Canonical authored exact remains **940/980**, native
  whole-program link and runtime/port gates still open.

- **ExAttack Type3 boss + Type4 exact initializer share the real GameManager (October 18):**
  Original `ExAttackUpdateCallbackType3 @0x00442750` (780-byte nonexact
  owner) and previously exact `ExAttackInitializeCallbackType4 @0x00442A60`
  (368-byte exact owner) were using two *different*, invented C++ types for
  the SAME real global `g_GameManager @0x004A7D90` and exact
  `InverseTransformPopupX/Y @0x00442420 / 0x00442450` callees. Both now
  include the original-backed shared `AsciiGameManagerView.hpp`. Actual
  side-array player pointer is `side+0x04` in a `0x38`-byte side record;
  Type3 uses genuine `PlayerLifecycleView::CheckBulletCollision`, and
  Type4 retains only its independently bounded player ANM field at
  `+0xBC`, with a compile-time member-offset assertion. Verified pinned
  before/after **complete raw machine code byte-for-byte equal** for Type3
  **780/780, 38 unchanged COFF fields** (15 renamed authentic caller
  symbols) and Type4 **368/368, 22 unchanged fields** (nine renamed source
  symbols). For Type4, all seven real global DIR32 fields and both real
  inverse-transform REL32 fields independently resolve back to the SAME
  immutable original PE VAs, field positions/types and actual COFF addends.
  Its strictly bound entire 368-byte exact match manifest was updated only
  in the nine decorated C++ name strings. Type3's original PE/COFF auditor
  now rejects the two fake GameManager names: it still protects all 38
  actual fields, 24 call targets, full CFG and 15 genuine still-different
  bytes (`middle.z` store after tail-X load vs original pre-load chronology).
  A focused `--owner type3` now source-cold replays the **four 46-byte
  already exact** `AsciiGameManagerView` coordinate methods in one actual
  optimizer carrier and the separately exact **368-byte Type4 initializer**,
  as well as the already exact Type3 initializer: **six independent exact
  original-bound units**, zero new exact credit. All native whole-program
  linking and runtime behavior remain open; canonical ledger **940/980**.
  Independent Factory forced-cold `pass / accepted` receipts for **Type4
  Initialize 368/368** and the two true **InverseTransformPopup 46/46**
  exact methods are archived in `RE_ACCEPTED_RECEIPTS.md`, all bound to
  clean source commit `bc80608`. They do not accept Type3 Update itself.

- **Full ECL timer type now genuinely shares ZunTimer (October 18):**
  Revisited `RunEcl` rather than relying on its former 0x0C source-only
  `Th09EclTimerStorageView` class. Original-target-backed
  `ZunTimer` is independently exact at several separate method entries;
  source now `typedef ZunTimer Th09EclTimerStorageView` so all ECL call
  contexts, interpolation slots, boss/movement timers and source helper
  functions refer to the SAME decorated C++ member ABI. The 0x0C timer
  and `previous +0x00,subFrame +0x04,current +0x08` layout are compile-time
  asserted; the one original raw current-store callsite is updated.
  Source-cold before/after **15,564 physical RunEcl bytes and all 598
  COFF fields are completely identical**, with **24 genuine timer method
  call records** replacing fake C++ names in RunEcl; actual PE-linked
  referents, switch/easing tables, operand positions, direct and indirect
  call order remain unchanged. Strict complete physical checks cover all
  **18 canonical exact** main EclManager functions, **six alternate-profile
  exact** EclManager functions, and **two exact** EclHelpers functions;
  their compiler-private `$L` names were reattested by section-relative VA
  and updated ONLY in reviewed manifest symbol strings. The nonexact
  `MoveRandomBiased` is still **517 candidate/516 target** with 148
  overlapping differing bytes; old alias bindings were removed from its
  28-original-field auditor and the 598-field full-callsite auditor.
  Eight separate real ZunTimer exact methods (including `/Ob0`
  greater-equal and distinct exact equality owner) are now rebuilt from
  **three pinned compiler/source carriers** under focused `--owner runecl`.
  Pure tests reject any faked class/target/address/carrier/profile, and
  specifically require the original `/Ob0` vs `/Ob1` optimization setting;
  the four-byte folded integer getter @0x435F00 stays origin unknown.
  Focused RunEcl checks **36 already exact** owners, whole
  `--owner all` **35 original-bound noncrediting diagnostics and 198
  distinct complete exact owners**. The true RunEcl nonexact residual is
  unchanged at **30 bytes** (7 opcode 4, 23 opcode 155); the signed-time
  and masked-bit semantic gates remain noncrediting. Canonical total stays
  **940/980**; no game product, static resource graph or native link is
  claimed complete. The separate Etama, Title, ExAttack frontiers remain open.
  Independent Factory forced-cold `pass / accepted` receipts for the
  **992-byte PostEclMovement**, **892-byte CompareOperands**, and the
  special `/Ob0` 20-byte `ZunTimer::operator>=` are archived in
  `RE_ACCEPTED_RECEIPTS.md` and tied to clean commit `61dfa79`. They
  reconfirm existing exact owners, not the unfinished RunEcl boss.

- **RunEcl authentic boss-marker interrupt and SoundPlayer owners (October 18):**
  Reopened **original** code at four RunEcl opcode-124..157 callsites.
  The previous `Th09EclRunState::BossUiView::SetBossMarkerState` was **misnamed**:
  its actual callee `0x004067D0` is the canonical **32-byte
  `AsciiBossMarkerVmView::SetBossMarkerInterrupt(int,short)`**, NOT
  `AsciiManager::SetBossMarkerState @0x0040F890` (20-byte distinct owner).
  Updated RunEcl's two interrupt calls, 42-byte exact marker-position call
  `@0x004067F0` and positioned audio @`0x0043E380` to their genuine shared
  C++ type/receiver/symbol; the exact post-ECL movement helper now also uses
  genuine `g_SoundPlayer` and `PlaySoundByIdx @0x0043E2F0`, not the old
  invented `EnemyEclSoundPlayerView` global. **Before/after full source-cold
  15,564-byte RunEcl raw function is byte-for-byte identical; all 598
  original-bound fields and 193 switch-table destinations agree**, leaving
  exactly the original **30** ordinary differences (opcode 4 seven,
  155 twenty-three), 146/148 handler shapes. Its **992-byte physical**
  `EnemyPostEclUpdateMovement` and **892-byte physical** alternate-carrier
  `CompareOperands` replay canonically exact; all source-identity-only
  symbols (two genuine SoundPlayer C++ names and **20 private-label spellings** across these two exact carriers)
  were updated only after confirming independently retail original VAs.
  The strict original callsite audit now requires eight genuine call/global
  source symbols, not old misleading aliases. Focused `--owner runecl`
  protects **30 existing full exact bodies**, including four genuine ASCII/
  audio callees from two additional pinned cold carriers. This fixes a real
  source-referent/ABI naming mistake and native linker split; **no new exact
  function** (940/980) and no completed product/native link/runtime claim.
  Independent Factory `strict-live-v1` forced-cold, source-commit-bound
  `pass / accepted` receipts on **clean `5808950`** reconfirm both the
  **992-byte physical exact Post-ECL Movement** and **892-byte physical
  exact CompareOperands**, not the nonexact RunEcl owner; see
  `RE_ACCEPTED_RECEIPTS.md`. The full `--owner all` run protected **35
  complete-owner diagnostics and 192 distinct exact full-function units**
  with no duplicate acceptance credit. CI **539 tests, 28 skipped, zero
  failures**; native product still open.

- **Front message runtime exact callee shared across two large lifecycle callers (October 18):**
  The independently canonical exact **133-byte / nine-field**
  `FrontMessageOwnerView::InitializeMessageRuntime @0x004181E0` previously
  had three different C++ caller/declaration type identities. Its real
  type and compile-time-checked `+0xE944 /+0x1095C /+0x10960 /+0x10964 /
  +0x11EA8` member offsets now reside in `src/FrontMessageOwnerView.hpp`.
  `GameManagerSetupLayout::CleanupGameplayState` (425-byte original /
  **410-byte nonexact candidate**) and `FrontAddedCallback` (706-byte
  nonexact candidate) both use the same true `FrontMessageOwnerView`
  thiscall, without inventing the retail C++ source name. Separate
  **before/after source-cold VC7.1 compile** confirms identical full raw
  machine bytes and unchanged COFF field positions/kinds/addends for
  Cleanup **410/410, 42 fields**, FrontAdded **706/706, 37 fields**,
  their independent exact 1,230-byte `GameManager OnUpdate` and exact
  **133-byte FrontMessage InitializeRuntime**. Only one genuine decorated
  external callee symbol changed in each nonexact caller, at Cleanup
  `+0x1C` and FrontAdded `+0x2B8`, both bound to target @0x004181E0.
  The Cleanup full-original inspector and its independent auxiliary caller
  diagnostic now accept that *one* authentic C++ method identity with
  unchanged target VA, not a convenient fake alias. Focused
  `--owner gamemgr` additionally cold-replays the already exact 133-byte
  callee (**13** protected exact owners, formerly 12); `--owner all`
  reuses the preceding Front-family cold replay without double credit.
  Full Background/CardAttack/FrontSide headers have an independently real
  `AnmVm` class declaration conflict with the separate AsciiManager TU;
  unsafe header replacement was **rejected** rather than hidden.
  Both large nonexact caller functions remain nonexact; **940/980** unchanged.
  Factory `pass / accepted` forced-cold receipts at clean source commit
  `76d283d` independently reconfirm the already exact **133-byte FrontMessage
  initializer** and **1,230-byte GameManager OnUpdate**; see
  `RE_ACCEPTED_RECEIPTS.md`. Neither nonexact caller is promoted.

- **EnemyManager OnUpdate real ANM/sound/ASCII owner and full callee protection (October 18):**
  Maintained **3,900-byte** nonexact `EnemyManager::OnUpdate @0x00410730`
  now refers to genuine `g_AnmManager`/`ExecuteScript`,
  `g_SoundPlayer`/`PlaySoundPositionedByIdx` and real
  `g_AsciiManager`/`SetBossMarkerState`; its separately exact 42-byte
  `AsciiBossMarkerVmView::SetBossMarkerPosition` has a common declaration
  with the genuine 32-byte interrupt sibling. Exact pre/post VC7.1 builds
  have **byte-identical 3,900-byte raw owners and all 97 unshifted relocation
  offsets/kinds/addends**; only external C++ symbol identities and verified
  section-local label names change. The boss still has **29** real differing
  original bytes, **69** ordered calls, **170** original control-flow blocks
  and the identical four-entry switch table. These repairs are native
  source-reference progress, NOT new byte-exact credit.
  `--owner enemy` now builds all **four distinct genuine external callee
  profiles**, including ANM ExecuteScript's separate pinned `/Ob0`
  9,358-logical/9,776-physical owner and the once-compiled shared ASCII
  marker carrier. It checks **12/12 already exact full-function owners,
  12,506 physical bytes and 437 relocation fields** (seven Enemy-family
  siblings plus five authentic ANM/sound/ASCII callees). Target-independent
  tests reject missing/swapped methods, addresses, profiles, source and
  carrier splits. Factory `strict-live-v1` `pass / accepted` source-commit-
  bound replays on `6e1f3b4` for the existing 32- and 42-byte ASCII marker
  siblings are archived in `RE_ACCEPTED_RECEIPTS.md`; they do not accept
  the Enemy boss itself. Full `--owner all` checks **35 original-bound
  noncrediting diagnostics and 191 distinct exact owners**. Overall
  **940/980** canonical exact; native linking/resources/runtime remain open.

- **RunEcl opcode-4 original-machine timer/redispatch safety (October 18):**
  The previously unfinished source-cold `scripts/audit-ecl-timer-jump.py` is
  now retained and executed by `--owner runecl` / `--owner all`, alongside
  the existing opcode-155 masked-XOR behavior gate. From the verified
  **15,564-byte /598-field** real PE/VC7.1 owner it separately decodes the
  **29-byte / eight-instruction opcode-4** sequence, tests all six raw signed
  and unsigned time operands x five positive/negative relative offsets x three
  context addresses (**90 original/candidate transitions**) and verifies the
  actual `ZunTimer::operator=(int) @0x00401500` thiscall's
  `activeContext+0x08` receiver, unmodified argument, one call, sole
  original ECL pointer advance and direct redispatch @`0x00408750`.
  Unknown instructions, memory/width changes, wrong call/branch/offset
  and false exactness are rejected by eleven pure-input tests. The retained
  source still emits the same **seven opcode-4 different bytes** (the
  `MOV ECX; PUSH ECX; MOV ECX` vs `MOV EAX; MOV ECX; PUSH EAX` register
  allocation), while opcode 155 still differs by 23 ordinary bytes.
  **146/148** handlers and all **24 same-source exact ECL/ExAttack
  siblings**, plus the genuine **9-byte ZunTimer::operator=(int)** and
  **25-byte ZunTimer::SetCurrent** exact callee bodies (**26 strict
  focused replays**) still pass; neither semantic gate grants codegen
  exactness. The full source-cold `--owner all` independently checks
  **35 diagnostics and 191/191 distinct complete exact functions**.
  Current target-independent CI passes **527 tests (28 skipped, zero
  failures)**; full Windows product/runtime remains open. Old 979/876/103 counts
  remain superseded by **980/940/40**. The original target and native-product
  state are unchanged; focused codegen proof and CI stay mandatory.

- **Title actual-added callback's genuine ASCII global/callee identity (October 18):**
  `TitleScreen.cpp` previously declared the physical `g_AsciiManager`
  `@0x004CE458` as a separate `TitleAsciiManagerView`, producing a linker
  symbol different from the canonical `AsciiManager` global used by other
  exact functions. The same exact `TitleScreenView::ActualAddedCallback`
  `@0x00424BF7` called the original **exact** `AsciiManager::Reset
  @0x004343E0` under a fabricated `TitleAsciiManagerView::Reset` method name.
  Both now use the genuine shared `AsciiManager` type/method, whose target
  address and ABI were independently proven. The next original **one-byte
  folded** call `@0x0043D2B0` still has `origin unknown/review` and remains
  behind a separate *declaration-only* adapter; no `AsciiManager::InitializeVms`
  origin is invented. Source-cold `ActualAddedCallback` is still **335/335
  full original bytes, 27 authentic fields**, with only the two reviewed
  assembler/COFF external names changed. The genuine `OnUpdateStartMenu`
  1,560/1,592 physical owner changes only **nine** compiler-private `$L`
  names, individually verified to the *same* original VAs/field offsets;
  all **41 Title exact siblings** still replay. The separate 1,939-byte
  DrawResult boss keeps its own **22** original-bound residual bytes.
  `--owner title`; this is a native source-reference repair, **not**
  newly exact codegen or a closed native product. 980/940/40 unchanged.

- **Title score-record producer/renderer unification (October 18):**
  `TitleScreen.cpp`'s exact `TitleScoreRecordView::InsertIntoTable` and the
  still-open **1,939-byte `DrawResult` boss** now share the *same* typed
  0x2C-byte record, singleton and 16×5×5 global table declarations in
  `src/TitleScoreRecordView.hpp`. The real target/VC7.1 C++ table symbol
  `?g_TitleScoreTable@@3PAY144UTitleScoreRecordView@@A` binds both consumers
  to **0x004A8380**, instead of the old renderer-only
  `ResultDrawScoreRecord` type with a *different* mangled linker symbol.
  Original TH09 independently fixes score `+0x0C`, category/rank, name
  `+0x18` and signed digit `+0x2B`; no data definition, allocation or native
  link/runtime acceptance is invented. On fresh source-cold compilation,
  `DrawResult` preserves **1,939/1,939 physical, 82 original address fields,
  16 ordered calls, 92 direct blocks and the identical 22 non-exact offsets**.
  Including the shared declaration shifts only nine compiler-private `$L`
  names in exact `OnUpdateStartMenu`. Each new COFF local-label section offset
  independently yields the SAME original VA at all nine immutable manifest
  field offsets; only the private symbol spelling was updated. The full
  **1,592-byte physical StartMenu remains exact** with 107 relocations,
  and **all 41 Title exact siblings** cold-replay. Independent Factory
  forced-cold `pass / accepted` replays on clean source commit `6b47d06`
  reconfirm the already-exact 216-byte `InsertIntoTable`, 1,592-byte
  physical `OnUpdateStartMenu`, and 534-byte `TitleSetupThread`;
  immutable source-bound receipts are in `RE_ACCEPTED_RECEIPTS.md`.
  This is a genuine source-owner/linker identity correction, NOT a newly
  exact function: 980/940/40.

- **RunEcl opcode-155 original-machine semantic guard (October 18):** A new
  source-cold `scripts/audit-ecl-timeout-bit.py`, invoked by the full
  `--owner runecl` / `--owner all` frontier, independently links the current
  real VC7.1 PE/COFF candidate to TH09's SHA-verified 15,564-byte original.
  It decodes the 45-byte opcode-155 handler at `0x0040B770`, interprets the
  eight real flag-update instructions on both sides, and checks **all 256
  wire-byte values across 12 whole-word flag patterns (3,072 transitions)**.
  Both implementations update **only bit 24** and agree for every checked
  case. Unknown instructions, operand widths and extra or displaced memory
  writes are rejected, with eleven pure-input semantic and orchestration tests. This is
  **semantic protection, NOT codegen exactness**: the full RunEcl remains
  **15,564/15,564 physical bytes, 598 bound fields, 30 unequal ordinary
  bytes**, divided as opcode 4 = 7 and 155 = 23; 24 same-carrier/alternate-
  profile exact siblings remain independently cold-protected. Additional
  source-cold variants in `.analysis/gpt-web-20261018/` (ignored diagnostic
  artifacts) were rejected when whole-owner relocations, byte positions or
  handler extents regressed rather than trusting local instruction similarity.
- **Current-source Factory regressions of three established exact big owners
  (October 18):** On the clean source commit `83a94cfb204a17518d4efd9db7f8ec8d2e87a4fe`,
  fresh Factory `strict-live-v1` *forced-cold*, original-bound receipts
  returned **`pass / accepted`** for Etama `OnUpdate` **2,216** physical
  bytes, `TitleSetupThread` **534** bytes and MusicRoom `OnUpdate` **2,258**
  bytes. Exact receipt IDs and their immutable pre-audit-change source binding
  are archived in `docs/RE_ACCEPTED_RECEIPTS.md`. No new exact match unit is
  claimed: the canonical ledger remains **940 / 980**, with **40** open.

- **Original-PE-attested ExAttack special members (October 18):** The
  `ExAttackRecord` and `ExAttackController` constructors are now checked by
  a fresh, *non-crediting*, full-owner diagnostic on **every** `--owner
  exattack` and `--owner all`. The original **15/15-byte record constructor**
  binds its real `ZunTimer::ZunTimer @0x00403390` REL32; the independent
  **28/28-byte controller constructor** under `/O2 /Os` binds the record
  constructor's real `0x004157A0` DIR32 and the original VC7 EH vector
  constructor at `0x00401470` REL32, including the exact **257 × 0x4C**
  array arguments. The functions' original SHA hashes, sizes, instruction
  decodings, every COFF field, and the *separate* `/Ob1` versus `/Os` real
  compiler profiles all agree. The original target does **not** reveal
  whether these empty special members were explicitly authored or implicitly
  compiler-generated, so **both remain `origin unknown/review`, exactness
  credit `none`**. No fake authored exact ledger rows or source-profile
  rewrites. `--owner exattack` also cold-protects the **four original-exact
  ExAttackController.cpp functions** sharing the normal `/Ob1` carrier, with
  codegen and origin fail-closed tests. **Spawn is separately already exact
  (321/321)**; only the two special-member origin classifications and the
  unrelated ECL/Etama/Title/ExAttack big-boss residuals remain open.
- **ExAttack controller full native-carrier and special-member check (October 18):**
  Source `ExAttackController.cpp` shares an authentic VC7.1 `/O2 /Ob1 /EHsc`
  optimization context among **four already exact** functions (`ReleaseExAttackRecords`,
  `Release`, `Register` and `Spawn`). `--owner exattack` now cold-compiles
  and independently strict-original PE/COFF replays **all four**, rather than
  omitting this lifecycle family when checking the hard Type19/21 big callbacks.
  Adjacent constructors `ExAttackRecord::ExAttackRecord @0x004157A0` and
  `ExAttackController::ExAttackController @0x004157B0` independently compile
  byte-for-byte to the entire original **15/15 (one field)** and **28/28
  (two fields)** physical extents under the true separately pinned `/Ob1`
  and `/Os` VC7.1 profiles. The controller's 28-byte constructor uses the
  actual 257-record vector-construction runtime target `@0x00401470`;
  the record's true Timer constructor is `@0x00403390`. The SHA-pinned new
  `inspect-exattack-constructors.py` checks **every original physical byte,
  all three independent call/address operands and complete decoded RETs**,
  but both constructor origins remain explicitly `unknown/review` because
  the retail binary cannot distinguish explicit empty source definitions
  from compiler-generated special members. Neither is counted as new exact;
  the authoritative `config/functions.csv` and exact ledger remain unchanged.
  The fresh full `--owner all` verifies **33 independent full owners**
  (including these two noncrediting provenance checks) plus **184/184
  DISTINCT already exact full-source functions**, up from 31 and 180.
  `scripts/ci.py` passes **489 target-independent tests (28 skipped,
  zero failures)**, plus tracking, documentation, build graph, generated
  progress, immutable target PE identity and whitespace checks.
  Four existing exact ExAttackController functions independently returned
  Factory `pass / accepted` on these source commits: `ReleaseExAttackRecords`
  at `704e6fe`; `Register`, `Spawn` (321 bytes) and `Release` at the
  documentation-corrected `7528a7e`. Their distinct source bindings and
  immutable receipts are archived in `docs/RE_ACCEPTED_RECEIPTS.md`. The
  two unknown-origin constructors still have **no accepted authored exact**.
- **Current Replay big-function machine-code improvement (October 18):**
  Full SHA-bound original Japanese `ReplayManagerView::BeginPlaybackStage
  @0x00420840` is **540 physical bytes**; pinned VC7.1 candidate remains
  **538 bytes**, but source now explicitly performs its FIRST original
  `g_ReplaySideState0` pointer read **after publishing header 1E7 and
  before publishing header 1E8**. Recompilation recovers the target's
  independently decoded order of **all 32/32 call/data fields**, up from
  31/32; the **first 30 real PE/COFF field offsets** now match literally,
  with only the final RNG field/call shifted -1/-2 bytes by the remaining
  two-byte code-length gap. Complete overlapping original-byte differences
  fell from **49 to 39**, protecting **499/538** already matching physical
  positions through an explicit 39-offset fail-closed residual guard;
  original/compiled calls remain **5/5**, blocks **12/12**, no partial
  mask/exact credit. The nine remaining header-window differences are solely
  the target EDX/ESI register allocation, with 30 in the seed/tail. A late
  scoped volatile pointer *read* preserves the original source-visible
  distinct owner load; remaining subsequent global SideState accesses are
  intentionally NOT cached. Twenty-four isolated header load/store variants
  and earlier SideState variants were checked; most are byte-neutral or
  change the original read multiplicity (32 to 29 fields), so were rejected.
  This is **a net ten-byte full-owner improvement**, not a newly exact
  function. The complete **12 exact ReplayInput siblings**, both real EH/noEH
  compiler profiles and separate GameManager callee guards remain mandatory.
  Fresh source-cold `--owner all` passes **31 complete boss/leaf original
  diagnostics and 180/180 unique exact sibling comparisons**; CI passes
  **482 tests (28 skipped, 0 failed)**, and immutable target, ledger,
  documentation, progress, build-graph and whitespace checks all pass.
- **Replay large-boss full exact-source profile guard (October 18):**
  Original `ReplayManagerView::BeginPlaybackStage @0x00420840` remains an
  original-PE-bound **540-byte** owner against **538-byte** source, 32 linked
  operands, five ordered calls, twelve direct blocks, **39** unlike overlapping
  bytes (down from 49) and two missing source bytes; it is **NON-EXACT**. Its native
  `ReplayInput.cpp` source contains **12** already exact functions in TWO
  *different* original VC7.1 profiles: **two in the no-`/EHsc`
  `ReplayInputNoEh.obj`** (BeginRecordingStage, Create) and **ten in the
  standard `/EHsc` `ReplayInput.obj`** (Update, Release, frame sync,
  record/playback input and related members). `--owner replay` now guards
  **all 12** complete exact siblings after a fresh cold compile of both
  pinned carrier/profile combinations, up from two. The full `--owner all`
  runs Replay before the still-open GameManager gameplay setup and therefore
  does **not** repeat the already-verified exact `ReplayManager::Create`
  **or** `ReplayManager::Release`; focused `--owner gamemgr` still
  individually rebuilds/replays those real callee ABIs. Fail-closed tests
  reject missing source units, compiler profile or output swaps, duplicated
  exact functions and stale/false prebuilt carriers. The source/target
  canonical function ledger remains **unchanged**; the subsequent scoped
  first-side-state-load correction restores all 32 linked original field
  identities/order and narrows the open boss from 49 to 39 bytes without
  claiming exactness. This is full-owner regression protection and genuine
  bounded codegen progress, not a new exact closure. Fresh cold
  `--owner all` passes **31 full physical boss/leaf diagnostics and 180/180
  UNIQUE existing exact member checks** (up from 171), with both Replay
  callee identities counted just once. `scripts/ci.py` passes **481 tests
  (28 skipped, zero failures)**, along with authentic PE SHA, tracking,
  documentation, progress, build graph and source whitespace checks.
- **Current EffectManager original-codegen guard (October 18):** The still-
  nonexact **51-byte** `EffectManager::AddedCallback @0x0040D1C0` is embedded
  in the SAME pinned `/O2 /Ob1` original VC7.1 compilation carrier as
  **15 separately exact `EffectManager.cpp` functions**. Previously
  `--owner effect` rebuilt that real carrier but rechecked only three
  exact update/draw methods. The original-bound owner now checks the entire
  **15/15** complete PE/COFF codegen family, including pool lifecycle,
  spawning, update, initialization and drawing; the 51-byte open callback
  remains **4 authentic fields, 3 blocks, 2 calls, 8 unlike original bytes**.
  Missing exact units, duplicate symbols, mixed compiler profiles, changed
  carrier and skipped comparisons fail closed. More than thirty complete
  ExAttack Type19/21 first-vertex/iterator source-shape experiments and
  seventeen Title cursor stack-home variants were independently compiled
  this session against the original; they did not improve the authentic
  **6/9** ExAttack or **38-byte** cursor frontiers and were **not** adopted.
  A fresh source-cold `--owner all` sweep passes **31 whole owner
  diagnostics** and **171/171 distinct exact functions**, up from 159;
  `scripts/ci.py` passes **477 tests, 28 skipped, zero failures** along
  with target SHA, ledger, docs, open build graph, progress and whitespace.
  Independent Factory strict-live forced-cold `pass / accepted` regressions
  on gameplay-source **`7d3f89b`** separately reconfirm `InitializeEffect`
  (407 bytes), `Create` (327 bytes), `SpawnEffectWithVelocity` (175 bytes)
  with exact physical bytes and 17/27/1 real field identities respectively.
  Their immutable source-bound receipts are in `RE_ACCEPTED_RECEIPTS.md`.
  This is original-byte regression protection, NOT newly accepted exactness.
- **Git state:** TH09 `main`; derive the actual HEAD and dirty work from
  `git status --short --branch` and `git log` at restart. Complete,
  review, and commit **every tracked/untracked change**; do not reset or
  sidestep it. New checkpoints use `gpt-web: ...`; local commits are
  **not** pushed automatically.
- **Original target:** Japanese TH09 **v1.50a** `resources/th09.exe`,
  immutable SHA-256
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  It is ignored, private, read-only, and never a cleanup candidate.
  Factory's registered IDA source is `th09-ida`; mapping metadata is not
  canonical codegen acceptance.
- **Newly canonical exact:** `FileSystem::TryDecryptFromTable @ 0x0042C290` cold-replays **220/220 entire code bytes and all 12 original-bound COFF fields**. Source-cold VC7.1 recovers the original row-offset/key-bias schedule without changed call/branch/return behavior. `--owner crypto` strictly rebuilds this exact selector as well as independently exact Decrypt/Encrypt siblings. Historical 38-byte nonexact experiments are superseded. Independent Factory `strict-live-v1` forced-cold `pass / accepted` receipt `e5fbed2e35363191833752be9a9daef1968ab0ec01a83b06088188cad80ea5a9` binds exactness to gameplay-source commit `9590206`; it does not prove whole-program link/runtime closure.
- **Latest code checkpoints (pre-handoff HEAD `944d95c`):**
  `e96bc16` narrows **ANM `ProjectCameraFacingQuad` from 13 to six**
  independently linked ordinary byte differences, with all 40 fields, 11
  calls and four exact siblings unchanged; `3223caf` restores the target's
  **two observable GameManager `limitB4` publications** (function remains
  nonexact); `ea5242d` pins 172-versus-171 `GameplaySetupThread` source/PE
  address-referent identities; `944d95c` protects **489 already exact bytes**
  within the still-nonexact 540/538-byte `Replay BeginPlaybackStage`.
  Earlier CalcShakeEnvelope (28→26) and 0x1EC Replay record corrections are
  retained. `RunEcl` remains **15,564 physical bytes / 598 authentic operand
  fields / 30 ordinary differences**, no table/field drift. None of these
  nonexact-owner improvements adds canonical exact function credit.
- **Cold regression scope:** the live full `--owner all` suite now verifies
  **30 open non-crediting boss/leaf original-bound functions plus one
  separately canonical-exact 220-byte crypt selector diagnostic and 146
  strict already-exact sibling comparisons**, all passing at the current
  checkpoint. The new, separately SHA-bound **959-byte ExAttack Type14/22
  Draw** original owner is checked on every `--owner exattack` and full replay;
  it emits 974 source bytes and remains NON-EXACT. The suite also protects
  the 15,564-byte RunEcl, 600-byte Etama OnDraw, 3,900-byte Enemy OnUpdate,
  1,280-byte ReplayMenu and 1,939-byte DrawResult owners. These diagnostics
  are not source-commit-bound accepted Factory receipts.
- **Pre-cleanup verification (2026-10-17):** a **fresh cold** original-PE/COFF
  `--owner all` sweep passed **31 owner reports (30 noncrediting plus one
  already-exact crypto selector diagnostic)** and **72/72 separate strict
  exact sibling units**. `scripts/ci.py`: **437 tests, 28 skipped, success**;
  target SHA, ledger, progress and docs passed. Evidence is retained under
  `.analysis/handoff-cleanup-20261017/`; this is not a new Factory
  source-commit-bound exactness receipt.
- **Latest live source-cold checkpoint (October 17):** The common
  `BulletSideStateView` now declares the target-backed `EffectManager *` at
  **+0x0C**. `EtamaController::OnDraw` calls the actual exact
  `EffectManager::DrawBulletLayerEffects` through that shared owner, replacing
  a duplicate private view without changing its physical **600/600**, 27
  address operands, 13 real calls, 17 blocks or **268 nonexact bytes**.
  The affected **`EtamaController::OnUpdate` 2,216/2,216**,
  `DrawSingleBullet` **232/232** and `EnemyPostEclUpdateMovement` **992/992**
  original physical bytes remain exact. VC7.1's source-label numbering changed
  across Etama/ECL carrier functions; the refreshed exact manifests were
  checked against original label destinations before a full replay. The live
  full `--owner all` now cold-checks **31 owner diagnostics** and **133
  independently exact sibling comparisons**, including every exact match in
  the 24-unit ECL and 38-unit TitleScreen source families, plus the separate
  MusicRoom and DrawCallback checks. These are noncrediting regressions, not
  a new canonical exact function. The 980 / 940 / 40 scoreboard is unchanged.
  The committed checkpoint passed `scripts/ci.py` with **446 tests, 28 skipped**,
  plus target SHA, tracking, documentation, progress and whitespace checks.
- **Latest big-owner source improvement (October 17):**
  `ProjectCameraFacingQuad @0x00439820` now cold-compiles **622/622**
  bytes with **only four full-owner original-linked ordinary differences**,
  down from six. The natural unsigned-word copy maintains the actual
  three-word data flow and all **40 real target fields, 11 calls, eight
  blocks, 201 instructions**. The four residual offsets are
  `+0xF6,+0xF7,+0xF9,+0xFA`, reflecting only the final Y/Z store order;
  the previous six-offset copy model is now superseded. The focused original
  replay protects **all four already exact ANM sibling functions**, with
  a new four-offset fail-closed frontier; exactness of the big owner remains
  unclaimed.
- **Enemy large-owner genuine Etama dispatch (October 17):** The
  **3,900-byte** `EnemyManager::OnUpdate @0x00410730` now calls the
  separately original-exact **203-byte** `EtamaController::SpawnBulletPatternPrimary`
  through the *real* `EtamaController` and `BulletSpawnDescriptor` interface,
  removing `EnemyCoreEtamaControllerView`'s speculative callback type.
  The source retains the independently exact **28-byte** descriptor-constructor
  call and target-checked **0x214-byte layout**, with C++ offset assertions
  for the consumed physical fields. The source-cold whole-owner audit remains
  **3900/3900, 97 bound fields, 69 calls, 170 matching direct blocks and
  29 ordinary unlike bytes**, and its exact same-TU owners are unchanged.
  `--owner enemy` now adds both genuine callee cold replays to the five
  already-exact protections, increasing the complete `--owner all` sibling
  comparison scope from **131 to 133**. The current fresh-cold `--owner all`
  sweep passes **31 physical owner reports and 133/133 original-bound exact
  siblings**; `scripts/ci.py` passes **448 tests, 28 skipped** plus target,
  tracking, docs and progress gates. No new exact credit is claimed.
- **Current complete native-carrier exact protection (October 17):** The
  still-open **622-byte ANM `ProjectCameraFacingQuad`** compiles alongside
  **15 exact** `AnmManagerRender.cpp` functions (including `Draw2D`,
  `TranslateRotation`, flush/set-state helpers and both drawing paths). All
  **15/15** are now independently original-PE/COFF replayed from the freshly
  built **same** real VC7.1 Render object, rather than checking just three
  nearby members. The separately compiled **595-byte exact `Project3DQuad`**
  is also replayed (ANM total **16**). The still-open **600-byte Etama
  `OnDraw`** now strictly protects both exact same-carrier functions,
  `DrawSingleBullet` **232/232** and `EtamaController::AddedCallback`, plus
  separately built **2,216-byte exact `OnUpdate`** (Etama total **3**).
  Inventory cardinalities, compile profiles and output carrier identities
  fail closed, including synthetic missing-member regression cases. The
  freshly source-cold `--owner all` passes **31 full original-bound owner
  diagnostics** and **146/146 strict existing exact unit checks**, up from
  **133** at the preceding checkpoint. `scripts/ci.py` passes **452 tests
  (28 skipped)** with target, tracking, documentation, build-graph and
  progress checks. No source function is newly exact;
  canonical progress remains **980 source-present / 940 exact / 40 open**.
- **ECL helper full-byte no-regression guard (October 17):** Reopened
  the independently SHA-attested retail `MoveRandomBiased @0x00407E30`
  in the same freshly compiled 15,564-byte `RunEcl` translation unit.
  The helper remains **516 original / 517 candidate physical bytes**;
  all **28 real COFF fields, 14 ordered calls, 24 CFG blocks and 145
  instructions** agree in identity/count, but **148 overlapping bytes**
  differ (and one source byte is extra). Full original-linked comparison now
  pins all 14 exact residual runs and rejects any new wrong byte among the
  already matching **368 original positions**. A second guard checks the
  only two differing normalized instructions: the repeated post-`Float3`
  Y-position comparisons. Independent source-cold 20-form Y-alias probes
  either retain 517 bytes or regress to **518/526 bytes**; no source edit
  masquerades as an exact solution. `--owner runecl` still independently
  protects all 24 exact ECL siblings; `RunEcl` itself remains 30 bytes open.
  The fresh full `--owner all` passes **31 diagnostics and 146/146 exact
  siblings**; `scripts/ci.py` passes **458 tests (28 skipped)** with tracking,
  docs, build-graph, progress and whitespace validation.
- **ReplayManager true owner shared across three source families (October 17):**
  The still-open **1,689-byte** `GameplaySetupThread @0x0041AF2D`
  now calls the *genuine* independently exact **318-byte**
  `ReplayManagerView::Release @0x00420AD0`, not the speculative
  `ReplayManagerSetupReleaseView` method alias. A common reviewed
  `ReplayManagerRuntimeView.hpp` holds the real **0x164-byte** ReplayManager
  layout and 0x18-byte ReplayBufferLink, with compile-time field offsets,
  for `ReplayInput.cpp`, `GameManagerSetup.cpp` and `TitleScreen.cpp`.
  The separate **42-byte** `ReleaseStageObject @0x00420C10` remains a distinct
  teardown contract. The original-linked GameplaySetupThread retains
  **1,689/1,689 physical bytes, 171/172 source/original fields, 35 ordered
  calls, 77/78 source/original blocks and 911 unequal bytes**; this is a
  real call/type correctness gain, **not** a newly exact boss. Independently
  cold compiled all **60/60** existing exact source functions across their
  **six real compiler carrier/profile groups** without a byte/field failure:
  12 ReplayInput, 10 GameManagerSetup and 38 TitleScreen. Nine compiler-local
  `OnUpdateStartMenu` `$L` labels changed only in spelling and were checked
  individually against their unchanged real original target addresses before
  refreshing their manifest names. Focused `--owner gamemgr` now additionally
  protects the true 318-byte Release and 361-byte Create exact callees;
  full `--owner all` de-duplicates Create because the replay owner already
  checked that no-EH source TU. The complete fresh `--owner all` passes
  **31 original-bound diagnostics and 147/147 DISTINCT strict exact sibling
  reports** (no duplicate Create credit); full `scripts/ci.py` passes
  **465 tests, 28 skipped**, plus tracking, docs, build graph, progress and
  whitespace validation. The canonical scoreboard remains
  **980 source-present / 940 exact / 40 nonexact**.
- **Latest whole-carrier ScreenEffect guard (October 17):** Independently
  re-opened the **336-byte `ScreenEffect::CalcShakeEnvelope`** and its real
  source compiler profiles, rather than trusting the old six-function
  dependency sample. Its maintained body still has **336 physical bytes,
  132 instructions, 20 authentic bound fields, 8 calls, 26 matched direct
  blocks** and **26 genuine different original bytes**; no exact credit.
  The exact-source `ScreenEffect.cpp` manifest contains **18** functions:
  **16** ordinary `/O2 /Ob1 /EHsc`, **one `RegisterChain` without `/EHsc`**
  and **one `DrawSquare` with `/Ob0`**. `--owner screen` now cold-compiles
  all **three** physical carriers, strict original-PE/COFF replays **18/18**
  existing exact bodies (previously 6), and rejects any missing member,
  changed compiler profile, changed object, duplicated function or false
  reused build. Complete `--owner all` now passes **31 owner diagnostics
  and 159/159 distinct original-byte exact sibling reports**. CI
  passes **469 tests (28 skipped, zero failures)**; target, tracking,
  documentation, progress and source whitespace gates pass. Separately
  cold-compiled nineteen legal ScreenEffect owner/timer/reference-source
  variants; neutral forms remain at 26 differences, early timer aliases
  degrade to 337 bytes /317 mismatches. The actual target is unchanged.
- **Independent Factory acceptance, ScreenEffect three source profiles:**
  Original-bound strict-live forced-cold jobs bound to exact gameplay-source
  commit **`cbfae2185e4ab8bacbc8cf95b1b5bc77c80f5c5f`** each return
  **`receipt_verdict=pass`, `acceptance_decision=accepted`**:
  standard `/O2 /Ob1 /EHsc` exact `ScreenEffect::CalcShake @0x00422660`
  (316 bytes; `receipt:4a636fe2e0141d41075ae7b69ced9123ba98a66aae7bff3002bb0f8f6fff4d83`),
  separately compiled no-EH exact `ScreenEffect::RegisterChain @0x00422D20`
  (447 authored/480 physical bytes; `receipt:ec759b50d81d19c2095b023d53e46cc94bb4bbe42aee92e8bcccdb958e2ea8dd`),
  and `/Ob0` exact `ScreenEffect::DrawSquare @0x00422960`
  (527 bytes; `receipt:a076aa0b462bca701b2b8f33e61081f9cdd5e0d36b8374f01382f2656f673379`).
  These confirm **three already exact functions under the new code commit**,
  not a previously open `CalcShakeEnvelope` match. Native whole-product
  link/gameplay, semantic and port stages are still open.
- **Phase:** original exact-reconstruction only. The 95% reviewed-authored
  byte milestone is open; whole Windows i386 build/link/resources/runtime,
  dual-Oracle semantic reconstruction and portability are still open.


## October 21: Effect literal and operand proof (962d501)

The operator requests continued exact reconstruction across **large functions
and small leaves**, serial low-priority compiles, focused affected replays and
regular cleanup. This checkpoint starts from pushed `0611108` and closes an
actual **Effect COFF literal/operand verification gap**. No maintained game
source, compiler profile, unit definition, original target or exact ledger row
changes. RunEcl/Enemy/Title DrawResult/Type3 remain **30 /29 /22 /15**; Effect34
Draw stays **959/946 bytes, 593 overlap differences +13 extra**. The 95%
milestone and product/runtime gates remain open.

- A private counterexample changes only the actual COFF definition of pi from
  `0x40490FDB` to `0x3F800000`; raw draw code and all 53 relocation records stay
  identical. The previous inspector and production frontier accept that object.
  The corrected inspector rejects it before using the source constant.
- All **four actual scalar definitions** now must match their symbol names and
  original PE values, with real section bounds/provenance and zero addends.
  All **53 candidate instruction address fields** must independently match the
  actual COFF operands, including indexed global displacements. Existing
  Type26/28/25 original indexed-field proofs remain intact. The production
  Type34 gate requires both complete checks; stale reports cannot pass.
- Twelve private source models yield eleven successful full comparisons and
  one C++ rejection of a nonexistent Supervisor member assumption. Six valid
  models are byte-neutral; five position-helper/wide-alpha models regress to
  975 bytes and damage old matches. No trial source, type, profile or expanded
  residual mask is adopted. The original loop/cursor/alpha allocation remains
  unresolved; a smaller generic score is insufficient.
- Focused replay checks **12 existing exact owners /4,123 physical bytes**
  across **four real source/profile carriers**, plus **13 related Effect
  diagnostics**. Other **989 unchanged exact source/profile proofs** are reused.
  The same fresh ECL object preserves RunEcl30, Enemy29, Type19 44 and Type29's
  existing frontier. No full-cohort replay or new authored exact credit.
- CI passes **852 tests /28 skips**. Frozen-source replay repeats the normal
  **959/53/593** draw and independently demonstrates that the old inspector
  accepts a wrong literal definition while the new one rejects it. The existing
  **178-byte initializer** also stays canonical exact. An initial private TOML
  serialization failure is retained; the corrected recipe passes.
- After all producers terminate, **144** verified loose evidence members become
  a **1,014,593-byte ZIP**. Archived duplicates plus **388** source-bound compiler/
  bytecode caches reclaim **12,937,994 bytes (~12.34 MiB)**. All 489 game inputs,
  1,031 unchanged unit definitions, four canonical config hashes, 95 source/
  provider pins and archive identity pass after cleanup. Earlier unique research
  and unrelated canonical objects remain.
- Evidence and frozen recipe: `.analysis/gpt-61sol-20261021-effect34-loops/`.
  No Factory accepted receipt, new authored exact credit or whole native
  product/runtime validation is claimed.
- Use direct local IDA Pro MCP and Bash, serial `nice -n 10` compiles,
  English `gpt-6.1-sol: ...` commits, push and independent remote verification.


## October 21: Controller native global interfaces (7294483)

The operator requests continued exact reconstruction across **large functions
and small leaves**, serial low-priority compiles, focused affected replays and
regular cleanup. Starting from pushed `962d501`, this checkpoint unifies the
**Controller input native global interfaces** while preserving every candidate
byte. No new authored exact credit: RunEcl/Enemy/Title DrawResult/Type3 remain
**30 /29 /22 /15**; Controller remains **796/808 bytes, 102 overlapping
differences +12 missing**. The 95% milestone and product/runtime gates stay open.

- `ControllerInput.cpp` now uses the real shared `ReplayInputState` array and
  `Supervisor` declaration. Its two fictitious external object types disappear.
  The Supervisor device prefix remains an explicitly bounded view; configuration
  thresholds use the existing fields with signed reads, preserving original
  MOVSX semantics. Shared headers, compiler profiles and unit definitions do not
  change. Original class spelling/ownership is not established by these names.
- All **nine native global COFF fields** must have their actual shared symbol,
  DIR32 kind, reviewed field position/addend and undefined-symbol provenance.
  Old aliases fail even though their encoded destination bytes agree. Complete
  decoding still checks all **32 original fields**, 18 ordered helper calls and
  eight direct branches. All **694 already equal bytes**, including the first
  **674 complete instruction bytes**, remain equal.
- **30 serial private source controls** compile: 23 complete diagnostics and
  seven excess-field rejections. Shared-axis range snapshots reduce the generic
  score to 98, but damage the previously equal +0x2C4; none is adopted. Measured
  UINT/DWORD conversions and commuted midpoint expressions do not recover the
  missing 12 bytes. These negatives do not prove the remaining source impossible.
- Focused canonical replay passes both affected same-TU exact helpers:
  **37 +47 =84 complete bytes**. Other **999 unchanged exact source/profile
  proofs** are reused. Full raw code, linked code and all 32 field destinations
  remain identical to the starting source; only nine external symbol identities
  change. Object-local section ordinals shift after header inclusion, and actual
  helper definitions are separately replayed rather than treating those ordinals
  as global identities.
- CI passes **854 tests /28 skips**, alongside target-required tracking,
  documentation, generated progress and whitespace checks. Frozen replay from
  the verified archive repeats the maintained/old-alias/candidate controls.
- **74** evidence members /4,322,717 loose bytes become a **494,392-byte ZIP**.
  Removing only verified archived duplicates and **383** terminal compiler/
  bytecode caches reclaims **10,398,135 bytes (~9.92 MiB)**. All 489 game input
  hashes, unchanged 1,031 unit definitions and canonical ledger hashes are checked;
  original input, toolchain, IDA and older unique research remain untouched.
- Evidence, full trial sources/COFF inventories and the frozen-source replay:
  `.analysis/gpt-61sol-20261021-controller-axis/`. No Factory accepted receipt,
  whole native link/runtime closure or new authored exact credit is claimed.
- Use direct local IDA Pro MCP and Bash, serial `nice -n 10` compiles,
  English `gpt-6.1-sol: ...` commits, push and independent remote verification.


## October 21: Type3 query and pointer controls (d5e0bf6)

The operator requests continued exact reconstruction across **large functions
and small leaves**, serial low-priority compiles, focused affected replays and
regular cleanup. Starting from pushed `7294483`, this checkpoint tests the
**Type3 query-order interaction and newly implemented native pointer bodies**.
All maintained game source/headers, compiler profiles and unit definitions stay
unchanged. No new authored exact credit: RunEcl/Enemy/Title DrawResult/Type3
remain **30 /29 /22 /15**; the 95% and product/runtime gates remain open.

- Eight component controls separate the existing query forwarding helper, qualified
  middle-Z writes and qualified tail-X word reads. All retain 780 bytes /38
  original referents /24 calls /17 blocks; differences are respectively
  **15,27,15,27,67,19,52,4**. The original Z chronology appears only with both
  qualifications in this tested panel. That helper alone regresses to 27.
  The four-byte model still damages four previously equal bytes and remains
  rejected. No original volatile type, aliasing intent or source spelling follows.
- Eight genuine definition-visibility controls test the two existing pointer
  conversion bodies before/after baseline/rejected callers, and each actual
  inverse-X/Y method separately. Pointer visibility is byte-neutral: caller 15
  or 4; both aliases retain the same original three-byte body and zero fields.
  Original ownership/origin is unknown; two aliases do not earn six authored bytes.
  Partial coordinate visibility changes extent/field inventory to 770/36,
  765/35 or 775/37 while each genuine 46-byte callee stays original-equal.
- Public producers now expose five component cases and `--case pointer-views`.
  Default word-order scope and all three previous compiled source hashes stay
  unchanged. New component reports are isolated; invalid/stale results cannot
  replace independent evidence. Seven focused public compiles reproduce all
  captured raw bytes and actual COFF rows, plus both three-byte aliases.
- Only diagnostic scripts/tests and evidence notes change. All **1,001 existing
  canonical exact source/profile proofs** are reused because no game input or
  canonical unit changes. Joint callee-body comparisons are separate from a
  canonical source-carrier replay and from new authored exact credit.
- CI passes **858 tests /28 skips**; target-required tracking, documentation,
  progress and whitespace checks pass. Five frozen source controls reproduce
  complete raw code/COFF fields, including both alias bodies and the rejected
  partial-Y context. A post-archive baseline reads only frozen archive inputs.
- **64** evidence members /2,636,841 loose bytes become a **395,124-byte ZIP**.
  Removing verified duplicates and **380** terminal CI bytecode caches reclaims
  **8,691,375 bytes (~8.29 MiB)**. All 489 game inputs, unchanged 1,031 unit
  definitions, 28 frozen source/provider inputs and four canonical input hashes
  are checked after cleanup; original input, tools and older unique research stay.
- Evidence, full sources/COFF records and frozen recipes:
  `.analysis/gpt-61sol-20261021-type3-query-factorial/`.
- Use direct local IDA Pro MCP and Bash, serial `nice -n 10` compiles,
  English `gpt-6.1-sol: ...` commits, push and independent remote verification.

## Enemy tracking checkpoint at a44c9ef (October 21)

## Current verified handoff checkpoint

The operator requests continued exact reconstruction across **large functions
and small leaves**, serial low-priority compiles, focused affected replays and
regular cleanup. Starting from pushed `d5e0bf6`, this checkpoint restores
**sixteen Enemy tracking bytes: 29 ->13 differences**, without damaging any
previously equal byte. RunEcl/Enemy/Title DrawResult/Type3 are now
**30 /13 /22 /15**. No complete new exact owner; the 95% and product/runtime
gates remain open.

- The genuine **20-byte ZunTimer::operator>(int)** becomes co-visible with
  Enemy through one ZunTimerGreater.inl definition in EclManager.cpp. Its old
  definition is removed from ZunTimer.cpp, keeping one implementation and the
  existing native symbol/ABI. Original TU ownership is not inferred.
- The full 3900-byte Enemy owner retains all 97 encoded original fields, 69 calls,
  1026 instructions, 170 blocks and four switch destinations. All 16 restored
  positions belong to tracking/homing; the guard now protects **3887 equal
  physical bytes** and permits only the **13** remaining draw/descriptor bytes.
  Four same-object native comparison calls and its complete original body are
  checked independently, including definition provenance and actual target E8s.
- All **94 affected existing exact units /24707 physical bytes** pass from
  three freshly compiled real source/profile carriers: ECL main/alternate and
  the remaining Timer source. The method moves between sources; no exact row
  is added. Other **907** unchanged exact source/profile proofs are reused.
- Two private label names change in movement/comparison. Full original bytes
  and actual owner-relative positions are proved before changing the manifests:
  +0x139 still targets 0x408548; +0x14 still targets 0x407A4C. Their offsets,
  kinds/addends and original destinations remain unchanged.
- Twenty-two successful private compiles comprise 21 Enemy controls and one
  typed-carrier constructor check. Greater-only visibility gives 13 with no
  damage. HasTickedEvery alone gives 61; both real bodies give 45, restoring old
  mismatches but damaging 45 old equal bytes. Those leads remain private. List,
  copy, typed payload, pointer visibility and expression alternatives do not
  close the owner. Native body equality does not validate a changed caller.
- CI passes **862 tests /28 skips**, with target-required tracking,
  documentation, generated progress and both whitespace checks. The first CI
  caught an old 73+4 carrier inventory; it is corrected to 74+4, and all 24 carrier
  regressions pass. Frozen source replay repeats retained 13/74 main exact units
  and rejects the real-bodies 45 lead; the archive replay uses the corrected gate.
- **157** evidence members /15094766 loose bytes become a **2301114-byte ZIP**.
  Removing verified duplicates and **387** source-bound compiler/bytecode caches
  reclaims **20329581 bytes (~19.39 MiB)**. Only the three fresh carriers' six
  OBJ/PDB files and terminal CI caches are removed. All 490 game inputs, 1031 unit
  definitions, canonical ledger hashes and frozen provider blobs are checked.
- Evidence, complete sources/COFF inventories and frozen recipes:
  `.analysis/gpt-61sol-20261021-enemy-draw-tracking/`.
- Use direct local IDA Pro MCP and Bash, serial `nice -n 10` compiles,
  English `gpt-6.1-sol: ...` commits, push and independent remote verification.

## Native Timer alias checkpoint at d2637e8 (October 21)

## Current verified handoff checkpoint

The operator requests continued exact reconstruction across **large functions
and small leaves**, serial low-priority compiles, focused affected replays and
regular cleanup. Starting from pushed `a44c9ef`, this checkpoint supplies the
already-declared **ZunTimer::operator int** native implementation. Its complete
**4-byte /zero-field** body equals original 0x00435F00. This address already has
the canonical exact **TextHelper::GetHeight** row; its source, origin, name and
physical credit stay unchanged. The new comparison unit is a **noncrediting
alias**. No unique original class or linker-folding mechanism is inferred.

- All **16 affected existing exact Timer units /313 physical bytes**, plus the
  four-byte alias, pass from one freshly compiled real source/profile carrier.
  The other **985** unchanged canonical source/profile proofs are reused.
  No header or ECL source changes. The maintained Enemy baseline repeats
  **13** differences; RunEcl/Enemy/Title DrawResult/Type3 stay **30 /13 /22 /15**.
- Fifteen successful private Enemy controls test five periodic source forms,
  four typed forwarders and six other genuine Timer body contexts. Co-visible
  periodic bodies still give **45** caller differences, restoring 13 but
  damaging 45 previously equal bytes. Early-return/ternary callees also shrink
  from 33 to 31 bytes and are not original-equal. Other native contexts retain
  13 or fail the existing RunEcl closure check. No private control is adopted.
- The tracked `check-timer-current-view.py` binds the alias ABI/layout/extent,
  existing TextHelper identity/credit, target and source/profile inputs. Three
  regressions reject metadata/credit substitutions and a wrong target before
  compiling. Native product/link/runtime closure remains unproved.
- CI passes **865 tests /28 skips**, plus target-required tracking,
  documentation, generated progress and both whitespace checks. Frozen replay
  repeats all 17 Timer units and the two selected rejected Enemy contexts.
- **130** evidence members /9200430 loose bytes become a **1444742-byte ZIP**.
  Removing verified duplicates and **387** owned compiler/bytecode caches
  reclaims **14814542 bytes (~14.13 MiB)**. Only the two fresh carriers' four
  OBJ/PDB files and terminal CI caches are removed; older unique evidence stays.
- Evidence, complete private sources/COFF rows, frozen sources/providers and
  selected serial cold replays:
  `.analysis/gpt-61sol-20261021-enemy-periodic-context/`.
- Use direct local IDA Pro MCP and Bash, serial `nice -n 10` compiles,
  English `gpt-6.1-sol: ...` commits, push and independent remote verification.

## Title actual-definition checkpoint at bbf92ac (October 21)

## Current verified handoff checkpoint

The operator requests continued exact reconstruction across **large functions
and small leaves**, serial low-priority compiles, focused affected replays and
regular cleanup. Starting from pushed `d2637e8`, this checkpoint independently
validates the **actual local literal definitions** used by Title DrawResult
and its shared-inspector DrawReplaySave sibling. **No game source or existing
unit definition changes; no new exact owner.** RunEcl/Enemy/Title/Type3 stay
**30 /13 /22 /15**. The 95% and native product/runtime gates remain open.

- DrawResult still has **1939 bytes /577 instructions /82 fields /16 calls /
  92 blocks /22 differences**. All **15** local definitions (seven scalar
  floats, eight C strings) now require actual initialized read-only COFF data,
  valid section/value bounds and complete original PE payload equality.
- Fifteen actual definition mutations keep all main bytes and actual COFF
  relocation records identical. The old name/address-only inspector accepts
  all fifteen; the current producer rejects all fifteen. Symbol spelling and
  a correctly linked operand cannot substitute for actual source data.
- Shared-inspector DrawReplaySave also passes all **18** local definitions
  (13 floats, five strings), retaining **800/245 instructions/41 fields /
  four calls/28 blocks/96 differences**. All current equal bytes remain guarded.
- Four successful natural entry/accessor controls give one unchanged1939/22,
  two1941-byte callers and one1910/83 caller with an unreviewed helper call.
  None is adopted; no forced-inline, variable-order or compiler-flag controls.
- New **`--owner resultdraw`** compiles and guards just the complete DrawResult
  owner under its recorded profile. Full Title/all routes reuse the same guard.
  All **1001** unchanged canonical source/profile proofs are reused.
- CI passes **871 tests /28 skips**, with target-required tracking,
  documentation, generated progress and both whitespace checks. Frozen replay
  repeats the two complete owners and all fifteen actual definition rejections.
- **56** evidence members /4233218 loose bytes become a **651924-byte ZIP**.
  CRC/hash verification precedes duplicate removal. Removing **389** owned
  compiler/bytecode caches reclaims **10278755 bytes (~9.80 MiB)** in total;
  only the two fresh OBJ/PDB pairs and terminal CI caches are removed.
- Complete controls, actual COFF metadata, frozen providers/sources and selected
  serial cold replays: `.analysis/gpt-61sol-20261021-title-entry/`.
- Use direct local IDA Pro MCP and Bash, serial `nice -n 10` compiles,
  English `gpt-6.1-sol: ...` commits, push and independent remote verification.

## Native Timer/queue context checkpoint at a97f9e6 (October 21)

## Current verified handoff checkpoint

The operator requests continued exact reconstruction across **large functions
and small leaves**, serial low-priority compiles, focused affected replays and
regular cleanup. Starting from pushed `bbf92ac`, this checkpoint tests the
**actual Timer setter/assignment context around RunEcl**, plus three genuine
two-side queue container views. **No game source, unit definition or ledger
changes; no new exact owner.** RunEcl/Enemy/Title/Type3 stay **30 /13 /22 /15**.
The 95% and native product/runtime gates remain open.

- The fresh ECL baseline and genuine **25-byte SetCurrent-only** context
  reproduce all **15564 raw bytes /598 fields /30 ordinary differences**,
  zero field/table differences and Enemy13. The callee is independently exact.
- Adding its actual **nine-byte void assignment wrapper**, before or after
  RunEcl, gives identical **15560-byte /598-field** callers. Both real callees
  remain exact. Complete RunEcl and native Enemy inventory checks reject both;
  a correct callee does not establish a correct caller or original TU ownership.
- The opcode4 argument/call raw window remains unchanged. Broader normalized
  instruction correspondence first diverges at +220F; two instructions and
  four physical bytes disappear overall. No relocated/graph-aligned substitute
  is accepted for the original full extent.
- The small received-frame baseline and 2D-array/grouped-container/typed-shift
  controls all emit identical **213 bytes /67 instructions /zero fields /
  nine blocks /35 differences**. No field, ABI, profile or source is adopted.
- New `probe-runecl-setter-context.py` compiles **one selected case** at nice10,
  pins project/provider/profile inputs and actual staged files, proves complete
  native callees and records both complete caller results or strict rejections.
  Temporary sources/OBJ/PDB self-clean. Five tests cover real ABI/extent,
  wrong-target preservation, failed compiler and changed project/staged inputs.
- All **490** game inputs, **1032** units and canonical ledgers remain unchanged;
  all **1001** existing exact source/profile proofs are reused.
- CI passes **876 tests /28 skips**, plus target-required tracking,
  documentation, generated progress and both whitespace checks. Final frozen
  replay repeats setter-only30, rejected15560 and small grouped213/35 cases.
- **138** evidence members /7947574 loose bytes become a **1091849-byte ZIP**.
  CRC/hash verification precedes duplicate removal. Removing **387** terminal
  CI bytecode caches reclaims **13379774 bytes (~12.76 MiB)** in total. All
  compiler carriers self-clean; older unique/unclassified evidence stays.
- Complete sources, actual COFF rows, separate public-provider epochs and
  selected cold recipes: `.analysis/gpt-61sol-20261021-runecl-setter-context/`.
- Use direct local IDA Pro MCP and Bash, serial `nice -n 10` compiles,
  English `gpt-6.1-sol: ...` commits, push and independent remote verification.


## October 30 archival of the cc19b484 handoff

The following handoff is preserved verbatim from cc19b484. Its October 24
stop, Factory/IDA routing, source checkpoint and CI counts describe historical
sessions. They are not current instructions or current-source acceptance.
Use [RE_HANDOFF.md](RE_HANDOFF.md) for the October 30 maintenance handoff.

# TH09 reconstruction — active handoff

> Current routing only. Use the live Git tree and ledgers before historical
> numbers or hypotheses. Earlier details remain in
> [the historical checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md),
> [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md) and Git history.

## Active local restart — October 29

The operator explicitly resumed reconstruction with direct local tools, focused
affected-owner replays and commits prefixed `gpt-6.1-sol:`. The latest routing is
**direct local Ghidra, without IDA, REA or Factory MCP**. This supersedes the
historical stop and tool routing below. Original Japanese v1.50a disk identity
and tracking pass. Ghidra supplements the original PE and pinned VC7.1 Oracles;
it does not grant source exactness credit.

### Direct Ghidra project

`python3 -B scripts/ghidra-analysis.py` imports the original once, then reuses its
saved project with `-noanalysis -readOnly`. Select functions with repeated
`--function 0x...` arguments; the default selects RunEcl, Enemy OnUpdate and
Type3. Use `--ghidra-dir /path/to/ghidra` or the ignored `.tools/ghidra` symlink.
The current direct Ghidra12.1.4 project and exports are below
`.analysis/ghidra-direct/`; `direct-import/` holds the initial three-function
export and `reused-type3/` proves cached export without another import/analysis.

Every invocation checks disk/Program SHA-256, i386 language, image base and entry,
then compares all685056 file-backed PE bytes in headers and five sections before
analysis and export. Tool and script hashes bind the receipts. Native projects,
decompiler text and logs stay ignored. Ghidra rejects hidden components in project
URLs, so the runner uses a temporary visible symlink to the actual cached project;
the database remains below `.analysis/`. Automatic types/boundaries are provisional:
RunEcl has14777 body-set bytes over14792 code-span bytes, Enemy3881 over3883,
and Type3 has780 contiguous bytes. Keep the original reviewed physical owners,
including padding and tables, for compiler comparison.

ReplayInput now uses the four existing `GameManagerModeView` predicates instead
of undefined private Replay method facades. Its data overlay and original
receiver remain unchanged. The previously unadopted H65 source was recovered;
SafeSEH `.sxdata` entries are compared as actual COFF symbol references, with
handler section/value/type/storage identity and code checked. Both canonical
Replay profiles were rebuilt. All **16 affected exact owners / 4,150 physical
bytes / 232 fields** pass, including the four native mode leaves. All nondebug
sections retain their effective contents; only ten manifest symbol strings
change, with no address, offset, addend or profile changes. SaveReplay remains
**1,238 bytes / 53 fields / three differences**; BeginPlaybackStage remains
**538 versus 540 bytes / 32 fields / 39 overlapping differences**. Exact credit
remains **244917/291883 (83.9093%)**, 1003 functions. Forty focused Replay tests
pass. No whole-product/runtime or original-exact EH claim follows.

Evidence and no-compile replay:
`.analysis/gpt-61sol-20261029-replay-mode-adoption/verify_current.py` and
`current-verification.json`; `check_sxdata.py` rejects four metadata/code
corruptions. The original H65 packet and failed verification remain preserved.
A fresh Enemy main-carrier build independently retains **3900/97/13**, 69 calls
and 170 blocks; its diagnostic is in
`.analysis/gpt-61sol-20261029-enemy-live/baseline-enemy.json`.
RunEcl and Enemy remain the principal unfinished fronts; the October 24 source
and receipt bindings below are historical baselines, not the latest source.

### Type29 constructor lifetime study after 962ddb20

Four bounded main-profile builds compare the current Type29 draw with three
ordinary geometric constructor models. Derived and composed 12-byte centers
inline completely and produce identical 2205-byte/129-field owners, but increase
overlap differences from1170 to1238. Owning the angle as well gives2204/129/1431.
None recovers the original six center receiver reloads; all three fail the
unchanged full-owner guard. No game source or profile is adopted. All75 existing
same-profile exact owners retain22463 physical bytes/990 fields in every case,
with actual literal definitions checked; RunEcl30 and Enemy13 also remain.
The source-bound packet is
`.analysis/gpt-61sol-20261029-effect29-witness/`; `verify.py` rechecks its full
objects, original-bound comparisons and source/provider hashes without compiling.
It retains the tested source recipes and native outputs. This closes these
constructor hypotheses, not the Type29 owner or the reconstruction goal.

### Enemy/Front native indicator connection

Enemy now calls the implemented FrontEnemyIndicatorView member through the
shared g_PlayerSharedRuntime pointer, removing its undefined private Front
method/global declarations and two inspector aliases. The shared header uses
the existing Float3 for the same twelve-byte input/position. Front's complete
213-byte/10-field owner remains exact. Both ECL profiles plus Front cold-replay
all80 existing exact owners:25428 physical bytes/1136 fields. All203 nondebug
main-carrier sections retain50452 raw bytes/2097 effective relocation identities;
five API names and nine private-label references change spelling only. Two
canonical private labels were refreshed after their actual coordinates passed.
RunEcl remains15564/598/30; Enemy3900/97/13; no new exact credit is claimed.

Nine bounded Enemy controls preserve75 exact siblings each. Explicit count
branches/store operations leave opaque13 or manager/periodic41 unchanged. A
current-frame snapshot preserves the exact33-byte periodic callee and caller45;
early-return/local-result alternatives emit31/37-byte callees and are ineligible.
Direct Ghidra independently confirms the native indicator and call paths. Its
missing Effect AddedCallback entry was created only in a read-only session from
the reviewed51-byte hash/extent; every mapped file byte was rechecked afterward.
The packet is .analysis/gpt-61sol-20261029-enemy-periodic-selection/;
verify_adoption.py replays the current complete proofs without compiling, and
verify_cases.py rechecks the archived experiments.148 focused tests pass.

### Replay native RNG connection and representation controls

Replay now uses the implemented RngRuntimeView range generator and shared
global instead of the undefined private int-return facade. Actual seed methods
remain behind their existing layout-checked projection. The unused declaration
is removed. Five affected canonical carriers preserve25 existing exact owners,
4553 physical bytes/257 fields, including the complete41-byte range generator
and39-byte NextU16. Both full Replay objects retain all effective nondebug
contents; only nine API references per profile and private EH labels rename.
SaveReplay remains1238/53/3 and BeginPlaybackStage538/540/39. Shared-header
network coverage retains808/46/105. The current proof/replay is
.analysis/gpt-61sol-20261029-replay-native-rng/verify_current.py;57 focused
tests pass. No new exact credit or full product closure is claimed.

Seventeen bounded serial compiles after a4a3cd13 test SaveReplay address/byte
operations and the213-byte received-frame leaf. Eight Replay source models
repeat its complete1238/53/3 baseline; signed-byte masking gives1246 bytes and
a64-bit accumulator1289, both rejected. All22 existing same-profile exact-owner
comparisons pass. Five queue copy models repeat every213-byte baseline byte
and35 differences. No game source/profile/ledger is adopted. The two packets
are .analysis/gpt-61sol-20261029-replay-byte-addresses/ and
.analysis/gpt-61sol-20261029-queue-representation/; each has verify.py, frozen
inputs and complete COFF. Their verify.py scripts use the preserved a4a3cd13
source/provider epoch, not the newly adopted RNG declarations. The direct
Ghidra exports are under .analysis/ghidra-direct/replay-save-20261029/,
frame-insert-20261029/ and replay-native-rng-20261029/.16 focused tests passed
before adoption; all17 archived observations replay afterward.

### Player cancel branches and complete collision-owner controls

CheckBulletCollision now uses separate type-2/type-3 branches into the existing
cancel response. A source-cold complete-owner comparison improves804 overlapping
differences to799, recovering original offsets+2F5,+311,+31B,+32D,+32E without
breaking any previously equal byte. It remains849 versus862 physical bytes,
266/277 instructions,26 fields,12 ordered calls and32/33 direct blocks; no exact
credit is awarded. Five same-TU exact owners retain1220 bytes/32 fields across
three canonical carrier paths. The native rotate callee remains62/2 and both
actual compiler math wrappers retain their complete12 bytes.74 focused tests pass.

Direct Ghidra exports RunEcl and collision with full mapped-file attestation at
.analysis/ghidra-direct/ecl-collision-20261029/. Twenty bounded serial compiles
test scratch lifetimes, shared workspaces, circle/box/reward block placement,
type gates and real rotate visibility; every case preserves the five exact
siblings. No workspace or TU move is adopted. Actual rotate visibility under
the caller's unchanged profile emits a nonexact42-byte callee; consumed trig
function pointers restore the whole62-byte callee and both12-byte wrappers,
but do not improve the caller beyond799 differences. Keep the separate native TU.

The packet .analysis/gpt-61sol-20261029-collision-workspaces/ retains frozen
source inputs, all20 COFFs, recipes and results. verify_cases.py rechecks that
historical source/provider epoch without compiling; verify_current.py rechecks
the adopted current-source proofs, or cold-builds only these affected carriers
with --build. The public diagnostic now checks all four actual read-only scalar
definitions, including double zero's upper four bytes. Its noncrediting guard
protects all50 currently equal original positions while allowing future complete
convergence. Exact totals remain1003 and83.9093%; RunEcl/Enemy use their unchanged
source/profile-bound proofs. Native product/runtime and98% remain open.
Public CI passes1005 tests with29 skips in125.079 seconds; its final log is
.analysis/gpt-61sol-20261029-collision-workspaces/ci-final.log. Tracking,
documentation, generated progress and whitespace checks pass.

### Enemy callback and special-attack boundary controls

Fourteen serial private compiles after015588e0 test three callback-entry models,
complete/partial special-attack operations and the51-byte Effect callback's
resource-return boundary. Eligible Enemy models retain every3900 baseline byte
and13 differences; the complete operation outlines a new290-byte/nine-field
callee, shrinking the caller to3608/88 and failing original call placement.
Effect models retain8 differences or regress to11. No source/profile/ledger
change is adopted. Both current canonical carriers were cold-built; all90
existing owners retain24993 physical bytes/1090 fields. The packet is
.analysis/gpt-61sol-20261029-chain-callback-context/; verify.py reproduces all14
cases and750 repeated existing-body proofs without compiling. See the KB for
the bounded conclusions and fresh mapped-byte-attested direct Ghidra export.

### Controller native range snapshot

ControllerInput now uses a size/offset-checked read-only view of each native
JOYCAPSA min/max pair, copying the low-edge bounds while retaining the source
for the high edge. Both original midpoint evaluations per axis return. The
complete current owner is810 versus808 bytes,247/246 instructions,32 fields,
18 ordered calls and eight branches; all nine native globals and the674-byte
prefix pass. It restores30 original bytes without damaging the694 prior equal
positions. There remain84 differences and two excess bytes, with no exact credit.
Both same-TU exact helpers cold-replay37+47 bytes. All other source/profile
proofs are unchanged. The packet .analysis/gpt-61sol-20261029-controller-accumulation/
has32 frozen captures and62 existing-helper proofs; verify.py replays that epoch,
and verify_adoption.py checks current source (one carrier with --build). The
799-byte/93-difference candidate breaks an old match and remains rejected.

## Historical operator stop — October 24, 14:09 UTC

TH09 research is paused at the operator's explicit request. Only cleanup and
a local commit were requested; the old autonomous continuation instructions
are superseded. Do not start new experiments, adoption or monitoring without a
new request. The 98% goal remains unfinished.

The only pending tracked source edit, ReplayInput.cpp, was preserved with its
patch and hashes under .analysis/th09-replay-native-mode-api-20261024-v01/
canonical-adoption/ and restored byte-for-byte to HEAD. No manifest, inspector
or test proposal was applied. Its two canonical captures compiled, but adoption
verification stopped at .sxdata raw symbol indices. A read-only observation
resolved those indices to matching handlers; no corrected acceptance verifier
was completed. This remains an unadopted experiment, not a source checkpoint.

Maintained gameplay/API source remains 1bccbf2a; source and exactness credit are
unchanged. H65/H66 below record the final isolated results. All workers stopped.
Original target/toolchains, unique objects, logs, failed gates and proposal
artifacts are retained; no blanket .analysis cleanup or proof deletion occurred.
Final closeout CI passes 1020 tests / 29 skips in 124.831 seconds, durable
exit 0 (.analysis/gpt-dots-20261024-continuation/ci-final-pause.log and .exit).
Tracking, documentation, open build graph, progress and whitespace checks pass.

## Current verified source checkpoint

The operator resumed reconstruction through Factory MCP on October 24 after
the clean successor checkpoint95300f57. Focus on RunEcl and Enemy, including
whole-function/shared-type/translation-unit hypotheses rather than only local
residuals. Disk target and six-sample mapped IDA attestation passed on this
resumption. The following predecessor measurements remain the starting baseline;
new attempts must be independently compiled and fully compared. Current work
is tracked independently in [the fresh hypothesis checklist](TH09_BOSS_HYPOTHESES_20261024.md),
not inferred from historical conclusions. Use AGENTS.md
for current routing and commit identity. Historical handoff cleanup is complete.

- Live exact credit is **244917/291883 (83.9093%)**, another **41129** whole
  exact bytes short of98%. This API-only source cleanup changes no compiler
  profile or exactness ledger. Native product/runtime and later gates remain open.
- Current maintained source is **1bccbf2a9376ba929cdaa71bc488c5e6b02bfdde**,
  the minimal H53 real Effect vector API checkpoint (local, not pushed);
  Documentation checkpoint **e3f3b9d6** adds no source/profile/ledger change.
  **22baf4e7** is its predecessor. It preserves the selected-ECL-manager ownership repair from **f595786** and adds
  the independently verified SDK resolver and effect-release API corrections.
  Preserve `.analysis/th09-selected-ecl-manager-owner-20261023-v01/`, especially
  its34 canonical objects and sidecars. Its140-unit adoption proof is scoped
  to its source/profile bindings; unchanged proofs may be reused after checking.
- RunEcl remains15564 physical bytes/598 fields/30 differences, split7/23 between
  opcodes4/155. Enemy remains3900/97/13, with69 calls and170 blocks. Preserve all
  existing matching bytes; natural source hypotheses still require full owners.
- Preserved predecessor packet: `.analysis/gpt-61sol-20261024-ecl-math-boundary/`.
  `verify.py` replays its archive without compiling:378 attempts,366 passing
  existing-body comparisons,6 expected moved-definition absences and6 actual
  callback regressions. Hiding all Float3 core definitions damages Type4/8 by55
  bytes each; constructor-only damages36, independently cold-reproduced.
- The opcode155 successor audit supports no invented old-flags use or Timer
  return contract. Real rank-reset visibility removes Enemy's original+CC3
  receiver reload. Read these dated KB observations before repeating models;
  they do not identify unique original source partition or exhaust alternatives.
- Cleanup/recovery instructions and exact retained/deleted scopes are in
  [cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md). The current audit packet is
  `.analysis/gpt-61sol-20261024-clean-handoff/`; ignored evidence must accompany
  the worktree at transfer. Keep original target, toolchains, SDK/Wine/IDA state
  and unique proofs; build caches may be absent and can be rebuilt.
- Cleanup archives32812 historical snapshot files into five verified tar.xz
  files and removes134 regenerated bytecode caches. Combined source/Git file
  allocation falls by about264MiB. All802 non-documentation inputs,14 existing
  build files,34 adopted objects and original target remain unchanged. CI passes
  1016 tests/29 skips; archive-only integrity and an actual restore both pass.

## Current source-interface correction

The October24 continued campaign replaces the isolated ECL resolver's private
vector Length member view with the actual SDK D3DXVec3Length API and existing
Float3 subtraction. Four resolvers remain original-exact:5718 physical bytes and
374 fields. Canonical-path replay refreshes only6 external symbol identities and
291 proved same-coordinate private labels, never target bytes or destinations.
The synthetic co-visible resolver model is rejected; EclManager.cpp is unchanged.
Evidence and current validation state are in H20 of the independent checklist
and .analysis/th09-ecl-sdk-vector-interface-20261024-v01/live-adoption/.
No exactness gain is claimed; RunEcl30 and Enemy13 remain active frontiers.
Earlier experiment packets are source/provider-scoped historical evidence and
may correctly reject current providers after this narrow manifest update.

The subsequent API-only correction directs Enemy attached-effect cleanup to the
existing64-byte Th09EclRunState::ReleaseAttachedEffects implementation instead
of an undefined Core facade. Definition visibility is unchanged. Two individually
proved private COFF label names are refreshed; target addresses/profiles and
comparison rules remain unchanged. Cold Enemy gate passes87 canonical siblings,
RunEcl30/Enemy13 remain. H30 records the current full-CI/checkpoint state.

## Historical continuation results — paused

Earlier continuation authorization is superseded by the explicit stop above.
Current exactness remains RunEcl 30 / Enemy 13 differences; no new
whole-owner credit was gained by the two API corrections. Latest source CI
passes1020 tests with29 skips in133.873 seconds, durable exit0. Strict H53
current-manifest replay passes53 jobs/208 proofs with zero symbol overrides or
local renames; all139 main owners remain unchanged at RunEcl30/Enemy13. Tracking,
progress, documentation, open build graph and whitespace checks pass. This is
the narrow source checkpoint following22baf4e7 and docs3acea9a8; its durable
verified-source-state receipt will bind the commit after creation.

- H31 common Enemy/operand graph and H32 Replay packed-frame pair are complete,
  independently verified negative results, not active preparations. H31 is
  paired-neutral at RunEcl42 / Enemy13 (live remains30 /13); H32 preserves the
  same three SaveReplay residuals. Full affected-header adoption is not claimed.
- H33 explicit descriptor initialization is neutral. H34 reference flag family
  changes whole-owner shape without repairing the residuals. H35 ordinary member
  flags introduce11 unmatched runtime calls and are rejected. All remain isolated;
  stock guards and current source/profile/test inputs remain unchanged.
- H36 current full-macro pair is independently verified: /Ob1 preserves75 exact
  witnesses but has15496-byte RunEcl/368 direct calls; /Ob0 has15540 bytes and the
 375-call destination multiset but fails15 witnesses. Neither fixes opcode4/155;
  counts/multisets are not full CFG or original-byte proof. No profile is adopted.
- H37 wide-wire/flag-successor audits are complete without a staged model: no
  proven four-byte payload extent, original byte reads, no uniquely recovered
  source type from MOVZX, and no actual later old-flag consumer. No compile or
  exactness result is claimed for these audits.
- H38 native-timeout interaction on frozen H36 /Ob0 is complete: same60/15
  witness result,112/113 emitted owners unchanged, neither residual repaired.
  Its inherited15 failures still block adoption. H36 /Ob0 and H38 spawn93/94
  raw paths also replace original integer MOV copies with FLD m32/shared FSTP.
  All-bit-pattern FP equivalence is unproved: signaling NaNs may be quieted and
  x87 invalid/denormal flags may change. This is instruction-semantic analysis,
  not a runtime test or finite-input guarantee, and independently blocks adoption.
  H39 returned-float-base leaf is
  fully neutral across139 emitted owners/50104 bytes/2097 fields;75 witnesses pass.
- H40 conditional five-state wide-native compilation is rejected: all five
  reads are DWORD instead of original BYTE. H37's missing source-extent proof
  remains true; this was a conditional compiler test, not source-memory
  equivalence or permission to adopt/run widened reads. H41 original metadata
  and Enemy/jump use-chain audits are complete without a new source model.
- H42 whole-TU /Ob0 recovers all15 inherited witnesses to75/75 and genuine
  opcode64/88 private caller transport. Of95 surviving owners,90 match maintained;
  four other nonexact owners remain changed. H43 CRT-retained RunEcl macros /Ob1
  also gives75/75 with all114 surviving non-RunEcl owners unchanged. Both have
  RunEcl15544/598/375 direct calls but remain nonexact; neither proves unique
  original profile choice, and both retain the raw-spawn FP semantic mismatch.
- H44 branch-owned spawn transfer is verified:15572/598, same60/15 witnesses,
  all112 other parent owners neutral. H45 composition on H43 /Ob1 is verified:
 15576/598, all75 witnesses and114 other surviving owners maintained-identical.
  Both restore six raw integer-MOV paths and six original-shaped resolver arms.
  The raw-FP limitation is fixed only in those six spawn transfer paths, not a
  blanket FP-equivalence claim; H42/H43 historical limitations remain. Opcode4
  and opcode155 still differ, strict full-owner guards reject, and no adoption
  or exactness credit follows.
- H46 removes two redundant integer-prvalue casts and is fully neutral across
 115 owners/2089 fields,75 witnesses passing. H47 uniformly replaces three do0
  procedural shells at12 safe sites: RunEcl15560/598, all75 witnesses and114
  other owners protected. Its943 blocks/root indices/structural shapes match
  original, but full bytes do not; opcode4/155 remain wrong and guards reject.
  The16-byte reduction is7+8+1 bytes of shell-related layout, not a padding patch
  or proved compiler-IR mechanism. Remaining slice lengths differ140−3/145+1/
 157−2; equal-size regions also differ. Four bytes short is not four mismatches.
- H48 existing four value-read APIs give15564/598 but include two padding bytes:
  stock identity binding passes, equality fails with4147 shifted differences.
  Independent H47/H48 maps bind all598 fields with zero unknowns,943 blocks and
 375 calls. H48 retains six changed regions4/135/139/140/145/155 versus H47 seven;
  only157 disappears from the instruction map, not newly exact owner credit.
- H49 WriteInt/Float retention is neutral across117 previous owners, plus one
  maintained WriteInt COMDAT (118 total), with no synthetic API calls. H50 native
  timeout changes only RunEcl among117 owners:75 witnesses pass,15564/598 but4139
  shifted differences;44-byte opcode155 still has wrong registers. All byte wire
  reads and six raw spawn transfers stay protected. No candidate is adopted.
- H51 State/Late family closes neutral across117 owners/75 witnesses, with byte
  wire and DWORD flag accesses protected. H52 module closure fixes none of13
  Enemy differences and removes the required+CC3 receiver reload; rejected.
  H54 seven raw copies are neutral across139 owners, with seventh-site abstract
  memcpy nonoverlap still unproved. H55 draw SDK/return audit finds no supported
  new mismatch or candidate; missing fresh-object proofs remain explicit.
- H53 three-arm Effect vector study is neutral. This checkpoint adopts only
  three real Float3 += API sites, unused POD facade declaration cleanup and
  three regression tests, not storage aliases or added header co-visibility.
  Strict post-refresh current-manifest53/208 proofs pass with zero overrides or
  local renames. Actual365 symbol-string changes are362 DIR32 same-owner labels
  plus three REL32 API identities; targets/offsets/addends/profiles stay unchanged.
  Twenty initially misattributed staged labels were corrected as introduced.
  CRLF introduced by git apply was rejected; exact reviewed LF source and original
  binding were restored before canonical compilation, without hash waiver.
  Historical failed guards/staged maps remain recorded, not used as exemptions.
  Two unprofiled TUs, native product/runtime and portability remain outside this
  canonical-profile closure. There is no exactness gain.
- H56 byte-flag census confirms exact ANM438DC9 byteArgs[1] assignment without
  later old-word use. Faithful direct compound preserves75 witnesses/137 other
  owners and exactly reproduces H15a's full raw/598-field emitted class:15564
  bytes,2048 ordinary/242 field shifted differences, no adoption. Removed helper
  is16 bytes, not the initial24-byte report; correction note and separate seal
  are complete, with the pre-correction seal retained.
  Oct17 missing break invalidates only that recipe, not every previous model.
- H57 direct-byte acquisition pair is complete:138 owners/2097 fields pair-neutral,
 75 witnesses per arm, inherited native-timeout differences remain. H58 anonymous
  primary owner is neutral across the same138 owners and75 witnesses. Neither
  grants portable lifetime/source uniqueness or exactness credit.
- H59 direct raw-member pair preserves135 common owners/2097 fields; four
  uncalled5-byte free accessors become uncalled14-byte members, zero references
  to them. Both75-witness passes and full RunEcl30/Enemy13 remain unchanged.
  Five-TU header closure is uncompiled, not needed for an unpromoted neutral model.
- H60 shared signed role is complete: 138 other owners remain neutral and 75
  witnesses pass, but RunEcl regresses to the already verified H18 class. Full
  raw bytes and all 598 effective references agree with H18; opcode 157 changes,
  while opcode 4/155 do not improve. This is not a new emitted solution.
- H61 direct single-use signed consumers is complete and fully neutral across
  139 owners / 50104 bytes / 2097 fields, with 75 canonical witnesses passing.
  The transform index and initial callback test are intentionally direct; later
  fresh callback capture and repeated sampleStride remain. RunEcl 30 / Enemy 13.
- H62 four-State low-bit publication pair is complete. Both arms pass 75
  canonical witnesses and are identical across 138 owners / 50088 bytes / 2097
  effective fields. Both inherit the native-timeout regression: 2140 linked
  differences = 1938 ordinary + 202 field bytes; the 26 table bytes are a subset,
  not additional. The unused 16-byte timeout helper is the only missing owner.
  The boolean RHS family adds no further change; maintained stays 30 / 13.
- H63 guarded shared jump body is fully neutral across all 139 owners and
  effective fields, with 75 canonical witnesses passing. The original op5
  prefix's full 50 bytes and two call fields match; direct case4 bypass, positive
  fallthrough, nonpositive advance and time/call/displacement chronology are
  preserved. No original source-style inference or exactness credit follows.
- H64 complete signed-role jump capture is rejected: 2165 shifted differences
  versus H60's2153 and maintained30, with 75 witnesses and138 other owners neutral.
  Exactly12 bytes differ from H60, all in opcode7; its full75-byte slice/two fields
  equal actual H14 output. All598 H60-relative effective fields agree. Actual
  four-byte copy lowers without escaping scratch address or memcpy call, and
  direct4/positive5 timer/displacement chronology remains. Neither boss improves.
- H65 Replay native-mode API experiment is neutral: all 32 emitted-owner pairs,
  24 canonical comparisons and four native callees pass. SaveReplay retains
  three differences; BeginPlayback retains its full prior mismatch boundary.
  Canonical adoption was not completed; the source edit is restored as above.
- H66 persistent trailing operand-record pair is neutral across 139 owners /
  2097 fields per arm, 75 witnesses each. RunEcl30/Enemy13 remain. The legacy
  extent/lifetime hypothesis and uncompiled five-TU/87-unit closure are not
  promoted. The independent deferred-shot audit was not completed.
- There is no active next task. Resume only on a new explicit operator request.
- Lossless archival of116 compiler PDBs passed full restores;1046 non-PDB files
  stayed unchanged, net allocated saving8548352 bytes. Postarchive strict53/208
  replay passes. See cleanup history; no fresh CI claim for this docs-only batch.
- The fresh checklist records H01-H66 and source-bound packets. Earlier provider
  hashes are intentionally historical; never update them blindly to current HEAD.
  Earlier source-bound CI1017/29 and documentation CI1017/29 (130.450 seconds)
  remain historical receipts, superseded for current validation by H53's1020/29.
- Superseded H31 v01 loose source snapshots are now preserved in a verified
 232628-byte tar.xz archive (522 files, actual full restore tested). v02 and all
  accepted evidence remain untouched; see cleanup history for the exact receipt.

Latest H38-H47 documentation validation: fresh full CI1017/29 passes in123.402
seconds, exit0. Only two documentation files change; maintained source22baf4e7
and exact credit stayed unchanged. That historical checkpoint is3acea9a8.
Current H53 source/API adoption and1020/29 validation supersede its active state;
chronological experiment evidence and unchanged exact credit are preserved.

## Live ledger snapshot


The canonical state is `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv`, and `config/match-units.toml`, checked by
`python3 scripts/validate-tracking.py --require-target` and
`python3 scripts/validate-docs.py`. The current restart summary is
**1035 source-present / 1003 exact / 32 non-exact**; earlier operator notes
mentioning 979/876/103 are obsolete for this checkout.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,248 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 37 |
| Confirmed authored | 1,035 |
| Classified exclusions | 1,176 |
| Source-present authored mappings | 1,035 |
| Canonical exact functions | 1,003 |
| Source-present non-exact functions | 32 |
| Source-present non-exact bytes | 46,966 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 244,917 |

**244,917 / 291,883 reviewed-authored bytes = 83.9093% exact**. To reach
98% of the *currently classified* denominator, another **41,129 bytes**
must become complete original-bound exact owners, reaching at least **286,046
exact authored bytes**. The 37 origin-unresolved functions are excluded and could change the denominator. Similarity fractions
and locally matching subsequences never count as accepted exact.

The catalog is not proof of complete executable-code inventory. The formerly
uncatalogued **0x0040D677 /0x00444A78** Timer references now belong to the exact
Effect 1/2 and Effect 34 updates. All **203** captured assignment callsites have
catalogued owners; that bounded census proves neither complete executable
coverage nor a return ABI. The Effect 34 **178-byte initializer at 0x00444960** is now exact; its
**946-byte draw at 0x00444AF0** is a maintained, explicitly nonexact frontier.
Do not silently treat uncatalogued code as excluded.

## Active frontiers

Code entrypoints: `src/EclManager.cpp` assembles RunEcl's lexical fragments and
`src/EnemyManagerCore.inl`; `src/EclRunControl.inl` and `src/EclRunState.inl`
contain the remaining opcode4/155 logic. The other principal entries are `src/TitleScreenResultDraw.cpp` and
`src/ExAttackUpdateType3.cpp`; canonical
source/profile bindings are in `config/match-units.toml`. Inspectors and replay
selection live in `scripts/check-boss-frontiers.py` and `scripts/inspect-*.py`.
Detailed prior table wording is preserved in commit77e0b2c2 and the KB.

Prioritize **key large functions**; small leaves remain in scope. Each row
refers to a maintained, authored, **non-exact** function in `config/functions.csv`.
Comparisons are current-source/original PE cold baselines; lengths are logical
unless explicitly marked physical. The ledger, not this selected table, lists
all32 maintained nonexact functions, including the2,220-byte authored Type29
draw whose maintained candidate remains nonexact.
For the complete <=256-byte cohort see
[`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md).

The isolated SetCurrent/assignment context lead has now been cold-tested:
setter-only retains30; both real bodies before/after RunEcl shorten the caller
to15560 and fail the complete owner. Do not reopen it as an untested hypothesis.
Earlier related noinline/all-method controls were already recorded in the KB;
these results do not establish unique original translation-unit ownership.

The earlier whole public/State/Core Enemy-storage model was byte-neutral
across 139 emitted functions and remains unadopted. The distinct narrow
selected-ECL-manager correction was adopted in f595786, with all 140 canonical
collateral units verified. RunEcl30 and Enemy13 remain unresolved. Earlier
context/adjacent-game audit links are historical evidence, not pending ownership
work. See the current checkpoint above and the dated knowledge-base entries.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode4 timer transport and opcode155 flag update. Keep all598 fields/tables and complete15564 bytes; actual Timer ABI and RNG ownership are already corrected. `--owner runecl`. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**,97 fields, 69 calls,170 blocks; **13 differing bytes** (29->13) | Draw index2, side/threshold4, descriptor-position copy7. Keep real20-byte Timer greater co-visible, all97 fields and3887 matching physical bytes. |
| **Effect Type29 draw** | 0x00448750 | **2,220 /2,205 bytes**,129 original referents,72 ordered calls,41/41 physical blocks; **1170 overlap differences +15 missing** | Main/satellite fan chronology, polar receiver reloads and ordinal graph. Bounded emulator fixtures are not whole-runtime or exactness proof. `--owner effect29`. |
| **Effect 34 draw** | 0x00444AF0 | **946/946 physical**,53 original referents,20 direct +12 COM calls,286 instructions,16 blocks; **78 linked differences** | Entry65, store order9 and radius operands4 remain. Preserve868 matching bytes, all53 fields, actual float data and full physical extent. `--owner effect34`. |
| **EtamaController::OnDraw** | 0x00413BE0 | **600/600 physical**, 27 fields, 13 calls, 17 blocks; **268** differences | Laser body-VM position cursor. Preserve332 matching bytes and the genuine final DrawBulletLayerEffects member identity. `--owner etama`. |
| **PauseMenu::OnUpdate** | 0x00434740 | **1,776 original physical / 1,732 candidate**; 46 direct calls plus 2/3 original/candidate timestamp calls; 10-way switch; **1,453** overlapping differences + 44 missing | Share timestamp tail for states3/8/9 and recover branch/VM scheduling. Keep real UnPause and existing exact siblings. `--owner pause`. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | Lower-bound EAX/EDX transport and encoding cascade. Native framerate/coordinate owners are corrected; preserve1771 matching bytes and both tables. `--owner movement`. |
| **Player charge update** | 0x0041F810 | **1,210 target /1,197 candidate bytes**, 70 fields, 58 calls, 350/342 instructions; **1,160 overlap differences +13 missing** | Recover source chronology and missing13 bytes; preserve repaired SpawnShots transport and actual call/field identities. |
| **Front weighted message RNG** | 0x004162D0 | **693 target / 681 candidate**, 15 fields, 6 calls, **569** overlap + 12 missing | Primary/opposing table selection and remaining control flow; exact FrontCalc sibling remains protected. `--owner front`. |
| **Front added initialization** | 0x00418480 | **706/706**, 37 original fields, 21 calls, 19 equal blocks; **222 differences** (236→222) | Receiver and panel-store scheduling; preserve484 matching bytes and all native GameManager/ANM identities. `--owner frontadded`. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions, 34 fields, 9 calls; **218** overlap + 1 missing | Character snapshot and remaining control-flow/byte deficit. Keep the exact Title setup siblings. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280/1,280 bytes**,376/376 instructions,57 original-bound fields,13 ordered calls,72 matching direct blocks; **29 differences** | Menu bank/record chronology. Preserve all57 fields,13 calls,72 blocks and the full original-bound residual guard. `--owner replaymenu`. |
| **TitleScreen::DrawReplaySave** | 0x004239F6 | **800/800 physical**, 245 instructions, 41 fields, 4 calls, 28/28 matching blocks; **96 linked differences** | VM/bank transport and scheduling. Keep245 instructions,41 fields and28 physical blocks. |
| **TitleScreen::DrawResult** | 0x00423D16 | **1,939/1,939**, 82 fields, 16 calls, 92 blocks; **22** differences | Record/bank/currentScreen lifetimes. Existing source/visibility models are bounded negatives; all1917 matching bytes remain protected. `--owner title`. |
| **TitleScreen::OnUpdateOptions** | 0x004276EB | **2,045 / 2,048**, 135 fields, 51 calls, 121 blocks | Options-menu loop and missing bytes; keep complete original-bound field/call/graph checks. `--owner title`. |
| **Title character cursors** | 0x00424EDD / 0x00424FB0 / 0x0042505F | Active target/candidate **211/215**; Reverse/Inactive **175/175** | Active cursor size and cursor state/transport; all three remain nonexact despite Reverse/Inactive having equal physical lengths. `--owner title`. |
| **ExAttack Type3 update** | 0x00442750 | **780/780**, 38 fields, 24 calls, 17 blocks; **15 maintained differences** | Middle-Z chronology; preserve765 matching bytes. Actual Timer visibility and native vector interfaces do not resolve15. `--owner type3`. |
| **ExAttack Type14/22 draw** | 0x00444130 | **959/959**,53 original referents,20 direct +12 COM calls,290/291 instructions; **525 linked differences** | Frame20 versus1C, polar/store scheduling and physical jumps9/10. Preserve434 matching bytes,53 fields and actual scalar/helper payloads. `--owner exattack`. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689/1,689**, 172/171 fields, 35 calls, 78/77 blocks; **904 differing bytes** (911→904, no damaged matches) | Side registration and shared limit/rate publication. Keep native creator calls and164 aligned identities; B4 multiplicity reflects branch sharing. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **804** overlap + 13 missing | Region producer, activeB100 pool and missing physical bytes. Preserve five same-source exact helpers and separate CalcItemBox/FindCollision proofs. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | **1,238/1,238 bytes**, 380 instructions, 53 fields, 23 call operands, 49 matching blocks; **3 differences** (152→3) | Frame-destination register at+B7/+BB and checksum SIB at+3F6. Preserve1235 matching bytes and all12 exact same-source siblings. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Seed/tail chronology and two missing bytes. All32 ordered referents and first30 physical field positions are already restored. `--owner replay`. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | EDI/ESI owner allocation. Preserve310 matching bytes; timer-float visibility regresses physical size and x87 lifetime. |
| **Controller input** | 0x0042B410 | **808 / 796**, 32 fields; **102** overlap differences /12 missing bytes | WinMM midpoint/range CSE and register chronology after the674-byte matching prefix. Preserve all32 fields and native global identities. |
| **Supervisor network HandleMessage** | 0x00432B70 | **768 authored /808 physical bytes**, 46 fields, 17 direct +2 virtual calls, 34 matching blocks; **105 differences** (125→111→105) | Packed-input/frame scheduling. Keep all six switch targets, map bytes and703 matching physical bytes. `--owner netmessage`. |
| **Supervisor network service** | 0x00430AA0 | **1,633 / 1,632**, 108 source fields | Queue ownership and1395 overlapping differences. Keep original referents and exact queue siblings independent. `--owner network`. |
| **Supervisor::OnUpdate** | 0x00431110 | **844 authored / 996 target physical**, 952 candidate physical | Whole-owner physical deficit, startup chronology and remaining GameManager registration alias debt. Keep real Title/Ending symbols and version data. `--owner supervisor`. |
| **ECL MoveRandomBiased leaf** | 0x00407E30 | **516 / 517**, 28 fields, 14 calls, 24 blocks; 148 overlap + 1 extra | Y-pointer LEA versus MOV alias after actual Float3 conversion. Keep complete owner, fields, branches and exact ECL cohort. `--owner runecl`. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Remaining receiver/allocation scheduling. Preserve43 matching bytes and genuine AnmLoaded/Chain interfaces. `--owner effect`. |
| **Supervisor received-frame leaf** | 0x0042E9E0 | **213/213**, 67 instructions, 9 blocks; **35** differences | Packed input/seed16-bit store order. Preserve178 matching physical bytes and both exact queue members. `--owner queue`. |


## Exact landmarks and evidence routing

The manifests govern **1003 exact** functions. Historical Factory receipts
remain in [the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md), with
immutable source-commit bindings; they do not accept a later HEAD.

- ExAttack Type19 update is **821/821 exact**, with35 original fields, all five
  actual float definitions and the full79-unit ECL closure protected.
- Network-config parser is **243/243 exact**, with its complete **679-byte**
  caller protected by `--owner parser` from one shared source/profile carrier.
- The native Timer integer conversion has a complete four-byte source alias
  at 0x435F00; existing TextHelper::GetHeight keeps canonical authored credit.
- Two pointer conversion interfaces now have complete three-byte source aliases
  for physical 0x4343D0. Its origin and unique class ownership stay unknown;
  alias-byte units carry no authored exact credit.
- GameManager CleanupGameplayState is **425/425 exact**, with all 43 fields
  and both final limit publications. Original high-level return intent remains
  unknown; the separate setup worker remains nonexact.
- Etama `OnUpdate` is2,195 authored/2,216 physical bytes exact. Its distinct
 600-byte `OnDraw` remains nonexact. TitleSetup and MusicRoom update/draw,
 ExAttack Type19/21 initializers and Player initialization are exact.
- Effect/Etama added callbacks share the target-proven `AnmLoaded` pointer at
 +0x04. The shared eight-byte view does not make Effect's51-byte callback exact.
- `audit-float3-conversion-calls.py --json` bounds 27 physical callsites;
  folded helper identity does not prove unique C++ authorship or full inventory.
- `probe-player-position-visibility.py` demonstrates real ECL 249/249 exact
  versus standalone 250 bytes. Preserve actual include routing in a replay.
- Historical63-file ECL staging preserved bytes but renumbered two private labels.
  Frozen replay stages the original eight files and hashes the other 55;
  all 598 literal COFF rows reproduce. The KB retains that routing evidence.

## Restart and verification

Read [`AGENTS.md`](../AGENTS.md), [`RE_WORKFLOW.md`](RE_WORKFLOW.md), and
[`ORACLES.md`](ORACLES.md). Inspect staged, unstaged and untracked work first.
When reconstruction resumes, use one affected owner at a time and reuse
unchanged proofs. Historical countermodels are bounded
negative evidence, not a reason to assume an entire source family is exhausted.

```bash
export PYTHONDONTWRITEBYTECODE=1
git status --short --branch
git diff
git diff --cached
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
# Direct local Ghidra: verify/import once or reuse the target-bound project.
python3 -B scripts/ghidra-analysis.py --function 0x004086c0 --function 0x00410730
# Choose one affected owner and reuse unchanged source/profile-bound proofs:
nice -n 10 python3 -B scripts/check-boss-frontiers.py --owner runecl
nice -n 10 python3 -B scripts/check-boss-frontiers.py --owner enemy
# Timer only: the shared alias plus its 16 authored exact siblings:
python3 -B scripts/check-timer-current-view.py
# Final checkpoint gates; CI precedes reviewed artifact cleanup:
python3 -B scripts/validate-tracking.py
python3 -B scripts/validate-docs.py
python3 -B scripts/progress.py --check
python3 -B scripts/ci.py
git diff --check
python3 -B scripts/clean-generated-artifacts.py  # Preview only; do not apply wholesale.
```

Metadata hashes alone do not establish an unmodified Ghidra mapping. Compare
all file-backed mapped bytes with the verified disk target before analysis and
each export; target identity is not exactness credit. Build a canonical unit
afresh before comparing its COFF; missing OBJ/PDB files are expected after cleanup.

For other large owners use the frontier table; for small leaves use
[SMALL_FUNCTION_FRONTIER.md](SMALL_FUNCTION_FRONTIER.md). Historical experiment
recipes and their exclusions remain in the knowledge base and Git history.
A diagnostic command may exit zero with a nonexact result: inspect full-byte
results, fields and deficits before making an exactness claim.

## Evidence and cleanup boundaries

Use [cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) for exact scopes, hashes
and recovery instructions. Original-bound accepted receipts are archived in
[RE_ACCEPTED_RECEIPTS.md](RE_ACCEPTED_RECEIPTS.md) with immutable source commits.
Replay, Enemy and setup-dataflow packets retain their source-bound comparisons,
cleanup/adoption proofs and bounded Oracles. Their identities and storage scopes
are indexed in cleanup history and the knowledge base.

Earlier source-copy archives remain recoverable: read the original packet's
ARCHIVE_README before replay, especially for the October13 RunEcl archive and
October23 command-consumer snapshots. Do not rerun producers over frozen inputs.
Ignored packets must accompany the worktree when handing local evidence over;
tracked source, scripts and KB observations provide the committed restart path.

Keep resources/th09.exe, toolchains, SDKs, Wine prefixes, IDA databases, adopted
source-bound proof objects and unclassified experiments. Build caches may be
absent; rebuild the configured unit/profile before comparison. The generic
cleaner previews a broader scope than the reviewed cleanup: do not apply its
extension-wide .analysis deletion or run git clean -X. No cleanup implies that
a nonexact owner became exact or that the98% milestone was achieved.


## October30 predecessor checkpoint760f01a1 — instruction-mask audit

Preserved before the extended-callback checkpoint condensed the active route.
This is historical state, not a current-source acceptance receipt.

# TH09 reconstruction — active handoff

## Operator status — October 30

The operator explicitly resumed reconstruction after maintenance checkpoint
7bc3b956, with the98% goal active. Audit dependencies, shared declarations, ABI
and layouts as well as the large-function bodies; correct tested discrepancies
without assuming earlier notes are conclusive. The98% milestone is unfinished.

The restart game-source baseline is **cc19b4848a6f3401d66b4034eb37dec6a2f74799**
(Controller axis ranges); maintenance checkpoint7bc3b956 adds no game-source
change. The charge, RunEcl and instruction-mask corrections are described below. Use
live Git status and ledgers before dated measurements. Exactness credit and
compiler profiles remain unchanged. New commits use `gpt-6.1-sol:` and the identity/trailer in AGENTS.md;
local commits do not establish remote publication.

Analysis routing is **direct local Ghidra only**, without IDA, REA or Factory
MCP. The original Japanese v1.50a input is ignored `resources/th09.exe`, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Do not substitute another executable or search Windows mounts.

## Player charge state-effect contract correction

Charge previously declared three implemented state-effect methods with void*
returns; their actual definitions return Effect*, producing different C++ symbols.
Caller and definitions now use one layout-checked PlayerStateEffectView.hpp.
A final source-cold comparison preserves every1197 caller byte,70 fields,58 calls
and all8 nondebug sections/1281 bytes. Only three external method references
change identity. The actual91+91+100-byte callees and existing20-byte Timer body
remain exact. Charge still has1160 overlap differences and13 missing bytes;
there is no new exact credit or product-closure claim.

Ten bounded real-callee/type controls keep actual native bodies checked: effects
and SpawnShots visibility are caller-neutral; resource-body visibility yields
1195 bytes/1162 differences/15 missing and is rejected. Full resource/spawn
declarations alone are neutral. All temporary source trees are removed; records,
recipes and current proof objects remain below
.analysis/gpt-61sol-20261030-charge-native-effects/. verify_current.py replays
that source-bound capture without build caches, or compiles only two affected
carriers with --build. See the KB for the scope and measured limits. Public CI
passes1009 tests/29 skips; ten focused charge tests pass. All10 captured cases
and28 native-body proofs replay before disposable object/debug cleanup; the
packet stays below1MiB with no source snapshots or new source archives.

Enemy's two3-byte callback definitions retain unresolved folded ownership;
matching the PlayerCheckState2 address does not make them that Player method.

## RunEcl native clamp, bullet and Background contracts

RunEcl now calls the existing Enemy clamp and Etama transition members, and the
Background spell method through its shared layout-checked prefix. Three undefined
facades and their diagnostic address aliases are removed. The audit requires the
real canonical members at original call sites409806/40B5B1/40BD13, independently
of candidate offsets. A three-case regression rejects retired names even when
their numeric destinations agree.

Both canonical ECL profiles and three native callee carriers cold-pass all82
existing exact owners:25371 physical bytes/1128 fields. Twenty private reference
names in two units are refreshed only after actual COFF coordinates prove the
same target. Each complete ECL object preserves all203 nondebug sections and
effective relocation identities; only three API and220 private reference strings
change. Complete RunEcl15564/598/30 and Enemy3900/97/13 remain raw-identical.
No definition move, compiler-profile change or new exact credit is adopted.

Eight genuinely connected native-body visibility controls test clamp, bullets,
Background and all three together before/after the caller. Both full callers
remain identical;612 repeated complete exact-body proofs pass. These are bounded
measurements, not proof of original TU ownership. Current source-bound replay is
.analysis/gpt-61sol-20261030-ecl-native-calls/verify_current.py; --build compiles
only five affected carriers. Source staging is temporary; preserve the records. All88
focused ECL tests pass; public CI passes1012 tests/29 skips. Current proof objects
remain after19 disposable object/debug files are removed; no source snapshots or
new code archives remain.

The instruction-mask audit below supersedes the proposed4A7EB0 investigation.
Remaining callback-table and BossLifeMarker data aliases still need independent
storage/definition review; no conclusion follows from address containment alone.

## Shared instruction-mask storage and GameManager global identity

Fresh direct Ghidra and independently decoded original PE accesses confirm
408885 reads DWORD4A7EB0,40FD2A reads its low BYTE, and41AF8A writes DWORD0xFF.
The old ECL and timeline declarations named distinct C++ globals for this same
storage; setup also declared g_GameManager with a competing local-layout type.
All three now use the existing AsciiGameManagerView global root4A7D90 and
layout-checked+120 storage. Setup retains its local view, accessed through local
references. The observed prefix/union does not establish original nominal member
ownership, the complete GameManager class, or whole-product link closure.

Five canonical carrier builds preserve92 existing exact owners:29274 complete
physical bytes/1411 fields, including timeline's complete1048-byte table-bearing
extent and setup's425-byte cleanup. Thirty-nine private names are refreshed only
at proven unchanged COFF coordinates. Ninety-three setup references adopt the
shared native root; timeline's field6C becomes root4A7D90+120, preserving4A7EB0.
All nondebug sections and effective relocation identities remain equal in all
five objects. RunEcl's linked15564 bytes/598 fields/30 differences, Enemy3900/97/13
and Setup1689/171/904 remain unchanged. No new exactness credit is earned.

The ECL audit removes the retired mask alias and requires the native undefined
root, canonical provenance, addend120 and decoded DWORD read. Regression cases
reject equal-address aliases, wrong roots/widths/accesses and missing fields;
the actual prior ECL object is also rejected. Current source/profile/provider-
bound replay is .analysis/gpt-61sol-20261030-ecl-difficulty-data/verify_current.py;
--build compiles only the five affected carriers, serially at lowered priority.
Default replay needs retained proof objects, not build/. Keep documentation,
compiler logs, records and recipes; no old C++ source copies or archives are made.
The unchanged residuals rule out this corrected storage declaration alone as the
explanation for either large owner's remaining bytes.

All92 focused ECL tests and22 focused GameManager/Front tests pass. Public CI
passes1016 tests/29 skips in105.471s. Two obsolete source-spelling assertions
are replaced by original-address/native-relocation checks, rather than making
the new local variable spelling an acceptance rule. Cleanup removes431 ignored
build/debug/bytecode and rejected-getter files,8361802 bytes; all27 research-record
hashes remain unchanged. Cache-free current replay passes after build/ removal.

## Source changes and retained current proofs

All listed improvements remain nonexact where stated. Full code/table/field
comparisons and actual literal definitions govern acceptance; matching fragments
and source-interface cleanup earn no partial exact credit.

| Source checkpoint | Maintained change | Local evidence below `.analysis/` |
| --- | --- | --- |
| 962ddb20 | Replay uses native GameManager mode predicates; 16 affected exact owners retain4,150 bytes/232 fields. | `gpt-61sol-20261029-replay-mode-adoption/verify_current.py` |
| a4a3cd13 | Enemy uses native Front indicator; ECL/Front profiles preserve80 exact owners/25,428 bytes/1,136 fields. | `gpt-61sol-20261029-enemy-periodic-selection/verify_adoption.py` |
| 6bc1377b | Replay uses native RNG; five carriers retain25 exact owners/4,553 bytes/257 fields. | `gpt-61sol-20261029-replay-native-rng/verify_current.py` |
| 015588e0 | Player cancel branches restore five original bytes:849/862 bytes,799 differences and13 missing. Five exact siblings retain1,220 bytes/32 fields. | `gpt-61sol-20261029-collision-workspaces/verify_current.py` |
| cc19b484 | Controller range views restore30 positions without damaging694 prior matches.810/808 bytes,84 differences and2 excess; two helpers retain37+47 exact bytes. | `gpt-61sol-20261029-controller-accumulation/verify_adoption.py` |

These paths locate dated source/profile/provider-bound evidence, not mandatory
restart dependencies. Strict verifiers can reject newer source/ledger inputs or
removed experiment copies. Use tracked build/inspection commands below to
replay the current source; historical captures grant no new acceptance.
This maintenance checkpoint passes public CI:1008 tests/29 skips in127.352s.
The obsolete README Factory/IDA requirement is removed from documentation
validation; canonical-target-path checks remain. Controller and Player proofs
passed after source-copy cleanup, before build-cache removal. Controller's saved
object still replays; Player's verifier needs --build now that build/ is absent.
No new game-source cold replay or exactness credit is claimed. Native
product/runtime remain open.

RunEcl and Enemy remain15564/598/30 and3900/97/13. The October30 bounded fresh
main carrier again preserves all75 same-profile exact owners. Direct Ghidra
exports for both are in `ghidra-direct/ecl-enemy-20261030/`; Pause's export is in
`ghidra-direct/pause-update-20261030/`. No new source experiment or adoption
followed before the operator switched to maintenance.

Recent rejected controls remain recorded in the dated KB and packets:
`gpt-61sol-20261029-effect29-witness/`, `...-replay-byte-addresses/`,
`...-queue-representation/`, `...-chain-callback-context/`, and the Controller
packet above. Some unused frozen source copies are deleted. Their negative
results constrain those concrete models, not all natural source forms; an old
replay may require regenerating its recorded source inputs. Actual COFF, findings,
failures and recipes remain as research records.

## Live ledger snapshot


The canonical state is `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv`, and `config/match-units.toml`, checked by
`python3 scripts/validate-tracking.py --require-target` and
`python3 scripts/validate-docs.py`. The current restart summary is
**1035 source-present / 1003 exact / 32 non-exact**; earlier operator notes
mentioning 979/876/103 are obsolete for this checkout.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,248 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 37 |
| Confirmed authored | 1,035 |
| Classified exclusions | 1,176 |
| Source-present authored mappings | 1,035 |
| Canonical exact functions | 1,003 |
| Source-present non-exact functions | 32 |
| Source-present non-exact bytes | 46,966 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 244,917 |

**244,917 / 291,883 reviewed-authored bytes = 83.9093% exact**. To reach
98% of the *currently classified* denominator, another **41,129 bytes**
must become complete original-bound exact owners, reaching at least **286,046
exact authored bytes**. The 37 origin-unresolved functions are excluded and could change the denominator. Similarity fractions
and locally matching subsequences never count as accepted exact.

The manifests govern **1003 exact** functions.

The catalog is not proof of complete executable-code inventory. The formerly
uncatalogued **0x0040D677 /0x00444A78** Timer references now belong to the exact
Effect 1/2 and Effect 34 updates. All **203** captured assignment callsites have
catalogued owners; that bounded census proves neither complete executable
coverage nor a return ABI. The Effect 34 **178-byte initializer at 0x00444960** is now exact; its
**946-byte draw at 0x00444AF0** is a maintained, explicitly nonexact frontier.
Do not silently treat uncatalogued code as excluded.

## Reconstruction frontiers

These are routing measurements, not exactness credit. The ledgers list all32
nonexact functions. The opcode4 setter/assignment visibility and whole Enemy
storage controls are already measured; consult their actual frozen inputs before
repeating them. Historical assumptions remain hypotheses.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode4 timer transport and opcode155 flag update. Keep all598 fields/tables and complete15564 bytes; actual Timer ABI and RNG ownership are already corrected. `--owner runecl`. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**,97 fields, 69 calls,170 blocks; **13 differing bytes** (29->13) | Draw index2, side/threshold4, descriptor-position copy7. Keep real20-byte Timer greater co-visible, all97 fields and3887 matching physical bytes. |
| **Effect Type29 draw** | 0x00448750 | **2,220 /2,205 bytes**,129 original referents,72 ordered calls,41/41 physical blocks; **1170 overlap differences +15 missing** | Main/satellite fan chronology, polar receiver reloads and ordinal graph. Bounded emulator fixtures are not whole-runtime or exactness proof. `--owner effect29`. |
| **Effect 34 draw** | 0x00444AF0 | **946/946 physical**,53 original referents,20 direct +12 COM calls,286 instructions,16 blocks; **78 linked differences** | Entry65, store order9 and radius operands4 remain. Preserve868 matching bytes, all53 fields, actual float data and full physical extent. `--owner effect34`. |
| **EtamaController::OnDraw** | 0x00413BE0 | **600/600 physical**, 27 fields, 13 calls, 17 blocks; **268** differences | Laser body-VM position cursor. Preserve332 matching bytes and the genuine final DrawBulletLayerEffects member identity. `--owner etama`. |
| **PauseMenu::OnUpdate** | 0x00434740 | **1,776 original physical / 1,732 candidate**; 46 direct calls plus 2/3 original/candidate timestamp calls; 10-way switch; **1,453** overlapping differences + 44 missing | Share timestamp tail for states3/8/9 and recover branch/VM scheduling. Keep real UnPause and existing exact siblings. `--owner pause`. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | Lower-bound EAX/EDX transport and encoding cascade. Native framerate/coordinate owners are corrected; preserve1771 matching bytes and both tables. `--owner movement`. |
| **Player charge update** | 0x0041F810 | **1,210 target /1,197 candidate bytes**, 70 fields, 58 calls, 350/342 instructions; **1,160 overlap differences +13 missing** | Recover source chronology and missing13 bytes; preserve repaired SpawnShots transport and actual call/field identities. |
| **Front weighted message RNG** | 0x004162D0 | **693 target / 681 candidate**, 15 fields, 6 calls, **569** overlap + 12 missing | Primary/opposing table selection and remaining control flow; exact FrontCalc sibling remains protected. `--owner front`. |
| **Front added initialization** | 0x00418480 | **706/706**, 37 original fields, 21 calls, 19 equal blocks; **222 differences** (236→222) | Receiver and panel-store scheduling; preserve484 matching bytes and all native GameManager/ANM identities. `--owner frontadded`. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions, 34 fields, 9 calls; **218** overlap + 1 missing | Character snapshot and remaining control-flow/byte deficit. Keep the exact Title setup siblings. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280/1,280 bytes**,376/376 instructions,57 original-bound fields,13 ordered calls,72 matching direct blocks; **29 differences** | Menu bank/record chronology. Preserve all57 fields,13 calls,72 blocks and the full original-bound residual guard. `--owner replaymenu`. |
| **TitleScreen::DrawReplaySave** | 0x004239F6 | **800/800 physical**, 245 instructions, 41 fields, 4 calls, 28/28 matching blocks; **96 linked differences** | VM/bank transport and scheduling. Keep245 instructions,41 fields and28 physical blocks. |
| **TitleScreen::DrawResult** | 0x00423D16 | **1,939/1,939**, 82 fields, 16 calls, 92 blocks; **22** differences | Record/bank/currentScreen lifetimes. Existing source/visibility models are bounded negatives; all1917 matching bytes remain protected. `--owner title`. |
| **TitleScreen::OnUpdateOptions** | 0x004276EB | **2,045 / 2,048**, 135 fields, 51 calls, 121 blocks | Options-menu loop and missing bytes; keep complete original-bound field/call/graph checks. `--owner title`. |
| **Title character cursors** | 0x00424EDD / 0x00424FB0 / 0x0042505F | Active target/candidate **211/215**; Reverse/Inactive **175/175** | Active cursor size and cursor state/transport; all three remain nonexact despite Reverse/Inactive having equal physical lengths. `--owner title`. |
| **ExAttack Type3 update** | 0x00442750 | **780/780**, 38 fields, 24 calls, 17 blocks; **15 maintained differences** | Middle-Z chronology; preserve765 matching bytes. Actual Timer visibility and native vector interfaces do not resolve15. `--owner type3`. |
| **ExAttack Type14/22 draw** | 0x00444130 | **959/959**,53 original referents,20 direct +12 COM calls,290/291 instructions; **525 linked differences** | Frame20 versus1C, polar/store scheduling and physical jumps9/10. Preserve434 matching bytes,53 fields and actual scalar/helper payloads. `--owner exattack`. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689/1,689**, 172/171 fields, 35 calls, 78/77 blocks; **904 differing bytes** (911→904, no damaged matches) | Side registration and shared limit/rate publication. Keep native creator calls and164 aligned identities; B4 multiplicity reflects branch sharing. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **799** overlap + 13 missing | Region producer, activeB100 pool and missing physical bytes. Preserve five same-source exact helpers and separate CalcItemBox/FindCollision proofs. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | **1,238/1,238 bytes**, 380 instructions, 53 fields, 23 call operands, 49 matching blocks; **3 differences** (152→3) | Frame-destination register at+B7/+BB and checksum SIB at+3F6. Preserve1235 matching bytes and all12 exact same-source siblings. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Seed/tail chronology and two missing bytes. All32 ordered referents and first30 physical field positions are already restored. `--owner replay`. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | EDI/ESI owner allocation. Preserve310 matching bytes; timer-float visibility regresses physical size and x87 lifetime. |
| **Controller input** | 0x0042B410 | **808 / 810**, 32 fields; **84** overlap differences /2 excess bytes | Both midpoint evaluations per axis are restored; register scheduling and two excess bytes remain. Preserve all694 old matches plus30 restored positions,32 fields and native globals. |
| **Supervisor network HandleMessage** | 0x00432B70 | **768 authored /808 physical bytes**, 46 fields, 17 direct +2 virtual calls, 34 matching blocks; **105 differences** (125→111→105) | Packed-input/frame scheduling. Keep all six switch targets, map bytes and703 matching physical bytes. `--owner netmessage`. |
| **Supervisor network service** | 0x00430AA0 | **1,633 / 1,632**, 108 source fields | Queue ownership and1395 overlapping differences. Keep original referents and exact queue siblings independent. `--owner network`. |
| **Supervisor::OnUpdate** | 0x00431110 | **844 authored / 996 target physical**, 952 candidate physical | Whole-owner physical deficit, startup chronology and remaining GameManager registration alias debt. Keep real Title/Ending symbols and version data. `--owner supervisor`. |
| **ECL MoveRandomBiased leaf** | 0x00407E30 | **516 / 517**, 28 fields, 14 calls, 24 blocks; 148 overlap + 1 extra | Y-pointer LEA versus MOV alias after actual Float3 conversion. Keep complete owner, fields, branches and exact ECL cohort. `--owner runecl`. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Remaining receiver/allocation scheduling. Preserve43 matching bytes and genuine AnmLoaded/Chain interfaces. `--owner effect`. |
| **Supervisor received-frame leaf** | 0x0042E9E0 | **213/213**, 67 instructions, 9 blocks; **35** differences | Packed input/seed16-bit store order. Preserve178 matching physical bytes and both exact queue members. `--owner queue`. |

## Restart after a new reconstruction request

Read [AGENTS.md](../AGENTS.md), [RE_WORKFLOW.md](RE_WORKFLOW.md),
[ORACLES.md](ORACLES.md), affected ledger rows and the relevant dated KB entry.
Recover staged, unstaged and untracked work first. Prioritize complete RunEcl
and Enemy, including whole-function dataflow, shared types and real TU boundaries;
small leaves and the other large owners remain in scope.

```bash
export PYTHONDONTWRITEBYTECODE=1
git status --short --branch
git diff
git diff --cached
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
# Analysis: cached direct Ghidra; every mapped file byte is checked.
python3 -B scripts/ghidra-analysis.py --function 0x004086c0 --function 0x00410730
# After relevant source changes, choose the affected owner only:
nice -n 10 python3 -B scripts/check-boss-frontiers.py --owner runecl
# Documentation/status checkpoint checks:
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
python3 -B scripts/progress.py --check
git diff --check
```

Ghidra12.1.4 is reached through `.tools/ghidra`. The runner imports once and
reuses `.analysis/ghidra-direct/243bffea8dcbe99a/th09.gpr` with
`-noanalysis -readOnly`. It checks disk/Program identity, language, entry and all
685056 file-backed mapped bytes before each export. Metadata hashes alone are
insufficient. Types and boundaries remain provisional; preserve reviewed physical
owners, including alignment and tables, for the pinned VC7.1 Oracle.

Build only affected canonical units/profiles and protect complete exact siblings
and callees; reuse unchanged source/profile-bound proofs. Missing `build/` objects
are expected after cleanup: run the configured build before comparing. Full CI
is needed for shared validator/CI changes, not ordinary docs or exact checkpoints.
No generic runtime hardening or product build is required for each exact edit.

## Evidence, archives and cleanup

The October24 stop/Factory session, H01–H66 narratives and older CI counts have
moved out of this active handoff to
[the checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md#october-30-archival-of-the-cc19b484-handoff).
[The October24 hypothesis checklist](TH09_BOSS_HYPOTHESES_20261024.md) is historical;
its old source SHA and queued language do not authorize work.
The records in [the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md) remain immutable source-commit-bound
history, not current-source credit. [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) retains
TH09 facts and measured models. For short owners use
[SMALL_FUNCTION_FRONTIER.md](SMALL_FUNCTION_FRONTIER.md).

October30 cleanup deletes unused experimental C++ copies, compiler debug/bytecode
caches and source-only archives:67,225 files/758,909,554 logical bytes, plus7,349
empty directories. `.analysis/` falls from about2GiB to1.2GiB. Documentation,
JSON/configuration records, logs, patches, COFF evidence and the working Ghidra project remain. No new old-code
archive is retained. `build/` is cleared after validation and can be rebuilt.

Use [cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) for exact retained/deleted
scopes and measured storage savings. Keep needed ignored research records with
this worktree when transferring local evidence. Deleted code snapshots are not
restart prerequisites; rebuild current source with the pinned configuration.

Original executable/game data, toolchains, SDK/Wine/provider state, Ghidra
projects, unique objects and current source-bound proofs are retained. The generic
cleaner previews a broader scope, including retained `.analysis/` objects:
review its paths and do not run `--apply` wholesale or `git clean -X`.

Native Windows i386 compile/link/runtime closure is open. Semantic reconstruction
with both original and native-product runtime Oracles, then portability, comes
later. Exact functions and this cleanup do not establish a runnable product.


## October 30 archival of the cbb6e3d5 handoff

The following is the complete predecessor handoff before the Enemy storage and
folded-callback verification checkpoint. Its current-source wording is scoped to
cbb6e3d527462fa19b3d33f9e0341975c1248438; newer source/provider hashes supersede it.

# TH09 reconstruction — active handoff

## Operator status — October 30

The operator explicitly resumed reconstruction after maintenance checkpoint
7bc3b956, with the98% goal active. Audit dependencies, shared declarations, ABI
and layouts as well as the large-function bodies; correct tested discrepancies
without assuming earlier notes are conclusive. The98% milestone is unfinished.

The restart game-source baseline is **cc19b4848a6f3401d66b4034eb37dec6a2f74799**
(Controller axis ranges); maintenance checkpoint7bc3b956 adds no game-source
change. The charge, RunEcl and instruction-mask corrections are described below. Use
live Git status and ledgers before dated measurements. Exactness credit and
compiler profiles remain unchanged. New commits use `gpt-6.1-sol:` and the identity/trailer in AGENTS.md;
local commits do not establish remote publication.

Analysis routing is **direct local Ghidra only**, without IDA, REA or Factory
MCP. The original Japanese v1.50a input is ignored `resources/th09.exe`, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Do not substitute another executable or search Windows mounts.

## Current checkpoint — common extended-callback contracts

The twelve callable entries at4A0B14 now have one source-callable ECX Enemy /
EDX instruction interface, real definitions and an initialized table in
EnemyExAttackLeaves.cpp. Ten previous no-argument member spellings were compatible
byte models, not proof of original member ownership; two existing free functions
also had different instruction parameter types. All twelve now use the checked
EnemyEclExInstructionView prefix, with only slot1's+10 integer operand named.
The separate copied-context prefix types callback+14 and instruction+18 without
changing the complete context layout. These names/types are target-facing source
contracts; original nominal declarations remain unknown.

The six affected canonical carriers preserve91 exact owners:26545 physical bytes /
1176 fields. All twelve complete callback bodies retain1330 bytes/50 fields.
The48-byte table prefix independently matches all twelve original pointers and
native source identities; the next zero DWORD's ownership and original full array
extent remain unknown. This data comparison grants no authored-function credit.
Six table entries resolve to actual same-object definitions and six to external
native definitions checked in the other carriers. Current strict proof is
.analysis/gpt-61sol-20261030-ecl-global-storage/verify_current.py; default replay
uses retained objects without build/, while --build compiles only six carriers.
The tracked table-only diagnostic is scripts/inspect-ecl-ex-instruction-table.py;
its --build compiles the existing spawn/table owner carrier.

Slot1 supplies a concrete type/codegen control: the shared formal instruction
prefix plus real SoundPlayer retains all76 original bytes. A raw-header formal
parameter converted to a local view gives74 bytes; retaining the old SoundPlayer
facade still gives74, and a word reference gives75. Those alternatives are
rejected. Both callbacks now use actual SoundPlayer declarations; callback2 also
uses the existing g_Rng root, replacing its misleading Replay alias. Twenty local
references are refreshed only at unchanged actual coordinates; six external
references adopt existing native SoundPlayer/RNG identities. Twelve function
source symbols/names are updated consistently across ledgers. Addresses, complete
extents, profiles, origin decisions and exact counts do not change.

Two genuine all-callbacks-before/after TU controls keep full RunEcl15564/598/30 and
Enemy3900/97/13 unchanged but shrink callback slot9 from226 to217 bytes. Its three
receiver reloads disappear with the existing32-byte polar definition visible.
Two controls hiding only that definition restore all226 bytes while22 other ECL
owners regress; the32-byte callee is separately rechecked from the current proof.
Neither TU change is adopted. These are bounded visibility constraints, not proof
of unique original TUs. Current separate carriers retain all91 original matches.

The caller audit requires the implemented table at the two original indexed calls
and installation load, including decoded DWORD read widths. Real prior objects
using the retired facade are rejected despite equal numeric addresses. Five new
regressions reject incomplete/reordered prefixes, wrong canonical targets,
member/void-pointer substitutions and invalid fields. All97 focused ECL tests
pass; public CI passes1021 tests/29 skips in105.228s. No new exactness credit is
earned:98%, native product/runtime, semantic and portability gates remain open.
Cleanup removes454 disposable build/debug/bytecode and control files,11161914
bytes; all57 existing packet record hashes remain unchanged. Twelve current/
baseline proof objects remain. Cache-free replay passes after build/ removal.

## Earlier dependency corrections

Player charge's three state-effect return contracts now use actual Effect*
methods. RunEcl calls actual Enemy clamp, Etama transition and Background spell
members. ECL, timeline and setup share GameManager+120 instruction-mask storage
while retaining original DWORD/BYTE accesses. Detailed evidence is in the KB and
dated packets; the predecessor handoff is preserved in the checkpoint archive.
Older source-bound captures may reject newer global ledger hashes. They are
historical evidence, not dependencies for the current six-carrier replay.

The remaining BossLifeMarkerProtocolValue alias deserves a separate source-storage
correction. Fresh original operands show opcode148 adds1800 at4A8110 and reset
clears manager+380; neither observation establishes boss-life-marker semantics.
The new scan also records a based+380 access in unrelated library code: offset
membership alone is not GameManager identity. Do not resume old rejected source
models without checking their actual inputs and scope.

## Source changes and retained current proofs

All listed improvements remain nonexact where stated. Full code/table/field
comparisons and actual literal definitions govern acceptance; matching fragments
and source-interface cleanup earn no partial exact credit.

| Source checkpoint | Maintained change | Local evidence below `.analysis/` |
| --- | --- | --- |
| e73311d9 | Charge uses actual Effect* state-effect methods;1197/70/1160 plus13 missing remains nonexact. | `gpt-61sol-20261030-charge-native-effects/verify_current.py` |
| 4514216e | RunEcl uses actual Enemy/Etama/Background members; complete RunEcl/Enemy remain30/13 differences. | `gpt-61sol-20261030-ecl-native-calls/verify_current.py` |
| 760f01a1 | Shared GameManager instruction-mask storage;92 existing owners preserve29274 bytes/1411 fields. | `gpt-61sol-20261030-ecl-difficulty-data/verify_current.py` |
| 962ddb20 | Replay uses native GameManager mode predicates; 16 affected exact owners retain4,150 bytes/232 fields. | `gpt-61sol-20261029-replay-mode-adoption/verify_current.py` |
| a4a3cd13 | Enemy uses native Front indicator; ECL/Front profiles preserve80 exact owners/25,428 bytes/1,136 fields. | `gpt-61sol-20261029-enemy-periodic-selection/verify_adoption.py` |
| 6bc1377b | Replay uses native RNG; five carriers retain25 exact owners/4,553 bytes/257 fields. | `gpt-61sol-20261029-replay-native-rng/verify_current.py` |
| 015588e0 | Player cancel branches restore five original bytes:849/862 bytes,799 differences and13 missing. Five exact siblings retain1,220 bytes/32 fields. | `gpt-61sol-20261029-collision-workspaces/verify_current.py` |
| cc19b484 | Controller range views restore30 positions without damaging694 prior matches.810/808 bytes,84 differences and2 excess; two helpers retain37+47 exact bytes. | `gpt-61sol-20261029-controller-accumulation/verify_adoption.py` |

These paths locate dated source/profile/provider-bound evidence, not mandatory
restart dependencies. Strict verifiers can reject newer source/ledger inputs or
removed experiment copies. Use tracked build/inspection commands below to
replay the current source; historical captures grant no new acceptance.
This maintenance checkpoint passes public CI:1008 tests/29 skips in127.352s.
The obsolete README Factory/IDA requirement is removed from documentation
validation; canonical-target-path checks remain. Controller and Player proofs
passed after source-copy cleanup, before build-cache removal. Controller's saved
object still replays; Player's verifier needs --build now that build/ is absent.
No new game-source cold replay or exactness credit is claimed. Native
product/runtime remain open.

RunEcl and Enemy remain15564/598/30 and3900/97/13. The October30 bounded fresh
main carrier again preserves all75 same-profile exact owners. Direct Ghidra
exports for both are in `ghidra-direct/ecl-enemy-20261030/`; Pause's export is in
`ghidra-direct/pause-update-20261030/`. No new source experiment or adoption
followed before the operator switched to maintenance.

Recent rejected controls remain recorded in the dated KB and packets:
`gpt-61sol-20261029-effect29-witness/`, `...-replay-byte-addresses/`,
`...-queue-representation/`, `...-chain-callback-context/`, and the Controller
packet above. Some unused frozen source copies are deleted. Their negative
results constrain those concrete models, not all natural source forms; an old
replay may require regenerating its recorded source inputs. Actual COFF, findings,
failures and recipes remain as research records.

## Live ledger snapshot


The canonical state is `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv`, and `config/match-units.toml`, checked by
`python3 scripts/validate-tracking.py --require-target` and
`python3 scripts/validate-docs.py`. The current restart summary is
**1035 source-present / 1003 exact / 32 non-exact**; earlier operator notes
mentioning 979/876/103 are obsolete for this checkout.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,248 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 37 |
| Confirmed authored | 1,035 |
| Classified exclusions | 1,176 |
| Source-present authored mappings | 1,035 |
| Canonical exact functions | 1,003 |
| Source-present non-exact functions | 32 |
| Source-present non-exact bytes | 46,966 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 244,917 |

**244,917 / 291,883 reviewed-authored bytes = 83.9093% exact**. To reach
98% of the *currently classified* denominator, another **41,129 bytes**
must become complete original-bound exact owners, reaching at least **286,046
exact authored bytes**. The 37 origin-unresolved functions are excluded and could change the denominator. Similarity fractions
and locally matching subsequences never count as accepted exact.

The manifests govern **1003 exact** functions.

The catalog is not proof of complete executable-code inventory. The formerly
uncatalogued **0x0040D677 /0x00444A78** Timer references now belong to the exact
Effect 1/2 and Effect 34 updates. All **203** captured assignment callsites have
catalogued owners; that bounded census proves neither complete executable
coverage nor a return ABI. The Effect 34 **178-byte initializer at 0x00444960** is now exact; its
**946-byte draw at 0x00444AF0** is a maintained, explicitly nonexact frontier.
Do not silently treat uncatalogued code as excluded.

## Reconstruction frontiers

These are routing measurements, not exactness credit. The ledgers list all32
nonexact functions. The opcode4 setter/assignment visibility and whole Enemy
storage controls are already measured; consult their actual frozen inputs before
repeating them. Historical assumptions remain hypotheses.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode4 timer transport and opcode155 flag update. Keep all598 fields/tables and complete15564 bytes; actual Timer ABI and RNG ownership are already corrected. `--owner runecl`. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**,97 fields, 69 calls,170 blocks; **13 differing bytes** (29->13) | Draw index2, side/threshold4, descriptor-position copy7. Keep real20-byte Timer greater co-visible, all97 fields and3887 matching physical bytes. |
| **Effect Type29 draw** | 0x00448750 | **2,220 /2,205 bytes**,129 original referents,72 ordered calls,41/41 physical blocks; **1170 overlap differences +15 missing** | Main/satellite fan chronology, polar receiver reloads and ordinal graph. Bounded emulator fixtures are not whole-runtime or exactness proof. `--owner effect29`. |
| **Effect 34 draw** | 0x00444AF0 | **946/946 physical**,53 original referents,20 direct +12 COM calls,286 instructions,16 blocks; **78 linked differences** | Entry65, store order9 and radius operands4 remain. Preserve868 matching bytes, all53 fields, actual float data and full physical extent. `--owner effect34`. |
| **EtamaController::OnDraw** | 0x00413BE0 | **600/600 physical**, 27 fields, 13 calls, 17 blocks; **268** differences | Laser body-VM position cursor. Preserve332 matching bytes and the genuine final DrawBulletLayerEffects member identity. `--owner etama`. |
| **PauseMenu::OnUpdate** | 0x00434740 | **1,776 original physical / 1,732 candidate**; 46 direct calls plus 2/3 original/candidate timestamp calls; 10-way switch; **1,453** overlapping differences + 44 missing | Share timestamp tail for states3/8/9 and recover branch/VM scheduling. Keep real UnPause and existing exact siblings. `--owner pause`. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | Lower-bound EAX/EDX transport and encoding cascade. Native framerate/coordinate owners are corrected; preserve1771 matching bytes and both tables. `--owner movement`. |
| **Player charge update** | 0x0041F810 | **1,210 target /1,197 candidate bytes**, 70 fields, 58 calls, 350/342 instructions; **1,160 overlap differences +13 missing** | Recover source chronology and missing13 bytes; preserve repaired SpawnShots transport and actual call/field identities. |
| **Front weighted message RNG** | 0x004162D0 | **693 target / 681 candidate**, 15 fields, 6 calls, **569** overlap + 12 missing | Primary/opposing table selection and remaining control flow; exact FrontCalc sibling remains protected. `--owner front`. |
| **Front added initialization** | 0x00418480 | **706/706**, 37 original fields, 21 calls, 19 equal blocks; **222 differences** (236→222) | Receiver and panel-store scheduling; preserve484 matching bytes and all native GameManager/ANM identities. `--owner frontadded`. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions, 34 fields, 9 calls; **218** overlap + 1 missing | Character snapshot and remaining control-flow/byte deficit. Keep the exact Title setup siblings. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280/1,280 bytes**,376/376 instructions,57 original-bound fields,13 ordered calls,72 matching direct blocks; **29 differences** | Menu bank/record chronology. Preserve all57 fields,13 calls,72 blocks and the full original-bound residual guard. `--owner replaymenu`. |
| **TitleScreen::DrawReplaySave** | 0x004239F6 | **800/800 physical**, 245 instructions, 41 fields, 4 calls, 28/28 matching blocks; **96 linked differences** | VM/bank transport and scheduling. Keep245 instructions,41 fields and28 physical blocks. |
| **TitleScreen::DrawResult** | 0x00423D16 | **1,939/1,939**, 82 fields, 16 calls, 92 blocks; **22** differences | Record/bank/currentScreen lifetimes. Existing source/visibility models are bounded negatives; all1917 matching bytes remain protected. `--owner title`. |
| **TitleScreen::OnUpdateOptions** | 0x004276EB | **2,045 / 2,048**, 135 fields, 51 calls, 121 blocks | Options-menu loop and missing bytes; keep complete original-bound field/call/graph checks. `--owner title`. |
| **Title character cursors** | 0x00424EDD / 0x00424FB0 / 0x0042505F | Active target/candidate **211/215**; Reverse/Inactive **175/175** | Active cursor size and cursor state/transport; all three remain nonexact despite Reverse/Inactive having equal physical lengths. `--owner title`. |
| **ExAttack Type3 update** | 0x00442750 | **780/780**, 38 fields, 24 calls, 17 blocks; **15 maintained differences** | Middle-Z chronology; preserve765 matching bytes. Actual Timer visibility and native vector interfaces do not resolve15. `--owner type3`. |
| **ExAttack Type14/22 draw** | 0x00444130 | **959/959**,53 original referents,20 direct +12 COM calls,290/291 instructions; **525 linked differences** | Frame20 versus1C, polar/store scheduling and physical jumps9/10. Preserve434 matching bytes,53 fields and actual scalar/helper payloads. `--owner exattack`. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689/1,689**, 172/171 fields, 35 calls, 78/77 blocks; **904 differing bytes** (911→904, no damaged matches) | Side registration and shared limit/rate publication. Keep native creator calls and164 aligned identities; B4 multiplicity reflects branch sharing. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **799** overlap + 13 missing | Region producer, activeB100 pool and missing physical bytes. Preserve five same-source exact helpers and separate CalcItemBox/FindCollision proofs. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | **1,238/1,238 bytes**, 380 instructions, 53 fields, 23 call operands, 49 matching blocks; **3 differences** (152→3) | Frame-destination register at+B7/+BB and checksum SIB at+3F6. Preserve1235 matching bytes and all12 exact same-source siblings. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Seed/tail chronology and two missing bytes. All32 ordered referents and first30 physical field positions are already restored. `--owner replay`. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | EDI/ESI owner allocation. Preserve310 matching bytes; timer-float visibility regresses physical size and x87 lifetime. |
| **Controller input** | 0x0042B410 | **808 / 810**, 32 fields; **84** overlap differences /2 excess bytes | Both midpoint evaluations per axis are restored; register scheduling and two excess bytes remain. Preserve all694 old matches plus30 restored positions,32 fields and native globals. |
| **Supervisor network HandleMessage** | 0x00432B70 | **768 authored /808 physical bytes**, 46 fields, 17 direct +2 virtual calls, 34 matching blocks; **105 differences** (125→111→105) | Packed-input/frame scheduling. Keep all six switch targets, map bytes and703 matching physical bytes. `--owner netmessage`. |
| **Supervisor network service** | 0x00430AA0 | **1,633 / 1,632**, 108 source fields | Queue ownership and1395 overlapping differences. Keep original referents and exact queue siblings independent. `--owner network`. |
| **Supervisor::OnUpdate** | 0x00431110 | **844 authored / 996 target physical**, 952 candidate physical | Whole-owner physical deficit, startup chronology and remaining GameManager registration alias debt. Keep real Title/Ending symbols and version data. `--owner supervisor`. |
| **ECL MoveRandomBiased leaf** | 0x00407E30 | **516 / 517**, 28 fields, 14 calls, 24 blocks; 148 overlap + 1 extra | Y-pointer LEA versus MOV alias after actual Float3 conversion. Keep complete owner, fields, branches and exact ECL cohort. `--owner runecl`. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Remaining receiver/allocation scheduling. Preserve43 matching bytes and genuine AnmLoaded/Chain interfaces. `--owner effect`. |
| **Supervisor received-frame leaf** | 0x0042E9E0 | **213/213**, 67 instructions, 9 blocks; **35** differences | Packed input/seed16-bit store order. Preserve178 matching physical bytes and both exact queue members. `--owner queue`. |

## Restart after a new reconstruction request

Read [AGENTS.md](../AGENTS.md), [RE_WORKFLOW.md](RE_WORKFLOW.md),
[ORACLES.md](ORACLES.md), affected ledger rows and the relevant dated KB entry.
Recover staged, unstaged and untracked work first. Prioritize complete RunEcl
and Enemy, including whole-function dataflow, shared types and real TU boundaries;
small leaves and the other large owners remain in scope.

```bash
export PYTHONDONTWRITEBYTECODE=1
git status --short --branch
git diff
git diff --cached
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
# Analysis: cached direct Ghidra; every mapped file byte is checked.
python3 -B scripts/ghidra-analysis.py --function 0x004086c0 --function 0x00410730
# After relevant source changes, choose the affected owner only:
nice -n 10 python3 -B scripts/check-boss-frontiers.py --owner runecl
# Documentation/status checkpoint checks:
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
python3 -B scripts/progress.py --check
git diff --check
```

Ghidra12.1.4 is reached through `.tools/ghidra`. The runner imports once and
reuses `.analysis/ghidra-direct/243bffea8dcbe99a/th09.gpr` with
`-noanalysis -readOnly`. It checks disk/Program identity, language, entry and all
685056 file-backed mapped bytes before each export. Metadata hashes alone are
insufficient. Types and boundaries remain provisional; preserve reviewed physical
owners, including alignment and tables, for the pinned VC7.1 Oracle.

Build only affected canonical units/profiles and protect complete exact siblings
and callees; reuse unchanged source/profile-bound proofs. Missing `build/` objects
are expected after cleanup: run the configured build before comparing. Full CI
is needed for shared validator/CI changes, not ordinary docs or exact checkpoints.
No generic runtime hardening or product build is required for each exact edit.

## Evidence, archives and cleanup

The October24 stop/Factory session, H01–H66 narratives and older CI counts have
moved out of this active handoff to
[the checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md#october-30-archival-of-the-cc19b484-handoff).
[The October24 hypothesis checklist](TH09_BOSS_HYPOTHESES_20261024.md) is historical;
its old source SHA and queued language do not authorize work.
The records in [the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md) remain immutable source-commit-bound
history, not current-source credit. [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) retains
TH09 facts and measured models. For short owners use
[SMALL_FUNCTION_FRONTIER.md](SMALL_FUNCTION_FRONTIER.md).

October30 cleanup deletes unused experimental C++ copies, compiler debug/bytecode
caches and source-only archives:67,225 files/758,909,554 logical bytes, plus7,349
empty directories. `.analysis/` falls from about2GiB to1.2GiB. Documentation,
JSON/configuration records, logs, patches, COFF evidence and the working Ghidra project remain. No new old-code
archive is retained. `build/` is cleared after validation and can be rebuilt.

Use [cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) for exact retained/deleted
scopes and measured storage savings. Keep needed ignored research records with
this worktree when transferring local evidence. Deleted code snapshots are not
restart prerequisites; rebuild current source with the pinned configuration.

Original executable/game data, toolchains, SDK/Wine/provider state, Ghidra
projects, unique objects and current source-bound proofs are retained. The generic
cleaner previews a broader scope, including retained `.analysis/` objects:
review its paths and do not run `--apply` wholesale or `git clean -X`.

Native Windows i386 compile/link/runtime closure is open. Semantic reconstruction
with both original and native-product runtime Oracles, then portability, comes
later. Exact functions and this cleanup do not establish a runnable product.


## October 30 archival of the 8866e604 Enemy checkpoint

These superseded current-checkpoint paragraphs are scoped to
8866e604cbdc72c4992ff30a5e1a72b5990a2f7b. Unchanged routing/ledger sections remain
in the active handoff and Git history; this archive preserves the replaced detail.

## Current checkpoint — Enemy storage and actual callback definitions

EnemyManagerCoreView now exposes the two existing ECL manager prefixes and all129
constructed enemy records, matching the maintained native manager view. Its
update loop still processes128 records. A fresh original-code full-pool path
through both SpawnEnemy and SpawnEnemyRecord reads exactly128 active flags,
returns manager+2A6F58 (record index128), and writes only failure flag+2AC42C=1.
The extra returned record is not accessed on that bounded path. The original
constructor independently supplies count129 and stride5430. Initial ECL failure
also sets the same flag while returning an earlier record; the two private spawn
views now name it spawnFailure rather than spawnOverflow. Original nominal
array/sentinel types and later caller behavior remain unproven. This fixture is
not whole-game or native-product runtime validation.

Six canonical carrier/profile builds preserve85 existing complete exact owners:
27307 physical bytes/1201 fields, with actual literal definitions checked. Two
private label names change only at identical actual coordinates. Full Enemy
3900/97/13 and RunEcl15564/598/30 remain raw-identical; there is no new exactness
credit. Current source/profile/provider-bound replay is
.analysis/gpt-61sol-20261030-enemy-formal-storage/verify_current.py. Default replay
uses seven retained current/baseline objects without build/; --build compiles only
ECL main/Ob0, lifecycle main/Ob0 and the two spawn carriers. Older callback/table
proofs remain dated; their six-carrier source binding legitimately rejects this
newer Core view and inspector.

The Enemy inspector now independently verifies the actual three-byte native life
and timer callback definitions at the shared physical4AB20 leaf. It no longer
borrows Player's canonical unit as proof of those Enemy bodies. Real external
calls require an explicit companion COFF via --folded-callback-object; changed or
missing bodies are rejected. A source call can move in a nonexact diagnostic;
its actual source offset and independently checked original call offset are
reported separately, and full operands/bytes still determine equality. Original
nominal ownership remains unknown and no extra function credit is assigned.

Six complete storage/formal-interface controls, four compiling field-type
controls, three genuine member-call controls and four complete28-byte initializer
controls all retain Enemy's13 differences and75 same-profile exact siblings. Two
unsigned-threshold controls fail the existing ZunTimer overload contract; a void
initializer gives26 rather than28 bytes and is rejected before scoring a caller.
Hiding the actual zero life callback changes13 to84 differences, losing71 old
matches and restoring none of the old13; hiding only timer leaves13. All three
opacity controls retain complete original three-byte companion bodies. These
results constrain those particular models, not every possible original source
or TU. Only the tested storage correction is adopted; rejected source copies and
compiler caches are removed after preserving research records.

Public CI passes1026 tests/29 skips in100.181s. Cleanup removes494 disposable
build/debug/cache and fully recorded control files,19610551 logical bytes
(about18.7MiB). All87 preexisting packet record/recipe/log hashes and seven
retained proof-object hashes remain unchanged. build/ is absent; current replay
passes after cleanup. No old source tree or new code archive is retained. The98%
milestone, native product/runtime, semantic and portability gates remain open.
Direct Ghidra, target attestation and pinned compiler profiles are unchanged.

The remaining BossLifeMarkerProtocolValue alias deserves a separate source-storage
correction. Fresh original operands show opcode148 adds1800 at4A8110 and reset
clears manager+380; neither observation establishes boss-life-marker semantics.
The new scan also records a based+380 access in unrelated library code: offset
membership alone is not GameManager identity. Do not resume old rejected source
models without checking their actual inputs and scope.


## October 30 archival of the f74444dc shared-storage checkpoint

These replaced checkpoint paragraphs are scoped to
f74444dc14a5b757515a923f7d49280b84ce87fb. Unchanged routing and ledgers remain
in the active handoff and Git history.

## Current checkpoint — shared GameManager ECL storage

Opcode148 now adds1800 through the real g_GameManager root+380, replacing the
independent BossLifeMarkerProtocolValue alias. Its enum name is the neutral
ADD_GAME_MANAGER_VALUE380; the original writes establish no boss-life-marker
meaning. ResetGameManager now uses the same field projection. The existing
instruction-mask projection is consolidated into GameManagerEclStateView.hpp,
covering+120 DWORD/BYTE accesses and+380 DWORD storage; the old single-purpose
header is removed. Original nominal types, gameplay meaning of+380 and full
GameManager/native-product storage remain unproven.

Independent original evidence binds opcode table148 to40B2F1's DWORD ADD1800 at
4A8110. Complete exact ResetGameManager100 bytes clears receiver+380, while its
complete425-byte cleanup caller loads ECX=4A7D90 and calls the actual reset owner.
The matching native COFF names that same global and real callee. This closes the
storage identity without treating arbitrary based+380 accesses as GameManager.
Direct Ghidra checks all685056 mapped file bytes and exports RunEcl, reset, main
update and cleanup; inferred names/types remain provisional.

Five canonical carrier/profile builds preserve92 existing exact owners:
29274 physical bytes/1411 fields, including actual literal definitions. All92
raw bodies remain baseline-identical. Thirty-nine private label names in three
units are refreshed only at identical actual coordinates. RunEcl's only two raw
byte changes encode the new+380 relocation addend; its fully resolved15564 bytes
remain identical, with598 fields and30 ordinary differences. Enemy3900/97/13
and gameplay setup1689/904 remain unchanged. No new exact credit is earned.

The caller audit independently requires the real undefined GameManager symbol,
+380 addend, original target, DWORD read/write access, ADD operation and immediate
1800. The real previous object with the old alias is rejected despite its equal
numeric address. Actual native COFF copies with a wrong add constant or member
are also rejected. Four new regressions cover root/provenance, missing/duplicate
fields, operand widths, access and constants. All101 focused ECL tests pass;
public CI passes1030 tests/29 skips in98.917s.

Current source/profile/provider-bound replay is
.analysis/gpt-61sol-20261030-ecl-manager380/verify_current.py; default replay uses
six retained current/baseline objects without build/, and --build compiles only
ECL main/Ob0, timeline and GameManager setup Os/O2. verify_storage.py independently
rechecks original opcode/reset/caller dataflow and complete native reset/caller
bodies. Older source-bound captures remain historical and can reject the new
header, symbols and global metadata. Their removed code copies are not restart
dependencies. Previously measured input-type and value-liveness controls do not
justify repeating identical opcode4/155 models after a byte-neutral storage fix.

Cleanup removes437 unused baseline/debug/cache/build files,8681243 logical
bytes (about8.3MiB). All34 preexisting packet records/recipes/logs and six retained
proof objects keep their original hashes. build/ is absent and the current
cache-free replay passes after deletion. No old source tree or new code archive
is retained. The98% milestone and native product/runtime, semantic and
portability gates remain open.


## October30 shared mode/root checkpoint, source8d05fa6a

## Current checkpoint — shared mode/global declarations

A real VC7.1 header combination reproduces C2371: the former GameManagerModeView
and existing AsciiGameManagerView declared the same global through incompatible
C++ types. The mode, Front, Replay and Ending consumers now use the existing
AsciiGameManagerView root and four actual mode-method definitions. The old mode
header and FrontDraw's empty method facade are removed. Side character fields
remain at20/58; addresses, method ABIs and compiler profiles stay fixed. Other
subsystems still have their own legacy global projections; this is a bounded
family correction, not complete GameManager or native-product closure.

GameManagerRuntimeView.hpp holds the shared data/method contract without ANM
rendering declarations. AsciiGameManagerView.hpp remains the renderer gateway.
Float2/Float3 declarations move verbatim into Float3.hpp; EndingAdded removes its
duplicate Float3 class. This avoids importing the rendering AnmVm into Ending's
separate lifecycle view. That broader AnmVm type debt is not claimed resolved.
The vector extraction has a byte-exact inverse; no math implementation changes.

Fifty-seven affected canonical source/profile groups preserve217 existing exact
owners:71337 physical bytes/3959 fields, with actual literal payloads checked.
Four extra nonexact carriers are also compiled. Across all61 groups,506 emitted
body/profile comparisons retain every raw byte and field identity, allowing only
the explicit class migration and actual identical owner-local coordinates.
Supplemental comparison covers15 aux-less compiler entry symbols across11
whole COMDATs,350 bytes/12 fields, including actual EH metadata bindings. Those
compiled contributions are unchanged and receive no authored credit. There are
no new emitted definitions. Four method names are reconciled across ledgers;367
private names in12 units change only at proven identical coordinates.
All217 complete existing exact bodies remain original-bound. No full1003-owner
cold replay is run and no new authored exactness credit is earned.

The newly coherent ECL+mode source control retains75 canonical ECL-profile
owners and four complete native companion bodies. The latter use ECL's existing
/Oi profile in this control; their canonical proofs remain under the separate
unchanged mode profile. RunEcl15564/598/30 and Enemy3900/97/13 remain raw-identical.
This single control proves neither original TU ownership nor all possible forms.
Standalone SaveReplay1238/53/3, playback538/39+2 missing, Front added706/222,
weighted Front681/569+12 missing and Title result1939/22 are also unchanged.

Updated diagnostics require the actual shared native symbols; the real old
Replay object is rejected despite identical numeric destinations. Public CI
passes1030 tests/29 skips in103.641s. Original Ghidra exports all four mode leaves
after checking685056 mapped file bytes. Earlier Effect export failures remain
recorded; target identity passes and no original or Program byte is changed.

Current source/profile/provider-bound replay is
.analysis/gpt-61sol-20261030-game-manager-mode-owner/verify_current.py. Default
replay checks217 owners,506 bodies,11 compiler contributions and seven complete
large-owner diagnostics;
--build serially rebuilds only these61 affected carriers. The dated baseline is
bound to f74444dc, including19 Git/worktree EOL conversions across155 inputs.
Older proofs can legitimately reject the new headers, symbols or ledger hashes.
Use tracked source and current commands; removed old code copies are not restart
dependencies. Cleanup removes611 unused baseline/control/debug/cache/build files,
18256593 logical bytes (about17.4MiB). All284 packet records and62 necessary
proof objects retain their hashes. build/ is absent and current cache-free replay
passes after deletion. No old C++ source copy or new code archive remains. The98%
milestone and native product/runtime, semantic and portability gates remain open.

## October30 native ExAttack side checkpoint, source17a11669

## Current checkpoint — native ExAttack side/EffectManager contracts

Type6/7/8/9 initializers now call the existing real side lookup and dynamic-data
allocator, replacing eight references through unimplemented class facades.
GetSideState uses the shared AsciiGameManagerView global. Its12-byte original
returns the side indexed by record+08, stride38; the original initializer then
reads side+0C and owner+2D4. Original GameManager setup stores the result of
EffectManager::Create in that same side slot. The shared side now declares
EffectManager*, and callers use its real effectAnm member. Six redundant structs
and one unused alias are removed. Other legacy GameManager/AnmVm views remain
independent source/product debt; no full nominal-type or product closure claim.

Fifty-nine canonical source/profile groups preserve219 existing whole exact
owners:71430 physical bytes/3963 fields with actual literal payloads checked.
Four nonexact carriers bring the scope to63. All508 regular body/profile
comparisons and11 compiler COMDATs/350 bytes/12 fields/15 entries retain their
raw bytes and identities. Nine native symbol references change, plus326 private
names in eight units at actual identical coordinates. All manifest offsets,
kinds, addends, targets, extents and profiles stay fixed. The first overly strict
private-name assertion is recorded; no byte or coordinate guard is relaxed.
The actual old side-root and Replay mode objects are rejected despite equal
numeric addresses. No new authored exact credit and no full1003-owner cold replay.

Genuine ECL+side and ECL+side+allocator source controls preserve76/77 exact
bodies,22475/22556 bytes and991/994 fields. Four/eight native calls bind their
actual same-object definitions; all138 preexisting regular ECL bodies remain
byte/field neutral. These finite controls do not identify original TUs. Complete
RunEcl15564/598/30, Enemy3900/97/13 and biased movement517/516 with28 fields/148
overlap differences plus1 extra remain unchanged. SaveReplay, playback, Front
and Title complete diagnostics also retain their previous frontiers.

Four separate RunEcl formal-type controls and two movement-helper formal controls
preserve75 existing ECL siblings per model. State-pointer/direct/void* controls
retain the original30 differences; Enemy& regresses to2153, including24 table
bytes. Complete opcode157 changes160 to158 bytes and shifts six easing plus18
opcode roots by-2; none of the original30 positions is repaired. Both movement
formals retain the517-byte candidate. These are bounded compiler observations,
not original nominal-type proof, and none is adopted.

Current source/profile/provider-bound replay is
.analysis/gpt-61sol-20261030-runecl-formal-state/native_side.py verify;
its build mode serially rebuilds only63 affected carriers. Older packet verifiers
are dated evidence and may reject newer source/ledgers. Existing Git plus saved
recipes reproduce all six private input hash sets without retained old C++
source copies. Ghidra reuses the working project and checks685056 mapped file
bytes before each export. Fourteen focused target-independent tests pass.
Cleanup removes220 superseded binary/debug/cache files,12349457 logical bytes
(about11.8MiB), preserving documentation/research and65 necessary current proof
or actual-counterexample objects. No new code archive is created; build/ is
absent. The98% milestone and later phase gates remain open.


## October 30: superseded attached-effect and receiver-control handoff

The following handoff is historical, superseded by the native Supervisor D3D
prefix and literal audit. Its receipts remain bound to their recorded inputs.

## Current game-source checkpoint — 4dd2212d

ECL, Enemy Core and release consumers now share the observed0x68-byte state at
Enemy+53B4:24 Effect* slots, signed count+60 and float distance+64. The source
reuses the existing Effect interface; original C++ storage/nominal types remain
unproved. Core's unused2A4-byte effect facade/method is removed. Unreviewed+541C
becomes four opaque bytes, without asserting its original role. No runtime
branch, bound, allocation or field offset changes. Original opcode128 establishes
the distance write and count increment; additional gameplay meaning stays open.

Six affected canonical source/profile carriers preserve84 existing exact owners,
26006 physical bytes/1146 fields and actual literals. All282 emitted regular
body/profile comparisons and two whole compiler COMDATs/76 bytes/zero fields
keep their raw bytes and identities. Twenty private names in two units change
only at actual identical coordinates; every other manifest value/profile stays.
Another140 existing proofs are reused from61 unaffected carriers after current
source/profile/object/provider and unchanged unit-metadata checks. No new exact
credit or full1003-owner cold replay. Complete RunEcl15564/598/30,
Enemy3900/97/13 and biased517/516 with28 fields/148 overlaps remain unchanged.

The already-documented private EDI entry of the151-byte attached-effect update
is rechecked at the actual original caller/callee. A genuine ordinary external
companion emits152 bytes and fails that complete native proof before any Enemy
score or target binding. The74 other canonical ECL bodies pass and RunEcl stays30.
An invalid qualified-declaration scaffold and a duplicate Timer-body scaffold
are recorded separately from the corrected single-definition result. No public
fastcall ABI, register hint or unique original TU is inferred.

Current source/profile/provider-bound replay is
.analysis/gpt-61sol-20261030-enemy-dependency-review/family.py verify;
build mode serially rebuilds only these six carriers. Existing Git17a11669 plus
recorded EOL rules reproduces79 baseline input hashes and the private proposal
without retained old C++ copies. Ghidra checks685056 mapped file bytes while
reusing the working project. Thirty-three focused tests pass. Dated older
verifiers may reject newer headers/ledgers; they are evidence, not restart
requirements. Cleanup deletes24 replaced baseline/control/debug/cache files,
3011745 logical bytes(about2.9MiB); all314 protected files and six current
objects retain their hashes. Documentation/research stay and no code archive
is created; build/ is absent.
The98% milestone and later product/runtime/semantic/port gates remain open.

## Latest review — receiver and Replay record controls

Game source remains4dd2212d. A const RunEcl receiver preserves all138 emitted
ECL body/field payloads,75 existing exact siblings and complete RunEcl30/Enemy13.
Original entry tracing confirms ECX is overwritten before use; original C++
constness/ownership is still unproved. No qualifier is adopted.

A genuine Replay header/payload member composition preserves12 authored exact
owners and both15/18-byte unknown-origin constructor bodies. Read-only manager
and whole-record memcpy controls are also neutral. SaveReplay stays1238/53/3
atB7,BB,3F6 and playback538/540 with39 overlaps plus2 missing. Across six control
objects,109 repeated complete comparisons protect87 unique authored units plus
two unknown-origin constructors; repeated bytes receive no new credit.

The source/provider-bound observations and recipes are in
.analysis/gpt-61sol-20261030-runecl-receiver/. Existing Git/EOL rules reproduce83
inputs and all candidate hashes without old C++ copies. Three stale-input guard
checks pass. Cleanup deletes12 fully recorded control/debug files,1118106
logical bytes(about1.1MiB), retaining all40 records and current canonical proofs.
No code archive is created; the98% goal is unfinished.

## October 30: superseded Supervisor D3D and original-literal handoff

Historical source4d83270e. The following replay is source/provider scoped;
current menu readers and its one affected carrier are described in the active
handoff. The remaining105 carrier proofs are reused after per-carrier checks.

## Current game source — native Supervisor D3D and original literals

This checkpoint supersedes game source4dd2212d. The canonical Supervisor now
exposes the observed IDirect3D8* at+4 and IDirect3DDevice8* at+8. Direct3D creation,
device setup, Effect34, ExAttack14/22 and TextHelper use that declaration.
Four redundant local views are removed. The startup helper's former private
`g_Supervisor` type produced a different native C++ global identity; its operand
now names the canonical global. Offsets, allocation, branches and SDK calls stay
unchanged. Original nominal C++ types and complete native linkage remain open.

A wider dependency audit finds two real old source errors: MIDI's English error
message and Ending's truncated ASCII error prefix differ from original CP932
literals. Both full original payloads, including MIDI's observed control bytes,
are restored. Historical instruction-only matches did not establish those data
payloads. The standard match-unit Oracle now checks actual read-only COFF
literal definitions against every original referent, including four/eight-byte
scalars and bounded narrow/UTF-16 strings. Matching symbol names alone cannot
pass this check.

The106 affected source/profile carriers protect **439 canonical authored exact
owners /121391 physical bytes /6843 fields**, plus three unknown-origin units
and one folded-reader alias. All443 configured comparisons total121489 bytes/
6851 fields; the extra units and repeated profiles earn no new credit. All857
emitted regular body/profile comparisons preserve code bytes;32 compiler COMDATs
retain962 bytes/49 fields/49 entry symbols. Three reviewed external/literal
identities and124 position-proven private fields are the only manifest changes.
The standard gate also rejects three actual old COFF objects: the false startup
global identity and both wrong literal payloads. All original sizes, target
addresses, profiles and match rows remain unchanged.

Current replay is `.analysis/gpt-61sol-20261030-supervisor-device/device.py verify`;
`build` serially rebuilds the106 carriers. `canonical_gate.py` runs the standard
Oracle and the three real negatives. Current verification uses maintained tools,
local packet helpers and current objects, without historical C++ copies.
Another564 canonical rows have unchanged source/profile/unit inputs; this batch
does not claim a new compile or literal audit for them. Earlier replay commands
below are dated evidence and may reject the newer shared header or Oracle.

Complete RunEcl15564/598/30, Enemy3900/97/13, biased517/516 with28 fields/148
overlaps and Effect34 draw946/53/78 remain unchanged. Exact credit stays1003;
the98% milestone and later product/runtime/semantic/port gates remain open.

## Latest review and cleanup

Direct cached Ghidra checks all685056 file-backed bytes before exporting the
Effect34 draw/angle helper and Direct3D creation/device setup. Draw's ignored
angle-normalization return is original behavior. An initial combined export
fails because the initializer has no Ghidra function; successful follow-up
exports and the failed request are both retained. Ghidra's provisional draw
body excludes three internal alignment bytes; the complete946-byte Oracle
comparison still includes them.

The11 focused literal tests and full public CI pass:1036 tests/29 skipped.
Actual old-source failures, complete positive comparisons, payload facts,
rejected intermediate captures,
recipes and cleanup hashes remain in the packet. Documentation and research
records are preserved; disposable baselines, duplicate proposals and compiler
PDB/cache files are removed after terminal checks. Cleanup deletes385 files/
19811875 logical bytes(about18.9MiB); all431 protected files retain their hashes.
Current106 objects and three actual negatives remain, and build/ is absent.
No new code archive is made.

## October 30: superseded Pause transition review

Historical review at03462e4f. Game source remains at that checkpoint; the active
large-function review now covers RunEcl byte/word input contracts.

## Latest large-function review

Fresh cached direct Ghidra exports Enemy410730 and Pause434740 after checking
all685056 mapped file bytes. Enemy's complete3900/97/13 result is rechecked;
prior constructor/periodic controls are not assumed to prove unique source/TU
ownership. No new Enemy model is adopted. RunEcl15564/598/30 and the other
unchanged frontiers retain their source-bound proofs.

Six complete Pause controls test result-first state9, an explicit shared clock,
confirm-frame reset chronology and native Supervisor readers. Result-first
alone reaches the original two-call state-sharing pattern and two-byte tail
alignment, with106 fields, but still1732 bytes/1459 overlap differences/44
missing. It restores48 old mismatches and damages54 old matches; no branch
reordering is adopted. The explicit clock label returns to the existing
three-call/1453 result. Confirm reset order is raw-neutral in both contexts;
canonical readers are likewise neutral in each context. These finite models
do not establish exhaustion or the original source spelling.

All six controls preserve the11 exact siblings. Maintained native-reader
adoption is separately cold-compiled and standard-gate checked.24 focused tests
pass. The diagnostic generator's strict source-preimage hash is refreshed for
the reviewed native-field substitution; the two initial stale-preimage failures
and a pre-compiler text-anchor setup failure are recorded separately. These
input guards do not grant exactness or replace the complete byte/field Oracle.

Documentation, research records, complete positive/negative comparisons and
recipes remain in the packet. Full public CI passes1036 tests/29 skipped.
After producer termination, cleanup removes144 files/2393619 logical bytes
(about2.3MiB): six disposable trial objects, the replaced old menu object,
PDB and Python caches. All148 protected files, including105 reused objects,
retain their hashes. The current one-carrier verifier passes after cleanup;
build/ is absent. No new code archive is made. Later product/runtime/semantic/
port gates remain open.

## October 30: superseded menu native-reader handoff

Historical source at03462e4f, superseded by the network reset literal correction.
The following provider-bound replay command is dated; later global manifest
edits can make it reject. Current unchanged proofs are routed through the active
handoff.

### Historical game source — native Supervisor readers in menu updates

This checkpoint follows4d83270e. Menu code now reads the existing canonical
Supervisor flags5D4 and framerateMultiplier5B8 fields. The old partial view
exposed flags without the maintained volatile qualifier and duplicated the
framerate field; those declarations and redundant offset guards are removed.
The existing native qualifiers are reused, not newly introduced. Other partial
fields retain their original offsets and remain provisional. Original nominal
C++ types/qualifiers and complete product/runtime closure are unproved.

One affected source/profile carrier preserves **11 canonical exact owners /
2448 physical bytes /158 fields** and all13 complete emitted body payloads.
Eight compiler-private fields in AsciiMenuState4 change spelling only at
independently verified identical owner-relative coordinates. Native identities,
profiles, addresses, addends, extents and match rows do not change. Complete
Pause remains1732/1776 bytes,107 fields,1453 overlap differences plus44 missing;
its three static timestamp calls still differ from the original two.

The current source/provider-bound verifier is
`.analysis/gpt-61sol-20261030-pause-transition-review/current.py verify`;
`build` cold-compiles only this one carrier, serially at lowered priority.
It checks every emitted body/field, all11 complete original matches and actual
literal definitions through the standard Oracle. Current verification needs
maintained tools, local packet helpers/records and current.obj; no historical
C++ source copies.428 other canonical proofs from105 unaffected carriers remain
source/profile/object/provider and unchanged-unit bound in
`unaffected-proof-reuse.json`. The other564 canonical rows receive no new cold
compile or literal audit. Exact credit stays1003;98% remains unfinished.

## October 30: superseded network literal and RunEcl handoff

Historical source/checkpoints through7ecfd7f9. The current registration-header
checkpoint supersedes these current-proof routes; original observations remain
source/provider scoped.

### Historical game source — complete network reset diagnostic

Following0358e69b, SupervisorNetworkState::ResetSession restores the missing
CP93289 C2 bytes for the final character in `CLSID_DP8SP_TCPIP 使用不可\r\n`.
The old source defined27 bytes including NUL versus the original29 at490E24;
its actual cold-built COFF fails the standard literal gate. Earlier200-byte
code/relocation comparisons had missed this local data defect.

One affected carrier is cold-built serially with the unchanged pinned profile.
The complete200 original bytes/11 fields and both actual string definitions
(37 and29 bytes including NUL) now pass the standard Oracle. Code bytes,
relocation coordinates/addends and native call boundaries do not change; one
literal symbol in the manifest changes to the actual new compiler identity.
No new exact credit is claimed. Direct cached Ghidra verifies all685056 mapped
file bytes and exports reset432F70/provider432770. Actual instructions support
the existing full-EAX0/1 return and one stack argument; no ABI change is made.

Current proof: `.analysis/gpt-61sol-20261030-network-literal-review/current.py verify`.
The small old object is retained as a runnable negative through negative.py,
which reconstructs its three source hashes from0358e69b Git/EOL inputs.
reuse.py checks439 unchanged canonical proof inputs/121391 bytes/6843 fields
from106 carriers, including the current native-reader menu carrier. It checks
individual unchanged unit records across the manifest edit rather than ignoring
a stale aggregate hash. No full1003-function cold replay is run. Other canonical
carriers receive no new literal-data or compile coverage.

A name/original census of237 C-string references/138 distinct destinations
finds this one length/prefix discrepancy before correction and none afterward.
This is negative triage only; names do not prove actual source definitions.
11 focused literal tests pass. Exact remains1003/83.9093%;98% and later gates
remain open. Native menu, Supervisor D3D and earlier dependency corrections
remain in source, with detailed records in the KB and prior handoff archive.
Cleanup removes two disposable PDBs/172032 bytes; all127 protected files keep
their hashes, including106 reused objects. Current positive, actual negative
and unchanged-proof verification pass afterward. build/ is absent.

### Historical large-function review — RunEcl input contracts

Reviewed at03462e4f/0358e69b; the later network correction leaves this carrier
unchanged. Direct cached Ghidra rechecks all685056 mapped
file bytes before exporting RunEcl4086C0 and the real Timer assignment401500.
Current complete RunEcl remains15564 physical bytes/598 fields/30 differences;
its75 same-profile canonical units pass the standard Oracle again from the
source/profile/object-bound unchanged carrier, without a new baseline compile.
The actual unchanged Timer setter/assignment pair also retains25+9 exact bytes.

Seven complete main-carrier compiles test signed-byte input, promoted helper
input, promoted byte capture, long/unsigned four-byte time captures, and native
flag-struct value/reference inputs returning a real u32 result. Each preserves
75 existing exact units/22463 bytes/990 fields, plus136 other regular bodies
and the38-byte compiler contribution.525 repeated unit comparisons earn no new
credit. No profile, native ABI, production code or ledger changes are adopted.

The signed byte changes MOVZX to MOVSX inside the existing mismatch set:30
unequal offsets remain, but the raw stream is not identical. Helper-formal-only
promotion and both time captures are raw-neutral. Promoted byte capture emits a
43-byte handler instead of45, despite retaining15564 total bytes; it produces
2048 ordinary+242 field-byte differences, with26 table differences included in
those242. Native flag-struct inputs outline actual25/18-byte helpers, add one
unbound call and emit15560/599. Full-owner inspection rejects both shapes
before assigning helper addresses or giving an incomplete similarity score.

Actual x86 execution checks four distinct closed flag-store prefixes: original,
maintained, signed and promoted capture. Each executes3072 fixtures covering
all256 wire bytes and12 representative words. All12288 executions preserve
other bits and perform one byte read, one word read and one word store. Equal
prefixes reuse this evidence. This is a flag-prefix diagnostic, not whole-owner,
whole-runtime, original nominal-type or original-helper ownership proof.

Source/input/provider hashes, all raw/COFF and complete diagnostics, positive
native references, actual machine-prefix observations and recipes remain in
`.analysis/gpt-61sol-20261030-runecl-byte-contract/`.
`verify_saved.py` is dated to0358e69b provider pins and can reject later manifest
edits. It checks stored observations and reproduces seven source hashes without
new compilation or acceptance. Current unchanged proof inputs are checked by
the network packet's reuse.py. Current cached main-object
inspection uses the normal tracked ECL tools; deleted trial objects must be
rebuilt with their recorded two/three-case producer before a new comparison.
No old C++ copies or source archive are needed.28 focused tests pass; unchanged
public tooling needs no new full CI. After producer termination, cleanup removes
seven trial objects/2117299 bytes(about2.0MiB); all45 protected files retain
their hashes and stored-record verification passes again. All76 baseline input
hashes are reproduced from03462e4f with recorded Git/EOL rules. build/ is absent.
The98% milestone remains unfinished.

## October 30: superseded native-registration handoff

Historical checkpoint01d7e6b6. Subsequent charge notes-only ledger changes can
make its strict aggregate provider verifier reject; current proof-input reuse
is routed through the active handoff. Original source and observations remain
scoped to that checkpoint.

### Historical game source — native GameManager registration

Following7ecfd7f9, Supervisor's three GameManager registration calls use the
actual GameManagerSetupLayout::RegisterChain definition at41B9D0. The prior
free GameManager_RegisterChain alias had no implementation. The existing setup
layout and side declarations move unchanged into GameManagerSetupView.hpp;
this exposes the maintained owner instead of introducing another facade.
Original nominal C++ ownership and complete product/link closure stay unproved.

Three affected source/profile carriers preserve11 existing canonical owners/
2991 complete bytes/243 fields, plus every one of25 emitted regular body
payloads. Three real external call names change;52 private labels retain their
actual owner-relative coordinates. All canonical manifest entries are unchanged.
The complete Supervisor remains952/996 physical bytes,92 fields,27 calls,
791 overlap differences and44 missing. Its actual version literal still passes.

Three further unavailable current callee carriers are cold-built under their
own pinned profiles, protecting25 existing canonical siblings/5216 bytes/
201 fields. In total36 existing canonical owners retain8207 bytes/444 fields
across six carriers. Three compiler contributions/74 bytes/eight fields/five
entries are inventoried without original-address or authored credit.
429 unchanged canonical proofs/118600 bytes/6611 fields from104 carriers reuse
source/profile/object/provider and unchanged-unit inputs. No full1003 cold
replay is run. Other538 canonical units receive no new compile/literal audit.

Current packet: `.analysis/gpt-61sol-20261030-supervisor-registration/`.
Run family.py verify for the three changed carriers, callees.py verify for the
three additional carriers and reuse.py for unchanged proof inputs. These need
current objects and records, not historical C++ copies or retired old objects.
The public supervisor focus now protects10 real dependencies, including the
actual127-byte GameManager registrar and its original /Os profile. Old undefined
alias objects are rejected rather than numerically normalized.
The Supervisor ledger note now describes10 dependencies/1285 bytes/114 fields;
all other CSV cells stay unchanged. This notes-only migration is checked when
rebinding the unchanged proofs, without recompilation.

16 focused tests and full public CI(1036 tests/29 skipped) pass. Exact remains
1003 and244917/291883 bytes(83.9093%);98% and later gates remain unfinished.
Prior literal, native menu/Supervisor and other dependency fixes remain in source.
After producer termination, cleanup removes440 disposable files/8131930 bytes
(about7.8MiB): three replaced old objects, two trial objects, eight PDBs and427
CI caches. All150 protected files keep their hashes, including six current and
104 reused objects. Current family/callee/reuse/saved-control checks pass again;
build/ is absent. Documentation and research records remain; no code archive.

### Historical large-function review — Supervisor startup dispatch

Direct cached Ghidra exports Enemy410730, Supervisor431110 and registrar41B9D0
after checking all685056 mapped file bytes. Two complete startup dispatch
controls use the maintained native call: a switch retains952/92/205 instructions/
791 differences with the same mismatch positions but different raw bytes;
failure-first dispatch retains952/92/205 but damages two old matches(793).
Both preserve27 calls/four switch tables and restore no old mismatch. Neither
is adopted. verify_controls.py reconstructs their source hashes and saved full
raw/field/linked observations without pretending a new compile or acceptance.

Fresh inspection of the unchanged source/profile/object-bound ECL carrier keeps
RunEcl15564/598/30 with zero field/table differences and Enemy3900/97/13 with69
calls. The earlier RunEcl input and Enemy ownership experiments remain bounded
records, not proof of source-model exhaustion. Prioritize these owners and their
dependencies; native product/runtime, semantics and portability remain later gates.

## October30 charge-configuration checkpoint6c5a019a

## Current game source — real charge configuration

Following01d7e6b6, Player charge uses the actual GameConfiguration pointer at
GameManager+E8 and its byte array at+B4. Its two old local views are replaced
by one uniquely named data projection. This removes this caller's conflicting
GameManagerReplayView definition; ReplayInput.cpp had used that same global
name for a different larger layout. Other partial views and original nominal
C++ ownership remain open. The original setup worker allocatesCC bytes,
initializes through41A8A2 and copies33 DWORDs into the pointer at4A7E78.

One maintained pinned-profile cold build preserves both complete emitted
bodies/1217 bytes and all eight nondebug sections/1281 bytes/70 fields. Charge
remains1197/1210 bytes,70 fields,58 ordered calls,1160 overlap differences and
13 missing. Its five actual scalar definitions pass; the actual co-visible
20-byte Timer comparison retains its original bytes. This alternate carrier
proof adds no canonical unit or exactness credit.

Current packet: `.analysis/gpt-61sol-20261030-charge-configuration/`.
Use current.py verify for current source/profile/provider/object-bound replay,
reuse.py for465 unchanged canonical proof inputs/126807 bytes/7055 fields from
103 carriers, and verify_saved.py for recorded controls/source reproduction.
effect_callees.py separately rechecks three unchanged native Effect owners/282
bytes/15 fields from their actual source/profile/object-bound carrier.
No full1003 cold replay is run. Other535 canonical units receive no new compile
or literal audit. The ledger changes only Charge's notes cell to update the
current route; all other CSV cells are independently unchanged from01d7e6b6.

Native GameManager registration, previous literal fixes and other dependency
corrections remain in source. The unchanged Supervisor/RunEcl/Enemy observations
remain source/profile/object bound; detailed prior reviews live in the existing
archive and KB.10 focused charge tests pass; public tooling is unchanged and
no new full CI is claimed. Exact remains1003/83.9093%;98% remains unfinished.
Cleanup removes11 ignored files/443722 bytes: four trial objects, two replaced
old charge objects and five PDBs. All138 protected files retain their hashes;
current, native-callee, reuse, saved-control and prefix replays pass afterward.
Documentation/research records remain, without a new source archive; build/
is absent.

## Latest large-function review — floating branch identity

Four source-cold charge controls cover baseline, real configuration type,
lower-first clamp and negated less-than clamp. Configuration is wholly raw/field
neutral. Lower-first changes the physical referent order and is rejected before
scoring; negation retains the same1160 count/13 missing but changes raw code.
Both floating alternatives choose a different branch for NaN in the bounded
actual-byte experiment and are not adopted. Equal mismatch counts or an inverted
decompiler condition do not establish floating semantic equivalence.

Five recorded streams/three distinct61-byte growth prefixes execute11 fixtures
each(55 emulator runs), with no calls. Framerate1/rate0, prior integer result0,
FPCW037F and fixture-owned memory/stack bound this diagnostic. It checks only
the stated prefix/store/branch observations, not all floats, full charge logic,
original nominal types or whole-game runtime. fp_prefix.py can replay after
trial-object cleanup; stored-control replay is not new compilation/acceptance.

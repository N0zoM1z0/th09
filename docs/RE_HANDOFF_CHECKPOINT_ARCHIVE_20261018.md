# TH09 — archived checkpoint details through 2026-10-18

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

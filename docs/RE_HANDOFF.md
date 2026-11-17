# TH09 reconstruction — current agent handoff (2026-11-17)

**Read first:** [AGENTS.md](../AGENTS.md) for operator rules,
[RE_WORKFLOW.md](RE_WORKFLOW.md) for the bounded reconstruction loop,
[ORACLES.md](ORACLES.md) for source/target evidence, and
[PROGRESS.md](PROGRESS.md) for generated statistics. This handoff describes
**current maintained source**; dated entries in
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) are historical observations, not
instructions to restore previous source or old scratch objects.

## First five minutes

~~~sh
git status --short --branch                # inspect and continue any dirty work
git log -6 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/ci.py                   # syntax, ledgers, docs, progress
~~~

Do not discard uncommitted changes or run an all-owner cold replay. Use the
registered live `th09` repository/worktree, not copies of source from old
experiments. Commit coherent changes as `gpt-web: ...`; never push silently.

## Authoritative target and phase

- **Only target:** original Japanese TH09 v1.50a,
  `resources/th09.exe`, Windows PE32 i386, image base `0x00400000`,
  entry `0x0047D45F`. SHA-256:
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  This ignored binary is protected: never patch, rename, commit or delete it.
- **Canonical status on 2026-11-17:** 2,248 provisional function candidates;
  35 reviewed but origin-unresolved; 1,037 confirmed authored mappings
  and source-present owners; **1,026 exact, 11 source-present nonexact
  (13,353 logical authored bytes)**. The authored-byte measure is
  **278,578 / 291,931 = 95.4260%**. It is *not* a whole-executable
  exactness percentage or proof of complete original-code inventory.
  Re-read `scripts/report-reconstruction-status.py` after any ledger change.
- **Native product:** `config/build.toml` remains a skeleton with
  unknown translation-unit and link order. Whole Windows i386 build and
  runtime validation are **open**; semantic dual-Oracle work and port
  stages have not started.
- **Exactly measured evidence:** `config/matches.csv` lists accepted
  authored exact functions; `config/match-units.toml` defines their pinned
  VC7.1 source/profile, full owned physical extent and symbolic relocation
  identities; `config/frontiers.toml` sets monotone nonexact baselines.
  An exact byte count, a matching Ghidra decompilation, or an old object
  alone is not enough to promote a function.

## Current source-native reconstruction checkpoints (2026-11-17)

The maintained source includes several **natural C/C++ ownership recoveries**;
none of these changes adds an exact-function ledger claim. All were checked
against the original Japanese 1.50a executable with pinned VC7.1 profiles,
complete compared code/tails and independently verified COFF relocations.

### Latest Player movement and native timer checkpoints

Five subsequent source commits continue the 1,900-byte Player movement boss
and its collision/shot leaves. All work is ordinary C++ and was compared to
the original Japanese 1.50a PE using the pinned VC7.1 owner profiles:

- `9b28b6e8` — the four real `PlayerOptionStateCtorView` records are traversed
  with `++option`, rather than reconstructing the same 0x2F4-byte stride via
  an integer-address cast. The 1,900-byte `UpdateMovementAndOptions` frontier
  remains unchanged at four previously recorded bytes.
- `b61b2ff3` — the sixteen real `PlayerConstructedVector3View` history records
  now shift through `positions1BA0[historyIndex]` and
  `[historyIndex - 1]`. The synthetic byte-address helper was removed; the
  same 1,900-byte boss retains precisely its four previous differences.
- `21e7aa0d` — `FindCollisionAtPlayer` reads the actual
  `PlayerLifecycleView::timer1B74` member directly. It is already the
  canonical `PlayerTimerCtorView` / `ZunTimer`, not an opaque timer overlay.
- `a6c3d780` — Player shot callback 1 invokes `shot->timer454.HasTicked()`
  directly on the real timer member.
- `2dadaef5` — Player shot callback 4 binds a
  `PlayerTimerCtorView &` to `nativeShot->timer454`; direct repeated member
  accesses changed four bytes, while the native reference preserves all 249.

The three exact timer-owner changes pass their complete original-bound
comparisons. `FindCollisionAtPlayer` and the four other exact siblings in
`PlayerCollisionHelpers.cpp` pass **5/5, 1,220 physical bytes, 32 relocation
fields**. Shot callbacks 1 and 4 pass **471/471 bytes / 19 relocations** and
**249/249 bytes / 9 relocations**, respectively. Together these three
source-scoped cohorts cover **7 existing exact owners, 1,940 physical bytes
and 60 relocation fields**. The Player movement boss remains nonexact at
**1,900/1,900 with four differences**; no exact-ledger promotion is claimed.

The source no longer contains any `reinterpret_cast<ZunTimer *>` pointer
overlay. Additional Player movement experiments with a volatile config
pointer, side-index pointer/reference, and memcpy/field-wise history copy
were reverted: they changed the original load order or X87/register schedule.
Enemy draw-head tail-reference and base-pointer variants also regressed the
3,900-byte Enemy OnUpdate and were reverted. The two-byte Enemy frontier is
unchanged.

Useful bounded checks:

~~~sh
python3 -B scripts/check-frontier.py player-movement enemy-on-update player-bullet-collision
python3 -B scripts/replay-scoped-exact.py --source src/PlayerCollisionHelpers.cpp
python3 -B scripts/replay-scoped-exact.py --source src/PlayerShotUpdateCallback1.cpp
python3 -B scripts/replay-scoped-exact.py --source src/PlayerShotUpdateCallback4.cpp
~~~

### Replay playback and Effect callback scheduling probes

`ReplayManagerView::BeginPlaybackStage @ 0x00420840` remains **540/540
bytes with 13 masked differences**. A byte-by-byte comparison identifies
one exact source-order question at `+0x14B..+0x15F`: retail stores the
`value1E7` byte, reloads the replay-data pointer, reads `value1E8`, then
loads the side-0 runtime pointer before publishing `value1E8`. The current
candidate hoists the runtime pointer read ahead of the preceding store and
`value1E8` read. Direct `g_GameManager` accesses instead of the local manager
reference leave the 13-position frontier unchanged. Local runtime pointers
or references declared before the field stores change the complete owner
(typically 532/540 with 135 differences); volatile pointer locals expand it
to 546/540 with 148.

A more targeted interleaving experiment first snapshots `value1E8`, then
reads the actual side-0 runtime pointer through a volatile pointer lvalue,
then stores the snapshot. This produces a **540/540, 9-difference** diagnostic,
removing four previous differences, but introduces seven new offsets
`+0x156`, `+0x162`, `+0x168`, `+0x16C`, `+0x172`, `+0x178`, and `+0x17A`.
The C2 allocator reverses the target EDX/ESI roles for the replay byte and
runtime pointer, and the later side/runtime stores inherit the swap. Changing
the snapshot width, pointer constness, declaration order, reference form,
or reloading the pointer at each later runtime write did not repair it. The
9-difference source is diagnostic only and was reverted; the frontier remains
13 and no `--update` was used.

`EffectManager::AddedCallback @ 0x0040D1C0` remains **51/51 bytes with four
masked positions**. Retail loads `g_AnmManager` into EDI before copying the
callback receiver from ECX to ESI; the candidate saves the receiver first,
loads the global into ECX, then copies ECX into EDI. Delaying the context cast,
using a manager reference, passing the helper parameter by reference, using a
pointer-to-global reference, and spelling out GetAnm/PreloadAnm did not
improve the complete owner. Volatile/register pointer aliases and explicit
helper expansion regressed it. All probes were reverted.

The large Player charge owner was also rechecked with direct volatile mode
assignment and input-pointer declaration/base-increment permutations. The
short-side/volatile-mode baseline remains **1,210/1,210, 183 differences**;
none of the new forms improved it. No dirty experiment remains in maintained
source.

### Latest shared Player +0x1B80 state-word checkpoint

Two further source commits recover the one real Player DWORD shared by
movement, charge, added-state initialization and runtime transition:

- `1d0f367a` — `PlayerFlagBits1B80View` is a four-byte bitfield view over the
  actual `PlayerLifecycleView +0x1B80` word. Bits 0-1 are the two-bit focus
  state and bit 2 is the charge-attack active state. `UpdateMovementAndOptions`
  and `UpdateChargeAttack` now use those named members instead of raw masks.
- `5971a7fb` — the same view names bit 3 as
  `frontSideClearPending03`. `InitializeAddedState` sets focus state to 2 and
  publishes the pending flag; `PlayerLifecycleView::OnUpdate` consumes and
  clears that same bit. The raw `flags1B80` DWORD remains an overlapping alias
  for owners whose remaining bits have not yet been named.

The original physical word, four-byte layout and all existing bit positions
are preserved. Pinned VC7.1 source-scoped replay passed **21/21 exact units**
in `PlayerRuntime.cpp` (**5,391 physical bytes, 288 relocation fields**) and
**4/4 exact units** in `PlayerAddedState.cpp` (**665 bytes, 30 fields**).
This is **25 existing exact owners, 6,056 physical bytes and 318 relocation
fields** across the two separate source cohorts. The 522-byte `OnUpdate`,
814-byte `UpdateBeforeState` (852-byte physical comparison extent), and
552-byte `InitializeAddedState` (572-byte physical comparison extent) all
retain their complete original comparisons. No exact-ledger membership or
frontier baseline changed.

The two edited source carriers renumbered compiler-private local labels.
Their updated spellings were accepted only after complete structural-byte
identity, unchanged relocation offsets/types/addends, and same-section COFF
symbol-value proofs mapped all six `InitializeAddedState` labels and all
eleven `UpdateBeforeState` labels to their exact original PE destinations.

The two large Player owners remain open: `UpdateMovementAndOptions` is still
1,900/1,900 with four differences and `UpdateChargeAttack` is still
1,210/1,210 with 183. ChargeAttack experiments with 32-bit side locals,
separate side aliases, pointer/reference input forms, narrowed lexical scopes,
and nonvolatile/direct focus snapshots were all reverted when they regressed
the complete boss (typically 717-900 differences or a shorter body). The
maintained 16-bit side / volatile focus snapshot is still a code-generation
compensation, not the desired final natural source; its EDI/EBX allocation
problem remains explicit work. A further isolated probe set covered direct
`sideIndex` array indexing without a side local, short-side/full-index input
aliases, unsigned/const/`size_t` side locals, pointer-base-plus-side increments,
32-bit native byte-offset expressions, side references, and volatile direct
mode snapshots. None improved the 183-byte frontier; several returned to the
717-900-difference full-owner regressions. All probes were reverted.

`CheckBulletCollision @ 0x0041DFF0` was also re-read from its original IDA
control flow. The retail owner is a three-level retry structure: an inner
region/type/delay scan, a middle rotated/AABB path, and an outer circle/hit/
cancel path. Four distinct slot-advance sites are real: cancel miss, rotated
final miss, AABB final miss, and the shared invalid/delay/early-geometry/
circle miss. New natural C++ loop experiments did expose the original entry
jump and loop-header schedule: a three-loop candidate reached **859/862 bytes
and 664 unlike positions**, and a second explicit hit/geometry dispatch
candidate reached **849/862 and 656**. Neither is ratchet-safe: the candidates
introduce new differing positions, shorten the physical body, and/or reverse
the original `this`/Bullet register roles and hit-dispatch layout. A direct
position-parameter geometry variant further regressed to **827/862** and a larger stack
frame. A flat explicit-label attempt reached 838/862 but changed additional
bytes. These are diagnostic source probes only; none was installed. The
maintained collision frontier remains **842/862, 705 masked differences**.
No frontier baseline was changed and no ASM, padding, or full cold replay was
used.

Useful bounded checks:

~~~sh
python3 -B scripts/check-frontier.py player-movement player-charge-attack
python3 -B scripts/replay-scoped-exact.py --source src/PlayerRuntime.cpp
python3 -B scripts/replay-scoped-exact.py --source src/PlayerAddedState.cpp
~~~

### Latest five native Player/GameManager checkpoints

Five further source commits deepen the large Player selector and GameManager
setup/update owners. The work was done on the live registered `th09` tree;
all exact sibling owners were checked with their original pinned VC7.1 profile,
complete physical code extent and PE-bound relocation fields:

- `26e847b3` — `PlayerUpdateSelectorState` now writes the actual
  `PlayerLifecycleView::timer303C8` ZunTimer member directly. The field is
  already the canonical `ZunTimer` alias and its +0x303C8 offset is pinned;
  the cast through `ZunTimer *` was unnecessary. The 3,238-byte selector
  boss and 412-byte pattern-offset resolver pass **2/2 exact, 3,650 bytes,
  154 relocations**.
- `f50245ed` — the two 12-byte XYZ transfers from `EnemyFloat3` to
  `PlayerPositionView` in that same selector now use `memcpy` between two
  distinct but statically verified 0x0C XYZ records, rather than type-punning
  one as the other. The selector boss and resolver again pass **2/2 exact,
  3,650 bytes, 154 relocations**. Field-by-field assignment changed the boss
  extent to 3,210 bytes; a PlayerPositionView reference changed it to 3,245.
  Those alternatives were rejected.
- `920b8979` — `GameManagerSetupLayout::OnUpdate` writes its actual
  `replayPauseRecorded` member directly rather than reinterpreting the same
  receiver as `AsciiGameManagerView`.
- `23187e2c` — the spawn-credit redemption subtracts 10,000 directly from
  the genuine `g_GameManager.valueCC` member. Its enclosing `> 10000` guard
  keeps the signed result representable; the unsigned-pointer alias is not
  needed.
- `6ffe88ae` — both card-attack spawn calls now downcast the real typed
  `sides[n].enemyManager10` base pointer to the zero-data derived
  `EnemyManagerCardAttackSpawnView` interface with `static_cast`, instead
  of retrieving the same receiver through the untyped `subsystem4` slot.

The three GameManager changes each preserve the entire **12-owner,
4,680-byte, 415-relocation** source cohort, including the **1,689-byte
GameplaySetupThread** and **1,230-byte OnUpdate** bosses. Together the two
distinct source cohorts prove **14 existing exact owners, 8,330 complete
physical bytes and 569 relocation fields**. No exact-ledger promotion or
whole-product build claim follows. A separate BackgroundRender attempt to
replace its VM float-pointer round trip with direct Float3 access expanded
the 3,274-byte boss to 3,275 bytes; direct BackgroundPoint component writes
expanded it to 3,298 bytes. Both were reverted. The original 1,026 exact /
11 nonexact canonical status and all nonexact frontier baselines are retained.

Useful scoped replays:

~~~sh
python3 -B scripts/replay-scoped-exact.py --source src/PlayerGameplayState.cpp
python3 -B scripts/replay-scoped-exact.py --source src/GameManagerSetup.cpp
python3 -B scripts/check-frontier.py enemy-on-update replay-save replay-begin-playback
~~~

### Previous five native C++ source checkpoints

The new work is in the registered live `th09` tree, not a separate source
copy. Each checkpoint includes the pinned VC7.1 exact comparison for all
co-resident authored owners, including complete physical tails/relocations:

- `2897b950` — the **2,258-byte** `TitleScreenView::OnUpdateMusicRoom`
  boss uses native file byte-pointer differences, real `musicTracks[]` array
  indexing and a direct music-pause toggle. Pointer subtraction alone
  changed four target bytes; the related typed track indexing restored the
  original entire codegen. Together with the DrawMusicRoom leaf this is
  **2/2 existing exact, 2,467 bytes, 80 relocation fields**.
- `b0b01f4f` — the 1,776-byte pause-menu state machine and the 1,172-byte
  second menu owner, plus ASCII manager update, now increment their real
  state and frame members directly instead of via unsigned-word casts.
  **10/10 exact, 4,206 bytes, 264 relocation fields**. A separate typed
  side-index popup loop changed 11 bytes; it was rejected and reverted.
- `37056aa6` — `ReplayInputState::Update` advances its real 16-bit held-frame
  cursor; `RecordInputAndFps` increments the native delay, frame count and
  frame counter members. All **12/12 exact, 4,093 bytes, 232 relocation
  fields** pass across the two source profiles. Nonexact `SaveReplay` (2
  differing bytes) and `BeginPlaybackStage` (13) remain protected. A trial
  converting all twelve BeginRecordingStage raw stage-slot expressions to
  direct array indexing grew its 1,323-byte owner to 1,333 bytes and
  disrupted its code/relocations; it was rejected.
- `7d310565`, `1a48f848` — `Ending::UpdateFade` and the full script parser
  now use native fade/wait timers, next-text-VM and two-character local
  text indexing. The 1,727-byte complete `RunEndingScript` owner and fade
  leaf are included in **10/10 exact, 3,015 bytes, 159 relocation fields**.

The four distinct source cohorts above prove **34 existing exact owners,
13,781 physical bytes and 735 relocation fields**, without a full-project
cold replay. All compiler-private label renames were independently checked
against their actual same-section COFF definitions and unchanged original PE
targets before the affected-source replay. The canonical total stays at
**1,026 exact, 11 source-present nonexact**; no ASM, NOP/PADDING, forced
register assignments or fabricated exactness was introduced. Whole-game
native build and runtime closure remain open.

Useful bounded checks:

~~~sh
python3 -B scripts/replay-scoped-exact.py --source src/TitleScreenMusicRoom.cpp
python3 -B scripts/replay-scoped-exact.py --source src/AsciiManagerMenu.cpp
python3 -B scripts/replay-scoped-exact.py --source src/ReplayInput.cpp
python3 -B scripts/replay-scoped-exact.py --source src/Ending.cpp
python3 -B scripts/check-frontier.py replay-save replay-begin-playback enemy-on-update
~~~

### Previous eight native-source checkpoints

Eight local source commits follow the earlier interpreter work, all using
natural C++ and the original Japanese v1.50a pinned VC7.1 comparison:

- `131ec4ee`, `7a40ed2d`: the **2,045-byte Title options boss** advances its
  three actual frame/timer members and directly updates eight genuine
  unsigned `GameConfiguration` volume-byte slots, while retaining the signed
  display/clamp reads. Its audio helper is also exact: **2 owners, 2,079
  physical bytes, 138 relocations**.
- `ec411301`, `b04e17f2`, `be05d70f`: **SoundPlayer** now uses byte-pointer
  RIFF/WAVE chunk navigation, contiguous command records, native queues,
  request/pan arrays, BGM paths/format records, DirectSound buffer pointers,
  `++commandCursor` and direct command-step members. The 1,414-byte
  `ProcessQueues` boss, 614-byte `LoadSound` and 74-byte WAV scanner are
  included in **20 exact owners, 5,400 bytes, 184 relocations** across eight
  compiler carriers.
- `be37244f`: **EtamaController::OnUpdate** increments the three genuine
  active-pool counters and controller frame counter without interpreting
  them as unrelated unsigned storage. All **18 exact BulletManager owners,
  6,711 bytes and 259 relocations** pass, including the large spawn and update
  functions.
- `ee41eb7d`: **RunEcl** reads its movement opcode words directly from
  `instruction->arguments` / `wireArgs[index]`, with no fabricated integer
  byte addresses. Its previous 84 exact siblings remain unchanged.
- `62912d3e`: the shared **ECL integer/float/byte/short operand leaves**
  borrow the same native variable-length instruction-tail union. The full
  affected include graph passes **92 exact owners, 51,865 bytes and 2,381
  relocations** across six compiler carriers, including the 14,792-byte ECL
  interpreter and ECL operand resolvers.

The four distinct current source cohorts account for **132 previously exact
owners, 66,055 complete physical bytes and 2,962 relocation fields**. Every
compiler-private label spelling update was independently proved against the
actual same-section COFF definition, exact relocation offset/type, linked
Japanese PE target and complete body/tail before a selected-source replay.
No new `matches.csv` exact credit, full cold replay, ASM, NOP/PADDING or
compiler-register forcing is claimed.

**Rejected nonexact probes:** Ten variations separating the Player charge
attack's original 16-bit focus-side local from a native 32-bit input index
usually regressed its 1,210/1,210-byte 183-difference frontier to
1,213–1,215 bytes with roughly 890–900 differences; a single byte-pointer
variant preserved the same baseline without improvement. Eight direct/const
nonvolatile forms of the ScreenEffect shake-envelope hold/fade calculation
regressed its 336-byte/26-difference frontier to 334 bytes/195 differences.
The maintained nonexact source and all strict frontier records remain
unchanged. The original Enemy draw-group two-byte and Player collision CFG
blockers are still open.

To recheck only the impacted source/headers:

~~~sh
python3 -B scripts/replay-scoped-exact.py --source src/TitleScreenOptions.cpp
python3 -B scripts/replay-scoped-exact.py --source src/SoundPlayerProcessQueues.cpp
python3 -B scripts/replay-scoped-exact.py --source src/BulletManager.cpp
python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunControl.inl --changed-file src/EclRunState.inl
python3 -B scripts/check-frontier.py player-charge-attack screen-shake-envelope enemy-on-update
~~~

### Previous interpreter checkpoints

- `e7c7f5ee`, `34098751` — The **2,852-byte** `FrontMessageRuntimeView::Update`
  boss reads short/byte script operands from their actual native typed payload
  pointers (20 short + 2 byte sites) and addresses the authentic
  `g_GameManager.sides[index]` allocation. Its complete physical owner and
  same-source decode leaf pass **2 exact / 3,007 bytes / 167 relocations**.
- `9058a27f` — The **2,472-byte** background stage-script interpreter reads
  its 22 integer, 4 float and 20 vector wire payloads from
  `RawStageInstr::args`, retaining the native 20-byte wire record layout.
  The script, camera interpolation, Hermite and timer owners pass
  **5 exact / 3,231 bytes / 160 relocations**.
- `49f2af51` — The **9,358-byte** `AnmManager::ExecuteScript` interpreter reads
  integer/float operands directly from its variable-length `AnmRawInstr`
  payload (27 integer + 3 float cursor forms), with readable opcode access
  macros. The whole instruction-switch tail and sibling ANM leaves pass
  **5 exact / 10,496 bytes / 421 relocations** across the two pinned profiles.
- `25c271d0` — The **14,792-byte** `EclManager::RunEcl` boss uses native
  `activeCallStack2CE4[depth]` context records at all three sites, rather
  than synthesizing integer-address byte strides. The full ECL source cohort
  passes **84 exact / 45,420 bytes / 1,987 relocations** across two profiles.
- `7bcee027` — Enemy's nonexact 3,900-byte `OnUpdate` now indexes the actual
  `trailSamples33E8` and `attachedEffectState53B4.effects00` arrays. The same
  **84 ECL exact** siblings pass their scoped replay after this edit; the
  Enemy calc frontier remains **3,900/3,900 with 2 differing bytes**.

Across the four distinct exact source cohorts above, the current verified
comparison totals **96 existing exact functions, 62,154 full physical bytes
and 2,735 relocation fields**. These are selected-source proofs, not a
whole-EXE replay, newly promoted functions or a successful native game build.
The compiler-private `$L...` symbols changed when natural source was restored;
**every** recorded rebinding was checked against its actual same-section COFF
definition and the unchanged linked original PE destination (as well as the
relocation offset, type and full linked bytes). No handwritten ASM, NOP/padding,
register forcing, altered target bytes or relaxed frontier ratchets were used.

### Earlier source-native checkpoints in this worktree

`bdd34f42`, `ba48f475`, `a4dc57a1`, `4d977749`, `78abe86d` restore
native HUD VM traversal, Effect pool iterators and allocation indexing,
background fog BGRA lanes, and Title key-config counters. Scoped replay covers
all associated exact owners. `40a561ee`, `79fecb41`, `5118ecad`, `c283bb54`,
`0f2b2902`, `8e872bff` additionally recover typed Player selector collision
vectors, genuine `ZunTimer` conversions and Player collision/angle receivers,
plus native Laser pool traversal. Their linked COFF symbols were validated
before rebindings; exact siblings remain unchanged. See the Git commits for
individual source edits and `docs/KNOWLEDGE_BASE.md` for older experiments.

### Unresolved work and useful negative results

The **11 source-present nonexact owners** remain nonexact. In particular,
Enemy `OnUpdate` still differs at just two register-encoding bytes in the
restricted-side draw-head selection. The one-use `NativeEnemyDrawHead` helper
was removed in `09660746`; both append sites now use typed pointer addition
on the native four-entry array. Further direct-index, integer-local, reference
and register-hint forms did not improve the EAX/ECX group-index residual.
`CheckBulletCollision` still needs its original loop backedges and early hit
routing, while `SupervisorServiceUpdate` has a larger control-flow mismatch.
Prior short-index-to-int substitutions in `UpdateChargeAttack` increased its
residual; do not promote unproved variants or fabricate padding/ASM.

Fast checks for the maintained work:

~~~sh
python3 -B scripts/replay-scoped-exact.py --source src/FrontMessageUpdate.cpp
python3 -B scripts/replay-scoped-exact.py --source src/BackgroundStageScript.cpp
python3 -B scripts/replay-scoped-exact.py --source src/AnmManagerExecuteScript.cpp
python3 -B scripts/replay-scoped-exact.py --changed-file src/EnemyManagerCore.inl
python3 -B scripts/check-frontier.py enemy-on-update player-bullet-collision supervisor-network-service
python3 -B scripts/ci.py
~~~

## Current nonexact work queue

The numbers below were rebuilt with the owner's recorded compiler profile
and `scripts/check-frontier.py` on 2026-11-17. They are **relocation-masked
differing byte positions**, not counts of differing instructions. The
instruction starting address can precede the differing byte offset.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **enemy-on-update (EclManager.cpp)** | 0x00410730 | 3,900/3,900, **2 masked bytes** | At `+0x19A` and `+0x1A1`, VC7.1 uses ECX where retail uses EAX for the early draw-group index. Both append sites use direct typed pointer addition; the 84 co-resident ECL exact functions passed scoped replay after the `.inl` change. |
| **replay-save (ReplayInput.cpp)** | 0x00420C60 | 1,238/1,238, **2 masked bytes** | `+0xB7` and `+0xBB`: header-copy destination register ECX instead of retail EDI. |
| **replay-begin-playback (ReplayInput.cpp)** | 0x00420840 | 540/540, **13 masked bytes** | Candidate hoists `sides[0].runtime` before the `value1E7` store and `value1E8` load; retail loads `value1E8`, then runtime0, then stores it. A volatile pointer-field probe reached 9 differences but introduced seven new positions through an EDX/ESI register swap; do not ratchet. |
| **player-movement (PlayerMovement.cpp)** | 0x0041C170 | 1,900/1,900, **4 masked bytes** | Config/side load order at `+0x127–0x12A`. |
| **player-charge-attack (PlayerChargeAttackUpdate.cpp)** | 0x0041F810 | 1,210/1,210, **183 masked bytes** | EDI/EBX side/input allocation remains unresolved. Direct member-index, signed/unsigned/size_t locals, pointer/reference aliases, explicit native byte offsets and mode-snapshot variants did not improve the complete boss. |
| **player-bullet-collision (PlayerCollisionHelpers.cpp)** | 0x0041DFF0 | 842/862, **705 masked bytes** | Three nested retry levels and four distinct slot-advance sites confirmed. 859/862 and 849/862 natural-loop candidates are not monotone-safe due new differences, size and register/hit-block scheduling; do not ratchet. |
| **effect-added-callback (EffectManager.cpp)** | 0x0040D1C0 | 51/51, **4 masked bytes** | EDI global ANM manager vs ESI callback receiver setup order. |
| **screen-shake-envelope (ScreenEffect.cpp)** | 0x004227A0 | 336/336, **26 masked bytes** | Volatile timer/receiver lifetimes and ESI/EDI selection. |
| **title-update-result (TitleScreen.cpp)** | 0x004266B5 | 484/485, **167 masked bytes** | Control flow / signed character load at result state transition; note the candidate is one byte shorter. |
| **title-draw-replay-menu (TitleScreenReplayMenuDraw.cpp)** | 0x004234F6 | 1,280/1,280, **29 masked bytes** | Register assignment around replay-record and menu selection paths. |
| **supervisor-network-service (SupervisorNetwork.cpp)** | 0x00430AA0 | 1,629/1,633, **966 masked bytes** | Larger host/network branch CFG, 20-byte stack frame and network receiver lifetime. The smaller scratch CFG is *not* ratchet-safe. |

**Additional frontier, not one of the eleven authored nonexact functions:**
`etama-fsincos` at `0x00412780`, `src/EtamaControllerDraw.cpp`:
maintained **36 / 33 bytes, 25 differing positions**.
Original authored-versus-library provenance remains *unclassified*.
The previous x87/FSINCOS inline-assembly source once produced exact bytes,
but it was deliberately replaced with natural `cosf/sinf` C++.
Its obsolete standalone match-unit was removed on 2026-11-17;
do not restore that exact claim, add inline ASM, or assume numerically
identical last-bit rounding.

The frontier file is authoritative for addresses, profile flags, byte
positions, and SHA-256 of each original extent. `check-frontier.py`
checks the monotone ratchet; it **does not award exact credit**.

## High-value next investigations

1. **Real boss progress:** restore `CheckBulletCollision`'s original
   early circle-hit dispatch/backedges in natural C++ and compare every
   exact Player sibling. A previous scratch top-tested-loop variant
   fixed some old offsets but introduced new ones; do not promote it.
   `SupervisorNetworkService` likewise needs the correct first
   network-pointer EDI lifetime and host-session branch placement.
2. **Two-byte blockers:** trace C2's early Enemy draw-group virtual
   register choice (vreg 7; equal-score descriptors 1/2) rather than
   repeating register hints. For `SaveReplay`, analyze the typed
   frame-header copy and temp-buffer lifetime. Both remained at two
   differences after earlier simple pointer/declaration sweeps.
3. **Small leaves / related bosses:** `EffectManager::AddedCallback`,
   `ScreenEffect::CalcShakeEnvelope`, Replay playback, Player movement,
   Title result update and replay menu all have bounded frontiers.
   Recheck both source siblings and the protected residual before
   keeping a variant.
4. **Origin and product closure:** resolve the 35 outstanding origin
   cases separately; establish real translation units, link order,
   resources, build/runtime tests. Do not equate per-function Exact
   with a runnable native product.

Recent natural-C++ refactoring has already restored real
GameManager/Player/Enemy/ANM/Supervisor object ownership, indexed
arrays and file-backed byte pointers in large exact bosses. Check
**maintained source and Git history** before repeating this work;
a private symbol spelling change without PE/COFF proof is not progress.

## Fast, scoped verification

~~~sh
# One exact owner and all its relocations / complete physical tail:
python3 -B scripts/build-match-unit.py --unit ecl-run-ecl
python3 -B scripts/compare-coff-function.py --unit ecl-run-ecl --json

# One affected source, all its exact owners and distinct pinned profiles:
python3 -B scripts/replay-scoped-exact.py --source src/ReplayInput.cpp

# For a changed shared .hpp/.inl, see actual source dependents first:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EnemyManagerCore.inl --dry-run

# Recompile and protect only the relevant nonexact boss:
python3 -B scripts/check-frontier.py enemy-on-update
python3 -B scripts/check-frontier.py --show replay-save

git diff --check
python3 -B scripts/ci.py
# Target-independent tool tests, when editing tooling/config:
python3 -B scripts/ci.py --full
~~~

`ReplayInput.cpp` has **both /EHsc and no-/EHsc** exact carrier
profiles; Title uses `/O1` without `/Oi` and some Effect owners omit
`/EHsc`. Always use the profile recorded for the target unit.
For `EclManager.cpp`, changing the included
`EnemyManagerCore.inl` can renumber private `$L...` labels across
84 exact functions. Before editing any manifest spelling, prove
nonrelocation byte equality, relocation offset/type, PE destination
and same-section COFF symbol definition. Do not pretend changed
label numbers establish new Exact credit.

Build products and compiler PDBs are **disposable ignored caches**;
missing `build/matching/*.obj` or `build/frontier/*.obj` after storage
cleanup is expected. Regenerate only the relevant carrier(s).
A missing historical `.analysis` probe is not missing proof: rebuild
from `src/` with the current target/profile.

## Protected inputs and storage policy

**Never delete:** the verified executable
`resources/th09.exe`, `.tools/` pinned runtime/toolchain selectors,
`_reference/`, saved Ghidra project
`.analysis/ghidra-direct/`, or the
`c2-analysis/` C2 investigation / Ghidra projects.
The native allocator investigation also retains
`.analysis/c2-heap.bin`,
`.analysis/FUN_107844a4.decomp.txt`, and four
`.analysis/gdb-c2-*.cmd` recipes. Two compact
natural-C++ diagnostic *sources* remain useful:
`.analysis/PlayerCollisionHelpers.natural-preheader-842.cpp`
and `.analysis/SupervisorNetwork.natural-ebx-reload.cpp`.
They are **not maintained source and not exact proof**.

Superseded compiler objects, PDBs, textual variant corpora,
captured logs and `build/` caches have been pruned to save disk.
Do not restore them en masse. Don't commit original game files,
toolchains, Ghidra databases, C2 DLLs, decompiler dumps or
generated comparison results. Do not touch the protected
target while cleaning cache state.


### 2026-11-17 continued source probes (all experimental edits reverted)

- `ReplayManagerView::SaveReplay`: 1,238/1,238 bytes, 2 register differences
  at `+0xB6/+0xB9`; typed assignment, direct typed lvalue, `memcpy`, register
  qualifiers and link-lifetime changes did not improve it.
- `ScreenEffect::CalcShakeEnvelope`: 336/336 bytes, 26 differences; hold/fade
  operand and declaration-order experiments did not improve the complete owner.
- `EnemyManagerView::OnUpdate`: 3,900/3,900 bytes, 2 differences; direct
  indexing, pointer-base/byte-offset helpers and register locals did not
  improve EAX/ECX group-index allocation.
- `PlayerLifecycleView::UpdateMovementAndOptions`: 1,900/1,900 bytes, 4
  differences; direct/global-reference/volatile and index-width probes left
  the global-configuration vs side-index load order unresolved.
- `EffectManager::AddedCallback`: 51/51 bytes, 4 differences; inline store
  helpers and argument-order variations did not alter the receiver/global
  register schedule. `TitleScreenView::DrawReplayMenu`: 1,280/1,280 bytes,
  29 differences; selected-index and pointer/reference variants did not help.

No frontier metadata was widened, no ASM/padding was introduced, and no full
cold replay was run. The last accepted maintained baselines remain unchanged.


### 2026-11-17 Player bullet-collision loop reconstruction

- Fresh IDA confirms `CheckBulletCollision @ 0x41DFF0` must keep the native
  `activeB100` cursor in ESI, Player in EDI, Bullet in EBX, and region in ECX.
  The cursor home at `[ebp-4]` is written before the first region dereference.
- Target control flow is three nested retry levels: type/delay filter,
  radius/angle geometry dispatch, then the shared hit/cancellation path.
  Distinct miss paths increment the cursor at `0x41E0A3`, `0x41E133`, and
  `0x41E19E`; cancellation saves the live cursor before calls.
- Temporary direct-slot nested loops reached 849/862 (676 differences) and
  854/862 (673 differences); a region-reference form reached 835/862 (668).
  Each introduced new differences or changed target register roles, so all
  were reverted. Maintained baseline remains 842/862, 705 differences.
- `BeginPlaybackStage`, `AddedCallback`, Enemy draw-group helper and Player
  focus load-order probes were also reverted; their accepted frontiers are
  unchanged. No cold replay, ASM, padding, or frontier widening was used.

### 2026-11-17 follow-up C++ scheduling probes

All experiments in this section were temporary and restored before the final
status check. No frontier baseline or manifest was changed.

- **Player collision preheader:** `activeB100` is an inline 514-entry
  `PlayerCollisionRegionCreateRegionView *` array at `Player +0xB100`.
  Making only the first element read a volatile lvalue after saving the cursor
  produces the correct initial `lea esi,[edi+0xB100]` / `[ebp-4]` store / first
  region load order. It removes four old byte mismatches but moves the loop
  header/body and leaves 701 differences with new offsets, so it is not
  monotone-safe. The three-loop diagnostic remains 849/862 with 656
  differences, but assigns Player/Bullet to EBX/EDI opposite retail. Direct
  Bullet, register, pointer-reference, and `PlayerLifecycleView &` variants
  did not restore the target EDI/EBX roles. Maintained source remains 842/862,
  705 differences.
- **Effect callback:** a member-reference assignment immediately after
  `GetAnm` recreates the target EDI-global / ESI-context prologue, but stores
  `effectAnm` before checking `rawData04` and before a possible `PreloadAnm`.
  That is observably earlier publication and was rejected. Delayed local
  results, out-reference helpers, direct global calls, pointer-slot aliases,
  aggregate state and register/volatile hints did not retain the target
  prologue while preserving the delayed store. `AddedCallback` remains
  51/51 bytes with four masked differences.
- **Title result:** direct array-value, pointer arithmetic, `char` reference,
  `register` index and assignment-in-call variants did not improve
  `OnUpdateResult`. Retail reuses ECX as the selected-character index and CL
  as the byte temporary; the candidate uses EAX for the indexed global load.
  The maintained owner remains 484/485 bytes with 167 differences.
- **Replay playback:** an explicit volatile read of the first `activeB100`
  element after cursor publication fixes the initial load order, but shifts
  the loop and leaves 701 differences. Register-hint and pointer-local forms
  did not repair the EDX/ESI roles. `BeginPlaybackStage` remains 540/540 with
  13 masked differences.
- **Header-layout probe:** temporarily aliasing `EnemyFloat2` to `Float2`, or
  adding another `PlayerLifecycleView` header assertion, renumbered many
  compiler-private ECL local labels without changing the maintained frontier
  bytes. The alias/assertion experiments were reverted rather than accepting
  a large manifest-only rebind. Existing native layout assertions remain.

The only accepted source changes from the preceding checkpoint are the three
Enemy commits already listed above. This follow-up produced no safe monotone
source variant; do not promote any of the diagnostics above.


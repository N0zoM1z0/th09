# TH09 reconstruction — live handoff (refreshed 2026-11-07)

**Start here:** [next-agent execution checklist](TH09_NEXT_AGENT_HANDOFF_20261107.md),
[workflow](RE_WORKFLOW.md), [original-byte/VC7.1 Oracles](ORACLES.md),
[tool routing](TOOLS.md), [accepted receipt archive](RE_ACCEPTED_RECEIPTS.md),
then `AGENTS.md` at the repository root. This file
is the **live routing and frontier index**, not a transcript. The previous
560-line live document was retained byte-for-byte in
[the November 7 historical archive](RE_HANDOFF_HISTORY_20261107.md).
Older dated notes, apparent "current HEAD" claims and compiler hypotheses are
history; check live Git and original-bound evidence before accepting them.

## Verified state — not a product build

| Measure | Count |
| --- | ---: |
| Function candidates | 2,248 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 38 |
| Confirmed authored | 1,034 |
| Classified exclusions | 1,176 |
| Source-present authored mappings | 1,034 |
| Canonical exact functions | 1,021 |
| Source-present non-exact functions | 13 |
| Source-present non-exact bytes | 29,834 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 262,034 |

**1034 source-present / 1021 exact / 13 non-exact**.
**262,034 / 291,868 reviewed-authored bytes = 89.7783% exact**.
To reach 98% of the currently classified authored denominator, another **23,997 bytes**
must become *fully original-bound exact*; similarity to
existing bytes is not exact credit. This target could change if further
origins are resolved.

Input is the operator-provided, ignored Japanese TH09 **v1.50a** executable
`resources/th09.exe`; required SHA-256:
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Source ownership is reconstructed, not automatically the historical original
C++ class/TU identity. The catalog is **not proven complete**. Exactness is
local PE/COFF/VC7.1 scope, not full Windows i386 linking, gameplay semantics,
portability or Factory acceptance. No current source cleanup adds exact credit.

**Latest relevant code checkpoint before this documentation refresh:**
`9c17b496` (exact ANM vertex-buffer release). The next-agent guide documents
`03cf4b33`, `d3c44fcf`, `910f653a` and `9c17b496` as the latest coherent
ANM counter/cache chain. Documentation and maintenance commits advance HEAD;
**never substitute those hashes for `git log -1`**. Local commits are not
proof of remote publication.

## Current native ANM/Player facts worth preserving

- **ANM frame counters (corrected from older swapped names):**
  `AnmManager +0x0C = scriptsStartedThisFrame0C`,
  `+0x10 = scriptsExecutedThisFrame10`,
  `+0x14 = renderStateChangesThisFrame14`,
  `+0x18 = flushesThisFrame18`.
  The exact 15-byte reset stores to **+0x10, +0x14, +0x0C, +0x18** in that
  order. Original-exact witnesses: 9,358-code/9,776-physical-byte
  `AnmManager::ExecuteScript`, 7 ANM runtime leaves (507 physical bytes),
  16 Render owners (5,814 bytes) and 31-byte ReleaseVertexBuffer.
- **Real ANM render and vertex owner:** `VertexDiffuseXyzrhw` is 20 bytes;
  shared 0x128E8 `AnmManagerQuadCacheView` has `quadVertexBuffer` at +0x12890,
  software vertices at +0x12894, and the cached world matrix at +0x12024.
  Four coherent ANM render/projection/draw source carriers have **30 already
  exact owners / 8,983 physical bytes**. Exact types here are source-facing
  receiver/projection views; do not claim their nominal names are original.
- **Real Player selector cross-owners:** the 3,238-byte selector and
  346-code/412-physical-byte resolver remain exact. Its former private
  `g_PlayerSideProtocols` is actually `g_ReplayInputStates` at 0x4ACE18 with
  0x8E stride and history words +0x2C/+0x32/+0x34. Its 0x4A7E4C output is
  shared with exact Player death-state rewards. The genuine FrontSide timing,
  ZunTimer, RNG, Player lifecycle and shared runtime method targets have
  independently exact leaves. Original-bound call targets are frozen.
- **Near-exact is still not exact:** Player ChargeAttack had experimental
  volatile-side-mode candidates that achieved the target 1,210-byte *length*
  but not target register use/control flow. The maintained 1,197-byte
  candidate remains **nonexact**; no experiment was silently adopted.
  SaveReplay (two bytes), Enemy OnUpdate (13), RunEcl (30) and other open
  functions below retain their existing residuals and need more research.

- **Replay recording storage sizes:** native three-stream `ReplayBufferLink`
  and 0x20-byte stage headers now drive the allocation/zeroing capacities
  shared by exact Replay recording producers and near-exact 1,238-byte
  SaveReplay. Wire, FPS, and runtime stage array counts now also drive
  the original 3-by-10 loader/releaser/save loops. Twelve scoped siblings
  remain exact (4,093 bytes/232 fields);
  SaveReplay still has two unequal bytes. See
  [recording capacity review](TH09_REPLAY_RECORDING_CAPACITY_20261107.md).

- **Enemy OnUpdate signed native extents:** the active 128-of-129 scan,
  two secondary VMs, four callback thresholds and four child blocks now
  derive from the genuine native arrays with an explicit signed comparison.
  The 3,900-byte boss preserves its 13-difference frontier. Two previously
  stale local `$L` name groups were rebound to their independently checked
  original addresses; **91 affected exact siblings / 41,868 bytes / 1,818
  fields** passed fresh scoped replay. See
  [signed extent and label review](TH09_ENEMY_SIGNED_EXTENTS_AND_LOCAL_LABELS_20261107.md).

- **Shared Etama ANM slot 8:** Etama draw/setup, Effect callback and Etama
  lifecycle now use a single narrow native resource enum rather than
  unrelated numeric slot literals. Original-bound scoped results: 16 Effect
  siblings / 2,571 bytes and 6 Etama draw/lifecycle functions / 1,705 bytes
  exact; the Effect callback remains 51 bytes with eight residual differences.
  Fifteen Etama compiler-local label names were checked against unchanged
  original target VAs and rebound. See
  [Etama ANM slot recovery](TH09_ETAMA_ANM_NATIVE_SLOT_20261107.md).

- **Player movement native loop extents:** the 1,900-byte movement boss
  now derives its four option records and final of sixteen history entries
  from the real native Player arrays. The original pointer-conversion call
  is deliberately preserved after rejecting direct-reference controls.
  Complete replay remains 66 fields/23 calls/122 ordinary differences, with
  540 bytes/40 relocations of scoped exact providers. See
  [Player native loop review](TH09_PLAYER_NATIVE_LOOP_EXTENTS_20261107.md).

- **Etama exact sprite-script table and loop:** the 832-byte exact
  AddedCallback now derives its 23-type initialization bound from the real
  23×5 read-only script table, distinct from the controller's 32 allocated
  sprite slots. An independent original-bound check covers all **460 table
  bytes and 7 source pointer operands**; all 4 exact same-source owners
  remain exact (1,667 bytes / 73 relocations). See
  [Etama sprite table proof](TH09_ETAMA_SPRITE_SCRIPT_TABLE_20261107.md).

- **SaveReplay original header spans:** the native 0xC0 wire header now
  determines its 0xAB-byte checksum range, 0xA8-byte obfuscation range and
  header file-write count; the user-info NUL is derived from its 0x0C-byte
  record. The complete 1,238-byte boss remains two bytes nonexact, while
  12 original-exact siblings / 4,093 bytes / 232 fields pass focused replay.
  See [SaveReplay native header-span review](TH09_REPLAY_SAVE_NATIVE_HEADER_SPANS_20261107.md).

- **ScreenEffect native shake-envelope phases:** its three native shared
  parameter words (+0x1C/+0x20/+0x24) now have a typed fade-in/hold/fade-out
  view and the envelope borrows the actual Timer. Full 336-byte original-
  bound owner remains 26 bytes nonexact, while all **99 exact functions**
  in 21 affected compiler carriers / 11 TUs (38,327 bytes / 2,315 fields)
  were rebuilt and verified. 79 internal compiler-label names were rebound
  with original unchanged target addresses. See
  [ScreenEffect phase review](TH09_SCREEN_EFFECT_ENVELOPE_PHASE_LAYOUT_20261107.md).

- **Etama native bullet-pool extents:** the real 176/361 bullet records
  carry 175/360 usable entries plus physical sentinel records, while its
  2,216-byte exact OnUpdate traverses a contiguous 536-record scan and
  its 1,932-byte SpawnSingleBullet uses the same real bounds. The init/reset
  leaves share this typed capacity. All **30 scoped Bullet exact owners**
  (21,091 complete bytes / 764 fields) pass source-cold original-bound
  replay; compiler-local table labels were target-VA-checked. See
  [Etama pool-source review](TH09_ETAMA_NATIVE_POOL_LIMITS_20261107.md).

- **Replay native recording frame capacity:** true `u16` inputs use
  2-byte frames, 7,200-byte streams and 3,600-frame capacity. Playback's
  FPS lookahead and 30-frame sampling give 121 FPS bytes; recorder rollover
  remains at 3,598. These same typed constants now drive SaveReplay byte
  sizes and wire fields. All **65 affected exact functions / 26,115 physical
  bytes / 1,431 fields** pass source-cold original checks across 7 carriers;
  SaveReplay still has two residual bytes. See
  [replay capacity review](TH09_REPLAY_FRAME_CAPACITY_20261107.md).

- **Replay native configuration value snapshot:** the true 0xCC-byte
  `GameConfiguration` object at replay `+0xDC` is now assigned in both
  directions between live GameManager and the serialized replay payload.
  Both original compiler profiles retain 12 exact ReplayInput owners;
  focused playback replays 15 exact dependencies / 4,141 bytes / 234 fields,
  and SaveReplay retains its two-byte residual. See
  [native config snapshot review](TH09_REPLAY_NATIVE_CONFIGURATION_SNAPSHOT_20261107.md).

- **Enemy native trail renderer relationship:** a single `EnemyView`
  owns 96 × 0x1C-byte history samples and 192 × 0x1C-byte draw vertices;
  each strip sample creates two vertices. The 4 native draw-group buckets and
  2 secondary ANM VMs now drive both the 3,900-byte OnUpdate boss and exact
  1,758-byte DrawImpl. **91 exact ECL dependencies** (41,868 bytes / 1,818
  fields) and **4 exact draw functions** (1,922 bytes / 61 fields) pass
  focused original-bound replay; OnUpdate stays 13 bytes nonexact. See
  [Enemy trail capacity review](TH09_ENEMY_NATIVE_TRAIL_RENDER_CAPACITY_20261107.md).

- **Effect native dynamic pool + failure sentinel:** `EffectManager`
  now derives all allocation/reset/release counts and the failed-spawn
  pointer from one genuine active-primary + active-secondary + one sentinel
  model. Its 475-byte exact OnUpdate retains the observed live primary
  count read order, while 327-byte Create and both Spawn source owners share
  the same record semantics. Focused original replay: **16 exact functions,
  2,571 bytes / 101 relocations**; 51-byte AddedCallback remains 8 bytes
  nonexact. See [Effect native dynamic pool](TH09_EFFECT_NATIVE_DYNAMIC_POOL_20261107.md).

- **Title replay native slot geometry:** 50 shared original replay paths
  and 50 complete replay records are rendered as two 25-row columns, with
  ten stage records per replay stream. Source-derived C++ lengths now drive
  both the 1,280-byte open DrawReplayMenu boss (29 differences unchanged)
  and the 800-byte exact DrawReplaySave owner (41 fields unchanged). The
  ten-stage bound does not describe menu animation or the 6-by-16 keyboard. See
  [Title replay slot review](TH09_TITLE_REPLAY_NATIVE_SLOTS_20261107.md).

- **Replay native two-side input gate:** recording/playback use one
  three-entry global ReplayInputState array (third input is shared) but
  the real GameConfiguration exposes only two side-focus mode bytes.
  Typed original array extents now drive both capture/playback gates and
  the shared state cursor, preserving **12 exact ReplayInput functions /
  4,093 bytes / 232 fields**. Both large Replay frontiers remain unchanged.
  See [Replay native input gates](TH09_REPLAY_NATIVE_INPUT_GATES_20261107.md).

- **Native Title name keyboard:** a single six-row × sixteen-column
  keyboard has 96 keys, skips index 93 and uses 94/95 for space/confirm.
  The actual 1,825-byte replay-save and 966-byte result name-entry bosses
  now share this type with the exact 800-byte save renderer. **49 exact Title
  dependencies / 22,710 physical bytes / 1,083 fields** pass focused
  original-bound replay (including nine target-VA-checked compiler-private
  labels). See [Title name keyboard review](TH09_TITLE_NATIVE_NAME_KEYBOARD_20261107.md).

- **Native Enemy active-pool/sentinel model:** the genuine 129 × 0x5430-byte
  array holds **128 active scan/spawn records plus one terminal sentinel**.
  The 3,900-byte OnUpdate boss and seven exact manager clear/lookup/release/
  spawn owners now derive their counts and failure index from one shared
  `EnemyNativePoolCapacity` type. Scoped original checks pass **90 exact
  owners / 31,366 physical bytes / 1,429 fields**; Enemy OnUpdate remains
  13 bytes different and RunEcl 30. Two independently target-checked ECL
  compiler-private `$L` names were refreshed without repointing relocations.
  See [native Enemy pool checkpoint](TH09_ENEMY_NATIVE_POOL_CAPACITY_20261107.md).

- **Replay manager native cursor array:** the original 0x164-byte manager
  has three contiguous 16-bit input-stream cursor pointers at +0x0C/+0x10/
  +0x14, followed by FPS at +0x18. Record and playback now share one
  `inputCursors[3]` with the wire's three streams; side gating remains two
  configurable sides. All **65 related exact functions / 26,115 bytes /
  1,431 fields** recompile with original equality. The 1,238-byte SaveReplay
  still has two differences; 540-byte BeginPlaybackStage still has 39
  overlapping differences plus two missing bytes. Nine target-checked Title
  compiler-local labels have refreshed names, not destinations. See
  [native stream cursor review](TH09_REPLAY_NATIVE_STREAM_CURSORS_20261107.md).

- **Gameplay Setup native 2+1 side topology:** the genuine GameManager
  exposes two playable 0x38-byte sides and two configuration input modes;
  setup/teardown additionally use a third physical 0x38-byte record solely
  for the shared EffectManager at +0x7C. All four loops, setup/cleanup and
  release paths now derive these bounds from native arrays rather than
  unrelated literals. **12 exact owners / 3,987 bytes / 335 fields** pass
  original-bound scoped replay. The 1,689-byte setup boss remains a
  1,682-byte nonexact candidate with 694 unequal overlaps and seven missing
  target bytes. See [native setup topology](TH09_GAMEPLAY_NATIVE_SIDE_TOPOLOGY_20261107.md).

- **Native gameplay setup side geometry:** original GameManager exposes two
  playable 0x38-byte sides. Setup/cleanup physically owns a third slot
  (+0x70), used for shared EffectManager at +0x7C—not a third player.
  Typed bounds now drive the setup boss, exact update and cleanup/delete
  owners. **12 exact functions / 3,987 bytes / 335 fields** pass across
  three affected carriers. The 1,689-byte gameplay setup boss still emits
  1,682 bytes, with all 172 fields, 694 overlapping differences and seven
  missing bytes. See [native setup side geometry](TH09_GAMEPLAY_NATIVE_SIDE_GEOMETRY_20261107.md).

- **RunEcl native ECL wire byte view:** the real 0x0C-byte instruction
  prefix and packed 4-byte union operands now drive byte indexing as well
  as existing integer/float readers. Original-bound scoped ECL replays
  pass **87 exact functions / 30,583 bytes / 1,409 fields** across five
  affected source profiles. The full RunEcl boss remains **15,564/598/30**
  (opcode 4: seven, opcode 155: 23); Enemy OnUpdate remains **3,900/97/13**.
  Twenty changed compiler-private ECL labels have verified unchanged
  original destinations. See [native ECL byte indexing](TH09_ECL_NATIVE_WIRE_BYTE_INDEXING_20261107.md).

- **RunEcl native wire byte indexing:** all current ECL opcode byte readers
  now index the same physical 4-byte `Th09EclWireArgument` union as the
  integer/float readers; the raw byte-pointer cast is gone. The complete
  **15,564-byte RunEcl boss** has the *identical* original-linked code and
  stays **598 fields / 30 ordinary differences** (opcode4 7, opcode155
  23). Its 3,900-byte Enemy sibling keeps 13 differences. **87 exact
  functions / 30,583 bytes / 1,409 fields** pass in the affected
  carriers. Twenty compiler-private symbol names were target-VA-verified
  and rebound without changing target destinations; old probe scenarios
  remain strict. See [native ECL wire byte review](TH09_ECL_NATIVE_WIRE_BYTE_ACCESS_20261107.md).

- **Effect creator real playable-side references:** the 327-byte exact
  Create now directly borrows the two native GameManager side records,
  while a separate shared-effect path addresses physical record 2 without
  indexing beyond the native two-element array. The exact 475-byte
  Effect OnUpdate and all other affected exact functions remain preserved:
  **16 exact owners / 2,571 bytes / 101 fields** under focused original
  replay. The 51-byte AddedCallback still has 8 differences. See
  [Effect native playable-side review](TH09_EFFECT_NATIVE_PLAYABLE_SIDE_20261107.md).

- **Native Player collision-query record capacity:** the genuine +0x36C
  Player query owns 128 × 0x30-byte records followed by a signed count at
  +0x1804. Enemy's 3,900-byte OnUpdate, exact 205-byte AppendBoxRecord,
  exact 245-byte AppendLaserRecord and exact 95-byte AppendCircleRecord
  now share the array-derived capacity. **85 exact functions / 30,196
  bytes / 1,391 fields** pass affected source-cold checks; the Enemy boss
  remains 13 bytes different. See
  [native query capacity review](TH09_PLAYER_COLLISION_QUERY_CAPACITY_20261107.md).

- **One native Player circle append method across ExAttack:** the exact
  95-byte Player collision method is now actually declared on the same
  `PlayerCollisionQueryStateView` that owns box/laser append. Seven complete
  ExAttack callback owners (including **1,436-byte Type18/24**) now use
  its real member; **11 compiled call fields** all preserve the original
  0x00440D30 target. Source-cold comparisons pass **8 full functions /
  4,907 physical bytes / 203 fields**. Five changed Type14/22 `$L` names
  retain proven same-section targets. See
  [native ExAttack circle owner](TH09_EXATTACK_NATIVE_CIRCLE_OWNER_20261107.md).

- **ExAttack Type4 real native Player/ANM receiver:** the complete exact
  739-byte Type4 update now uses `PlayerLifecycleView *` and `AnmVm *`
  directly instead of two private opaque duplicates. Five verified original
  REL32 sites bind to the actual 18-byte `AnmVm::SetInterrupt` at 0x406790;
  its five circle collision calls still use the real 95-byte Player method.
  **8 affected exact owners / 1,246 bytes / 52 fields** pass focused
  source-cold original checks. See
  [Type4 native receiver proof](TH09_EXATTACK_TYPE4_NATIVE_PLAYER_ANM_20261107.md).

- **Native GameManager/Player across six ExAttack bosses:** the original
  475/422/824/496/420/739-byte update owners now use the real shared
  GameManager side array and Player collision/animation fields, eliminating
  six unrelated local owner overlays. All **25 GameManager data fields**,
  **6 Player CheckBulletCollision calls**, and five Type14/22 private labels
  retain their independently reviewed original targets. With Type4's real
  AnmVm interrupts, scoped source-cold replay passes **13 distinct exact
  owners / 3,883 physical bytes / 183 relocations**. The 862-byte Player
  collision boss remains nonexact. See
  [native ExAttack runtime owner proof](TH09_EXATTACK_NATIVE_GAMEMANAGER_PLAYER_20261107.md).

- **Six ExAttack update owners now borrow real Player and GameManager:**
  Type01/4/13/14-22/16-23/17 use the genuine two-side GameManager array,
  Player +0x36C query, and header24.anmFile98 (+0xBC) instead of local
  prefix clones. Type4 additionally keeps five original target-linked
  native `AnmVm::SetInterrupt` calls. **6 exact owners / 3,376 physical
  bytes / 159 fields** source-cold match the immutable retail PE; 41
  changed compiler symbol *names* preserve original destinations.
  The called Player collision boss remains nonexact. See
  [native ExAttack Player/GameManager source review](TH09_EXATTACK_NATIVE_GAMEMANAGER_PLAYER_20261107.md).

- **1,436-byte Type18/24 now uses native Player/VM/angle:** replaced
  synthetic GameManager/Player/VM classes with the actual side array,
  Player +0xBC ANM header/+0x36C query, four 0x2A4-byte PlayerAnmVm
  instances and the original exact angle-view receiver. All **17**
  name-changed COFF fields retain their original PE destinations and
  addends. Source-cold replays pass **10 distinct exact functions /
  2,064 physical bytes / 73 fields** including the providers; this is
  not new authored exact credit. See
  [Type18 native Player/ANM/angle evidence](TH09_EXATTACK_TYPE18_NATIVE_PLAYER_VM_20261107.md).

- **Player native collision-region allocation recovered in a large boss:**
  the real Player now directly owns a 0x44-byte region at +0xB0BC,
  514-entry active/released pointer arrays at +0xB100/+0xB908 and two
  counters at +0xC110/+0xC114. The nonexact 862-byte
  `CheckBulletCollision` now scans the actual member, not a private
  reinterpret overlay. **41 related exact owners / 9,912 complete bytes /
  417 fields** source-cold match the original, including independent
  Type18/24 ExAttack caller proof. The boss still has a
  849-byte candidate, 799 unlike overlaps and 13 missing original bytes;
  no exact credit is added. See
  [Player native region member review](TH09_PLAYER_NATIVE_COLLISION_REGION_MEMBERS_20261107.md).

- **Real Player SpawnShots method and Charge boss call chain:** the
  exact 190-byte shot producer is now declared on actual
  `PlayerLifecycleView` and derives its 128-slot loop limit from the
  native Player shot array. The 1,210-byte charge boss retains both
  original-linked calls at 0x41F4C0 (still 1,197 candidate bytes and 13
  missing), and the exact Effect Type35 leaf uses the same method.
  **24 affected exact functions / 5,691 bytes / 292 fields** pass scoped
  original replay. See [native SpawnShots owner](TH09_PLAYER_NATIVE_SPAWN_SHOTS_20261107.md).

- **Player State3 real region producer fixed:** native `PlayerLifecycleView`
  now exposes the real collision-region producer type; the three
  91/91/100-byte exact State3 effect leaves no longer redeclare conflicting
  private region structs. This repairs a reproducible VC7.1 C2011 failure
  and restores **11 exact region/State3 owners / 759 bytes / 29 fields**.
  The 1,210-byte ChargeAttack boss retains its original nonexact frontier.
  See [native Player State3 region proof](TH09_PLAYER_STATE3_REAL_REGION_PRODUCER_20261107.md).

- **Player State3 and resource methods now have native receiver:**
  the exact 51-byte clamp and three complete 91/91/100-byte charge effect
  helpers are declared/implemented on `PlayerLifecycleView`, sharing its
  actual +0x30388 resource, +0x1B88 position and native collision-region
  producer. The 1,210-byte ChargeAttack boss retains 70 bound fields,
  58 ordered calls and its existing 13-byte deficit. **25 exact source
  owners / 5,724 bytes / 304 fields** pass source-cold replay.
  See [native Player resource/State3 audit](TH09_PLAYER_NATIVE_RESOURCE_STATE3_20261107.md).

- **Full Player ChargeAttack boss now has its real native receiver:**
  1,210-byte original ChargeAttack is declared and implemented on
  `PlayerLifecycleView`, and the exact 522-byte Player OnUpdate calls
  it directly. The original 0x41F810 call destination, 70 charge fields,
  58 calls and 1,197-byte candidate remain fixed; **13 bytes are still
  missing**, so no exact credit. Related affected source-cold replays
  pass **28 exact functions / 6,024 bytes / 308 fields**. See
  [native ChargeAttack owner review](TH09_PLAYER_NATIVE_CHARGE_BOSS_20261107.md).

- **Complete Player ChargeAttack owner now native:** the entire
  1,210-byte original charge boss is declared/implemented as
  `PlayerLifecycleView::UpdateChargeAttack`; exact Player OnUpdate now
  calls it without the no-storage derived class. The duplicate
  `PlayerChargeAttackView.hpp` has been deleted. Full boss still has
  **1,197/1,210 bytes, 70 field identities, 58 original calls and
  13 missing bytes**; 28 related exact owners / 6,024 bytes / 308 fields
  pass scoped cold replay, with 17 independently proven private label
  renames. See [native ChargeAttack owner](TH09_PLAYER_NATIVE_CHARGE_BOSS_20261107.md).

- **Type35 Effect side owner now the native GameManager allocation:**
  the exact 72-byte initializer and 38-byte updater borrow
  `PlayerSideStateView::player04` instead of a local four-byte-prefix
  side clone. The exact 327-byte EffectManager Create and 190-byte native
  Player SpawnShots provide original-backed allocation/callee evidence.
  See [Effect35 native Player side](TH09_EFFECT35_REAL_PLAYER_SIDE_20261107.md).

- **Real native Player shot/SHT fields in SpawnShots:** the complete
  190-byte exact shot producer now reads actual `PlayerShtFileView`
  descriptors and `PlayerShotCtorView shots[128]`, with native +0x47C
  collision callback/+0x480 descriptor slots, not independent overlay
  classes. **24 affected exact owners / 5,691 bytes / 292 fields**
  source-cold reproduce the original. The 1,210-byte ChargeAttack boss
  retains its full nonexact 13-byte deficit. See
  [native shot/SHT fields](TH09_PLAYER_NATIVE_SHOT_SHT_FIELDS_20261107.md).

- **Player ChargeAttack now has no unused zero-storage helper facades:**
  SpawnShots, resource clamp and the three exact State3 effects now include
  the native Player header directly. The last three empty legacy derived
  role headers are deleted; all five helper methods and complete
  1,210-byte ChargeAttack remain original-target-bound. **28 exact
  affected functions / 6,024 bytes / 308 fields** pass scoped replay;
  the boss still lacks 13 target bytes. See
  [native Player/Charge leaf consolidation](TH09_PLAYER_NATIVE_BOSS_LEAF_CONSOLIDATION_20261107.md).

- **1,900-byte Player movement boss now uses typed focus input:** the
  same native ReplayInputState slot supplies directional `IsHeld` and
  `auxiliary2A` focus history; no byte-offset/reinterpret overlay remains.
  The full boss retains **1,900 bytes / 66 fields / 23 calls / 122
  ordinary differences** and both jump tables. Its exact 522-byte
  caller and 18-byte VM interrupt leaf pass source-cold replay. See
  [typed focus-input original comparison](TH09_PLAYER_MOVEMENT_TYPED_FOCUS_INPUT_20261107.md).

- **485-byte Title result boss uses genuine PLST unlock capacity:**
  the 3×16 native `characterUnlocked` array and 16-entry Title cursor
  character order now share a compile-time checked full 16-slot extent;
  the separate 14 normal unlock characters remain distinct. The complete
  485/484-byte boss keeps **34 fields / 9 calls / 218 unlike overlap
  bytes + 1 missing**. **41 existing exact Title functions / 18,972
  bytes / 953 fields** pass scoped cold replay. Nine compiler-private
  start-menu names retain verified original destinations. See
  [native Title result unlock audit](TH09_TITLE_NATIVE_RESULT_UNLOCK_CAPACITY_20261107.md).

- **540-byte Replay BeginPlaybackStage now uses its real 3×10 frame-table
  row address:** replaces untyped byte-offset cast with the native
  `replayData->frameStart[stage]` array row expression, intentionally *not*
  testing a frame data pointer. **538-byte candidate / 32 fields / 5
  calls / 39 unlike overlaps + 2 missing bytes** remain unchanged.
  **12 exact same-source functions / 4,093 bytes / 232 fields** pass
  scoped replay; SaveReplay still has 2 residual bytes. See
  [native replay playback row audit](TH09_REPLAY_NATIVE_PLAYBACK_FRAME_ROW_20261107.md).

- **Player/Enemy nearest-target tracking is now one native POD member:**
  Player +0x30364 holds three position floats, opaque 12-byte reset state,
  +0x3037C homing Enemy pointer and +0x30380 last status word, without
  introducing an extra `Float3` constructor. The **3,900-byte Enemy
  OnUpdate** and exact Enemy cleanup, Player constructor/OnUpdate and
  shot callbacks use the same native allocation. **108 exact source-cold
  owners / 36,289 bytes / 1,712 fields** pass; the Enemy boss retains
  its **97 fields / 69 calls / 13 ordinary byte residuals**. ECL's two
  and PlayerRuntime's 17 private `$L` changes retain verified original VAs.
  See [native Player/Enemy tracking POD review](TH09_PLAYER_ENEMY_NATIVE_TRACKING_POD_20261107.md).

- **Three exact shot callbacks now use native Player Shot/SHT records:**
  471-byte Update Type1, 248-byte Update Type3 and 139-byte Spawn Type7
  no longer invent shot/timer/descriptor prefix structs. They consume the
  same 0x484 PlayerShotCtorView, genuine +0x14/+0x18 SHT angle/speed,
  exact ZunTimer conversion (+0x435F00) and Player position conversion
  (+0x4343D0). **108 existing exact functions / 36,295 bytes / 1,711
  fields** pass scoped replay with original addresses preserved. The
  Enemy 3,900-byte boss remains 13 bytes different. See
  [native Player shot callback owner audit](TH09_PLAYER_NATIVE_SHOT_CALLBACK_OWNERS_20261107.md).

- **Full RunEcl native halfword wire access:** opcode 130/150 now read
  genuine signed 16-bit `halfWords[2]` in the four-byte ECL argument union,
  replacing a byte-base-to-short pointer cast. The 15,564-byte RunEcl
  boss stays **598 fields / 30 unlike ordinary bytes (7 opcode4 + 23
  opcode155), zero table differences**. **83 existing exact ECL owners /
  29,856 physical bytes / 1,389 fields** pass scoped replay; 20 local
  `$L` name changes retain proven same original addresses. See
  [native ECL wire halfwords](TH09_ECL_NATIVE_WIRE_HALFWORD_INDEXING_20261107.md).

- **Supervisor frame-queue methods have their actual native receiver:**
  the four exact 213/191/119/49-byte queue methods belong directly to
  `Supervisor`, which owns the 20-entry +0x47C and two-entry +0x56C frame
  arrays. Mode4 reset and network message call the same original
  `0x0042E9E0` method with five unchanged call fields. **6 exact full
  owners / 1,557 physical bytes / 59 fields** pass original-bound replay,
  including the network handler's 40-byte switch/table tail. Eight `$L`
  names retain verified original VAs. The network service big boss remains
  nonexact. See [native Supervisor queue methods](TH09_SUPERVISOR_NATIVE_FRAME_QUEUE_METHODS_20261107.md).

- **1,633-byte Supervisor network service now calls real native queue
  methods:** all ten InsertPredicted/InsertReceived/Pop/Sync callsites
  use the actual `Supervisor` receiver, not an empty derived queue view.
  Ten natural compiler controls reproduce identical **1,627 candidate
  bytes / 106 referents**, with original **1,381 overlap differences + 6
  missing bytes**. Its exact 996-byte Supervisor OnUpdate caller and four
  queue leaves pass **5 owners / 1,568 bytes / 93 fields** after 40
  original-VA-audited private labels. See
  [network service native queue closure](TH09_SUPERVISOR_NETWORK_SERVICE_NATIVE_QUEUE_20261107.md).

- **Network message's three queue calls now use true `g_Supervisor`:**
  complete **808-byte physical handler** (768 code + 40 switch/table)
  and 46 original PE-bound fields are byte-identical under five natural
  C++ models. All three existing InsertReceivedFrame calls remain at
  offsets +0x89/+0x156/+0x16F targeting 0x42E9E0. Eight changed
  compiler-private symbols were proved at their original target VAs.
  See [native network message queue audit](TH09_NETWORK_MESSAGE_NATIVE_SUPERVISOR_QUEUE_20261107.md).

- **Supervisor has no remaining empty queue façade:** the exact
  177-byte `ResetTitleMode4Supervisor` now defines its native
  `TitleSupervisorView : Supervisor` and directly uses the real queue
  methods. The empty `SupervisorFrameQueueView.hpp` is deleted from all
  source. Scoped source-cold comparisons pass **7 exact owners / 2,553
  physical bytes / 151 fields**, including complete network-message
  switch data and the exact Supervisor caller. The separately rebuilt
  1,633-byte network service keeps its original 1,627-byte candidate,
  106 fields and 1,381-plus-six difference frontier. See
  [native Supervisor queue façade removal](TH09_SUPERVISOR_REMOVE_QUEUE_FACADE_20261107.md).

- **TitleScreen core and both replay draw functions now share the entire
  native 0x1B388-byte Title owner:** removed the conflicting separate
  0x1B234 replay-only class. Real replay allocation pointers +0x681C,
  0x1EC-byte records +0x68E4, `currentReplay` +0xC8FC, 57 ANM VMs
  +0x11BA0 and config +0x1B2BC now have one common C++ allocation.
  **42 exact core/draw-save functions / 19,772 bytes / 994 fields** pass
  source-cold; **1,280-byte replay-menu boss** retains 57 fields,
  13 calls and its existing 29 ordinary residual bytes. Nine Title
  compiler-private label names have verified immutable original VAs.
  See [complete native Title owner](TH09_TITLE_NATIVE_COMPLETE_SHARED_OWNER_20261107.md).

- **The exact 2,045-byte Title Options boss now uses the same complete
  0x1B388 native Title owner as core/Replay draws:** deleted a second
  same-named local TitleScreen class, moved native ANM resource ownership
  to the shared header, restored the +0xC910/+0xC918 options fields, and
  kept `configSnapshot` as 0xCC-byte POD (not a newly constructed
  GameConfiguration) with original +0xAD/+0xB1 bytes. **44 exact Title
  owners / 21,851 bytes / 1,132 fields** pass scoped original replay;
  ReplayMenu's 1,280-byte boss remains 29 bytes unlike. See
  [native Title Options/serialized POD proof](TH09_TITLE_NATIVE_OPTIONS_POD_OWNER_20261107.md).

- **Exact 1,939-byte ResultDraw now uses the complete native TitleScreen
  class:** removed its separate 0x1B234-byte Title prefix and borrowed
  the genuine 0x1B388-byte owner shared with core, Options and Replay
  draws. Strictly preserved the **1,939-byte/82-field ResultDraw**, all
  **45 exact affected owners / 23,790 bytes / 1,214 fields**, and the
  **1,280-byte ReplayMenu** (29 unlike bytes) plus 485-byte ResultUpdate
  (218 overlap + 1 missing) nonexact frontiers. Nine original-VA-audited
  private labels are name-only. See
  [complete native ResultDraw owner](TH09_TITLE_RESULT_DRAW_NATIVE_FULL_OWNER_20261107.md).

- **Exact 1,939-byte ResultDraw shares the real 6×16 native Title name
  keyboard:** rows, columns, last row and space/confirm columns now use
  `TitleNativeNameKeyboard` already shared by the Title input state
  machines and exact 800-byte ReplaySave draw. Four natural C++ models
  preserve **all 1,939 code bytes and 82 fields**. The affected
  **45 exact Title functions / 23,790 bytes / 1,214 fields** remain
  scoped-original-bound; the nonexact 1,280-byte ReplayMenu retains 29
  different bytes. See
  [native Title ResultDraw keyboard](TH09_TITLE_RESULT_DRAW_NATIVE_KEYBOARD_20261107.md).

- **Exact 1,939-byte Title ResultDraw shares the native six-by-sixteen
  name-entry keyboard:** its full renderer now uses the same
  `TitleNativeNameKeyboard` source constants as exact ReplaySave rather
  than repeating literal row/column/special-key values. The entire
  1,939-byte ResultDraw and its **82 original fields** remain strictly
  byte-exact; 178 focused Title tests pass. See
  [native ResultDraw keyboard proof](TH09_TITLE_RESULT_DRAW_NATIVE_KEYBOARD_20261107.md).

- **Player native 162-byte respawn-resource leaf and 862-byte collision
  boss:** real `PlayerLifecycleView::AddRespawnResource` replaces the
  artificial four-byte PlayerState4 method owner. The exact 400-byte
  PlayerCheckState4 and 852-byte UpdateBeforeState and the complete open
  CheckBulletCollision now call the same original entry 0x41BC90 without
  a receiver cast. **109 exact functions / 36,467 physical bytes / 1,709
  fields** pass scoped cold replay; **26 original fields / 12 calls /
  13 missing bytes** remain in the collision big boss. Twenty-five
  compiler-private Player/ECL labels are original-VA audited. See
  [native Player respawn resource proof](TH09_PLAYER_NATIVE_RESPAWN_RESOURCE_BOSS_20261107.md).

- **Native Player 12-byte SetUpdateState is shared by the entire state
  chain:** actual `PlayerLifecycleView` now owns the setter at 0x403C20,
  used by the **3,238-byte exact selector**, its 412-byte physical
  resolver, exact State3 effects/Runtime and the nonexact 3,900-byte
  Enemy OnUpdate. **110 exact affected functions / 39,191 physical bytes /
  1,846 fields** pass scoped replay; 37 compiler-private Player/ECL/
  selector labels were checked against immutable original PE VAs.
  Enemy retains 97 fields/69 calls/13 unlike bytes; RunEcl 598 fields,
  30 unlike bytes. See
  [native Player state setter review](TH09_PLAYER_NATIVE_SET_UPDATE_STATE_20261107.md).

- **Native Player state setter across big Enemy, selector and ECL owners:**
  original-exact **12-byte `SetUpdateState` @0x403C20** now lives on
  `PlayerLifecycleView`, no longer a fabricated four-byte method owner.
  Full **3,238-byte selector** and **3,900-byte Enemy OnUpdate** use its
  original target. Source-cold proof: **110 exact complete owners /
  39,191 physical bytes / 1,846 fields**; 37 compiler-private Player/ECL
  selector labels were audited at immutable PE targets. The Enemy boss
  retains **97 fields / 69 calls / 13 unlike bytes**; RunEcl retains
  **598 fields / 30 unlike bytes**. See
  [native Player state setter audit](TH09_PLAYER_NATIVE_UPDATE_STATE_BOSSES_20261107.md).

- **Full 2,968-byte FrontMessage update physical owner now points to
  the real native Player:** replaces its 0x303FC private prefix model with
  `PlayerLifecycleView *` and uses true `SetUpdateState @0x403C20` plus
  +0x303C8 timer. Original complete owner stays **2,968/2,968 bytes and
  all 167 fields**, including 30 PE-audited compiler-private labels.
  See [native FrontMessage Player receiver](TH09_FRONTMESSAGE_NATIVE_PLAYER_RECEIVER_20261107.md).

- **Full 2,968-byte FrontMessage script now uses the real FrontSide HUD:**
  removed its duplicate +0xA678 prefix and 0x558-byte ANM resource view;
  the +0x18 gameplay side pointer is the actual native `FrontSide *`.
  Both exact LoadScript67 callsites preserve offsets +0x905/+0x910 and
  original target 0x41A210, and the complete interpreter remains
  **2,968/2,968 bytes / 167 fields**. Scoped replay passes **17
  exact FrontMessage/FrontSide owners / 7,094 bytes / 343 fields**;
  30 compiler-private labels retain original-VA-audited definitions.
  See [native FrontSide in FrontMessage](TH09_FRONTMESSAGE_NATIVE_FRONTSIDE_HUD_20261107.md).

- **Full FrontMessage 2,968-byte script now uses real GameManager and
  Background:** replaced the independent manager/side projections with
  `AsciiGameManagerView` and its two actual 0x38-byte sides. Opcode `0x1C`
  correctly writes `Background::pendingStageLabel18` through native side
  +0x00 instead of a falsely inferred score-state object. All **37
  original GM fields**, 3 original mode-method targets, **30 audited
  local labels**, and complete **2,968/2,968 physical code/table bytes**
  are preserved. Scoped closure passes **22 exact owners / 7,230 bytes /
  344 fields**. See [native FrontMessage GameManager/Background](TH09_FRONTMESSAGE_NATIVE_GAMEMANAGER_BACKGROUND_20261108.md).

- **FrontMessage uses the native third ReplayInputState instead of an
  invented input global:** the five original effective pointer operands
  at 0x004ACF34 now bind to `g_ReplayInputStates` base 0x004ACE18 plus
  true 0x11C COFF addends. Its four IsHeld and one WasPressed call target
  the exact original Replay input leaves. **The full 2,968-byte physical
  FrontMessage interpreter and all 167 PE-bound fields remain exact**;
  scoped cold replay passes **16 exact owners / 7,136 bytes / 399
  relocations**, and 30 name-only local labels have independently proved
  original destinations. See
  [native FrontMessage shared Replay input](TH09_FRONTMESSAGE_NATIVE_REPLAY_INPUT_SHARED_SLOT_20261108.md).

- **Full FrontMessage script also uses the actual native Supervisor audio
  receiver:** removed `FrontMessageSupervisorView` and called genuine
  `Supervisor::StopAudio/PlayMusic/FadeOutMusic` with all four original
  global references at 0x4B3100 and exact method targets 0x42FD30,
  0x431930 and 0x42FE20. The complete **2,968/2,968 physical bytes and
  167 fields** remain original-bound; scoped cold replay protects
  **10 exact owners / 4,061 bytes / 249 fields** and 30 same-owner
  PE-verified compiler-private labels. See
  [native FrontMessage Supervisor audio](TH09_FRONTMESSAGE_NATIVE_SUPERVISOR_AUDIO_20261108.md).

- **Real opponent-selection owner inside the full 1,689-byte gameplay
  setup boss:** the exact **799-byte `SelectOpponentConfiguration`** now
  belongs to the genuine `AsciiGameManagerView`, with native +0xD8 used
  characters, +0xF0 opponent pointer, +0x138 selector and side +0x2C
  parameter. **Four original GameplaySetupThread calls** directly use
  `g_GameManager` instead of an unrelated 0x13C-byte prefix. The full
  worker retains its **1,682-byte candidate / 172 fields / 35 calls /
  694 overlapping differences + 7 missing bytes**. Scoped source-cold
  replay preserves **18 exact functions / 6,854 bytes / 442 fields**,
  including 30 PE-audited private FrontMessage labels. See
  [native GameManager opponent-selection review](TH09_GAMEMANAGER_NATIVE_OPPONENT_SETUP_BOSS_20261108.md).

- **The exact 799-byte opponent selector now belongs to real GameManager:**
  removed `GameManagerOpponentSelectionView` and its duplicate side prefix,
  restoring native +0xD8 `usedCharactersD8[16]` as the *same physical
  storage* as setup's `clearD8[16]`, plus genuine +0xF0/+0xF4/+0xFC/
  +0x11C/+0x138 selection fields. The **1,689-byte GameplaySetupThread**
  calls the native method four times; its **1,682-byte candidate, 172
  fields, 35 calls, 694 unlike overlap + 7 missing** remain unchanged.
  Scoped cold replay protects **18 exact owners / 6,854 bytes / 442
  fields**. See [native GameManager opponent owner](TH09_GAMEMANAGER_NATIVE_OPPONENT_SELECTION_20261108.md).

- **98-byte GameManager_CutChain uses real Supervisor +0x5B8**:
  deleted the invented 0x5B8-byte `GameManagerCutChainSupervisorView`
  and directly writes `g_Supervisor.framerateMultiplier5B8`. Three
  natural compiler models reproduce the exact **98/98 bytes and ten
  original fields**; 11 same-source exact GameManagerSetup owners /
  2,991 physical bytes / 243 fields pass scoped replay. The separate
  1,689-byte setup boss retains its 1,682-byte candidate/694-plus-seven
  deficit. See [native cut-chain rate](TH09_GAMEMANAGER_CUTCHAIN_NATIVE_SUPERVISOR_RATE_20261108.md).

- **PlayerRuntime now uses the real Supervisor viewport/frame-rate owner:**
  removed the duplicate `PlayerSupervisorRuntimeView` and its fake
  `PreparePlayerDraw` method; native `g_Supervisor` now owns the original
  SelectSide, ConfigureGameplayViewport and ConfigureScreenViewport calls
  and the +0x5B8 shot velocity factor. **21 exact PlayerRuntime functions /
  5,391 physical bytes / 288 fields** and eight exact Supervisor viewport
  siblings (1,530 bytes/106 fields) pass scoped original replay. Seventeen
  compiler-private Player labels were PE/COFF audited. The 862/849-byte
  Player bullet collision boss retains its 26 fields/12 calls/13-byte gap.
  See [native PlayerRuntime Supervisor owner](TH09_PLAYER_RUNTIME_NATIVE_SUPERVISOR_20261108.md).

- **PlayerRuntime uses genuine Supervisor side/viewport/time scale:**
  the 522-byte OnUpdate, 166/31-byte draw owners and 251-byte UpdateShots
  plus the complete 852-byte UpdateBeforeState now consume real
  `g_Supervisor` at 0x4B3100. The fake +0x5B8 PlayerSupervisor prefix
  is removed; +0x5B8 time scale and `SelectSide`/two viewport call VAs
  remain original. Scoped PE replay passes **21 exact owners / 5,391
  bytes / 288 fields**, with 17 private labels independently bound to
  their immutable original PE destinations. See
  [native Player/Supervisor owner](TH09_PLAYER_RUNTIME_NATIVE_SUPERVISOR_20261108.md).

- **The complete 1,540-byte physical Player ApplyReward now uses the
  true GameManager and Supervisor:** deleted both duplicate reward-only
  global receiver classes. Four Supervisor::SelectSide calls and four
  native popup-transform calls keep original VAs 0x401440/0x401680/
  0x4016B0; all **1,519 code bytes, 1,540 physical bytes, 91 fields
  and 33 calls** remain exact. Six local `$L` names were PE/COFF
  verified, and source-cold closure passes **13 exact reward/manager/
  viewport owners / 3,254 bytes / 205 fields**. See
  [native Player reward manager ownership](TH09_PLAYER_REWARD_NATIVE_GAMEMANAGER_SUPERVISOR_20261108.md).

- **GameManager native side AddScore across Enemy/Player/FrontResult:**
  33-byte exact `AsciiGameManagerSideView::AddScore` replaces a separate
  0x1C-byte side-score prefix; genuine side+0x1C runtime+0x08 score
  aliases the original `value08` word and preserves signed score/10.
  The full 1,540-byte reward and 553-byte FrontResult owners remain
  byte-exact; reward also uses real `ZunTimer::operator int` @0x435F00.
  Scoped replay protects **102 exact functions / 36,193 physical bytes /
  1,782 fields**. Enemy OnUpdate remains 3,900 bytes / 13 unlike;
  Player bullet collision remains 862/849 with 13 missing. See
  [native side score proof](TH09_NATIVE_GAMEMANAGER_SIDE_SCORE_20261108.md).

- **Original-exact FrontResult now consumes the complete native Player
  and GameManager:** removed the duplicate partial FrontResultPlayer/
  FrontResultSide/GameManager structs; real `PlayerHeaderStateView` at
  Player +0xA8/+0xB0/+0xB4/+0xB8 and `PlayerOwnerStateView::maxState08`
  at +0x30418 now directly feed the 553-byte bonus renderer. Both native
  gameplay sides, score runtime, phase word and exact AddScore leaf are
  shared. **119 related exact functions / 39,978 physical bytes / 1,891
  fields** pass original-bound scoped replay; 25 ECL/Player/reward
  compiler-private labels are original-VA audited. See
  [native FrontResult Player/Manager proof](TH09_FRONT_RESULT_NATIVE_FULL_PLAYER_MANAGER_20261108.md).

- **Native 0x44-byte Player owner-state now directly owns exact ApplyReward:**
  removed `PlayerOwnerRewardStateView` and typed the real Player
  +0x30410 owner with both constructed ZunTimers, counters and callback.
  The complete 1,540-byte reward physical function and its 91 original
  fields remain exact. Scoped cold replay protects **131 complete exact
  functions / 40,829 physical bytes / 1,936 fields**. Twenty-five
  Player/Reward/ECL private-label names are original-VA audited; Enemy and
  Player bullet collision nonexact frontiers remain unchanged. See
  [native full Player owner state](TH09_PLAYER_NATIVE_FULL_OWNER_STATE_20261108.md).

- **PlayerUpdateOwnerState's exact 126-byte fastcall now uses the real
  native 0x44-byte PlayerOwnerStateView:** removed a third local state and
  Player prefix plus fake `AdjustOwnerMetric`; the original +0x68 REL32
  call now resolves by true `AsciiGameManagerSideView::AddScore` at
  0x40F860. All 126 code bytes/9 fields remain exact; **23 full scoped
  exact functions / 6,964 bytes / 379 fields** pass cold replay and eleven
  PlayerRuntime local labels have PE-audited original VAs. The 862/849
  bullet-collision boss still lacks 13 bytes. See
  [native Player owner-state update](TH09_NATIVE_PLAYER_OWNER_STATE_UPDATE_20261108.md).

## Reconstruction frontiers

Counts below are **complete-owner noncrediting diagnostics**, not exact
credit. Use only the appropriate focused `--owner`, **never** `--owner all`.
This table retains the previously reviewed counts; recheck changed owners
against the current original and compiler results before a new commit.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode4 timer transport and opcode155 flag update. Keep all598 fields/tables and complete15564 bytes; actual Timer ABI and RNG ownership are already corrected. `--owner runecl`. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**,97 fields, 69 calls,170 blocks; **13 differing bytes** (29->13) | Draw index2, side/threshold4, descriptor-position copy7. Keep real20-byte Timer greater co-visible, all97 fields and3887 matching physical bytes. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **122** differing bytes | Lower-bound EAX/EDX transport and encoding cascade. Native framerate/coordinate owners and the shared SHT speed type are corrected;preserve1778 matching bytes and both tables. `--owner movement`. |
| **Player charge update** | 0x0041F810 | **1,210 target /1,197 candidate bytes**, 70 fields, 58 calls, 350/342 instructions; **1,160 overlap differences +13 missing** | Recover source chronology and missing13 bytes; preserve repaired SpawnShots transport and actual call/field identities. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions,34 fields,nine calls,40 agreeing ordinal blocks; **218** overlap +one missing | Character snapshot and remaining control-flow/byte deficit. Keep the exact Title setup siblings. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280/1,280 bytes**,376/376 instructions,57 original-bound fields,13 ordered calls,72 matching direct blocks; **29 differences** | Menu bank/record chronology. Preserve all57 fields,13 calls,72 blocks and the full original-bound residual guard. `--owner replaymenu`. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689 target /1,682 candidate**, 172 bound fields, 35 calls; **694 unequal overlaps +7 missing** | Shared limit/rate publication and side/rank chronology. All172 referent multiplicities now agree; old1689/171/904 is historical. Preserve native creators and the paired int Player producer. See [shared-rate review](TH09_SETUP_SHARED_RATE_CONTEXT_20261106.md). |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **799** overlap + 13 missing | Region producer, activeB100 pool and missing physical bytes. Preserve five same-source exact helpers and separate CalcItemBox/FindCollision proofs. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | **1,238/1,238 bytes**, 380 instructions, 53 fields, 23 call operands, 49 matching blocks; **2 differences at +0xB7/+0xBB (152→3→2)** | Frame-destination register remains. Shared full-pipeline index closes checksum SIB+3F6. Preserve1236 matching bytes and all12 exact same-source siblings. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Seed/tail chronology and two missing bytes. All32 ordered referents and first30 physical field positions are already restored. `--owner replay`. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | EDI/ESI owner allocation. Preserve310 matching bytes; timer-float visibility regresses physical size and x87 lifetime. |
| **Supervisor network service** | 0x00430AA0 | **1,633 target /1,627 candidate**,106 source fields;51 call-identity multiplicities agree |1381 unequal overlaps plus6 missing after the reviewed unsigned-16 queue API change (previously1376+7). Native RNG and packet lifetimes remain;wait/stack/input transport stay open. `inspect-supervisor-service.py` checks the full compiled owner; `--owner network` checks its queue dependencies. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Remaining receiver/allocation scheduling. Preserve43 matching bytes and genuine AnmLoaded/Chain interfaces. `--owner effect`. |

## Restart and scoped validation

```bash
git status --short --branch
git diff --check
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/validate-docs.py
# For the selected changed unit and any related exact siblings only:
nice -n 12 python3 -B scripts/build-match-unit.py --unit NAME
python3 -B scripts/compare-coff-function.py --unit NAME
python3 -B scripts/check-boss-frontiers.py --owner movement
```

Missing `build/matching/*.obj` is **normal after reviewed cache cleanup**.
Rebuild just the pinned source/profile unit. Nonexact diagnostics are
noncrediting; comparisons must preserve original bytes, physical tails,
relocation symbols/targets/types/addends, literal definitions, calls, graph
and unaffected exact siblings. Preserve, investigate and continue dirty
tracked/untracked work; no silent reset, no register hacks/inline assembly,
no wholesale archive deletion. New commit subjects: `gpt-web: ...`.

## Ignored local evidence, cleanup and restoration

The latest guarded cleanup removed **312 rebuildable generated cache files,
25,372,141 logical bytes (about 24.20 MiB)**, from `build/`; a second dry run
reported **zero remaining disposable cache files**. `build/` can still
contain empty directories; do not repeat old claims that its directory must
be absent. The original SHA, all three SHA-verified historical archive bundles,
**507 `.analysis/*.obj`** and **1,467 Ghidra-project files** were checked
unchanged. All these ignored local inputs are **absent from a plain Git clone**.
Only `scripts/clean-generated-artifacts.py` (preview, then `--apply`) is the
normal cache cleaner; analysis objects need explicit path/SHA-256 review.
Do **not** delete protected `.analysis/handoff-archive-*` packages,
`.analysis/ghidra-direct/`, candidate sources, SHA-pinned original or toolchain.

The local cleanup receipt is at
`.analysis/gpt-web-handoff-refresh-20261107/verified-cleanup-receipt.json`;
tracked cumulative history and restore guidance:
[RE_ARTIFACT_CLEANUP_HISTORY.md](RE_ARTIFACT_CLEANUP_HISTORY.md).
Previous November 7 cleanups reported **215,166,500 bytes** from a *separate*
prior cache/Git repack, and November 6 archived a separate older epoch.
Do **not** add historical totals as simultaneously recoverable bytes.
The [November 7 source proof log](TH09_GPT_WEB_NATIVE_BOSSES_20261107.md),
[legacy handoff history](RE_HANDOFF_HISTORY_20261107.md), and older dated
receipts remain navigable research evidence, **not live state**.

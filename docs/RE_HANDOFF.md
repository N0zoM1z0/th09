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

# TH09 short-function frontier

This is a live routing snapshot, not a second exactness ledger. The
authoritative live rows are `config/functions.csv`; `config/matches.csv` and
`config/match-units.toml` alone grant canonical exact credit. Re-filter the
ledger before starting work, and update this snapshot when a short function is
promoted. The original Japanese v1.50a target remains mandatory.

The filter is **confirmed authored + maintained source + not canonical exact +
target logical size at most 256 bytes**. It yields **11 functions**, of which
**2 are at most 128 bytes**. The latest short-function promotions are
`EtamaController::SpawnBulletPatternPrimary @ 0x004130F0` and
`SpawnBulletPatternSecondary @ 0x004131C0`; the overall canonical total is 858 exact
functions. Size is only a routing heuristic: this list is not a verified
call-graph leaf set, a difficulty ranking, or product-build progress. All
names and boundaries remain subject to their target-local ledger evidence.

| Target address | Bytes | Module | Non-exact source function |
| --- | ---: | --- | --- |
| 0x0040D1C0 | 51 | EffectManager | `EffectManager::AddedCallback` |
| 0x00410250 | 82 | EnemyManager | `EnemyAppendCollisionView::AppendPlayerCollisionBox` |
| 0x00424FB0 | 175 | TitleScreen | `TitleScreenView::SetCharacterCursorReverse` |
| 0x0042505F | 175 | TitleScreen | `TitleScreenView::SetCharacterCursorInactive` |
| 0x00442220 | 204 | Player | `PlayerShotDrawCallbackType1` |
| 0x00424EDD | 211 | TitleScreen | `TitleScreenView::SetCharacterCursorActive` |
| 0x0042E9E0 | 213 | Supervisor | `SupervisorFrameQueueView::InsertReceivedFrame` |
| 0x0042C290 | 220 | FileSystem | `FileSystem::TryDecryptFromTable` |
| 0x00432240 | 243 | SupervisorNetwork | `ParseNetworkConfigValue` |
| 0x00443B10 | 249 | Player | `PlayerPositionCallback30404Type4` |
| 0x00447620 | 249 | Player | `PlayerPositionCallback30404Type8` |

## Selection notes

Use the table above only for routing. Before changing any candidate, read its
live `config/functions.csv` row and recompile from the maintained source. The
notes below summarize only constraints that are still current; superseded probe
history belongs in `docs/KNOWLEDGE_BASE.md`.

- `Float3::FromAngleMagnitude` and `EffectPolarScaleView::SetFromAngleAxes`
  are both off the frontier. Fresh TH09 caller review fixes `void` contracts for
  both: callers preserve/use the receiver in ECX rather than consume EAX. Their
  bounded x87 FSINCOS bodies are source-family corroborated by committed TH08
  and TH095 evidence. The replayable units reproduce 32/32 bytes with no
  relocations; ordinary VC7.1 sin/cos spellings remain target-disproved.
- `DecodeFrontMessageString` is no longer on the frontier. Its two real callers
  pass `char[64]` stack buffers; preserving that array-reference type lets VC7.1
  naturally select the target private EAX/stack ABI and `ret 4`. A combined
  decoded assignment with postfix source/output increments also reproduces the
  target loop schedule, yielding a relocation-free 39/39 replay.
- `EffectManager::AddedCallback` is already target-sized at 51 bytes. The
  residual is the ESI/EDI assignment of callback argument versus ANM manager;
  bounded local-order, direct-store, `void *`, and profile probes do not
  change it.
- `AnmVmBase::AnmVmBase` is no longer on the frontier. The old 61/62
  `/O1 /Ob1` result was a profile dead end: `/O2 /Ob0` naturally emits
  the target 62-byte vector-constructor form with two independent ZunTimer
  constructor-address pushes; all six relocations replay exact.
- `Supervisor::Supervisor` is no longer on the frontier. Rechecking the old
  source assumption showed that the target dword OR at +0x5D4 is recovered
  naturally by a volatile 32-bit flags member and source-level compound OR
  after the clear; the replayable 62-byte unit is exact.
- `EnemyAppendCollisionView::AppendPlayerCollisionBox` is target-sized at
  82 bytes with both relocations solved; the residual is temporary-copy/argument
  scheduling. EnemyView::IntegrateMotion at 0x0040F9B0 is now canonical exact;
  restoring the TH08-family direct mirror branch reproduces all 188 bytes.
- `FrontMessageOwnerView::InitializeMessageRuntime @ 0x004181E0` is now
  canonical exact. The old LEA/push-order plateau was a source-shape error:
  direct `Setup` calls in the two game-mode branches let VC7.1 merge the common
  call tail and produce the target ordering naturally.
- UpdateScreen16SelectionVisuals @ 0x00425390 is now canonical exact outside
  this <=256 routing table. Fresh caller and target review disproved the old
  side-dependent integer-return reconstruction: the first stack argument is
  unused, marker VMs are fixed at 189/190, and the helper is void. The target
  position and color-control-flow source now reproduce all 279 bytes.
- The two horizontal TitleScreen character-cursor helpers are no longer on the
  frontier. The old goto/cached-input source shape was target-disproved.
  Reusing the same-TU exact cursor family -- direct indexed input-state calls
  plus structured 0x40/0x80/0x10/0x20 if/else-if control flow -- naturally
  reproduces both 167-byte targets and all seven relocations each. The three
  remaining cursor helpers are Reverse/Inactive at 183/175 and Active at
  217/211 after bounded natural alias/loop probes.
- The two Bullet pattern wrappers are no longer on the frontier. Rechecking
  TH08 source-family local declarations against fresh TH09 target bytes showed
  that the apparent 16-byte loop-head padding was a source-lifetime effect:
  function-scope indices plus an uninitialized `Bullet *result` make stock VC7.1
  reproduce both 203-byte targets and all four relocations each. Explicit
  `result = NULL` is target-disproved and emits 187 bytes.

- PlayerLifecycleView::UpdateShots @ 0x0041F580 is now canonical exact.
  The old 254/251 plateau came from a y-value spill caused by the maintained
  position-pointer lifetime. A natural float reference alias used only for the
  y integration removes the second stack local while the bounds check continues
  to reload position[1] from memory, yielding 251/251 with all eight relocations.
- `PlayerShotDrawCallbackType1` remains 190/204, but the old scheduling plateau
  is superseded. Source order `++index; ++point; alphaAccumulator += alpha`
  reproduces the first 190 target bytes with 165/166 ordinary comparable bytes;
  the only difference is the zero-trip `jle` destination because VC7.1 merges
  the target's second 14-byte restore/return epilogue. All six relocations solve.
  Bounded natural CFG/profile/alias variants still merge the tail; do not pad or
  force a second epilogue.

- Only `SupervisorFrameQueueView::InsertReceivedFrame` remains non-exact in
  the short frame-queue family. `PopFrame` and `InsertPredictedFrame`
  are exact. The 213-byte target currently resists natural
  queue-base/index/argument-home lifetime spellings; do not revive
  var_order/register forcing.
- `FileSystem::TryDecryptFromTable` remains 215/220. The target's biased
  cursor can read `g_CryptSignature[0]` as terminal index 8 before the later
  `index < 8` rejection. Maintained source intentionally bounds the declared
  eight-row array and avoids that adjacent read. This supersedes the older
  Packet-524 description that said the target checked the bound first.
- `ParseNetworkConfigValue` is target-sized at 243 bytes with its private
  ABI correct; the residual is equivalent return-cursor block placement.
- `PlayerPositionCallback30404Type4` and Type8 remain 235/249 with a 0x3C
  candidate frame versus the target's 0x34 shared hidden-return layout. Natural
  ternary/if/branch-temp and same-TU type probes do not recover that lifetime.
- `PlayerLifecycleView::UpdateShots` remains 254/251. The extra four-byte
  local is an updated-Y spill used for the playfield call; natural
  sprite/extent local rewrites do not remove it.

Former short ECL interpolation helpers are **not** part of the current
frontier: `ApplyInterpolationOperation`, `InstallInterpolationSlot`,
`CallSubroutine`, and `InterpolateHermite` are all canonical exact.
In particular Packet 555 supersedes Packet 527's old 182/188
`InterpolateHermite` plateau.

The current filter is unchanged by later exact promotions above 256 bytes
(Packets 568-577), including Player collision query/regions, CardAttack
draw/update, Player bomb/death state, and TextHelper alpha inversion.

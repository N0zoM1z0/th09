# TH09 short-function frontier

This is a live routing snapshot, not a second exactness ledger. The
authoritative live rows are `config/functions.csv`; `config/matches.csv` and
`config/match-units.toml` alone grant canonical exact credit. Re-filter the
ledger before starting work, and update this snapshot when a short function is
promoted. The original Japanese v1.50a target remains mandatory.

The filter is **confirmed authored + maintained source + not canonical exact +
target logical size at most 256 bytes**. It yields **22 functions**, of which
**6 are at most 128 bytes**. The latest short-function promotion is
`Supervisor::Supervisor @ 0x00431500`; the overall canonical total is 836 exact
functions. Size is only a routing heuristic: this list is not a verified
call-graph leaf set, a difficulty ranking, or product-build progress. All
names and boundaries remain subject to their target-local ledger evidence.

| Target address | Bytes | Module | Non-exact source function |
| --- | ---: | --- | --- |
| 0x0040D4D0 | 32 | EffectManager | `EffectPolarScaleView::SetFromAngleAxes` |
| 0x00441890 | 32 | Math | `Float3::FromAngleMagnitude` |
| 0x00415C60 | 39 | Front | `DecodeFrontMessageString` |
| 0x0040D1C0 | 51 | EffectManager | `EffectManager::AddedCallback` |
| 0x004033A0 | 62 | AnmManager | `AnmVmBase::AnmVmBase` |
| 0x00410250 | 82 | EnemyManager | `EnemyAppendCollisionView::AppendPlayerCollisionBox` |
| 0x004181E0 | 133 | Front | `FrontMessageOwnerView::InitializeMessageRuntime` |
| 0x0042474A | 167 | TitleScreen | `TitleScreenView::MoveCharacterCursorHorizontal` |
| 0x004247F1 | 167 | TitleScreen | `TitleScreenView::MoveCharacterCursorHorizontalForInput` |
| 0x00424FB0 | 175 | TitleScreen | `TitleScreenView::SetCharacterCursorReverse` |
| 0x0042505F | 175 | TitleScreen | `TitleScreenView::SetCharacterCursorInactive` |
| 0x0040F9B0 | 188 | EnemyManager | `EnemyView::IntegrateMotion` |
| 0x004130F0 | 203 | BulletManager | `EtamaController::SpawnBulletPatternPrimary` |
| 0x004131C0 | 203 | BulletManager | `EtamaController::SpawnBulletPatternSecondary` |
| 0x00442220 | 204 | Player | `PlayerShotDrawCallbackType1` |
| 0x00424EDD | 211 | TitleScreen | `TitleScreenView::SetCharacterCursorActive` |
| 0x0042E9E0 | 213 | Supervisor | `SupervisorFrameQueueView::InsertReceivedFrame` |
| 0x0042C290 | 220 | FileSystem | `FileSystem::TryDecryptFromTable` |
| 0x00432240 | 243 | SupervisorNetwork | `ParseNetworkConfigValue` |
| 0x00443B10 | 249 | Player | `PlayerPositionCallback30404Type4` |
| 0x00447620 | 249 | Player | `PlayerPositionCallback30404Type8` |
| 0x0041F580 | 251 | Player | `PlayerLifecycleView::UpdateShots` |

## Selection notes

Use the table above only for routing. Before changing any candidate, read its
live `config/functions.csv` row and recompile from the maintained source. The
notes below summarize only constraints that are still current; superseded probe
history belongs in `docs/KNOWLEDGE_BASE.md`.

- The two 32-byte math/effect leaves remain natural-codegen negatives:
  `EffectPolarScaleView::SetFromAngleAxes` and `Float3::FromAngleMagnitude`
  compile to 30-byte FCOS/FSIN forms while the target uses FSINCOS. Do not add
  inline assembly or fabricate a runtime wrapper merely to fuse them.
- `DecodeFrontMessageString` remains a private-ABI mismatch (36/39 bytes).
  Natural source selects register transport while the target uses one stack
  argument and `ret 4`; do not force a private register/stack ABI.
- `EffectManager::AddedCallback` is already target-sized at 51 bytes. The
  residual is the ESI/EDI assignment of callback argument versus ANM manager;
  bounded local-order, direct-store, `void *`, and profile probes do not
  change it.
- `AnmVmBase::AnmVmBase` remains 61/62 under the closest natural
  `/O1 /Ob1` profile because VC7.1 reuses the element-constructor address
  across the two vector-constructor calls. This is not a license for padding,
  fake wrapper types, or register forcing.
- `Supervisor::Supervisor` is no longer on the frontier. Rechecking the old
  source assumption showed that the target dword OR at +0x5D4 is recovered
  naturally by a volatile 32-bit flags member and source-level compound OR
  after the clear; the replayable 62-byte unit is exact.
- `EnemyAppendCollisionView::AppendPlayerCollisionBox` is target-sized at
  82 bytes with both relocations solved; the residual is temporary-copy/argument
  scheduling. `EnemyView::IntegrateMotion` is target-sized at 188 bytes
  after the mirrored-X source correction, but the remaining EBX/EDI position
  versus previous-position allocation is compiler-local.
- `FrontMessageOwnerView::InitializeMessageRuntime` is target-sized at 133
  bytes with all nine relocations solved. Only the `Setup` call evaluation
  order differs. Do not use volatile, padding, assembly, or source-order
  steering to force the LEA/push order.
- The five remaining TitleScreen cursor helpers are all source-present but
  non-exact. The two horizontal helpers are 167-byte target-sized candidates
  with register/input-pointer lifetime differences. Reverse/Inactive remain
  183/175; Active remains 217/211 after bounded natural alias/loop probes.
- Both Bullet pattern wrappers remain natural 187/203-byte candidates. Target
  loop-head LEA/jump no-op sequences are compiler layout, not source padding to
  reproduce.
- `PlayerShotDrawCallbackType1` remains 190/204. The target retains
  duplicated epilogues that stock VC7.1 tail-merges under the established
  Player-shot profiles; `void` is target-disproved because the target
  explicitly zeros EAX on both exits.
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

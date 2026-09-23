# TH09 short-function frontier

This is a Packet-527 routing snapshot, not a second exactness ledger. The
authoritative live rows are `config/functions.csv`; `config/matches.csv` and
`config/match-units.toml` alone grant canonical exact credit. Re-filter the
ledger before starting work, and update this snapshot when a short function is
promoted. The original Japanese v1.50a target remains mandatory.

The filter is **confirmed authored + maintained source + not canonical exact +
target logical size at most 256 bytes**. It yields **44 functions**, of which
**13 are at most 128 bytes**, after the Packet-517 ResetPool promotion. Size is
only a routing heuristic: this list is not a verified call-graph leaf set, a
difficulty ranking, or product-build progress. All names and boundaries remain
subject to their target-local ledger evidence.

| Target address | Bytes | Module | Non-exact source function |
| --- | ---: | --- | --- |
| 0x00403390 | 12 | ZunTimer | `ZunTimer::ZunTimer` |
| 0x0040D4D0 | 32 | EffectManager | `EffectPolarScaleView::SetFromAngleAxes` |
| 0x00441890 | 32 | Math | `Float3::FromAngleMagnitude` |
| 0x00415C60 | 39 | Front | `DecodeFrontMessageString` |
| 0x00423158 | 42 | Input | `InputView::IsPressedScrolling` |
| 0x0040D1C0 | 51 | EffectManager | `EffectManager::AddedCallback` |
| 0x004033A0 | 62 | AnmManager | `AnmVmBase::AnmVmBase` |
| 0x00431500 | 62 | Supervisor | `Supervisor::Supervisor` |
| 0x00440D90 | 81 | ExAttackController | `ExAttackAllocateRecordView::AllocateDynamicData` |
| 0x00410250 | 82 | EnemyManager | `EnemyAppendCollisionView::AppendPlayerCollisionBox` |
| 0x0041BE70 | 106 | Player | `PlayerLifecycleView::CalcCircleCollision` |
| 0x0041A6EB | 116 | GameManager | `GameManagerPlayfieldView::IsWithinPlayfield` |
| 0x0042EB80 | 119 | Supervisor | `SupervisorFrameQueueView::PopFrame` |
| 0x004181E0 | 133 | Front | `FrontMessageOwnerView::InitializeMessageRuntime` |
| 0x00407A80 | 149 | EclManager | `Th09EclRunControl::CallSubroutine` |
| 0x00407560 | 157 | EclManager | `Th09EclRunControl::ApplyInterpolationOperation` |
| 0x00403A40 | 160 | Background | `Background::Background` |
| 0x004124E0 | 161 | BulletManager | `EtamaController::SelectBulletSprite` |
| 0x0042474A | 167 | TitleScreen | `TitleScreenView::MoveCharacterCursorHorizontal` |
| 0x004247F1 | 167 | TitleScreen | `TitleScreenView::MoveCharacterCursorHorizontalForInput` |
| 0x0041BBE0 | 172 | Player | `LoadPlayerShtFile` |
| 0x00424FB0 | 175 | TitleScreen | `TitleScreenView::SetCharacterCursorReverse` |
| 0x0042505F | 175 | TitleScreen | `TitleScreenView::SetCharacterCursorInactive` |
| 0x00410110 | 184 | EnemyManager | `EnemyView::CleanupAfterDeactivation` |
| 0x004074A0 | 188 | EclManager | `InterpolateHermite` |
| 0x0040F9B0 | 188 | EnemyManager | `EnemyView::IntegrateMotion` |
| 0x0042EAC0 | 191 | Supervisor | `SupervisorFrameQueueView::InsertPredictedFrame` |
| 0x00432770 | 199 | SupervisorNetwork | `SupervisorNetworkState::IsServiceProviderAvailable` |
| 0x004130F0 | 203 | BulletManager | `EtamaController::SpawnBulletPatternPrimary` |
| 0x004131C0 | 203 | BulletManager | `EtamaController::SpawnBulletPatternSecondary` |
| 0x00442220 | 204 | Player | `PlayerShotDrawCallbackType1` |
| 0x00424EDD | 211 | TitleScreen | `TitleScreenView::SetCharacterCursorActive` |
| 0x0042E9E0 | 213 | Supervisor | `SupervisorFrameQueueView::InsertReceivedFrame` |
| 0x00421AA0 | 215 | ScoreData | `ScoreFileView::LoadLastName` |
| 0x0042C290 | 220 | FileSystem | `FileSystem::TryDecryptFromTable` |
| 0x0042CAE0 | 241 | Chain | `ChainReleaseView::ReleaseSingleChain` |
| 0x004361C0 | 242 | TextHelper | `TextHelper::CopyTextToSurface` |
| 0x00432240 | 243 | SupervisorNetwork | `ParseNetworkConfigValue` |
| 0x00443930 | 249 | Player | `PlayerShotUpdateCallbackType4` |
| 0x00443B10 | 249 | Player | `PlayerPositionCallback30404Type4` |
| 0x00447620 | 249 | Player | `PlayerPositionCallback30404Type8` |
| 0x0041F580 | 251 | Player | `PlayerLifecycleView::UpdateShots` |
| 0x00435B00 | 253 | AsciiManager | `AsciiManager::OnUpdate` |
| 0x00407600 | 255 | EclManager | `Th09EclRunControl::InstallInterpolationSlot` |

## Selection notes

- The four short `BulletManager.cpp` transform-update siblings are now exact;
  see Packet 514 in `docs/KNOWLEDGE_BASE.md`. Other Bullet functions on this
  list (sprite selection and spawn-pattern wrappers) have independent target
  extents and are not promoted by association. The 295-byte boundary-bounce
  transform is also exact as of Packet 518 but lies outside this size-filtered
  table.
- The TitleScreen horizontal cursor helpers (167/167 bytes) now have
  target-sized, source-present candidates after Packet 516's input-owner and
  directional-branch corrections; neither is exact. The other character-
  cursor helpers (175/175/211 bytes), the
  Supervisor frame-queue helpers (119/191/213), and the ECL interpolation
  helpers (149/157/188/255) are other bounded families. They are not one
  translation-unit/profile claim merely because they appear in this table.
- Packet 522's natural address-of-`vms`-slot alias probe for
  `SetCharacterCursorActive` compiles back to the original 217-byte candidate;
  it remains non-exact alongside the other character-cursor variants.
- Packet 523's conventional entry-guard plus do/while rewrite leaves both
  Bullet pattern wrappers at 187/203 bytes; target loop-head no-op sequences
  remain compiler-layout evidence, not source padding to reproduce.
- Packet 524 corrects `FileSystem::TryDecryptFromTable`'s scan guard: target
  checks the eight-record bound before indexing, while the prior source
  evaluated the key expression first. Corrected natural source compiles to
  215/220 bytes and remains non-exact; do not restore the unsafe order to chase
  the previous candidate size.
- Packet 525 tried function-scope and tail-only `vms`-slot aliases plus
  precomputed-end `for` and guarded `do/while` loops for
  `SetCharacterCursorReverse`; all stayed at 183/175 bytes. Keep the
  baseline source and skip these shapes absent new target evidence.
- Packet 526 audits `EffectManager::AddedCallback`: the canonical `/O2` object
  is target-sized at 51 bytes, but natural local-order and direct-store
  variants retain the same callback/global-manager register inversion. Keep
  the baseline and do not steer preserved-register assignment.
- Packet 527 retests `InterpolateHermite`: reversing declaration order while
  preserving target call order leaves the 182-byte candidate byte-identical.
  Together with Packet 515's summand retry this remains a local/FPU codegen
  residual without a semantic discrepancy.
- Small size or a one-byte residual is not a quick-win guarantee. Both 32-byte
  trigonometric helpers have a durable `FSINCOS` versus `FCOS`/`FSIN` compiler
  plateau. `AsciiManager::OnUpdate` is exact-sized at 253 bytes but still has
  one `JB`/`JL` byte after bounded signedness probes. `ZunTimer::ZunTimer`,
  `InputView::IsPressedScrolling`, and several exact-sized candidates have
  documented register, return or private-ABI residuals. Consult their ledger
  rows and `docs/KNOWLEDGE_BASE.md` before repeating a rejected probe.

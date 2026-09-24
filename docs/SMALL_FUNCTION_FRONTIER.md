# TH09 short-function frontier

This is a Packet-531 routing snapshot, not a second exactness ledger. The
authoritative live rows are `config/functions.csv`; `config/matches.csv` and
`config/match-units.toml` alone grant canonical exact credit. Re-filter the
ledger before starting work, and update this snapshot when a short function is
promoted. The original Japanese v1.50a target remains mandatory.

The filter is **confirmed authored + maintained source + not canonical exact +
target logical size at most 256 bytes**. It yields **40 functions**, of which
**13 are at most 128 bytes**, after Packet 531's Background constructor promotion.
Size is only a routing heuristic: this list is not a verified call-graph leaf set, a
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
  Supervisor frame-queue helpers (119/191/213), and the remaining non-exact ECL
  helpers (149/188/255) are other bounded families. They are not one
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
  the previous candidate size. Packet 543 clarifies that the target's row-1-
  biased byte cursor can compare `g_CryptSignature[0]` at terminal index 8,
  then rejects index 8 before decrypting; maintained source avoids this
  adjacent read and preserves the eight valid key cases.
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
- Packet 528 closes `Th09EclRunControl::ApplyInterpolationOperation` at
  157/157 bytes. Its sole RunEcl caller passes Enemy in EDI and instruction in
  ESI; moving the unchanged semantic body into that TU as an internal `static`
  helper naturally reproduces the private transport and all five relocations
  in two cold canonical replays. This is not a public ABI claim. The short
  non-exact frontier briefly fell to 43 functions; 13 remain at most 128 bytes.
- Packet 529 closes `Th09EclRunControl::InstallInterpolationSlot` at
  255/255 bytes after a natural same-TU static move, with all nine relocations
  solved and two cold canonical replays. Its callback-table data definition
  remains unresolved, so this function match is not product-build closure.
  The current non-exact frontier is 42 functions; 13 remain at most 128 bytes.
- Packet 530 closes `Th09EclRunControl::CallSubroutine @ 0x00407A80` at
  149/149 bytes in two cold canonical replays, with its `InitializeSubroutine`
  relocation solved to `0x00406850`. Its two target call sites are the ordinary
  CALL and CALL_REMOTE paths in RunEcl. Natural same-TU static source preserves
  the context-pointer rereads. The frontier is now 41 functions; 13 remain at
  most 128 bytes.
- Packet 531 closes `Background::Background @ 0x00403A40` at 160/160 bytes in
  two cold `/O2 /Ob0` replays, with all thirteen relocation destinations
  aligned. Its constructor callback at `0x004343D0` remains a shared/folded
  owner unknown. The frontier is now 40 functions; 13 remain at most 128 bytes.
- Packet 546 refines `SupervisorFrameQueueView::InsertReceivedFrame` to return
  the target-observed object-relative cursor in EAX; maintained call sites
  ignore it. The natural source now models that return but remains 211 bytes
  versus 213, with the shared prefix diverging from `+0x1C`; do not mistake the
  size delta for a near-match or steer registers/stack homes to chase it.
- Packet 547 aligns `ChainReleaseView::ReleaseSingleChain` with the target's
  separately observed allocation, conditional construction, and registry
  calls. Its cold-stable `/EHsc /O2 /Ob1` body is 251/241 bytes and remains
  non-exact; this semantic correction does not change the 40-function frontier.
- Packet 548 improves `Supervisor::Supervisor` to a cold-stable target-sized
  62-byte candidate with a naturally equivalent post-clear flags assignment;
  all relocations resolve and 41/50 ordinary bytes match, but nine tail bytes
  still differ, so it remains non-exact.
- Small size or a one-byte residual is not a quick-win guarantee. Both 32-byte
  trigonometric helpers have a durable `FSINCOS` versus `FCOS`/`FSIN` compiler
  plateau. `AsciiManager::OnUpdate` is exact-sized at 253 bytes but still has
  one `JB`/`JL` byte after bounded signedness probes. `ZunTimer::ZunTimer`,
  `InputView::IsPressedScrolling`, and several exact-sized candidates have
  documented register, return or private-ABI residuals. Consult their ledger
  rows and `docs/KNOWLEDGE_BASE.md` before repeating a rejected probe.

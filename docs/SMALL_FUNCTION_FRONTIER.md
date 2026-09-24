# TH09 short-function frontier

This is a Packet-563 routing snapshot, not a second exactness ledger. The
authoritative live rows are `config/functions.csv`; `config/matches.csv` and
`config/match-units.toml` alone grant canonical exact credit. Re-filter the
ledger before starting work, and update this snapshot when a short function is
promoted. The original Japanese v1.50a target remains mandatory.

The filter is **confirmed authored + maintained source + not canonical exact +
target logical size at most 256 bytes**. It yields **27 functions**, of which
**7 are at most 128 bytes**, after Packet 563's Enemy cleanup promotion.
Size is only a routing heuristic: this list is not a verified call-graph leaf set, a
difficulty ranking, or product-build progress. All names and boundaries remain
subject to their target-local ledger evidence.

| Target address | Bytes | Module | Non-exact source function |
| --- | ---: | --- | --- |
| 0x0040D4D0 | 32 | EffectManager | `EffectPolarScaleView::SetFromAngleAxes` |
| 0x00441890 | 32 | Math | `Float3::FromAngleMagnitude` |
| 0x00415C60 | 39 | Front | `DecodeFrontMessageString` |
| 0x0040D1C0 | 51 | EffectManager | `EffectManager::AddedCallback` |
| 0x004033A0 | 62 | AnmManager | `AnmVmBase::AnmVmBase` |
| 0x00431500 | 62 | Supervisor | `Supervisor::Supervisor` |
| 0x00410250 | 82 | EnemyManager | `EnemyAppendCollisionView::AppendPlayerCollisionBox` |
| 0x004181E0 | 133 | Front | `FrontMessageOwnerView::InitializeMessageRuntime` |
| 0x0042474A | 167 | TitleScreen | `TitleScreenView::MoveCharacterCursorHorizontal` |
| 0x004247F1 | 167 | TitleScreen | `TitleScreenView::MoveCharacterCursorHorizontalForInput` |
| 0x0041BBE0 | 172 | Player | `LoadPlayerShtFile` |
| 0x00424FB0 | 175 | TitleScreen | `TitleScreenView::SetCharacterCursorReverse` |
| 0x0042505F | 175 | TitleScreen | `TitleScreenView::SetCharacterCursorInactive` |
| 0x0040F9B0 | 188 | EnemyManager | `EnemyView::IntegrateMotion` |
| 0x00432770 | 199 | SupervisorNetwork | `SupervisorNetworkState::IsServiceProviderAvailable` |
| 0x004130F0 | 203 | BulletManager | `EtamaController::SpawnBulletPatternPrimary` |
| 0x004131C0 | 203 | BulletManager | `EtamaController::SpawnBulletPatternSecondary` |
| 0x00442220 | 204 | Player | `PlayerShotDrawCallbackType1` |
| 0x00424EDD | 211 | TitleScreen | `TitleScreenView::SetCharacterCursorActive` |
| 0x0042E9E0 | 213 | Supervisor | `SupervisorFrameQueueView::InsertReceivedFrame` |
| 0x0042C290 | 220 | FileSystem | `FileSystem::TryDecryptFromTable` |
| 0x0042CAE0 | 241 | Chain | `ChainReleaseView::ReleaseSingleChain` |
| 0x00432240 | 243 | SupervisorNetwork | `ParseNetworkConfigValue` |
| 0x00443930 | 249 | Player | `PlayerShotUpdateCallbackType4` |
| 0x00443B10 | 249 | Player | `PlayerPositionCallback30404Type4` |
| 0x00447620 | 249 | Player | `PlayerPositionCallback30404Type8` |
| 0x0041F580 | 251 | Player | `PlayerLifecycleView::UpdateShots` |

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
- Packet 551 closes `ZunTimer::ZunTimer @ 0x00403390` at 12/12 bytes.
  The prior 8-byte candidate was an artifact of placing the constructor and
  `Initialize` definition in the same translation unit: VC7.1 could see that
  callee body and kept `this` in ECX. Moving only the ordinary constructor
  definition to its own TU makes the compiler preserve `this` in ESI across
  the call, exactly matching the target and resolving the sole REL32 to
  `ZunTimer::Initialize @ 0x004014A0`. The frontier is now 39 functions; 12
  remain at most 128 bytes.
- Packet 552 closes `InputView::IsPressedScrolling @ 0x00423158` at
  42/42 bytes. Target callers test AX after the call, supporting the retained
  16-bit return ABI, while the callee reads the mask as a full 32-bit stack
  argument. Reusing one `u16` temporary across the short-circuit OR makes
  VC7.1 preserve both target zero-extend/mask/test stages and emit the shared
  false/true EAX result blocks. The frontier is now 38 functions; 11 remain at
  most 128 bytes.
- Packet 553 closes `AsciiManager::OnUpdate @ 0x00435B00` at 253/253
  bytes with all fourteen relocations solved. The old signed-side-index probe
  addressed the wrong expression: target EBX already contains the sideFlags
  absolute address, and the sole target/candidate difference is signed `JL`
  versus pointer-order `JB`. Comparing the two 32-bit addresses as signed
  integers changes only that opcode and produces the exact target. The frontier
  is now 37 functions; 11 remain at most 128 bytes.
- Packet 554 closes `TextHelper::CopyTextToSurface @ 0x004361C0` at
  242/242 bytes without changing its function body. The old 224-byte candidate
  lived in the same TU as trivial getter definitions, allowing VC7.1 to keep
  `srcBuf` live in EDX across getter calls and shrink the frame to 0x48.
  Moving the unchanged function to its own TU restores ordinary call-clobber
  assumptions, the target 0x4C frame, all pointer spill/reload instructions,
  and all eight relocation targets. The <=256-byte frontier is now 36.
- Packet 555 closes `InterpolateHermite @ 0x004074A0` at 188/188
  bytes. The old formula was mathematically correct, but VC7.1 reassociated the
  unparenthesized expression into a 182-byte x87 schedule with different local
  slots. Explicitly grouping each Hermite basis before multiplying its parameter
  and explicitly left-associating the four weighted terms restores the target
  -0x10/-0x14/-0x18/-0x1C parameter slots, -0x08/-0x04/-0x0C derived locals,
  and target term0->term1->term2->term3 x87 order. The frontier is now 35.
- Packet 556 closes `ScoreFileView::LoadLastName @ 0x00421AA0` at
  215/215 bytes. The previous 212-byte source order caused VC7.1 to use EAX
  alone for the nine-byte default-name copy. Keeping the same final record
  values but assigning `chapterSize` before `chapterSizeCopy`, then
  magic/version/runtimeMarker, and placing the independent `strcpy` last
  reproduces the target EBX/ESI copy schedule and all 23 DIR32 relocations.
  The <=256-byte frontier is now 34 functions.
- Packet 557 closes `PlayerLifecycleView::CalcCircleCollision @ 0x0041BE70`
  at 106/106 bytes. The old direct `squaredDistance < squaredRadius`
  spelling matched the arithmetic but not the target x87 unordered behavior.
  TH09 uses `TEST AH,41h / JP` after `FCOMPP`; expressing the control
  flow as `if (squaredDistance >= squaredRadius) return 0; return 1;`
  naturally reproduces the target status-word branch and epilogues. The
  <=256-byte frontier is now 33 functions; 10 remain at most 128 bytes.
- Packet 558 closes `GameManagerPlayfieldView::IsWithinPlayfield @ 0x0041A6EB`
  at 116/116 bytes. TH09 uses mixed comparison orientation: the right/bottom
  tests are `144.0f < x - width/2` and `448.0f < y - height/2`,
  producing the target constant-load/`FCOMPP` forms. Under the neighboring
  GameManager-proven `/O2 /Os /Ob1` profile this also recovers the target
  shared x87 cleanup and 0/1 materialization. The <=256-byte frontier is now
  32 functions; 9 remain at most 128 bytes.
- Packet 559 closes `SupervisorFrameQueueView::PopFrame @ 0x0042EB80`
  at 119/119 bytes. The old explicit `sideBase` local made VC7.1 keep
  the adjusted queue base in EDX. Repeating the same receiver-relative address
  expression at each access lets the optimizer CSE `this + side * 0x78`
  directly into ECX, matching the target's side-base/predicted/count register
  lifetimes without changing semantics. The <=256-byte frontier is now 31
  functions; 8 remain at most 128 bytes.
- Packet 560 closes `ExAttackAllocateRecordView::AllocateDynamicData @ 0x00440D90`
  at 81/81 bytes. Repeating `vmCount * 0x2A4` at the allocation-size
  and tail-pointer uses gives VC7.1 one long-lived common subexpression, so it
  keeps the original count in EBX from entry and scales EBX in place exactly as
  the target does. The previous destructive parameter update inserted a
  two-byte `mov ebx,eax`. The <=256-byte frontier is now 30 functions; 7
  remain at most 128 bytes.
- Packet 561 closes `EtamaController::SelectBulletSprite @ 0x004124E0`
  at 161/161 bytes by correcting the source API contract to `void`. All
  three target callers discard EAX, and TH08 independently uses a void selector.
  Branch-local `SetSprite(...); return;` arms make VC7.1 reload
  `bulletAnm` from +0x25E1BC in each arm exactly as the target does. The
  <=256-byte frontier is now 29 functions; the <=128 subset remains 7.
- Packet 562 closes `SupervisorFrameQueueView::InsertPredictedFrame @ 0x0042EAC0`
  at 191/191 bytes. The exact shape keeps `targetFrame` live across the
  shift instead of recomputing it, advances a frame-field cursor from the side
  base, increments index before cursor at the scan tail, and forms one
  object-relative entry cursor for the final stores. VC7.1 then naturally keeps
  side in EBX, spills side-base and targetFrame exactly like the target, and
  reproduces the loop alignment. The <=256-byte frontier is now 28 functions;
  7 remain at most 128 bytes.
- Packet 563 closes `EnemyView::CleanupAfterDeactivation @ 0x00410110`
  at 184/184 bytes. The previous direct timer-field store made VC7.1 prepare
  the child-ECL fastcall receiver before all five -1 stores. Taking a normal
  pointer alias to `timerCallbackThreshold33D0` after the four life-threshold
  stores makes the compiler place `mov ecx,esi` exactly between those groups,
  matching the target without register or scheduler directives. The <=256-byte
  frontier is now 27 functions; the <=128 subset remains 7.
- Small size or a one-byte residual is not a quick-win guarantee. Both 32-byte
  trigonometric helpers have a durable `FSINCOS` versus `FCOS`/`FSIN` compiler
  plateau. several exact-sized candidates have
  documented register, return or private-ABI residuals. Consult their ledger
  rows and `docs/KNOWLEDGE_BASE.md` before repeating a rejected probe.

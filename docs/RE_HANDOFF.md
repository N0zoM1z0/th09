# TH09 reconstruction — next-agent handoff

**Start here.** Read [AGENTS.md](../AGENTS.md), the
[bounded workflow](RE_WORKFLOW.md) and [Oracle rules](ORACLES.md).
The [knowledge base](KNOWLEDGE_BASE.md) records target layouts and useful
failed approaches; old trial results are history, not current codegen proof.
Git retains earlier source revisions.

## Verified baseline and boundaries

- **Target:** original Japanese TH09 **v1.50a**, Windows i386, base
  `0x00400000`, ignored `resources/th09.exe`, SHA-256
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  Never patch or delete it.
- **Last verified 2026-11-11:** **1,025 / 1,037** source-present functions
  byte-exact; **12 nonexact / 28,145 original bytes**. Another 35 candidate
  origins remain reviewed but unresolved; inventory completeness is unproven.
  These are *snapshots*, not a new agent's live proof: regenerate totals with
  `python3 -B scripts/report-reconstruction-status.py`. Canonical authority
  is `config/matches.csv`, `config/functions.csv` and
  `config/match-units.toml`, not manually copied status prose.
- **Closed large owners:** `GameplaySetupThread @ 0x0041AF2D` is exact
  (1,689 bytes / 172 relocations). `Title DrawResult` is also exact
  (1,939 bytes); do **not** confuse it with nonexact `OnUpdateResult`.
- **Phase:** source exactness remains open. No complete Windows i386 link
  and runtime closure, dual-Oracle semantic baseline or port is proven.
- **2026-11-12 Pause/side-gameover menu checkpoint:** state
  and frame counters across PauseMenu, AsciiMenuState4 and
  AsciiMenuState5 use native i386 DWORD INC, and the live
  difficulty sprite script ID wraps at 32 bits. The original
  GameManager retry counter is a BYTE and remains an 8-bit INC.
  Two pinned VC7.1 carriers protect 11/11 exact owners,
  5,442 full PE bytes, 350 relocation fields and 23
  independently proven private compiler-label rebindings.
- **2026-11-12 Supervisor draw checkpoint:** the native
  loading-state frame counter, ANM manager's 32 surface slots,
  and the three real 0x2A4-byte loading VMs now use i386 DWORD
  arithmetic without extra bounds/call changes. Three focused
  VC7.1 carriers preserve 5/5 exact owners, 445 bytes,
  26 original-bound relocations, including DrawMain and
  its three small leaf helpers.
- **2026-11-12 Etama sprite checkpoint:** the
  signed ANM VM sprite base and four size/script-selected
  offsets now combine as native modulo-32 i386 DWORD
  results. Three affected VC7.1 carriers preserve 18/18
  exact owners, 6,711 physical bytes and 259 relocation
  fields including large Etama update/spawn functions.
  Sixteen private label rebindings are original PE/COFF-proven.
- **2026-11-12 Player pattern selector checkpoint:** the live
  signed 0x1C-byte pattern-config opening interval now
  multiplies by 60 and subtracts the timer as an original
  i386 DWORD before publishing the signed display value.
  Two VC7.1 carriers retain 2/2 exact owners, 3,650 PE
  bytes and 154 relocations, including the 3,238-byte
  PlayerUpdateSelectorState boss. Two compiler-private
  labels have independent original PE/COFF proofs.
- **2026-11-12 Supervisor viewport checkpoint:** dynamic
  signed view selectors now address the three true 0xF0-byte
  Supervisor camera/viewport records by modulo-32 native
  i386 DWORD strides. The complete local SupervisorViewport
  VC7.1 carrier preserves 8/8 exact owners, 1,530 original
  bytes and 106 relocation fields.
- **2026-11-12 Supervisor screenshot checkpoint:** signed
  Direct3D lock pitch, first/next source rows, destination BMP
  row and inner RGB24 pixel cursor strides now form native
  i386 DWORD byte addresses in the original screenshot owner.
  One pinned VC7.1 carrier protects 1/1 exact 543-byte
  TakeSnapshot function with all 23 relocation fields.
- **2026-11-12 Supervisor frame queue checkpoint:** the
  original Supervisor +0x47C twenty-record network queue and
  +0x56C last-received frame slots, separate Replay input
  states, insertion/scan cursor and two shift loops now use
  authentic i386 DWORD byte-address strides. Two affected
  VC7.1 carriers protect 2/2 exact owners, 310 physical
  bytes and one bound PE relocation field.
- **2026-11-12 PBG packed table checkpoint:** the variable-length
  filename and four-byte integer SeekPast cursor leaves now
  advance the genuine LZSS-decoded file buffer through native
  DWORD addresses. The signed entry count + sentinel allocation
  size also wraps as an unsigned i386 word. The one pinned
  PbgArchiveHelpers VC7.1 carrier preserves 6/6 exact owners,
  441 bytes and 18 original-bound relocation fields.
- **2026-11-12 Supervisor version parser checkpoint:** the
  opened .ver file's six-byte version tag, newline cursor and
  remaining-length difference now use native 32-bit i386
  byte-address words. Two focused VC7.1 carriers retain
  2/2 exact owners, 344 physical bytes and 22 relocations.
- **2026-11-12 PBG archive checkpoint:** three real encrypted
  header DWORD values now subtract obfuscating offsets modulo
  2^32 before existing validity checks, and the 0x10-byte
  next-entry sentinel determines compressed stream extent
  through a native i386 record address. Two scoped VC7.1
  carriers preserve 3/3 exact owners, 839 PE bytes and
  38 original-bound relocations.
- **2026-11-12 FrontMessage loader checkpoint:** primary
  file-table offsets and variable-width script command strides
  now resolve true packed wire-data addresses as native
  unsigned i386 DWORD words. The 405-byte LoadForSideCount
  owner remains exact, all 18 original-bound relocation fields
  verified by a single local VC7.1 carrier.
- **2026-11-12 Network config parser checkpoint:** the
  original IP.txt key, whitespace, comment and value cursors now
  advance through native i386 DWORD byte addresses, with the
  existing strlen(key) advance and original parser branches.
  The one pinned VC7.1 carrier preserves 2/2 exact owners,
  922 complete PE bytes and 23 bound relocation fields.
- **2026-11-12 RunEcl interpolation cursor checkpoint:** both
  runtime-counted interpolation loops now advance the genuine
  0x30-byte context records through i386 DWORD byte addresses.
  Full 15,564-byte RunEcl remains NONEXACT at precisely 30
  ordinary opcode-4/155 bytes, all 598 fields and switch tables
  unchanged. Two VC7.1 carriers protect 83/83 exact siblings,
  29,856 physical bytes, 1,389 relocations; 8 private-label
  rebindings independently proven against original PE/COFF.
- **2026-11-12 Supervisor music checkpoint:** eight live
  0x10-byte track record fields and four signed-ID music
  unlock bytes now use genuine original-table native DWORD
  addresses. The one SupervisorAudio VC7.1 carrier preserves
  8/8 exact owners, 1,054 physical bytes and all 82 fields,
  including PlayMusic, LoadMusic and ReleaseMusic.
- **2026-11-12 Supervisor core update checkpoint:** the
  signed persistent version-check frame counter and screen
  effect countdown now advance/decrement as native i386 DWORDs.
  One pinned VC7.1 carrier preserves the entire 996-byte
  Supervisor::OnUpdate owner, all 92 relocations and 14
  independently original-PE/COFF-proven local jump labels.
- **2026-11-12 ANM variable-mask checkpoint:** the genuine
  GetFloatVarPtr and GetIntVarPtr mask tests now shift an
  unsigned DWORD instead of potentially shifting a signed
  one into its sign bit. The two affected VC7.1 carriers
  preserve 5/5 exact functions, 10,496 original bytes,
  421 fields including the 9,776-byte ANM script boss.
- **2026-11-12 Replay decoder checkpoint:** the actual
  0x1EC-byte ReplayDataView header's 3x10 frame offset/pointer
  row and ten FPS slots now use native i386 DWORD address
  computations in LoadReplayData. The two ReplayInput VC7.1
  carriers retain 12/12 exact owners, 4,093 bytes, 232 fields;
  the separate SaveReplay and BeginPlaybackStage nonexact
  extent/field guards also remain unchanged.
- **2026-11-12 Effect pool/VM checkpoint:** the runtime-counted
  0xD8-byte Effect pool and 0x2A4-byte ANM VM allocations now
  use native i386 DWORD record traversal in the initialize,
  update, spawn, draw and release paths. The 8 affected VC7.1
  carriers preserve 15/15 exact owners, 2,530 physical bytes
  and 100 fields. AddedCallback remains 51/51 with 4 fields
  and its unchanged 8 nonexact register bytes.
- **2026-11-12 Replay Save Draw index checkpoint:** selected
  replay row+1 and the signed name-edit cursor's underline offset
  now form original unsigned 32-bit i386 arithmetic before use.
  Focused VC7.1 replay preserves the complete 800-byte exact
  DrawReplaySave owner and all 41 relocations.
- **2026-11-12 Replay SaveReplay checkpoint:** the signed stream
  selector now chooses from the actual live ReplayBufferLink
  three-input pointer row using native i386 DWORD byte addresses.
  The 1,238-byte nonexact boss retains 53 fields, 23 calls and
  only its two preexisting EDI/ECX byte differences. Two
  replay-scoped VC7.1 carriers protect 12/12 exact sibling
  owners, 4,093 complete bytes and 232 relocations.
- **2026-11-12 MIDI ProcessMsg cursor checkpoint:** the
  1,574-byte original-bound switch owner now expresses eight
  file-backed event/SysEx/meta cursor steps as modulo-32
  DWORD addresses. One VC7.1 carrier retains 32/32 exact
  canonical owners, 4,270 bytes and 132 fields; independent
  full-object proof covers 33/33 physical owners (4,271 bytes)
  and 23 compiler-private labels verified against original PE.
- **2026-11-12 GameWindow frame loop checkpoint:** the original
  signed BYTE frame-skip counter and signed DWORD transition
  countdown now advance/decrement using their real i386 storage
  widths. Two focused VC7.1 carrier replays preserve 2/2 exact
  owners (GameWindow Render 326 bytes / 30 fields, Present
  232 bytes / 22 fields).
- **2026-11-12 Background keyframe checkpoint:** opcode zero
  of the 2,652-byte stage-script interpreter advances the packed
  0x14-byte file-backed instruction pointer via native DWORD
  arithmetic, retaining the original next-frame read. One focused
  VC7.1 carrier preserves 5/5 exact owners, 3,231 bytes and
  160 fields, with 48 independent COFF/PE private-label proofs.
- **2026-11-12 Replay Save Draw checkpoint:** the real 50-row
  native replay header allocation and 6x16 keyboard character
  pointer now use source-level DWORD byte addresses in the
  800-byte complete DrawReplaySave owner. One pinned VC7.1 carrier
  preserves 800/800 physical bytes and all 41 relocations.
- **2026-11-12 MIDI parser checkpoint:** the real file-backed SMF
  header, chunk and variable-length input cursors now use native
  32-bit byte addresses, including authentic +2/+4 field reads.
  The one MIDI VC7.1 carrier protects 32/32 exact units,
  4,270 bytes and 132 original-bound relocations; independent
  same-object proof covers 33/33 physical owners including the
  one-byte virtual timer thunk. All 23 changed private labels
  are PE/COFF-proven.
- **2026-11-12 Player movement checkpoint:** the genuine
  Player +0x1B88/+0x1B8C position x/y members now replace
  cross-member float-pointer indexing while retaining the original
  exact operator float*() call. The complete nonexact 1,900-byte
  owner stays at 66 fields, 23 calls, and 122 ordinary differences;
  no exactness is claimed.
- **2026-11-12 SoundPlayer command checkpoint:** five BGM
  command-step increments, one retry decrement and the SFX pan
  mean's accumulator now use explicit native i386 DWORD arithmetic.
  Eight VC7.1 carriers preserve 20/20 exact owners, 5,400 bytes
  and 184 relocation fields with nine PE/COFF-proven local labels.
- **2026-11-12 ANM script checkpoint:** JmpDec's live
  signed script operand and scriptsExecutedThisFrame10 now use
  native modulo-32 DWORD arithmetic. Both VC7.1 carriers preserve
  5/5 exact functions, 10,496 bytes and 421 relocation fields,
  including the full 9,776-byte interpreter. All 100 private
  label renames were independently PE/COFF-proven.
- **2026-11-12 FrontMessage portrait-wire checkpoint:** nine
  signed script/sprite wire-ID sums now form native modulo-32
  DWORD results, preserving all live ANM receivers. The exact
  2,968-byte script boss and 39-byte decode leaf replay 2/2,
  3,007 bytes/167 original-bound fields, with 27 PE/COFF-proven
  compiler-private label name rebindings.
- **2026-11-12 D3D formatter checkpoint:** 11 live formatted
  capability-buffer pointer advances now preserve original i386
  DWORD byte-address arithmetic. Three affected VC7.1 carriers
  retain 3/3 exact owners, 2,444 physical bytes and 245 fields,
  including the 1,561-byte D3D startup boss and two formatter leaves.
- **2026-11-12 FrontSide draw checkpoint:** the complete
  3,094-byte OnDraw boss now recovers each genuine ANM VM
  owner from its +0x208 position cursor via wrapping native
  DWORD subtraction. One pinned VC7.1 carrier preserves all
  209 original bound fields and complete bytes.
- **2026-11-12 FrontCalc transition checkpoint:** panel timers,
  front frame, dynamic sprite ID, opposing side and native
  stage/result-table expressions now wrap as real i386 DWORDs.
  The complete 1,491-byte FrontCalcCallback passes one scoped VC7.1
  carrier with all 107 bound original relocations and owner bytes.
- **2026-11-12 MIDI word checkpoint:** long-message ring cursor,
  tempo DWORD, signed next-event tick and fade-time word now use
  native i386 wrap. A scoped MIDI VC7.1 replay preserves 32/32 exact
  owners, 4,270 bytes and 132 original-bound fields; independent
  same-carrier proof includes the extra one-byte timer virtual owner
  (33/33, 4,271 bytes). 23 compiler-private labels PE/COFF-proven.
- **2026-11-12 Title Options checkpoint:** three signed menu
  state/frame counters and four live selected-VM script indices now
  use native DWORD arithmetic. OnUpdateOptions (2,045 bytes) and
  PlayMenuSound leaf (34 bytes) retain 2/2 exact owners, 2,079
  bytes and 138 fields with a focused VC7.1 replay.
- **2026-11-12 MusicRoom checkpoint:** the 2,258-byte script/BGM
  boss now uses native DWORD music pause/state counter semantics.
  Its 209-byte draw leaf also remains exact: 2/2 owners, 2,467
  physical bytes and 80 fields from one scoped VC7.1 carrier.
  Cursor/track-pointer variants shifting four SIB bytes were rejected.
- **2026-11-12 Title KeyConfig checkpoint:** three signed
  frame/phase counters now use native i386 DWORD increments. The
  complete 2,803-byte key-config boss and its 106-byte swap helper
  replay 2/2 exact (2,909 bytes, 118 fields) in one VC7.1 carrier;
  side-index rewrites that changed SIB bytes were not installed.
- **2026-11-12 Background renderer checkpoint:** variable-width
  stage quads advance by signed-16-to-DWORD native byte strides and
  the signed special-effect point count wraps as an i386 DWORD.
  The 3,274-byte RenderObjects owner passes a fresh one-carrier
  exact replay of all 113 original relocations and physical bytes.
- **2026-11-12 FrontMessage script-index checkpoint:** four live
  portrait/ANM script index sums use native signed-result DWORD wrap.
  The 2,968-byte script boss and 39-byte decoder remain exact:
  2/2 owners, 3,007 bytes and 167 fields in two VC7.1 carriers.
  24 compiler-private jump labels were independently COFF/PE-proven.
- **2026-11-12 FrontSide HUD checkpoint:** live signed HUD
  side/rank reads and the separate +0x108 marker-count lookup now
  address real GameManager storage with original DWORD byte strides.
  The 2,381-byte OnUpdate boss and fourteen leaves replay 15/15
  exact, 4,087 physical bytes and 176 relocations.
- **2026-11-12 FrontMessage script checkpoint:** variable-length
  command and text payload addresses now advance as native 32-bit
  words; the live signed frame-wait count wraps as a DWORD.
  The 2,968-byte switch owner and 39-byte decode leaf replay 2/2
  exact, 3,007 bytes/167 fields in two VC7.1 carriers, with 22
  independently PE/COFF-proven private label rebindings.
- **2026-11-12 GameManager cleanup checkpoint:** signed stage,
  frame threshold, reward and limit arithmetic now reproduces the
  actual i386 DWORD wrap in CleanupGameplayState, retaining signed
  final comparisons. Both affected VC7.1 carriers preserve 12/12
  exact owners, 4,680 physical bytes and 415 relocations, including
  the update callback and 1,689-byte GameplaySetupThread.
- **2026-11-12 SoundPlayer SFX producers checkpoint:** signed sound
  IDs and the live post-incremented pan counts now access their real
  8-byte metadata, four-byte state and 12x128 pan allocations through
  native DWORD addresses. Two SFX leaves and their big ProcessQueues
  sibling pass all eight involved VC7.1 carriers: 20/20 exact
  owners, 5,400 bytes, 184 relocations. One PE/COFF-proven label.
- **2026-11-12 Etama update checkpoint:** the bullet transform
  repetition/bounce counters, offscreen-cull delay, spawn-suppression
  and frame counter now wrap as genuine i386 DWORDs. Three VC7.1
  carriers protect 18/18 exact owners, 6,711 bytes and 259 fields,
  with 16 independent COFF/PE private-label proofs.
- **2026-11-12 SoundPlayer BGM checkpoint:** signed BGM format,
  preload pointer, size and filename selectors now use genuine native
  0x34/4/0x100-byte DWORD-scaled addresses. All eight involved VC7.1
  carriers retain 20/20 exact owners, 5,400 bytes and 184 fields;
  11 compiler-private labels have independent original-PE/COFF proof.
- **2026-11-12 SoundPlayer WAV checkpoint:** eight file-backed
  RIFF/WAVE and chunk size/cursor operations now reproduce native
  unsigned i386 address and word arithmetic. Eight affected VC7.1
  carriers protect 20/20 exact owners, 5,400 bytes and 184 fields;
  11 private labels were original-PE/COFF-proven.
- **2026-11-12 Title menu/update checkpoint:** 32 live signed
  screen/phase/frame counter increments now use native DWORD wrap.
  Two VC7.1 carriers retain 41/41 exact functions, 18,972 bytes and
  953 fields; nine compiler-label bindings proven against COFF/PE.
  The separate nonexact OnUpdateResult stays at 484/485 bytes,
  34 fields, 218 unlike overlaps and unchanged call/CFG structure.
- **2026-11-12 Ending interpreter checkpoint:** the file-backed
  script parser's 19 single-byte cursor increments and one two-byte
  advance now use native DWORD address arithmetic. Both affected
  VC7.1 carriers protect 10/10 exact owners, 3,015 bytes and 159
  relocation fields, with 31 original-PE/COFF-proven private labels.
- **2026-11-12 ANM loader checkpoint:** 20 live 32-slot surface
  cache accesses plus the 0x120-byte file-slot cursor now compute
  actual native unsigned DWORD addresses. The two affected VC7.1
  carriers preserve 17/17 exact owners (3,258 physical bytes,
  133 bound relocation fields).
- **2026-11-12 GameManager update checkpoint:** seven signed
  counter/reward operations now preserve i386 DWORD wrap (including
  signed comparison after limitB4 increment). The two real VC7.1
  carriers protect 12/12 exact owners, 4,680 physical bytes, and 415
  original-bound relocations, including OnUpdate and GameplaySetupThread.
- **2026-11-12 Enemy timeline checkpoint:** ten variable-length
  instruction payload references now use native DWORD header+0x08
  addresses rather than C++ pointer arithmetic past a header view.
  The 1,048-byte boss and 20-byte timer leaf replay byte-exact
  (2/2 owners, 1,068 bytes, 42 fields); 16 private labels PE/COFF-proven.
- **2026-11-12 Background script checkpoint:** the 2,652-byte exact
  stage-dispatch owner now uses the original native 0x14-byte record
  indexing and unsigned DWORD index wrap. Its camera/scalar/timer leaves
  all remain exact: 5/5 owners, 3,231 physical bytes, 160 relocation
  fields; 49 local compiler-label rebindings have PE/COFF proof.
- **2026-11-12 Player selector checkpoint:** the 3,238-byte exact pattern
  boss now preserves signed cooldown decrement and alternate-pattern
  arithmetic as native wrapping DWORDs. Its helper and full boss pass
  two VC7.1 carriers (2/2 owners, 3,650 bytes, 154 fields), with two
  original-PE/COFF-proven compiler-private label rebindings.
- **2026-11-12 ANM checkpoint:** the 9,776-byte exact script interpreter
  now uses native modulo-32 integer arithmetic for six arithmetic bytecode
  opcodes. Both affected VC7.1 carriers preserve 5/5 exact owners,
  10,496 physical bytes and 421 original-bound relocation fields. All 62
  compiler-private label renames were independently PE/COFF-proven.
- **Current source:** native Win32 32-bit address math and word-wrap
  semantics have been recovered across Enemy/ECL, Player/collision, Replay,
  Title, Effect, Sound, ANM, ExAttack, and Supervisor frame queues.
  Replay BeginRecordingStage now also uses native signed-stage strides for
  three frame-pointer rows, the FPS row and inline 0x18-byte buffer links;
  the two Replay focus loops now address their original 0x8E-byte side-state
  records by native DWORD side strides. EffectManager's nine indexed
  template fields now use the real table's native 0x18-byte DWORD stride;
  its 15 exact owners (2,530 bytes / 100 fields) remain cold-verified.
  ScreenEffect's shake side/camera indices now use native 0x38/0xF0-byte
  strides; all 18 same-source exact owners (2,562 bytes / 164 fields) pass,
  with nine verified VC7.1 private-label name rebindings in RegisterChain.
  The 1,323-byte Replay stage initializer and all 12 ReplayInput exact units
  (4,093 bytes / 232 fields) also remain verified.
  Replay capture now reads its original config focus byte through a native
  side offset; Replay Release now addresses real 0x18-byte stage links,
  3x10 frame pointers and FPS pointer slots by unsigned DWORD strides.
  The two affected ReplayInput carriers still protect 12/12 exact units
  (4,093 bytes / 232 fields), with both original-bound open replay guards
  checked and unchanged. Playback's prior-stage
  character marker byte and SaveReplay's compressed checksum stream now
  use genuine Win32 DWORD byte-address reads, while both complete nonexact
  Replay owner guards retain their previous 39 overlapping playback differences and 2 SaveReplay register bytes. The exact 151-byte Enemy
  attached-effect updater now computes the real 24 pointer slots by native
  DWORD addressing; both affected ECL compiler carriers protect 83 exact
  units (29,856 bytes / 1,389 fields) and preserve both large Enemy/ECL
  nonexact frontiers. The already-exact 1,758-byte Enemy draw renderer now
  uses native 0x1C-byte sample addresses and wrapped trail/vertex index
  arithmetic; its four same-source exact owners (1,922 bytes / 61 fields)
  remain verified. Enemy OnUpdate now shares the native ECL secondary VM,
  life-threshold and child-pointer slot lvalues at eight read/write sites;
  the 3,900-byte boss remains at 13 unlike bytes and the same 83 ECL exact
  siblings remain verified. Title OnUpdateResult now also reads the
  selected character-order byte through its native 32-bit table address;
  its 484/485-byte frontier remains open, while all 41 same-source exact
  Title owners (18,972 bytes / 953 fields) remain verified. Seven
  score-name table lvalues in two Title update owners now share real
  0x44C/0xDC/0x2C native bank/row/record strides; those 41 exact Title
  functions still pass their two-carrier scoped cold replay. Title replay
  metadata, path and allocation indices now use the real 0x1EC/0x200/4-byte
  record strides; the exact 1,387-byte replay-menu update and 1,825-byte
  replay-save update remain byte-exact across those same 41 Title owners.
  The standalone Player movement boss now advances its real `0x2F4`-byte option records and
  `0x0C`-byte position-history slots through native DWORD addresses; its
  full 1,900-byte, 66-field owner guard still reports just 122 differences.
  ANM file-loading now computes 33 sprite/texture/script/file-slot addresses
  from the actual 0x44/0x0C/4/0x120-byte record bases and signed indices;
  nine format/pixel-stride table reads also use real native DWORD addresses;
  both affected compiler carriers preserve 17 exact ANM owners (3,258 full
  bytes / 133 fields). The 9,776-byte exact ANM script interpreter also now
  computes its six instruction-stream jumps/advances as native 32-bit byte
  addresses; all five affected exact owners (10,496 full bytes / 421 fields)
  pass two-carrier scoped cold replay with 148 independently verified private
  `$L` reference rebindings. RunEcl's polar/bias movement helpers now form
  16 real four-byte wire-argument accesses through native DWORD addresses;
  the laser-creation opcode also writes its original 32-slot pointer table
  through the native signed selected-slot address. The full 15,564-byte
  RunEcl frontier remains at 30 unlike bytes, while the latter change's
  five involved carriers protect 87/87 exact functions (30,583 full bytes /
  1,409 relocations) with eight fresh independently verified private labels.
  ANM D3D capture now computes 17 surface, descriptor and file-slot accesses
  through authentic 0x04/0x14/0x120-byte native strides; all eight affected
  AnmManagerUtility owners (1,723 bytes / 49 fields) cold-replay exact.
  Etama controller registration and sprite selection now address the real
  0x38-byte GameManager sides and four live sprite-offset DWORD table entries;
  three affected BulletManager carriers retain 18 exact functions (6,711
  bytes / 259 fields), with 16 original-target-verified private labels.
  `GetAnm` and `SpriteHasTexture` now address the same real 0x120-byte ANM
  resource slots by native DWORD stride; 12 exact functions across three
  compiler carriers (603 bytes / 26 fields) remain verified. Etama's six
  draw heads and 23 active 0xD48-byte sprite templates now use native
  index addressing, as do seven live script reads from the real 23x5
  0x14-byte sprite-script rows (column two via an original element const
  reference). The 3 same-source exact draw/added owners (1,664 bytes /
  73 fields) remain exact; 11 further private-label bindings were verified.
  Front's 553-byte exact `InitializeResultBonus` now uses native
  0x38-byte GameManager side addresses for 12 live Player/result reads,
  including final AddScore (553 bytes / 27 relocation fields).
  Background stage camera interpolation now uses real 0x04-byte
  duration/mode and 0x0C-byte ZunTimer slot strides for ten live reads
  and writes; its 2,652-byte stage-script boss and four exact siblings
  remain cold-verified (3,231 complete bytes / 160 relocation fields)
  with 55 original-target-verified compiler-private label references.
  The already-exact 1,436-byte ExAttack Type18/24 boss now uses native
  0x0C-byte position, 4-byte angle and 0x2A4-byte VM lvalue addresses;
  its one-carrier 1,436-byte / 44-relocation exact replay remains verified.
  The separate exact Type4 attack callback now also forms live 0x0C-byte
  trail, 0x2A4-byte VM and 0x38-byte opponent-side addresses, preserving its
  739 complete bytes and 28 original-bound relocations. Type3's matching
  32-record history and live VM/side selections are native too, with its
  780-byte exact body and 38 relocations still verified. The separately
  exact Type14/22 update now forms native 0x294-byte fan and 0x84-byte
  random-row addresses plus five 0x38-byte gameplay-side lvalues; its
  824-byte complete owner and 44 fields pass scoped replay with five
  original-target-verified private labels. FrontSide's exact
  2,381-byte OnUpdate also computes its dynamic 0x2A4-byte HUD VM records
  natively; its create/callback side and auxiliary path indices join a
  one-carrier 15/15 exact replay (4,087 bytes / 176 relocation fields).
  `Background::RenderObjects` now reads actual stage-object pointer slots,
  0x2A4-byte VM entries and 0x0C-byte effect-point records by native
  DWORD index addressing; its full 3,274-byte/113-field exact owner passes.
  The 3,094-byte exact `FrontSide::OnDraw` now advances eleven real VM
  position cursors by native 0x2A4-byte address words, and both viewport
  side slots use the Supervisor 0xF0-byte stride (209 bound fields).
  The 357-byte exact FrontMessage Setup leaf now also reads four text
  and shadow color values through real native DWORD table addresses,
  retaining all 10 relocations. The separate 1,491-byte exact FrontCalc
  callback now uses original 0x64-byte panel rows/0x14-byte vertices,
  DWORD timer/color/result slots and 0x38-byte opponent-side addresses;
  all 107 original relocations remain exact. The full 3,238-byte exact Player pattern
  selector now uses native word addresses for 38 flag/grid/ReplayInput/
  config/history accesses; two impacted compiler carriers protect both
  exact functions (3,650 full bytes / 154 fields), including 18 independently
  verified private-label rebindings in ResolvePatternOffset.
  The 209-byte DrawMusicRoom leaf now reads/writes the selected actual
  0x2A4-byte song VM through native DWORD addresses; the same-source
  2,258-byte OnUpdateMusicRoom remains exact, with 2/2 owners and
  2,467 full bytes / 80 fields scoped cold-verified. The 2,045-byte exact
  Title options updater now uses native help VM, selected menu VM and
  global help-text pointer addresses; both its source owners remain exact
  (2,079 complete bytes / 138 original relocation fields).
  The distinct exact 2,803-byte Title key-config updater now also uses
  native help-text/VM byte bases and eight real 0x2A4-byte paired digit
  advances; its two same-source owners remain byte exact (2,909 bytes /
  118 original-bound relocation fields).
  The 2,968-byte exact FrontMessage opcode interpreter now reads 20 packed
  halfwords, two live wire bytes and five third-input-state references
  through native DWORD addresses; two affected compiler carriers preserve
  both exact source functions (3,007 bytes / 167 fields), with 30
  PE-target-proven private/internal label rebindings.
  The already-exact 1,939-byte Title DrawResult owner now uses native
  0x44C-byte score banks, 0xDC-byte difficulty rows, 0x2C-byte rank
  records and genuine label/alphabet indices; all 82 relocation fields
  remain exact under its single-carrier replay.
  These patches preserved exact siblings but **did not** promote any of the twelve
  open functions. Use `git log --oneline` for the actual commits; do not
  restore scratch files as if they were accepted changes.

The complete ASCII pause-menu and popup source now also uses true
0x2A4-byte VM and 0x38-byte score-popup/side native byte addresses.
Its one-carrier scoped replay verifies both large 1,776/1,172-byte
menu updates plus eight exact siblings: **10/10 owners, 4,206 bytes,
264 relocations**, with 19 COFF-proven compiler-private label rebindings.

The exact SoundPlayer queue processor now indexes all original audio
queue/pan/buffer/volume and BGM row/format storage with native unsigned
DWORD words. The full 1,456-byte `ProcessQueues` physical owner and
all its 58 fields matched; eight affected compiler carriers preserve
**20/20 exact SoundPlayer functions (5,400 bytes / 184 relocation fields)**,
with eight PE-target-verified private-label rebindings.

The `Ending::RunEndingScript` interpreter also now advances its live
0x2A4-byte ANM VM records and 68-byte text buffer by native DWORD
addresses. Its **1,727-byte complete original PE owner / 88 fields**
remains exact, as do all nine affected exact siblings across two
compiler carriers (**10/10 functions, 3,015 bytes / 159 fields**), with
31 COFF/PE-target-proven private label rebindings.

The 1,507-byte exact `SampleNetworkInput` now also uses native DWORD
addresses for its replay side record (0x8E stride), Supervisor controller
mode bytes and sixteen live 16-bit held-key cursor advances. The one-carrier
scoped replay remains **1/1 exact, 1,507 bytes / 13 relocations**.

The full GameManager setup/update/cleanup chain now selects the original
0x38-byte side records by native DWORD addresses at 48 dynamic sites.
The 1,230-byte OnUpdate, 1,689-byte GameplaySetupThread, 425-byte
CleanupGameplayState and nine other exact owners remain protected across
their two original VC7.1 carriers: **12/12 functions, 4,680 original bytes
and 415 relocation fields**.

`WinMain` now also scans its original executable directory by native
unsigned-DWORD byte addressing, including the signed initial index and
terminating NUL. The exact WinMain and CreateGameWindow source owners
remain **2/2 exact, 1,636 complete bytes / 171 fields** under the sole
relevant VC7.1 carrier.

The Title character-select family now uses true native byte addresses
for 55 live mode-0/mode-123 order, unlock and Screen16 character slots.
Its two affected VC7.1 carriers still protect **41/41 exact functions,
18,972 original bytes and 953 relocation fields**; nine StartMenu
private-label rebindings were original-target verified. The independent
OnUpdateResult nonexact owner guard remains at its prior 484/485-byte
candidate/target boundary and 218 unlike overlapping bytes.

The **9,776-byte original ANM ExecuteScript interpreter** now addresses
all seven live interpolation channels through the actual 0x0C-byte
current/end timer records and single-byte easing-mode entries.
Its two scoped VC7.1 compiler carriers verify **5/5 exact functions,
10,496 complete bytes and 421 relocation fields**, with 22 original-
PE-target-verified private-label rebindings across boss and operand leaves.

The Etama spawn/update mainline also now accesses live bullet sprite
templates by native 0xD48-byte records and all four dynamic draw-bucket
head reads/writes through genuine DWORD slots. Three actual VC7.1
carriers protect **18/18 exact source functions, 6,711 physical bytes
and 259 original relocation fields**, with seven independently proven
private-label rebindings across major bullet owners.

The exact score.dat record producer now writes/initializes its original
16x5x5 high-score table using native 0x44C-byte bank, 0xDC-byte row and
0x2C-byte rank-record address words, matching Title DrawResult's table.
Six `ScoreData.cpp` compiler carriers protect **7/7 exact functions,
2,572 complete bytes and 169 original relocation fields**.

The exact Background stage-script camera path now forms 19 actual
Supervisor 0xF0-byte viewport record addresses by native DWORD index.
The 2,652-byte script boss and four exact siblings pass one scoped
compiler carrier: **5/5 exact functions, 3,231 bytes and 160 original
relocations**, with 45 actual original-PE-target-verified private labels.

The separate 1,236-byte exact `AsciiMenuState5::OnUpdate` now also
addresses its nine live 0x2A4-byte HUD ANM VM slots through native DWORD
arithmetic, protecting all 1,236 bytes and 86 relocation fields in one
carrier with six original-target-verified private label changes.

The exact 799-byte opponent selector now also uses true native DWORD
addresses for its stack candidates, pointer/parameter/mode lists, live
used/unlock character bytes and 0x38-byte randomly chosen GameManager
side. Its sole compiler carrier remains **1/1 exact, 799 bytes and 32
original relocation fields**.

The 1,574-byte exact `MidiOutput::ProcessMsg` mainline now uses the
true 0x17-byte channel, 0x20-byte track and native long-header/file
pointer slots alongside other Midi leaves (34 live address lvalues).
The official one-carrier scoped replay protects **32 exact units,
4,270 original bytes and 132 bound relocations**; 23 ProcessMsg local
labels were PE-target verified before manifest rebinding.

The exact 643-byte PlayerShot Type2 callback now also selects its
live even-index trail collision samples from the true 0x0C-byte
Float3 array at shot +0x2B0 using a native DWORD byte address.
The sole compiler carrier retains **643/643 exact bytes and 16 fields**;
history-copy edits that shifted actual relocations were rejected.

## Remaining source-present nonexact owners

Each row describes the **complete original PE owner**, not partial exact
credit. These are the last reviewed source frontiers; recompile the chosen
owner after editing it. `--owner` routes refer to
`scripts/check-boss-frontiers.py` unless another inspector is specified.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | 15,564/15,564 bytes, 598 fields; 30 unlike | Opcode 4 timer register scheduling (7), opcode 155 flag write (23). Original owned tables intact. `--owner runecl`. |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900 bytes, 97 fields, 69 calls; 13 unlike | Draw-index and bullet-descriptor register lifetime. `--owner enemy`. |
| **Player movement** | 0x0041C170 | 1,900/1,900 bytes, 66 fields, 23 calls; 122 unlike | Option/index scheduling and Float3/history transport. `--owner movement`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210 bytes, 70 fields, 58 calls; 235 unlike | Mode byte and input receiver; 53-block CFG retained. `--owner charge`. |
| **Title OnUpdateResult** | 0x004266B5 | 484/485 bytes, 34 fields, 9 calls; 218 unlike overlaps, 1 missing | Character byte load/sign extension. **Use `inspect-title-update-result.py`**, not `--owner resultdraw` (which checks the already-exact DrawResult). |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280 bytes, 57 fields, 13 calls; 29 unlike | Selected record EDX/ECX lifetime; 19 list/path/stage-frame reads now use native 32-bit strides. `--owner replaymenu`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | 849/862 bytes, 26 fields, 12 calls; 799 unlike overlaps, 13 missing | Original 33 CFG blocks vs candidate 32; needs a real branch. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238 bytes, 53 fields, 23 calls; 2 unlike | First frame-header copy uses EDI vs ECX at +0xB7/+0xBB. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | 538/540 bytes, 32 fields, 5 calls; 39 unlike overlaps, 2 missing | Seed-load/input-delay tail. `--owner replay`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336 bytes, 20 fields, 8 calls; 26 unlike | ESI/EDI timer/owner lifetime. `--owner screen`. |
| **Supervisor network service** | 0x00430AA0 | 1,627/1,633 bytes, 106 fields; 1,381 unlike overlaps, 6 missing | Waiting/prediction block layout. Compile `SupervisorNetwork.cpp` and use `inspect-supervisor-service.py`; `--owner network` tests other owners. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51 bytes, 4 fields, 2 calls; 8 unlike | ANM manager/owner EDI/ESI schedule. Keep exact Create caller. `--owner effect`. |

## Resume with only affected compiler carriers

```bash
git status --short --branch         # recover all staged/dirty/untracked work
git log -6 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

# One original-bound nonexact owner (not a whole-game replay):
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner runecl --skip-exact-siblings

# Only exact siblings affected by an edited .cpp/.inl/.hpp:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl --dry-run
nice -n 12 python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl

# One canonical exact owner, only when relevant:
nice -n 12 python3 -B scripts/build-match-unit.py --unit ecl-pop-context
python3 -B scripts/compare-coff-function.py --unit ecl-pop-context --json

# Correct Title OnUpdateResult path (resultdraw is DIFFERENT):
nice -n 12 python3 -B scripts/build-match-unit.py --unit title-screen-setup-thread
python3 -B scripts/inspect-title-update-result.py build/matching/TitleScreen.obj

# Network service: standalone source, no exact siblings in that .cpp:
# The generated build/matching directory may be absent after cleanup.
mkdir -p build/matching
nice -n 12 scripts/compile-probe.sh src/SupervisorNetwork.cpp \
  build/matching/SupervisorNetwork.obj \
  /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src
python3 -B scripts/inspect-supervisor-service.py build/matching/SupervisorNetwork.obj

# Documentation/ledger checks (no game compiler required):
python3 -B scripts/validate-docs.py
python3 -B scripts/ci.py
git diff --check
```

The scoped exact replay **does not** accept a nonexact boss; run its
complete original-bound inspector separately. Some standalone nonexact
sources have no canonical exact unit and correctly cannot run scoped
exact replay. Exact acceptance covers **complete physical code, tails,
owned switch tables and all original-bound relocations**, not a partial
code slice or a similarity score. Do not use `--owner all`, an all-source
cold replay or `ci.py --full` for routine iteration. Do not change the
pinned VC7.1 profile or add inert register hints to force a match.

The 2026-11-12 original-attested Player collision and Title result
experiments are summarized in the knowledge base. Collision loop/type/
bounds spellings did not recover the missing original CFG block; Title
selected-character snapshot/reference/reload changes produced 483-488
byte owners rather than the exact 485. No source candidate from those
failed trials was installed.

## Best next questions; avoid repeating failed experiments

Work on large bosses **and** small leaves. Previous changes to natural
32-bit addresses and signed wrap are already included in maintained source.

- **RunEcl:** opcode 4 has seven register bytes; opcode 155's byte-to-bit-24
  XOR/write has 23. Fields/tables are already right. Pointer aliases,
  bitfields, union operands, timer receivers and source-order variations
  generally preserved 30 differences or broke table targets. See the
  [opcode notes](KNOWLEDGE_BASE.md); don't replay cosmetic rewrites.
- **Enemy:** 13 specific register bytes remain in draw-group and
  spawn-descriptor construction. Avoid improving a local register at
  the expense of original literal relocations.
- **SaveReplay:** only +0xB7/+0xBB differ; frame-header constness,
  natural dest arithmetic, typed copies and moving link initialization
  have not solved the EDI/ECX choice. Preserve the 23 calls.
- **Player collision:** one genuine original CFG block and 13 bytes
  are missing; manufactured `goto`, `ret` or padding is not recovery.
- **Effect callback:** eight receiver/manager register bytes remain;
  pointer/ref/inline wrappers seldom change them. Its Create caller
  remains exact.
- **Supervisor network service:** the retail owner has 1,633 bytes versus
  the 1,627-byte candidate, with 106 relocations and 41 direct calls but a
  different wait/success block schedule. Reversing branches or sharing an
  exit did not restore the original layout; do not mistake fewer overlapping
  differences in a longer candidate for a strict improvement.
- **Title OnUpdateResult / DrawReplayMenu:** distinguish them from the exact
  DrawResult leaf. The result updater needs its actual signed character-byte
  read and one missing byte; replay menu has 29 register-lifetime differences.
  Pure pointer casts and cached source-index snapshots have not fixed either.

The pinned neighboring `_reference/Cirno-Labs-th09`, TH08 and TH095
code are hypothesis sources, **not** original TH09 evidence.

## Protected inputs and scratch lifecycle

The obsolete ignored `build/` cache was removed for this handoff,
including `build/gpt-web/` trial copies/PDBs/diagnostics and
`build/matching/` compiled objects. **The absence of an object is expected**:
use `scripts/build-match-unit.py --unit <unit-id>` for a selected exact
owner or `scripts/replay-scoped-exact.py --changed-file <source>` for its
*affected* exact cohorts. For a standalone nonexact probe, recreate
`build/matching/` and use `scripts/compile-probe.sh` with the pinned profile
above. Do not run project-wide `--owner all` or cold replay to rebuild caches.
Removing generated objects does not revoke existing ledger matches, nor does
an old receipt prove the current source without recompilation.

**Preserve** ignored `resources/th09.exe`, `.tools/wine/` (the game-specific
VC7.1/Wine runtime and prefix), `.analysis/ghidra-direct/` (saved Ghidra
projects), `.analysis/factory-runtime/`, shared Factory tools and
`_reference/`. The six small ignored
`.analysis/gpt-web/20261111-prove-*.py` scripts are retained *only in this
live worktree* as optional private-label same-section PE/COFF proof helpers;
they are not canonical evidence and require fresh selected objects first.
Do not delete these protected inputs to save space, or commit originals,
SDKs, executable assets or analysis databases. No remote push was requested.
See [tool routing](TOOLS.md) and [shared runtimes](SHARED_TOOLS.md).

Commit real improvements as `gpt-web: ...`. Always inspect and recover
dirty/untracked work instead of deleting it. Check live Git ahead/behind
status; do not carry forward an old count.

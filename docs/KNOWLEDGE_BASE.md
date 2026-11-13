# TH09 reconstruction knowledge — durable findings

This file keeps **verified original layouts, compiler/COFF lessons,
and the unsolved source questions worth pursuing** for Japanese TH09 v1.50a.
It is not a commit-by-commit journal or a live exactness ledger.
The authoritative [current handoff](RE_HANDOFF.md) lists every open owner,
its current comparison and the right inspector; [generated progress](PROGRESS.md)
is derived from the canonical ledgers. This is a historical evidence notebook;
its dated experiments and prior source spellings can be superseded, and
original-bound diagnostic differences do not mean canonical exactness.
Old full handoff checkpoints are recoverable through Git history.

**Never promote a near match** from these notes. Only a complete target-bound
VC7.1 comparison of code, owned tables/tails and real relocation identities
establishes exactness. Dated paragraphs below preserve *observations at the time of each experiment*,
not live compiler receipts. Old exact counts, label names and temporary build
paths can be superseded; do not apply them without checking live source and
canonical ledgers. Any claim describing a "current" candidate is historical
unless confirmed in the active handoff. In particular,
GameplaySetupThread and Title DrawResult are exact; the 12 active nonexact
owners and the unfinished product/runtime gates remain separate.

## Where to look

**Size accounting:** the canonical nonexact total (28,145 bytes) sums
twelve authored `functions.csv` logical extents. The original-bound
RunEcl, Enemy OnUpdate and Player movement inspections additionally
cover 772, 17 and 65 bytes of owned physical code/tails/tables respectively,
so summing the twelve physical comparison sizes instead yields 28,999 bytes.
Do not conflate physical-owner proof with the exact authored-byte ledger:
at the 2026-11-13 ledger check, 263,786/291,931 reviewed-authored bytes
(90.3590%) were exact. See the live generated [PROGRESS.md](PROGRESS.md)
for changes; the complete-image inventory and Windows product remain open.

- **Current exactness/frontiers and safe rebuild commands:**
  [RE_HANDOFF.md](RE_HANDOFF.md); generated figures in
  [PROGRESS.md](PROGRESS.md).
- **ECL/Enemy:** [RunEcl opcode 4/155](#runecl-opcode-4-and-opcode-155),
- **Player:** [CheckBulletCollision boss register-lifetime frontier](#2026-11-13-player-checkbulletcollision-boss-register-lifetime-frontier).
  [Enemy OnUpdate](#enemy-onupdate), and the canonical `EclRun*.inl` views.
  Original switch tables and private-label bindings matter more than local
  instruction similarity.
- **Player/Replay:** [movement, charge and collision](#player-movement-charge-and-collision),
  [SaveReplay/BeginPlaybackStage](#replay-savereplay-and-beginplaybackstage).
- **Title/UI:** [replay-menu and result-update blockers](#title-replay-menu-and-result-update).
- **Other open owners:** [ScreenEffect, network, Effect callback](#remaining-screeneffect-network-and-effect-callback-frontiers).

Keep the canonical match manifests, compiler source profiles and original PE
as the proof inputs. The longer sections below are topic-specific evidence,
not instructions to restore old scratch experiments or previous snapshots.

**Retention at the 2026-11-13 handoff:** superseded probe binaries, copied
sources, JSON result dumps and compiler intermediates were removed to save
space. A small number of ignored `.analysis/gpt-web/*prove*.py` helpers remain
as **templates**, not current-source proof receipts. Recompile the affected
carrier and check original PE bytes and all relocations afresh. Do not treat
a missing historical experiment script or a dated receipt as a blocker.
The saved `.analysis/ghidra-direct/` analyzer project, original executable,
compiler/toolchain and Git history have not been pruned.

## Practical interpretation

- An odd signed array index or 32-bit integer wrap is not automatically a
  new original C++ type/guard. For fixed i386 owners, form physical native
  DWORD addresses from the **real verified subobject** without inventing
  bounds checks, then compare the complete affected compiler carrier.
- Changing a shared .inl or owner can renumber VC7.1 private $L labels.
  Rebind names only after checking each actual same-section COFF definition
  and original PE target **and all affected exact owner bytes**. An old
  generated PDB/OBJ is never a substitute for rebuilding current source.
- An original-bound nonexact inspection validates a *frontier* and may pass
  despite residual differences. Do not confuse it with canonical exact
  credit. A previous negative register-lifetime experiment is a clue,
  not proof that every natural source form is exhausted.
- For the other agent's fastest starting point, see the
  [12-owner table and scoped commands](RE_HANDOFF.md). Do not run a routine
  all-owner cold replay, do not change VC7.1 profiles to chase registers,
  and do not treat adjacent-game decompilation as an original TH09 Oracle.

## 2026-11-11 native Effect template indexed storage

`EffectManager::InitializeEffect @ 0x0040CA60` reads nine fields from the
original `g_EffectTemplates` table at `0x004A0B48`. Its signed effect ID
selects a genuine 0x18-byte `EffectTemplate` record: retail Win32 forms
`base + uint32(effectId) * sizeof(EffectTemplate)` modulo 2^32. Ordinary
`g_EffectTemplates[effectId]` imposed a C++ array-bound requirement absent
from the original index calculation. A single inline const reference to the
real table entry now preserves the native address word at every access,
without caching a read, creating a new backing table, or adding a guard.

After applying the maintained source change, the pinned VC7.1 one-carrier
scoped cold replay passed **15 complete exact EffectManager owners / 2,530
physical bytes / 100 relocation fields**, including all **407 bytes and 17
fields** of `InitializeEffect`. The separately original-bound 51-byte
`AddedCallback` still has four bound fields, both correct calls and its
existing eight nonexact register bytes. This improves source semantics but
adds no exact function or whole-product/runtime credit.

## 2026-11-11 native ANM capture surface and file-slot selectors

The exact `AnmManager::CaptureToSurface` and `CaptureToTexture` consumers
use genuine native arrays in the one `AnmManager` allocation: surface
pointers at +0x123FC and +0x1247C (four-byte pointer strides), D3D
surface information at +0x125FC (0x14-byte descriptors), and 0x120-byte
file-slot records with the native loaded texture pointer at each slot +0x38.
Seventeen dynamic surface/descriptor/file-slot lookups now reuse the same
`NativeAnmSurfaceSlot` lvalue already used by the exact backbuffer copy
path. Each signed index is converted into the actual unsigned 32-bit
`base + index * native stride` byte address; original null/Direct3D return
checks, `ReleaseSurface` calls, repeated member reads and allocation
ownership are preserved without a new cache, guard or backing object.

Three isolated and combined pinned VC7.1 source builds stayed fully exact
for all eight actual `AnmManagerUtility.cpp` owners. The installed combined
change passed the **one-carrier targeted cold replay: 8/8 complete exact
functions, 1,723 target bytes and 49 original-bound relocation fields**.
There was no full cold replay and no additional exact function credit; the
result is native D3D capture/source-memory fidelity rather than product or
runtime completion.

## 2026-11-11 native ANM file-slot lookup consumers

`AnmManager::GetAnm` and `AnmManager::SpriteHasTexture` now share the
same genuine Win32 0x120-byte file-slot stride as the exact resource loader.
The former addresses the real loaded resource at manager +0x24 through
`uint32(anmIdx)`; the latter reads the live texture pointer at manager
+0x38 through the signed sprite-owned ANM index. Crucially, the runtime's
lexical `anmFiles[1]` is a *one-slot view* over the larger original file
allocation: indexing it as a C++ one-element array would impose a bound
missing from the original i386 address arithmetic. The maintained source
forms both lvalues from their real bases plus `uint32(index) * 0x120`.
It preserves the original null and negative-index tests, repeated live
reads, callee identities and physical resource ownership, with no new
validation, cache, or storage object.

Five isolated VC7.1 inline/lexical variants matched all **47 bytes of
SpriteHasTexture** and all **20 bytes of GetAnm**. The installed sources
passed focused cold replays for their two actual translation units and
three compiler carriers: **12/12 previously exact functions, 603 complete
physical bytes and 26 original-bound relocation fields**. Seven
`DrawPlayerBullet` compiler-private `$L` references were rebound only
after confirming real same-section COFF definitions and original PE
jump targets. No new exact owner or native-game/runtime credit is claimed.

## 2026-11-11 native ANM texture-format and byte-stride table reads

The exact ANM texture-loading path (`AnmGetFormat`, `CreateEmptyTexture`,
`CreateTextureFromFile` and `CreateTextureFromAnm`) reads seven independently
live entries from the real `g_AnmTextureFormats` table and two entries
from the real `g_AnmTextureBytesPerPixel` table. The file-controlled
`format`/`textureHeader->format` values select genuine four-byte table
words. The maintained source now forms native i386 DWORD byte addresses
from each actual initialized table base instead of depending on C++
array-bound preconditions. Original texture creation, source-pitch and
D3D error paths remain unchanged, without new range checks or caches.

An inline **const lvalue** for D3D formats is essential: returning the
D3DFORMAT by value shortened the exact `CreateTextureFromFile` and
`CreateTextureFromAnm` owners by one byte each, while reference, lexical
and native macro forms retained their entire original instruction streams.
The installed const-reference and native pixel-byte reader passed scoped
cold replay for both genuinely affected compiler profiles: **17/17 exact
ANM owners, 3,258 complete bytes and 133 original-bound relocations**.
This is source-semantic Win32 address fidelity, not added exact-function
credit or whole-game runtime closure.

## 2026-11-11 native ANM sprite, texture, script and file-slot addressing

The original ANM resource-loading path owns real dynamically allocated
`AnmLoaded` sprite, texture and script-pointer arrays, plus the real
`AnmManagerReadView::slots` beginning at manager +0x24. The C++ loader now
forms each record lvalue as `uint32(real allocation base) +
uint32(signed index) * sizeof(native entry)` for **19 sprite reads/writes**
(`0x44` bytes per `AnmLoadedSprite`), **nine texture reads/writes** (`0x0C`
bytes per `AnmEntry`), **one script entry store** (four-byte pointer), and
**four manager file-slot uses** (`0x120` bytes per slot). These live indices
come from ANM file entries, the caller's sprite/script IDs and the manager
file-slot ID. No new range gate, alias allocation, separate layout or
read-value cache was introduced; every original lookup remains at its own
execution point.

Three source-cold sprite-address variants and five texture/script/manager
combinations were compiled with pinned VC7.1. The four directly impacted
canonical units (`AnmLoaded::LoadSprite`, `LoadExternalTextureData`,
`LoadTextureData`, `ReadAnmEntries`) all remained completely byte exact,
including the `138`-, `150`-, `558`- and `390`-byte original physical owners.
The installed combined source passed the **two affected compiler carriers,
17/17 existing exact ANM owners, 3,258 complete physical bytes and 133
original-bound relocations**. This improves real memory-model fidelity; it
does not increase the exact owner count or establish a native product/runtime.

## 2026-11-11 ANM interpreter native instruction-byte addressing

The exact `AnmManager::ExecuteScript @ 0x00436F30` (9,358 covered code
bytes, **9,776 complete original physical bytes** including its owned
jump tables and tails) consumes the genuine variable-length ANM script
buffer. Three `instruction->instructionSize` advances now form the original
Win32 `uint32(instruction address) + uint32(u16 instructionSize)` word.
Three branch/interrupt targets now form `uint32(vm->beginningOfScript) +
uint32(instruction->intArgs[0/1/2])`, preserving the original signed wire
DWords, authentic script base, execution/interrupt branches and every
separate live read. These are byte offsets within the *same script allocation*,
not native C++ array elements; there is no new bounds gate, shadow script
storage, alternate instruction representation or call.

Three pinned VC7.1 candidate combinations (step-only, jump-only, both)
were compared against **every one of the 9,776 physical bytes and all 368
original-bound relocation fields**. Each remained byte-identical after
rebinding actual same-section COFF definitions. The installed combined
candidate rebinds **107 compiler-private `$L` references in the interpreter**
and **41 references in the four same-source AnmVm variable helpers**; every
reference's original PE target, COFF definition, offset and kind was verified
before updating `config/match-units.toml`. The genuine two-carrier scoped
cold replay passed **5/5 exact functions, 10,496 full physical bytes and
421 relocation fields**. `interpCurrentTimers[i]` is a native `ZunTimer`,
not an integer array; an experimental arithmetic rewrite failed its real
operator ABI and was not installed. This improves native script-pointer
semantics but does not grant new exact owners or whole-game runtime proof.

## 2026-11-11 native FrontCalc panel vertices, timers and result words

The exact `FrontCalcCallback` owner at its original 1,491-byte extent
walks the same real `FrontFactoryConstructView` allocation used by the
already-exact FrontSide and FrontMessage callbacks. Each of its two
`panelVerticesE86C` rows is five true **0x14-byte** vertex records
(row stride **0x64**), with separate two-element DWORD arrays of
`panelColorsE934` and `panelTimersE93C`. The nested `side` and `vertex`
loop indices now address the original inline storage with Win32
unsigned-DWORD base-plus-index arithmetic. The three transition side
receivers use the live signed `1 - transitionSide10964` over real
**0x38-byte GameManager side records**. Two score/result table reads
from the actual global `g_TransitionResultTable` and
`g_TransitionResultTableAlt` use original DWORD addressing. The
maintained code changes **12 live lvalues** in place (three vertices,
three timers, one color, three side receivers, two result values), without
adding a range check, second table, cache, call, or changing the timer,
score or transition state machine.

The pinned VC7.1 `/O2 /Ob1` probe matrix separately compiled panel
vertices, timer words, colors, side receivers and both result tables;
all individual and combined variants matched the whole **1,491/1,491
physical bytes**. The installed combined source passed the single
original-profile compiler carrier's scoped cold replay: **1/1 exact
owner and all 107 original-bound relocation fields**, with no private
label rebindings. This is better native data addressing of an already
exact callback, not an additional exact owner or a native runtime proof.

## 2026-11-11 native FrontSide draw position cursors and camera side slots

The original exact `FrontSide::OnDraw` (3,094 complete bytes) walks eleven
HUD VM position arrays by a projection cursor at `AnmVm+0x208`. The second
position is at `+0x288`, and consecutive genuine VM records have the
original **0x2A4-byte stride**. The maintained source now advances each
cursor using the native Win32 word `uint32(cursor) + sizeof(AnmVm)` rather
than incrementing a C++ projection pointer across a different object's
array. The two initial camera-shake clears likewise select the actual
`Supervisor::ctorArray078[3]` 0xF0-byte viewport record using the live
signed `frontSide->sideIndexA66C` via `uint32(base) + uint32(side)*0xF0`.
This retains all original independent writes, viewport configuration,
HUD draw calls, transitions, primitive construction, and loop bounds,
without an additional safety check, pointer cache, allocation, or call.

Three separate and combined VC7.1 `/Ob0` trials proved **3,094/3,094
complete bytes**, and the installed combination passed the sole
`FrontSideDraw.cpp` compiler carrier's scoped cold replay: **1/1 exact
owner, 3,094 physical bytes, all 209 original-bound relocations**.
There were no compiler-private label rebindings, no new exact owner
credit, and no claim about native product or runtime closure.

## 2026-11-11 native Background RenderObjects stage and VM record indices

The original exact `Background::RenderObjects` (3,274 complete physical
bytes) walks variable-length stage object instances and quad records.
Its signed on-disk `instance->objectId` selects the original
`Background::stageObjects` pointer allocation with **four-byte pointer
slots**; the quad's signed `vmIndex` selects a genuine 0x2A4-byte
`stageObjectVms` allocation, whose fields are shared with the exact
ANM loader. Special-effect point publication uses the real inline
`BackgroundSpecialEffectPoint[32]` records (0x0C-byte stride) in the
same Background allocation. Both independent live reads of
`specialEffectPointCount` remain at their original source positions.
The maintained code forms all three lvalues as
`uint32(real base) + uint32(live index) * sizeof(actual entry)`, without
inventing a new guard, cache, object, or altered render/fog decision.

Four individual and combined pinned VC7.1 compiler experiments preserved
**3,274/3,274 full original PE bytes**. The installed combination passed
the one actual `BackgroundRender.cpp` compiler carrier's scoped cold
replay with **1/1 exact function, 3,274 complete bytes and 113
original-bound relocation fields**. No private-label manifest rebindings
were needed. This improves source-level native stage-memory semantics,
not the number of exact functions, complete native product or runtime.

## 2026-11-11 native Background stage interpolation record indices

The original `BackgroundStageView` owns three interpolation arrays:
`interpolationDurationBF4[5]` at +0xBF4 (4-byte entries),
`interpolationTimersC08[5]` at +0xC08 (the canonical 0x0C-byte
`ZunTimer` records) and `interpolationModesC44[5]` at +0xC44
(4-byte entries). `InterpolateBackgroundCameraVector` reads and writes
**ten independent lvalues** selected by its live signed `index`, in
addition to receiving the original four camera-vector inputs.
The maintained code now calculates each access from its actual array
base plus `uint32(index) * sizeof(native record)` in an in-place lexical
macro; there is no helper ABI, alternate timer class, added range gate,
reordered timer increment, changed easing mode or cached read.

Four independent and combined pinned VC7.1 `/Ob0` source probes proved
**2,652/2,652 complete bytes of BackgroundRunStageScriptPhase**
(including its owned tables/tails and 133 original-bound relocations)
and **448/448 bytes of InterpolateBackgroundCameraVector** (23 fields).
The installed lexical version cold-replayed the one affected compiler
carrier, passing **5/5 exact functions, 3,231 complete bytes and 160
relocation fields**. The optimizer's 55 changed private `$L` reference
spellings were rebound only after independently verifying each actual
same-section COFF definition, relocation identity and original PE jump
target. Existing exactness was preserved; no new exact owner or native
runtime-product closure is implied.

## 2026-11-11 native Player strategy selector, grid and replay input records

The original `PlayerUpdateSelectorState` is a **3,238-byte** authored
combat-pattern selector. It consumes actual global `g_PlayerPatternGrid`
(12 rows of 10 signed DWORDs), `g_PlayerPatternFlags` (signed DWORD
entries), `g_PlayerPatternAlternates` (signed DWORD candidates) and the
real `g_PlayerPatternConfigs` (0x1C-byte configuration records), not
separate bounded or checked duplicate tables. The original on-player
inputs are `g_ReplayInputStates` records of **0x8E bytes**, whose +0x2C,
+0x32 and +0x34 protocol words are shared with independently exact
ReplayInput owners. Finally, the selector's two eight-entry recent-flag
history shifts write `PlayerGameplayHeaderView::recentPatternFlags30`
from its true record base. The maintained code now uses original Win32
unsigned-DWORD base-plus-scaled-index lvalues for **38 live accesses**:
12 flag reads, two grid reads/writes, 20 replay-input state reads/writes,
one alternate pattern, one configuration receiver, and two history moves.
No new range checks, side-state caches, shadow global arrays or changes
to collision-path predicates, Player callback order or RNG calls were
introduced. Signed indices retain the original modulo-32 address-word
behavior, including the previous history slot as current address minus
one DWORD.

Six independent native-address families and the combined core source
were compiled using pinned VC7.1, maintaining the complete original
**3,238/3,238-byte selector owner and all 136 relocations** in every
trial. The installed combined source, including history copies, passed
the two actual affected compiler carriers' scoped cold replay:
**2/2 exact units, 3,650 complete physical bytes, 154 original-bound
relocation fields**. `PlayerGameplayMethods::ResolvePatternOffset`
remained byte exact at 412 bytes; the compiler renumbered eighteen
private `$L` references, rebound only after verifying actual same-section
COFF definitions, unchanged offsets/types and original PE jump targets.
No new exact owner or Windows native runtime credit is claimed.

## 2026-11-11 native MusicRoom selected-song VM lvalues

The original `TitleScreenView::DrawMusicRoom` is a fully exact 209-byte
leaf. It reads and temporarily changes a live selected-song ANM VM
at `vms[musicSelectedSongIndex + 159]` and restores its original color
and position after the draw. The maintained source now forms **eight
independent reads/writes/call receivers** from the real TitleScreen
`AnmVm` allocation via `uint32(&vms[0]) + uint32(songVmIndex)*0x2A4`,
using the canonical `sizeof(AnmVm) == 0x2A4` layout with no new bounds
gate, cached VM pointer, caller, or changed save/draw/restore order.

During the same targeted research the 2,258-byte `OnUpdateMusicRoom`
was separately probed with signed script-record, native music-track,
help-VM and track-list edits. Those variants differed only in four
x86 SIB base/index order bytes (`[eax+edi+disp]` vs
`[edi+eax+disp]`) or additional codegen, so were *not installed*;
the exact update owner is still protected. The accepted DrawMusicRoom
edit passed one original-bound two-unit scoped VC7.1 replay:
**2/2 exact functions, 2,467 complete bytes and 80 original relocations**,
including 209/209 DrawMusicRoom bytes. No new exact owner or original
Windows product/runtime credit was claimed.

## 2026-11-11 native Title Options help-text and ANM VM selector slots

The original exact `TitleScreenView::OnUpdateOptions` (2,045 complete
physical bytes) uses the genuine TitleScreen embedded 0x2A4-byte ANM VMs
for the nine help-text rows and the selected help cursor, and the real
TitleScreen `vms` allocation for the current keyboard/menu selection.
Seven independent runtime accesses now use native Win32 byte-address
lvalues: two help VM pointers (`i` and `keyboardSelection`), four
selected menu-VM receivers and one global `g_TitleOptionsHelpText[i]`
pointer table entry. Help VM pointer lvalues keep the original opaque
`AnmVmView*` ABI, not an incompatible `AnmVm*`; the help-text pointer
read keeps the original byte-base calculation into four-byte global
pointer slots. Original audio volume, help message, UI fade, and
`PlayMenuSound` calls stay in place. No bounds checks, new allocation,
cache, additional calls or alternate TitleScreen view were introduced.

Five independent and combined pinned VC7.1 `/O1 /Ob1` probes showed
which native expression spellings preserve the original exact code:
a direct DWORD-base `const` help-text lvalue grew the owner two bytes
and was rejected; the real byte-base pointer returned to the correct
original instruction schedule. The installed combined source passed
one actual compiler-carrier scoped cold replay: **2/2 exact functions,
2,079 complete original bytes and 138 bound relocation fields**
(2,045-byte OnUpdateOptions and 34-byte PlayMenuSound). No exact owner
was newly promoted, and no Windows native runtime/product proof follows.

## 2026-11-11 native Title key configuration help and digit VM strides

The original exact `TitleScreenView::OnUpdateKeyConfig` (2,803 complete
physical bytes) has a live help-row index and several display-digit VM
pointers in the actual TitleScreen state. The help-row source addresses
one of ten real global `g_TitleKeyConfigHelpText` pointers (four-byte
entries), and the embedded helper VMs use the canonical **0x2A4-byte
`AnmVmView`** records starting at TitleScreen+0x11BA0. During display
construction, `vmPair` also walks eight two-VM increments of the actual
TitleScreen `vms` allocation. The maintained source now forms **ten
native byte-address operations** (one helper-text pointer, one helper
VM receiver, eight two-VM advances) from their live bases, without
introducing a second table, memory snapshot, added bounds guard or extra
callee. The result preserves the original opaque `AnmVmView*` ABI and
repeated live side/key bindings.

The pinned VC7.1 `/O1 /Ob1` trial matrix showed that directly forming
help/VM addresses as integer-DWORD base operations changed codegen,
whereas original native `u8*` byte-base expressions retained exact
instruction scheduling. The combined installed variant cold-replayed
the one affected source carrier with **2/2 exact owners, 2,909 complete
PE bytes, 118 original-bound relocation fields** (2,803-byte options
updater plus 106-byte duplicate-key leaf). Side-selection experiments
which changed byte-exact code were rejected. No additional exactness
credit, native product or runtime completion is claimed.

## 2026-11-11 native FrontMessage variable-length operands and input state

The large original `FrontMessageRuntimeView::Update` interpreter has an
original **2,968-byte physical owner** (including its native dispatch
information) and 167 relocation fields. Its opcodes 2/3/5 read two
`short` values from packed instruction bytes immediately after the
real variable-length wire header; opcode 5 also reads a live third
control byte. Twenty halfword and two byte operand reads now use the
actual wire pointer plus native unsigned-DWORD byte offsets, rather than
C++ indexing an artificial array of unknown extent. Five skip/confirm
input reads likewise use the genuine global third `ReplayInputState`
record at base + `2 * 0x8E`, shared with other Replay and Player owners.
No bounds checks, alternate state allocations, cached operands, extra
calls or reordering of instruction dispatch and message/ANM updates were
introduced.

Four grouped pinned VC7.1 compiler experiments showed the installed
combined source retains **2,968/2,968 complete original PE bytes and all
167 original-bound relocations**, including intact table bytes. The
compiled source renumbered **29 `$L` references and one `run_scripts`
local branch symbol**; each new source symbol was rebound only after
proving its actual same-section COFF definition, identical relocation
offset/type and unchanged original PE target. The final two-carrier
scoped cold replay passed **2/2 exact source functions, 3,007 complete
physical bytes and 167 relocation fields** (the main interpreter and its
independent 39-byte text decode leaf). There is no new exact owner or
native Windows runtime/product claim.

## 2026-11-11 native Title DrawResult score banks and rank records

The original exact `TitleScreenView::DrawResult` (1,939 physical bytes)
reads the real `g_TitleScoreTable` owner: **16 banks × 5 difficulty rows ×
5 ranked `TitleScoreRecordView` entries**, 0x2C bytes per score record.
The maintained source now forms the live bank pointer with the actual
0x44C-byte bank stride; four difficulty-row advances use the 0xDC-byte
row stride, and fifteen rank name/score/digit reads take their lvalues
from the true 0x2C-byte record addresses. Five rank-label pointers are
independently read from the original global four-byte pointer table,
and the 6×16 name keyboard's live alphabet character is read through
its real byte base with native 32-bit index addition. These address
calculations preserve the source's repeated reads, score string formatting,
branch decisions, keyboard highlight and original AsciiManager calls.
No new bounds gate, cache, array allocation or alternate score table
has been introduced.

Individual pinned VC7.1 trials for bank, rank, label, alphabet and
row stride, plus their combination, all preserved **1,939/1,939 complete
original PE bytes**. The installed combined lexical lvalues passed the
single affected `TitleScreenResultDraw.cpp` compiler carrier's scoped
cold replay: **1/1 exact owner, 1,939 physical bytes, all 82
original-bound relocation fields**. No changed private labels were
needed. The score producer and renderer still share the original table;
this is a source-memory fidelity gain, not a new exact function or native
Windows runtime claim.

## 2026-11-11 native Replay capture focus and release buffer slots

The canonical `ReplayManagerView` owns real 0x18-byte
`stageBuffers[10]` link records at +0x1C. Its recorded replay payload
contains three 10-pointer frame-start rows and a separate 10-pointer
FPS row. The exact `ReplayManagerView::Release` now forms the stage link,
three independently read/written `frameStart[stream][stage]` lvalues,
and FPS read/write from actual native unsigned-DWORD word addresses.
The stream index is scaled by the original 10-pointer row before the
stage pointer index, with no second ownership model, new guard, cache,
or altered free/zero chronology. The exact
`ReplayManagerView::CaptureFrameSyncState` also reads the genuine
`GameConfiguration::sideFocusMode` byte at +0xB4 by native side-DWORD
address rather than bounded array indexing. Eight source lvalues were
changed; original input calls and focus-mode branches remain live.

Five independent/combined pinned VC7.1 source probes preserved the
entire 257-byte capture callback and 318-byte release owner. The
installed source passed both real `ReplayInput.cpp` compiler carriers:
**12/12 exact functions, 4,093 complete physical bytes and 232
original-bound relocation fields**. Both affected-source nonexact
owners were also guarded independently against the complete original:
`BeginPlaybackStage` remains 538/540 bytes, 32 relocation fields and
39 unlike overlapping bytes; `SaveReplay` remains 1,238/1,238 bytes,
53 fields, 23 ordered calls and only two original register bytes unlike.
No full cold replay was run, nor was any open function promoted to exact.

## 2026-11-11 native ASCII pause-menu VMs and score-popup rows

The original `AsciiManagerMenu.cpp` owns ten exact functions, including
the **1,776-byte physical `PauseMenu::OnUpdate`** and **1,172-byte
`AsciiMenuState4::OnUpdate`**. Their real `menuSprites[7/5/4]` arrays
contain canonical **0x2A4-byte AnmVm records**. Twenty-four dynamic
`menuSprites[i]` accesses now use native Win32 `uint32(actual base) +
uint32(i) * sizeof(AnmVm)` addresses, preserving each independently
live VM interrupt, script call, and visibility test. The actual
`AsciiManager::scorePopups[2][100]` allocation contains **0x38-byte
`AsciiManagerPopup` records**. The popup row is now selected by native
`uint32(playerIndex) * 100 * sizeof(AsciiManagerPopup)`; advancing the
bank by 100 records likewise computes a native byte address rather
than stepping a C++ pointer outside its inner 100-record row. Popup
processing also uses the real 0x38-byte GameManager side records and
forms the last text character address from the popup's live byte
character count. The original two-side traversal, 100-entry loop,
draw calls, and timer/alpha update remain unchanged. No new bounds
gates, caches, shadow objects, or altered callbacks were added.

Isolated native menu-VM and popup-path trials, plus their combination,
were compiled with pinned VC7.1 against every complete original
physical owner. **All ten functions match 4,206/4,206 bytes and all
264 relocation fields**, including the 503-byte popup renderer and
253-byte AsciiManager OnUpdate. Nineteen compiler-private `$L` reference
names differed already under the clean baseline; each was rebound only
after an independent same-section COFF definition and original PE
branch-target proof, without changing any relocation offset/type/target.
The installed source passed the one genuine compiler-carrier scoped
cold replay: **10/10 exact owners, 4,206 original bytes and 264
original-bound relocations**. This is native source ownership/stride
fidelity on already-exact code; no newly exact boss or Windows runtime
claim is implied.

## 2026-11-11 native SoundPlayer queues, DirectSound pointers and BGM records

The original `SoundPlayer::ProcessQueues` owns 1,414 reviewed logical
bytes and **1,456 complete physical bytes**, including its owned
switch tails. Its 12 real `soundQueue` DWORDs and 12 separate request
counts, `soundQueuePanData[12][128]` DWORDs, 128 live DirectSound buffer
pointers and global `SoundBufferIdxVolume[54]` eight-byte records are
all part of the original SoundPlayer/SFX allocations. The maintained
source now forms 15 live queue, pan, buffer and volume lvalues through
`uint32(real base) + uint32(index) * sizeof(native record)`. The BGM
command dispatch has two further original-format lookups: the true
`bgmFileNames[32][256]` byte rows and the actual `bgmFmtData` format
record allocation. Their signed command/format indices now likewise
form native unsigned-DWORD byte addresses, retaining the original
pointer/array lvalues. All original sound queue loops, 12×128 pan
averaging, wave-format lookup, DirectSound calls, command phases,
active record reads, and audio-volume transformations remain live.
No new bounds gate, cache, shadow array, command or buffer ownership is
introduced.

All seven independent native-address variants and the installed
combined source matched the complete original **1,456/1,456-byte
ProcessQueues owner and all 58 relocations**. Eight compiler-private
`$L` references were rebound only after verifying actual same-section
COFF definitions, unchanged field offset/type and identical original
PE targets. The installed source passed a scoped cold replay of all
**eight genuinely affected VC7.1 compiler profiles**, protecting
**20/20 exact source functions, 5,400 complete physical bytes and
184 original-bound relocations**. This improves native audio-memory
fidelity in both major queue processing and related exact leaf carriers,
without promoting any of the still-open source-present functions or
claiming a native Windows product/runtime.

## 2026-11-11 native Ending script ANM VM and live text byte slots

The exact `Ending::RunEndingScript` interpreter has **1,499 reviewed
logical bytes and 1,727 complete original physical bytes**, including
its owned original switch table and tails. `@a` takes a signed `vmIndex`
from the actual ending script file and addresses one of the real
`Ending::endingVms` 0x2A4-byte ANM records. Text events select the same
VM storage using the live signed `nextTextVmIndex`. The runtime also
updates a genuine 68-byte stack text buffer through its `textIndex` and
`temporary + 1` positions. The maintained code now forms these
independent VM and text-buffer lvalues as native Win32 unsigned-DWORD
byte addresses, including script-driven out-of-range indices without
a synthetic bounds gate. The two 15-VM callback loops and the 16-VM
script-index reset likewise advance through the actual 0x2A4-byte VM
allocation. All original OnUpdate, OnDraw, cursor movement, text
rendering, audio and resource reload calls are preserved at their
original execution points, without caching, alternate arrays or
additional conditions.

Seven isolated and one combined pinned VC7.1 trial proved the entire
**1,727/1,727-byte interpreter owner and 88 original-bound relocation
fields** unchanged. The installed lexical macros passed scoped cold
replay for the two genuinely affected compiler carriers:
**10/10 exact source owners, 3,015 complete bytes and 159 relocations**.
The compiler renumbered 25 private `$L` references in the interpreter
and six in the `UpdateFade` function; each was rebound only after
independently proving identical same-section COFF targets and original
PE relocation offset/type/target, plus complete machine-code equality.
The former 1,499-byte logical interpreter and all short leaf functions
remain exact; no new exact owner or native runtime/product credit is
claimed.

## 2026-11-11 native SampleNetworkInput side and held-key records

The exact `SampleNetworkInput` owner (1,507 complete physical bytes)
selects one real `g_ReplayInputStates[3]` record by the incoming signed
`side`, with the **0x8E-byte original record stride** shared by
ReplayInput, Player and network consumers. Its three keyboard/DirectInput
controller-mode reads use the actual consecutive Supervisor configuration
bytes at config +0xB7/+0xB8, indexed by the same side word. The maintained
source now forms all four lvalues through unsigned i386 DWORD byte
addresses, rather than relying on C++ array bounds or a pointer outside
the single byte member. The repeat-key loop advances across the sixteen
original 16-bit `networkHeldFrames0A` counters by native `+sizeof(u16)`
address words. Each existing input poll, repeat threshold, key mask and
controller call remains in its original execution position; no bounds
check, cache, shadow state or extra call was introduced.

Four individual/combined pinned VC7.1 source probes matched the complete
**1,507/1,507 original bytes and all 13 bound relocation fields**. The
installed source passed the only actually affected compiler carrier's
scoped cold replay: **1/1 exact owner, 1,507 bytes, 13 fields**, with no
manifest label changes. This is source-memory fidelity in an already-exact
network input owner, not a newly exact function or native runtime claim.

## 2026-11-11 native GameManager gameplay side record addressing

The original `GameManagerSetupLayout` contains the actual three contiguous
**0x38-byte GameManager side records**, with two playable sides and a
third shared EffectManager side. `OnUpdate` walks both real 0xA0-byte
per-side score/phase buffers through the `sideIndex` selection and
`CleanupGameplayState` dispatches the same actual background, Player,
Etama, Effect, Enemy, CardAttack and FrontSide owners. The
`GameplaySetupThread` allocates and registers each playable subsystem
using the same original side pointer slots. Forty-eight independent
source lvalues previously indexed `globalManager.sides[sideIndex]` (26)
or `g_GameManager.sides[sideIndex]` (22); all now form native Win32
`uint32(actual base) + uint32(sideIndex)*sizeof(0x38-byte record)`
addresses rather than relying on C++ array-bound assumptions. Each
original score publication, allocation/call, failure branch, release
and side-scoped setup order remains unchanged, without caching a second
side object, inserting bounds checks, or changing the three-record ABI.

Isolated update/cleanup, setup-thread and combined pinned VC7.1 probes
matched the complete original physical owners, including **1,230 bytes
and 87 original relocations for OnUpdate**, **425 bytes / 43 fields for
CleanupGameplayState** and **1,689 bytes / 172 fields for
GameplaySetupThread**. The installed lexical native lvalues passed a
scoped cold replay of both actually affected compiler carriers:
**12/12 exact functions, 4,680 complete original bytes and 415 bound
relocation fields**; no private-label manifest rewrite was needed.
No new exact owner was promoted and Windows product/runtime closure
remains open.

## 2026-11-11 native WinMain executable directory byte indexing

The exact original `WinMain` starts with `GetModuleFileNameA` filling
`g_ExecutableDirectory[MAX_PATH]`, then scans backward for the last
path separator. The initial signed `pathIndex` is `strlen(...) - 1`
and the retail path performs its first character read before testing
whether that index is nonpositive. The maintained source now forms all
four live scan/read/write characters and the subsequent terminating
NUL address as genuine i386 `uint32(&g_ExecutableDirectory[0]) +
uint32(index)` byte words; it does not introduce a new range gate,
cache, path buffer, or branch. The fallback `./` string and existing
CreateWindow/DirectInput initialization call order are unchanged.

Three independent/combined pinned VC7.1 `/Ob0` probes matched both
original source owners in full. The installed source passed the only
affected compiler carrier's scoped cold replay: **2/2 exact functions,
1,636 complete original physical bytes and 171 relocation fields**,
comprising **1,351/1,351 WinMain bytes (152 fields)** plus
**285/285 CreateGameWindow bytes (19 fields)**. No compiler-private
label rebinding, newly exact owner, or native-runtime closure is implied.

## 2026-11-11 native Title character-order, unlock and Screen16 tables

The exact Title character-select family uses two distinct 16-byte
character-order arrays (`g_TitleCharacterOrder` for mode 0 and
`g_TitleCharacterOrderMode123` for modes 1/2/3), three real 16-byte
character-unlock tables, and the genuine 16-byte Screen16 order/entry
arrays. **Fifty-five independent runtime byte lvalues** now form the
original i386 address `uint32(real byte base) + uint32(live signed
character/cursor/index)`: 18 mode-0 order reads, 11 mode-123 order
reads, 17 unlock reads across normal/alternate modes, and 9 Screen16
order, selection and result reads/writes. The original per-use character
loads, unlock branches, cursor searches, ANM calls and menu transitions
remain live. No range gates, shadow arrays, copied character maps or
additional helper calls have been introduced.

The distinct mode-0, mode-123, unlock, Screen16 and combined variants
were compiled with the target's pinned VC7.1 settings. The final
combined source preserved the entire character select mode-0 **2,241
bytes / 116 relocations**, mode-123 **2,153 bytes / 89 relocations**,
Screen16 **897 bytes / 62 relocations**, and all other same-source
original owners. The actual two-carrier scoped cold replay passed
**41/41 exact units, 18,972 complete original physical bytes and 953
relocation fields**. Nine optimizer-private `$L` references in
`OnUpdateStartMenu` were rebound only after separately proving their
real same-section COFF definitions, unchanged offsets/types and
original PE destinations; no other manifest values were modified.
The still-nonexact sibling `TitleScreen::OnUpdateResult` was also
checked separately and retains its previous full 485-byte target,
484-byte candidate, 34 fields and 218 unlike overlapping bytes.
No exact-function promotion or native Windows runtime proof is claimed.

## 2026-11-11 native ANM ExecuteScript interpolation timer and mode records

The original exact `AnmManager::ExecuteScript` is a **9,776-byte complete
physical owner**, including its original ANM opcode dispatch and tables.
After executing the native script instruction stream, it advances seven
interpolation channels in the real `AnmVm` allocation. Its live per-channel
`interpCurrentTimers` and `interpEndTimers` are separate **0x0C-byte
`ZunTimer` records** at VM +0x50/+0xA4; `interpModes` are seven real
single-byte modes at +0xF8. The maintained source now forms all eight
runtime `i` accesses as original Win32 unsigned-DWORD base-plus-index
addresses (three current-timer, four end-timer, one easing-mode read).
No new interpolation buffer, index guard, timer snapshot, external
helper call, switch branch or mode calculation was introduced.

Four isolated/combined pinned VC7.1 trials preserved the **entire
9,776/9,776 original PE bytes and all 368 relocation fields**. The
installed lexical-native source passed its two genuinely affected compiler
carriers: **5/5 exact owners, 10,496 complete physical bytes and 421
original-bound relocations** (the large interpreter and four original
operand-accessor leaves). Eighteen compiler-private `$L` references in
the interpreter and one in each of the four leaf owners were rebound
*only* after comparing the complete original code and independently
proving the same-section COFF definitions, unchanged field offsets/kinds
and exact original PE jump destinations. No new exact owner, full cold
replay, native product or runtime credit is implied.

## 2026-11-11 native Etama bullet type templates and draw-bucket heads

The exact `EtamaController::SpawnBulletPatternPrimary` and
`SpawnBulletPatternSecondary` take the live signed bullet type from the
original `BulletSpawnDescriptor`, and now address the real
`bulletTypeSprites` allocation at native **0xD48-byte
`BulletTypeSprites`** strides instead of requiring a bounded C++
subscript. The complete 2,216-byte original `EtamaController::OnUpdate`
owner also uses the actual six-entry DWORD draw-bucket pointer array:
four independent head read/write lvalues are indexed by each bullet's
live `sprites.drawBucketIndex` byte. These become native Win32
`uint32(real base) + uint32(index) * sizeof(actual record)` addresses
without a new bounds gate, cached head, duplicate sprite allocation,
changed spawn loop or reordered drawing/ANM calls. The native bullet
template size and draw-head ownership are shared with the already exact
Etama draw and registration owners.

Separate primary/secondary template, bucket-head and combined pinned
VC7.1 probes retained complete original PE bytes in the major
1,932-byte `SpawnSingleBullet`, 2,216-byte `OnUpdate` and both
203-byte bullet-pattern functions. The installed combined source
passed **three genuinely affected compiler profiles, 18/18 exact
functions, 6,711 complete original physical bytes and all 259 bound
relocation fields**. Seven compiler-private `$L` references changed
spellings (one in SpawnSingleBullet and six in OnUpdate); each was
rebound only after confirming actual same-section COFF definitions,
unchanged field offsets/types, original PE targets and complete bytes.
No additional exact function, full cold replay or Windows runtime
closure is claimed.

## 2026-11-11 native score.dat record initialization and import lvalues

The original `ScoreFileView::LoadScoreRecords` producer uses the same
`g_TitleScoreTable[16][5][5]` actual allocation as the separately exact
Title result browser and DrawResult renderer. Each ranked score record is
**0x2C bytes**; one bank is **0x44C bytes** and one difficulty row is
**0xDC bytes**. Fourteen initialization-time lvalues and one live
score.dat input record selector now compute the original i386 byte
address `uint32(real table base) + uint32(bank)*0x44C +
uint32(difficulty)*0xDC + uint32(rank)*0x2C`. The importer continues
reading category/rank bytes directly from each wire score record and
publishes the whole original record at that address, without a new
bounds gate, alternate record table, copy, or changed chapter traversal.

Independent initialization/import and combined pinned VC7.1 source
trials matched the complete **441/441-byte LoadScoreRecords owner and
all 26 original-bound relocations**. The installed combined source
passed the full *affected* `ScoreData.cpp` scope, not a global replay:
**7/7 exact owners, 2,572 complete original physical bytes and
169 relocations across six genuine compiler carriers**. No compiler-
private label rebindings were necessary. This is native score-table
source semantics and writer/reader ownership agreement on previously
exact code, not newly exact function or Windows runtime credit.

## 2026-11-11 native Background stage camera viewport record selectors

The exact 2,652-byte `BackgroundRunStageScriptPhase` interprets
stage-script camera targets and interpolations for the live signed
`background->viewportIndex00C` selector. Nineteen separate writes/reads
of camera position, look-at, up, forward, and field of view now address
the original `g_Supervisor.ctorArray078[3]` storage as native i386
`uint32(real base) + uint32(index) * 0xF0`. Its actual
`SupervisorCtorArrayElement` is **0xF0 bytes**, established by the
independently exact Supervisor viewport and Front draw source and
original ABI layout assertions. These remain independent live source
lvalues inside the script opcode handlers and the camera interpolator;
no new view allocation, bounds gate, timer snapshot, call, or altered
camera event ordering was introduced.

Three pinned VC7.1 prototype source forms (native original record type,
native alias, and byte-pointer base) all matched the **complete
2,652-byte physical stage script owner and all 133 relocation fields**.
The installed lexical native-camera lvalue passed the one actually
affected compiler carrier's scoped cold replay, protecting **5/5 exact
source functions, 3,231 complete physical bytes and 160 relocations**,
including the 448-byte camera interpolation leaf and 91-byte Hermite
scalar leaf. Forty-four optimized `$L` references in the stage-script
boss and one in the interpolation leaf were rebound only after complete
code equality and independent same-section COFF definition / unchanged
original PE branch-target proof. No exactness count increase or whole
Windows native runtime claim is implied.

## 2026-11-11 native ASCII MenuState5 ANM VM slot addressing

The original exact `AsciiMenuState5::OnUpdate` is **1,236 complete
physical bytes** and owns its live `menuSprites` ANM VM allocation.
Nine dynamic `menuSprites[i]` accesses (initial script executions,
visibility interrupts and exit-time `SetInvisible` calls) now form
true Win32 unsigned-DWORD byte addresses from the real VM base and
`sizeof(AnmVm) == 0x2A4`. The original five-VM and three-VM menu
loops, cursor/color changes, state/timer branches and GameManager
mode transitions remain at their original call sites. No new range
gate, cache, VM allocation or additional helper call was introduced.

Three separately compiled native pointer/byte-pointer/inline forms
retained the complete original 1,236-byte owner and 86 relocation
fields. The installed lexical variant passed the sole source compiler
carrier's scoped cold replay: **1/1 exact owner, 1,236 complete bytes,
86 original-bound relocation fields**. Six VC7.1 private `$L` label
spellings were rebound only after verifying their same-section COFF
definitions, unchanged relocation offsets/types and unchanged original
PE targets. This strengthens existing exact source fidelity but does
not promote another owner or prove Windows runtime completeness.

## 2026-11-11 native opponent selection tables and candidate byte slots

The original exact `AsciiGameManagerView::SelectOpponentConfiguration`
selects opponent characters from live 0x10-byte `OpponentSelectionEntry`
records. Its 16-entry stack candidate pointer array, external list of
entry pointers, mutable GameManager `usedCharactersD8` byte row, and
16-byte global `g_TitleCharacterUnlocked` state all address the real
original allocations. The maintained source now computes **21 live
selection lvalues** through native unsigned-DWORD byte addresses:
five stack candidate-pointer reads/writes, one source list pointer,
four currently-used character bytes, six unlock bytes, two weighted
opponent parameter table words, two mode-value table words and one
randomly selected 0x38-byte GameManager side. No bounds gates,
character list clones, RNG refactoring or cached byte snapshots were
added. The original weighted selection, rejection, fallback, online
availability check and ordered game-mode decisions remain unchanged.

Seven isolated native-address families and their combined source were
compiled with the original pinned VC7.1 profile; a first combination
mistakenly rewrote an `extern ...[16]` declaration and was rejected
before compilation. Restricting all rewrites to the function body
preserved the complete original **799/799 physical bytes and 32
relocation fields** with no private label changes. The installed source
passed the sole affected compiler-carrier scoped cold replay:
**1/1 exact owner, 799 bytes, 32 fields**. This preserves existing exact
credit while improving native source/storage fidelity; original Windows
runtime and product closure remain open.

## 2026-11-11 native MIDI ProcessMsg channels and source-buffer slots

The original `MidiOutput` is a genuine 0x300-byte Win32 MIDI owner.
Its main `ProcessMsg` interpreter is **1,574 complete physical bytes**,
including 43 original-bound relocation fields. Its 16 `MidiChannel`
records have the exact **0x17-byte layout**; the long MIDI message
header queue and source `fileData` pool each hold 32 native DWORD
pointers. The real variable-length track array uses **0x20-byte
`MidiTrack` records**, shared by the parser, timer processing and
cleanup leaves. The maintained source now forms **34 independently
live address lvalues** with native `uint32(actual base) +
uint32(index)*sizeof(real record)`: dynamic channel/member receivers,
long-message header slots, file pointers, parsed track rows, timer
track traversal, fade-volume channel state, and the actual SysEx
payload byte at `uint32(lpData) + uint32(index) + 1`. Each original
MIDI event, loop meta-event, tempo change, DirectSound MIDI call,
buffer release and system-exclusive payload write remains at its
original position, without new range gates, caches or buffer owners.

Isolated/combined pinned VC7.1 probes showed the header/channel/SysEx
mainline retained **1,574/1,574 exact original bytes and 43 fields**;
separate file/track/header-cleanup/fade-leaf groups also retained their
full target owners. Replacing the nested key-pressed bitfield byte
lookup *did* create 22 unlike bytes, so that variant was rejected.
The installed combined source passed the single MIDI carrier's official
scoped cold replay: **32 exact units, 4,270 complete original physical
bytes and 132 relocation fields**. Twenty-three optimizer-private
`$L` names in ProcessMsg were rebound only after confirming actual
same-section COFF definitions, field offset/type identities, unchanged
original PE destinations, and complete byte equality. No function was
newly promoted to exact, and native product/runtime gates remain open.

## 2026-11-11 native PlayerShot Type2 trail collision sample address

The exact `PlayerShotUpdateCallbackType2` uses the original
`PlayerShotCtorView` allocation: its shot record is **0x484 bytes**,
with an inline array of genuine 0x0C-byte `Float3` trail samples at
+0x2B0. The collision-region loop chooses the active sample using the
live even index `i * 2`, and now forms its pointer as the native i386
`uint32(&trailVectors2B0[0]) + uint32(i * 2) * sizeof(Float3)` byte
address, without a bounded-array precondition or changing the original
collision query. This retains the exact timer, velocity, 31-entry
history shift and `CreateRectType4` call chronology.

Five isolated/combined pinned VC7.1 probes were compiled. Replacing
the **history-copy and predecessor** source expressions changed two
original relocation offsets and over 300 physical bytes, so those
variants were rejected. Replacing only the real even-index collision
sample preserved the complete **643/643-byte owner and all 16 original
relocations**. The installed source passed the sole actual compiler
carrier's scoped cold replay: **1/1 exact function, 643 complete bytes,
16 fields**, with no private-label rebindings. Existing exactness was
preserved; there is no newly exact function or Windows runtime claim.

## Complete comparison and compiler context

Compare physical contributions, including owned alignment and switch tables.
A Ghidra flow body can be shorter than the required PE/COFF owner. Examples:
RunEcl has 14,792 code bytes plus 772 table bytes; Player movement has 1,835
code bytes, one alignment byte and two eight-entry tables, totaling 1,900.
The main RunEcl dispatch follows a separate interpolation table: six words
at `0x40C088`, then 187 entries at `0x40C0A0`, indexed by opcode minus one.
Do not index all 193 words as one dispatch table.

Relocation names alone do not establish data identity. Check actual COFF
section/value definitions, initialized storage, payload, width and addend
against the original PE. The movement zero literal and Effect `etama.anm`
string once accepted counterfeit payloads under unchanged symbols; their
inspectors now reject that. Keep those literal checks.

VC7.1 `$L` labels can change after a header/source edit. Rebind only after
checking same-section definitions and owner-relative destinations against
original PE operands. Title and FrontMessage switch tables are examples.
Indexed global operands also need real addends: MusicRoom uses seven warning
pointers at +4 through +0x1C and four ReplayInput bytes at +0x122/+0x123.
See the corresponding units in `config/match-units.toml`.

Definition visibility changes codegen. Keep the genuine `.inl` compiler
carriers and selected profiles. An isolated source, changed helper visibility
or another exact leaf implementation can change a complete caller even when
the C++ behavior looks equivalent. Recheck affected exact consumers, using
[scripts/replay-scoped-exact.py](../scripts/replay-scoped-exact.py).

Local exact-producing profiles do not recover project-wide retail flags.
For example, ANM base construction uses `/Ob0` to preserve two vector-helper
calls and separately pushed timer constructor addresses. Chain callback
construction, deletion and EH paths need their own configured profiles.
Canonical build/compare parses PE/COFF directly without Capstone; some decoded
diagnostics require it. Do not change a compiler profile to chase residuals.

## Native layouts and corrected meanings

- `AnmLoaded` is 0x1C bytes, with resource fields at +0x04 through +0x18.
  The old eight-byte resident projection was superseded. `AnmVmBase`/`AnmVm`
  have 0x228/0x2A4 extents and share the genuine timer-containing base.
- ANM counters at +0x0C/+0x10/+0x14/+0x18 mean started scripts, executed
  scripts, render-state changes and flushes. Earlier swapped names were wrong.
  The complete 9,776-byte ExecuteScript and independent leaves support these
  names. The texture cache is at AnmManager +0x12880.
- The shared `Supervisor` is 0x7B0 bytes. Its startup, deletion, input and
  frame-queue consumers use the same native owner.
- GameManager has two playable 0x38-byte side records. Setup/cleanup also uses
  the following physical slot at +0x70, including a shared EffectManager pointer
  at +0x7C. That third slot is not a third playable side.
- Replay has three 16-bit stream-cursor pointers at +0x0C/+0x10/+0x14,
  followed by the FPS cursor at +0x18. The third replay stream is shared;
  only two sides are separately configured.
- Title uses the complete 0x1B388-byte `TitleScreenView`; draw/calc/MusicRoom
  and their native ANM/text consumers now share it. MusicRoom contains 32
  0x292-byte POD tracks. Existing exact functions stayed exact.
- Enemy constructs **194** 0x1C-byte trail vertices at +0x3E68, ending at
  +0x53A0. Rendering consumes 192 vertices for 96 samples. The supposed
  +0x5368..+0x53A0 opaque gap was wrong; the purpose of two extra elements
  remains unknown. See `scripts/inspect-enemy-ecl-constructors.py`.
- Player contains 512 real 0x44-byte collision records at +0x28BC and an
  embedded fallback at +0xB0BC, followed by pointer tables. Its physical
  initializer and region allocator share the real Reset.
- `0x40F860` is the 33-byte `AsciiGameManagerSideView::AddScore`, not handle
  release. Player owner-state Reset at `0x41D7E0` publishes its reward counter
  through AddScore, then clears counters/timer.
- The 309-byte `PlayerLifecycleView::InitializeShot` at `0x41F350` belongs
  to the real Player and uses native shot/descriptor pointers. Its seven
  exact shot callers retain that original call target.
- Front is 0x1293C bytes with a constructed 0x1D70-byte message runtime.
  The 24-byte signed `FrontFactoryConstructView::IsBlocked` at `0x404980`
  reads Front +0xE94C; Player, GameManager, Replay and Enemy timeline use
  that native interface. Front message Update/Setup share the constructed
  runtime rather than unrelated receiver classes.
- Front message random selection at 0x004162D0 indexes a variable-length
  eight-byte message-entry table whose C++ wire view declares only one entry.
  The maintained source now uses the actual entry base plus signed-index
  modulo-32 byte stride for all weight/pointer reads and final selection.
  GameManager side lookup also uses the original 0x38-byte side-record stride,
  while the character-range math and weight subtraction retain signed results
  of native DWORD operations. Pinned VC7.1 replay of the whole 693-byte owner
  and all 15 original relocation fields remains exact; no new bounds guards,
  resource layout or behavior were introduced.
- The CRT thunk at `0x48D950` sets ECX to `g_AsciiManager` at `0x4CE458`
  and jumps to the 227-byte constructor at `0x435DC0`. The 0xE0AC-byte owner
  uses genuine string, popup, PauseMenu and menu-state member constructors.

Complete constructor byte equality does not distinguish explicit authored
source from implicit compiler generation or folding. Enemy/ECL, Front/FrontSide,
Ascii and Bullet/Laser constructor equivalents retain unresolved origins where
the canonical ledger says unknown. Source equivalence adds no authored credit.
Likewise the 15-byte angle wrapper at `0x405700` is reproducible by both SDK
`atan2f` and a game wrapper; it remains origin-unresolved.

## 2026-11-10 Variable-length ECL operand native-word access

The TH09 ECL header ends at +0x0C, followed by four-byte tagged instruction
operands and opcode-defined byte/halfword views. The maintained VC7.1 view
uses `arguments[0]` only to describe that zero-size trailing field; indexing
that C++ member to reach any operand does not itself define a valid object
extent. `RawInt`, `RawFloat`, `RawByte`, and `RawShort` now compute native
modulo-32 addresses from the instruction base plus the attested prefix width,
then return their typed views. The byte/short helpers keep the same packed
wire-address interpretation as the integer/float helpers. No defensive bounds
check or branch has been added: the retail interpreter uses those raw offsets.

Seven independent single/combined VC7.1 source probes preserved the complete
**15,564-byte** RunEcl candidate, **598** original-bound references, all
owned tables and the same 30 ordinary differences in opcodes 4 and 155.
Enemy OnUpdate remains 3,900 bytes/97 fields/13 differences. Across the six
actual affected compiler carriers, the installed source cold-replayed **91
exact units**, **36,301 physical bytes**, and **1,783 relocation fields**.
The compiler changed **291 private `$L` names** in four genuine ECL operand
leaves; every rebind was checked against its actual same-section COFF symbol
value, addend and original PE destination before running the unchanged strict
replay. This advances native-source semantics without new exact owner credit,
or any original C++ spelling, product or runtime claim.

## 2026-11-10 ECL movement, jump and trail native wire consumers

The four recovered `RawInt`/`RawFloat`/byte/halfword accessors were not the
last consumers of `Th09EclRawInstructionHeaderView::arguments[0]`. The
`StartTimedPolarDisplacement` and `MoveRandomBiased` owner bodies still used
16 direct array-member reads, opcode 4's jump still copied element 0 from
that zero-size C++ array, and the trail handler cast the same trailing
member to a packed record. Their source now builds the i386 operand byte
address from the real `offsetof(arguments) == 0x0C` and reads a typed wire
union at that address. The original opcode-4 **union-by-value** copy is
retained; replacing it with `value.integer = RawInt(...)` incorrectly changes
12 more bytes in the adjacent opcode-5 path.

A source-only call through `RawInt`/`RawFloat` looked identical in the
15,564-byte `/Ob1` RunEcl but **regressed the real `/Ob0` helper carrier**:
VC7.1 did not inline those functions there, displacing the exact 288-byte
`StartTimedPolarDisplacement` and 516-byte `MoveRandomBiased`. The native
union-pointer view within each helper instead maintains their independent
original byte/call shape in both compiler profiles. Individual jump, trail,
movement and combined full-boss probes establish no new RunEcl exactness:
RunEcl still has 598 reviewed fields, its complete switch tables, and the
same 30 ordinary differences (7 opcode-4, 23 opcode-155); Enemy OnUpdate
still has 3,900 bytes, 97 fields and 13 ordinary differences.

The final edited sources were compiled in only six actually affected carrier
profiles. The strict targeted cold replay passed all **91** existing exact
functions over **36,301** physical bytes and **1,783** relocation fields.
The 20 renamed compiler-local `$L` entries (13 in `CompareOperands`, seven in
post-ECL movement) were rebound only after a separate complete original PE /
actual COFF comparison confirmed same-section definitions, unchanged label
positions, addends and original target operands. This source-only change
created no new exact-function credit; consult current canonical ledgers rather
than the count recorded during that experiment.

Separate source-cold register/lifetime explorations did not yield a safe boss
byte improvement: Player movement remains 1,900/1,900 with 122 different
bytes, and Title `OnUpdateResult` remains 484/485 with 218 overlapping
differences. Direct character/index spellings for Title lost another byte;
whole-switch direct `.sht` reads in Player movement rearranged many original
switch blocks. Keep those variants out of maintained source.

## 2026-11-10 CompareOperands native ECL conditional jump target

The independently exact `Th09EclRunControl::CompareOperands` returns the next
raw ECL instruction after a successful branch. Its displacement comes from
operand 3 as a signed 32-bit word, including backward-branch offsets. The
maintained source previously added that word to an `unsigned char*`, potentially
leaving its C++ object bounds. It now forms the same native i386 modular sum
from the 32-bit instruction address and displacement bits, without adding a
retail-absent bounds check or changing the comparison operators.

The complete **892/892 physical-byte** `CompareOperands` target (842 logical
bytes plus owned trailing table data) remains strict-exact. Both affected ECL
VC7.1 carriers cold-replayed **83/83 exact units, 29,856 physical bytes and
1,389 original relocation fields**; eight renamed same-section compiler
labels were independently checked against real COFF definitions and target
operands before updating manifest names. The 15,564-byte RunEcl boss remains
nonexact by 30 ordinary bytes, and Enemy OnUpdate by 13; no new exact
ledger credit or whole-product claim is made.

## 2026-11-10 native ECL laser and shot-packet argument addressing

After the main four ECL scalar readers and the movement/compare handlers were
recovered, two lexical consumers still formed C++ pointers from the header's
zero-length `arguments[0]`: `EclRunBullet.inl` read typed laser float/int and
color fields from that pseudo-array, and `EclPostRuntime.inl` interpreted the
same tail as a shot packet. Both now form the real packed ECL wire payload
address as `unsigned i386 instruction address + offsetof(arguments)`; the
laser float/int readers add their native byte offset as an unsigned 32-bit
word. The color reader preserves the signed two-byte wire field and its
existing scalar-resolution mask. The bullet and laser packet structures
retain their genuine field sizes and offsets; no callable replacement,
defensive branch, or new runtime behavior was introduced.

Pinned VC7.1 builds under both original `/Ob0` and `/Ob1` profiles confirm
that local typed pointer views preserve the **516/516** exact biased-movement
leaf as well as the whole **15,564-byte** RunEcl owner, its **598** address
fields and complete switch tables. The original 30 RunEcl differences in
opcodes 4 and 155, plus the Enemy OnUpdate 13-byte residual, remain unchanged.
The five actually affected compiler carriers cold-replayed **87/87** exact
functions over **30,583** physical bytes and **1,409** original-bound fields.
Twenty VC7.1 same-section `$L` names were rebound only after independently
checking their COFF owner-relative locations, addends and retail PE operands.
The canonical exact count is unchanged: this is faithful native source
semantics work, not a newly exact RunEcl boss or Windows product closure.

## 2026-11-11 Enemy timeline native remote slots and wire-step address

The independently exact `EnemyScheduleRuntimeView::Run @ 0x0040FCC0`
executes variable-length native Enemy timeline instructions and uses the
same actual nine remote-enemy pointer slots at
`EnemyManagerView::remoteSlots2AC388` as the large Enemy/ECL owners.
Opcode 8 writes to the selected remote enemy and opcode 10 tests the
same remotely selected enemy twice with distinct live table reads. Their
indices come from signed instruction operand words. The original i386
computes each slot address from the real manager table base and
`unsigned(index) * sizeof(Enemy *)` modulo 2^32, without a new bounds
check. The maintained source now uses one genuine inline lvalue helper,
retaining every original read, callback, branch and field write. It
also advances the authentic variable-length `instruction20` through a
native DWORD `original instruction pointer + live size06` byte address,
rather than potentially out-of-bounds C++ pointer arithmetic. No
instruction buffer, loop bound, RNG call or manager owner has changed.

Five independent/combined pinned VC7.1 source probes first showed the
same complete physical function extent and relocation count. Their
renumbered compiler-private `$L` references were then independently
proved against actual same-section COFF symbol definitions and original
PE destinations: **all five candidate forms matched the original 974-byte
code plus its 74-byte owned jump table (1,048 physical bytes / 42 target-
bound relocation fields)**, and the accompanying exact 20-byte
`ZunTimer::operator==(int)` helper remained completely exact. After
installing the combined source, the one genuinely affected native
compiler carrier passed canonical scoped cold replay of **2 exact
functions / 1,068 complete physical bytes / 42 relocation fields**.
The 19 private label spellings in the original-owner ledger were updated
only after this PE/COFF/full-body proof. No newly exact owner or
whole-product closure is claimed; the separate 3,900-byte Enemy
OnUpdate boss retains its original unresolved 13 register bytes.

## Enemy timeline opcode 13/14 native event-slot words

The existing exact EnemyScheduleRuntimeView::Run owner at 0x0040FCC0
has a 974-byte function body and 74-byte owned switch table. Its
opcodes 13 and 14 enumerate the real four DWORD event slots within
EnemyManagerView at +0x2AC430, testing for an instruction event,
clearing matched slots, or filling each available negative slot.
The maintained source now forms each word's original Win32 address
as a native byte pointer to the actual manager slot allocation plus
uint32(cursor)*sizeof(int); no separate event table, new bounds
guard, read caching, reordering, or loop break is introduced.

Pinned VC7.1 /O2 /Ob1 probes of direct DWORD/byte/char addressing and
the shared native helper reproduced the entire original
1,048-byte owner and **42 reviewed relocation fields**, including the
switch table. Compiler-private $L spellings can change when this TU
is rebuilt; those must be proved against same-section COFF definitions,
actual original PE operands and full bytes before rebinding. The
actual cold compiler carrier passed **2/2 exact owners: 1,068 full
original bytes and 42 relocation fields**. Nineteen proven private
$L symbol spellings were rebound without altering their offsets,
addends or original target destinations. The independently exact
20-byte ZunTimer::operator==(int) leaf remained unchanged. This is
a fidelity improvement to existing exact functions, not new credit.

## 2026-11-12 Etama single-record spawn position and sprite word arithmetic

The exact EtamaController::SpawnSingleBullet owner creates native game
projectiles from the real BulletSpawnDescriptor and template ANM VMs.
The original Float3-to-float* conversion callee still executes
at its original point, but the source now writes the genuine
bullet position z member through the returned owning Float3
subobject, rather than indexing beyond a float pointer to x.
The fan-pattern signed index1+1 expression likewise uses
i386 modulo-32 DWORD addition before its original signed
division and floating-angle calculation.

Six further signed sprite-selection word sums in this same
large owner now combine the original template active sprite ID
and live color or size-dependent sprite offset through native
unsigned DWORD addition, restoring the signed result for
the original SetSprite calls. No pattern clamp, RNG read,
sprite table, reordering or new callback is introduced.

The three actually affected VC7.1 BulletManager carrier
profiles replayed **18/18 existing exact functions, 6,711
complete original PE bytes and 259 relocation fields**,
including both major Etama update and individual-spawn paths.
Fourteen private compiler switch labels were independently
verified from current COFF same-section definitions, exact
original PE operands and full owner bytes before manifest-only
renaming. No newly exact owner or native product/runtime
closure is inferred.

## 2026-11-12 GameManager per-side difficulty and stage gate DWORD words

The full GameManagerSetupLayout::OnUpdate runs two real Player side
difficulty counters (state7C/state80 in each authentic Player
header), the GameManager's persistent signed valueB8 and
its timed stage threshold. The four side counters and valueB8
now increment through their actual unsigned 32-bit i386 word
lvalues, retaining their original signed less-than-16/six
gates and independently repeated receiver reads. The stage
threshold in game mode 1 forms the genuine unsigned-DWORD
sum phase10+1 *before* the original unsigned 500000 multiplier,
rather than allowing a C++ signed-overflow step on the live
mode/stage data. This preserves native wrap at INT_MAX without
changing the frame gate, side state, ANM/sound interactions,
score accumulation or transition call order.

Only two affected pinned VC7.1 GameManagerSetup compiler
carriers cold-replayed **12/12 existing exact functions,
4,680 complete original PE bytes and 415 relocation fields**,
including the entire 1,230-byte OnUpdate owner, 1,689-byte
GameplaySetupThread and 425-byte cleanup callback.
No private label changes, new exactness credit or native
Windows runtime closure is asserted.

## 2026-11-12 Title replay-menu native filename suffix byte reads

The complete TitleScreenView::DrawReplayMenu owner displays both
the 50-row replay listing and the runtime-selected replay entry.
Custom replay files carry a four-character user-name signature at
byte positions +15..+18 of the actual 0x200-byte replayPaths row,
not four independently owned C++ character arrays. The eight
original conditional byte reads now form the native Win32 address
of the real signed-index replay path plus each original fixed byte
offset using unsigned modulo-32 arithmetic. A lexical macro
retains every separate path read and original format argument,
so the original 13 text/SDK-related calls and 72 control-flow
blocks are unchanged. No filename guard, duplicate path
allocation or extra load is introduced.

The pinned original-PE/VC7.1 full nonexact owner guard was rerun:
**1,280/1,280 physical bytes, all 57 original relocation fields,
16 literal definitions/30 literal references, 13 ordered calls
and 72 direct CFG blocks** are preserved, along with the
prior **29 ordinary register-choice byte differences**. This
is still NONEXACT and earns zero new exact-function credit.
Nine additional selected replay/record-reference declaration
order and lifetime variants were tested against the complete
original owner; several shortened it by nine or sixteen bytes
and damaged hundreds of previously matching bytes, while the
ABI-preserving variants retained the same 29 differences.
All failed declaration variants were restored and did not
enter the maintained source.

## 2026-11-12 Player shot homing position and history-trail native storage

The independently exact 471-byte PlayerShotUpdateCallbackType1
homes a constructed 0x484-byte Shot against the genuine Player
tracking position. The source still executes the original
PlayerPositionView::operator float*() callee at the original
site, but interprets the returned address as the real
PlayerPositionView subobject and loads x/y by their named
members, not by indexing past a pointer to its first float.
The angle, speed, sqrt, tracking gates and all original
callee chronology remain unchanged.

PlayerShotDrawCallbackType1 and Type2 render separate native
0x0C-byte history/trajectory point records from the true
shot storage at +0x2B0. Both draw paths now advance their
live point cursors by modulo-32 i386 byte addresses plus
sizeof(the physical point), rather than C++ typed-pointer
increments that presume a bounded projected subarray.
The invalid-history sentinel, color/alpha restoration,
draw order and actual ANM VM remain unchanged.

The three independently affected pinned VC7.1 carriers
cold-replayed **3/3 already exact functions, 968 complete
original PE bytes and 30 relocations**: homing update
471 bytes / 19 fields, Draw Type1 204 bytes / six fields,
Draw Type2 293 bytes / five fields. No compiler-private
labels, new exact owner, or whole-Windows runtime claim.

## 2026-11-12 RunEcl boundary-aware movement reads genuine Enemy position

The full 15,564-byte EnemyEclManagerView::RunEcl dispatches the
boundary-aware movement opcode against the actual EnemyView
position2D74 Float3 in the +0x2D74 movement state. The original
Float3::operator float*() callee still executes at the same
source location, but the returned address is now treated as
the containing genuine 12-byte Float3 subobject and its
x/y members are read for all four original boundary-angle
comparisons. This avoids cross-member C++ float-pointer
indexing while preserving the original live receiver, RNG
calls, signed branch gates and floating-point computations.

The target-bound full-owner inspector continues to report
**15,564 physical RunEcl bytes, all 598 original relocation
fields, unchanged switch tables and exactly the same 30
ordinary nonexact bytes** (opcode 4: seven; opcode 155:
23). Two affected pinned VC7.1 EclManager carriers were
independently verified and replayed: **83/83 already-exact
sibling functions, 29,856 full original PE bytes and 1,389
bound relocation fields**. Twenty compiler-private $L
symbol spellings changed; each current COFF same-section
definition and unchanged original PE jump/call destination
was proven before rebinding only its manifest name.
RunEcl remains NONEXACT; no new exactness, game link
or runtime validation is claimed.

## 2026-11-12 Background renderer native stage instance and projected XYZ

The full 3,274-byte Background::RenderObjects boss traverses the
real file-loaded sequence of RawStageObjectInstance records, ending
at the original negative object-ID sentinel. Its live instance
cursor now moves by the genuine full native record size using
modulo-32 i386 byte addresses, rather than presuming a separate
C++ array object for each stage instance. No new instance-count
gate or cache is introduced, and the original culling/draw
semantics, camera matrix calculations and object pointers remain.

The sprite-projection path retains its independently original-bound
Float3::operator float*() conversion on the ANM VM's genuine
+0x208 position. The returned address is now interpreted as
the real 12-byte Float3 object and its x/y/z named members
are read into the world-matrix translation slots. This avoids
C++ cross-member pointer subscripting from a pointer to x,
without deleting the original call or changing any float
operations, matrix publication or draw order.

The single affected pinned VC7.1 BackgroundRender carrier
cold-replayed **1/1 exact complete RenderObjects owner,
3,274 original physical PE bytes and 113 relocation fields**.
No compiler-private labels or new exactness/runtime claims.

## 2026-11-12 FrontSide HUD stock and dynamic chain/ANM IDs are DWORDs

The complete FrontSide::OnUpdate displays the signed live stock count
by rendering five status ANM VMs. The original i386 subtracts two
from that signed count on every loop iteration, before the
subsequent negative/zero clamp. The maintained source now performs
this subtraction modulo 2^32 and restores the original signed result,
preserving retail behavior even at INT_MIN where C++ signed subtraction
would be undefined. The same native source restores modulo-32
integer additions to the dynamic side's +5 ANM slot, +23/+26
calc/draw chain priorities, the signed marker level + 53
and the external state value + 76 animation ID. No new side clamp,
script call, stock buffer or altered HUD branch is introduced.

The one genuinely affected pinned VC7.1 FrontSide compiler carrier
cold-replayed **15/15 existing exact owners, 4,087 complete original
PE bytes and all 176 relocations**, including the full 2,381-byte
OnUpdate and the corresponding create, callback and sprite helper leaves.
No compiler-private label changes or newly exact function/runtime
claim is involved.

## 2026-11-12 Enemy OnUpdate trail samples derive from actual Enemy owner

The still-nonexact 3,900-byte EnemyManagerView::OnUpdate boss
reads and shifts 0x1C-byte trailing position/velocity/angle
records starting at the genuine +0x33E8 offset of its live
0x5430-byte EnemyView. The source now derives that sample
cursor from the full real Enemy object pointer and its
independently verified member offset, rather than assuming
that each runtime/script-selected sample index belongs to
a separately constructed bounded C++ array of one sample.
The original sample widths, volatile Enemy pool state,
signed script-provided sample indices, 3D copy/read
sequence, damage and draw-group callbacks are unchanged.
No bounds gate, second sample array or extra function
call was introduced.

The complete original-bound Enemy OnUpdate diagnostic
continues to verify **3,900 target and candidate physical
bytes, all 97 relocation fields and 69 ordered calls**,
with precisely the same **13 unlike ordinary bytes** and
the original control graph/switch destinations. The two
actually affected pinned VC7.1 EclManager compiler
carriers separately cold-replayed **83/83 existing
exact sibling functions, 29,856 complete original PE
bytes and 1,389 relocations**. Two private switch
label spellings were independently proved from current
same-section COFF symbols and original PE destinations
before manifest-only rebinding. The Enemy OnUpdate boss
remains NONEXACT; no new exactness or runtime closure.

## 2026-11-12 Player shot spawning uses native SHT power and shot record strides

The original PlayerLifecycleView::SpawnShots consumes a variable-count
power-level descriptor list from the file-backed PlayerShtFileView
at +0x42C. Each PlayerShtPowerLevelView occupies a true eight-byte
native record, though the C++ wire projection exposes only its
first element. The live signed powerLevel now selects that
real record using unsigned i386 base + index*eight addressing.
The original 128-slot Player shot pool consists of 0x484-byte
PlayerShotCtorView records. The scan loop now increments both
its signed slot index and physical shot pointer with native
DWORD arithmetic. The file-loaded SHT descriptor cursor
likewise advances by its genuine 0x38-byte physical record
stride, not a typed pointer over a separate C++ array.

The original shot state/availability check, fireInterval
sentinel, callback dispatch, VM flag mutation and jump back
to processEntry remain unchanged; there are no new bounds
tests, RNG calls or caching of the live descriptors.
The one pinned VC7.1 PlayerSpawnShots carrier cold-replayed
**1/1 exact complete 190-byte original owner and its sole
relocation field**. The source strengthens the tie between
Player spawn mainline and the existing 0x38-byte SHT callback
leaves, without new exactness or native product/runtime closure.

## 2026-11-12 Enemy spawn pool full-record native address and index

The genuine EnemyManagerView::SpawnEnemy routine finds an unused
slot in the original 128-element native Enemy pool at +0x5758,
each EnemyView exactly 0x5430 bytes. Its source now advances
the live pool pointer by native i386 DWORD base+sizeof(EnemyView)
address arithmetic and advances the signed slot index modulo
2^32 before the existing 128-slot comparison, rather than
assuming a separate C++ array allocation for the pointer
and signed defined-overflow semantics. This aligns the
regular SpawnEnemy path with the independently recovered
card-attack SpawnEnemyRecord pool traversal. The original
ECL RunEcl selection, initial variable memcpy, Enemy
template, position/life/score publications and failure
sentinel/return paths remain unchanged.

The single affected pinned VC7.1 EnemyManagerSpawnEnemy
carrier cold-replayed **1/1 exact complete 373-byte
original PE owner, all four relocation fields**.
This improves a medium spawn function and its relation
to the larger Enemy/ECL mainline without introducing a
new exact owner or native game/runtime claim.

## 2026-11-12 FrontMessage random-dialogue native text and shadow color rows

FrontMessageRuntimeView::SetupRandomForSide selects a file-backed
dialogue through the original RNG and weighted byte-stream table
and initializes two live 0x2A4-byte ANM text VMs. Both
DrawTextLeft calls independently fetch their text and shadow
colors using the live colorIndex1D64 byte. The four original
reads now resolve actual i386 DWORD addresses from the
respective four-element textColors1D20 and shadowColors1D30
allocations with a native scaled unsigned index, avoiding an
independently bounded C++ array-subscript contract on the
runtime-selected color. The original RNG calls, weight
subtraction, selected message record, VM setup and text
draw order remain unchanged; no new color clamp or
extra draw/cached storage was introduced.

The single affected pinned VC7.1 FrontMessageRandomSetup
carrier cold-replayed **1/1 exact complete 693-byte owner,
15 original-bound relocation fields**. No compiler-private
labels, newly exact function, or Windows product/runtime
closure is implied.

## 2026-11-13 WinMain and both critical-section leaves use the full native Supervisor

The exact **1,351-byte WinMain** and independently exact **285-byte
CreateGameWindow** now use the same actual 0x7B0-byte process-global
`Supervisor` allocation at **0x004B3100** as the full D3D, gameplay,
network and scheduler functions. The previous synthetic `MainSupervisorView`
with its partial field layout is gone. The real Supervisor now names the
original instance handle at +0x00, 0x34-byte D3D present parameters at
+0x350, GameConfiguration at +0x388, window transition at +0x5A4, flags at
+0x5D4, and the five actual **0x18-byte Win32 CRITICAL_SECTION records**
starting at +0x6C0. The final eight bytes through +0x740 remain in the
original undisturbed storage overlay. Public layout assertions prove all
positions and preserve `sizeof(Supervisor)==0x7B0`.

The separate exact **33-byte `Supervisor::InitializeCriticalSections`**
and **33-byte `Supervisor::DeleteCriticalSections`** now operate directly
on that same genuine object, not a second short
`SupervisorCriticalSectionArrayView`. WinMain calls their canonical method
identities; the two independent exact `Supervisor::IsWindowed` and
`Supervisor::LoadConfig` identities are reused with their original call
chronology, unchanged 32-bit field views and no additional helpers.
All 27 changed COFF relocation symbol identities were checked against
actual original target PE operand words, offsets, kinds and addends before
updating names. The original two critical-section mapping names were
synchronized across source, function, implemented, reccmp and exact ledgers;
physical ownership, exact counts, target addresses and original callee
symbols were not invented or redirected.

The **two source-affected pinned VC7.1 carriers** replayed **4/4 existing
exact owners, 1,702 complete PE bytes and 173 relocations**. Additional
consumer-focused D3D initializer and GameManager builds reverified **the
source-header-dependent exact owners and their bound relocations** without
a project-wide cold replay. All new declarations are nonvirtual; offsets
and the full Supervisor allocation size remain unchanged. This is source
ownership and object layout recovery, not new exact-function or a complete
Windows product/runtime claim.

## 2026-11-13 Complete Chain release and WinMain share the actual 0x40-byte Chain

The exact original **241-byte `Chain::ReleaseSingleChain`** and **34-byte
`Chain::Release`** now belong to the same genuine 0x40-byte `Chain` object
that owns the two actual 0x20-byte calc/draw list heads and the independently
exact Cut callback. The previous `ChainReleaseView` projection with a cast
back to `Chain` and WinMain's third independent `MainChainReleaseView`
allocation alias are gone. ReleaseSingleChain naturally calls `this->Cut`
on the original live snapshot cursor; Release passes `&calcChain` and
`&drawChain` directly. WinMain calls the exact same process-global
`g_Chain` at **0x004ACFC8**, with no second destructor, chain copy or
cleanup reorder. The two original function identities were renamed in
`functions.csv`, `reccmp-functions.csv`, `implemented.csv`, `matches.csv`
and the pinned COFF unit symbols; authored status and exact byte coverage
were unchanged.

The target's C++ exception machinery was **not** treated as incidental
padding: the real separately emitted **18-byte EH code at 0x0048D360**
(three relocations) and **36-byte EH metadata at 0x0049DC2C** (two
relocations) were reconstructed from the current VC7.1 COFF sections and
compared against every original disk PE byte and relocated operand.
The actual 10-byte `__ehhandler` subentry at +8 retains its original
0x0048D368 address; the unwind info points to the same original targets.
Only after these independent EH proofs were five call/handler/global
relocation symbol spellings rebound, with all kinds, offsets, original PE
addresses and COFF addends unchanged.

The **three source-affected pinned VC7.1 compiler carriers** passed
**4/4 existing exact owners, 1,911 full original PE function bytes and
193 relocation fields**, including WinMain's 1,351 bytes and 285-byte
CreateGameWindow helper. The additional EH extents are distinct evidence,
not duplicate canonical function credit. The shared header gains only
nonvirtual member declarations; physical layout and unrelated callers do
not change. No new exact-function or whole Windows runtime is implied.

## 2026-11-13 WinMain MIDI shutdown and WindowProc header use one physical output pointer

Two separately original-bound modules observe one genuine
`MidiOutput*` global at **0x004B36BC**. The exact **1,351-byte WinMain**
checks it independently before StopPlayback, delete and null publication,
while the exact **249-byte GameWindowProc** reads the same pointer when
handling the native 969/MM_MOM_DONE MIDI message and calls the original
`MidiOutput::UnprepareHeader`. Both previously declared different
provisional external variables (`g_MainMidiOutput` and
`g_WindowMidiOutput`) for this *single allocation*. They now use the
same source-facing `g_MidiOutput` declaration, with unchanged signed
message dispatch, queue teardown, null branches, deleted-object ordering,
and real `MidiOutput` class. This neutral maintained name is not claimed
to be an independently discovered original identifier; the separately
recovered Title PlayFile convenience caller still has its own provisional
source-facing external view and is not silently retyped.

Four global-pointer relocation symbols were rebound only after checking
both target-original PE addresses, complete VC7.1 COFF function bytes,
all original fields, kinds, offsets and addends, and the independent
Title consumer's same +0x4B36BC global address. The affected two pinned
VC7.1 carriers passed **3/3 previously exact owners, 1,885 complete PE
bytes and 187 relocation fields**, including 285-byte CreateGameWindow.
The independent 83-byte exact StopPlayback implementation at 0x0043DE30
remains unchanged. No new exact-function or Windows runtime credit follows.

## 2026-11-13 WinMain's real 0x2B2934-byte AnmManager allocation and methods

The complete **1,351-byte original-bound WinMain** allocated and published
the same process-global manager at **0x004DC550** that all 100+ original-bound
ANM, player, enemy, title, rendering and resource callbacks use. Its former
local `MainAnmManager` contained an unrelated opaque 0x2B2934-byte storage
array with separate constructor, ReleaseSurfaces, destructor and global
symbol names. WinMain now instantiates the *real* `AnmManager` class with
its original full lifecycle layout enabled: `new AnmManager`,
`sizeof(AnmManager)==0x2B2934`, native `g_AnmManager`, original-bound
`AnmManager::AnmManager` (target 0x0043CE30), exact
`AnmManager::ReleaseSurfaces` (target 0x0042D2F0), and the original
cleanup/delete sequence. The class exposes a nonvirtual destructor
*declaration* without changing field offsets, size, or virtual dispatch.

The last WinMain reference targets a **33-byte compiler-emitted deleting
destructor at 0x0042CC30**. Its full original PE bytes and both call
instructions were checked: one calls the folded, source-unattributed
one-byte `ret` at **0x0043D2B0**, then the other invokes the exact
operator-delete body. This is a verified WinMain *call destination*, not a
claim to have reconstructed an independently owned `AnmManager` destructor
body or unique authorship of the folded ret. The destructor's out-of-line
implementation / final whole-Windows linkage remains unresolved.

All seven global/constructor/release/deleting-destructor symbol spelling
changes were accepted only after target-hash and original WinMain full
byte, field, addend and independently identified address proof. The
**three affected pinned VC7.1 WinMain and ANM-loader carriers** reproduced
**19/19 pre-existing exact functions, 4,894 full original PE bytes and 304
relocation fields**, including WinMain and complete authentic ANM resource
constructor/loader leaves. The source no longer invents a second giant
ANM owner. No exact owner added; no whole product/runtime closure claimed.

## 2026-11-13 WinMain shares real SoundPlayer owner and all three audio callbacks

The exact **1,351-byte original-bound `WinMain`** no longer declares a
second miniature `MainSoundPlayer` source class for the true 0x6224-byte
SoundPlayer global at **0x004DC698**. Startup now calls
`g_SoundPlayer.InitializeDSound` (original 0x0043E500), the main release
loop calls `g_SoundPlayer.ProcessQueues` (0x0043F1F0), and final cleanup
calls `g_SoundPlayer.Release` (0x0043EEB0). Each callee is already separately
original-bound and exact. Their chronological order, signed loop control,
live window handle, original three distinct global reads and all source
arguments remain unchanged. No shadow audio state, helper call, queue
rebuild or different SoundPlayer object lifetime is introduced.

Six provisional symbol identities were replaced by these independently
proven canonical SoundPlayer names in WinMain's relocation manifest. Every
existing target PE field, actual current VC7.1 COFF type, offset, addend
and complete physical function body was checked before rebinding names;
not one address or binary operand changed. The one affected pinned VC7.1
carrier passed **2/2 preexisting exact functions, 1,636 complete PE bytes
and 171 relocations**, including the separately exact 285-byte
CreateGameWindow leaf. No new exact-function or Windows build/runtime
credit follows; the improvement is removal of duplicate native audio
ownership models in main process startup and teardown.

## 2026-11-13 The real GameWindow zero-initializing constructor

The exact **18-byte constructor** at original TH09 Japanese v1.50a
`0x0042D290` now belongs to `GameWindow::GameWindow()` in the same
0x44-byte class used by original-bound WinMain, D3D startup, Present,
Render, and GetTimestamp. The former separate `GameWindowView` had one
fake 0x44-byte opaque array and no shared class identity; the actual
constructor zeroes **the real 0x44-byte instance** in place using the
same natural VC7.1 `memset` source and unchanged exact bytes. The original
startup initializer at `0x0048D8E0` passes process-global `g_GameWindow`
at `0x004B30B0`. The canonical source function, source mapping, exact match,
unit and implemented rows were renamed together; no target address, source
extent, status or exactness count was changed.

Since `GameWindow.hpp` is a shared native owner, all **nine impacted pinned
VC7.1 carriers** were recompiled (one explicit carrier beyond the scope
runner's default eight-job guard). All **10/10 original-bound exact owners,
5,173 complete original PE bytes and 502 relocations** passed, including
1,561-byte InitializeD3DRendering, 1,351-byte WinMain, the constructor,
CreateGameWindow, window startup, Render/Present and timestamp.
No new codegen/ABI feature, separate allocation or full Windows product
closure is implied.

## 2026-11-13 Actual GameWindow closing DWORD across startup, D3D and main loop

The actual `GameWindow` is a **0x44-byte process-global allocation**.
Original `InitializeD3DRendering` clears its +0x04 closing DWORD, and the
exact WinMain loop polls the same word twice before and after message
processing. The native `GameWindow` type now identifies that real
`windowIsClosing04` signed DWORD in the original 0x0C-byte prefix (with
the byte layout preserved as an overlapping union), instead of projecting
two independent synthetic `GameWindowD3DInitView` and `unsigned char[12]`
receivers. The D3D initialization and main startup use
`g_GameWindow.windowIsClosing04` directly, while all real adjacent fields
(window handle, frame skip, x87 timestamps and startup path byte) retain
their separately proven offsets and load/write chronology. The original
receiver lifetime, D3D setup, screen effects, main event loop and Release
calls are unchanged. No copied window object, extra range guard or method
boundary is introduced.

The eight source-affected pinned VC7.1 carriers passed full scoped replay
of **9/9 pre-existing exact owners, 5,155 full original PE bytes and 502
relocations**, including 1,561-byte `InitializeD3DRendering`, 1,351-byte
WinMain, 285-byte CreateGameWindow, game window Render/Present/Timestamp and
the independent 3D capability wrappers. No new exact-function or whole
Windows product/runtime credit is implied.

## 2026-11-13 WinMain uses the authentic constructed GameWindow allocation

The exact **1,351-byte original-bound `WinMain`** and independent **285-byte
`CreateGameWindow`** previously used a second `MainGameWindowView` shadow
struct and a provisional `g_MainGameWindow` global name for the real
0x44-byte process-global GameWindow at **0x004B30B0**. Both now consume the
*actual existing* `GameWindow` native type and `g_GameWindow` allocation,
shared with the separately exact Render, Present, timestamp and D3D
startup owners. The 0x04 closing DWORD is read through the genuine
`GameWindow::unknown04` prefix rather than inventing an independently
allocated member; native asserts pin the object size and +0x04/+0x14/+0x3C
Windows fields. All live window handle, frame skip, timing, cooperative
D3D loop, restart and release accesses retain their original chronology.
The main render call now uses the already exact
`GameWindow::Render` at **0x0042DC40**, not a duplicate fake caller symbol.

Fifteen manifest relocation spellings (14 in WinMain, one in
CreateGameWindow) were rebound from the provisional view names to these
*existing independently proven* GameWindow identities. Every actual
current VC7.1 COFF relocation kind, offset and addend was checked against
the **same original PE target address/operand word**; all complete owner
bytes matched before manifest changes. The one affected pinned VC7.1
carrier replayed **2/2 preexisting exact functions, 1,636 complete PE
bytes and all 171 relocations**. No executable bytes, linker identities,
original target or exact counts were modified. This is substantial
native source-class ownership recovery, not a new exact-function or
complete Windows product/runtime claim.

## 2026-11-12 WinMain startup system metrics and restart countdown DWORDs

The original WinMain startup calls CreateGameWindow to construct
the actual fullscreen/windowed TH09 Win32 window. Windowed
dimensions are the signed GetSystemMetrics dialog-frame width/height
times two plus the original 640/480/caption components.
These native i386 DWORD multiply/add operations are now
expressed through unsigned intermediate bits and their final
signed original window dimensions, rather than potentially
overflowing signed C++ arithmetic on unusual OS metric values.

The original WinMain executable-directory backslash search's
signed pathIndex and the options-restart 60-frame waitFrames
counter now decrement their actual DWORD lvalues with native
wrap, retaining the original path fallback, WM dispatch
loop, resumption and Direct3D/replay state chronologies.
The sole affected pinned VC7.1 WinMain carrier cold-replayed
**2/2 existing exact functions, 1,636 complete original
PE bytes and all 171 relocations**, covering the 1,351-byte
WinMain and 285-byte CreateGameWindow physical owners.
No compiler-private label change, exactness promotion,
whole-game link or runtime closure is claimed.

## 2026-11-12 Player hitbox damage accumulation and shot-pool DWORDs

The complete PlayerLifecycleView::CalcDamageToEnemy boss scans
the actual 128-by-0x484-byte PlayerShot array before it
accumulates damage from the original live Player collision
region-pointer slots. The shot scan now advances the real
shot record address and signed loop index with unsigned
i386 DWORD operations, preserving all original hitbox,
type, timer, state and callback gates. Shot damage and
collision-region value additions, the region's signed
hitCount2C accumulator, secondary damage accumulator and
the cap-minus-current-hit correction now explicitly preserve
native 32-bit wrap before restoring their original
signed result. The hit animationIndex+6 also uses native
DWORD addition. The active region slot cursor advances
through the genuine contiguous four-byte pointer entries
rather than presuming an unrelated constructed C++ array.

The original signed/float rectangle and circular collision
tests, region type dispatch, primary/secondary return values,
animation callbacks and effect/sound calls all retain their
original order. The single affected pinned VC7.1 PlayerDamage
carrier cold-replayed **2/2 existing exact functions,
1,051 complete original PE bytes and 21 relocation fields**,
including the complete 996-byte CalcDamageToEnemy owner
and its 55-byte PlayerBuildAabb leaf. No extra bounds
check, new exact owner or product/runtime closure is claimed.

## 2026-11-12 Enemy defeat/reward native effect and region ID words

The complete 1,142-byte EnemyView::HandleDeathRewards receives a
signed script-controlled deathEffectVariant3368 from the live
EnemyView, then publishes the corresponding +20 effect ID or
+8 Player CreateCircleType2 region ID in two distinct branches.
Those three original additions now form unsigned modulo-32 i386
DWORD results before restoring the signed ID for the unchanged
native SpawnEffect/region calls, rather than relying on
C++ signed-overflow behavior for possibly negative or high
original script values.

The original return gates, enemy token and death-sound choice,
reward-tier interval and score arithmetic, RNG sequence,
other-side switch, pop-up positioning and all recipient
calls are unchanged. The single affected pinned VC7.1
EnemyManagerUpdate carrier cold-replayed **1/1 exact
complete 1,142-byte owner and all 39 original PE
relocation fields**. This is native source fidelity in
the large reward callback called by the still-nonexact
EnemyManager::OnUpdate, not new exact-function credit
or whole-game runtime completion.

## 2026-11-12 Bullet transform program next-instruction and Sprite ID DWORDs

The complete 1,320-byte Bullet::AdvanceTransformProgram interprets
an 18-entry native 0x18-byte transform record sequence inside
the genuine 0x10C4-byte Bullet object. Runtime transformIndex
is a signed i386 DWORD; all five continuation increments now
advance its actual storage with unsigned word wrap. The
SPAWN_CHILD_PATTERN opcode reads a second real contiguous
transform instruction through the native base +0x18 address,
and increments the current index by a native 32-bit word.
For SET_SPRITE, the live signed type ID selects one of 32
actual BulletTypeSprites records in the real controller
allocation using scaled native unsigned byte addressing,
and the script-selected Sprite ID is the modulo-32 sum
of the active sprite ID and signed opcode color offset.

The original C++ transform priority, sound effects, scheduler
gates, secondary pattern descriptor, timer calls, polar
movement and switch-table order remain unchanged. The sole
affected pinned VC7.1 BulletAdvanceTransformProgram
carrier cold-replayed **1/1 exact complete original
1,320-byte owner and all 26 relocation fields**.
Two compiler-private labels were independently proved
against same-section COFF definitions and original PE
operands before manifest-only name rebinding. This is
native source fidelity for the large bullet interpreter,
not new exact-function or whole-product runtime proof.

## 2026-11-12 Enemy draw-group and trail vertex native record traversal

The original complete 1,758-byte EnemyManagerDrawImpl draws four
authentic EnemyManager draw-group linked lists, primary/secondary
0x2A4-byte ANM VMs and the real EnemyView's 194-element
0x1C-byte trail vertex storage. Its live signed draw-group
index now advances as a native i386 DWORD and selects the actual
four pointer heads from the true EnemyManager allocation via
unsigned word-scale addresses. Both secondary-VM loops and the
two per-trail-sample output vertex advances now use genuine
native 0x2A4-byte and 0x1C-byte record strides instead of
C++ pointer arithmetic over one projected subobject.
The signed vertexCount+1 calculation likewise forms a
native unsigned DWORD sum before the same signed
division and UV-spacing calculation.

The original linked Enemy traversal, wrapped-angle math,
sprite properties, taper/alpha logic, D3D draw calls and
high/low-priority viewport callbacks remain unchanged.
The one affected pinned VC7.1 EnemyManagerDraw carrier
cold-replayed **4/4 existing exact owners, 1,922 complete
original PE bytes and 61 relocation fields**, including
the full 1,758-byte draw boss and its three attached
helper/callback owners. No private-label rebinding,
new exactness promotion or game-runtime claim is made.

## 2026-11-12 Background scripted quad VM update native record addresses

Background::UpdateStageObjectVms traverses the actual file-backed
stageObjectCount pointer table, separately allocated runtime
0x2A4-byte AnmVm records and variable-width RawStageQuadBasic
instructions from the original .std stage file. The signed
object index and active script counter now increment as
native i386 DWORD words, while the real object-pointer and
ANM VM indexes form modulo-32 base + index*record-size addresses
from their distinct genuine allocations. Each stage quad's
signed 16-bit byteSize is sign-extended into its original
32-bit byte stride, without treating the 0x1C-byte quad
header as an independently allocated array object.

The original ExecuteScript calls for types zero and one,
currentInstruction220 liveness check, object flags,
variable-length terminator and original call order
remain unchanged, with no new file bounds checks or VM cache.
The sole affected pinned VC7.1 BackgroundUpdateStageObjectVms
carrier cold-replayed **1/1 exact complete 190-byte
original PE owner and both relocation fields**. This
joins the matching 3,274-byte BackgroundRender boss and
2,652-byte stage-script interpreter's native allocation model,
not a new exact-function/runtime claim.

## 2026-11-12 Laser spawn pool and sprite IDs native DWORD words

The genuine EtamaController::SpawnLaserPattern fills the first unused
of 48 native 0x59C-byte Laser records. The live signed pool index and
laser pointer now advance as original modulo-32 i386 word operations,
without a separate C++ array bound on the pointer. The actual body
ANM script ID adds the signed descriptor bulletType+10, the body
activeSpriteIndex adds the live color, and the laser start-cap
uses the genuine color-offset table value plus 146. All three
script/sprite sums now form unsigned 32-bit wrap results before
restoring the original signed ID passed to the ANM calls.
The original suppression gate, pool sentinel, AngleToPoint
receiver, position/velocity copies and laser timings remain intact.

The single affected pinned VC7.1 BulletLaserSpawn compiler
carrier cold-replayed **1/1 exact complete 450-byte owner
and all six original-bound relocation fields**. This provides
native source-level fidelity for the laser counterpart of
the already exact Etama bullet spawning/update mainline,
not a newly exact function or whole-game runtime claim.

## 2026-11-12 Network HandleMessage peer slots and prediction frame words

The original complete SupervisorNetworkMessageView::HandleMessage
dispatches DirectPlay receive, join/host handshake and peer
creation/deletion callbacks. The real peerIds table contains two
native four-byte DPNID entries. Both the local-side index and
one-minus-the-live-local-side index now resolve their original
i386 unsigned base + uint32(side)*four-byte slot addresses
without a C++ array-bound assumption. The handshake's
one-minus-packet-side calculation and the two predicted
frame IDs (loopIndex-9998) likewise preserve the original
signed results of native wrapping DWORD subtraction.
The actual DirectPlay identities, received frame buffers,
original InsertReceivedFrame/RNG calls and packet state
branches remain unchanged. No new network validation,
packet boundary guard, array or synthesized callback is added.

The single affected pinned VC7.1 SupervisorNetworkMessageHandler
carrier cold-replayed **1/1 exact complete 808-byte original PE
owner, all 46 target-bound relocation fields**, including
the complete original jump-table tail. Six compiler-private
jump/switch labels were proved from actual same-section COFF
definitions, unchanged original PE destinations and full
owner bytes before rebinding only their manifest spellings.
This is an already-exact source owner and does not establish
the separate nonexact SupervisorServiceUpdate or game runtime.

## 2026-11-12 MIDI ProcessMsg loop checkpoint and reset track cursors

The original 1,574-byte MidiOutput::ProcessMsg dispatcher handles
the two repurposed MIDI loop markers by saving or restoring the
live cursor and next-event tick of every current MidiTrack. Those
tracks are real separately allocated 0x20-byte records, as
independently supported by LoadTracks and the original MIDI
device playback. Both the loop-checkpoint and loop-reset
record cursors now compute native unsigned i386 DWORD
base + sizeof(MidiTrack) addresses rather than doing
typed-pointer arithmetic with an implicit C++ array-bound
assumption on a file-controlled track count.

The original running-status, tempo snapshot, stream position,
event timing, track count and two controller switch branches
are unchanged. The one affected pinned VC7.1 MIDI carrier
cold-replayed **32/32 already-exact functions, 4,270 complete
original PE bytes and all 132 relocation fields**, including
the complete 1,574-byte ProcessMsg boss. Independent full-COFF
comparison also verified the adjacent one-byte virtual timer
owner: **33/33 physical owners, 4,271 bytes and 132 fields**.
Eight compiler-private switch-label spellings were proven
against actual same-section COFF definitions, unchanged
retail PE destinations and complete bytes before manifest-only
rebinding. No new exactness or native-runtime claim is made.

## 2026-11-12 Replay input held-frame and FPS stream native cursors

ReplayInputState::Update reads sixteen consecutive native u16
held-frame counters inside the real 0x8E-byte input-state
allocation. The live held cursor now advances by its genuine
two-byte word stride using a modulo-32 i386 address, preserving
all original repeat/press/release bit manipulations and
held-duration resets. PlaybackInputAndFps similarly traverses
the two GameConfiguration focus-mode bytes with native
unsigned one-byte addresses, and its two independently live
FPS lookahead reads now form the real fpsCursor+1 address
instead of relying on C++ array-subscript provenance.
The original FPS high-bit split, signed counter, input
history update and three playback stream advances are unchanged.

Only the two actually affected pinned VC7.1 ReplayInput
compiler profiles were cold-compiled; exact source-scoped
replay passed **12/12 already-exact owners, 4,093 complete
original PE bytes and all 232 bound relocation fields**.
Independent full nonexact guards also confirm SaveReplay
remains 1,238/1,238 bytes and 53 fields with its original
two register differences, while BeginPlaybackStage remains
538/540 bytes and 32 fields. No new exactness, product
link or runtime validation is claimed.

## 2026-11-12 Player ANM preload side-record and slot DWORD traversal

The original PreloadPlayerAnmResources leaf walks two real
0x38-byte GameManager side records, testing each side's live
shot type, previous shot type and resource mode before loading
the corresponding ANM file into slots five and six. The
source now advances its true side-state pointer through an
unsigned i386 DWORD address plus the full native record size,
and increments the live signed ANM slot as a native DWORD.
The already native six-pointer-per-shot-type resource path
lookup, GetAnmRawData calls, preload early return, distinct
manager reload and two-side limit remain intact.

The single affected pinned VC7.1 PlayerResourcePreload
carrier cold-replayed **1/1 already-exact owner, 114
complete physical original PE bytes and six relocation
fields**. No new side/shot bounds check, private label
change, new exact owner or product/runtime closure is
implied. This extends the same genuine GameManager
side-record fidelity used by ANM and Player owners.

## 2026-11-12 FrontMessage script ID and decoded text byte cursors

The 2,968-byte FrontMessageRuntimeView::Update story interpreter
selects an animation for script opcode eight from the genuine
side-1 Player's signed tableValue303F0 plus four. That addition
now produces the original modulo-32 i386 DWORD result before
calling the same ANM SetAndExecuteScriptIdx, without a new
script gate or cached Player value.

The 39-byte DecodeFrontMessageString helper processes a
file-backed XOR-obfuscated text stream into the real 64-byte
local destination. Both source and destination byte pointers
now advance using explicit unsigned native Win32 addresses
after the original input/output byte transfer. The decoder's
byte-sized key and step arithmetic, decoded-NUL test and
retail absence of an extra text-length guard remain intact.
The source still makes the same exact original byte reads
and writes in order.

Only the two affected pinned VC7.1 FrontMessageUpdate
carriers were cold-compiled, and source-scoped replay passed
**2/2 existing exact owners, 3,007 complete original PE
bytes and 167 relocations** (2,968-byte Update boss and
39-byte decoder leaf). Thirty compiler-private labels were
independently verified from current same-section COFF
definitions, unchanged original PE targets and complete
owner bytes before manifest-only rebinding. No new exact
owner or Windows runtime closure is implied.

## 2026-11-12 SoundPlayer native BGM command queue record traversal

SoundPlayer owns an authentic 32-element commandQueue of 0x10C-byte
SoundPlayerCommand records, each holding the original opcode,
argument, step and 256-byte path. QueueCommand now resolves each
of its five original live field reads/writes from the true
array base plus a native unsigned i386 index*record-width
address, rather than a separately bounded C++ array
subscript. The complete 1,456-byte ProcessQueues command
dispatcher also advances its live retirement cursor by a
native record stride; the source and destination of each
one-element memcpy are the genuine adjacent record addresses.
No command ordering, 31-slot vacancy threshold, recording
allocation, BGM callback or shift-loop exit condition changed.

The eight affected pinned VC7.1 SoundPlayer carriers cold-replayed
**20/20 already-exact owners, 5,400 complete original PE bytes
and 184 relocation fields**, including ProcessQueues and the
QueueCommand leaf. Eleven compiler-private switch labels were
proved against their current same-section COFF definitions,
unchanged original PE operand targets and complete physical
owner bytes before manifest-only rebinding. This recovers the
actual native command queue's address calculation; no newly
exact owner or Windows runtime closure is claimed.

## 2026-11-12 Replay SaveReplay native inline stage-root traversal

The 1,238-byte nonexact ReplayManagerView::SaveReplay serializer
computes the amount of serialized frame and FPS data by visiting
ten original 0x18-byte ReplayBufferLink stage roots embedded
directly in its 0x164-byte manager, then traverses each linked
continuation. The first loop now advances the genuine inline
stage-root pointer by its native unsigned i386 byte address
plus sizeof(ReplayBufferLink), rather than assuming it
belongs to an independent bound-checked C++ array. The ten
stage gate, frame-header reads, link chain contents and
allocation arithmetic remain unchanged: no second
stage-table allocation, new bounds check or cached pointer.

A complete target-bound SaveReplay guard still reports **1,238
target and 1,238 candidate physical bytes**, all **53 original
relocation fields**, 23 direct calls, 49 control-flow blocks
and only the original **two nonmatching register bytes at
+0xB7/+0xBB**. The two affected pinned VC7.1 ReplayInput
carriers also cold-replayed **12/12 separately exact siblings,
4,093 complete original PE bytes and 232 relocation fields**.
The SaveReplay boss remains NONEXACT and is not credited as
a new exact owner or native runtime/product closure.

## 2026-11-12 EnemyManager trailing collision index native stride

The complete 3,900-byte nonexact EnemyManagerView::OnUpdate boss
creates additional player collision boxes at script-defined
trail samples. Its signed trailIndex begins at one, advances
by six each step and is compared as signed against the live
script-controlled trailCollisionLength53A4 short. That original
i386 DWORD add now explicitly wraps modulo 2^32 before
restoring the signed result for the existing condition.
The original sample record address, Float3 movement math,
damage handling, collision callbacks and draw-bucket
scheduling are unchanged; no range guard, trail allocation,
or new collision branch is added.

The complete original-bound Enemy update guard remains
**3,900 target and 3,900 candidate bytes**, with all **97
reviewed relocation fields**, 69 ordered calls, intact
control flow and exactly the previous **13 unmatched ordinary
register bytes**. The full two-carrier pinned VC7.1
EclManager scope replayed **83/83 already-exact siblings,
29,856 physical original bytes and 1,389 bound fields**.
Two private switch labels were independently proved against
current same-section COFF definitions and retail PE operands
before manifest-only renaming. EnemyManager::OnUpdate remains
NONEXACT, not a new exact owner or runtime closure.

## 2026-11-12 ANM file entry chain and resource counts use native DWORDs

The ANM resource loader consumes the real file-backed linked sequence of
0x40-byte AnmRawEntry headers. Its entry-specific texture, name, offset
table, Sprite and Script source addresses, plus the nextOffset traversal
in both ReadAnmEntries and PostloadAnmEntry, now compute unsigned i386
base+byte-offset addresses, not C++ pointer arithmetic over fixed header
subobjects. The offset-table cursor advances by one/four or two/eight
physical bytes and separately rereads the real wire data on each use.

The native signed current sprite/script indexes, total entry/script/sprite
counts, stage entry indexes, postload task counter and its -1 comparison
also now perform explicit modulo-32 DWORD arithmetic before the original
integer tests, resource allocation and D3D texture/ANM calls. No new
bounds check, shadow record, texture cache or change to error paths
is introduced. The source preserves native overflow semantics of counts
read from the ANM file rather than invoking undefined signed C++ overflow.

Only the two actually affected pinned VC7.1 AnmManagerLoad carriers
were cold-compiled. A complete source-scoped replay passed **17/17
already-exact functions, 3,258 original physical PE bytes and
133 relocation fields**, including LoadTextureData, ReadAnmEntries,
PostloadAnmEntry, the surface-loading family and manager constructor.
No private-label rebinding, new exactness credit or full native
Windows executable/runtime claim is implied.

## 2026-11-12 ANM variable-argument mask native unsigned shifts

The 9,776-byte original-bound AnmManager::ExecuteScript interpreter
looks up file-backed integer and float opcode arguments according
to its true unsigned 16-bit ANM variable mask. Both lexical
GET_INT_VAR and GET_FLOAT_VAR source paths now shift an unsigned
32-bit single bit by the original argument number, rather than
performing a C++ signed left shift that is undefined when the
result has the sign bit set. The pinned VC7.1 compiler still
emits the genuine i386 shift instruction and low-five-bit shift
count behavior; no new ordinal clamp, read cache or alternate
mask was added. Both original variable-resolver calls, source
reads and opcode dispatch remain intact.

Only the two actually affected VC7.1 ANM interpreter carriers
were rebuilt. The complete original PE comparison passes
**5/5 already-exact functions, 10,496 physical bytes and
421 bound relocations**, including the complete 9,776-byte
ExecuteScript and four variable resolver leaves. No
compiler-private label rebinding, newly exact owner or
whole-product runtime assertion is implied.

## 2026-11-12 Title replay menu list and stage cursor DWORD wrap

The complete TitleScreenView::OnUpdateReplayMenu owner selects one
of 50 replay-file records and an active stage within its chosen
replay. Twelve signed keyboardSelection mutations now use native
i386 DWORD modulo-32 arithmetic: list -1/+1 and their signed
comparison results, +/-50 and +/-25 wrap, the stage +direction
increment, +/-10 stage wrap, and the initial invalid-frame
cursor advance. The source preserves the original signed branch
gates, selection search, replay-file allocations, title animation
calls, path publishing and stream start order. No new bounds
check, record or replay-header validity test is introduced.

Both pinned VC7.1 TitleScreen carriers replayed **41/41 already
exact owners, 18,972 complete original PE bytes and 953
relocations**, including the large replay-browse and stage-selection
owner alongside its exact title leaves. Nine compiler-private
labels were independently verified against current same-section
COFF definitions, unchanged original PE targets and complete
owner bytes before manifest-only rebinding. The separate
nonexact OnUpdateResult guard remains 485 original / 484
candidate bytes, 34 fields and 218 unlike overlapping bytes;
it is not promoted. No whole-product build or runtime claim.

## 2026-11-12 score.dat reader and variable-size chapter traversal

ScoreFileView::OpenScore decodes the actual file-backed score.dat
allocation, then walks variable-size ScoreChapterView records using
the signed/unsigned original chapter header fields. The byte cursor
for checksum de-obfuscation, four next-byte uses, and both source/
decompressed buffer pointers for the native LZSS call now use i386
modulo-32 address words. The initial file header's dynamic headerSize
and each chapterSize additionally select their next packed record
through true native unsigned base+length addresses rather than
C++ pointer arithmetic over a single projected header object.

The three real LoadScoreRecords/LoadLastName/LoadPlayStats owners
likewise derive the first record from the actual ScoreFileView
headerSize and every subsequent record from that record's real
chapterSize using the same native address arithmetic. The original
version/size checks, checksum, LZSS decoder, score table, last-name
and play-stats publications, failure branches and allocation order
are unchanged; no parser rewrite or new buffer validation is implied.

All six actually affected pinned VC7.1 ScoreData compiler carriers
cold-replayed **7/7 already exact functions, 2,572 complete
original PE bytes and 169 bound relocation fields**. The 523-byte
OpenScore, 441-byte LoadScoreRecords, 215-byte LoadLastName and
178-byte LoadPlayStats owners retain their complete exact extents.
This is packed-file source fidelity, not newly exact credit,
native Windows executable closure or runtime validation.

## 2026-11-12 score.dat writer's packed chapter and checksum byte addresses

SaveTitleScoreData writes original TH09 score.dat chapters to a live
0x640000-byte allocated output buffer, appending the TH9K, score,
last-name, PLST and version payloads at its original signed byte
offset. Six packed-record destinations now form native unsigned i386
base + uint32(currentOffset) addresses instead of C++ pointer arithmetic
over a projected chapter subobject. The checksum loop similarly indexes
the genuine output allocation with uint32(byteIdx), and the trailing
obfuscation cursor begins at the native byte address scoreData+1.
All RNG order, chapter widths, checksum/encryption calls and file writes
remain unchanged; no extra bounds check or alternate buffer is introduced.

The six source-scoped pinned VC7.1 carriers cold-replayed **7/7 existing
exact functions, 2,572 complete original PE bytes and 169 relocations**,
including the 745-byte SaveTitleScoreData boss and its original score
parsing/timekeeping leaves. No compiler-private label changes or
new exact-function/runtime claims were needed.

## 2026-11-12 PBG file mode and module path native byte cursors

PbgFileView::Open examines the real caller-provided NUL-terminated
mode string to select the original Win32 GENERIC_READ/WRITE,
creation disposition and append behavior. The scanning char*
cursor now advances through its genuine byte address using an
unsigned modulo-32 i386 addition instead of relying on a
separate C++ fixed-size array object. GetFullFilePath likewise
places the terminating NUL immediately after the last module-path
separator by a native DWORD +1 byte address in the original
MAX_PATH buffer. The original filename and module path reads,
NULL-separator behavior (including its existing lack of
additional guard), strcpy/strcat calls and all file system
side effects are unchanged.

The one affected pinned VC7.1 PbgFile compiler carrier
cold-replayed **11/11 existing exact functions, 794 complete
original PE bytes and 22 relocation fields**, including
Open, GetFullFilePath, ReadWholeFile, Read/Write/Seek and
the small constructor/destructor leaves. No private label
rebind, new range guard, exact function promotion or native
Windows-product runtime closure is claimed.

## 2026-11-13 Native ASCII menu VM rows and score-text color byte

The exact **1,776-byte PauseMenu::OnUpdate** and **1,172-byte
AsciiMenuState4::OnUpdate** draw/update real 0x2A4-byte ANM VM records in
their original pause/menu arrays. Twenty-one fixed `menuSprites[0/1/2]`
uses now share the existing `TH09_ASCII_NATIVE_MENU_VM(owner,index)`
with dynamic pause-button loops. This records the single real allocation
and its native Win32 record stride rather than two separate indexing
models; every original read, conditional store and VM method call remains
at its original point. The exact **503-byte
AsciiManager::OnDrawHighPrioImpl** now assigns the opacity byte through
the genuine `AnmVm::color1Bytes[3]` scalar union in each of its three
live popup glyph drawing branches. The union is the same native +0x1F0
D3D color DWORD consumed by the exact ANM renderer; there is no shadow
palette, extra alpha cache or altered draw loop.

One affected pinned VC7.1 compiler carrier replayed **10/10 previously
exact owners, 4,206 complete original PE bytes and 264 relocations**.
Eight private `$L` references in the two menu update bosses were
rebound only after the original PE destinations, current same-section
COFF definitions and all complete physical owner bytes were proved.
This is source and memory-layout fidelity, not new exact-function or
complete Windows runtime/product credit.

## 2026-11-12 ASCII score popup consumer native record cursor and frame word

AsciiManager::OnDrawHighPrioImpl is the consumer of the genuine
two-by-100 array of 0x38-byte score popup records previously
recovered in CreateScorePopup. The high-priority draw loop now
advances its live popup pointer as an unsigned modulo-32 i386
byte address plus sizeof(AsciiManagerPopup), instead of relying
on C++ typed-pointer arithmetic over its one-row view. The
original 100 iterations, live per-side player position lookup,
score character rendering, pop-up timers, alpha gating and
draw calls are unchanged. The separately exact
AsciiManager::OnUpdate callback advances its persistent signed
frameCounter through the same native i386 DWORD INC lvalue,
without changing the original menu/chain gates.

The actually changed AsciiManagerMenu pinned VC7.1 carrier
cold-replayed **10/10 exact functions, 4,206 complete original
PE bytes and 264 relocation fields**, including the 503-byte
OnDrawHighPrioImpl, the 253-byte OnUpdate, the 1,776-byte PauseMenu
boss and the 1,172-byte state-4 menu boss. Independent full
proof also checked the unaffected state-5 owner in its separate
carrier: **11/11 physical owners, 5,442 bytes, 350 fields**.
The three changed private $L labels were verified from actual
same-section COFF definitions, unchanged PE operands and all
physical owner bytes before only their symbol spellings were
refreshed. This preserves existing exactness, not product
runtime closure or a new exact-function promotion.

## 2026-11-12 Title name-entry and replay-save grid DWORD cursor wrap

The complete TitleScreenView::OnUpdateResultNameEntry and
UpdateReplaySave owners share the original 6-by-16 character grid,
with live signed keyboardSelection and replayNameCursor state words.
Twenty original instances across the two actual owners now
explicitly preserve native modulo-32 i386 DWORD semantics:
row changes of +/-16, wrapping by +/-96, confirm-column
navigation +/-13, horizontal left/right increments and
decrements, and name insertion/deletion cursor increments
and decrements. The existing signed row/wrap tests, gap-key
handling, real selected name record, replay SaveReplay call,
ANM state switches and side effects remain unchanged. No new
index clamp, alternate keyboard or buffer is introduced.

Both actually affected pinned VC7.1 TitleScreen compiler
carriers cold-replayed **41/41 already exact functions,
18,972 complete original PE bytes and 953 relocation fields**,
including both large name-entry/replay-save update owners and
their small helper leaves. Nine compiler-private labels changed
spellings, independently proved against current COFF same-section
definitions, identical original PE target operands and every
owned byte before manifest-only rebinding. The separate nonexact
Title OnUpdateResult still has the same 485 target / 484 candidate
bytes, 34 fields and 218 unequal overlaps; it is not promoted.
No new exactness or completed Windows runtime is claimed.

## 2026-11-12 Enemy timeline packed opcode argument words

EnemyScheduleRuntimeView::Run is the native 1,048-byte timeline
interpreter for Enemy schedule spawn/remote/event opcodes. The
original instruction header is eight bytes, followed by variable
numbers of packed four-byte arguments. The six ordinary spawn
arguments, extended spawn/score fields, range/random-position
arguments, and remote slot parameters across opcodes 0, 1, 2,
3, 4, 5, 8, 10, 11, 12, 15 and 17 now use the real
file-backed payload address plus uint32(ordinal)*4. The
lexical TH09_TIMELINE_NATIVE_ARG lvalue intentionally rereads
each operand at its original caller site, rather than treating
the eight-byte header's successor as a separate C++ int array
with a new in-bounds contract. No new argument limit, cached
wire field, memory owner, RNG call or original spawn action
has been introduced.

The single affected pinned VC7.1 EnemyScheduleRuntimeAdvance
carrier cold-replayed **2/2 exact functions, 1,068 complete
physical original PE bytes and 42 relocations**, including the
1,048-byte Run boss and its 20-byte timer leaf.
Sixteen private switch-label spellings changed; their actual
current COFF same-section definitions and unchanged original
PE operands were independently proved before updating only
the matching manifest symbol names. These remain existing
exact owners, not new exactness or game-runtime closure.

## 2026-11-12 Enemy timeline variable-width argument word references

The original EnemyScheduleRuntimeView::Run reads a sequence of variable
length packed ECL/timeline instructions, whose argument DWORDs follow the
fixed eight-byte instruction header in the same file-backed allocation.
For the real enemy-spawn position/flag argument vectors, ranged random
spawn arguments, and remote-slot state handlers, each indexed argument
read now computes its native i386 address as the live payload byte base
plus uint32(argumentOrdinal) * 4, rather than a C++ int array subscript
that requires independently bounded array provenance. The original
float-word aliasing, independent repeated reads, RNG call order,
remote-slot accesses and original spawn/control CFG remain untouched.
No new record length check, table allocation, argument caching or
bounds guard was introduced.

One genuinely affected pinned VC7.1 timeline compiler carrier
cold-replayed **2/2 existing exact physical owners, 1,068 complete
original PE bytes and 42 reviewed relocations**, including the
1,048-byte Run timeline boss and its timer helper leaf. Sixteen
compiler-private switch/jump labels were each independently proved
against the actual same-section COFF definitions, unmodified PE
operands and every owner byte before updating only their manifest
symbol spellings. This is native packed-wire semantics in existing
byte-exact code, not a new exact owner or Windows runtime closure.

## 2026-11-12 RunEcl child context state-copy raw byte addresses

The 15,564-byte EnemyEclManagerView::RunEcl ECL dispatcher creates a
genuine ChildEclBlock (with a 0x22C-byte context), initializes its
subroutine and copies exactly 0x78 bytes of native context-local
state from the active ECL context's +0x1C payload to the child's
own +0x1C payload. Both memcpy operands now explicitly use original
i386 modulo-32 addresses based on the real distinct context
subobjects. This avoids C++ pointer arithmetic outside a projected
0x22C-byte object; the actual ChildSlotRef reread, memcpy size,
state layout, allocation, null gate and subroutine call remain intact.

Original-target full-owner inspection still reports the complete
15,564-byte RunEcl, 598 correctly bound relocation fields and
unaltered switch tables. The original 30 unlike opcode-4/opcode-155
ordinary bytes remain: **this boss is still NONEXACT**.
The two actually affected pinned VC7.1 EclManager carriers separately
cold-replayed **83/83 already-exact sibling functions, 29,856
complete physical original bytes and 1,389 relocation fields**.
Eight private $L symbols changed spelling; their actual current
COFF same-section definitions, retail PE target operands and
complete exact-owner bytes were independently proved before the
manifest symbol names were updated. No exact promotion or Windows
runtime closure is claimed.

## 2026-11-12 Replay metadata formatting native append cursor

The original ReplayManagerView::SaveReplay builds a file-backed user
information block through several calls to the shared exact
AppendReplayFormat leaf. That leaf computes the destination for the
next formatted field as the actual caller-owned byte-buffer base
plus the full strlen of the newly formatted text. The source now
forms this next write cursor with native modulo-32 i386 address
arithmetic rather than applying typed C++ pointer arithmetic
requiring a bounded array inside the generic helper. There is
no additional text bound, duplicated buffer, modified va_list,
or changed format/read chronology.

Both affected pinned VC7.1 ReplayInput carriers cold-replayed
**12/12 exact sibling owners, 4,093 complete original bytes and
232 bound relocation fields**, including the 45-byte append
leaf. The complete still-nonexact 1,238-byte SaveReplay owner
was separately recompiled and compared to the retail PE; it
continues to preserve all 53 fields and 23 ordered calls, with
exactly the preexisting two register bytes at +0xB7/+0xBB
different. This is a native source-level leaf improvement,
not new SaveReplay exact credit or a product/runtime claim.

## 2026-11-12 Etama live bullet/laser pool and count DWORD traversal

EtamaController::OnUpdate walks the actual contiguous primary,
secondary and laser allocations inside the real 0x25E1C0-byte Etama
controller. The maintained source now advances its two primary/
secondary lookup-loop cursors, the full linear bullet update cursor,
and the laser update cursor as i386 modulo-32 byte addresses plus
sizeof(Bullet) or sizeof(Laser). It neither invents new storage nor
changes the pool gates, 176+361 Bullet capacities, 48 laser records,
draw-bucket writes or any action/timer call. The three persistent
signed activeTotalCount/activePrimaryCount/activeSecondaryCount
words are now incremented by native unsigned DWORD lvalues,
preserving original INC wrap and their separate side/capacity gates.

All three affected pinned VC7.1 Etama compiler carriers were rebuilt.
An independent original-PE-versus-real-current-COFF check proved the
complete physical owner bytes, offsets, target operands and
same-section definitions. Official scoped replay passed **18/18
already-exact functions, 6,711 original PE bytes and 259
relocation fields**, including the complete OnUpdate and large
SpawnSingleBullet owners plus the sprite/template leaves. No
newly exact function or Windows product/runtime closure is claimed.

## 2026-11-12 Pause and gameover menu native state/frame counters

The exact pause-menu and side-gameover menu owners share the actual
AsciiManager and native GameManager side storage. PauseMenu::OnUpdate,
AsciiMenuState4::OnUpdate and AsciiMenuState5::OnUpdate now advance their
signed state and frames words using native modulo-32 i386 DWORD
increments, preserving the existing signed selection and timeout
comparisons. PauseMenu's live difficulty-based sprite index also
forms an unsigned native DWORD addition before restoring the signed
script ID for the original SetSprite call. State5's side-owned
GameManager runtime retry counter at +0x14 is **a BYTE**, not an int;
its native one-byte increment is expressly preserved. An initial
DWORD-sized attempt was rejected after a complete 1-byte PE mismatch
and before committing the correction. No alternate VM allocation,
new selection guard or changed menu call was introduced.

Independent full COFF-versus-original-PE proof covered all
11 source-present exact owners in the two affected actual compiler
carriers: **5,442 complete original bytes and 350 relocation
fields**. Twenty-three renamed compiler-private jump labels were
proven against actual same-section COFF definitions, the unmodified
PE operand targets and every complete owner before updating only
their manifest spellings. Official focused cold replay then passed
10/10 AsciiManagerMenu exact owners (4,206 bytes, 264 fields) and
1/1 AsciiMenuState5Update owner (1,236 bytes, 86 fields). These
were already exact; no new exact function or runtime closure is claimed.

## 2026-11-12 Supervisor loading draw state, ANM surfaces and startup VM strides

The real SupervisorDrawMain callback updates the signed +0x740
loading-state word before displaying the shot-button prompt and
copying back-buffer surface eight. The original i386 increment
is now expressed as an unsigned DWORD lvalue, retaining the
signed state gates, animation opacity and reset timing without
introducing new branches.

SupervisorDrawMainAnmManagerView::GetSurface reads from the true
ANM manager's 32 contiguous 4-byte surface pointers at +0x123FC.
The live signed surfaceIndex now forms a native unsigned
base + uint32(index)*sizeof(pointer) slot address rather than
a bounded C++ array subscript. SupervisorDrawLoading's existing
three genuine 0x2A4-byte loading animation VM records are likewise
addressed with a single lexical i386 DWORD stride at all five
separately evaluated positions (position update, draw, position
restore). The actual SDK float operations and the original call
sequence remain unchanged.

The three independently affected pinned VC7.1 compiler carriers
cold-replayed **5/5 exact existing owners, 445 complete original
PE bytes and 26 bound relocation fields**: the full 312-byte
SupervisorDrawMain, three 62-byte DrawMainLeaves in total, and the
71-byte SupervisorDrawLoading callback. No compiler-private label
rebinding or newly exact function/native runtime claim was involved.

## 2026-11-12 Title result and replay name-entry native byte slots

The TitleScreenView result-name editor and Replay save-name editor
consume the actual g_TitleAlphabet runtime byte pointer, genuine
0x2C-byte title score name records and a separate eight-character
replay-name buffer. Six original dynamic character reads/writes
now compute the live signed keyboardSelection/replayNameCursor
offset as an unsigned modulo-32 i386 byte address from each
actual backing allocation. The source preserves all separate
table lookups, space-key paths, erase behavior and original
PlayMenuSound, SaveReplay, and ANM calls. It introduces no
new index gate, string allocation, alternate row layout or
register-specific compiler decoration.

The two actually affected pinned VC7.1 TitleScreen compiler
carriers cold-replayed **41/41 already exact owners, 18,972
complete original PE bytes and 953 relocations**, including
large title menu, result-name and replay-save update owners
and their smaller helpers. Nine compiler-private jump labels
were verified from current COFF same-section definitions
and original PE operand targets before their manifest names
were updated. The separate nonexact OnUpdateResult remains
484/485 physical bytes, 34 fields and 218 ordinary
differences (one missing byte); no newly exact function,
native-product or runtime closure is claimed.

## 2026-11-12 ASCII text queue and score popup native records

AsciiManager owns a real 256-entry string queue at its original
+0x2254-relative allocation, each AsciiManagerString 0x60 bytes.
The complete DrawStrings owner now advances its live string-record
cursor by one native unsigned i386 0x60-byte stride and scans
each NUL-terminated input text byte with the same native
32-bit byte-address arithmetic. AddString resolves the
real queue write lvalue from the signed numStrings field as
base + uint32(index)*0x60, then increments that live
count through an unsigned 32-bit DWORD. The original text
copy, scale, viewport switch, draw and call chronology is
unchanged; no new buffer or bound guard is introduced.

CreateScorePopup likewise selects the real two-by-100 array
of 0x38-byte AsciiManagerPopup records at +0xB240 using
native 32-bit side*row-width plus write-index*record-width
addresses. Its signed nextScorePopupIndex increments with
the original modulo-32 DWORD behavior before the existing
wrap-at-100 logic; the popup score formatting and timer
initialization remain intact.

The three separately affected VC7.1 carriers replayed
**4/4 existing exact functions, 680 complete original PE
bytes and 15 relocation fields**: DrawStrings (341 bytes
and 12 fields), AddString/AddFormatText (185 bytes and
two fields), and CreateScorePopup (154 bytes and one
field). No compiler-private labels or newly exact
functions were needed. This restores source fidelity
to actual text/popup storage rather than proving
native Windows runtime closure.

## 2026-11-12 Etama bullet, laser and live pool-count native traversal

The full EtamaController::OnUpdate and SpawnSingleBullet owners
iterate the genuine native Bullet allocations, including primary
and secondary pool records and the original sentinel/unused-state
checks. The source now advances these live Bullet pointers by
native unsigned i386 base + sizeof(Bullet) addresses in both
spawn loops and the update scan, without imposing an unrelated
C++ subobject/array-bound condition or changing the sentinel
restart. The update's separate Laser pool similarly advances
by sizeof(Laser) native byte addresses. Three signed live pool
counts (activeTotalCount, activePrimaryCount, activeSecondaryCount)
now increment through their real unsigned DWORD lvalues before
the original signed gate comparisons. No new capacity gate,
shadow pool, callback or timer behavior is introduced.

The three affected pinned VC7.1 BulletManager carriers cold-replayed
**18/18 already exact owners, 6,711 complete original PE bytes
and 259 original-bound relocations**, including the full Etama
update and SpawnSingleBullet bosses and their small leaves.
Sixteen compiler-private $L spelling changes were independently
verified from current COFF same-section definitions, actual
original PE destinations and every complete owner before
rebinding only manifest symbol spellings. This recovers native
i386 pool traversal semantics without promoting a new exact
function or claiming a completed Windows runtime.

## 2026-11-12 Etama bullet sprite ID signed DWORD wrap

The full Etama controller's sprite-selection path combines the
live signed AnmVm::activeSpriteIndex with script-controlled
sprite offsets, including the two original size-dependent
g_BulletSpriteOffsetSmall/Medium pointer-table reads. Four
separate source expressions now compute the resulting original
sprite ID as unsigned modulo-32 i386 word arithmetic before
restoring its signed value. This preserves the independently
reloaded source VM, target sprite size branches and original
SetSprite call sites. The sprite owner, script tables and
controller memory remain unchanged; no extra sprite bound
check or allocation is introduced.

The three genuinely affected pinned VC7.1 BulletManager
compiler carriers passed **18/18 existing exact functions,
6,711 complete original PE bytes and 259 relocation fields**,
including the large EtamaController::OnUpdate and
SpawnSingleBullet owners. Sixteen compiler-private $L
jump/switch labels changed their spelling only. Every
current same-section COFF symbol definition, original PE
operand and complete owner was independently validated
before refreshing manifest symbol names. This improves
native arithmetic semantics in already-exact codegen,
not the canonical exact count or native Windows runtime gate.

## 2026-11-12 Player gameplay selector configured timing DWORD arithmetic

The complete 3,238-byte PlayerUpdateSelectorState owner publishes
opening pattern timing to the opposing front side. The signed
PlayerPatternConfig::openingShort08 field is read live from the
genuine 0x1C-byte pattern configuration row and multiplied by
60, then the original signed ZunTimer opening-frame value is
subtracted before calling SetTimingDigits. That combined
multiply/subtract now uses the native i386 unsigned DWORD word
arithmetic and returns the original signed word bits, rather
than depending on C++ signed-overflow behavior for game data.
No extra clamping, replay input read, timer increment or
change to RNG/pattern selection is introduced.

Both actually affected pinned VC7.1 carrier profiles cold-replayed
**2/2 existing exact functions, 3,650 complete original PE
bytes and 154 bound relocation fields**, including the
3,238-byte full selector and its 412-byte ResolvePatternOffset
leaf. Two compiler-private jump labels were renumbered and
independently proved from current same-section COFF definitions,
unchanged original PE operands and full owner bytes before
re-binding only their manifest spellings. No new exact
function or Windows-native runtime closure is claimed.

## 2026-11-12 Supervisor camera and viewport native record selection

The original Supervisor allocation owns three actual 0xF0-byte
camera/view/projection/viewport records at +0x78. SelectSide,
ConfigureScreenViewport and ConfigureGameplayViewport receive
a signed native view selector and compute the live camera owner
as the base of that real array plus uint32(index)*0xF0.
The maintained source now expresses all three dynamic selections
through one lexical native i386 lvalue, rather than applying
a potentially out-of-range C++ array subscript to the three
configured views. The existing ApplyScreenCamera, ApplyGameplayViewport,
D3D SetViewport, projected math, and currentIndex publishing calls
retain their original ordering. The three fixed setup camera records
also remain unchanged; there is no new index gate, alternate
camera, or backing allocation.

The single affected pinned VC7.1 SupervisorViewport compiler
carrier cold-replayed **8/8 exact owners, 1,530 complete original
PE bytes and all 106 relocations**, including the full viewport
initialization and both screen/gameplay camera application paths.
No changed private labels or new exact-function/runtime claims.

## 2026-11-12 Supervisor screenshot D3D lock pitch and bitmap cursors

The original Supervisor::TakeSnapshot owner reads a real Direct3D 8
locked back buffer and writes a separate RGB24 BMP allocation. The
signed driver-provided D3DLOCKED_RECT::Pitch is negated as an i386
DWORD and multiplied by 0x1DF to select the first source row.
Both the initial source row, its signed per-row negative stride,
the destination bitmap row advance, and the inner two-byte source
and one-byte destination cursor advances now explicitly form native
unsigned modulo-32 byte addresses. This replaces unsafe C++
signed-pitch overflow and typed-pointer arithmetic assuming a
single bounded CPU array for a driver-owned surface, while keeping
all original pixel reads, RGB24 byte order, buffer allocations,
D3D surface lock/unlock, WriteFile calls and cleanup branches.

The sole affected pinned VC7.1 SupervisorSnapshot carrier passed
**1/1 exact complete TakeSnapshot owner, 543 original physical
PE bytes and 23 relocation fields**. No compiler-private label
rebinding, newly exact owner, alternate file format or Windows
runtime closure is implied.

## 2026-11-12 Supervisor network frame queue native record cursors

The original Supervisor owns twenty contiguous 0x0C-byte input/prediction
queue records at +0x47C and two last-received records at +0x56C;
replay input samples live separately in the real 0x8E-byte
g_ReplayInputStates array. The full InsertPredictedFrame and PopFrame
owners now form these real byte addresses from their live bases using
unsigned i386 DWORD strides, including the signed side selector and
combined queue index. The 0x480/0x0C-byte scan increments, native
+0x4E8 insertion tail address, and both previous/next record shifts
also use modulo-32 native pointer words rather than C++ typed-pointer
arithmetic over a projected single queue element. Lexical PREV/NEXT
macros preserve all four independently read fields and the original
shift/copy/loop order, with no added queue, bounds check or frame
prediction logic.

The two genuinely affected pinned VC7.1 carrier replays verify
**2/2 existing exact functions, 310 complete original PE bytes and
one original-bound relocation**, covering both native queue
insert/predict and consume owners. No private label rebinding, newly
exact owner, whole-product build or runtime validation is claimed.

## 2026-11-12 PBG packed entry-count and file cursor words

The native PbgArchive::AllocEntries parser walks the real LZSS-decoded
PBG file-table buffer, consuming variable-length NUL-terminated
filenames and three contiguous four-byte words per entry. SeekPastInt
now advances its original void* cursor by exactly one unsigned
i386 DWORD address stride, not a typed pointer into a fictional
C++ int array. SeekPastString uses the real live strlen result plus
the trailing NUL as a native byte-address addition, preserving
its original independent read and return. The signed on-disk
entry count plus the extra end-of-archive sentinel element is
also formed in native modulo-32 unsigned arithmetic before the
existing array allocation/construction.

The one affected pinned VC7.1 PbgArchiveHelpers carrier cold-replayed
**6/6 already-exact physical functions, 441 full original PE bytes
and 18 bound relocation fields**, covering both packed-file cursor
leaves, entry allocation, copy, entry construction and release.
No new bounds gate, file-table buffer, deobfuscation step, runtime
behavior claim or new exact function is introduced. This preserves
the original packed archive's actual i386 source semantics.

## 2026-11-12 Supervisor version file-backed line and byte cursors

SupervisorVersionView::CheckVersion scans the actual opened .ver
file's packed version/name/size/checksum lines. The original i386
advances the file cursor six bytes past a matching version tag,
then moves one byte past the next newline; its remaining-byte
count decrements by the native difference of these two cursor
addresses. The source now computes both cursor increments as
unsigned modulo-32 i386 byte addresses and the consumed span
as their unsigned DWORD difference, rather than relying on C++
pointer arithmetic outside an individual fixed-size line object.
The live strcmp/sscanf paths, newline search, version gates and
error returns are unchanged, without a new bounds check or
file buffer allocation.

The two affected pinned VC7.1 SupervisorVersion compiler carriers
cold-replayed **2/2 existing exact owners, 344 original physical
PE bytes and 22 relocation fields**, including CheckVersion
and its original data-load companion. No compiler-private label
rebinding, exact-function promotion or whole-product/runtime
claim was involved.

## 2026-11-12 PBG archive encoded header and sentinel entry words

PbgArchive::ParseHeader reads the genuine 12-byte encrypted archive
header's three signed DWORD fields for the entry count, file-table
offset and decompressed size. The native original subtracts the
three obfuscation constants as modulo-32 i386 words before its
existing signed count and unsigned file-length gates. The
maintained source now explicitly forms those signed result bits
from unsigned subtraction rather than invoking C++ signed overflow.
The actual allocation and LZSS decode sequence remain unchanged.

PbgArchive::ReadDecompressEntry uses a next-record data offset to
determine compressed size. AllocEntries constructs a real
entryCount+1 sequence with an end sentinel, each PbgArchiveEntry
0x10 bytes wide. The source now resolves the next live entry's
dataOffset as the original i386 base address plus one native
record stride, not from an imagined independent header allocation.
The existing difference, seek, read, decompression, and error
paths are unchanged.

The two truly affected pinned VC7.1 compiler profiles were
cold-replayed independently: **2/2 exact archive Load/ParseHeader
owners, 646 complete physical bytes and 31 fields**; **1/1
exact ReadDecompressEntry, 193 bytes and seven fields**. Combined
these preserve 3/3 already-exact owners, **839 complete original
PE bytes and 38 relocation fields**, without changed labels,
new error gates or exact-function promotion.

## 2026-11-12 FrontMessage file-backed table and variable instruction addresses

The exact FrontMessageRuntimeView::LoadForSideCount loader reads
the primary message table's authentic packed file allocation. Each
nonzero native DWORD entry offset is rebased from the actual
file-buffer address, and the opcode-7 music-preload scan moves from
one variable-length instruction to the next by its original
size03 byte plus four-byte header. Both source expressions now
form modulo-32 i386 byte addresses rather than doing C++ pointer
arithmetic outside the individual projected wire structs. No new
music guard, cached record, table allocation, changed label or call
is introduced; the nullable-path/error path and both file caches
remain unchanged.

Only the affected pinned VC7.1 FrontMessageLoad carrier was
rebuilt, and complete original-bound replay passed **1/1 exact
owner, 405 complete physical PE bytes and 18 relocation fields**.
This improves real file-wire source semantics in an existing exact
loader; no function promotion, native-product or runtime closure
is implied.

## 2026-11-12 Supervisor network config parser file-backed byte cursors

The native Supervisor IP.txt configuration parser walks one real
file-backed character stream through whitespace, comments, key names
and parsed values. SkipNetworkValueWhitespace, ParseNetworkConfigValue
and PrepareTitleMode4Network now advance their existing cursor by
native modulo-32 i386 byte addresses, including the original
strlen(key)-sized advance and six single-byte advances. Each read and
remaining-length decrement retains its original position, and the
parser still uses the same genuine 256-byte output and state fields.
There is no new buffer, alternate parsing grammar, bounds check,
allocation, or call-order change.

Only the actually affected pinned VC7.1 SupervisorNetworkConfig
carrier was cold-compiled. The complete source-scoped original-bound
replay passed **2/2 existing exact owners, 922 physical original PE
bytes and 23 relocation fields**, including the network config
parser leaf and the full title mode-4 network setup owner. This
source-level native-wire repair neither promotes new exact functions
nor claims the separately nonexact Supervisor network service or
Windows-game runtime closure.

## 2026-11-12 RunEcl interpolation slot cursor native record strides

The full 15,564-byte EnemyEclManagerView::RunEcl owns an eight-slot
array of authentic 0x30-byte Th09EclInterpolationSlotView entries
in each active 0x22C-byte ECL context. Both the interpolation
installation loop and the after-dispatch interpolation callback
loop now advance the actual slot cursor using its native unsigned
i386 DWORD byte address + sizeof(Th09EclInterpolationSlotView)
instead of relying on C++ typed-pointer advancement. The original
eight-slot limits, script-controlled interpolation parameters,
callback lifetimes, timer comparisons and per-frame call order
remain unchanged. No extra capacity check, reordering or duplicate
storage was added.

The original-bound full-owner inspector continues to verify the
complete **15,564-byte** RunEcl candidate, **all 598 relocations
and unchanged original switch tables**, with the same 30 ordinary
nonexact bytes (opcode 4: 7, opcode 155: 23). Both actual
EclManager VC7.1 carrier profiles cold-replayed **83/83
already exact sibling functions, 29,856 complete physical
bytes and 1,389 bound relocation fields**. Eight compiler-private
$L spellings changed; their current COFF same-section definitions,
original PE target operands and each complete exact owner were
verified independently before updating only those manifest names.
This is source fidelity in the still-NONEXACT interpreter, not
a new exact owner, runtime proof or whole-game link claim.

## 2026-11-12 Supervisor music track and unlock native byte slots

The actual Supervisor audio source resolves one of 19 live
0x10-byte g_SupervisorMusicTracks records by track ID, then
rereads the resolved track, path, sound-player ID or unlock ID
at each original PlayMusic/LoadMusic/ReleaseMusic call site.
Eight such record references now form native unsigned i386
base + uint32(index)*sizeof(SupervisorMusicTrackRecord) addresses.
The four dynamic g_MusicUnlocked byte writes, which accept a
signed ID from the original table or the PlayAudio caller, likewise
form real native byte addresses without C++ array-subscript
bounds assumptions. Original search order, replay-state gates,
MIDI/BGM calls and all separate live reads remain intact, with
no new unlock checks or storage buffers.

The sole pinned SupervisorAudio VC7.1 carrier cold-replayed
**8/8 existing exact functions, 1,054 complete physical PE
bytes and 82 original-bound relocation fields**, including
PlayMusic, PlayAudio, LoadMusic, ReleaseMusic, StopAudio and
their related leaves. The natural lexical lvalue expressions
introduce no extra call, label remapping or new exactness.
This improves source fidelity to actual i386 music storage,
not Windows-native product or runtime closure.

## 2026-11-12 Supervisor state-machine frame and screen-effect counters

The complete Supervisor::OnUpdate handles the network service
state machine, game/title/ending transitions, D3D reset paths and
per-frame ANM/sound updates. Two original persistent signed
i386 DWORDs are updated at the end of its existing branch graph:
Supervisor::calcCount454 increments before the modulo-4000
version-check cadence, and global g_ScreenEffectCounter decrements
when nonzero. Their source now writes through the same native
unsigned 32-bit lvalues, removing C++ signed-overflow undefined
behavior while retaining the original signed modulo/test and
network/transition-call chronology. No new frame gate, state
clamp, branch or callback is introduced.

The one affected pinned VC7.1 carrier independently proved and
cold-replayed **1/1 exact complete Supervisor::OnUpdate owner,
996 original bytes and all 92 original-bound relocations**.
Fourteen compiler-private state-machine jump labels were
verified against actual COFF same-section definitions and the
original PE operand targets before updating only manifest
symbol spellings. This is native source fidelity on a previously
exact core state-machine boss, not a newly exact function or
whole-Windows-game runtime claim.

## 2026-11-12 ANM variable-mask leaves preserve unsigned bit tests

The real AnmVm::GetFloatVarPtr and GetIntVarPtr resolve dynamic ANM
script variable arguments from a 16-bit wire mask and a native
32-bit argument number. Their mask tests now shift an unsigned
one-bit DWORD, rather than a signed int whose sign-bit shift is
undefined in C++. This preserves the original i386 zero/nonzero
mask check without a new guard or variable conversion. The same
real variable-ID switches, pointer returns and read chronology
remain unchanged. An explicitly masked shift-count trial changed
the GetFloatVarPtr complete physical extent and was rejected
before installation; the natural unsigned-DWORD bit test is
already byte-identical to retail.

Both genuinely affected VC7.1 carriers cold-replayed **5/5 exact
functions, 10,496 complete original PE bytes and 421 relocation
fields**, including both variable-mask leaves and the 9,776-byte
AnmManager::ExecuteScript complete owner. No compiler-private
label changes were required. This recovers the unsigned nature
of the variable-mask bit test in existing exact source, not new
exact-function or Windows-product/runtime closure.

## 2026-11-12 Replay decoded file-header frame and FPS wire slots

ReplayManagerView::LoadReplayData interprets the actual file-backed
0x1EC-byte ReplayDataView header as u32 frame/FPS offsets and then
rebases each populated field to its native runtime pointer in the
same storage. Three separate ten-element frame rows and the ten
FPS slots are physically four-byte words. The stage loop now
reads its six original frame-offset values, writes three frame
pointers, reads two FPS-offset values and writes the FPS pointer
through native i386 base + uint32(stage)*sizeof(word) addresses.
The source uses the genuine replay record bases and preserves all
original conditional pointer publications and null tests; it does not
construct a second header or introduce bounds checks or new calls.

The two affected pinned VC7.1 ReplayInput carriers cold-replayed
**12/12 exact siblings, 4,093 complete original bytes and 232
relocations**, including the 591-byte LoadReplayData boss.
The separately nonexact SaveReplay remains 1,238 bytes with
53 fields and only its old two register bytes unlike. The
separately nonexact BeginPlaybackStage remains 538 bytes versus
540 target bytes with all 32 fields. Neither is promoted.
The modification strengthens native packed-wire source fidelity,
not whole-game Windows/runtime closure.

## 2026-11-12 Effect pool and animation VM native record traversal

The actual EffectManager pool contains live 0xD8-byte Effect records,
and each active effect owns a separately allocated runtime-counted
0x2A4-byte AnmVm list. ResetPool, SpawnEffect, SpawnEffectWithVelocity,
OnUpdate and ReleasePool now advance real Effect pointers as native
unsigned i386 DWORD addresses plus sizeof(Effect), rather than C++
pointer arithmetic presuming an independently constructed array.
InitializeEffect's six original distinct VM writes and calls and
OnUpdate's repeated runtime VM script calls resolve the real VM
allocation by base + uint32(index)*sizeof(AnmVm). Both draw loops
also advance the genuine 0x2A4-byte VM record cursor using its native
DWORD stride. A lexical lvalue macro keeps the six initialization
accesses readable while preserving their separate original loads.
No pool capacity, sentinel, allocation, bounds gate, call or drawing
behavior was introduced or changed.

The eight affected pinned VC7.1 EffectManager carriers cold-replayed
**15/15 already-exact owners, 2,530 complete physical original PE
bytes and 100 bound relocations**. The separate 51-byte AddedCallback
owner remains nonexact with its original four relocation fields,
two ordered calls and precisely eight ordinary register bytes
unmatched; that result was independently rechecked after the edits.
The source repair is native storage fidelity for genuine pool and
script-driven VM lifetimes, not new exactness or native runtime closure.

## 2026-11-12 Title replay-save selected-row and name-cursor DWORDs

TitleScreenView::DrawReplaySave renders the live selected replay
index as a one-based row and positions the blinking name-entry
underline using replayNameCursor. Those two values are signed
32-bit fields in the authentic TitleScreen allocation. The source
now computes selectedReplay+1 and
(replayNameCursor+6)*9 as native unsigned i386 DWORD arithmetic,
then restores the original signed result for the existing text and
float-position operations. The state gates, 6x16 name grid, SDK
vector math and actual ANM/text calls are unchanged.

The pinned VC7.1 TitleReplaySave carrier cold-replayed the
**complete 800-byte DrawReplaySave owner and all 41 original PE/COFF
relocation fields, 1/1 exact**. No additional helper, bound, compiler
profile change, private label rebinding or new exact function is
claimed. This is source-level recovery of the original 32-bit
arithmetic at two signed dynamic selectors.

## 2026-11-12 SaveReplay linked input-stream native pointer slots

The nonexact 1,238-byte ReplayManagerView::SaveReplay serializes each
live ReplayBufferLink's three input streams. Within the file-backed
serialization loop, the stream index selects a real three-DWORD
input[3] pointer row in the 0x18-byte link object, not an independent
C++ bounded array lookup on unrelated storage. The source now forms
the original Win32 unsigned byte address from the live &link->input[0]
plus uint32(stream)*sizeof(pointer), and rereads the pointer at its
existing memcpy argument site. All three source streams, chain
traversal, source size, destination, allocation and serialization
chronology remain as originally observed. No bound check, shadow
pointer table or cached source is introduced.

The pinned VC7.1 full nonexact owner guard still reports **1,238
target and 1,238 candidate physical bytes**, all **53 reviewed
relocations**, 23 ordered calls, and exactly the previous **two
unmatched ordinary register encoding bytes** at +0xB7/+0xBB.
It remains NONEXACT. Both actually affected ReplayInput compiler
carriers cold-replayed **12/12 separately exact sibling owners,
4,093 original bytes and 232 bound relocations**, with no
regression. This is native-address source fidelity; no newly exact
function or Windows product/runtime completion is claimed.

## 2026-11-12 MIDI event and SysEx raw track stream cursor semantics

The large MidiOutput::ProcessMsg owner executes note, SysEx and
Meta-event dispatch directly from the live MIDI track file stream.
Six one-byte cursor increments, one identical post-increment during
the variable-length SysEx payload copy, and one dynamic meta-event
data-length advance now compute the original unsigned 32-bit i386
byte addresses from the actual cursor value. Every MIDI event read,
channel update, allocator call and running-status publication is still
performed at its original point. No input bounds check, cached track,
extra stream allocation, or new MIDI validation branch was introduced.

An additional two-site attempt to use a raw byte-address lvalue
for the transposed per-channel note flags changed 22 physical bytes
and was rejected; the installed source retains its prior proven
C++ note-flag expression. The single pinned VC7.1 MIDI carrier
then cold-replayed **32/32 exact canonical functions, 4,270 physical
original PE bytes and all 132 relocations**, including the full
1,574-byte ProcessMsg owner. An independent proof also checked the
one-byte neighboring virtual timer entry (33/33 physical owners,
4,271 bytes). The 23 compiler-private switch label spellings
were verified against actual same-section COFF definitions,
unchanged original PE destinations, and every owner byte before
re-binding their names. This is source fidelity in already-exact
machine code, not a new function-promotion or runtime claim.

## 2026-11-12 GameWindow frame-skip and D3D present countdown widths

The retail GameWindow::Render loop increments its signed one-byte
framesSinceRedraw10 at +0x10 with an i386 BYTE operation, then compares
that actual signed byte against the live Supervisor frameskip setting.
The source now increments the same physical byte through its unsigned
view, retaining the native eight-bit wrap without relying on narrowing
a signed C++ integer. In GameWindow::Present the signed
screenTransitionCountdown field at Supervisor +0x5A4 is similarly
decremented through its real 32-bit storage, matching the retail
i386 DWORD wrap while preserving the original nonzero/registration
guard, D3D reset, snapshot and swap-chain calls.

The two separate pinned VC7.1 carrier replays both passed their
complete original physical owners: **GameWindow::Render 326/326
bytes and 30 relocations; GameWindow::Present 232/232 bytes and
22 relocations** (2/2 existing exact units, 558 bytes / 52 fields).
No compiler-private label rename or new exactness credit is involved;
the modification preserves original integer width and loop timing
semantics rather than proving native runtime closure.

## 2026-11-12 Background stage-camera keyframe native next-record pointer

The complete BackgroundRunStageScriptPhase interpreter reads the initial
camera position from a packed file-backed 0x14-byte RawStageInstrProbe
record. Opcode zero then reads the next physical instruction to obtain
the interpolation destination/ending frame. Instead of assuming a
C++ array containing at least two full instruction objects, the source
now advances the original instruction pointer by exactly one native
unsigned i386 DWORD byte stride, sizeof(RawStageInstrProbe).
The later frame load, camera vector transport and all original
jump/dispatch paths stay in their original order. No added bounds check,
shadow record, cache or second source allocation is introduced.

The sole affected pinned VC7.1 carrier replayed **5/5 existing
exact functions, 3,231 complete physical bytes and 160 original-bound
relocations**, including the full 2,652-byte stage-script switch owner
and its camera interpolation, scalar and timer leaves. The compiler
renamed 48 private $L labels; every actual COFF same-section
definition, unchanged retail PE destination and complete owner byte
was proved independently before rebinding only their manifest
spellings. This improves file-wire source fidelity, not the exact
function count or Windows native-product closure.

## 2026-11-12 Title replay-save name-grid and record addresses

The complete TitleScreenView::DrawReplaySave owner reads replay player
names and play times from the real TitleScreen 50-row ReplayDataView
allocation (0x1EC bytes per entry), then draws the 6x16 native
name-entry grid using the original g_TitleAlphabet byte buffer.
Two independent magic reads and one each of the playerName/playTime
fields now resolve the real replay record from 32-bit base +
uint32(row)*sizeof(ReplayDataView); the name-grid byte likewise
comes from its own genuine pointer plus uint32(row)*16+uint32(col).
No extra bounds gate, copied record, cached source or changed
text-drawing call was introduced; the original finite loops remain.

The one pinned VC7.1 TitleReplaySave carrier passed **1/1 exact
complete 800-byte owner with all 41 original PE/COFF relocation
fields**. Neither compiler-private labels nor the canonical exact
function ledger required modification. This is native record and
pointer storage fidelity for an existing exact drawing function,
not new exact-function or runtime closure credit.

## 2026-11-12 MIDI file-backed SMF parser and variable-length cursor words

MidiOutput::ParseFile reads native SMF file data through a live byte cursor:
the eight-byte header, variable-length header payload and each eight-byte
MTrk prefix and variable-size track body. The maintained source now forms
those four cursor advances through real unsigned modulo-32 i386 byte
addresses. It also reads the authentic big-endian header fields at +2/+4
and each track length word at +4 through that same native address arithmetic,
without assuming a fixed C++ array object beyond the input pointer.
MidiOutput::SkipVariableLength likewise advances the actual caller-owned
byte cursor by one native DWORD byte address per variable-length chunk
byte. No new null check, file bounds limit, MIDI allocation, read cache,
or call-order change was added.

The original-bound pinned VC7.1 MIDI carrier cold-replayed **32/32 exact
units, 4,270 physical original bytes and 132 bound relocations**, including
the 358-byte ParseFile, 32-byte SkipVariableLength and 1,574-byte
ProcessMsg complete owner. A separate independent full-COFF proof also
verified the adjacent one-byte virtual timer thunk: **33/33 physical
owners, 4,271 bytes and 132 fields**. The 23 private switch/jump labels
renumbered by the compiler were each proven against their actual
same-section COFF definition and original PE target before only
their manifest spelling was updated. Existing exactness is preserved,
not newly granted; product/native/runtime closure is still open.

## 2026-11-12 Player movement native x/y position member accesses

The 1,900-byte nonexact PlayerLifecycleView::UpdateMovementAndOptions
owner operates on the genuine Player position at +0x1B88, with x/y
at +0x1B88/+0x1B8C. The independently original-bound Float3 conversion
callee PlayerPositionView::operator float*() still executes at its
original call site. The source now treats its returned address as the
real PlayerPositionView subobject and updates/clamps position.x and
position.y directly, rather than indexing position[1] through a
pointer to the first float member. This removes a cross-member C++
pointer arithmetic assumption without deleting the retail conversion
call or introducing any copied position state.

The refreshed full-owner VC7.1 original PE/COFF inspector verifies
**1,900/1,900 physical bytes, 66 bound relocation fields and all
23 ordered direct calls**, with the unchanged 122 ordinary register/
transport differences. That is strictly nonexact; no exact-function
credit or runtime success is claimed. A direct reference to the object
without the operator float*() call shortened the physical owner
and dropped an original relocation, so that control was rejected.

## 2026-11-12 SoundPlayer queue commands and pan accumulator DWORDs

The full SoundPlayer::ProcessQueues dispatch executes BGM preload,
load, stop, and release steps from real signed commandQueue.step
words. Five native increments and one retry decrement now write the
same original signed step storage as unsigned 32-bit i386 DWORDs,
so stage progression/retry wraps instead of depending on signed
C++ overflow. The accumulated SFX pan value likewise uses explicit
unsigned 32-bit addition before the existing signed mean and pan
publication. Each live command, pan word and count is still reread
at its original source point, with no new guard or helper callback.

The eight actual pinned VC7.1 SoundPlayer carriers cold-replayed
**20/20 existing exact owners, 5,400 complete physical original PE
bytes and 184 relocation fields**, including the 1,456-byte
ProcessQueues boss and its real SFX/BGM leaves. Nine compiler-private
jump labels changed names; their actual same-section COFF definitions
and unchanged PE operands were independently proved before the
manifest symbol spellings were updated. No new exact owner is claimed.

## 2026-11-12 ANM script jump-decrement and frame word wrap

The full original-bound AnmManager::ExecuteScript owns the ANM
instruction dispatch and exact data tails. The JmpDec bytecode
decrements the genuine live VM integer lvalue as an i386 DWORD
before testing the resulting signed operand. The manager's signed
scriptsExecutedThisFrame10 counter also advances with a native
DWORD increment. These two formerly signed C++ operations now
explicitly preserve the original wrap without introducing a clamp,
new opcode branch, duplicated variable read or altered wait/interrupt
path.

The exact script owner and its four independently exact variable
accessors were cold-compiled in both genuine /Ob0 and /Ob1 VC7.1
carriers: **5/5 exact functions, 10,496 complete original physical
bytes and 421 bound relocations**, including the 9,776-byte boss.
The first carrier introduced four accessor-local label spellings;
the second compiled switch carrier introduced 96 additional
compiler-private labels. Every renamed actual same-section COFF
symbol and original PE operand/physical owner was independently
verified before only the manifest spellings were refreshed.
This is source fidelity for existing exact owners, not a newly
exact function or whole native executable build claim.

## 2026-11-12 FrontMessage signed portrait wire sprite ID sums

The exact FrontMessageRuntimeView::Update interpreter executes
multiple file-backed portrait/script instructions that combine live
signed Player tableValue303FC with signed argument08/argument0C,
including second-sprite offsets +9 and +6. Nine original expressions
across the 0x0F, 0x11 and sprite-selection handlers now use native
modulo-32 i386 additions before restoring the signed final ANM
script ID. All separately evaluated receiver and wire reads remain
at their original sites; no input clamp, extra ANM call or new
portrait array was introduced.

The two actually affected VC7.1 FrontMessageUpdate carrier objects
cold-replayed **2/2 existing exact owners, 3,007 physical PE bytes and
167 relocations** (2,968-byte complete switch owner and 39-byte
DecodeFrontMessageString leaf). 27 changed compiler-private jump
labels, including owner-local named dispatch labels, were independently
checked against actual COFF same-section definitions and unchanged
original PE operand addresses before refreshing only those manifest
spellings. This is native source semantics without an exact promotion
or whole-product claim.

## 2026-11-12 D3D startup capabilities formatter native byte cursor

The retail initialization reports Direct3D capabilities by repeatedly
writing to the same stack-backed output buffer. AppendD3DCapabilitySupportLine
and FormatD3DCapabilities advance their live char pointers by each
sprintf result. Eleven increments now form native unsigned i386
DWORD base + native byte-count addresses, rather than requiring
C++ pointer-in-array arithmetic for arbitrarily sized formatted text.
The unchanged sprintf arguments and order guarantee no new error
path, buffer guard, scratch allocation or external call.

Only the three affected pinned VC7.1 carriers cold-replayed:
**3/3 existing exact owners, 2,444 complete original PE bytes and 245
relocation fields**, including the 1,561-byte InitializeD3DRendering
boss, the 809-byte format routine and the 74-byte append leaf.
No private-label rebinding was required, and this does not establish
a whole-product Windows runtime or another newly exact function.

## 2026-11-12 FrontSide draw cursor recovers native ANM VM base

The 3,094-byte complete FrontSide::OnDraw boss walks real
0x2A4-byte AnmVm records while the local cursor points at each
VM's +0x208 position. The original native i386 code recovers
the owning AnmVm address by subtracting the byte offset
as a 32-bit word, not by performing C++ byte-pointer
arithmetic outside the interior Float3 member. The lexical
FRONT_SIDE_DRAW_BASE expression now explicitly forms the
actual owner address modulo 2^32. All eleven original
draw/update loops, GameManager transformations, backend draw
calls, and cursor widths remain unchanged.

The single affected pinned VC7.1 carrier cold-replayed
**1/1 exact FrontSide::OnDraw, 3,094 complete original PE bytes
and 209 independently bound relocation fields**. Neither
the compiler-private labels nor the exactness ledger required
changes. This is real native source-storage fidelity on an
already-exact rendering boss, not product/runtime proof.

## 2026-11-12 Front transition and result-index native DWORD arithmetic

The complete FrontCalcCallback controls the front overlay ANM VMs,
two panel color/timer rows, scene transitions, popup effect triggers
and original result-table publication. Six source expressions now
explicitly restore native modulo-32 i386 operations: dynamic
sprite ID + 82, each live signed panel-timer decrement, opposing
side subtraction, both 45*title + 5*stage result-table indices
(with the live mode term on the second), and the final signed
transitionFrame10960 increment. Signed branch guards and all target
calls remain in the original order; the existing exact native result
tables and GameManager side allocation are not replaced.

The one pinned VC7.1 original-bound FrontCalcCallback carrier
cold-replayed **1/1 existing exact owner, 1,491 complete physical
bytes and 107 relocation fields**. No manifest label rebindings
or new exactness credit were needed; the changed C++ avoids
overflow undefined behavior but retains the original wire-word
results and every physical instruction.

## 2026-11-12 MIDI timer, SysEx and tempo native DWORD arithmetic

The full MIDI ProcessMsg switch/dispatch owner handles file-backed
running-status messages, SysEx long-message slots and tempo changes.
Its signed pendingLongMessageHeaderCursor now increments as a native
i386 DWORD before the original modulo-32 slot selector. The signed
microsecondsPerQuarterNote accumulator uses 32-bit wrapping
multiply/add with live MIDI wire bytes; nextEventTick restores the
signed result of its native word addition. The signed fadeOutElapsedMs
counter in OnTimerElapsed also uses a wrapping DWORD lvalue. These
edits preserve MIDI event ordering, hardware calls, track cursors,
speed/volume controls and the actual 32-pointer header array.

The official pinned VC7.1 source-scoped replay passed **32/32 exact
owners, 4,270 complete physical original bytes and 132 relocations**
including the 1,574-byte ProcessMsg boss and MIDI timer/device leaves.
An additional unchanged one-byte MidiTimer::OnTimerElapsed virtual
owner shares the physical compiler object: independent proof of
**33/33 owners, 4,271 physical bytes and the same 132 relocations**.
The 23 renamed compiler-local ProcessMsg switch labels were rebound
only after validating real same-section COFF definitions, unchanged
PE jump-table targets, and complete bytes. These were existing exact
owners, not a new function promotion.

## 2026-11-12 Title options state and selection script DWORD arithmetic

The original 2,045-byte TitleScreenView::OnUpdateOptions menu owner
advances three persistent signed frame/phase counters and constructs
four live menu-VM script addresses from signed keyboardSelection + 9.
The source now uses native unsigned 32-bit word arithmetic, restoring
the signed index for the preexisting actual 0x2A4-byte ANM VM lookup,
and unsigned lvalue INC for optionsFrameCounter/stateTimer/stateTimer2.
No menu branch, audio call, or SFX state publication is changed.

The pinned /Ob1 VC7.1 carrier cold-replayed **2/2 existing exact
functions, 2,079 complete original bytes and 138 relocation fields**
(the 2,045-byte OnUpdateOptions and the 34-byte PlayMenuSound leaf).
A separate attempt to convert the help-text pointer-table calculation
to an integral base changed the owner length by two bytes and was
rejected before installation. This is source semantics for an
already-exact boss/leaf pair, not new exact credit or runtime closure.

## 2026-11-12 MusicRoom pause and state counters use native i386 words

The complete TitleScreen::OnUpdateMusicRoom owner runs the file-backed
music-comment parser, list management, BGM play/pause, animation calls,
and three persistent signed state counters. Retail computes the
musicPaused inversion as a native DWORD subtraction, and advances
stateTimer, screenFrameCounter, stateTimer2 with wrapping 32-bit
INC instructions. The maintained source now expresses those operations
without signed C++ overflow while preserving every original branch
and live input/memory access.

The sole affected pinned VC7.1 carrier cold-replayed **both existing
exact owners, 2,467 complete original bytes and 80 relocation fields**
(the 2,258-byte OnUpdateMusicRoom boss and 209-byte DrawMusicRoom leaf).
No new exactness credit is asserted. Eight separate and combined
native cursor-address probes, and a direct native song-track lookup,
preserved the owned extent but changed exactly four SIB address-encoding
bytes (+0x71F/+0x746/+0x755/+0x762). These source variants were
rejected and the correct compiled bytes restored; no target bytes
or relocations were altered.

## 2026-11-12 Title KeyConfig native frame and phase DWORD wrap

The exact TitleScreenView::OnUpdateKeyConfig code owns three persistent
signed counters: screenFrameCounter, stateTimer and stateTimer2. The
original i386 increments their full 32-bit words, even across signed
INT_MAX, while C++ signed increments would leave overflow undefined.
All three now advance through unsigned DWORD lvalues, without changing
the cursor/key-mapping modes or any update/VM calls.

The separate signed keyConfigSide and keyboardSelection expression
probes produced exactly two SIB-encoding byte differences at +0x82E
and +0x843 despite preserving the 118 original fields and 2,803-byte
extent; a 25-read native mapping-table expression changed another
eight register/SIB bytes. Those forms were rejected rather than
weakening exactness. Only the three counter expressions were installed.

The one affected pinned VC7.1 carrier cold-replayed **2/2 exact owners,
2,909 full original bytes and 118 relocations**, including the entire
2,803-byte key-config boss and its 106-byte SwapDuplicateCharacterIndex
helper. No new exact owner or Windows runtime proof follows.

## 2026-11-12 Background render variable-width quad record cursor

The full Background::RenderObjects stage renderer iterates file-backed
RawStageQuadBasic instructions of variable byte widths. The retail i386
cursor takes the signed 16-bit byteSize of the live quad, extends it,
then adds that DWORD to the 32-bit byte address of the current record.
The source now forms exactly that modulo-32 address instead of applying
C++ pointer arithmetic beyond the fixed 0x1C-byte quad projection.
The signed specialEffectPointCount increment uses an unsigned DWORD
lvalue as well, retaining the original wrap. Neither change adds a
record-bound check, alters the fog/projection callbacks, changes the
effect point storage or invents a new stage format.

One pinned VC7.1 compiler carrier cold-replayed **the complete 3,274-byte
RenderObjects boss and all 113 original-bound relocation fields**.
No compiler-private label rebindings were required. This is a genuine
native-storage/source-semantics refinement of an already-exact large
owner, not a new exactness promotion or runtime claim.

## 2026-11-12 FrontMessage script and portrait ID DWORD sums

The FrontMessageRuntimeView::Update interpreter publishes script and
sprite indices after combining signed side values, Player's live
tableValue303F8/tableValue303FC words, and file-backed portrait
parameters. Four expressions in its portrait/script switch paths now
explicitly form modulo-32 unsigned i386 sums and restore the signed
result before the existing ANM calls. This retains the repeated real
GameManager/Player reads, call chronology, branch gates, and the
original native portrait tables without clamping inputs.

The two actually affected VC7.1 FrontMessageUpdate carrier objects
replayed **2/2 exact owners, 3,007 physical bytes and 167 fields**
(2,968-byte script switch and 39-byte decoder). 24 compiler-local
jump/dispatch labels changed spellings, including the run_scripts
label. Each current same-section COFF definition and PE operand
destination and every original byte was independently compared
before changing only the symbol names in the exact manifest.
This repairs signed-wire source arithmetic in an already-exact
large boss; it does not promote a new exact function.

## 2026-11-12 FrontSide HUD live side and marker-count DWORD slots

The complete FrontSide::OnUpdate reads the current side's native
0x38-byte GameManager runtime record (the HUD rank count) and a
different two-entry marker-count DWORD table at GameManager +0x108.
The live sideIndexA66C is a signed native i386 word. These two reads
now use the already verified FRONT_NATIVE_SIDE lvalue and a direct
modulo-32 byte address from the real markerCounts108[0] member,
respectively, without imposing a bounded C++ array-index condition,
copying a count, or changing the rank/marker loops and ANM calls.

The single pinned FrontSide VC7.1 carrier passed **15/15 exact
functions, 4,087 complete physical PE bytes and 176 relocation
fields**, covering the full 2,381-byte OnUpdate boss and its create,
meter, transition and score leaves. No compiler-private label change
or new exactness credit was needed; this repairs source-level native
storage access, not an unproved product/runtime boundary.

## 2026-11-12 FrontMessage interpreter native packet stride and frame word

The complete FrontMessageRuntimeView::Update switch owner traverses a
file-backed variable-length command stream. The next instruction is a
native 32-bit address computed from the real argument04 byte prefix
plus the independently reread unsigned instruction size03. The packed
text handler likewise advances its short-argument payload by exactly
four bytes as a Win32 unsigned address. The signed framesElapsed1D4C
counter is now incremented as a DWORD lvalue, matching retail i386
wrap rather than invoking C++ signed-overflow semantics. No added
buffer bounds, side clamping, packet cache, or changed callback/VM
execution order is implied.

Only the two actual FrontMessageUpdate VC7.1 carriers were rebuilt:
the 2,968-byte complete script owner and 39-byte DecodeFrontMessageString
leaf are both **exact, 3,007 complete physical bytes and 167 relocation
fields**. 22 compiler-private COFF label spellings (including the
run_scripts local label) were independently proved by actual same-section
definitions, original PE operand targets, and every owner byte before
rebinding only their manifest names. The source improvement does not
promote a new exact function.

## 2026-11-12 Player collision CFG and Title result-byte trials

The original v1.50a Player::CheckBulletCollision at 0x0041DFF0 was
re-exported through Ghidra with every mapped PE byte reattested. The
original is 862 bytes, 277 instructions, 33 direct CFG blocks,
26 bound external fields and 12 ordered calls. The unchanged source
candidate is 849 bytes, 266 instructions, 32 blocks, all 26 fields
and 12 ordered calls, with 13 missing tail/extent bytes and 799 unlike
overlapping byte positions. The original has distinct rotated-rectangle
fallthrough and loop-back sites around 0x0041E07E / 0x0041E19E.
Tests replacing the native pointer loop with ordinary pointer increments,
positive type-selection predicates, reordered rotated bounds and
axis-aligned comparison spellings mostly compiled to that same 849-byte
candidate. The reversed rotated-order trial shortened it to 845 bytes;
the loop-break/while restructures changed the three original ret 12
exit contracts and were rejected. There is still one genuine original
CFG block to reconstruct; no padding, manufactured goto or byte credit
was installed.

The original TitleScreenView::OnUpdateResult at 0x004266B5 was also
re-exported from the attested target: 485 bytes, 147 instructions and
40 direct blocks. Its cursor-table byte supplies both the cursor call
and a signed selected-character publication. Ten focused VC7.1 source
trials compared a by-value char/signed-char/int snapshot, live signed
rereads, reference aliases, and post-call reloads. The candidate ranged
from 483 to 488 bytes; some variants reduced raw overlapping differences
by one while growing the owner and changing its complete offset layout.
None reproduced the complete 485-byte original, although 34 external
fields, nine calls and 40 CFG blocks stayed bound. The maintained
484-byte candidate still has 218 differing overlapping positions and
one missing byte. The remaining question is the distinct original
source-load/sign-extension schedule, not an unverified exact match.
All trial source edits were restored after the comparisons.

## 2026-11-12 GameManager cleanup stage/difficulty native word arithmetic

The exact CleanupGameplayState owner computes difficulty limits from
the live signed GameManager counterFC, stage value100, limitB4 and
valueCC words. Its per-mode 600-frame interval formulas, four direct
counterFC additions, final two-times-stage difficulty adjustment,
the mode-2 limit decrement and the post-cleanup score decrement now
explicitly reproduce native modulo-32 i386 arithmetic. The existing
signed threshold/maximum tests still see those resulting DWORD bits as
signed integers. No new branch, stage clamp, side state, allocation
or callback was introduced; the original input read and store order
is retained.

Only the two actually involved VC7.1 GameManagerSetup carriers were
cold-compiled. The exact replay passed **12/12 owners, 4,680 complete
original PE bytes and 415 relocation fields**, including the 425-byte
CleanupGameplayState, 1,230-byte OnUpdate and 1,689-byte
GameplaySetupThread. No compiler-private label changes were required.
This is a source semantic repair in existing exact code, not newly
promoted exactness or complete Windows product/runtime closure.

## 2026-11-12 SoundPlayer SFX producer native metadata/pan slots

The two independently exact SFX producers, PlaySoundByIdx and
PlaySoundPositionedByIdx, receive signed sound IDs and use the live
request-count word as a post-incremented pan-slot index. The native
8-byte g_SoundBufferIdxVol records, four-byte
unconsumedMetadataBySound slots and 12x128 four-byte pan table are
now addressed through the actual allocation bases and unsigned
DWORD index strides. This recovers retail i386 address formation
without inventing an index clamp, a shadow queue, or different
postincrement/callback behavior.

An attempted generic callable inline native-slot accessor enlarged
the PlaySoundByIdx owner by thirteen bytes and was **rejected**.
Seven isolated/combined lexical expression probes instead preserved
the full 140-byte PlaySoundByIdx and 153-byte positioned-sound owner,
with their reviewed relocations. The installed lexical version
also cold-replayed **all eight real SoundPlayer compiler carriers:
20/20 exact functions, 5,400 physical bytes and 184 relocation
fields**. The one new ProcessQueues compiler-private label was
independently proven against its actual same-section COFF definition
and original PE operand target before the manifest was updated.
The larger ProcessQueues callback stays exact. No previously nonexact
owner is promoted by this source-level native-address repair.

## 2026-11-12 Etama bullet transform and frame-counter DWORD wrap

The original EtamaController::OnUpdate owns the bullet/laser update
loop and transform-state dispatch. Its three live direction-change
completion increments, the bounce completion counter, the bullet's
offscreen-cull delay, the controller's spawn-suppression countdown
and the long-lived frame counter now use explicit unsigned
modulo-32 operations before restoring the original signed state.
The script-controlled transform repeat and bounce counts can carry
arbitrary signed words, so ordinary C++ signed overflow did not
faithfully express the existing retail INC/DEC instructions.
No new game rules, clamp, transition, callback or null check is added.

All three affected original VC7.1 BulletManager carriers cold-replay
**18/18 existing exact functions, 6,711 complete original physical
bytes and 259 relocations**, including the large Etama update/
SpawnSingleBullet functions and their leaf helpers. Sixteen private
$L label renames were each independently proven through their
actual COFF same-section definitions and original PE operand targets
before refreshing the canonical manifest spellings. This is
verified source meaning, not a newly exact owner or runtime closure.

## 2026-11-12 SoundPlayer native BGM format and preloaded-slot addressing

The original file-loaded BGM metadata has 0x34-byte
ThBgmFormatProcessView rows, and the SoundPlayer owns distinct native
preload-format, allocation, data, size and 0x100-byte filename slots.
GetFmtIndexByName, ReopenBGM, PreloadBGM, FreePreloadedBGM and LoadBGM
now form their actual signed runtime indices as unsigned 32-bit
base + uint32(index) * element-size addresses. A shared lexical inline
accessor preserves all original rereads, side effects, call chronology
and the live backing arrays without introducing new bounds checks,
shadow storage or a changed BGM state machine.

The pinned VC7.1 source-scoped cold replay rebuilt **eight genuine
SoundPlayer carriers**, verifying **20/20 exact owners, 5,400 complete
original bytes and 184 relocation fields**, including the 1,456-byte
ProcessQueues callback and relevant BGM leaves. Eleven renumbered
compiler-private labels were rebound only after independently verifying
their actual same-section COFF definitions, original PE operands and
every complete physical owner. This preserves existing codegen exactness;
the nonexact game functions and whole-product/runtime stages remain open.

## 2026-11-12 Enemy clamp, Player shot init and EX4 native vector fields

Three additional gameplay owners retain their **original external**
`Float3::operator float*()` / `PlayerPositionView::operator float*()`
conversion calls while replacing byte-adjacent `float*` indexing with
members of the same genuinely allocated three-float vector:
`EnemyView::ClampPositionToMovementBounds` clamps the Enemy's native
+0x2D74 x/y fields; `PlayerLifecycleView::InitializeShot` applies the
signed-format shot offset and 0.495f depth at shot +0x2A4; and the
739-byte EX-attack type 4 updater builds its live (12,12,0) collision
size in `PlayerPositionView`. The original conversion calls, exact
branch/callee order, signed comparisons and all allocated storage are
unchanged. No fallback, bounds check or replacement aggregate is added.

Only their three changed compiler carriers were rebuilt under pinned
VC7.1 and all **3/3 full original PE owners (1,175 bytes, 36 relocation
fields)** passed canonical byte/field comparison: Enemy clamp 127 bytes,
Player InitializeShot 309, EX-attack Type4 739. These were already exact;
this correction removes accidental C++ pointer-to-float-subobject array
assumptions and makes no new exact-function or whole-product claim.

## 2026-11-12 Native Title two-side cursor DWORD address, not scalar pointer-array

The original `TitleScreenView` stores `side0CharacterCursor` at +0x10 and
`side1CharacterCursor` at +0x14 as **two distinct `i32` fields**; they are
not the first two elements of a C++ array. Eleven uses in the three
`MoveCharacterCursor*` unlock walkers and the two horizontal cursor
routines formerly formed `&side0CharacterCursor + side` or `+ cursorIndex`.
Those expressions step out of an actual scalar subobject when the selector
is nonzero. The maintained source now forms the real Title-owner address
as `uint32(&side0CharacterCursor) + uint32(side) * 4`, preserving the
original Win32 modulo-32 selection for both live cursors. Both separate
input/cursor selector arguments, original cursor writes, unsigned native
increment/decrement, unlock lookups, sound calls and all loop tests remain
at their original sites; no new array, bounds gate or helper ABI was added.

The pinned VC7.1 two-carrier **41/41 complete exact Title owners** cold
replay passed all **18,972 original PE bytes and 953 relocation fields**.
Nine compiler-private `$L` spellings in `OnUpdateStartMenu` changed solely
from source renumbering, and each was proven from the actual same-section
COFF label definition to the unchanged PE operand and full owner before
rebinding. This strengthens the physical two-side cursor reconstruction,
not exact-function counts or Windows/runtime-product status.

## 2026-11-12 Native Title character-option matrix shared with selection boss

The original Title configuration owns one **16 x 6 x 4-byte** option-word
matrix at +`0x7C`, with 0x18-byte character rows. The independent exact
`TitleCharacterConfigView::GetOptionState` uses a signed `char` character
selector and signed runtime option number, while the full character-select
owner reads its fixed fifth option word for the selected signed character.
Both now share a single lexical native lvalue computing real base plus
`uint32(sign_extended_character)*0x18 + uint32(option)*4` modulo 2^32.
The new static layout check identifies the same backing allocation; no
second options array, new validation or cached snapshot is introduced.
All original branch gates, four independently reloaded options for a
negative setting selector, and character-selection UI calls remain intact.

The pinned VC7.1 two-carrier source replay proved **41/41 previously exact
Title owners / 18,972 complete physical bytes / 953 original-bound
relocation fields**, including the **2,241-byte** `OnUpdateCharacterSelect`
and the **54-byte** option getter. Nine compiler-private `$L` spellings
inside the exact start-menu owner were rebased only after proving actual
same-section COFF definitions, unchanged PE relocation targets, and every
full owner. This is native source-data fidelity, not new exact-owner or
Windows/runtime-product credit.

## 2026-11-12 ChargeAttack boss shared physical focus-mode byte lvalue

The nonexact **1,210-byte `PlayerLifecycleView::UpdateChargeAttack`**
reads one real `GameConfiguration::sideFocusMode` pair at `+0xB4` in three
separate gameplay branches. Those receiver expressions share a lexical
`TH09_CHARGE_NATIVE_FOCUS_MODE(index)` that forms the genuine Win32 byte
address from the original global GameConfiguration allocation. It does not
cache the read, add a range check, or merge the separate input conditions.
The three 0x8E-byte ReplayInputState slots, independently observed input
calls, charge sound/rate, timers, FrontSide callbacks and attack thresholds
remain unchanged.

A later fresh target decompile resolves the original local `v2` as the full
DWORD at `this+0x08` and the mode snapshot `v6` as one stack `char` at
`[ebp-1]`; the original prefix is `mov edi,[esi+8]; mov eax,edi;
imul eax,0x8E; lea ebx,[base+eax]`. The maintained source's signed-short
`side` is therefore a candidate-side alias, not evidence that the retail
local was 16-bit. Simply widening it to `int` is semantically closer to the
observed DWORD read but reverses the target EDI/EBX allocation and expands
the full-owner residual to 929 bytes. The original source-level local
spelling and register lifetime remain unresolved.

The affected pinned VC7.1 ChargeAttack compiler profile reproduces its
previous **1,210/1,210 physical bytes, 70 original relocation operands,
58 ordered calls and 53 reviewed control blocks**, with exactly the same
**235 ordinary unlike bytes**. This remains a nonexact boss; the work
recovers source-level data ownership and repeated wire-byte access, not
new exact-function credit. No whole-product/runtime claim is made.

## 2026-11-12 Title Result Draw original i386 fade alpha multiplication

The complete **1,939-byte exact `TitleScreenView::DrawResult`** renders all
five difficulty score rows from the real on-disk Rank table with a live
Title-owned signed fade frame `stateTimer2`. Its fading branch previously
multiplied this signed source word by 255 in C++, where extreme negative
frame values could overflow before the signed divide by 30. It now forms
the original i386 modulo-32-bit product as
`int(uint32(stateTimer2)*255u)/30` before the same signed alpha clamp
and the already authenticated unsigned high-byte D3D color packing.
All four title/rank display quadrants, real 0x2C-byte score rows, signed
name input cursor, ASCII text calls and native `stateTimer2 < 30` gate
retain their exact original read and call ordering. A separate experiment
to mask the runtime difficulty shift count explicitly changed the compiled
owner by three bytes and was discarded.

The one affected pinned VC7.1 source carrier replayed **1/1 previously
exact owner, 1,939 complete original PE bytes and all 82 relocation fields**.
No new exact-owner credit or complete Windows runtime/product claim follows.

## 2026-11-13 Title key-config 25 live controller side-row lookups

The **2,803-byte exact `TitleScreenView::OnUpdateKeyConfig`** uses one real
0x6C-byte serialized `TitleKeyMapping` union containing **two physical
0x36-byte `TitleKeySideMapping` records**. Twenty-five independent reads
and writes of `sides[keyConfigSide]` (initial display, repeated five-key
refresh, and the five duplicate-key remap branches) now share one lexical
`TH09_KEY_NATIVE_SIDE` row lvalue formed from the actual mapping union base
plus its signed native row selector. This does not materialize another
mapping allocation, cache the selector, invent a bounds gate or merge
successive memory accesses. Both key remap metadata and the externally
serialized GameConfiguration retain the same original physical storage.
The original key-swap calls, input chronology, 57 AnmVm owner records,
0x36-byte row layout and signed side-toggle/cursor DWORD writes remain.

One affected pinned VC7.1 key-config carrier replayed **2/2 preexisting
exact functions, 2,909 full original PE bytes and 118 relocations**,
including the 2,803-byte boss and 106-byte duplicate-index leaf. No new
exact-function count or complete Windows product claim follows.

## 2026-11-12 Title key-config boss's native side and menu cursor DWORD writes

The exact **2,803-byte `TitleScreenView::OnUpdateKeyConfig`** owns one
actual signed `keyConfigSide` DWORD at Title+0xC920, and an independent
signed `keyboardSelection` DWORD. Both left/right side-selection handlers
perform the original i386 `1-keyConfigSide` subtraction through the
**real unsigned DWORD lvalue** of that original signed word. The scrolling
menu's repeated `keyboardSelection+=MoveCursorVertical(10)` likewise adds
through its actual DWORD before retaining the existing signed range checks.
No mirrored selectors, extra bounds, derived state or new C++ array object
is introduced; the two real 0x36-byte controller mapping side records,
0x2A4-byte help VMs, settings byte lanes and original controller checks
remain authoritative.

Generic `static_cast<i32>(1u-uint32(side))` and value-returning unsigned
addition trial forms produced **two changed bytes** in VC7.1's otherwise
exact OnUpdateKeyConfig and were explicitly rejected. The original lvalue
form preserves both native wrap semantics and original compiler register
schedule. The single affected pinned VC7.1 carrier replayed **2/2 existing
exact functions, 2,909 original PE bytes and all 118 relocations**, including
the entire key-config boss and the 106-byte duplicate key mapping helper.
No new exact-owner or Windows runtime/product credit is claimed.

## 2026-11-12 Bullet transform interpreter's authentic packed child-spawn bytes

`Bullet::AdvanceTransformProgram` is an exact **1,320-byte physical
interpreter** for the 18 actual 0x18-byte `BulletTransformRecord` entries.
Its child-pattern opcode packs bullet type and color into bytes 2 and 1
of the *same real DWORD* `record->payload.raw.int0`. Both field reads now
form their original Win32 byte addresses from the genuine integer member
`uint32(&int0)+uint32(lane)`, instead of projecting an independently owned
`unsigned char[4]` through the scalar. The real bullet template selection,
transform flags, count/speed/angle payloads, following 0x18-byte record,
18-record `memcpy` and `SpawnBulletPatternPrimary` call order remain intact.
No new parser object, bounds gate, cached input, or callable opcode boundary
was added.

The affected pinned VC7.1 compiler carrier replayed **1/1 previously
exact owner, 1,320 complete original PE bytes and 26 relocation fields**.
The compiler renamed two private `$L` references; their same-section
actual COFF definitions, source-to-PE offsets and entire physical owner
were independently proven before only manifest spellings were changed.
This is source-native script payload recovery, not a new exact-function
or complete Windows product claim.

## 2026-11-13 Player death and FrontFactory Calc share authentic side panel DWORDs

The original **753-byte `PlayerLifecycleView::EnterDeathState`** writes
a 60-frame hit-flash timer and 0x80FF0000 panel color into the
*same physical FrontFactory allocation* used by the exact **1,491-byte
FrontCalcCallback**. Their live signed side selectors now share the
`FrontNativePanelState.hpp` source-facing native DWORD lvalues rooted in
the actual FrontFactory+0xE93C (two timers) and +0xE934 (two colors).
Both use `uint32(sideIndex)*sizeof(int)` over the original two-word
arrays, rather than having separately bounded C++ side arrays in the
Player source and native address math in the FrontFactory source.
No second panel state, hidden cap, alternate player side or new callable
method was introduced. The death callback's independent hit-effect,
SoundPlayer, CardAttack, Player flags, panel animation, and 0x38-byte
GameManager side calls retain their original chronology and semantics.

The two actually affected pinned VC7.1 carriers passed **2/2 existing
exact functions, 2,244 complete original PE bytes and all 145 relocation
fields**. Neither needed compiler-private `$L` rebasing. This ties a large
gameplay death handler to the original FrontFactory state owner, not
new exact-function count or a whole native Windows runtime claim.

## 2026-11-13 FrontFactory's three callbacks share two physical panel vertex strips

The exact **706-byte `FrontAddedCallback`**, **1,491-byte
`FrontCalcCallback`** and **1,474-byte `FrontDrawCallback`** operate on one
0x1293C-byte `FrontFactoryConstructView` allocation, with two real panels
of five **0x14-byte `FrontPanelVertexView` records** each at owner+0xE86C.
The new `FrontNativePanelVertices.hpp` supplies one actual Win32
`uint32(&panelVerticesE86C[0][0])+uint32(side)*0x64+uint32(vertex)*0x14`
lvalue instead of independent construction/draw/runtime vertex addressing
models. AddedCallback now writes the live `rhw` and `diffuse` words through
this genuine storage in both dynamic initialization loops, and all 28
physical corner coordinate writes and duplicate closing vertex copies
select the same real records. The runtime CalcCallback retains its
original dynamic side/vertex indices with the identical preexisting
physical address expression, and DrawCallback passes the two actual
line-strip starts to `FrontDrawLineStrip` without copying vertices.
No synthetic panel storage, signed-index range guard, extra function,
changed vertex constructor, or modified drawing order was introduced.

Only the three affected pinned VC7.1 source carriers were recompiled;
**3/3 previously exact owners, 3,671 original PE bytes and all 271
relocation fields** passed a complete function comparison. No private
`$L` reference needed rebinding. The exact 1,491-byte calculation and
1,474-byte drawing bosses remain byte-identical, not newly counted exact
functions. This is cross-callback native allocation and source fidelity;
no complete Windows runtime or product claim follows.

## 2026-11-12 FrontFactory calculation/draw callback native wire and stage arithmetic

The independently exact **1,491-byte FrontCalcCallback** handles end-of-stage
transitions and consumes a genuinely file-backed neutral message source
through two live 16-bit words at original Win32 byte offsets `+8` and
`+10`. Both source reads now form `uint32(g_NeutralMessageSource)+8u/+10u`
before casting to their actual wire `short` lvalues; the prior C++ byte
pointer arithmetic imposed a non-evidenced array extent on an opaque
message record. The message setup calls, side/transition conditions,
actual FrontFactory state and byte order are unchanged. A shift-count
mask trial in the transition flag expression altered three bytes of the
physical owner and was rejected rather than changing the existing match.

The independently exact **1,474-byte FrontDrawCallback** displays the
stage number with the original i386 wrapping `g_GameStageValue+1` DWORD,
then passes the signed value to the exact `AsciiManager::AddFormatText`
call. Its existing genuine panel vertices, real 0x2A4 ANM VMs, score
positions and other formatting calls remain unchanged.

The two affected pinned VC7.1 carriers replayed **2/2 previously exact
owners, 2,965 original PE bytes and 234 relocation fields**. No new
exact-function or whole Windows product/runtime credit follows; this
is native wire-pointer and integer-source fidelity in both large callbacks.

## 2026-11-12 ExAttack Type18/24 actual trail positions and angles across update phases

The exact **1,436-byte `ExAttackUpdateCallbackType18_24`** owns one actual
0x158-byte `ExAttackType1824UpdateExtra` record, with sixteen 0x0C-byte
`Float3` trail positions at +0x58 and sixteen four-byte angle words at
+0x118. The existing native trail lvalues already handled the variable
initialization/shifting loop. Eight fixed uses at indices 0, 1, 3, 5
(including zeroing the newest position and assigning three ANM VM `pos2`
/rotation pairs) now reuse those **same physical `TH09_EX_TRAIL_POS` and
`TH09_EX_TRAIL_ANGLE` lvalues**, removing two competing address/ownership
idioms inside the big state-machine callback. All observed individual
Float3 operations, timer tests, 0x2A4-byte VMs, reflection, homing collision
calls and original sequencing are retained without a second trail buffer.

A separate attempted rewrite of the previous trail element as
`NativeTrail(i-1)` produced a 1,435-byte candidate and was rejected; the
true original compiler shape retains the explicit native byte-minus-stride
`TH09_EX_TRAIL_PREV` expression for that loop. The one pinned VC7.1 carrier
passed the full original-bound replay of **1/1 preexisting exact owner,
1,436 complete PE bytes and all 44 relocated fields**. This is source
layout fidelity, not new exact-function or complete Windows runtime credit.

## 2026-11-12 Shared native Player active collision-region slot cursor

The original `PlayerLifecycleView` allocation holds an `activeB100` chain
of live `PlayerCollisionRegionCreateRegionView*` words, consumed by two
independent collision calculations. The exact **996-byte
`CalcDamageToEnemy`** and the nonexact **862-byte target
`CheckBulletCollision`** now share `PlayerNativeCollisionSlots.hpp`, a
single lexical `TH09_PLAYER_NATIVE_NEXT_REGION_SLOT(slot)` lvalue that
advances the genuine pointer slot by its 4-byte native i386 stride.
This preserves repeated live region dereferences, existing signed gates,
circular/rotated AABB tests and direct reward/effect calls; no array copy,
bounds check, alternate list owner, pointer cache or new function boundary
was introduced. The source-recovered address is the actual Player-owned
chain, not a second projected collision-region array.

Four affected pinned VC7.1 compiler profiles replayed **7/7 existing
exact canonical functions, 2,271 complete original PE bytes and all 53
relocation operands**, including CalcDamageToEnemy and the Graze, laser,
item and query support leaves. The separately reconstructed
`CheckBulletCollision` was also rebuilt: it retained its prior
**849-byte candidate vs 862-byte original, 26 relocations, 12 ordered
calls and 32 vs 33 CFG blocks**, with no exactness credit and no regression
(13 missing bytes, 799 unequal positions). No complete Windows runtime or
newly exact function is claimed.

## 2026-11-13 Recording, release and save share actual Replay stage-root records

The TH09 `ReplayManagerView` allocation contains exactly ten inline
0x18-byte `ReplayBufferLink` roots beginning at manager +0x1C. The two
independently exact **1,323-byte `BeginRecordingStage`** and **318-byte
`Release`** methods, plus the nearly exact 1,238-byte `SaveReplay`, now
use the same `ReplayNativeStageRoots.hpp` source-facing lvalue for signed
stage/index selections. The accessor calculates
`uint32(&manager->stageBuffers[0])+uint32(index)*sizeof(ReplayBufferLink)`
over the *real* 0x164-byte manager allocation; the first/second recording
root snapshots remain distinct, and the serializer continues to use
immutable source links while release mutates and frees its current chain.
No fake stage table, bounds gate, new helper call, cached link head or change
to the ten-stage record layout was introduced.

The **two affected pinned VC7.1 ReplayInput carriers** passed complete
scoped comparison for **12/12 previously exact owners, 4,093 original PE
bytes and all 232 relocations**. The separate SaveReplay boss was rebuilt
and checked: **1,238/1,238 physical bytes, 53 fields, 23 ordered calls and
exactly its previous two register-byte differences** (+0xB7/+0xBB).
This is cross-boss data-ownership fidelity rather than new exact-function
or complete Windows product credit.

## 2026-11-12 Shared physical Title replay record addressing in both renderers

The independent `TitleScreenView::DrawReplaySave` and
`TitleScreenView::DrawReplayMenu` renderers use the *same actual* 50-record
`TitleScreenView::replays` storage, rooted at Title +0x68E4 with a verified
`ReplayDataView` stride of **0x1EC bytes**. The new
`TitleReplayNativeRecords.hpp` provides one lexical
`TH09_TITLE_NATIVE_REPLAY_RECORD(owner,index)` lvalue, preserving signed
runtime selectors as native 32-bit byte offsets without a second replay
record allocation. Four independent replay header reads in the save screen
and sixteen independent reads in the menu screen now share exactly this
one authentic owner and no extra snapshot, check, cached magic, or callable
helper boundary. The existing `NativeMenuFrameStart` reads still select
real 0x20-byte stage headers from each original ReplayData record.

One pinned VC7.1 replay-save-draw carrier cold-replayed **1/1 previously
exact function, 800 original PE bytes and 41 relocations**. The separate
nonexact replay-menu boss was also actually rebuilt: it retained its
**1,280-byte physical extent, 57 bound fields, 13 ordered calls, reviewed
control graph and the same 29 unlike ordinary bytes**, with no regression
and **no claimed exactness credit** for that boss. Neither renderer's text,
frame, animation or selection chronology changed. This is big-function
cross-owner source fidelity, not a whole-product/runtime result.

## 2026-11-12 Unified authentic six-bucket Etama draw-head ownership across update and draw

`EtamaController::OnUpdate` and `EtamaController::OnDraw` share the **same
six contiguous `Bullet *` draw-head words** in the real controller allocation.
A new lightweight `EtamaNativeDrawBuckets.hpp` now supplies their single
`TH09_ETAMA_NATIVE_DRAW_BUCKET(owner,index)` original Win32 byte-address
lvalue. This replaces the duplicated local macro in the **2,216-byte
physical update boss** and the independently written lookup in the
**600-byte draw boss**. The selected bucket remains the true on-wire byte
in the Bullet VM; the native index multiplies the actual pointer stride,
with no bounds gate, new storage, second owner, or cached head. OnUpdate
preserves its distinct live reads/writes and linked-list append sequence;
OnDraw preserves the original six-bucket enumeration, real laser geometry,
`DrawSingleBullet`, effect-layer calls and exact interleaving.

The **four pinned VC7.1 affected carriers** replayed **21/21 preexisting
canonical exact functions, 8,375 complete original PE bytes and all
332 relocation fields**. The 3-byte compiler/SDK view, excluded from exact
function credit, was additionally independently proven in the 4-owner COFF
original-bound audit (1,667 bytes/73 fields) of the complete drawing
carrier. Both update and draw proof scripts independently verified the
original PE destinations, byte ranges and definitions for **27 renumbered
private `$L` references** before updating only manifest spellings.
This removes divergent reconstruction models for the same game allocation;
it is not a new exact-function or complete Windows runtime claim.

## 2026-11-12 D3D startup's three physical 0xF0 Supervisor viewports

The exact **1,561-byte InitializeD3DRendering** creates the original D3D8
device, checks refresh/vsync and capabilities, and initializes precisely
three actual `Supervisor::ctorArray078` viewport records: the two game
sides and the global 640x480 scene. All **33 live member assignments**
(camera `Float3` position/look/up, FOV, viewport dimensions/depth and
mode) now reuse a single native Win32 lvalue formed from the **real
Supervisor +0x78 array** and 0xF0-byte element stride. The source asserts
that actual allocation contains three records, that each record is 0xF0
bytes, and that the first begins at +0x78. Those are the same viewport
fields consumed by the separately exact Supervisor, ScreenEffect, and
background render owners. The original execution order, float constructors,
D3D present parameters, adapter capability query and all error fallbacks
are unchanged, with no duplicated viewport array or new bounds gate.

The three affected pinned VC7.1 carriers replayed **3/3 previously exact
owners, 2,444 original PE bytes and all 245 relocation fields**, including
the big D3D initialization function and both exact capability formatter
leaves. This is authentic Win32 source layout, not new exact-function,
full executable, or runtime credit.

## 2026-11-13 Ending script and AddedCallback share authentic ANM VM ownership

The exact **1,727-byte `Ending::RunEndingScript`** and independent
**800-byte `Ending::AddedCallback`** both operate on the same genuine
16-element `Ending::endingVms` array, with authentic 0x2A4-byte record
strides at Ending+0x14. Their native i386 byte-address lvalue is now
shared through `EndingNativeVm.hpp`, not maintained independently as
one exact interpreter macro plus separate unchecked C++ array accesses.
Both live VM reads in AddedCallback (the `ExecuteAnmIdx` receiver argument
and position write) retain their original scheduling. The callback also
uses the actual `Supervisor::textAnm5CC` field rather than a second
`EndingSupervisorTextAnmView` allocation, and assigns the original
`AnmVm::pos` member rather than a 0x208-byte opaque prefix cast. Its
character selector now reads the true file-backed 16-DWORD
`g_EndingFilePaths` table through native Win32 base-plus-index addressing,
with no extra bounds gate or file-path cache.

The **three affected pinned VC7.1 carriers** replayed **11/11 already
exact owners, 3,815 complete original PE bytes and all 244 relocations**,
including the large Ending interpreter, its script/timer/alpha leaves,
Ending constructor and AddedCallback. Compiler-private `$L` spellings
changed in 31 Ending functions' relocation occurrences; each actual
same-section COFF definition, original PE destination and complete physical
owner was independently verified before changing only manifest labels.
This is cross-function native data ownership recovery, not newly exact
function credit or whole Windows runtime completion.

## 2026-11-13 Etama DrawSingleBullet's actual ANM VM type word

The exact **232-byte physical `DrawSingleBullet`** callback checks the
active ANM VM's native signed `type` at byte offset **+0x1FC** before
setting its Z rotation. The maintained source previously read a `short`
through a cast from the nominal `unknown_01FC` byte prefix. It now
uses the *same real* `AnmVm::type` 16-bit union member that the exact
ANM script interpreter sets. This preserves the original test, call
ordering, two exact D3DX float-vector conversions, real bullet/VM
positions and native color update; no alternative VM allocation or
artificial byte-array owner is introduced.

The sole affected pinned VC7.1 Etama drawing carrier replayed **3/3
previously exact functions, 1,664 full original PE bytes and all 73
relocations**, including the 600-byte OnDraw boss and 832-byte AddedCallback.
Its separate noncrediting three-byte SDK conversion view was independently
checked as part of the **1,667-byte four-owner COFF/PE proof**.
Eleven compiler-private `$L` spelling updates were allowed only after
verifying current same-section definitions, original PE destinations,
relocation addends and entire physical owner bytes. No new exact-function
or whole Windows runtime claim follows.

## 2026-11-12 Etama big OnUpdate's authentic six-bucket draw heads

The **2,216-byte physical `EtamaController::OnUpdate`** owns two bullet
pools and a separate **six-element `Bullet *drawBuckets` array** in the
same genuine EtamaController allocation. The paused-side append path
and ordinary fired-bullet path now use one lexical native Win32 lvalue
`uint32(&controller->drawBuckets[0]) +
uint32(bullet->sprites.drawBucketIndex)*sizeof(Bullet*)` for all four
independent draw-head loads/stores. This lvalue preserves the original
per-bullet byte selector, native DWORD wrapping, old-head link order,
current-bucket writes and iteration of real Bullet records. No shadow
buckets, copied heads, bounds guard, new call boundary or altered sprite
program was added.

The independently exact **283-byte `EtamaController::Register`** uses that
same native signed side word for its two Chain scheduling priorities,
with modulo-32 sums `sideIndex+12` and `sideIndex+21`. The existing real
0x38-byte GameManager side lookups, opposing side, allocation, chain
registration and both callbacks are unchanged.

Three affected pinned VC7.1 Etama carriers passed full source-scoped
replay of **18/18 pre-existing exact owners, 6,711 original PE bytes and
259 relocation fields**, including complete OnUpdate, SpawnSingleBullet,
transform leaves and both pattern-generation functions. The compiler
renumbered three private `$L` references; their actual same-section
COFF definitions, target PE destinations and complete physical owners
were independently verified before changing only manifest spellings.
No new exact-function credit or complete Windows product is implied.

## 2026-11-12 FrontSide status VM position/color and HUD rank word ownership

The full **2,381-byte `FrontSide::OnUpdate`** HUD display boss updates four
flashing character/health sprite colors from three separate loops. Their
`5-i` dynamic selectors now reuse the already validated native 0x2A4-byte
`FRONT_NATIVE_VM` lvalue into the genuine 11-element `statusVms0000` array,
instead of a second projected C++ subscript. The `AsciiGameManagerSideView`
side-runtime `counter14` sprite offset now adds 8 as an i386 unsigned DWORD
before the same signed SetSprite argument, avoiding signed overflow at the
source boundary while preserving every live read and call order.

Its independent **460-byte `FrontSide::AddedCallback`** initializes and
copies `pos` to `pos2` through the *same* authentic VM lvalue used for its
script construction, not a separately projected pointer for the assignment.
Both records remain true fields of the one original 0x2A4-byte `AnmVm` with
position +0x208 and position2 +0x288, rather than new temporary arrays.
No color caching, status bounds guard, animation change or new callable
boundary is introduced.

The one affected pinned VC7.1 `FrontSide.obj` carrier passed **15/15
pre-existing exact owners, 4,087 full original PE bytes and 176 relocation
fields**, including both big OnUpdate and added-callback functions plus
supporting leaf callbacks. This improves native ownership fidelity without
new exact-function or complete Windows product credit.

## 2026-11-12 SampleNetworkInput native keyboard and controller-mode byte lanes

The **1,507-byte original-bound SampleNetworkInput** function scans WinMM
and DirectInput keyboard state from the **same genuine 256-byte stack buffer**.
Its two keyboard families use over a hundred independent scan-code reads;
all now select bytes by native `uint32(&keyboardState[0])+uint32(key)` rather
than treating unrelated input API metadata as an independent C++ array.
The input source remains the exact GetKeyboardState/DirectInput GetDeviceState
backing buffer, with no key filtering, new buffer or changed held/pressed
state timing. Three separate branch selectors now share the real Supervisor
GameConfiguration controller-assignment bytes at +0x43F/+0x440 through
one physical native byte lvalue. The output remains the authentic
three-record 0x8E-byte ReplayInputState allocation, with its original
16-word repeat counters and signed short keys unchanged.

The sole pinned VC7.1 compiler carrier reproduced **1/1 previously exact
owner, 1,507 complete original PE bytes and all original relocations**, with
the full branch graph, input calls and original keyboard/controller path
ordering preserved. No additional exact-function or Windows product credit.

## 2026-11-12 Supervisor 517-byte FPS sampler native clock and stack float words

The **517-byte original-bound `SupervisorCheckFps`** times the real
Direct3D scene/present loop with native WinMM `DWORD` milliseconds. Its
window frame counter and in-window frame count now advance as original
wrapping i386 DWORDs, and the elapsed-time calculation explicitly keeps
the modulo-2^32 clock subtraction before its original *signed* threshold
checks. Both the sample write and subsequent average use native four-byte
addresses computed from the **actual 30-float local stack array**, not a
replacement history or one-element projected object. Original render,
`timeBeginPeriod`/`timeGetTime`, 500/700-ms gates, 57/65/160-fps tests,
logging and forced vsync actions remain at their exact original sites.

Only its one pinned VC7.1 source carrier was recompiled and compared:
**517/517 complete original PE bytes, all relocations and exact callback
order** passed. This is native source semantics in an already exact
supervisor calibration callback; no new exact-function or product claim.

## 2026-11-12 Background native stage file, side and spell VM ownership

The Background resource path now uses the original TH09 allocation/address
rules in several independently exact functions. In `Background::Create`,
both playable-side receivers are selected from the **one real pair of
0x38-byte GameManager side records** with modulo-32-bit index and
`1-viewportIndex` arithmetic. `Background::AddedCallback` selects the
real `g_BackgroundStageFiles` table's **0x08-byte two-pointer rows**, retaining
each live ANM/STD path read, resource gate and original stage load sequence.

`Background::LoadStageData` is the **321-byte original-bound STD file loader**.
Its serialized object offsets, `next` quad byte sizes, object-instance and
script pointers are now represented as true file-backed Win32 DWORD base +
offset addresses rather than relying on unrelated C++ array pointer ranges.
Its runtime `stageObjectVms` allocation remains the actual `quadCount`-sized
0x2A4-byte ANM records. The variable quad VM lookup and the `OnUpdate` /
`OnDrawLowPrio` spell VM traversal now share native byte-stride addressing
over their **separate genuine 32-element embedded spell VM array**.
The four `OnUpdate` persistent signed stage/transition/spell/frame counters
now use their original wrapping i386 DWORD increments/decrements; the
`ZunTimer` methods are unchanged. No clamping, shadow arrays, new file
loads, changed VM calls, or interpreter boundary was introduced.

Three actually affected pinned VC7.1 compiler carriers replayed **9/9
previously exact functions, 2,828 complete original PE bytes and all
171 relocations**, including the stage loader, creator, added callback,
update and both draw callbacks. This is authentic native data layout and
file-pointer source fidelity, not a new exact-function or Windows product
claim.

## 2026-11-12 Title StartMenu's native help text and demo replay pointer rows

The exact **1,592-byte physical `TitleScreenView::OnUpdateStartMenu`**
reads the real eight-element `g_TitleHelpText` pointer table and the actual
three-element `g_DemoReplayPaths` pointer table at their original global
storage addresses. Two dynamic selector reads now use native Win32
`uint32(base)+uint32(index)*sizeof(pointer)` addressing instead of a C++
array subscript on the terminal `stateTimer2`/demo byte. This preserves
the separately observed live text/ANM calls, real `embeddedVms` storage,
recorded input and cursor chronology, demo index modulo 3, and original
idle/transition logic, without an extra bounds guard or path copy.

The two affected pinned VC7.1 Title carriers passed the full scoped replay
of **41/41 previously exact canonical owners, 18,972 complete original
PE bytes and all 953 relocations**, including the StartMenu boss and
its associated Character/Score/Title leaves. Nine compiler-private `$L`
labels changed; all nine current same-section COFF definitions, their
independently determined original PE destinations and complete physical
owner bytes were verified before rebinding manifest *names*. The separate
nonexact `OnUpdateResult` remains **484 candidate vs 485 original bytes,
218 unlike bytes and one missing byte, 34 fields**. No new exact-function
or Windows runtime/product credit follows.

## 2026-11-12 Supervisor frame callbacks use exact AnmManager surface owners

The 517-byte `SupervisorCheckFps` and 312-byte `SupervisorDrawMain` originally
called original ANM methods through two temporary unrelated
`Supervisor*AnmManagerView` source projections. Both now call the real
`AnmManager::CopySurfaceToBackbuffer` (original PE `0x0043BFA0`), while
the DrawMain branch additionally calls the real
`AnmManager::ReleaseSurface` (`0x0043BF30`). Both destinations already
have canonical exact ANM-manager owners in the original-bound ledger.
The DrawMain `GetSurface(8)` receiver remains the previously recovered
17-byte independent view because its nominal original method owner is
unresolved; after that call, the *same* live returned manager receiver is
cast to its genuine `AnmManager*` for `ReleaseSurface`, avoiding a second
read of the global pointer and reproducing original register scheduling.

Three relocation entries were bound from their old provisional view names
to the **already proven canonical owner symbols**, with identical
`REL32` kinds, physical offsets, original PE targets and addends. The
actual VC7.1 COFFs, complete original PE bytes and all fields were
independently checked rather than changing binary addresses to fit.
The two affected carriers passed **2/2 complete exact owners, 829 original
PE bytes, 55 original-bound relocation fields**. No new exact-owner credit,
new game subsystem ABI, or whole Windows runtime is claimed.

## 2026-11-12 ANM release of true texture entries and 0x120-byte clearing window

The exact 163-byte `AnmManager::ReleaseAnm` is a game resource callback
on the authentic 0x120-byte manager load-slot allocation, not a duplicate
ANM resource-table object. Its runtime-counted 0x0C-byte texture entries
now reuse `NativeAnmTextureEntry(&slot->loaded, i)`, the same real
texture backing pointer and unsigned Win32 byte stride used by the loader.
The signed texture index advances as a real wrapping i386 DWORD while
retaining its original signed bound test and the ordered ReleaseAnmEntry
calls. The original final `memset` deliberately starts at the slot's
`+0x24` loaded-member address and clears **0x120 bytes**, not merely the
0x1C-byte `AnmLoaded` subobject. Its pointer is now expressed from the
full slot's native address + `offsetof(...,loaded)`, without shortening,
clamping or redirecting this observed cross-subobject clearing range.
All four original resource frees, null gate, and record ownership remain.

The two affected pinned VC7.1 carriers passed local exact replay for
**17/17 pre-existing exact owners, 3,258 complete original PE bytes and
133 relocation fields**, including the loader's constructor and all
16 ANM resource-load/release functions. No private label changes were
required and no newly exact owner or whole-Windows runtime credit follows.

## 2026-11-13 SoundPlayer live twelve-slot SFX queue at native DWORD addresses

The two separately exact SFX producer leaves, the **140-byte
`SoundPlayer::PlaySoundByIdx`** and **153-byte
`SoundPlayer::PlaySoundPositionedByIdx`**, append commands to one real
12-element `SoundPlayer::soundQueue` DWORD array, with parallel 12-element
request counts and real 12x128 pan words in the same 0x6224-byte owner.
Their six independent selected-command comparisons/stores now share
`NativeSoundQueue(this,i)`, which forms the physical Win32
`uint32(&soundQueue[0]) + uint32(i)*4` lvalue without a second queue,
cached slot, new bounds gate or change to the original queue chronology.
The two functions still use their separately observed live per-sound
metadata, 0x80 count cap and native panning words. The same accessor
also reproduces the exact **245-byte `InitSoundBuffers`** initial queue
clear. However the initialization loop in the exact **613-byte
`InitializeDSound`** depends on the original C++ array expression for
VC7.1's scheduling; forcing the helper there shortens the compiled owner.
That trial was rejected, keeping its original exact machine-code shape.

The eight affected pinned VC7.1 SoundPlayer carriers passed **20/20
already exact owners, 5,400 complete original PE bytes and 184 relocation
fields**, including the 1,456-byte ProcessQueues boss and queue producer,
DSound initialization, WAV loader and supporting leaves. Eleven private
`$L` labels in ProcessQueues were updated only after a fresh source-driven
compile independently confirmed their real same-section definitions,
original PE branch destinations and complete physical owner. No new
exact-function or complete Windows product/runtime claim is implied.

## 2026-11-13 SoundPlayer native 128-slot DirectSound duplicate pointer ownership

The exact 245-byte `SoundPlayer::InitSoundBuffers` builds 54 hardware/volume
pairs from original process-global 0x08-byte SFX metadata records, then
publishes the duplicate DirectSound buffers in the authentic 128-pointer
`SoundPlayer::duplicateSoundBuffers` array. Its three separate live
read/write receivers now share `NativeDuplicateSound(this,i)`, formed from
the genuine full SoundPlayer allocation and native Win32 `uint32(i)*4`
byte stride. This complements the already original-bound `NativeActiveSound`
primary buffer lookup; it does not allocate a shadow array, cache the
selected pointer or introduce bounds gates. Retaining distinct reads into
`DuplicateSoundBuffer`, `SetCurrentPosition` and `SetVolume` preserves their
original side effects and ordering.

The equivalent accessor is **intentionally not applied in the exact
232-byte `Release` loop**: natural VC7.1 compilation there produced
234/247 bytes instead of its exact 232-byte original, even though the
underlying addresses were identical. Both rejected Release edits were
restored; the existing cleanup order remains exact. The eight affected
pinned VC7.1 SoundPlayer carriers replayed **20/20 pre-existing exact
owners, 5,400 original PE bytes and 184 relocations**, including the
1,456-byte ProcessQueues boss and independent playback/WAV leaves.
The compiler renumbered 11 private `$L` labels solely inside ProcessQueues;
their current same-section COFF definitions and complete original PE
branches were independently proven before changing manifest spellings.
This is native data ownership recovery, not new exact-function or Windows
product/runtime credit.

## 2026-11-12 Shared native SoundPlayer SFX paths and 0x08-byte volume rows

`SoundPlayer::InitSoundBuffers` owns an exact 245-byte controller that loads
the 39 actual SFX path pointers and creates 54 original DirectSound duplicate
buffers from the true 0x08-byte `SoundBufferIdxVolume` records. Both repeated
path reads and the buffer-index/volume field reads now share actual original
Win32 memory addresses (`base + uint32(index)*sizeof(row)`), rather than
requiring a separate bounded C++ projected path or metadata table.
The same native row accessor is reused **inside the existing 1,456-byte
SoundPlayer::ProcessQueues boss**, at the original SFX volume calculation
site. Its queued commands, 12x128 native pan words, request counts, original
DirectSound calls and live snapshot/read order are unchanged. There are
no new arrays, signed-index gates, caches, functions or audio timing paths.

Only the eight affected pinned VC7.1 SoundPlayer carriers were replayed:
**20/20 previously exact source-present owners, 5,400 complete original PE
bytes and 184 relocated fields** passed, including the 245-byte initializer,
1,456-byte physical ProcessQueues and independent leaf callbacks.
The inline accessor changed 11 compiler-private `$L` label spellings in
ProcessQueues. They were applied only after independent proof of all
same-section COFF definitions, their original PE destinations, and the
entire physical function. This is stronger original memory-ownership
semantics, not new exact-function or whole-product credit.

## 2026-11-12 Player Shot Type2 even-trail index and loop DWORD arithmetic

The **643-byte exact PlayerShotUpdateCallbackType2** draws collision regions
for the authentic `PlayerShotCtorView`'s 32 embedded 0x0C-byte trail vectors,
using the shot-owned `field46A` count. Its signed `i` counter now advances
through the original unsigned 32-bit `INC DWORD` lvalue, preserving the
comparison and the original behavior even for a malformed/wrapped count.
Likewise, the second loop's even-sample index is calculated as
`uint32(i)*2` *before* scaling by the genuine 0x0C-byte record stride,
instead of relying on signed-C++ `i*2` overflow rules. The real trail pool,
position/velocity, ZunTimer calls, original CreateRectType4 receiver,
collision region geometry and historical 31-to-1 shift all remain intact.

The pinned VC7.1 one-carrier scoped replay proved **1/1 existing exact owner,
643 complete original PE bytes and 16 relocation fields**. Attempts to
project the whole history as a separate pointer/array and to rewrite the
fixed descending loop changed original control-flow/codegen and were
rejected; only the proven native arithmetic changes are committed. No new
exact-function, full build or runtime claim follows.

## 2026-11-12 WAVE memory-stream leaves use native Win32 cursor words

The independently exact `WaveFileProcessView::ResetFile` and
`WaveFileProcessView::Read` consume the actual file-backed WAVE buffer
owned by the original sound-stream manager, not a synthetic fixed-size
C++ array. The in-memory ResetFile loop intro now advances `dataCursor`
through its true Win32 byte address `uint32(dataCursor)+introLength`.
The Read path compares the original native cursor-plus-request address
against the actual buffer-end word, calculates the remaining bytes from
`uint32(dataCursor)-uint32(data)`, and advances that same real cursor by
the effective read size. This removes out-of-array pointer arithmetic
and non-array pointer subtraction while preserving the original i386
wrap, signed/unsigned comparison semantics and byte-for-byte Windows
code generation. No new file-size gate, memory copy, buffer allocation
or DirectSound call was introduced.

The pinned VC7.1 source-scoped cold replay covered only the **five affected
SoundManager carriers** and passed **25/25 previously exact owners,
4,004 complete original PE bytes and 80 relocation fields**. This includes
the **215-byte ResetFile** and **218-byte Read** leaves and the exact
sound-buffer/streaming callers. The DSUtil SDK manager core's third-party
provenance is unchanged; this is source fidelity in the target-specific
file-backed WAVE extension, not new exact-function or full product credit.

## 2026-11-13 Player selector's fixed and dynamic half-size vectors share stack rows

The exact **3,238-byte PlayerUpdateSelectorState** gameplay-pattern boss
constructs three genuine 0x0C-byte `PlayerConstructedVector3View` objects
on its own stack, then views them as `PlayerPositionView` hitbox extents.
Seven fixed accesses in the initial half-size initialization and later
collision-query branches now use the *same*
`TH09_PATTERN_NATIVE_HALF_SIZE(index)` real Win32 base-plus-0x0C-byte
stride already used by the dynamic 0..2 selector. This unifies the
source-facing allocation/alias model without a new bounds gate, second
vector array, cached dimensions or helper call. The original 16-bit
pattern input flags, timer thresholds, collision geometry, vector copy
and RNG/control-flow ordering remain unchanged; the grid cursor still
uses its independently observed original pointer increment.

The two affected pinned VC7.1 compiler carriers replayed **2/2 existing
exact owners, 3,650 complete original PE bytes and 154 relocation fields**,
including the giant selector and its 412-byte `ResolvePatternOffset` leaf.
Two private `$L` spellings in the leaf were rebound only after proof of
the current COFF section definitions and their original PE target words.
This is original-source memory ownership fidelity, not a newly exact
function or complete Windows runtime claim.

## 2026-11-12 Player selector's three native stack collision radius words

The **3,238-byte** exact `PlayerUpdateSelectorState` gameplay boss builds
three real `PlayerConstructedVector3View` half-size vectors next to a
three-float local collision radius allocation (`48.0f`, `16.0f`, `0.0f`).
Two candidate-pattern collision queries previously used direct C++
`radii[halfSizeIndex]` and `radii[testHalfSize]`. These now read the actual
stack float words via the original Win32 address `uint32(&radii[0]) +
uint32(index) * sizeof(float)`, with each live index and original call
order retained. The fallback grid traversal also advances `gridRadius`
through its true four-byte word stride as a 32-bit byte address, in parallel
with the separately genuine 0x0C-byte half-size vector stride.

There is no replacement radius allocation, bounds check, cache, altered
collision decision or changed RNG/option geometry. The two affected
pinned VC7.1 profiles reproduced **2/2 existing exact canonical owners,
3,650 complete original PE bytes and all 154 relocation fields**, including
the whole selector boss and 412-byte `ResolvePatternOffset` leaf. Two
compiler-private `$L` names in that exact leaf changed with source lines;
their real same-section COFF definitions, original PE destinations and
complete owner bodies were proven independently before manifest updates.
This is source-level native stack-layout recovery, not added exact-owner
or Windows product/runtime credit.

## 2026-11-12 Post-ECL movement's real Float3 z component after conversion

The unsplit `EclManager.cpp` translation unit owns both the **15,564-byte
RunEcl** and the exact `EnemyPostEclUpdateMovement` alongside Enemy calc.
In the polar-velocity handler, the original VC7.1 executable makes a
`Float3::operator float*()` conversion and then stores a zero at the *third*
component of the native `EnemyView::velocity2D8C` (offset +0x2D94).
The maintained source now retains the exact conversion invocation but
interprets its result as the original real `Float3*` and writes the `z`
member; it no longer treats a returned pointer to the first float as an
independently owned `float[3]`. This preserves all x/y polar motion,
movement-angle updates, timer/deceleration calls, and live field rereads.
An initial `float *` temporary declaration was rejected by the pinned
compiler's case-label lifetime checks, and replaced with the verified
single expression rather than introducing a different function boundary.

Only the two affected pinned VC7.1 carriers were replayed: **83/83 previously
exact owners, 29,856 complete original PE bytes and 1,389 relocation fields**
passed, including the entire Post-ECL movement owner. Eight compiler-private
`$L` label spellings changed, but every same-section COFF definition,
original PE destination and entire owner body was independently proved
before updating manifest *names* alone. The co-compiled RunEcl retains its
**30 ordinary differences / 598 fields**, and Enemy OnUpdate retains its
**13 differences / 97 fields / 69 calls**; this source change is not a new
exact owner or complete Windows runtime credit.

## 2026-11-13 Music-room parser authentic dynamic descriptor rows and counters

The **2,258-byte exact `TitleScreenView::OnUpdateMusicRoom`** parses
file-backed track paths, titles and description lines into the *real*
0x292-byte `MusicRoomTrackDescriptor` records in its full TitleScreen
allocation. Six dynamic path/title/description writes and selected-song
playback reads now use the same verified `TH09_MUSIC_NATIVE_TRACK(index)`
lvalue as the other music-room consumers. The original run-time track
number, three separate `charIndex` increments, seven-line description
counter, displayed `vmIndex`, and row-Y progression now advance the
existing signed native i386 **DWORD** storage with modulo-32 arithmetic.
The actual descriptor cursor walks native `uint32(musicTrack)+0x292` byte
addresses, never a shadow C++ track array. Every live string read, original
file-size/line parser gate, unlock flag, `DrawTextLeft` and audio call stays
at the same point. An attempt to replace the inner 66-byte description
line's native indexing with a second projected row changed codegen and
was rejected.

The one affected pinned VC7.1 carrier replayed **2/2 previously exact
functions, 2,467 original PE bytes and all 80 relocations**, including
the update boss and independently exact 209-byte music-room Draw leaf.
This is source-level file/data and overflow fidelity, not a new exact
function or full Windows runtime/product claim.

## 2026-11-12 Music-room comment parser's native file byte offsets

The **2,258-byte** exact `TitleScreenView::OnUpdateMusicRoom` parser reads
variable-length comments from the real file loaded by `FileSystem::OpenFile`.
Seven live cursor-vs-file-start comparisons now calculate their original
i386 byte displacement as `uint32(cursor)-uint32(musicCmtFile)` modulo
2^32. The previous C++ pointer subtraction was defined only while the
cursor remained within a common array; the native file parser's signed
source and return gates are unaffected. The original 32 actual 0x292-byte
music descriptor slots, file size gates, newline conditions, live cursor
increments and string publication order are unchanged. A companion trial
replaced eight pointer increments with native integer-address increments;
VC7.1 then produced a 2,258-byte target vs 2,254-byte candidate and that
trial was rejected.

The one affected pinned VC7.1 profile replayed **2/2 previously exact
owners, 2,467 complete original PE bytes and 80 relocation fields**, including
all 2,258 physical music-room update bytes and the independent 209-byte
music-room renderer. No new exact-function or Windows product credit follows.

## 2026-11-13 Stage-script boss's unified authentic STD wire operands

The complete **2,652-byte `BackgroundRunStageScriptPhase`** interprets
STD camera, fog, interpolation, sprite and stage-control opcodes directly
from a file-backed sequence of original 0x14-byte instruction headers.
The live instruction's operand prefix begins at **+0x08**, with physical
DWORD/float strides. Twenty-seven direct integer operands, four direct
floating operands, one packed byte operand, and **twenty-one complete
12-byte `Float3`/SDK `D3DXVECTOR3` reads** now share authentic native
`uint32(instruction)+offsetof(RawStageInstrProbe,args)+uint32(index)*4`
(or + byte index / unchanged vector start) lvalues. The previous source
mixed `instruction->args[0..2]`, a pointer to an embedded array, and
unrelated byte/float casts for those very same wire bytes. Its maintained
source now makes the single real allocation boundary and layout explicit,
without allocating a second payload, caching operands, checking unobserved
bounds or changing file-cursor chronology.

The original switch cases, all camera interpolation/`ZunTimer` calls,
Hermite/x87 math, 0xF0-byte native Supervisor viewport access, fog color
writes, live STD instruction-index increment and owned jump tables remain
unchanged. The sole affected pinned VC7.1 carrier passed **all 5/5
previously exact functions, 3,231 original PE bytes and 160 relocation
fields**, including the big interpreter, interpolation helper and timer
leaves. Forty-eight compiler-private `$L` reference names were rebound
only after checking the real same-section definitions, original PE
operand destinations and whole physical source/target bodies. No new
exact-function or complete Windows runtime credit follows.

## 2026-11-12 Background RenderObjects native color lanes in both quad paths

The **3,274-byte exact Background::RenderObjects** boss traverses file-backed
stage quad records and uses two actual native 32-bit colors: the drawn
ANM VM's `color1F0` and the stage `skyFog.color`. Twenty-three
fog-interpolation channel reads in the projected-quad and strip-quad
branches now use the genuine source DWORD's +0/+1/+2/+3 bytes rather
than treating a DWORD field as a separately allocated `BackgroundRenderColor`
union. Four whole-color reads similarly use the original VM DWORD directly.
The first path's interpolated color is mutated as four byte lanes of the
**same** VM DWORD, not a separate C++ overlay object; the path still restores
its saved original DWORD after drawing. The real vertex-local diffuse
colors remain actual `BackgroundRenderColor` values with their original
component assignments and drawing chronology unchanged. A source layout
assertion verifies the exact 4-byte color and byte lane order.

The pinned VC7.1 single affected carrier replayed the complete
**3,274/3,274 original PE bytes and all relocations** of `RenderObjects`.
There are no additional alpha caches, clamps, buffers, helper calls, or
changes to x87 interpolation, quad dispatch, D3DX calls or switch ownership.
This improves the original data aliasing fidelity of a large already-exact
renderer; it is not new exact-function or whole-Windows-build credit.

## 2026-11-13 Title options volume digit VMs share real 0x2A4-byte records

The **2,045-byte exact `TitleScreenView::OnUpdateOptions`** owns the actual
TitleScreen VM pointer at +0x11B98, referencing 0x2A4-byte `AnmVm`
records. Its six fixed sprite indices 26..31 are the separate music/SFX
volume high/low digits and associated display markers. All **28 independent
member reads/writes** (`baseSpriteIndex`, `color1Script.a`, `drawEnabled`
and `SetSprite` receivers) now use the same existing native Win32
`TH09_OPTIONS_VM(index)` lvalue rather than a second bounded typed-array
lookup model. The dynamic help-text VM selectors, real `AnmVm` storage,
signed volume comparisons, 8-bit i386 wrap writes and original audio/menu
input scheduling remain unchanged. No array copy, cache, hidden range check,
new setter or alternate font/ANM manager owner has been added.

The one affected pinned VC7.1 compiler carrier reproduced **2/2
pre-existing exact functions, 2,079 complete original PE bytes and all
138 relocation fields**: the full Options state-machine boss and its
34-byte PlayMenuSound leaf. No compiler-private `$L` labels changed,
and no new exact-function or whole Windows runtime is claimed.

## 2026-11-12 Title options signed-volume byte writes use native i386 arithmetic

The 2,045-byte exact `TitleScreenView::OnUpdateOptions` changes actual
`Supervisor::config388` music/SFX byte volumes. Four +/-4 adjustments and
four guarded one-step increments/decrements previously wrote through a
`signed char &` reinterpretation. Retail writes the **same native byte**
with i386 wrapping `ADD/SUB/INC/DEC`; the maintained source now uses the
unsigned 8-bit lvalue of each real `GameConfiguration` member for those
physical stores, avoiding implementation-dependent signed-narrowing
arithmetic. All *reads* and clamp comparisons keep the original signed
`int8` meaning (0/100, negative, overflow), with unmodified input/menu
selection, SoundPlayer calls and exact interrupt/VM field writes.

The one affected pinned VC7.1 carrier passed the full source-scoped
original-bound replay for **2/2 canonical exact owners, 2,079 complete
PE bytes and all original relocations**, including the entire 2,045-byte
options boss and the 34-byte `PlayMenuSound` leaf. This is authentic
byte-storage semantics, not new exact-function or product credit.

## 2026-11-12 Enemy OnUpdate hit-flash writes to authentic color2 DWORD

The 3,900-byte source-present nonexact `EnemyManagerView::OnUpdate`
writes the RGBA hit-flash color to the one actual `AnmVm::color2` DWORD
owned by `EnemyView::primaryAnmVm0008`. Four lvalues which projected
`unsigned char[4]` over that scalar now address the real Win32 word base
plus native byte offsets 2, 1, 0 and 3. The live alpha source remains
the high byte of the *other* actual `color1` word, with unchanged sound,
flash timer, VM flags, field rereads, and write chronology. No duplicate
palette, byte-array storage, new clipping, or helper call is introduced.

The original-bound whole-boss diagnostic still reproduces **3,900/3,900
physical bytes, 97 fields, 69 original ordered calls, identical reviewed
control graph and exactly the previous 13 ordinary register differences**.
The co-compiled 15,564-byte `RunEcl` retained its 598 field operands,
owned tables and the 30 previously isolated differences (opcodes 4/155).
The two selected pinned VC7.1 carriers cold-replayed **83/83 previously
exact sibling functions, 29,856 physical original bytes and 1,389
relocations**. Two changed compiler-private `$L` spellings were applied
to the manifest only after independent same-section COFF/PE target and
complete-owner byte proof. This is big-boss source ownership fidelity,
not a newly exact function or whole executable product claim.

## 2026-11-13 Enemy calc and draw share one authentic four-head allocation

The nonexact but full **3,900-byte `EnemyManagerView::OnUpdate`** and
independently exact **1,758-byte `EnemyManagerDrawImpl`** operate on the
same four `EnemyView*` linked-list heads beginning at manager+0x2AC410.
The maintained source now uses `EnemyNativeDrawHeads.hpp` for a single
native Win32 `uint32(&drawGroupHeads2AC410[0])+uint32(group)*4` lvalue.
The template/view parameter preserves each original consumer's pointer
representation, without a second draw-only array or a new callable helper.
OnUpdate's two live append sites still obtain a pointer to the actual
head slot; the DrawImpl group loop reads that same physical word as an
`EnemyDrawView*`. The four head resets, animation VM selection, linked
list order, trail rendering and both enemy chain priorities are unchanged.
No new group bounds gate, cached list head or additional list allocation
was introduced.

The three affected pinned VC7.1 carriers (EclManager, its Type11 profile,
and EnemyManagerDraw) replayed **87/87 preexisting exact owners, 31,778
complete original PE bytes and 1,450 relocation fields**, including the
entire 1,758-byte DrawImpl and 83 exact ECL/Enemy support leaves. Twenty
compiler-private `$L` label references were changed only after proving
each current same-section COFF definition and its independent original PE
destination against complete function bytes. The nonexact OnUpdate was
also rebuilt: **3,900/3,900 physical bytes, 97 bound relocation fields,
69 ordered calls and precisely 13 remaining register-code differences**,
unchanged from baseline. This is cross-boss source-level ownership
recovery, **not** new exact-function credit or a whole Windows runtime.

## 2026-11-12 Enemy OnUpdate unified native draw-group head addresses

`EnemyManagerView::OnUpdate @ 0x00410730` stores the visible enemies in the
one actual four-pointer draw-head allocation at manager+`0x2AC410`.
Both the restricted-side/hidden-visibility path and ordinary active-enemy
path now use one inlined native address computation: authentic first head
address plus `uint32(drawGroup3387) * sizeof(EnemyView*)` on the original
32-bit i386 word. The draw group is a live unsigned byte on each path,
not a checked C++ subscript or a cloned table; the original append order,
branch tests, enemy linked-list updates and all field rereads are retained.
No new callable function boundary, validation or shadow allocation is added.

The complete nonexact **3,900-byte Enemy OnUpdate** still has exactly its
previous **13 ordinary differences**, all **97 original-bound relocation
fields** and **69 ordered calls** (same CFG and switch destinations).
The co-visible **15,564-byte RunEcl** still has exactly its prior **30**
differences (7 in opcode 4, 23 in opcode 155), **598** fields and both switch
tables intact. The pinned /Ob0 and /Ob1 VC7.1 source-scoped replay passed
**83/83 previously exact sibling owners, 29,856 physical bytes and 1,389
fields**. Only two private `$L` references changed numbering, and each was
proven from actual same-section COFF definitions, unchanged original PE
operands and complete linked bytes before updating the manifest spelling.
This is source-level native owner consolidation, not new exact owner credit.

## 2026-11-12 ANM 9,358-byte interpreter native jump payload words

`AnmManager::ExecuteScript` executes variable-size ANM file records, not
heap-allocated fixed `intArgs[10]` objects. Six independent live script
DWORD reads in ordinary jump, conditional jump and decrement/jump handlers
now address the genuine file-backed operand bytes via the original 32-bit
`instruction + offsetof(AnmRawInstr, intArgs) + uint32(index)*4`. The
original interpreter's signed branch inputs, timer assignments, subroutine
calls, continue targets, bit masks and shared jump tail remain unchanged.
There is no new bounds guard, operand snapshot, buffer or invented callable
opcode boundary; each argument is read at its original use site.

The pinned VC7.1 two-carrier target-bound proof reproduced **5/5 existing
exact units, 10,496 complete original PE bytes and 421 relocation fields**,
including every **9,358 byte** of the single massive ANM interpreter. A
source-line change renumbered 101 compiler-private `$L` references in that
large switch and its four exact TU leaves; the manifest was updated only
after independently checking each actual same-section COFF definition,
original PE target and every full owner, then scoped cold-replaying both
actual compiler profiles. This improves original wire/address semantics,
not the number of byte-exact functions or the whole Windows build status.

## 2026-11-12 Player state-4 and shot update native XYZ members

The complete `PlayerCheckState4` state transition clamps the live
Player+0x1B88 position after calling its actual `Float3::operator float*()`.
The independent `PlayerLifecycleView::UpdateShots` likewise advances
all 128 shot records through their real `Float3` positions at shot+0x2A4
and tests each projected x/y against the playfield. Both source bodies
previously used `[0]`/`[1]` through the conversion's `float*`, treating the
first float subobject as if it were a three-element float array. They now
preserve the conversion call **and** access the genuine `Float3` x/y
members. No shot allocation, update cadence, clamp, call order or branch
has changed. `UpdateShots` also retains its independently bound y-lvalue,
without caching a new source value.

The five affected pinned-VC7.1 source carriers passed a complete scoped
cold replay of **21/21 existing exact PlayerRuntime owners / 5,391 original
bytes / 288 relocation fields**. The 400-byte state-4 transition and the
251-byte UpdateShots are both fully byte/field exact. Eleven private `$L`
spellings shifted in another owner solely due to the compiler's source
numbering; before rebinding these the proof verified actual same-section
COFF label definitions, unchanged PE operands and all 21 complete bodies.
The separately nonexact Player movement, charge attack and bullet-collision
owners were not promoted by these source semantics changes.

## 2026-11-12 Etama draw authentic SDK vector component lvalues

`DrawSingleBullet` selects one of the original five bullet ANM VMs,
then calls both retail `D3DXVECTOR3::operator float*()` conversions: the
actual bullet position and selected VM position. The source used the
returned x-float pointer as an ersatz three-element `float` array. It now
reinterprets each unchanged conversion result as a pointer to the genuine
12-byte `D3DXVECTOR3`, and names `x`, `y`, and `z` when writing projected
popup coordinates and the 0.05f depth. The original callback/callee ABI,
D3DX layout, live reads, component order and target values are retained.

One VC7.1 Etama drawing carrier rebuilt and independently verified **three
complete canonical owners, 1,664 original-bound bytes and 73 relocation
fields**, including both `DrawSingleBullet` and the entire
`EtamaController::OnDraw`. Eleven purely compiler-private `$L` references
were rebound only after proving each actual same-section COFF definition,
original PE destination and unchanged complete bytes. The fourth support
unit in that object (`etama-sdk-vector-pointer-identity-view`) is not in
the canonical exact cohort and was not credited. No new exact function or
Windows runtime-product closure is inferred.

## 2026-11-12 Etama update original Float3 collision coordinates

Two original-bound BulletManager consumers call the real, independently
exact `Float3::operator float*()` at the same site as the retail game:
`UpdateBulletBoundaryBounce` (the boundary wrap helper) and the full
`EtamaController::OnUpdate` boss. Both previously indexed `position[1]`
through the pointer returned to the x float subobject, which unnecessarily
relied on treating a single float as an array. The source now casts that
unchanged live conversion result to the **existing** `Float3` allocation
and reads `.x`/`.y` directly. There is no new vector, no call removed,
no field snapshot and no new collision gate; x/y retain their observed
0x00/0x04 layout and all original call arguments.

The pinned VC7.1 exact replay rebuilt only **three BulletManager carriers**
and proved **18/18 complete owners, 6,711 original bytes and 259 relocation
fields**. That includes the full **2,216-byte physical Etama OnUpdate**
and the 113-byte boundary helper. Sixteen private `$L` spellings changed
in the two large BulletManager functions; each renamed actual same-section
COFF definition was independently proved against its original PE operand
and the *entire* related owner before updating only its manifest spelling.
This is a real native-source semantic recovery inside previously exact
functions, not new exact-function or Windows/runtime-product credit.

## 2026-11-12 SoundPlayer original SFX mapping-table receiver

`SoundPlayer::InitSoundBuffers @ 0x0043EC80` duplicates 54 DirectSound
buffers selected by the *signed* `bufferIdx` field of the actual 8-byte
`g_SoundBufferIdxVol` records. The selected index is not the loop ordinal.
Its native source now uses the same `NativeActiveSound` lvalue as `LoadSound`,
computing `uint32(&soundBuffers[0]) + uint32(bufferIdx) * 4` rather than a
potentially out-of-bounds C++ array subscript. The genuine mapping table,
original live lookup, volume, `DuplicateSoundBuffer` receiver and all calls
remain untouched. The accessor definition was moved before its new caller;
no duplicate buffer storage or index guard was introduced.

Pinned VC7.1 proved the complete **245/245-byte InitSoundBuffers owner and
all eight original-bound relocation fields**. The affected one-file scoped
replay passed **all 20 canonical SoundPlayer owners / 5,400 physical bytes /
184 fields in eight compiler carriers**. This improves native source semantics;
there is no new exact-function or runtime-product claim.

## 2026-11-12 SoundPlayer file-backed WAV cursor and chunk word arithmetic

SoundPlayer::LoadSound parses original RIFF/WAVE file data as an i386
byte stream. Its three successive 4-byte header cursor advances,
the GetWavFormatData length header at +4 and data at +8, and the
second locked DirectSound buffer copy now form unsigned modulo-32
native addresses from their real source pointers. The format scanner
also subtracts each signed on-disk chunk length plus eight as an
unsigned DWORD and advances the cursor by that same native word,
rather than invoking C++ signed overflow for unusual chunk lengths.
These are ordinary retail operations: no new bounds checks, caps,
allocation, format repair or error branches have been added.

The eight actual VC7.1 SoundPlayer compiler carriers cold-replayed
**20/20 existing exact owners, 5,400 complete physical bytes and
184 relocation fields**, including the 1,456-byte ProcessQueues
owner and both BGM/WAV file handling paths. The eleven changed
compiler-private $L spellings were independently verified against
actual same-section COFF definitions and original-PE target
addresses before updating only manifest symbol names. No new
exact owner or executable runtime closure is claimed.

## 2026-11-12 Exact 1,939-byte result-renderer native color and cursor words

The existing complete `TitleScreenView::DrawResult @ 0x00423D16`
owner calculates the active difficulty's mask, packs its replay/name-entry
alpha into the high byte of each display color, and draws five live
ranking tables. Its full result-renderer body now uses the actual Win32
unsigned DWORD for its mask shift and **25** alpha-mask shifts: retail
`SHL` permits setting bit 31, which signed source left shifts could
otherwise make undefined. The renderer also uses unsigned 32-bit
addition/multiplication before signed conversion in **ten** highlight
cursor-offset evaluations across all five difficulty panels. Original
layout, cursor reads, floats, call chronology, colors and normal inputs are
unchanged; no alternative tables, branches or clipping are introduced.

The pinned VC7.1 one-carrier replay of the complete **1,939-byte**
DrawResult owner passed **all 82 original-target relocation fields** and
full byte equality after the combined change. This is a native-expression
correction within a previously exact large function; the independent
nonexact Title OnUpdateResult remains open. C++ shift counts outside the
language range are *not* claimed portable even though retail x86 masks CL.

## 2026-11-12 MIDI 1,574-byte ProcessMsg native channel key lanes and endian words

`MidiOutput::ProcessMsg` consumes the genuine 0x17-byte `MidiChannel`
records in its source-facing 16-channel allocation. On NOTE ON and NOTE OFF,
the eight-bit translated note selects one bit in the real 16-byte
`keyPressedFlags` array. Both branches now calculate the live slot from
the actual channel bitmap byte base plus `uint32(note) >> 3`, rather than
using a second projected array index. The final source uses byte-pointer
addition over the authentic bitmap, preserving VC7.1's original
EDX/ECX/EAX ordering; an integer-base variant was explicitly rejected
because it changed 22 ordinary opcode bytes. There is no extra check,
buffer, changed MIDI channel, note transpose, or event ordering.

The two exact endian conversion leaves `MidiOutput::Ntohs` and `Ntohl`
now read byte 0..1 of the real on-stack 16-bit word and byte 0..3 of the
32-bit word through the original Win32 byte address. They retain the
original temporary destination bytes, swap order and result reads.

The one affected pinned VC7.1 MIDI carrier passed the full scoped replay
of **32/32 canonical exact owners, 4,270 complete original PE bytes and
132 relocation fields**, including all **1,574 physical bytes** of
`ProcessMsg` and both 18-/38-byte endian leaves. The same actual object
contains the separately noncrediting 1-byte MidiTimer callback (33 physical
units, 4,271 bytes), checked by an independent COFF/PE proof. Twenty-three
compiler-private `$L` labels in ProcessMsg were rebound only after proving
the actual same-section COFF definitions, original PE destinations and
complete owner bytes. No additional exact-function or Windows product
credit follows.

## 2026-11-12 FrontSide 3,094-byte HUD draw owner-based position cursors

The full `FrontSide::OnDraw` boss walks multiple native ANM VM arrays and
single instances in its actual HUD allocation, drawing 0x2A4-byte VM
records from the corresponding embedded +0x208 position cursor. The
existing `FRONT_SIDE_DRAW_CURSOR` constructed a projected 0x2A4-byte
cursor pointer from `&(vm->pos)` — a C++ Float3 subobject, not an owning
full record. Its address is now derived as `uint32(real AnmVm owner) +
offsetof(AnmVm,pos)` modulo 2^32, before the unchanged lexical position
window projects `pos` and `pos2` at the original +0x80 spacing.

This preserves all actual cursor advances, original `Draw2D` and
`DrawNoRotation` receiver reconstruction, player-relative HUD offsets,
local vertex construction, Direct3D state calls and floating-point draw
ordering. Neither a shadow VM array nor an additional branch/callee has
been introduced. The pinned VC7.1 one-carrier scoped replay proved the
entire **3,094-byte physical FrontSide draw owner and its original-bound
relocation fields** still byte-exact. No new exact-function or whole
Windows runtime credit is implied.

## 2026-11-12 Controller native replay input, device and joystick slots

The exact `Controller::GetControllerInput` owner receives two signed
runtime selectors, the replay input slot and the hardware pad. The
source now resolves the original 0x8E-byte `ReplayInputState` source
from the real three-record `g_ReplayInputStates` allocation instead of
a potentially out-of-range C++ array expression. Four live DirectInput
poll/acquire/state receivers use the real two `IDirectInputDevice8A*`
slots at `Supervisor+0x14/+0x18` via original Win32 DWORD-scaled pad
addresses. Twelve WinMM threshold/capability reads likewise address the
two authentic process-global JOYCAPSA records with their true record
stride. Both callback systems, retry logic, button order and original
threshold tests are unchanged; no fallback or new device list is created.

The independent `SetButtonFromDirectInputState` leaf now reads the true
DirectInput `rgbButtons` byte buffer at `uint32(base)+uint32(int16(button))`
after the original negative-index check. Both repeated live reads remain
separate, with no upper-bound cap. An experiment to explicitly mask the
joystick bit-shift count changed the other exact leaf's extent and was
rejected. The pinned VC7.1 one-carrier scoped replay passed **all 3
previously exact owners / 892 complete original PE bytes / all original
relocation fields**, including 808-byte GetControllerInput and the
37-/47-byte helper leaves. No new exact-function or product credit follows.

## 2026-11-12 Enemy 1,758-byte draw boss authentic alpha-byte receivers

The retained **1,758-byte** `EnemyManagerDrawImpl` controls both high-/low-
priority Enemy drawing callbacks, including the two different trail
rendering paths. Eight saved-VM/vertex color alpha reads and writes now
share one native lvalue accessing byte three of the **actual 32-bit
color word** at the original Win32 address `uint32(&color)+3`. These are
three distinct real storage owners: live primary ANM VM `color1`, the
local `savedColor`, and the true 0x1C-byte trail vertices' `diffuse10`.
The prior `reinterpret_cast<u8 *>(&color)[3]` imposed an array fiction
on a scalar subobject. The new formulation keeps every original live
read, assignment, fade formula and VM/vertex ownership without additional
buffers, bounds guards, changes to the float trail math or callable code.

The pinned VC7.1 one-carrier scoped replay verified **all 4 canonical
EnemyManagerDraw functions, 1,922 complete original PE bytes and all 61
relocation fields**, including all 1,758 bytes of the draw boss, both
high-/low-priority wrappers and the 91-byte angle interpolation leaf. This is stronger native C++ source
semantics, not additional exact-function or Windows runtime credit.

## 2026-11-13 GameManager native spawn-credit threshold deduction

The exact **1,230-byte `GameManagerSetupLayout::OnUpdate`** owns the real
signed `g_GameManager.valueCC` spawn-credit accumulator at +0xCC. Its
periodic enemy spawn branch redeems precisely 10,000 credits from that
existing physical DWORD. The source now performs the original wrapping
i386 `SUB DWORD` via its real unsigned word lvalue instead of relying on
undefined signed C++ arithmetic in `valueCC -= 10000`. All original
spawn-lane selection, independent RNG reads, dual 0x38-byte GameManager
sides, two enemy spawn calls, sound, and threshold checks are unchanged.
No new resource allocation, accumulator copy, clamp, or branch is added.

The affected pinned VC7.1 GameManagerSetup carriers passed scoped replay of
**12/12 pre-existing exact owners, 4,680 complete original PE bytes and
415 relocation fields**, including the 1,230-byte update boss, 1,689-byte
gameplay setup thread, and the 100-byte reset leaf. This restores signed
storage's native i386 behavior without adding an exact mapping or proving
a whole Windows runtime.

## 2026-11-12 GameManager gameplay setup physical side and focus slots

The complete **1,689-byte GameplaySetupThread** initializes two playable
GameManager side records from the real `GameManagerSetupLayout` and the
separately allocated `GameConfiguration`. Its per-side focus-reset byte now
uses the real `configurationE8->sideFocusMode[0]` byte address plus the
native 32-bit unsigned loop index, matching the direct Win32 lookup in
independently exact Player/Replay owners. No additional clamp or stale
cached focus field is introduced; only the original two playable sides
are passed to the Player/Bullet/Background constructors.

The exact **100-byte ResetGameManager** separately visits *three* physical
0x38-byte side records at +0x00, +0x38 and +0x70: the third contains the
shared EffectManager receiver, not a third playable Player. Its original
index loop now uses the same authentic passed GameManager allocation base
and `uint32(i) * sizeof(SetupSideState)` native byte stride as the existing
setup and cleanup paths. These are real allocation members, not a fourth
side, a replacement table or a change in reset chronology.

The two affected pinned VC7.1 profiles passed the complete scoped comparison
of **12/12 previously exact GameManagerSetup owners, 4,680 original PE bytes
and 415 relocation fields**. The 1,689-byte thread, 100-byte reset, other
GameManager callbacks and Supervisor loading owner all retained byte/field
exactness. This improves native source/address semantics rather than
adding an exact function or establishing a Windows runtime product.

## 2026-11-12 Ending interpreter and fade callback native DWORD arithmetic

The exact `Ending::RunEndingScript` owner is **1,727 complete physical
bytes**, including its real switch table and file-backed command cursor.
Its persistent page/line skip-lock counters, next-text-VM index and
script text-buffer cursor are signed native words, but the original i386
`DEC/INC/ADD` executes modulo 2^32. The maintained source now decrements
the two independently owned lock counters and increments the text-row word
through their unsigned DWORD lvalues; the independent two-character text
cursor advances with a 32-bit unsigned sum before its original signed
storage. No new array, limits or script call boundaries were introduced;
`ZunTimer` methods remain the same actual separately observed objects.

The exact **308-byte** `Ending::UpdateFade` similarly advances its signed
fade frame as a wrapping DWORD. All four fade-mode `timer*255` products
now use native unsigned DWORD multiplication, then the original *signed*
division and unsigned alpha-byte color shift. The black fade-in's
`255 - quotient` also preserves the original modulo-32 result. The original
fade gates, timing modes and calls are unchanged, with no zero-divisor
fallback or validation. That removes signed-C++ overflow dependencies
without changing documented normal gameplay.

Only the two affected pinned VC7.1 compiler carriers replayed and passed
**10/10 previously exact Ending owners / 3,015 complete original PE bytes
/ 159 relocation fields**, including both the large interpreter and fade
callback. Thirty private `$L` labels in those two owners were rebased only
after source-cold proof of every actual same-section COFF definition,
original PE operand destination and complete physical owner bytes. This is
native source semantic recovery, not new exact-function or whole-product
credit.

## 2026-11-13 ANM interpreter authentic variable-length instruction prefix

The exact **9,776-byte physical `AnmManager::ExecuteScript`** interprets
file-backed bytecode with an eight-byte opcode/time/mask header followed by
a **variable-length argument payload**. The original source projection
claimed *ten* owned integer/float operands and forty owned argument bytes
even for zero-argument opcodes. It now describes only a real eight-byte
prefix plus the **first four-byte operand's integer/float/byte union**.
All later typed argument reads and writable results already use independently
proven `uint32(instruction)+offsetof(AnmRawInstr,intArgs/floatArgs)+4*index`
Win32 byte addresses; `byteArgs[1]` remains a genuine on-wire byte inside
the first DWORD. A source assertion fixes the exact +8 operand address and
12-byte prefix size. The original variable-sized stream cursor, zero-operand
instructions, signed jumps, VM state and all ANM opcode boundaries remain
unchanged. No new interpreter ABI, bounds check, fixed payload allocation
or function boundary is introduced.

The **two affected pinned VC7.1 carriers** passed the complete original-bound
comparison of **5/5 existing exact functions, 10,496 complete PE bytes and
421 relocations**, including the 9,776-byte interpreter and its four
independent scalar-variable leaf resolvers. The compiler renumbered **148
private `$L` references** across those owners; each current same-section COFF
definition, original PE destination, relocation addend and whole function
body was checked before changing only manifest spellings. This is
source-level file format recovery, not new exact-function or full native
Windows product/runtime credit.

## 2026-11-12 Complete ANM interpreter integer/float variable wire addressing

The **9,776-byte physical `AnmManager::ExecuteScript` boss** decodes
variable-length ANM instructions from the real loaded script buffer. Its
constant direct `intArgs[0/1]` reads still crossed nominal C++ array bounds
without expressing the original signed DWORD index as Win32 pointer
arithmetic. All 12 remaining direct integer reads — blend/alpha, interrupt
labels and the interpolation mode selectors — now reuse the previously
verified native `instruction + offsetof(AnmRawInstr, intArgs) +
uint32(index)*4` lvalue.

More significantly, **both sides of every variable-mask resolver**
`GET_INT_VAR` and `GET_FLOAT_VAR`, and their write-lvalue paths
`GET_INT_VAR_PTR` / `GET_FLOAT_VAR_PTR`, now calculate the same authentic
four-byte wire addresses. The float path uses its actual union member
`floatArgs` and real `f32` type; the integer and float lvalues are writable
where the script VM stores results. No opcode boundary, clone, bounds gate,
value cache, inline assembly or call-order change was added. Individual
original operand reads and masked-variable resolution remain distinct.

Two affected pinned VC7.1 compiler profiles replayed **all five
previously exact canonical owners / 10,496 full original PE bytes / 421
relocations**, including all **9,776 physical bytes** and the owned jump
table of the big interpreter. The compiler renumbered 107 private `$L`
references; each corresponding actual same-section COFF definition,
relocation addend and original target VA was independently verified before
updating its *spelling* in the manifest. There is no new exact-function
or complete Windows product/runtime credit.

## 2026-11-12 CardAttack StartAttack native callback rows and Player DWORD counters

The complete `CardAttackStartView::StartAttack` (923 bytes) updates either
the primary or secondary attack from the *same actual* table of four callback
pointers per 0x10-byte row. Eight `begin/update/draw/end` uses now share a
native i386 `base + uint32(primaryRow134C or secondaryRow1358)*0x10`
address instead of an implicitly bounded C++ table subscript. Each live row
is reread at its original call/assignment site; no external new ABI or
cached callback snapshot was added. The original source-facing table is
retained, and a static source assertion fixes the row's physical stride.

Six independently used Player header words at +0xA0/+0xA4/+0xB0/+0xB4/+0xB8
are incremented through their unsigned DWORD lvalues, matching native Win32
`INC DWORD` even when a signed source value would overflow. The three live
Player +0x303F0 script-VM ID additions (+1/+2/+3) likewise use modulo-32
arithmetic before preserving the original signed callee argument. The
original side/attack choice, runtime caps, update/draw/end pointers, display,
callback order and animation launch are unchanged. A dynamic side-bit shift
mask experiment was **rejected** because it changed the owner's length.

The one affected pinned VC7.1 carrier passed a complete original-bound
cold comparison of **923/923 physical bytes and all 29 relocations**.
No additional exact function or runtime-product credit follows; this is
genuine original memory/overflow semantics inside an already exact boss.

## 2026-11-12 Title music-room native selected-song VM ordinal

`TitleScreenView::DrawMusicRoom` draws the selected BGM description from
the *same* genuine `vms` 0x2A4-byte ANM allocation used by the 2,258-byte
music-room update boss. The selected script/UI song word is a signed DWORD;
the original i386 adds 159 modulo 2^32 before computing the real VM
record address. The maintained source now explicitly performs this unsigned
addition, then interprets the resulting word with its original signed
representation. It preserves the live song selection, saved position/color,
identical original DrawNoRotation calls and independent VM read/writes,
without a new bounds check, VM clone or changed rendering schedule.

The exact one-carrier VC7.1 scoped replay passed both existing canonical
owners: **2,467 original PE bytes, 80 relocations**, including the entire
**2,258-byte update boss** and **209-byte music-room draw**. A separate
`musicTrackCount = trackIndex + 1` unsigned-wrap trial changed four SIB
register encodings in the formerly exact update owner and was reverted;
this commit contains only the validated source improvement. It establishes
neither new exact-owner credit nor a complete Windows runtime build.

## 2026-11-12 ANM interpreter direct VM-write opcodes use real wire DWORDs

`AnmManager::ExecuteScript` is a single 9,358-byte authored interpreter
with a **9,776-byte complete physical original owner**, including its
switch tables. Its variable-length ANM file instruction has an authentic
8-byte prefix and variable packed DWORD arguments. Six direct setters —
`PosMode`, `Visible`, `Ins25/type`, `ZWriteDisable`, `Ins31/flag15` and
`Ins83/player-bullet-hit-animation` — now fetch their live immediate
operand via the same native `instruction + 8 + uint32(index)*4` lexical
source lvalue previously proved for jumps. That lvalue was renamed from
the jump-only spelling to `TH09_ANM_NATIVE_WIRE_INT` to express its shared
role. There is no second record, bounds gate, temporary snapshot or
callable opcode boundary; switches and real ANM VM fields are unchanged.

The two affected pinned VC7.1 compiler profiles passed the scoped replay
of **5/5 pre-existing exact owners, 10,496 complete PE bytes and all
421 relocations**. The entire 9,776-byte interpreter physical span,
including its target-owned jump tables, matched. Ninety-eight compiler-
private `$L` label spellings were updated only after proving the actual
same-section COFF definitions, their original PE operand destinations
and complete owner bytes. This is native script-wire fidelity, not
additional exact-function or full-product/runtime credit.

## 2026-11-12 Background stage script native fog color bytes

The original `BackgroundRunStageScriptPhase` boss interpolates a four-byte
sky/fog color from two authentic DWORDs, at its active/final/initial fog
fields. Four dynamic byte lvalues in the live fog loop now use the actual
`unsigned int &color` allocation's Win32 address plus `uint32(i)` rather
than relying on C++ subscript arithmetic from an unsigned-byte cast to a
single member. The original loop count (four), each repeated initial
color read, x87 interpolation and signed/float conversions remain intact;
there is no second palette, index guard, cached value or helper call.

The one affected pinned-VC7.1 `/O2 /Ob0` compiler carrier reproduced
**5/5 previously exact owners, 3,231 complete original PE bytes and 160
relocation fields**. This includes all **2,652 physical bytes** of the
stage-script boss, its switch table, **448 bytes** of the camera
interpolation helper and the timer/math leaves. Five compiler-private
`$L` name changes were accepted only after independently proving their
actual same-section COFF definitions, original PE destinations and all
complete linked owner bytes. This improves native source memory semantics
but does not increase exact-function credit or establish a Windows build.

## 2026-11-13 FrontMessage Update/Setup real ANM font dimensions

The complete **2,968-byte original-bound `FrontMessageRuntimeView::Update`**
sets the widths/heights of actual text `AnmVm` records at offsets +0x298
and +0x299 in script opcodes 3/0x10. Both chained stores now target the
real `AnmVm::fontWidth` and `AnmVm::fontHeight` byte members, instead of
writing through an unrelated `unsigned char*` cast from the beginning of a
0x2A4-byte VM. The opcode still rereads the live runtime text index and
uses the existing native byte-stride VM selection, without an extra cache,
clamp or helper call. The **357-byte `FrontMessageRuntimeView::Setup`**
now addresses the very same authentic two font members in the two actual
text VMs, preserving its original height-then-width store order and script
initialization. These fields are independently verified and consumed by
`AnmManager::DrawTextLeft` / text rendering in other exact owners.

Only the three affected pinned VC7.1 function carriers replayed: **3/3
pre-existing exact functions, 3,364 complete original PE bytes and all
177 relocation fields** passed, including the giant script interpreter,
39-byte string decoder and setup callback. The optimizer renumbered 22
private `$L` references, and each same-section COFF definition, original
PE target and whole physical function was independently proven *before*
updating manifest labels. This is native source/member recovery, not a new
exact-function or complete Windows runtime/product claim.

## 2026-11-12 FrontMessage big Update authentic portrait pair receivers

`FrontMessageRuntimeView::Update` owns the actual eight `AnmVm` portraits
at +0x14 as four native adjacent pairs, each 0x548 bytes. Twelve fixed
first/second portrait-field writes and `ExecuteScript` receivers used an
unrelated projected `FrontMessagePortraitPair *` C++ array; they now reuse
the already-authenticated `NativeMessagePortrait(this, slot)` lvalue that
the variable-width script opcodes use. The inlined lookup forms the original
Win32 byte address from `&messageVms14[0]` plus unsigned pair-index *
`sizeof(FrontMessagePortraitPair)`. Original 8 ANM calls, lookup order,
interrupt values and the single physical backing allocation are unchanged;
there is no added VM object, branch or callable replacement.

Only the two original VC7.1 FrontMessage carriers were replayed. Both
existing exact owners passed: **3,007 complete original PE bytes, 167
relocation fields**, including the **2,968-byte physical main Update**
owner with its trailing switch table and the **39-byte decode leaf**.
Twenty-seven compiler-private `$L`/`$run_scripts$` labels were accepted
only after checking each live same-section COFF definition, its independently
identified original PE destination and all full-owner physical bytes.
No new exact-function or whole-executable claim follows.

## 2026-11-12 Player selector's authentic 0x0C-byte stack collision vectors

`PlayerUpdateSelectorState` is one **3,238-byte** gameplay selector boss.
Its three temporary collision half-sizes are actually constructed on the
stack as `PlayerConstructedVector3View[3]`, each 0x0C bytes. The existing
`PlayerPositionView*` cast used for collision testing was incorrectly
advanced or indexed as if it owned a *separate* C++ array of this second
type. The two dynamic collision-query arguments now use an inlined native
Win32 `uint32(&constructedHalfSizes[0]) + uint32(index)*0x0C` lvalue;
the matching grid cursor also advances by an unsigned native 0x0C-byte
address. The real three-vector allocation, exact Float3 geometry, the
independent float radii array, all collision tests/branches and retry
ordering remain unchanged. No bounds cap, side-buffer or new callee is
introduced.

The pinned VC7.1 profiles replayed **2/2 existing exact PlayerGameplayState
owners, 3,650 full original PE bytes and 154 relocation fields**. The
large selector itself still matched all 3,238 original bytes and its
independent `ResolvePatternOffset` leaf stayed exact. Two compiler-private
`$L` spellings in that leaf were separately bound to real same-section
COFF definitions and unchanged original PE addresses, with all complete
bytes verified before the manifest name update. This improves original
native stack ownership but grants no new exact function or product credit.

## 2026-11-12 Title native character table and start-menu embedded VM addresses

The original Title owner has two *distinct adjacent* signed character
cursor DWORD fields at +0x10 and +0x14. The cursor navigation's existing
native side-byte addressing now has an independent source-level check of
the real layout, without pretending the two fields form a C++ array.
In the **2,241-byte** character-select-mode0 owner and its adjacent
selection code, six accesses to the true sixteen-byte
`g_TitleCharacterOrder14` use the same already proven native character
lookup as the other Title modes. Each actual signed cursor/value is scaled
modulo 2^32, with the live original global byte and no new bounds check.

The **1,592-byte physical start-menu** owner also now calculates the
two dynamic `embeddedVms` addresses from its actual 57-entry
`AnmVmView` allocation at +0x11BA0: one for the staged help-text renderer,
and one for the selected help-text VM. The exact original argument order,
reads and in-object VM ownership remain intact, with no new allocation or
observable helper call.

The two affected pinned VC7.1 compiler carriers reproduced **all 41 exact
Title owners (18,972 complete PE bytes, 953 original-bound relocations)**.
The nine source-line-renumbered `$L` references in the start menu were
accepted only after independent actual COFF same-section definition,
original PE destination and full-body equality checks, followed by the
scoped replay. These source semantics corrections do not grant additional
exact-function credit or prove a whole Windows runtime product.

## 2026-11-12 SoundPlayer SFX request counters use native DWORD increment

Both exact `SoundPlayer::PlaySoundByIdx` and
`PlaySoundPositionedByIdx` update the **same 12 genuine, signed 32-bit
request-count words** as the large `SoundPlayer::ProcessQueues` owner.
Four producer post-increments—two selecting a live pan slot and two
publishing the initial queued request—now act on the unsigned view of
those original DWORDs, preserving the i386 increment modulo 2^32 rather
than relying on undefined signed overflow when a count has negative
bits. The 0x80 gating, metadata load, original pan/value storage,
original field rereads and all call/return chronology remain unchanged.
No new queue, clamp, retry or emitted callable boundary is introduced.

Eight actual pinned VC7.1 SoundPlayer carriers cold-replayed **20/20
previously exact owners, 5,400 original PE bytes and 184 relocations**,
including the **1,456-byte physical ProcessQueues owner**, both SFX
producer leaves (140 and 153 bytes), and the BGM paths. One compiler-
private `$L` reference in ProcessQueues was rebound only after a
source-cold proof of its actual same-section COFF definition, identical
original PE destination, and all complete owner bytes. This is native
source-semantics recovery and not new exact-function or Windows product
credit.

## 2026-11-12 Title character scrolling native input-state receivers

Both `TitleScreenView::MoveCharacterCursorHorizontal` and
`MoveCharacterCursorHorizontalForInput` call the original scrolling input
leaf on the actual three-element `g_TitleInputStateTable` allocation. A
record is **0x8E bytes**, shared with the independent replay/input view.
Their eight runtime receiver lookups now use native Win32 modulo-32
`&states[0] + uint32(inputIndex) * 0x8E` addressing rather than an array
subscript with additional C++ bounds preconditions. All four original
priority checks, separate input and cursor selectors, wrap behavior and
sound/callback calls remain intact; there is no new validation or storage.

After the source change, a fresh VC7.1 two-carrier target-bound proof checked
**all 41 exact TitleScreen owners, 18,972 complete bytes and 953 fields**.
Nine compiler-private `$L` renames in the exact start-menu owner were
accepted only after verifying the actual same-section COFF definitions,
retail PE operands and full complete-owner bytes. The private spellings,
not target destinations or source behavior, changed. The separately
nonexact Title OnUpdateResult is still open and this adds no exact-function
or executable/runtime credit.

## 2026-11-12 Title menu, result, replay and start-state DWORD counters

The TitleScreenView owner uses one real screen-state allocation for
stateTimer, stateTimer2/phaseTimer, screenFrameCounter/replayUiFrameCounter
and idleFrames. Thirty-two persistent-frame/phase increments across
character/difficulty/mode/result menus, replay browsing/saving, and
start-menu updates now use unsigned DWORD lvalues, preserving retail
i386 INC wrap instead of invoking undefined C++ signed overflow. The
result updater's separately installed native counter increments are
left unchanged. No new frame gate, clamped index, reordered callback
or helper call is introduced.

The two actual source/profile carriers cold-replayed **41/41 already
exact functions, 18,972 complete original bytes and 953 relocation
fields**, including the large title menu and save/update owners and
their small helper leaves. Nine compiler-private label spellings
changed solely due to the source edits. Each was independently bound
from an actual same-section COFF definition to the unchanged retail PE
target before the manifest names were rebased. The separately
nonexact OnUpdateResult remains 484/485 bytes, all 34 target-bound
fields, nine ordered calls, 40 graph blocks, and 218 unequal overlapping
bytes; no new exact function is claimed.

## 2026-11-12 Ending interpreter's native file-backed script cursor

The Ending::ReadScriptParameter and Ending::RunEndingScript owners consume
the real mutable scriptCursor pointer into the file-loaded ending script,
not a private fixed-size array. Nineteen one-byte cursor increments and
one two-byte advance now explicitly form unsigned modulo-32 i386 byte
addresses and publish the resulting pointer word. The existing end-of-line,
NUL, control-opcode and skip loops retain their actual read order, as
do all 0x2A4-byte ending ANM VM slot references and the genuine stack
text buffer. No new bound, allocation, or sanitization behavior was added.

The pinned VC7.1 scoped build rebuilt only the two real Ending.cpp
carriers and proved **10/10 exact physical owners, 3,015 original PE
bytes, and 159 relocations**, including the complete 1,727-byte script
interpreter and its ReadScriptParameter leaf. 31 compiler-private label
spellings changed; their actual COFF same-section definitions and all
original PE operands/owner bytes were checked independently before
updating only those manifest names. These remain already-exact owners,
not a new promotion or game product/runtime claim.

## 2026-11-12 ANM surface cache and file-slot native addresses

The ANM loader's LoadSurface and PreloadSurface paths access actual native
32-slot storage within the AnmManager owner. Twenty lvalue expressions
across the primary/secondary D3D surfaces, preloaded file data, file
sizes, and 0x14-byte D3DXIMAGE_INFO records now use one templated
native slot reference. Every address is formed as the real member-array
base plus uint32(surfaceIndex) times the actual element size. The source
does not impose new C++ bounded-array pointer arithmetic or clamp the
signed surface index. It preserves each independent read, publication,
D3D call, cleanup and original error path.

The resource loading and service path also now advances its original
0x120-byte ANM manager file-slot records from the live manager base
using native modulo-32 word arithmetic rather than treating the manager
as an unbounded C++ array of file slots. No shadow storage is introduced.

The two actually affected pinned VC7.1 compiler carriers cold-replayed
**17/17 existing exact functions, 3,258 complete physical bytes and 133
original-bound relocation fields**, including both surface owners,
PreloadAnm/LoadAnm and the manager constructor. The complete original
PE fields and code are unchanged. This is source fidelity for real
array backing, not a new function promotion or runtime/port claim.

## 2026-11-12 GameManager update native frame and reward counters

The already exact GameManagerSetupLayout::OnUpdate callback uses seven
mutable signed 32-bit words as retail i386 counters. The playtime frame,
gameplay-setup wait, demo playback frame, difficulty valueD0, running reward
valueCC, manager value348, and conditional limitB4 increment now explicitly
use unsigned DWORD bitwise arithmetic before restoring the intended signed
result where needed. This avoids undefined signed-overflow semantics while
preserving the original unrestricted 32-bit wrap; limitB4 is still compared
*as signed* against valueC8 after its increment. The resource/score gates,
RNG calls, side updates, and original call order are unchanged.

The complete source-scoped VC7.1 check rebuilt only the two actual GameManager
carriers and proved **12/12 exact owners, 4,680 complete original bytes,
415 relocation fields**, including the 1,230-byte OnUpdate and the
1,689-byte GameplaySetupThread. The affected statements compile to the
original unmodified machine instructions. This is a source-level native
arithmetic improvement, not a new exactness claim or whole-product closure.

## 2026-11-12 Enemy timeline variable-length argument address identity

The native EnemyScheduleRuntimeView::Run decodes variable-size timeline
opcodes through eight-byte header records followed by raw argument DWORDs.
Ten caller expressions for these argument words (eight instruction-local
reads and two live instruction20 event-queue comparisons/publications)
previously used C++ char-pointer addition past the projected eight-byte
header object. Those reads now compute uint32(instruction address) plus
sizeof(EnemyTimelineInstructionView), preserving the same original
0x08-byte payload position without requiring an in-bounds header array.
The timeline dispatch, repeated live event-slot reads, RNG calls and
spawn argument order are unchanged.

Original-bound VC7.1 compilation of the complete timeline Run code and
owned switch tables plus its genuine ZunTimer::operator== leaf passed
**2/2 exact owners, 1,068 complete physical bytes and 42 original-bound
relocation fields** (1,048-byte timeline owner plus 20-byte timer leaf).
Sixteen compiler-private $L symbol spellings shifted. Every actual
same-section COFF definition and original PE target was independently
proved alongside all owner bytes before rebinding only those names in
the match manifest. This is native source semantics for the already exact
timeline boss, not new exactness or whole native game closure.

## 2026-11-12 Background stage-script native record address and index wrap

The complete 2,652-byte BackgroundRunStageScriptPhase physical owner
includes the stage instruction dispatch/switch table. Its 0x14-byte
RawStageInstrProbe records belong to the actual stageScript028 buffer.
Retrieval now forms the native 32-bit base address plus
uint32(stageScriptInstructionIndexC64) * sizeof(RawStageInstrProbe), without
assuming an in-bounds C++ pointer subscript. The script instruction index
is separately incremented as an unsigned DWORD lvalue, preserving the
original i386 INC DWORD wrap when signed INT_MAX is crossed. Neither change
adds a bounds clamp, instruction copy, new execution branch or other state.

The pinned VC7.1 carrier cold-replayed **five exact owners, 3,231 complete
physical bytes and 160 original-bound relocation fields**, including the
2,652-byte stage interpreter, 448-byte camera interpolator and three
related scalar/timer leaves. The combined native-address and signed-index
changes shift 49 compiler-private $L spellings; each current same-section
COFF definition, original PE operand destination and every byte of all
five owners were independently compared before updating names in the
manifest. Exactness is preserved, not newly granted, and does not certify
portable runtime closure.

## 2026-11-12 Player pattern boss native cooldown and alternate-index words

The complete original-bound 3,238-byte PlayerUpdateSelectorState owner
decrements its live signed retryCooldown50 as an i386 DWORD, which wraps
at INT_MIN instead of invoking C++ signed-overflow behavior. The selector
also combines its original RNG value (either zero or one, multiplied by
nine) with the active selectedPattern as a wrapping native DWORD before
addressing the original alternate-pattern lookup table. The real lookup
still uses the existing 32-bit table base and index stride; no clamping,
extra read, copied table or altered RNG call was introduced.

The two actual VC7.1 carriers for PlayerUpdateSelectorState and its
ResolvePatternOffset helper cold-replayed **2/2 exact functions, 3,650
physical bytes, 154 original-bound relocation fields**. The two
compiler-private local label spellings in the 412-byte helper changed,
but their actual same-section COFF definitions and original PE operand
targets were independently checked, together with every source byte and
field, before rebinding the manifest. This is a native-meaning repair to
an already-exact large boss and exact helper, not a new exact promotion.

## 2026-11-12 ANM integer bytecode arithmetic wraps at the native DWORD width

The 9,776-byte complete physical AnmManager::ExecuteScript owner (9,358
logical code bytes plus owned tails/tables) consumes signed ANM wire integers.
The six integer arithmetic instructions ISetAdd, ISetSub, ISetMul,
IAdd, ISub and IMul now explicitly compute modulo-2^32, then restore
the signed result when required. In-place operations write through the same
original integer lvalue using its corresponding unsigned 32-bit type;
argument evaluation, variable ID resolution and fault behavior for integer
division are unchanged. This removes C++ signed-overflow assumptions without
inventing saturation, guards or other runtime behavior.

Both pinned /Ob0 and /Ob1 original-bound compiler carriers cold-replayed
**5/5 exact functions, 10,496 complete physical bytes and 421 relocation
fields**, including the full 9,776-byte script interpreter (368 fields) and
four ANM variable accessors. Four accessor labels and 58 interpreter
compiler-private $L references changed names; each actual COFF same-section
definition and original PE operand address was independently checked before
rebinding names in config/match-units.toml. All 62 spelling updates preserve
original targets, kinds, addends and byte-identical physical ownership. This
is source semantic fidelity, not a new codegen-exact owner or native-product
runtime/port claim.

## 2026-11-11 RunEcl and PopContext native child/stack record addresses

The exact 147-byte `Th09EclRunControl::PopContext` and still-nonexact
15,564-byte `RunEcl` share one genuine EnemyView allocation, including
four 32-bit child ECL pointers at +0x33D8 and a 0x22C-byte context stack
whose base is read live from +0x2CE4. The original VC7.1 code addresses
these fields with native modulo-32 byte offsets from signed child index
and signed-short call depth. The maintained `EclManager.cpp` now reuses
the independently verified `Th09EclRunState::ChildSlotRef` for all three
PopContext child accesses and three RunEcl current/next child reads, and
explicitly scales the three call-stack entry addresses by `sizeof
(Th09EclContextView)` as unsigned Win32 words. The context-selection
loop's starting index and stored ordinal similarly wrap as native 32-bit
additions. All existing call stack bookkeeping, original field-read
chronology, transitions, copying, frees and error paths remain intact.

Seven separate or combined pinned VC7.1 source-cold experiments each
reproduced **147/147 bytes and two original relocation fields** for
PopContext, plus the entire **15,564-byte RunEcl with all 598 fields,
identical jump tables and the pre-existing 30 ordinary mismatches**.
The installed source recompiled only the two actual affected EclManager
profiles and passed the strict canonical cold replay of **83 byte-exact
functions / 29,856 complete bytes / 1,389 original relocation fields**.
Twenty compiler-private `$L` label renamings were rebounded only after
independent PE/actual COFF same-section targets and entire original owner
bytes were proved. Opcode 4/155 audits still pass 90/3,072 behavior cases;
no newly exact function or whole-game closure is claimed.

## 2026-11-11 RunEcl EX instruction dispatch native callback addresses

`RunEcl @ 0x004086C0` has two distinct EX-instruction opcodes: one calls
a table entry immediately with the original Enemy and instruction
parameters, while another can store the table entry for a repeating
callback in the active ECL context. The genuine twelve-callable prefix
of `g_EnemyEclExInstructions` lives at the original TH09 EX function
table address, with four-byte pointer words. The signed script index
comes from `ReadInt`; retail i386 calculates each callback address as
`original table base + unsigned(index) * sizeof(EnemyEclExInstruction)`
modulo 2^32. The maintained source formerly used ordinary C++ array
subscripting, which is undefined for out-of-range wire indices. A
`Th09EclRunState::ExInstruction` native-address lvalue now covers both
uses without inventing additional table entries, pointer validity checks,
a fallback callback, or a new allocation. Importantly, the repeating
handler still performs its two distinct original `ReadInt` calls (one
for the signed gate and one for the indexed load), preserving all resolver
side effects and the original context publication order.

Immediate-call-only, callback-store-only, combined inline helper and
direct-byte-address VC7.1 probes reproduced the **full 15,564-byte
RunEcl, 598 original-bound relocation fields, both original jump tables,
and its same 30 ordinary mismatches** (seven opcode 4, 23 opcode 155).
The installed source cold-replayed only its **five affected compiler
carriers**, proving **87 exact functions / 30,583 complete bytes /
1,409 real relocation fields**. Twenty private `$L` spellings were
rebound only after checking actual same-section COFF definitions,
original PE targets and complete byte equality of all 87 owners.
The original-bound opcode 4 and opcode 155 semantic checks continue
passing 90 and 3,072 cases. Neither exactness count nor Windows i386
product closure has changed.

## 2026-11-11 RunEcl native state and callback-slot DWORD addressing

The maintained `RunEcl @ 0x004086C0` now uses native byte-address lvalue
helpers for **five real EnemyView state-table families**: the 32-element
signed-short subroutine table (+0x2D30), four-word life callback thresholds
(+0x33B0), four-word life callback subroutine IDs (+0x33C0), four child ECL
block pointers (+0x33D8), and two secondary ANM VM records (+0x02AC,
stride 0x2A4). Their indices originate from signed script ECL wire words.
Retail i386 uses modulo-32 `actual subobject base + index * element size`
math; the old source used C++ array indexing that could become undefined
on out-of-bounds signed inputs. Each helper preserves the actual native
allocated field, every repeated live read, the original resolver and
subroutine call ordering, exact primary/secondary manager ownership and
all branch/error paths. Boss-slot remote clearing also now reuses the
previously original-bound `Th09EclRunRemote::RemoteEnemy` pointer lvalue.
No bounds checks, synthetic tables or alternate storage were introduced.

Five independent slot-family probes and their complete combination plus
boss-remote clearing compiled under the pinned VC7.1 EclManager profile.
Every accepted candidate retained the **full 15,564-byte RunEcl**, all
**598 original-bound relocation fields**, original switch/jump tables and
precisely its existing **30 ordinary differences** (seven in opcode 4,
23 in opcode 155). The installed combined source cold-replayed only its
**five affected compiler carriers**: **87 exact functions, 30,583 complete
physical bytes and 1,409 true original-bound relocations**. Twenty
compiler-private `$L` names were rebound only after independent real
same-section COFF definition, original PE operand destination and full
byte-exact owner proof. The original-bound opcode-4 and opcode-155
behavior audits still passed 90 and 3,072 cases respectively. No newly
exact function or Windows i386 product closure is claimed.

## 2026-11-11 RunEcl remote enemy slot native DWORD addressing

`RunEcl @ 0x004086C0` uses the actual nine pointer slots at
`EnemyManagerView::remoteSlots2AC388` for opcode 86–89 remote
operand resolve, float read, remote subroutine call and deferred callback.
The signed ECL wire index was previously used with a C++ array subscript,
which becomes undefined when outside the nine declared elements. The
original i386 forms a 32-bit byte address from the real slot-base and
`unsigned(index) * sizeof(EnemyView *)`, with no added clamp. The shared
`Th09EclRunRemote::RemoteEnemy` helper now returns the lvalue at that
native address, and both the remote-slot macro and opcode 86's
volatile-qualified parameter read reuse it without changing repeated
resolver side effects, real slot ownership, read chronology or ABI.

Seven isolated/combined native VC7.1 source probes produced the entire
**15,564-byte RunEcl original-bound candidate with all 598 reviewed
relocations, unchanged native jump tables, and precisely 30 existing
ordinary differences** (seven opcode 4, 23 opcode 155). The installed
combined source cold-replayed only the **five genuinely affected compiler
carriers**: **87 exact function owners / 30,583 complete original bytes /
1,409 original relocation fields**. Twenty VC7.1 compiler-private `$L`
names were rebound only after independent target-PE operand and real
same-section COFF definition proof verified all 87 complete owners.
The official complete-RunEcl audit continued passing **90 original-
bound opcode-4 behavior cases and 3,072 opcode-155 transition cases**.
This is stronger source-level Win32 ownership fidelity, not a newly exact
owner or native whole-game product closure.

## 2026-11-11 RunEcl spell-background VM native stride and loop word

The late `TH09_ECL_OPCODE_TRIGGER_SIDE_OWNER_STATE` handler in the
15,564-byte `RunEcl` traverses the real `Background::spellVms[32]` array
at +0xCE0. The VM stride is the independently proved 0x2A4-byte native
`AnmVm`, and the count is read live from the authentic `spellVmCount`
at +0xCD8. The original i386 forms the VM pointer as native 32-bit
`base + signed lateIndex * sizeof(AnmVm)` before calling the existing
`SetAndExecuteScriptIdx`; the maintained code had used C++ potentially
out-of-range `spellVms[lateIndex]` subscripting. The loop's signed
index increment now also wraps modulo 2^32 before its original signed
count gate. Existing opposing-side ANM/script bases, original replay
RNG order, per-iteration manager reads and the already preserved native
script-index sum are unchanged. No extra index clamp or alternate VM
allocation was introduced.

Five independent and combined pinned VC7.1 compilations reproduced
**15,564 source bytes, 598 original-bound relocations, unchanged switch
and jump tables and exactly 30 existing ordinary residual bytes**.
The installed source cold-replayed only its **two genuine affected
EclManager profiles: 83 byte-exact functions / 29,856 physical bytes /
1,389 real relocation fields**. Eight private VC7.1 `$L` names were
rebound only after independently verifying same-section COFF definition,
original PE target and complete byte equality of all 83 owners.
The separate original-bound opcode 4/155 semantic audits still passed
90/3,072 cases. Neither exact count nor native-product closure changed.

## 2026-11-11 native selected laser creation slot in RunEcl

The reviewed 15,564-byte `EclManager::RunEcl @ 0x004086C0` creates a
laser through its genuine `EtamaController` and stores the result in
`EnemyView::laserSlots32D8[32]` (the original +0x32D8 DWORD pointer
array) at the script-owned signed `selectedLaserSlot3358` index.
Subsequent laser control opcodes already use the native `LaserSlot`
lvalue. The creation opcode now also forms the actual i386 byte address
`uint32(&laserSlots[0]) + uint32(selected slot) * sizeof(pointer)`
without a new range gate, second table, pointer cache or changed
`SpawnLaserPattern` call. The spelling must remain **lexical**: a
call through `LaserSlot` itself changed the original-bound callsite
shape and was rejected. The lexical form retained all native reads,
script dispatch edges and original switch-table bytes.

Four isolated source probes were compiled against pinned VC7.1.
The installed lexical form passed the **complete original-bound RunEcl
boss guard: 15,564/15,564 physical bytes, all 598 original relocation
fields and intact dispatch tables**, with only the existing 30 unlike
opcode-4/155 bytes (7+23). The changed `.inl` affects five real
compiler carriers; scoped cold replay passed **87/87 exact sibling
functions, 30,583 complete bytes and 1,409 original-bound relocation
fields**. Eight private `$L` references were rebound only after
confirming their actual same-section COFF definitions, unchanged
field offsets/types and original PE branch destinations. This is
substantive ECL native laser-state source fidelity, not newly exact
RunEcl credit, full product or runtime closure.

## 2026-11-11 native ECL movement wire argument words

The `RunEcl` movement helpers `StartTimedPolarDisplacement` and
`MoveRandomBiased` consume a true **variable-length** instruction stream.
Its 0x0C-byte header is followed by native 0x04-byte wire arguments; the
maintained source forms the original `wireArgs` pointer from the physical
header base and now calculates **16 actual operand reads** through
`uint32(wireArgs) + uint32(index) * sizeof(Th09EclWireArgument)`.
All repeated resolves, operand masks, float/int bit interpretations and
original call ordering remain live; there is no extra bounds gate,
alternate argument pool, cache or change in the zero-sized header layout.

A callable inline accessor matched the full `RunEcl` body but made the
independently exact `MoveRandomBiased` fail its `/Ob0` 516-byte owner extent.
Both lexical and macro-native variants preserved **516/516 bytes** of
`MoveRandomBiased`, **288/288 bytes** of `StartTimedPolarDisplacement`, and
the complete **15,564-byte original-bound RunEcl candidate** with **598
relocations**, unchanged original switch tables and just its existing
**30 nonexact opcode-4/155 bytes**. The selected lexical native macro
preserves both compiler profiles. The final two-carrier scoped cold replay
passed **83/83 exact siblings, 29,856 complete bytes and 1,389 relocation
fields**; 20 VC7.1 private `$L` labels were rebound only after confirming
actual same-section COFF definitions and unchanged original PE addresses.
This is source-level ECL wire-memory fidelity, not new RunEcl exact credit.

## RunEcl native pending-subroutine, remote boss and bullet-transform slots

Three script-controlled table accesses in the complete 15,564-byte RunEcl
interpreter now use the real native bases and DWORD-wrapped byte strides
rather than C++ array subscripts: the 0x02-byte Enemy pending-subroutine
slot, the manager's nine DWORD remote boss pointers, and the descriptor's
0x18-byte bullet-transform records. The ECL wire resolver is still called
at each original site; no range checks, new storage or ordering changes were
added. The selected-laser creation subscript was deliberately **not** changed:
the attempted rewrite failed the full-owner layout/identity comparison.

The combined pinned VC7.1 original-bound candidate retained **15,564/15,564
physical bytes, all 598 relocation fields and tables, with the same 30
nonmatching opcode-4/155 bytes**. This is native semantics recovery, not
RunEcl byte-exact promotion. All five include-affected VC7.1 compiler
carriers were cold-rebuilt and the **87 existing exact functions** passed:
**30,583 complete original bytes and 1,409 relocation fields**. Eight
compiler-private $L label spellings changed, but independent same-section
COFF definition/PE operand/full-byte checks proved the identical real targets
before manifest rebinding. No target bytes, toolchain flags or exact status
were modified.

## RunEcl: opcode 4 and opcode 155

The maintained 15,564-byte/598-field owner has 30 ordinary differences,
concentrated in two handlers. Its fields and tables match. Use
`check-boss-frontiers.py --owner runecl --skip-exact-siblings` and
`inspect-ecl-complete.py` for the entire owner.

At original opcode 4 (`0x408937`), ECX receives the integer and is pushed
before ECX loads the active context. The candidate loads EAX, loads the
context and then pushes EAX. Both call real Timer assignment at `0x401500`;
seven physical bytes differ. Explicit operator syntax and direct wire-lvalue
forms with the integer copy were neutral. Removing that copy left opcode 4
unchanged and introduced twelve differences in opcode 5.

A fresh 2026-11-12 targeted ten-variant current-worktree probe also tested
raw-operand assignment, explicit `operator=`, signed/const/unsigned local
snapshots, direct packed-wire pointer dereferences, reference variants,
volatile wire reads, and split timer pointers. All ten compiled under the
original pinned VC7.1 RunEcl profile, but each yielded **42 ordinary
nonmatching bytes** in the complete **15,564-byte / 598-field** owner,
versus **30** in the maintained source. No candidate was installed. This
is register/operand-evaluation evidence, not a newly exact ECL handler.
The source variants were diagnostic only; their conclusions are retained
here. Historical scratch scripts/JSON were pruned during handoff cleanup.
To retest, rebuild the current `EclManager.obj` from maintained source with
the pinned profile and compare **the entire** 15,564-byte owner. No
repository-wide cold replay is necessary.

A further targeted set of 18 VC7.1 opcode-155 trials used compound XOR
assignment, temporary bitfield copies, masked clear/OR, and direct versus
memcpy byte snapshots. Full-owner comparison showed the only variants with
unshifted original tables/fields still retain all 30 baseline differences;
other variants changed hundreds or thousands of bytes and moved table
bindings. None was installed. Treat the ESI/EAX/ECX value lifetime as still
unresolved, not a license to patch instructions or compiler flags.

At original opcode 155 (`0x40B770`), ECX receives the zero-extended wire byte,
ESI holds old flags, and bit-24 masked XOR is computed in ECX before the store
through EAX. Twenty-three physical bytes differ. Direct native bitfield forms
shortened an encoding and displaced later code/table targets, producing far
more complete-owner differences. Do not judge them only by the local block.

The pinned Cirno-Labs `d7653152` reference gives the desired local instruction
shapes: opcode 4 matches its whole local block, and opcode 155 has the desired
operations/registers but starts two bytes earlier. Its complete interpreter
still differs at 4,149 positions. Child-context initialization, effect spawning
and ANM rotation are important remaining divergences in that reference.
Explicit copy, parameter constness, native-word and helper-visibility changes
did not resolve that complete candidate. It is a hypothesis, not a replacement.

A prior `EnemyView&` RunEcl parameter trial was fully diagnosed: the postcompile
harness looked in `symbol_aliases` instead of `semantic_aliases`. The recovered
complete comparison retained 30 RunEcl and 13 Enemy differences; 77 exact
same-object units survived. Pointer/reference stack transport and RET4 do not
identify the original source formal. No pending interrupted acceptance remains;
rebuild from source if this question becomes useful again.

ECL's native 0x0C instruction header precedes four-byte union operands. Byte
and halfword access now index inside those words; an extra raw-prefix storage
projection is unnecessary.

## 2026-11-11 RunEcl extra ANM VM native slot addresses

`SetExtraAnmScript` serves two distinct RunEcl opcodes and rereads the
signed wire slot index on every branch. Its two `SetAndExecuteScriptIdx`
receivers and the negative-script `scriptIndex = -1` write now use the
existing `Th09EclRunState::SecondaryVm` native address view of the **actual**
secondary VM array at Enemy +`0x2AC`. The helper computes Win32 modulo-32
`base + index * sizeof(AnmVm)` without adding an index guard or eliding any
wire reads. A fresh pinned /Ob1 VC7.1 build of the complete 15,564-byte
RunEcl owner retained exactly 30 ordinary differences, 598 independently
resolved relocation fields and unchanged tables; no new exactness credit.
Both affected /Ob0 and /Ob1 carriers were cold-built and all **83**
canonical exact owners replayed successfully: **29,856 physical bytes and
1,389 relocation fields**. Two VC7.1 private `$L` labels in CompareOperands
and post-update movement were renamed only after independent full-body proof
of actual same-section COFF definitions and unchanged original PE operands.
Neither internal label target nor any authored-code bytes were changed.

## 2026-11-11 Enemy attached-effect pointer slots

`EnemyCoreUpdateAttachedEffects @ 0x0040F4C0` walks the original 24
`Effect *` slots in the shared `EnemyView` allocation at `+0x53B4`, using
the live signed count at `+0x5414`. The retail i386 addressing multiplies the
index by four as a native 32-bit word; direct C++ array subscripting would
impose an array-bound precondition not present in that code. The maintained
inline lvalue uses the real `effects00[0]` base plus `uint32(index) * 4`,
without an alternate pool, snapshot, call, guard, or changed loop limit.

Both affected VC7.1 EclManager compiler carriers cold-replayed **83 exact
owners / 29,856 physical bytes / 1,389 relocation fields**, including the
complete 151-byte attached-effect updater. `CompareOperands` and
`EnemyPostEclUpdateMovement` received two compiler-private `$L` name updates
only after verifying the actual same-section COFF definitions and unaltered
original PE destinations (`0x00407A4C` and `0x00408548`). The complete
3,900-byte Enemy OnUpdate still has exactly 13 register bytes different
(with 97 fields / 69 ordered calls), and the entire 15,564-byte RunEcl
owner still has 30 ordinary differences and all 598 fields and switch
tables intact. This is semantic native-pointer fidelity, not additional
exactness or Windows product/runtime proof.

## 2026-11-11 native Enemy draw trail sample and loop words

The original 1,758-byte `EnemyManagerDrawImpl @ 0x00411670` reads the same
`EnemyView::trailSamples33E8` allocation as the ECL updater: the 96 genuine
trail samples are 0x1C-byte records at `Enemy+0x33E8`. Its trail length and
step are live signed fields, including the strip, vertex and interpolation
paths. Eleven dynamic reads now form `uint32(enemy sample base) +
uint32(index) * 0x1C` before dereferencing a **const** view of that real
record. The fixed sample zero is left untouched. This removes assumptions
about C++ array bounds without adding fallback reads, a shadow allocation,
or caching the live sample values.

The two trail loop increments, one decrement, the previous/next sample
address arithmetic and both vertex-count updates now explicitly wrap as
Win32 unsigned DWORD words, restoring the original i386 arithmetic for
extreme script-controlled step/count values. All signed comparisons, draw
calls, NaN-sensitive angle tests, taper/fade handling, vertex emission and
original branch structure remain unchanged. Isolated helper and arithmetic
probes matched the entire original body, not just individual disassembly
windows. The installed pinned VC7.1 **one-carrier scoped cold replay passed
all four exact EnemyManagerDraw owners / 1,922 full physical bytes / 61
relocation fields**, including the full **1,758/1,758-byte renderer with
53 original-bound fields**. This is a native-source semantic improvement;
it does not promote a new exact function or prove complete game runtime.

## 2026-11-11 Enemy OnUpdate native ECL-owned slot sharing

The still-nonexact `EnemyManagerView::OnUpdate @ 0x00410730` is the
original 3,900-byte Enemy update boss. Three secondary-ANM-VM reads/writes,
two death/reset life-threshold stores, and three child-ECL-slot reads/frees/
writes now use the **same already-maintained native EnemyView accessors** as
`RunEcl`: `Th09EclRunState::SecondaryVm`, `LifeCallbackThreshold`, and
`ChildSlotRef`. These form the original allocation's physical 32-bit base
plus signed-index * 0x2A4 (VM), 4 (threshold) or 4 (child pointer), without
creating parallel Enemy pools, bounds gates or cached copies. Each original
lvalue is still reread independently at its existing point, and loop bounds,
callbacks, frees, exits and ownership stay unchanged.

Four isolated/combined VC7.1 experiments yielded the **complete 3,900-byte
Enemy OnUpdate candidate**, all **97 target-bound relocations, 69 ordered
calls, matching CFG and precisely its prior 13 ordinary register bytes**.
The installed combined source cold-replayed the two actual EclManager
compiler carriers: **83/83 exact siblings, 29,856 physical bytes and
1,389 relocations**. The complete **15,564-byte RunEcl** boss, including
all 598 relocations and both owned switch tables, still has only its former
30 opcode-4/155 differences. This is shared native storage semantics, not a
new exact function or native-product/runtime closure.

## Enemy OnUpdate

The current 3,900-byte owner retains 97 fields, 69 calls and 13 differences:
`+19C +1A3 +252 +263 +26D +272 +294 +29A +29C +2A5 +2A9 +2B7 +2BD`.
The remaining question is draw-index and bullet-descriptor position/threshold
register scheduling. It uses the actual GameManager side (+0x08 Etama,
+0x20 character), not an Enemy-only side owner.

Index/base-plus-index locals, pointer/reference aliases of the native side and
EffectManager, and Float3 pointer/reference/value forms left the frontier.
Moving the side reference before the position copy changed literal ordering
and worsened the comparison; the strict literal check rejected it. An alias
rename alone is not a new codegen hypothesis.

## Enemy OnUpdate native history-shift and tail sample words

The still-nonexact 3,900-byte EnemyManagerView::OnUpdate has a
script-controlled signed trail length. Its six history-shift
position/velocity/angle reads and writes, plus two last-sample
in-bounds tests, use the real EnemyView trailSamples33E8 array
of 0x1C-byte records. The maintained source now uses the actual
byte pointer to this native Enemy suballocation and a wrapping
uint32(index) * sizeof(sample) offset. Both trailIndex and
trailHistoryLength53A2 - 1 retain their original signed operands,
without introducing a C++ subscript's bounds requirement, clamping,
or a separate trail allocation. The earlier collision append already
uses its original native sample address and is unchanged.

A pinned VC7.1 diagnostic tested all eight sites individually
and in history/tail groups with typed char, byte, and integer base
addresses. The natural byte-pointer form (including one inline
shared accessor) retained **all 3,900 physical bytes, 97 original
relocation fields, 69 calls, 170 blocks and precisely the preexisting
13 unmatched register bytes**. An integer-cast base displaced original
literal operand fields and was rejected. This is faithful Enemy
history layout recovery, not Enemy OnUpdate exact promotion.

The installed /Ob0 and /Ob1 include-consumer compiler carriers passed
cold replay of **83 existing exact owners, 29,856 full original
bytes, and 1,389 relocation fields**. Two private VC7.1 $L names
changed in the exact CompareOperands and post-update movement owners.
Their actual same-section COFF labels, unchanged original PE operand
addresses and the entire 83-owner physical byte corpus were
independently proved equal before updating the two manifest spellings.


## Enemy OnUpdate native trail collision sample address

The nonexact `EnemyManagerView::OnUpdate` traverses the native 96-entry
`EnemyView::trailSamples33E8` array (stride `0x1C`) in its collision pass.
The signed collision index is controlled by the live trail length. The
original i386 code forms a 32-bit `base + index * 0x1C` address without a
bounds check, whereas `trailSamples33E8[trailIndex]` imposed C++ array
bounds requirements. The `AppendPlayerCollisionBox` argument now uses the
native word address of the **actual** Enemy sample array, with no new check,
layout, call or owner. A pinned VC7.1 complete 3,900-byte owner probe retained
all 97 relocation fields, 69 calls, 170 direct CFG blocks and exactly the
prior 13 different register bytes. This corrects native addressing, not
byte-exact owner status. The actual /Ob0 and /Ob1 source carriers were cold-built and all **83**
exact functions replayed successfully: **29,856 complete original bytes and
1,389 relocation fields**. VC7.1 changed two compiler-private `$L` spellings
in unchanged `CompareOperands` and post-update movement. Independent
same-section COFF label-definition, original-PE operand and full-byte proofs
confirmed identical targets before the manifest spellings were updated.

## Enemy death reward spread native DWORD addition

The independently exact 1,142-byte `EnemyView::HandleDeathRewards`
(`0x004102B0`) computes the tier-zero spread from the live signed
`rewardAttack30410.rewardLevel38` plus four. The path accepts negative
levels; the original i386 addition wraps at 32 bits, whereas plain signed
C++ addition would rely on overflow behavior. The maintained source now uses
unsigned DWORD addition before restoring the signed result. A fresh pinned
VC7.1 complete-owner replay remains **exact**: all 1,142 bytes and 39
original-bound relocations agree. This is a native-semantics correction, not
a new function-exactness promotion.

## 2026-11-11 Player runtime collision-region native words and list strides

The independently exact Player UpdatePlayerCollisionRegions owner maintains
the real Player active-region pointer table at +0xB100 and released-region
pointer table at +0xB908, with separate native signed counts at +0xC110
and +0xC114. Retail i386 uses a physical four-byte pointer stride for
active and released entries and for the removal/shift traversal of the
active list. The maintained source now forms those array/iterator
addresses as unsigned 32-bit base-plus-stride words, without a new bound
check or a different region pool. It retains every original live field
read, Deactivate call, list shift, early return and embedded region
activation path.

The original lifetime/delay decrements for both dynamic and embedded
regions use native DEC DWORD instructions; these now operate through
the corresponding unsigned integer object representation, avoiding
signed C++ overflow when those timer fields contain INT_MIN. The
released-region count post-increment similarly operates on its native
unsigned DWORD word, while the active count and loop index decrements
use explicit modulo-32 arithmetic. The signed tests and recorded
integer field bit patterns remain unchanged.

Independent physical-pointer/array-slot/counter probes (including
a rejected common-helper form for the released-slot postincrement)
proved the complete original 273-byte UpdatePlayerCollisionRegions
owner and two relocation fields. The accepted direct native-released
address, unsigned count postincrement and four duration decrements
combined without introducing any byte differences. Five genuinely
affected PlayerRuntime VC7.1 profiles were rebuilt and all **21 exact
original owners / 5,391 full physical bytes / 288 original relocation
fields** were independently proven against the original PE. Eleven
private compiler-label spellings were rebound only after checking
actual same-section COFF definitions and complete original owner
bytes, then canonical affected-only cold replay passed. The larger
Player collision/ChargeAttack owners remain nonexact and the whole
Windows i386 product is not yet closed.

## 2026-11-11 Player SHT loader and AddedCallback native table addresses

The exact Player resource setup routines operate on the one actual
SHT file allocation shared with the nonexact movement/ChargeAttack owners.
The SHT header's shot-power-level table is physically variable-length,
although its C++ view declares the first 0x08-byte entry only. The
maintained LoadPlayerShtFile now obtains both original per-power-level
descriptor pointers with native Win32 base + unsigned(i) * 8 addresses.
It also resolves the four distinct shot callback pointer words from their
actual global spawn/update/draw/collision tables using the original encoded
32-bit descriptor index. The in-file descriptor relocation, four live
callback reads, fire-interval loop and original allocated SHT owner are
unchanged.

The exact PlayerLifecycleView::AddedCallback now fetches, rechecks,
releases and republishes the real two-side SHT pointer words through
native modulo-32 byte addresses. It similarly reads the genuine 0x18-byte
SHT resource path rows, five independent per-shot-type integer tables and
three installed Player callback tables from their distinct original bases.
The ANM slot argument's signed sideIndex + 5 is also explicitly summed
as a 32-bit DWORD before passing the original signed ABI word. A small
typed PlayerNativeWordSlot inline helper expresses these authentic
physical lvalue addresses while retaining the original repeated side/type
field reads, SHT reload, callbacks and VM initialization order; no input
clamping, placeholder table or synthetic buffer is introduced.

Independent source-cold pinned VC7.1 experiments verified four original
table families and three more data/callback families individually and in
combination. The 172-byte LoadPlayerShtFile owner (six fields) and the
438-byte AddedCallback owner (20 fields) were each completely byte-exact
under their original distinct profiles in every accepted variant.
After installing the maintainable combined helper shape, independent
real COFF/same-section private-label and PE-target-bound proof established
**21 complete exact owners / 5,391 physical bytes / 288 original fields**
in the five actually affected PlayerRuntime.cpp compiler carriers.
Eleven internal compiler-private label name changes were rebound only after
that full proof; canonical affected-only cold replay then passed. The large
ChargeAttack/Movement owners and whole Windows i386 product remain open;
no new exactness credit follows.

## 2026-11-11 native Front result bonus side-record addressing

`FrontFactoryConstructView::InitializeResultBonus` at the original
553-byte owner computes Player score/charge/result totals for the live
`transitionSide10964`. Twelve separate accesses to
`g_GameManager.sides[side]` or the stored `transitionSide10964` now use
the **same real GameManager 0x38-byte side allocation** as GameplaySetup,
Etama and Player, via
`uint32(&sides[0]) + uint32(sideIndex) * sizeof(native side)`.
The source keeps each original side/Player/Front read and the final
`AsciiGameManagerSideView::AddScore` call at its existing position;
there is no new bounds gate, cached side pointer, alternate score owner
or changed scoring arithmetic.

Five individual/combined pinned VC7.1 probes kept the full **553/553
physical bytes** exact, including all original-bound relocations. The
installed helper-based variant passed the one real compiler carrier's
scoped cold replay: **one exact owner / 553 original bytes / 27
relocation fields**. The game-mode and FrontScore lifetime remain
unchanged; this is native source-address fidelity, not newly exact
owner credit or Windows runtime closure.

## 2026-11-11 native FrontSide HUD VM, side and auxiliary path records

The complete exact `FrontSide::OnUpdate` at its original **2,381-byte**
owner operates on six real ANM HUD VM arrays inside `FrontSide`:
`statusVms0000`, `stockVms1D0C`, `gaugeVms2A40`, `scoreVms3F60`,
`rankVms86AC` and `counterVms9928`. Each slot is the canonical
**0x2A4-byte `AnmVm`**. Fourteen dynamic VM calls/flag updates now form
the native i386 unsigned DWORD `base + uint32(index) * sizeof(AnmVm)`
address on every original use. The exact FrontSide create function also
forms both live 0x38-byte GameManager side lvalues with native DWORD
arithmetic (`sideIndex` and original `1 - sideIndex`), and AddedCallback
reads the actual global auxiliary ANM path pointer at `uint32(base) +
uint32(g_GameManager.sides[0].auxPathIndex30)*4`. No new bounds checks,
extra ANM calls, cached side snapshots or second object allocations were
introduced, and the original HUD animation and side-selection chronology
is preserved.

Four isolated/combined pinned VC7.1 builds maintained the complete
**2,381-byte OnUpdate**, 460-byte AddedCallback and 283-byte Create
machine-code owners. The installed lexical native lvalues passed the
single actually affected compiler-carrier scoped cold replay:
**15/15 exact functions, 4,087 complete original bytes and 176 original
relocation fields**, without a manifest change. This is improved source
memory semantics on already-exact substantial HUD code, not new exact
credit or Windows runtime closure.

## 2026-11-11 FrontMessage Setup native text and shadow color words

The original exact `FrontMessageRuntimeView::Setup` owns two separate
inline four-entry color arrays, `textColors1D20` at +0x1D20 and
`shadowColors1D30` at +0x1D30. Its four `DrawTextLeft` arguments
read the two live table words selected by the genuine `u8
colorIndex1D64` at +0x1D64. The maintained source now forms native
Win32 `uint32(real array base) + uint32(color index)*4` addresses
on each independent read rather than relying on a C++ four-element
array subscript precondition. The original reset/ANM calls, text
colors, shadow colors, glyph dimensions and owner allocation remain
untouched, without bounds checks, cache or shadow tables.

Three independent/combined VC7.1 probes each preserved the complete
**357-byte original owner**. Installed source passed the only actual
`FrontMessageSetup.cpp` compiler carrier's scoped cold replay:
**1/1 exact function, 357/357 bytes, all 10 original relocations**.
No extra exact function or complete product/runtime credit was claimed.

## Front random-message selection native variable-length wire entries

`FrontMessageRuntimeView::SetupRandomForSide @ 0x004162D0` selects from a
file-backed table with 8-byte entries. The C++ wire header declares only
`entries04[1]`, but the original accesses character banks and the fallback
entries **210..219** through `table + 4 + signedIndex * 8`, with i386
modulo-32 addressing and no extra bounds check. The maintained source now
uses one local inline projection of the actual `FrontMessageTableView`
storage for all seven live pointer/weight reads. It also preserves native
DWORD subtraction for signed weights, the opposite-side 0x38-byte address,
character-bank scaling, both first-bank end tests and loop increments.
No message-validation gates, extra table owner, cached reads or random calls
were added.

A pinned VC7.1 source-cold build proves the **complete 693/693 original
bytes and all 15 target-bound relocation fields** remain exact, so this is
faithful wire-layout and integer-word behavior, not a new match promotion.

## Player ApplyReward native DWORD counters and thresholds

The original-bound `PlayerOwnerStateView::ApplyReward @ 0x0041D150` is
**1,519 authored bytes plus 21 bytes of owned physical table/tail**.
Its reward score/product/rounding arithmetic, state counter, reward meter,
spawn counter, side-number inversion, effect type sum and timer assignment
use signed results of **i386 modulo-32 operations**. The source now makes
those intermediate word widths explicit while retaining the original
signed division/modulus, loop tests, real Player owner and exact external
calls. In particular, multiplication is truncated to a native DWORD before
signed division; only the original integer mathematics is reproduced.

The combined maintained-source VC7.1 /O2 /Ob1 build, not merely isolated
probes, is **byte-exact across all 1,540 physical bytes, 91 relocation fields
and their original PE referents**. Six VC7.1-private `$L` label spellings
were rebound only after independently proving their actual same-section COFF
definitions, unchanged original target operands and the entire owner bytes.
This is faithful semantics and a preserved existing exact owner, not an
additional exact-function promotion.

## Front message file-table Setup/Load/Release native entry addressing

The existing exact FrontMessageRuntimeView::Setup, LoadForSideCount and
ReleaseForSideCount owners all read the same variable-length, file-backed
FrontMessageTableView. Its C++ projection declares only entries04[1],
but retail indexes the entire actual allocation via the signed input
DWORD and a native 0x08-byte entry stride. These three maintained sources
now recover the real base + uint32(index) * 8 read/write, without adding
an artificial array-bound condition or second allocation. They preserve
the raw-to-pointer union conversion, the two table caches and the
original signed loop/count tests.

Load and Release also test/update a 256-byte stack music marker using
the script instruction's signed music ID as a native byte offset rather
than a bounded C++ array subscript. The startIndex + 10 endpoint and
the original loop post-increments use native 32-bit wrapping addition
before each signed bound comparison. Neither music call ordering nor
the original empty-entry, free or error paths changed.

Pinned VC7.1 /O2 /Ob1 individual and combined source experiments
reproduced the complete owners: **357 / 405 / 271 physical bytes** with
**10 / 18 / 14 original-bound relocation fields**, respectively.
All three affected exact compiler carriers were independently
cold-replayed and passed: **1,033 full physical bytes, 42 reviewed
relocations, three existing exact functions**. This is native
source-semantics recovery, not promotion of a new exact owner.

## Title key configuration native per-side byte settings and help VM slots

The complete original exact OnUpdateKeyConfig at 0x00427EE8 is a
2,803-byte switch owner with 118 reviewed address/relocation fields.
The key/controller UI uses the real Supervisor configuration at +0x388.
In particular valueB7 is **one byte** in that native configuration,
not a two-element C++ array: the retail side-selected read/write uses
an i386 DWORD offset from the address of valueB7, including the
opposing-side selector computed as 1 - keyConfigSide modulo 2^32.
The side-focus mode byte array is at its own genuine address; it too
is indexed by the native side word rather than a bounded C++ subscript.

The maintained owner now uses explicit native byte addresses for those
two settings, plus the keyboard-selected VM in the actual 57-entry
helpTextVms allocation (0x2A4 stride) and the controller input byte
indexed by the live short key code. No bounds clamping, new storage,
new input reads or other behavior were introduced. The 0x36-byte
key-mapping side view and the help-text pointer table were deliberately
left alone: tested typed pointer rewrites of those other views displaced
original instruction bytes or physical extent.

Direct DWORD byte-pointer and grouped lexical-expression forms were
compiled under the pinned VC7.1 /O1 /Ob1 profile. The combined maintained
source retains the **entire 2,803/2,803 physical bytes and 118 relocations**
of OnUpdateKeyConfig and the independent exact
SwapDuplicateCharacterIndex 106/106-byte leaf with no relocations.
These were already exact functions: there is no new exactness credit.

## Title MusicRoom selected track and unlock flags native addresses

The existing exact TitleScreenView::OnUpdateMusicRoom owner
(0x00426E05) uses the real 32-track TitleScreen music table: each
file-backed music record is 0x292 bytes. Its live signed
musicSelectedSongIndex is now converted into the original Win32
byte address (&musicTracks[0] + uint32(index) * 0x292) for the
eight description renders and selected replay path. The per-song
unlock tests and script-generated vmIndex likewise read the
actual 32-byte g_TitleBgmUnlocked global at a native DWORD byte
offset. No music-record guard, second cache, fabricated song record,
or reordered sound calls have been introduced.

Pinned VC7.1 /O1 /Ob1 trial builds found a source-sensitive codegen
distinction: casting the record base to unsigned int changed four
unrelated VM SIB operand bytes (0x38 to 0x07); a true byte-pointer
base in a lexical expression restores full original codegen. Applying
a second pointer change at the keyboard-selection/track-parse sites
also made those four original SIB bytes differ, so those speculative
changes were rejected. The installed combination covers selected
record and unlock reads without displacing the original loop schedule.

Complete target PE/COFF comparisons of the affected source retain
the preexisting 2,258-byte OnUpdateMusicRoom exact owner with
74 relocation fields, plus its separate 209-byte DrawMusicRoom
exact leaf with six relocation fields. This corrects native
source-address semantics without promoting any new exact function.

## Front message 2,968-byte Update native wire indexing

The existing exact FrontMessageRuntimeView::Update physical owner at
0x00416590 is 2,852 instruction bytes plus 116 owned switch-table
bytes. Script-provided signed indices select real message portrait pairs
(0x548-byte stride), text VMs (0x2A4-byte stride), text/shadow palette
DWORDs, and 0x38-byte GameManager side records. The maintained source now
takes every dynamic slot through its original base + signed-index-scaled
native i386 DWORD address, rather than assuming C++ array bounds. This
includes the instruction's side argument, selected or alternate
portrait slots, text line and color script words; literal fixed slots
remain unchanged. Three signed opposing-side expressions also explicitly
use native wrapping DWORD subtraction.

The portrait and side lexical inline views keep the native owner and
object storage; scalar palette/text VM accesses use a single lexical
address macro because callable inline replacements changed VC7.1's
register/switch-table layout. No new table, index check, synthetic
helper call or original program bytes were introduced.

All parts were probed individually and in combination. The final
maintained-source pinned VC7.1 /O2 /Ob1 build reproduced the complete
**2,968 / 2,968 original physical bytes** with **167 original-bound
relocation fields**. Thirty compiler-private local/jump-table label
spellings were re-bound only after proving their actual same-section
COFF definitions, unchanged original PE destination operands and
complete function equality. Both affected compiler carriers were
cold-replayed: the full Update owner and independently exact 39-byte
DecodeFrontMessageString leaf, totaling **2 exact functions / 3,007
physical bytes / 167 relocation fields**. The function was already
exact: this improves native wire semantics without changing exact-function
credit.

## ASCII and Front enemy marker leaf native slot pointers

The two AsciiManager Boss Marker VM leaves SetBossMarkerInterrupt
(0x004067D0) and SetBossMarkerPosition (0x004067F0) use the
real four-entry AnmVm array at +0x17C4. The separate
SetBossMarkerState leaf (0x0040F890) writes one of four 32-bit state
words at +0x2254. Their signed slot selectors are now scaled modulo
2^32 from those actual allocations, with 0x2A4-byte VM records and
four-byte state words, rather than assuming C++ in-bounds subscripts.

The FrontFactoryConstructView::SetSideEnemyIndicatorPosition
(0x00416080) uses two actual marker VMs at +0x123F4 and the
GameManager's real pair of 0x38-byte side records. Its signed
sideIndex is now used for the original native byte addresses for
all six marker writes and the opposing Player X lookup, keeping
the existing transparent/offscreen branch, playfield threshold and
original Draw/Transform calls. No extra range checks, aliases to
other allocations, new branches or changed write order were added.

Pinned VC7.1 /O2 /Ob1 isolated probes verified every owner after
native addressing. The maintained source cold-replayed all
three affected compiler carriers: **four exact function bodies,
307 complete physical bytes and 11 original relocation fields**.
This recovers authentic native leaf semantics; all four were
already exact before the change.

## 2026-11-11 native ExAttack Type14/22 fan and random rows

The pinned `ExAttackUpdateCallbackType14_22` works on the same real
0xF48-byte extra state used by the original Type14/22 draw and
initialization callbacks. The stage-three fan generation writes the
first of 33 actual **0x14-byte vertex entries** in each of four
`fanGroups2E8` rows, with physical row stride **0x294**. Its random
phase writes one of 33 four-byte floats in each of four `randomD38`
rows (stride **0x84**). Each loop index forms the native Win32 DWORD
address from the actual allocation base, not a bounded C++ projection.
Five further live gameplay side lookups (one `side08`, four
`opponentSide04`) now address the authentic `g_GameManager.sides`
**0x38-byte** records by signed DWORD scaled offset. The original
`GetRandomF32SignedInRange` calls, ANM script, collision tests, phase
transitions and source chronology remain untouched; no range guards,
cache, shadow fan storage, call or extra view was introduced.

Four individual and combined pinned VC7.1 `/O2 /Ob1` builds were
attested against **all 824 complete physical owner bytes** (805-byte
canonical body and owned tail), including **44 original relocation
fields**. All four remained byte-identical after checking real
same-section COFF definitions. The installed lexical macro form
passed the only relevant carrier's **1/1 exact, 824/824 bytes and 44
fields** scoped cold replay. Its five `$L` symbol renames were rebound
only after confirming unchanged field offsets/types and original PE
jump destinations. No new exact owner or Windows runtime claim follows.

## 2026-11-11 native ExAttack Type3 history, VM and side selectors

The original exact `ExAttackUpdateCallbackType3` handler has **780
physical bytes** and shares the authentic 0x190-byte Type3/Type4 extra
layout: one 0x04-byte state, one 0x0C-byte motion vector, and 32 genuine
0x0C-byte `Float3` history records starting at +0x10. The update's
moving history index and previous slot now form native unsigned-DWORD
byte addresses from the real array base, with the predecessor at
`current byte address - sizeof(Float3)`. Nine ANM VM position/interrupt
receivers likewise use the real 0x2A4-byte `AnmVm` allocation,
and three enemy-collision receiver lookups use the true
`g_GameManager.sides` 0x38-byte records and the signed live
`opponentSide04` word. The original loops, short-circuit three-part
collision test, exit conditions and callee identities remain untouched.

The independent history/VM/side and combined pinned VC7.1 probes all
matched the complete original owner. The installed source passed
**1/1 exact unit, 780/780 complete physical bytes and 38 original-bound
relocation fields** in the sole genuinely affected compiler carrier.
The Type3 shared-layout header's stale claim that this update is nonexact
was corrected after verification. No new exact-function credit or native
runtime/product claim is implied.

## 2026-11-11 ExAttack Type4 native trail, VM and opponent-side lvalues

The complete exact `ExAttackUpdateCallbackType4` owns a 739-byte TH09
handler over one real +0x10 `Float3 history10[32]` allocation (0x0C-byte
entries), the original `record->vms1C` 0x2A4-byte native ANM VMs and the
shared 0x38-byte `g_GameManager.sides` records. The maintained source
now computes the moving history index and its immediate previous record
using unsigned-DWORD native byte address words. It likewise uses the
real live VM pointer for nine script/position/interrupt receiver accesses
and the original signed `opponentSide04` for five collision-query side
receivers. The original loops, method calls, state/timer conditions and
five opponent collision checks remain in place, without adding bounds
checks, side caches, copies or independent allocations.

Four separate/combined VC7.1 probes each preserved the complete 739-byte
physical owner, and the installed combined source passed **1/1 exact
function, 739/739 original bytes and 28 independently bound relocation
fields** on only the relevant compiler carrier. No manifest rebindings
were necessary. This is a native-layout fidelity improvement on a
previously exact attack callback, not a newly exact function or native
product/runtime proof.

## 2026-11-11 ExAttack 18/24 native history and ANM VM byte addresses

The exact `ExAttackUpdateCallbackType18_24` owner (1,436 target bytes)
initializes and shifts 16 real `Float3` trail positions at extra +0x58
(0x0C-byte records), 16 independent float trail angles at extra +0x118
(4-byte records), and updates four native 0x2A4-byte `PlayerAnmVmView`
objects in the attack's actual `dynamicData1C` allocation.
The maintained source now forms the real Win32 `uint32(base) +
uint32(index)*native_stride` lvalues for three dynamic position loads/stores,
three dynamic angle loads/stores and fifteen exact VM method/field receivers.
The previous position slot is addressed as **current native word address
minus one original 0x0C-byte record**, avoiding an out-of-bounds C++ index.
The original 16-entry loops, sequence of four ANM script calls,
independent interpolation reads, rotation and interrupt calls remain in
place; no new allocation, bounds gate, copy, or cached pointer was added.

Initial i-1-base expressions shortened the owner by one byte; four
current-slot/previous-slot source forms retained the complete original
body. The installed lexical native version was verified with the *only*
actual pinned VC7.1 `/O2 /Ob1` compiler carrier: **1/1 exact function,
1,436/1,436 original physical bytes and 44 original-bound relocation
fields**, without a manifest change. This improves native memory-model
fidelity of an already-exact substantial boss, not the exact-function count.

## ExAttack Type18/24 update native GameManager side and state counter

The full ExAttackUpdateCallbackType18_24 original physical owner
at 0x004491E0 is 1,436 bytes with 44 original-bound relocation fields.
It reads the actual GameManager side record by the signed side08 word
for four ANM calls, and the native opposing-side04 word for both
AngleToPoint and collision/target queries. A local source inline view
now forms the genuine base + uint32(side)*0x38 record address from
g_GameManager.sides[0], without a C++ bounded-subscript assumption
or an invented side owner. No new call, branch, allocation or clamp
was introduced. The native attack state transition also explicitly
increments the signed state00 using a 32-bit wrapping DWORD ADD.

Independent pinned VC7.1 /O2 /Ob1 /Oi source experiments tested
integer address casts, typed byte pointers, side subsets, explicit
counter wrap and the combined native inline helper. The installed
combined function retains **all 1,436 original physical bytes and
44 original PE/COFF relocation fields**. This is a faithful Win32
source-semantics repair to an already exact large owner, not another
new exact function.

## ExAttack draw native VM record indexing

Both ExAttackController::OnDraw0 (0x00415460) and OnDraw1
(0x00415580) render a live array of ANM VMs, whose physical
0x2A4-byte record count is read from the ExAttack record at +0x2C.
The existing source used 16 C++ subscripts on vms1C[vmIndex].
The current lexical TH09_EXATTACK_DRAW_VM expression explicitly forms
the original i386 native address from the actual record-owned VM
pointer plus uint32(vmIndex) * 0x2A4. It retains the distinct type,
rotation, position, Draw2D and draw-callback operations in each
original list path, with no range clamping or extra helper call.

The pinned VC7.1 /O2 /Ob1 trials compared all record uses.
Direct DWORD, byte-pointer and char-pointer addresses all emitted
the exact original two owners. A callable inline VM accessor
lengthened OnDraw0 and was rejected; the lexical native expression
preserved **OnDraw0 286/286 bytes and nine relocations, and OnDraw1
193/193 bytes and five relocations**. The source is already exact;
this recovers the explicitly native word/record-address semantics,
not a new exact-function promotion. The installed source separately
cold-replayed **2/2 owners, 479 physical original bytes and 14
relocations** in its single affected VC7.1 compiler carrier.

## ExAttack callback difficulty limits and side-state native words

ExAttackController::AddedCallback at 0x00415750 reads each side's signed
selector from the genuine 0x38-byte GameManager side records and forms
the difficulty-scaled spawn limit from the real global int32 coefficient
and base tables. The maintained source now reads the table words with
native DWORD base + uint32(selector)*4 addressing and performs the
coefficient*difficulty+base calculation modulo 2^32 before storing the
signed limit. This avoids a C++ bounded-array precondition or signed
overflow assumption while retaining the two separate selector loads,
real tables, original 9 relocation fields and existing Reset callback.

The independent ExAttackRecordSideView::GetSideState exact leaf at
0x00440CB0 also forms the live GameManager side pointer by unsigned
DWORD(index)*0x38 from the actual two-row allocation, rather than a
bounded array subscript. No memory backing, side clamp, call, or
original data identity was introduced.

Isolated pinned VC7.1 codegen probes remained exact for all **three**
owners in these two TUs: AddedCallback 66 bytes / nine relocations,
ResetExAttackRecords 88 bytes / two relocations, GetSideState
12 bytes / one relocation. These are semantic improvements to existing
exact sources; no new exact-function promotion was claimed.

## ExAttack Spawn native type/side dispatch words

ExAttackController::Spawn at 0x004151F0 is an existing 321-byte exact
owner. Retail's template dispatcher uses the signed type DWORD shifted
four bits, then reads the original callback words at 0x004A0F78..0x004A0F84
(no C++ bounds check). The controller's side limit is a native index into
its real two-word limit table (this + 0x14, four-byte stride). The
opponent-side selector is computed by native i386 (1 - side) DWORD subtraction.
Source now expresses these actual 32-bit address/arithmetic semantics instead
of relying on C++ signed-array bounds or overflow rules, without introducing
guards, a second table, or changing the callback/lifetime order.

Fresh pinned VC7.1 combined-source comparison retained all 321 bytes and
all seven original-bound relocation fields. Other exact functions from
ExAttackController.cpp require affected-carrier replay before acceptance.
This corrects native semantics, not the exact-function count.

## CardAttack/Player preload/Laser leaf native table addresses

The six independently exact CardAttackBeginPl00/01Type0/1/2 callbacks
use the genuine GameManager.sides[0] array to find the side's Enemy
manager. Their signed sideIndex00 is now interpreted as a native 32-bit
word scaled by the original 0x38-byte side record stride without a
C++ array-bound gate; the original six SpawnFromCardAttack calls,
allocation paths, and target relocations stay unchanged.

PreloadPlayerAnmResources selects a path from the real global six-column
ANM resource path table by the signed live shotType20 DWORD and the
mode-selected pathIndex (1 or 5). The source now addresses that actual
table at base + uint32(shotType)*24 + uint32(pathIndex)*4 instead of
using two bounded array subscripts. Each original receiver refresh,
lookup, allocation and error path remains in place.

The exact EtamaController::SpawnLaserPattern owner reads the integer
color sprite offset from the true g_LaserColorSpriteOffsets table by
the signed descriptor color. A native base + uint32(color)*4 lookup
replaces the bounded C++ subscript without changing any laser spawn,
ANM or Player aim handling.

Isolated pinned VC7.1 native-address and source-helper probes retained
all **eight existing exact owners**: CardAttack callbacks 456 bytes
and 30 relocation fields, Player resource preload 114 bytes / six
fields, Etama laser spawn 450 bytes / six fields. The callable
resource-path helper changed four unrelated bytes and was rejected;
the lexical native address expression preserved the original copy.
All three affected compiler carriers passed scoped cold replay:
**8/8 exact, 1,020 complete physical bytes, 42 original relocation
fields**. None of these were newly exact functions.

## 2026-11-11 native Etama sprite-script row and column reads

The exact **832-byte** `EtamaController::AddedCallback` initializes 23
real sprite templates by reading a private `kBulletSpriteScripts[23][5]`
table. Its seven live reads (five distinct startup scripts and two
later sprite-type switches) now form the authentic Win32 byte address
`uint32(&table[0][0]) + uint32(row) * 0x14 + uint32(column) * 4`.
For the second script column the source retains the **const int element
lvalue** at the original column-2 row base; VC7.1 `/Ob0` otherwise
coalesces its source expression and shortens the callback by 20 bytes,
violating the complete physical owner. All other six reads use lexical
native-DWORD addresses. No bounds check, new script table, shadow data,
extra function call or altered switch branch is introduced.

Individual column probes established that column 0/1/3/4 lvalues preserve
the full original 832-byte body (30 relocation fields). A combined
column-2 const-reference variant also matches **832/832 bytes** and all
original relocation offsets/kinds/targets. The installed source passed
the one actual `EtamaControllerDraw.cpp` `/Ob0` compiler carrier:
**3/3 exact owners, 1,664 complete physical bytes, 73 relocation
fields**. Eleven compiler-private `$L` references were rebound only
after independently confirming same-section COFF definitions and the
unchanged original PE destinations. This is native script-table indexing
fidelity; it does not promote any new exact function or runtime claim.

## 2026-11-11 native Etama draw bucket and bullet-template records

The complete exact `EtamaController::OnDraw` reads the six real draw-bucket
head pointers at `EtamaController+0x25E198`; the exact `AddedCallback`
iterates the first 23 entries of the authentic `bulletTypeSprites[32]`
allocation. The maintained code now computes the two live addresses as
`uint32(real base) + uint32(i) * sizeof(native record)` using the true
four-byte draw-head and **0xD48-byte `BulletTypeSprites`** strides. This
preserves independent live reads, original loop bounds and all draw/ANM
calls; there is no new clamping, allocation, copy or stored view.

Native table rewrites for `kBulletSpriteScripts[23][5]` were rejected:
that spelling shortened the exact `AddedCallback` owner by 20 bytes.
The two accepted lvalues cold-replayed only the actual /Ob0 compiler
carrier. All **three exact functions, 1,664 original physical bytes and
73 relocation fields** matched completely. Fifteen affected compiler-
private `$L` references in `DrawSingleBullet` and `AddedCallback` were
rebound only after confirming the actual same-section COFF definitions and
unchanged original PE destinations. No additional exact owner was claimed.

## 2026-11-11 Etama native side and sprite-offset table selectors

`EtamaController::Register` stores two pointers into the original
`g_GameManager.sides` allocation, using the genuine `0x38`-byte side
stride. Both the caller-supplied signed `sideIndex` and the retail
opponent-side `1 - sideIndex` calculation now use native Win32 32-bit
address/word arithmetic, rather than potentially out-of-range C++ array
subscripting. `EtamaController::SelectBulletSprite` and
`SpawnSingleBullet` also fetch four independently live color/offset
entries through real small/medium sprite-offset DWORD tables. They use
the original signed runtime color/offset, without clamping, shadow arrays,
new calls or additional caching.

Three isolated and combined pinned VC7.1 compiler profiles were built.
The installed source passed the official three-carrier scoped cold replay:
**18/18 exact owners, 6,711 complete physical bytes and 259 original-bound
relocation fields**, including the 283-byte `Register`, 161-byte
`SelectBulletSprite`, 203-byte primary bullet pattern and full 1,932-byte
`SpawnSingleBullet`. The two large `OnUpdate`/`SpawnSingleBullet` switch
owners had **16 compiler-private `$L` reference renamings**, all rebound
only after verifying the real same-section COFF definitions and the
unchanged original PE addresses, kinds and offsets. Existing exactness is
preserved; no newly exact function or runtime/port claim is implied.

## Bullet transform installer leaves and AdvanceTransformProgram native indices

The independent exact Bullet and BulletSpawnDescriptor InstallTransformRecord
leaves at 0x00412610 / 0x00412650 receive a signed transform index, without
a lower-bound test. Each record is physically 0x18 bytes in its actual
18-element transform array. The maintained code now explicitly computes
native modulo-32 base + DWORD(index) * 0x18 instead of relying on an
in-bounds C++ array subscript. Pinned VC7.1 /O2 /Ob1 builds preserve
both complete owners, 54 and 37 bytes, with no relocations.

The much larger existing exact Bullet::AdvanceTransformProgram at
0x004141C0 uses the same native 18-record storage and accepts a signed
current transform index. It now computes that record's modulo-32 address
and explicitly wraps index + 1 before storing the next DWORD. Its fresh
full original-bound PE/COFF comparison retained **1,320 / 1,320
physical bytes** (including the complete owned tail/table) and
**all 26 relocation fields**. Seven private VC7.1 switch-label
renames were verified against actual same-section definitions,
independent original PE operands and full-body equality before
rebinding the manifest. Exact-function totals do not change.

## Player movement focused input uses the real side-slot stride

The 1,900-byte still-nonexact Player movement owner reads the 0x2A
auxiliary focus counter from the original ReplayInputState array at
0x004ACE18. The signed side DWORD scales by the native 0x8E-byte record
stride and the input pointer is read again at the focus decision point;
the old g_ReplayInputStates[inputSide] expression incorrectly required
a valid C++ array subscript. The maintained source now computes the
genuine Win32 base + DWORD-scaled address for that separate read.
Using the earlier cached input receiver was rejected by the complete
COFF operand checker, so no new read-caching or bounds gate was introduced.

The pinned VC7.1 complete 1,900-byte / 66-field / 23-call owner
comparison retains the previous 122 different bytes and the same original
field identities. It is a native wire-access correction, not new
byte-exact function credit.

## 2026-11-11 Player movement native option/history record addresses

`PlayerLifecycleView::UpdateMovementAndOptions @ 0x0041C170` steps through
four real option records at `Player+0x1CEC`, each `0x2F4` bytes, then shifts
the sixteen actual `Player+0x1BA0` position history records, each `0x0C`
bytes. The maintained pointer increment now forms an unsigned i386 DWORD
`option address + sizeof(option record)`; the history reads and writes use
one inline lvalue from the authentic history array base plus
`uint32(index) * sizeof(history record)`. Each read/write stays at the
original site; the original finite counts, call order, callbacks and
history-copy predicate are unchanged. This records native address transport
without inventing a second allocation, boundary check or helper function.

Three isolated/combined pinned VC7.1 source probes matched the complete
**1,900-byte** maintained movement candidate against the original-bound
physical owner, including both original eight-entry switch tables and
**66 relocation fields / 23 ordered calls**, with precisely the former
**122 ordinary register/transport differences**. A separate cached history
pointer trial regressed to 1,896 bytes / 191 differences and was rejected.
The installed source passed the targeted `--owner movement
--skip-exact-siblings` full-owner guard; this standalone nonexact TU has no
existing exact siblings to cold replay. No movement exactness credit or
whole-product/runtime completion is implied.

## Player movement, charge and collision

The still-nonexact 1,900-byte Player movement owner reads the focus-effect
ID from the native table at `0x004A1A10`. Original instruction
`0x0041C322` is `mov ecx, [edx*4 + 0x004A1A10]`: it scales the signed
side-state `shotType20` DWORD by four in native Win32 address arithmetic,
without a C++ array-bound precondition. The maintained source now computes
that modulo-32 byte address from the actual table base, with no new guard or
replacement table. A fresh VC7.1 full-owner comparison retained 1,900 physical
bytes, all 66 original-bound relocation fields, all 23 calls, and exactly the
prior 122 unmatched bytes; this is a wire-address fidelity correction, **not**
a new exact owner.

Movement remains 1,900 bytes/66 fields/122 differences. Horizontal and vertical
speed already occupy the original EBP-8/EBP-4 homes. Several residuals are
register choices in bounds clamps and position/history transport, not swapped
speed locals. Preserve native option, position-history and focus Effect owners;
old 129-difference snapshots are obsolete.

ChargeAttack now covers the full original 1,210 bytes, 70 fields, 58 calls,
350 instructions and 53 direct blocks, with 235 ordinary differences. Earlier
1,195/1,197-byte candidates missed extent and are superseded. The remaining
question includes the mode byte at EBP-1 and input receiver lifetime. Its
complete CFG is recovered; do not reopen it as a missing-tail problem.

CheckBulletCollision remains 849 candidate bytes against 862 original, with
26 fields/12 calls and 799 differing overlap bytes. Use the genuine region
allocator and reconstructed collision pool when investigating branch chronology.
Do not introduce a replacement pool to improve codegen.

## 2026-11-11 Effect pool native 0xD8-stride byte addresses

The real `EffectManager` owns 0xD8-byte effect records and returns the
allocation's one trailing sentinel on a full-pool spawn failure. Four
source sites previously formed dynamic C++ pointers using signed pool
indexes: the `EffectNativePoolCapacity::FailureSentinel` helper,
`SpawnEffect`, `SpawnEffectWithVelocity`, and `SpawnEffectInFixedSlot`.
Each now forms the **original Win32 unsigned 32-bit byte address** from
the observed live effects allocation base and native record stride. The
fixed slot's `primaryCount + slotIndex` similarly wraps as native DWORDs
before scaling, rather than depending on signed C++ overflow and array
subscript behavior. These expressions preserve source-visible live field
reads, caller arguments, short-circuit decisions, return values and the
real failure sentinel, without inventing a second pool or expanding it.

Four independent and one combined pinned VC7.1 probes preserved all **15
original-bound exact owners (2,530 physical bytes, 100 relocation fields)**
and the separate 51-byte `EffectManager::AddedCallback` retained its four
real fields, two calls and eight current nonexact register bytes. The
installed combined source re-passed the one actually affected compiler
carrier's complete strict cold replay and the callback boss guard. This
is a source-ownership/native-address improvement, not a newly exact
function or whole-product closure claim.

## 2026-11-11 BeginRecordingStage native signed stage-slot addressing

`ReplayManagerView::BeginRecordingStage @ 0x00421140` reads the live signed
`g_GameManager.field0FC` stage. The retail VC7.1 code calculates native
Win32 pointer-word addresses from each of the three actual ten-stage frame
row bases and the FPS base, plus a 0x18-byte stride into the manager's inline
stage-buffer links. Ordinary `table[stage]` C++ subscripts imposed array
bounds requirements that the original i386 address math does not have. The
maintained source now forms each native 32-bit base + uint32(stage) * sizeof
slot address before reading/freeing/replacing the four stage pointers and
both link receivers. It preserves separate live reads, original call order,
allocations, failure paths and source storage identity, with no extra guards.

All five families were compiled separately and in combination under pinned
VC7.1. The **complete 1,323-byte BeginRecordingStage owner and all 82
original-bound relocation fields remained exact** for every individual and
combined variant. Both genuine ReplayInput compiler profiles then replayed
**12 exact owners / 4,093 complete original bytes / 232 relocations**.
The still-nonexact SaveReplay stays 1,238 bytes, with the same two differing
bytes at +0xB7/+0xBB and all 53 fields; BeginPlaybackStage stays 538/540
candidate/target bytes with 39 overlapping differences and 32 fields. This is
semantic address fidelity, not a new exact owner or runtime/product proof.

## 2026-11-11 Replay SaveReplay native frame, FPS and stage slot words

The still-nonexact SaveReplay at 0x00420C60 serializes the genuine
three-by-ten stage frame-pointer and offset tables plus ten FPS-pointer
and offset slots from ReplayDataView. Their signed stream/stage indices
use original native i386 modulo-32 byte addresses from the real array
bases, rather than potentially invalid C++ array subscripts. The two
serialized frame-table reads and frame-offset write use the true 10-stage
row width and four-byte word size. The FPS reads and writes use their
actual one-dimensional table bases. Both stage-buffer link lookups use
the original 10-stage ReplayManager allocation and real
sizeof(ReplayBufferLink) stride, while retaining distinct live reads,
source chain ordering, original loop bounds, memcpy sizes and offset
bookkeeping. The original serializer's allocation, compression, mode,
user-info and checksum behavior has not been changed.

Six independent table-family native-address probes and three combined
variants reproduced the complete **1,238/1,238-byte SaveReplay candidate,
all 53 bound fields, 23 calls, 49 CFG blocks and precisely its prior
two differing register bytes at +0xB7/+0xBB**. Installed-source
original-bound full-owner checks confirm those same two mismatches;
the separate nonexact BeginPlaybackStage remains 538/540 bytes with
32 fields and 39 ordinary overlaps. The two genuinely affected
ReplayInput compiler profiles cold-pass **12 exact sibling owners,
4,093 complete original bytes and 232 real relocation fields**.
No SaveReplay exactness credit or complete native-product claim follows.

## 2026-11-11 native Replay playback marker and compressed checksum reads

The still-nonexact `ReplayManagerView::BeginPlaybackStage @ 0x00420840`
marks previously cleared characters using the real GameManager marker byte
array at +0xD8. The index is read live from the original previous-stage
frame header's `value06` wire byte, with no retail range check. The source
now uses `uint32(&clearD8[0]) + uint32(header->value06)` to address the
same physical marker rather than relying on a 16-element C++ array bound.
The still-nonexact `SaveReplay @ 0x00420C60` similarly counts checksums
through the actual `Lzss::Encode` allocation using native unsigned-DWORD
byte offset `uint32(compressedData) + uint32(loopIndex)`. Its original
signed loop exit, source byte reads, checksum and serialization order
remain unchanged; no extra allocation, guard, or cached data was added.

Three isolated/combined pinned VC7.1 probes all retained the complete
**538-byte BeginPlaybackStage candidate vs 540-byte original** (32
relocations / 39 unlike overlapping bytes / two missing tail bytes), and
the **1,238-byte SaveReplay candidate vs 1,238-byte original** (53 fields,
23 calls, 49 blocks and only existing +0xB7/+0xBB unlike register bytes).
The installed combined source passed both original-bound boss guards and
the **two actually affected ReplayInput compiler carriers: 12/12 exact
siblings / 4,093 complete physical bytes / 232 relocations**. These are
native data-lvalue reconstructions, not newly exact owners or runtime proof.

## 2026-11-11 BeginPlaybackStage native frame/FPS table slot addresses

The nonexact 540-byte `ReplayManagerView::BeginPlaybackStage @ 0x00420840`
reads three stage-frame header pointers, one FPS stream pointer and the
prior-stage clear marker from the real, 0x1EC-byte replay layout. The
original i386 loads these words through native 32-bit **row base + stage *
sizeof(pointer)** addresses; the signed game-stage word is not clamped
before the original read. The maintained source previously expressed the
five reads as C++ `frameStart[row][stage]` and `fpsStart[stage]`
subscripts, which depend on in-bounds array indexing. All five reads now
form the actual unsigned 32-bit byte addresses from the independently
verified `frameStart[0/1/2][0]` and `fpsStart[0]` subobject bases, followed
by the same pointer-word load. The prior-stage loop continues to reread its
live index and the current-stage pointers retain their original ordering.
No bounds check, null check, backing array, input cursor, seed call, or
failure behavior was added or removed.

Five isolated and three combined pinned VC7.1 compiler probes reproduced
the complete existing **538-byte candidate against the 540-byte original**,
all **32 target-bound fields**, five ordered calls, full direct CFG, **39
unlike overlapping bytes**, and the same two still-missing target bytes.
The installed combined source cold-replayed **both genuine ReplayInput
compiler carriers**: **12 exact sibling functions / 4,093 full original
bytes / 232 relocation fields**. The separately original-bound 1,238-byte
`SaveReplay` owner retained all 53 fields, 23 calls and precisely its old
two differing register bytes `+0xB7/+0xBB`. These are meaningful native
wire-address semantics, not newly exact owners or whole-product closure.

## Replay SaveReplay native name-table indexes

The still-nonexact ReplayManagerView::SaveReplay (0x00420C60) formats
replay metadata using the signed difficulty, mode and side-character
indices from live GameManager words. Retail addresses the real
g_ReplayDifficultyNames / g_ReplayModeNames / g_ReplayCharacterNames
pointer arrays by 32-bit base + uint32(index)*4. Ordinary C++ table
subscripting imposes a bounded-array precondition that was not present
in the original Win32 lookup. The maintained source now uses one
lexical native DWORD slot expression for five reads at their original
execution points, retaining separate side-0/side-1 inputs, format-call
order and original table identities. A callable inline accessor altered
VC7.1 register scheduling; the lexical form did not add a new call,
guard, array or temporary cache.

Direct native-address probes and the combined lexical form were
compiled with pinned VC7.1. They preserve the **full 1,238-byte
original-owned SaveReplay extent, 53 original-bound relocation fields,
23 ordered calls, 49 CFG blocks and precisely the previous two
different register bytes at +0xB7/+0xBB**. The function remains
NONEXACT. The installed source passed the two existing ReplayInput
compiler carriers: **12 previously exact functions / 4,093 full
original bytes / 232 actual relocation fields**; BeginPlaybackStage
still has its existing 39 unlike bytes in 538/540 candidate bytes.

## Replay SaveReplay and BeginPlaybackStage

SaveReplay is 1,238 bytes/53 fields/23 calls, with only +0xB7/+0xBB differing.
The original loads destination into EDI and then computes the EDI address;
the candidate loads ECX and computes EDI from ECX. Frame-copy size and serialized
cursor advance both use `sizeof(*frameHeader)`.

Typed/indexed copies, local cursors, eight-word copies, const/mutable headers,
signed/unsigned/literal offset arithmetic and declaration-order trials retained
the two bytes. Volatile cursors were rejected. Do not add volatility or register
hints to force EDI.

BeginPlaybackStage remains 538 candidate bytes against 540 original, with
32 fields, five calls, 39 differing overlaps and two missing bytes. The target
seed schedule is `xor edx,edx; mov dx,[eax+4]; push edx`. Wider SetSeed formals,
union/two-half views, pointer/reference receivers, const frame headers and
zero-plus-memcpy seed locals did not close it. A wider SetSeed implementation
can itself be byte-identical: callee bytes do not uniquely determine formal
width. Keep the pre-runtime-read snapshot of side-0 value1E8.

## 2026-11-11 native Title replay-record, path and allocation slots

The complete, exact `TitleScreenView::OnUpdateReplayMenu @ 0x0042689A`
uses three live allocations inside the same genuine Title object:
`replays[50]` at `+0x68E4` with 0x1EC-byte wire records,
`replayPaths[50][0x200]` at `+0x28C`, and `replayAllocations[50]`
at `+0x681C` with genuine four-byte pointers. The maintained source now
forms the original Win32 unsigned-DWORD `base + uint32(index)*stride`
addresses for the dynamic scanned/selected replay metadata, string paths
and owned pointers. The actual cursor and scan indices, repeat reads,
file enumeration, allocation/free lifetime and failure branches are
unchanged; there is no shadow pool or extra bounds gate.

Isolated and combined VC7.1 /O1 `/Ob1` source probes retained the full
**1,387/1,387-byte exact replay-menu update** and **1,825/1,825-byte
exact replay-save update**, including their original relocations. The final
source-cold replay checked both actually affected TitleScreen carriers:
**41/41 exact functions, 18,972 complete original bytes and 953 relocation
fields**. Nine `OnUpdateStartMenu` compiler-private `$L` names were updated
only after confirming each actual COFF same-section definition, original
PE destination and unchanged relocation offset/kind. This does not add
new exact owner credit; the separate 484/485-byte `OnUpdateResult`
remains nonexact by 218 overlapping bytes and one absent target byte.

## 2026-11-11 native Title score-name table bank/row/record addresses

The original global `g_TitleNameTable @ 0x004A8398` is a three-level
`TitleNameRecordView[][5][5]` allocation: a genuine 0x2C-byte record, a
0xDC-byte five-record row and a 0x44C-byte five-row bank. Two distinct
Title update owners use its **seven live reads/writes** of the selected
`nameBankIndex`, `g_TitleNameTableIndex` and `nameSlotIndex`. The maintained
source now shares one inline lvalue that computes the authentic base plus
`uint32(bank)*0x44C + uint32(table)*0xDC + uint32(slot)*0x2C`, each term
in Win32 modulo-32 arithmetic. It preserves separately evaluated live
indices and all original per-character reads/stores. It neither clamps
inputs, caches the chosen record across callbacks nor creates a new table.

Both direct complete-owner probes stayed byte-exact:
`OnUpdateResultNameEntry` **966/966 bytes** and the replay-save update
**1,825/1,825 bytes**, including their original relocation owners.
Installing the combined source passed the pinned VC7.1 **41/41 exact
TitleScreen units, 18,972 complete physical bytes, 953 relocation fields
in two actually affected compiler carriers**. Nine private `OnUpdateStartMenu`
`$L` symbol names shifted; each canonical rebind was accepted only after
checking the same-section COFF definition against the unchanged original
PE destination, followed by full 41-owner replay. The separate nonexact
`OnUpdateResult` remains 484/485 bytes and 218 unlike overlapping bytes.
This advances native source data/layout fidelity but no new exact-owner,
link, or runtime claim.

## 2026-11-11 Title result native character-order cursor lookup

`TitleScreenView::OnUpdateResult @ 0x004266B5` reads the active signed
`keyboardSelection` from its live Title object and looks up one byte in
the independently identified `g_TitleResultCharacterOrder` table at
`0x004A1DAC`. Original Win32 i386 computes the full DWORD indexed address
(`mov cl, [ecx + 0x4A1DAC]`) without an additional C++ array-bound check.
The maintained source now forms `uint32(&table[0]) + uint32(selection)`
before taking the same byte lvalue. It retains the original separate
pre-call `character` snapshot, second live dereference, cursor call and
post-call `nameBankIndex` store, without clamping or caching the table row.

Three signed-address source probes left the entire original-bound
**484/485-byte** result update owner at its existing **218 unlike overlap
bytes, one missing byte, 34 relocations, nine ordered calls and 40-block
direct graph**; this owner remains NONEXACT. The installed source also
cold-replayed both genuine TitleScreen compiler carriers: **41/41 exact
owners, 18,972 physical bytes and 953 relocations**, including the exact
1,939-byte DrawResult (a different function). Nine `OnUpdateStartMenu`
compiler-private `$L` labels were rebound only after verifying actual
same-section COFF definitions and all their unchanged original PE target
addresses. This is native lookup fidelity, not a newly exact result updater
or native product proof.

## 2026-11-11 Title OnUpdateResult native unlock-table row addresses

Inside the still-open `TitleScreenView::OnUpdateResult @ 0x004266B5`,
the original Japanese TH09 i386 code reads the two character-unlock
bytes through native 32-bit **row base + signed cursor word** addresses.
The maintained source formerly subscripts `characterUnlocked[1/2]`
with that signed index; the current expressions instead form the actual
byte address from `&g_PlayStatsRecord.characterUnlocked[1/2][0]` and the
unsigned 32-bit index, then read the same byte. No index clamping, loop
bound, unlock predicate, sound/input behavior, or different backing array
was introduced. This retains the original physical access semantics even
when an extreme selected DWORD would be outside one C++ array subobject.

Primary-row-only, secondary-row-only and combined pinned VC7.1 probes each
kept the complete **484-byte candidate against the 485-byte original**,
**34 reviewed fields, nine calls, identical direct graph, 218 unlike
overlapping bytes and one absent byte**. The installed combined source
cold-replayed exactly the two affected compiler carriers: **41 still-exact
functions, 18,972 complete physical bytes and 953 actual original-bound
relocations**. Nine compiler-private `$L` spellings in the exact
`OnUpdateStartMenu` were independently checked against real same-section
COFF definitions, target PE addresses/operands and all complete original
owners before manifest rebinding. `OnUpdateResult` remains **NONEXACT**;
no exact ledger entry was promoted and whole-game build remains open.

## 2026-11-11 DrawReplayMenu complete native list and frame-pointer indexing

`TitleScreenView::DrawReplayMenu @ 0x004234F6` is a still-nonexact,
complete **1,280-byte** renderer. The first list branch reads twelve
separately live fields from the true 50-row `TitleScreenView::replays`
allocation (0x1EC-byte records) and four name characters from the 0x200-byte
`replayPaths` rows. The selected-record branch reads the two native ECL
replay frame-pointer rows and stage positions from the original 0xC0-byte
replay header (`frameStart[3][10]`), including repeated tests and values.
The source now forms these nineteen dynamic addresses from their real
record/row bases and `uint32(index)*sizeof(element)` Win32 DWORD strides;
the third row remains untouched. There is no new bounds check, backing table,
field cache, call, or changed branch. The existing native selected-record,
selected-path and replay name-table views are reused rather than duplicated.

Five isolated and combined pinned VC7.1 owner trials kept the **complete
1,280-byte** physical candidate, **57 independently original-bound
relocations, 13 calls, 16 literal definitions / 30 literal references,
376 instructions, 72 original CFG blocks and precisely the same 29
unmatched ordinary bytes**. The installed combined form passed the focused
`--owner replaymenu --skip-exact-siblings` original-bound full-owner guard.
The separate lexical carrier has no canonical exact siblings; no whole-source
cold replay was run. The 29 EDX/ECX register/lifetime differences are still
open; this change improves native-source indexing semantics, not exactness.

## Title DrawReplayMenu native replay-name table slots

In the still-nonexact TitleScreenView::DrawReplayMenu owner at 0x004234F6,
the selected replay header and its stage frame headers supply the unsigned
difficulty/mode/character IDs used to fetch character and mode text from
global pointer tables. Retail reads the *real* four-byte pointer slot at
base + unsigned(header ID) * sizeof(pointer), with no range check. The C++
array indexing had an implicit in-bounds assumption; the source now uses a
single lexical NativeReplayTextName view for the two difficulty reads,
one mode read and two stage-character reads. The array layout and the
13 ordered native calls are unchanged, with no new guard or cache.

Individual and combined pinned VC7.1 /O1 /Ob1 probes preserved the
entire 1,280-byte candidate against the original SHA-pinned Japanese PE:
**57 reviewed relocation fields, 13 calls, 376 decoded instructions,
72 CFG blocks and the same 29 remaining register-choice bytes**. This is
native wire-address fidelity, not byte-exact status or a new ledger entry.

## 2026-11-11 Title DrawReplayMenu selected record and path native addresses

The still-nonexact 1,280-byte TitleScreenView::DrawReplayMenu renders one
runtime-selected replay from the real TitleScreen allocation: 50 native
0x1EC-byte ReplayDataView records beginning at +0x68E4 and 50 native
0x200-byte replay-path rows beginning at +0x028C. The signed selectedReplay
word and its separately maintained selected display index are converted to
native 32-bit byte offsets for both the selected replay reference and the
three repeated type tests, plus the four character-name path bytes. Two
natural inline lvalue/pointer accessors preserve the distinct physical
record/path bases, original separate reads, text calls, SDK Float3 math,
selection states, and the bounds of the unrelated 50-row display loop.
No selected-index validation, cached replay reference or second allocation
was introduced.

Eight source-cold indexed physical-address variants and three inline-helper
forms were compiled with the pinned VC7.1 Title profile. The selected-only
and inline forms retained **all 1,280 candidate bytes, 57 original-bound
fields, 13 ordered calls, 72 original CFG blocks and precisely the
existing 29 ordinary register-byte differences**. A different cached
selected-record reference failed the complete physical-extent audit and
was rejected. The installed two-inline-helper source passed both the
full-original-bound standalone owner inspector and the official
result-specific replay-menu boss guard. This standalone nonexact source
carrier has no canonical exact siblings; no unrelated cold replay was run.
The selected record owner remains NONEXACT and the Windows i386 product
is still unclosed.

## Title replay-menu and result update

DrawReplayMenu remains 1,280 bytes/57 fields/13 calls/29 differences. Its native
text API takes `Float3*`, but SDK `D3DXVECTOR3` math preserves the required x87
schedule. Current source borrows one checked Float3 view of that same 12-byte
position. Pure Float3 expressions introduced unsupported relocation identities;
early ReplayData/mode-index snapshots changed hundreds of bytes. The selected
replay record's lifetime remains the useful question.

OnUpdateResult is a different owner from exact DrawResult. Update remains
484/485 bytes, 34 fields/nine calls, 218 differing overlaps and one missing
byte. At original +0xFD/+0xFF/+0x107, index reload, signed byte load and
sign-extension occur separately before the cursor call and result publication.
Simple snapshots/accessors combine the load and extension and did not recover
that schedule. DrawResult is already fully exact at 1,939 bytes/82 fields;
its former barrier and three-byte allowance must not return.

## 2026-11-10 ScreenEffect envelope native 32-bit phase endpoint

`ScreenEffect::CalcShakeEnvelope @ 0x004227A0` accumulates its fade-in,
hold and fade-out frame counts into the 32-bit envelope endpoint. The original
Win32 `add` wraps modulo 2^32; the previous signed `int update = hold + fade;
endFrame += update` depended on C++ signed-overflow behavior. The maintained
source now forms `update` and the endpoint using native unsigned-word
arithmetic, then keeps the same signed `ZunTimer` comparison and native float
conversion for the falling edge. It does not add range checks, new branches
or change the unusual original fade-phase behavior.

The full pinned-target 336-byte/20-field/8-call shake owner retains precisely
its previous **26 nonmatching ordinary bytes**, confined to receiver/timer
and flag transport, and the complete original branch graph. Twenty-five
isolated natural forms of the fade/hold computation were actually compiled:
nonvolatile direct forms produced a shorter 334-byte owner with displaced
relocations and ~249 overlapping differences; the selected unsigned-word
form retained the exact former instruction layout. All **three** affected
ScreenEffect compilation profiles were rebuilt, with full canonical cold
replay of **18 existing exact functions, 2,562 physical bytes and 164
relocations**. VC7.1 renumbered nine `RegisterChain` private `$L` symbols;
each current COFF same-section definition and original PE operand target was
verified before changing its manifest name. The source remains nonexact in
the complete shake owner, and there is no new exact credit.

## 2026-11-11 Replay focus side-state native address and exact carriers

The exact `ReplayManagerView::PlaybackInputAndFps` (299 bytes) and
`ReplayManagerView::CaptureFrameSyncState` (257 bytes) both update per-side
focus history at the live `g_ReplayInputStates` base. They now form each
`ReplayInputState` receiver with native i386 DWORD arithmetic:
`uint32(side) * sizeof(ReplayInputState)` (actual size 0x8E), rather than
`g_ReplayInputStates[side]` C++ indexing. The playback path retains its
independent `sideFocusMode` cursor and the capture path retains the nullable
configuration gate. Each continues to call `IsHeld(1)` and to read/update the
original `auxiliary2A` word at +0x2A; no additional boundary or fallback
behavior is added.

Separate and combined VC7.1 source-cold tests preserve the **complete 299-
and 257-byte owners and their 22+28 relocations**. The combined installed
source passes the full **12 exact ReplayInput.cpp siblings / 4,093 physical
bytes / 232 original-bound fields** on the two affected compiler profiles.
The nonexact SaveReplay and BeginPlaybackStage retain their preexisting
1,238/1,238-byte (two differences) and 538/540-byte (39 overlapping
ordinary differences) frontiers. This is native source fidelity, not a newly
exact function or product/runtime verification.

## Network queue and Replay input native frame addresses

The real Supervisor frame queues occupy twenty 0x0C-byte entries
starting at +0x47C, with two 0x0C-byte last-received entries at
+0x56C. The exact InsertPredictedFrame and PopFrame methods now
address these actual word records by native uint32(side)*10
and uint32(flatIndex) strides instead of assuming a C++ array
bounds precondition. Their 0x78-byte per-side scan cursor and
flatIndex = side*10+index now perform the retail i386 modulo-32
arithmetic before computing the real queue addresses. Every
original PopFrame load and the shift loop remains distinct.

The exact AreFrameQueuesSynchronized uses the same true queue base
for its signed side-selected head; its fixed heads remain untouched.
The exact ApplyNetworkInput callback now addresses the real
0x8E-byte ReplayInputState global array by signed side index as
an unsigned native DWORD byte offset. PopFrame's live fallback
ReplayInput lookup and its last-received side record are similarly
recovered, without new caching, range guards or schedule changes.
The SupervisorServiceUpdate boss is separately nonexact and is
not promoted by these changes.

All three source families were compiled with pinned VC7.1 /O2 /Ob1:
isolated native pointer variants and the combined queue helpers
preserved the original four complete bodies -- ApplyNetworkInput
131 bytes, AreFrameQueuesSynchronized 49 bytes,
InsertPredictedFrame 191 bytes, PopFrame 119 bytes -- and their
two original relocation fields. The installed maintained sources
passed scoped cold replay of **four exact functions, 490 complete
physical bytes and both original relocation fields** across the four
actually affected compiler carriers. No new exact function was claimed.

## 2026-11-10 Supervisor network native frame-word publication

`SupervisorServiceUpdate @ 0x00430AA0` has two integer `frameCounter`
advances (predicted-frame and packet-ready paths) and publishes the network
frame number as `60 / syncRate + frameCounter - 1`. Their original i386
instructions operate on **native 32-bit words**, wrapping even if the signed
counter reaches its limit. The C++ source now performs both increments and
the packet-frame sum with unsigned 32-bit operations before interpreting the
saved fields as signed, avoiding signed-overflow undefined behavior. The
DirectPlay session, queue methods, sender, packet checksums, scheduling,
error-handling and original arithmetic operands are otherwise unchanged.

Ten natural counter/packet source-cold probes and seven natural wait-block
probes were compiled with the pinned VC7.1 service profile. All selected
native-word variants emitted the **identical 1,627-byte raw service owner**,
including its 106 fields and 41 direct calls; original PE still owns 1,633
bytes with 1,381 unlike overlapping positions, six missing bytes and six
call-order positions caused by the still-unresolved early-wait block layout.
Relocating C++ wait labels alone did not repair that physical scheduling.
This remains a nonexact boss, not exactness credit.

The live source was separately recompiled and the full raw owner compared to
the earlier source-cold object byte-for-byte. Cold rebuild of exact caller
`Supervisor::OnUpdate` surfaced **40** changed private `$L` / `$title$`
label names in its own compiler carrier; these were re-bound in
`match-units.toml` only after an independent original PE / actual same-section
COFF proof found all **996 physical bytes and 92 relocation fields exact**.
Five related exact queue/packet leaves also cold passed (191 + 213 + 119 +
49 + 95 bytes). Total tested exact subset: **six functions, 1,663 full
physical bytes and 93 relocation fields**. Both native network source-owner
and queue-call contract tests passed. No whole-product cold replay or native
build/semantics closure is claimed.

## 2026-11-11 ScreenEffect CalcShake native side and camera words

The exact `ScreenEffect::CalcShake @ 0x00422660` reads its live signed
`contextIndex` against two different original storage owners:
`g_GameManager.sides[0]` (a 0x38-byte side record) for the low flag and
`g_Supervisor.ctorArray078[0]` (a 0xF0-byte camera/shake record) for all
six X/Y writes across the random switch arms. The maintained source now
uses two inline typed lvalue accessors on those real subobjects, scaling
the signed index as an **unsigned 32-bit byte-address word**. This removes
the ordinary C++ array-subscript bounds precondition without clamping a
side, introducing parallel arrays, caching either value or changing the
original RNG order.

Independent side-only, viewport-only, combined lexical, and combined inline
VC7.1 probes all reproduced the **complete 316/316-byte** original
`CalcShake` owner with its **16 exact relocation fields**. The installed
inline version passed the official source-scoped cold replay of all **18
exact ScreenEffect owners / 2,562 physical bytes / 164 fields** across
three genuine compiler profiles. Nine compiler-private `$L` labels in the
unchanged 480-byte `RegisterChain` owner were renamed only after checking
each actual same-section COFF definition, local offset, zero addend and
unchanged original PE destination, including the owned switch-table tail.
The separate nonexact 336-byte `CalcShakeEnvelope` still has its previous
26 unlike bytes and 20 bound fields. This is native-source fidelity, not a
new exact function or a completed game/runtime claim.

## 2026-11-11 ScreenEffect CalcShake native signed amplitude delta

The exact `ScreenEffect::CalcShake @ 0x00422660` computes a floating-point
shake envelope from two signed effect parameters. Retail VC7.1 first
subtracts the original `rawParameter0` from `rawParameter1` using native
i386 32-bit integer arithmetic and then converts the **signed result**
for the existing floating-point calculation. The maintained C++ formerly
used a potentially overflowing signed subtraction. Its source now
subtracts the two unsigned DWORDs, restoring the 32-bit signed result
before multiplication, while keeping the original timer, RNG branches,
float order, side writes and removal conditions unchanged.

Two independent natural-word source variants compiled to the complete
**316-byte exact owner with 16 original-bound relocation fields**.
A variant introducing a separate new signed local changed the owner and
was rejected. The installed source cold-replayed the **three genuinely
affected ScreenEffect compiler carriers**: **18 exact functions / 2,562
physical bytes / 164 relocation fields**. Nine renumbered VC7.1 private
`$L` labels in the exact `RegisterChain` owner were accepted only after
independent original PE operands and same-section actual COFF definitions
proved the same addresses and all eighteen complete physical owners were
byte-exact. The separate nonexact **336-byte CalcShakeEnvelope** retained
20 fields, eight calls, its complete control graph and exactly the prior
26 ordinary differences. This is source-level native fidelity, not new
exact-function credit or a complete-product claim.

## 2026-11-10 GameplaySetupThread complete 1,689-byte byte-exact closure

**Exact function promoted:** `GameplaySetupThread @ 0x0041AF2D`, target
`th09.exe` original Japanese v1.50a, 1,689/1,689 complete physical bytes,
SHA-256 of original `8dc5dd11495f34c6b1998653e0094195d6d891fff374d403750141e9936f0d6a`.
The true pinned `/O2 /Os /Ob1` VC7.1 compiler, unchanged ABI/toolchain and
actual `src/GameManagerSetup.cpp` produce exactly **413/413 original x86
instructions, 172/172 independently bound COFF fields** (139 DIR32,
33 REL32), **35 original-ordered calls, 78/78 identical control-flow blocks,
original control-flow edges and one verified original static literal**.
All 1,689 bytes compare equal *after* canonical original PE field relocation;
no executable byte is patched or padded. The `game-manager-gameplay-setup-thread`
exact manifest records every offset/type/actual symbol and independently
reviewed target address. `functions.csv` and `matches.csv` now credit the
function `matching / 100.00`, not just a similar partial body.

The source recovery came from two *semantically equivalent* VC7.1 switch
structure changes: present the mode cases in C++ lexical order `4,1,3,2,0`,
and test difficulty one's `gameMode == 2` fixed-rate branch first rather
than the round-dependent arm first. `case 1` still returns neither early nor
fallthrough; all original values, original signed/unsigned arithmetic,
case/side boundaries, exact native creator calls and empty default behavior
are unchanged. The compiler now schedules difficulty 2/3 fixed-rate
`valueC8=22` at exact original owner field offset `+0x433`, and restores
mode zero's original *two-byte short backward jump* (`EB 95` at `+0x062B`)
instead of the former five-byte near jump. That resolves the last three
bytes and the entire earlier 170-byte residual naturally. No assembly,
`volatile` lifetime hack, dummy branch, forced jump encoding or compiler
flag change was introduced.

The **official `--changed-file src/GameManagerSetup.cpp` scoped cold replay**
passes **12/12 canonical exact units**, **4,680 complete physical bytes and
415 relocation fields** in **two actual translation-unit carrier profiles**.
The new 1,689-byte function coexists with the 11 previously exact GameManager
siblings, all independently verified. This is one newly exact source-present
function, not evidence for the still-open whole Windows i386 native product,
linker closure or runtime semantics. The immediately following section
records the now-superseded 1,682/1,692-byte diagnostic experiments.

## Remaining ScreenEffect, network and Effect callback frontiers

GameplaySetupThread is exact and is no longer part of this unfinished-owner
group. The comparisons below describe still-nonexact functions, not the
separate original-bound GameManager worker already closed above.

ScreenEffect shake remains 336 bytes/20 fields/26 differences. The final phase
extends endFrame by rawParameter1 + rawParameter2, and both the final guard and
ramp-down numerator use rawParameter0 + rawParameter1 + rawParameter2. An older
candidate incorrectly kept the ramp-up endpoint. The remaining question is
receiver/fade lifetime, with the current full-phase behavior preserved.

Supervisor network service remains 1,627/1,633 bytes and 106 fields. The
original result home/frame size and successful-prediction/synchronized-wait
block ordering differ. Swapping source arms or spelling early returns did
not control emitted block order. The network-message handler is separately
exact: its input is unsigned 16-bit, while seed is signed short. Its ret16
contract and four physical argument slots stay fixed.

Effect AddedCallback remains 51 bytes/four fields/eight differences. ESI holds
the owner and EDI captures the ANM manager, not the returned resource; EAX holds
GetAnm/PreloadAnm's resource. Bytes +0x0E..+0x32 already match; residuals are
+0x03..+0x09 and +0x0D. Borrowed owner/captured receiver/interface variants
left eight or eleven differences. RET at +0x32 ends the owner; the following
thirteen CC bytes are unowned. Keep the independently exact Create caller.

Eighteen additional current-source VC7.1 trials varied the order of owner and
ANM manager initialization, cached versus direct GetAnm receiver, reference
versus pointer owner, conditional assignment and result destination storage.
All complete 51-byte/4-field comparisons either retained the original eight
register-scheduling differences or regressed to eleven; a pointer-reference
trial changed the actual relocation count and was rejected. None was adopted.

## Reference integration and phase limits

Useful source was adapted from Cirno-Labs/th09 at immutable `d7653152`.
Attribution, revision and GPL-3.0-only scope remain in
[THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md) and source headers. The
local `_reference/` checkout is a hypothesis source; original TH09 bytes and
complete pinned-compiler comparisons decide acceptance.

Natural SDK/local-vector lifetime and genuine helper visibility closed several
formerly difficult owners, including result drawing, Etama draw, biased ECL
movement and Type29 fans. These successes do not justify copying upstream
layout assumptions or replacing the remaining large bosses wholesale.
Whole-product linkage, resources/data owners, runtime behavior, semantic
reconstruction and portability remain separate unfinished work.

## 2026-11-13 target-attested re-audit: Enemy boss and small leaves

A fresh TH09 1.50a target verification, tracking validation and status report
were run before this session's target-dependent work. The focused owner
inspectors below are diagnostic-only; none of their results is promoted to
canonical exactness by this note.

### EnemyManagerView::OnUpdate and its collision leaf

The complete original `EnemyManagerView::OnUpdate @ 0x00410730` remains
3,900 physical bytes. The current candidate is also 3,900 bytes, all 97
original fields bind at their original offsets, all 69 ordered calls agree,
and the complete direct branch graph and switch destinations agree. The
remaining frontier is 13 ordinary bytes, not a missing function tail or a
control-flow mismatch. A fresh target-bound Ghidra export and attested IDA
semantic read were used to re-audit the owner.

The two residual groups have distinct causes:

- The first two bytes are the draw-group selector: retail zero-extends the
  native byte into EAX and uses EAX as the scaled array index; the candidate
  uses ECX for the same zero-extended value. The native helper's DWORD address
  arithmetic is intentional. A byte-typed helper parameter and removal of
  the explicit unsigned cast did not alter the emitted instructions; direct
  four-element C++ subscripting would introduce a bounds precondition absent
  from the target and was rejected.
- The remaining eleven bytes are in the special-attack descriptor setup.
  Retail loads the manager side-state through EAX, the threshold through EDX,
  and copies the three position components into the descriptor's stack
  storage in its own register/store schedule. The candidate uses EDX for the
  side-state pointer and EAX for the threshold, with a different register and
  store order for the position. A field-by-field x/y/z rewrite moved literal
  field positions and failed the owner inspector, so it was reverted. Earlier
  pointer/reference aliases and vector copy forms are already recorded above;
  do not repeat them as if they were new hypotheses.

The independently maintained 82-byte `EnemyView::AppendPlayerCollisionBox`
leaf still compares byte-for-byte, with its two original bound fields and
calls to `0x0040F5A0` and `0x0040F8E0`. The 151-byte attached-effect update
leaf also remains an exact same-carrier sibling. The focused
`--owner enemy --skip-exact-siblings` diagnostic passed with the 13/0 boss/leaf
frontier. A full `--owner enemy` exact-sibling pass was also attempted; its
canonical phase stopped inside `compare-coff-function.py` while enumerating
large COFF local-label symbol tables. No Enemy source change was retained
from that experiment; rerun the canonical cohort separately before relying on
that full-owner check.

### EffectManager::AddedCallback leaf

Target-attested IDA analysis of `EffectManager::AddedCallback @ 0x0040D1C0`
confirms a 51-byte function with four bound fields, two ordered calls and the
same three-block control-flow shape as the candidate. It requests ANM slot 8
through `GetAnm @ 0x00401660`, tests the returned resource's `+0x04` field,
falls back to `PreloadAnm @ 0x0043C7A0` with `etama.anm` when that field is
null, stores the resulting `AnmLoaded *` at owner offset `+0x2D4`, and returns
zero. `GetAnm` computes the slot entry using the native 288-byte stride and
36-byte entry offset. The existing source has the same resource and fallback
semantics; the eight residual bytes are confined to the prologue/receiver
register schedule (retail loads the manager into EDI before saving the owner
from ECX into ESI; the candidate saves the owner first and copies the global
manager load through ECX). A local receiver, delayed owner cast and reference
aliases did not improve the complete body and some variants regressed to
eleven differences. None was retained. Keep the exact `Create` caller
protected when this owner is edited.

### ReplayManagerView::SaveReplay and the replay leaf boundary

`ReplayManagerView::SaveReplay @ 0x00420C60` remains a 1,238-byte function
with 53/53 original fields, 23 ordered calls, 49 matching direct blocks and
two ordinary differences at `+0xB7` and `+0xBB`. Retail loads the temporary
buffer base directly into EDI and forms the destination with
`lea edi,[edi+ebx-0xC0]`; the candidate loads that same stack local into ECX
and forms `lea edi,[ecx+ebx-0xC0]`. The pointer-arithmetic and `void *`
experiments did not change those two instructions. The accepted source
cleanup is commit `fe6ee2c7` (`gpt-web: model replay scratch allocation as
void`): `Alloc`'s untyped allocation remains `void *` until the explicit byte
pointer boundary at `Lzss::Encode`. The focused `--owner replaysave` check
passed all twelve same-TU canonical exact siblings and retained exactly those
two differences. This is not an exact-function promotion.

The 15-byte `ReplayPayloadView` constructor leaf was also re-read in IDA: it
constructs the `GameConfiguration` member at `this + 0x1C` and returns the
owner. The maintained source expresses the same construction, but its
compiler-generated-versus-authored origin is still unresolved; byte
similarity alone does not settle that classification.

## 2026-11-13 Player CheckBulletCollision boss register-lifetime frontier

The current PlayerLifecycleView::CheckBulletCollision at 0x0041DFF0 source
checkpoint is 9656ccd0 (gpt-web: pin collision AABB scratch offsets), following
3e25037d (gpt-web: place collision cursor in scratch home). It remains a
NONEXACT boss. Focused pinned-VC7.1 /O2 /Ob1 /Oy- evidence:

- 862/862 physical bytes; 275 candidate / 277 target instructions;
- 34 candidate / 33 target direct CFG blocks;
- all 26 relocation fields are bound and all 12 ordered calls agree;
- zero missing and zero extra physical bytes, but 729 overlapping linked
  bytes differ and the direct control-flow graphs disagree;
- the independent CheckGrazeCollision leaf remains exact at 204/204 bytes.

The current entry matches the retail prologue through the aligned loop header:
the 0x34 stack frame, EBX Bullet argument, ESI active-region cursor,
[ebp-4] cursor home, short jump, seven-byte NOP, and first
mov ecx,[esi] / test ecx,ecx. The source models the reused geometry and reward
stack storage as CollisionScratch; its CursorHome overlay places the current
slot pointer after a 48-byte prefix. A volatile scratchHome view is used for
the initial and final slot-home accesses. Rotated geometry uses the direct
position parameter. AABB temporaries use the target stack homes [ebp-24h],
[ebp-34h] and [ebp-30h], and the x-maximum x87 add uses the target operand
order.

The remaining mismatch is not a missing tail: the candidate occupies the full
862-byte physical extent. It still has an extra six-byte lea ebx,[ebx] before
region-type dispatch, a bottom-of-loop next-slot null test instead of the
target's unconditional loop-back, x87 comparison/branch scheduling
differences, AABB comparison register/branch differences, reward-effect
payload register-order differences, and a different final hit-count exit
layout. The existing byte-emission alignment shim is a narrow retained source
workaround, not an exactness proof or a pattern to expand.

The focused checks in this checkpoint rebuilt only PlayerCollisionHelpers.cpp
and compared the exact graze sibling. No full cold replay was run. Do not
promote the boss based on equal extent, relocation inventory or call order;
require a complete accepted Oracle receipt. Historical collision trials above
this section remain failed experiments and should not be mistaken for the
current source or current comparison.

## 2026-11-13 GPT-web: TH09 boss reconstruction session

### `PlayerLifecycleView::CheckBulletCollision`

A fresh target-bound review confirmed the full 862-byte function extent, 277 target instructions, 33 target basic blocks, 12 external calls, and 26 relocation fields. The baseline candidate remains 862 bytes but has 275 instructions, 34 blocks, and 729 differing bytes. The three shape paths (circle, rotated rectangle, AABB) and the cancel/reward path were all traced against the original PE and IDA/Ghidra views.

Experiments tested volatile stack-home writes, plain versus volatile hit-count locals, direct versus indirect position reads, one-axis pointer changes, reward X/Y argument reads, and difficulty/reward-value load ordering. Volatile stack-home writes make the direct-hit epilogue retain the target's slot-home store, but the whole-function mismatch count rises; direct position-pointer variants also worsen the overall match. Difficulty snapshots preserve the baseline byte count without reducing the residual. Do not repeat these exact variants as if they were new progress. The next useful line of inquiry is the true lifetime/alias relationship of the slot-home local across the direct-hit and cancel/reward exits, rather than adding more volatile aliases blindly.

### `EnemyManagerView::OnUpdate`

The focused owner check measured a 3,900-byte physical extent, 97 relocation fields, 69 calls, 170 matching blocks, and 13 residual bytes grouped into draw-group register selection, side-state/threshold register selection, and the three-float descriptor copy. The `NativeEnemyDrawHead` helper now accepts the native byte-sized draw-group selector (`unsigned char`); the full enemy owner check retained all 77 exact sibling proofs. Direct macro expansion, a local integer group, side-state pointer/reference aliases, and descriptor pointer/reference aliases did not by themselves remove the remaining register windows. Continue from the live disassembly rather than repeating these aliases.

### Other frontiers reviewed

- `EffectManager::AddedCallback`: routing `GetAnm` through the saved manager pointer changes ESI/EDI allocation and moves the residual outside the accepted frontier; that probe was reverted.
- `ReplayManagerView::SaveReplay`: the two-byte EDI/ECX header-copy residual is already covered by prior pointer, cast, constness, register, size-type, and loop-order experiments. Do not repeat them without a new lifetime hypothesis.
- `RunEcl`: opcode 4 timer/advance semantics and opcode 155 timeout-bit transitions were rechecked. Existing semantic probes cover 90 timer cases and 3,072 timeout-bit states; prior wire-snapshot and packed-struct variants are already documented and should not be repeated.

No full cold replay was run in this session; work stayed on the selected owners and their focused build/inspection paths.

## 2026-11-13 GPT-web: continued target-bound frontier experiments

### EnemyManager::OnUpdate descriptor and leaf boundary

A fresh IDA decompilation of `0x00410730` confirms the special-attack path constructs the 0x214-byte Enemy bullet descriptor, copies the Enemy's three position floats, computes speed1 from `GameManager+0xB4`, then publishes the descriptor through the side Etama controller. The separate `EnemyBulletDescriptorView` constructor is independently byte-identical to the original 28-byte constructor at `0x0040D500`; substituting that type for the stack descriptor did not change the 13-byte OnUpdate frontier. Directly assigning `EnemyFloat3` to `Float3` is ill-formed (VC7.1 C2679). Explicit scalar position writes changed the full owner to 469 differences; moving speed1 ahead of the other descriptor fields changed it to 44. Direct macro/native-address draw-head forms and side-state pointer/reference locals retained the original 13 differences. Do not promote any of these variants.

### CheckBulletCollision loop control-flow frontier

Fresh linked disassembly isolates the CFG discrepancy. Retail checks `*slot` at the loop header (`mov ecx,[esi]; test ecx,ecx`) and each region advance is an unconditional `add esi,4; jmp 0x41E010`. The current candidate transforms the continuation into a bottom-of-loop next-slot load/test/conditional branch, yielding 34 rather than 33 blocks; it also retains a six-byte `lea ebx,[ebx]` at `+0x2A` that retail does not have. Removing the three `register` hints did not change the 862-byte/275-instruction/34-block/729-difference baseline. A volatile slot-element read worsened it to 864 bytes/276 instructions/741 differences; a volatile pointer-element view worsened it to 899 bytes/289 instructions/800 differences. Replacing the region-type guard with `switch` restored 33 blocks but produced only 856 bytes, six missing bytes and 760 differences; an integer range guard produced 857 bytes, five missing and 767 differences. These are rejected probes, not progress receipts. The useful next question remains how the original loop keeps its top-of-loop read and unconditional continuation without synthetic volatility.

### Replay and small-leaf follow-up

- `BeginPlaybackStage` target tail is `xor edx,edx; mov dx,[eax+4]; mov ecx,0x4ACE0C; push edx; call SetSeed`, followed by restoring EDI/ESI before clearing inputDelay. The current candidate instead uses `movzx eax,[eax+4]`, pushes EAX before loading ECX, and stores inputDelay before the pops. Zero-plus-memcpy, low-word pointer, union, u32-local, and RNG pointer/reference receiver variants all retained the 538/540-byte, 39-difference frontier. The two missing bytes are the seed-load schedule; do not repeat these variants without a new register-lifetime hypothesis.
- `SaveReplay` u8/char buffer types and a separate byte-pointer destination local retained the existing +0xB7/+0xBB two-byte residual.
- `EffectManager::AddedCallback` local-pointer, declaration-order, reference, const and aggregate variants did not improve the eight-byte baseline; several changed it to eleven. Keep the current source.
- At this checkpoint the 33-byte `0x00412780` `fsincos` leaf was only an external `__fastcall` declaration. Its later maintained x87 source recovery and 33/33 diagnostic replay are recorded below; original authored-vs-library ownership remains unresolved. Do not replace its single `FSINCOS` operation with separate `sinf`/`cosf` calls.
- The small Enemy constructor wrappers at `0x00412090`..`0x00412100` remain ambiguous between explicit and compiler-generated construction; byte similarity does not settle origin. The three-byte `0x004343D0` conversion remains a shared/folded identity, not a new uniquely owned leaf.

No project-wide cold replay was run. All measurements above are focused, diagnostic-only probes and grant no partial or exact owner credit.


## 2026-11-13 GPT-web: Enemy constructor chain recovered

Six previously unclassified Enemy/ECL constructor leaves now have dedicated focused comparison units in `src/EnemyManagerLifecycle.cpp` under the unchanged `/O2 /Ob0` carrier. Local target-bound diagnostic replay is exact for every byte and every relocation: `EnemyEclTimelineStateView` (28 bytes, 2 fields), `EnemyEclManagerView` (15, 1), `EnemyScheduleRuntimeView` (20, 2), `Th09EclInterpolationSlotView` (15, 1), `Th09EclContextView` (47, 4), and `EnemyView` (207, 18).

The recovered object graph is concrete: the manager owns two 0x188 ECL managers; each ECL manager constructs its 0x180 timeline state; the timeline state constructs eight Float3 positions; an ECL context constructs two ZunTimers and eight 0x30 interpolation slots; an Enemy constructs its ANM VMs, 16 ECL contexts, two 0x214 bullet descriptors, 96 trail samples, 194 trail vertices and timer members. The manager constructor's existing 101-byte exact replay independently anchors the Enemy spawn template and 129-element pool construction.

All six functions were rebuilt with `scripts/build-match-unit.py` and passed `scripts/compare-coff-function.py --unit` with zero differing bytes and fully reviewed relocation manifests. The `0x004343D0` empty-constructor callback remains a folded/shared physical body with unresolved unique source ownership. More importantly, each constructor is observationally compatible with both explicit-empty and compiler-generated special-member source; all six retain `owner=unknown`, `origin=unknown`, and `disposition=review`. They are not entered in `matches.csv` or `reccmp-functions.csv`, and receive no authored exact credit. These are local diagnostic codegen results, not a factory-accepted receipt or whole-image claim.

## 2026-11-13 GPT-web: collision loop CFG continuation probes

The 862-byte `PlayerLifecycleView::CheckBulletCollision` boss remains nonexact and the live source was not changed. A new focused set of VC7.1 `/O2 /Ob1 /Oy-` probes tested whether the retail top-tested region loop can be recovered with a native `for` increment expression rather than the existing shared `goto` labels.

- A `for (;; slot = next(slot))` body with one shared `next_region` label, and the corresponding `for (; *slot != NULL; slot = next(slot))` form, both reach 33 candidate CFG blocks (the target has 33). The former is 879 bytes / 277 instructions / 785 overlapping differences / 17 extra bytes; the latter is 876 bytes / 276 instructions / 797 differences / 14 extra bytes. Neither direct CFG matches the target, and both still perform a bottom-of-loop next-slot load/test before branching back to the header.
- Moving the `*slot` read to the loop condition and assigning `region` in the body removes one candidate instruction but does not remove the bottom null test. Adding a volatile element read retains the 879-byte/33-block shape and changes register allocation, not the loop backedge. Making both the slot pointer and element volatile expands the candidate to 992 bytes and is rejected by the original-bound extent/relocation guard.
- Removing the existing short-jump/seven-byte alignment shim from either `for` form reduces the candidate to 30 blocks (865/273 or 870/274 bytes); the shim therefore remains necessary for the current entry shape. The target loop still differs at its backedge: retail performs `add esi,4; jmp header`, while these candidates read and test the next slot at the bottom.

The useful remaining hypothesis is not merely `for` versus `while`: VC7.1 is rotating the null test to the bottom even when the source loop is infinite with a top exit. A future attempt must preserve the single top read and unconditional backedge without introducing a second speculative next-slot read. Do not promote any of these diagnostic variants.

The EnemyManager 13-byte frontier was also rechecked with direct draw-head subscripting/address arithmetic, function-scope side/effect pointer aliases, pointer/reference/field-wise descriptor copies, and a temporary `Float3` view for `EnemyView::position2D74`. All accepted original-bound probes retained the same 13 offsets (`+0x19C,+0x1A3,+0x252,+0x263,+0x26D,+0x272,+0x294,+0x29A,+0x29C,+0x2A5,+0x2A9,+0x2B7,+0x2BD`); the `Float3` field experiment also retained all 13. The temporary type experiment restored every source file after compilation. No source change or exactness credit resulted.

Only focused object builds and inspectors were run; no full cold replay was run.

## 2026-11-13 GPT-web: EffectManager AddedCallback receiver/register follow-up

`EffectManager::AddedCallback @ 0x0040D1C0` remains a 51-byte diagnostic-only leaf with 19 instructions, four bound relocation fields, three matching CFG blocks and eight differing bytes at `+0x03..+0x09` and `+0x0D`. Fresh target/candidate disassembly narrows the issue precisely:

- Retail saves the global ANM manager directly into EDI (`mov edi,[g_AnmManager]`), then saves the incoming context from ECX into ESI (`mov esi,ecx`), pushes slot 8, copies EDI to ECX and calls `GetAnm`.
- The maintained candidate saves the context into ESI first, loads the global manager into ECX, then copies ECX into EDI before the same call. The fallback, owner store, ordered calls and CFG already agree.

New source probes tried explicit `register` on each pointer/both, pointer and pointee `volatile`, const pointers, references, aggregate locals with both field/assignment orders, comma sequencing, and local/global direct `GetAnm` variants. They either retained eight differences or regressed to eleven; none was retained. The `volatile EffectManager *` plus direct-global `GetAnm` variant retained the eight-byte baseline. The unresolved compiler fact is why an ordinary local manager snapshot is first materialized through ECX rather than directly into the callee-preserved EDI while the context remains in ESI. Do not repeat these declaration-order or alias variants without a new lifetime/visibility hypothesis.

No source was changed. Only `EffectManager.cpp`'s focused object and the 51-byte callback inspector were run; no full cold replay was run.

## 2026-11-13 GPT-web: Bullet/Laser constructor `/Ob0` closure

The three native constructors in `src/BulletNativeConstructors.cpp` now have dedicated canonical match units using pinned VC7.1 `/O2 /Ob0` rather than the former `/Ob1` profile. This is a compiler-visibility distinction, not register forcing or manual CRT calls: `/Ob1` expanded `Bullet::exStates[7]` into a per-element loop and emitted a 67-byte Bullet constructor; `/Ob0` emits the original EH vector-constructor call and the original 58-byte body.

Focused `build-match-unit.py` + `compare-coff-function.py` results:

- `bullet-type-sprites-constructor`: 56/56 bytes, five original AnmVm constructor relocations
- `bullet-constructor`: 58/58 bytes, five original relocations, including `??0BulletExState@@QAE@XZ` at `0x0041EE20` and `??_H@YGXPAXIHP6EPAX0@Z@Z` at `0x00401470`
- `laser-constructor`: 34/34 bytes, three original AnmVm/ZunTimer relocations

The complete-owner `inspect-bullet-native-constructors.py` additionally checks all four bodies: `BulletTypeSprites` 56/56, implicit `BulletExState` 12/12, `Bullet` 58/58 and `Laser` 34/34, with every original relocation field and no byte differences. The `BulletExState` constructor has no independent `functions.csv` row because its 12-byte timer-prefix body is physically shared with the separately reviewed `PlayerState3031CView` constructor at `0x0041EE20`; this does not prove a unique original class/source owner. The three visible constructor rows therefore remain `unclassified` with origin `unknown`: no codegen exactness or authored-origin credit is claimed despite the complete local byte/relocation replay. No runtime or full cold replay was run.

## 2026-11-13 GPT-web: EnemyManager::OnUpdate register-allocation probes

The 3,900-byte `EnemyManagerView::OnUpdate @ 0x00410730` remains at the same 13 differing instruction bytes after a new round of source-level register/lifetime probes. The unchanged offsets are `+0x19C,+0x1A3,+0x252,+0x263,+0x26D,+0x272,+0x294,+0x29A,+0x29C,+0x2A5,+0x2A9,+0x2B7,+0x2BD`.

Newly tested, without retaining any source changes:

- Reordered the five function locals (`bombHit`, `damageOccurred`, `trailIndex`, `hitKind`, `enemyIndex`) in four ways; all four remain at the same 13 offsets.
- Moved selected locals into the enemy-update scope and tried function-scope pointer/value locals for descriptor position, special-attack threshold and side state. Individual variants remain at 13 differences; combined local lifetimes increase the mismatch substantially.
- Removed the `spawnSide` reference, moved it before the descriptor, used a function-scope side pointer, and used a separate 0x38-byte side-state view type. The accepted minimal variants all retain the same 13 offsets; broad replacement of the side type changes the complete owner extent/relocation inventory and is rejected.
- Tried `const`, `register`, function-scope and direct-field variants for the primary flags and draw-group index, a group-index return helper, direct array indexing and pointer arithmetic. All accepted variants retain the same 13 offsets.
- A temporary inline-assembly attempt to force the target's EAX draw-head sequence caused the complete owner to expand to 5,832 bytes/111 relocation fields; it was discarded. The source remains unchanged.

The current target/candidate register rotation is still localized: target uses EAX for the draw-group byte and side pointer, EDX for the threshold and position Y word, and EAX again for the descriptor position base; candidate chooses ECX/EDX/EAX differently at those sites. No candidate is promoted. Continue with a new liveness/translation-unit hypothesis rather than repeating local declaration-order, simple alias, or array-index variants.

A focused `/Ob0` compile of the complete EnemyManager carrier does not match the `/Ob1` boss profile. The target's 13-byte frontier remains tied to the `/O2 /Ob1 /Oy-` profile. Only focused object compiles/inspectors were run; no full cold replay was run.

## 2026-11-13 GPT-web: PlayerMovement boss reduced to an eight-byte frontier

`PlayerLifecycleView::UpdateMovementAndOptions` at `0x0041C170` is an 1835-byte logical function inside a 1900-byte original physical owner (one post-RET alignment byte plus two 32-byte switch tables). A source-cold pinned VC7.1 `/O2 /Ob1` diagnostic preserves all 66 original relocation fields, 23 ordered direct calls, and 489 instructions.

The key code-generation discovery was the lifetime/scope of `PlayerMovementFields &runtime`. Using `player->movementFields1CCC.<member>` directly for movement-angle, speed-multiplier and velocity accesses recovers 114 of the previous 122 residual positions. The vertical velocity addition is the exception: keep a function-scope `PlayerMovementFields &runtime` and use `runtime.velocity1CCC.y` at that one site. Removing the alias there changes the X87 operand order for the y-position update and reintroduces eight bytes. With the alias retained only for that access, the fresh full-owner diagnostic reports **8 ordinary byte differences**, at `0x0127-0x012E`, and no new mismatches elsewhere (1827/1835 logical bytes; 1892/1900 physical bytes).

The remaining eight bytes are only the instruction order for loading `g_GameConfiguration` and `PlayerLifecycleView::sideIndex` before the focus-mode comparison. Exactness is not claimed; `functions.csv` remains `unclassified`, origin remains unresolved, and the frontier guard is diagnostic-only. The focused `check-boss-frontiers.py --owner movement --skip-exact-siblings` replay passed at 8/1900 residual bytes. No full cold replay was run.


## 2026-11-13 GPT-web: Player collision-query constructor leaf

Target-bound IDA inspection of `0x0041CEB0` identifies a 28-byte `PlayerCollisionQueryStateView` constructor. Its body pushes the element callback at `0x004343D0`, count `0x80`, stride `0x30`, and `this+4`, then calls the vector-constructor helper at `0x00401470`. The callback body is the shared three-byte `mov eax, ecx; ret` sequence (`8B C1 C3`). The Player constructor's existing relocation manifest independently binds the same `0x004343D0` physical body as `PlayerConstructedVector3View`'s constructor.

The maintained source now leaves `PlayerCollisionQueryStateView`'s default constructor implicit and defines the nontrivial `PlayerCollisionQueryRecordView` and `PlayerConstructedVector3View` empty constructors out-of-line in `VectorPointerViews.cpp`. This is a compiler-visibility requirement, not a manual helper call: an explicit state constructor with the element body visible in the same TU was optimized to a three-byte return; an explicit state constructor with an out-of-line element constructor under `/Ob1` emitted a 35-byte inline loop. The implicit state constructor emitted in `PlayerRuntime.cpp` with the original `/O2 /Ob0` constructor profile instead has the target's 28-byte helper-call shape. Its COFF relocation records are `DIR32 ??0PlayerCollisionQueryRecordView@@QAE@XZ` at `+0x02` and `DISP32 ??_H@YGXPAXIHP6EPAX0@Z@Z` at `+0x14`; binding to `0x004343D0` and `0x00401470` yields the target bytes.

Focused guards after the source change: `player-constructor` 243/243, `player-on-update` 522/522, `player-update-stage-c` 74/74, and `player-calc-item-box-collision` 121/121. The 3-byte element constructors both match target `8B C1 C3`. This is a source reconstruction/codegen result, not a Factory-accepted receipt. Retail's explicit-empty versus compiler-generated origin remains unknown, so the constructor stays out of authored exact ledgers. No full cold replay or ledger promotion was performed.

### PlayerMovement eight-byte load-order probes

`PlayerLifecycleView::UpdateMovementAndOptions @ 0x0041C170` remains at eight physical differences (`+0x127-0x12E`) after fresh VC7.1 builds. Target loads `g_GameConfiguration` before reloading `player->sideIndex`; the candidate emits those independent loads in the reverse order. Direct array indexing, explicit config/side locals in either declaration order, an inline `GameConfiguration` accessor, a materialized focus byte, `this->sideIndex`, reversed branches, raw config-byte-base indexing and reuse of the earlier input-side local did not improve the complete owner (the last variant regressed heavily). Keep the eight-byte frontier; do not introduce volatile/barrier/register hints.


## 2026-11-13 GPT-web: Etama fsincos leaf source recovery

The 33-byte `fsincos @ 0x00412780` helper is now represented by a maintained
source body in `src/EtamaControllerDraw.cpp`. Its observed ABI is
`__fastcall(float *sine, float *cosine, float angle)`: ECX is the sine
output, EDX is the cosine output, and the angle is the sole stack word. One
x87 `FSINCOS` leaves cosine above sine; the source stores cosine through EDX
first and sine through ECX second. The original function ends in `ret 4`.

The new `etama-fsincos` focused unit under the existing EtamaControllerDraw
`/O2 /Ob0` VC7.1 profile reproduces **33/33 bytes and zero relocations**.
Rebuilding the co-resident `etama-controller-on-draw` unit also preserves
**600/600 bytes**, including its call to the now source-present helper. This
is source and machine-code evidence only: the helper remains
`unclassified`, with original authored-vs-library/runtime origin unresolved;
no `matches.csv` or authored exactness credit was added. Do not replace the
single x87 operation with separate `sinf`/`cosf` calls.

## 2026-11-13 GPT-web: ChargeAttack and Enemy OnUpdate follow-up probes

`PlayerLifecycleView::UpdateChargeAttack` was rebuilt from maintained source
after focused local-width and pointer-lifetime experiments. Fresh target
decompilation identifies `v2 = *(DWORD *)(this+8)` and one `char v6` at
`[ebp-1]`; retail keeps `v2` in EDI while it forms the input pointer in EBX
through an EAX multiply. Widening the maintained signed-short `side` local
to `int` produced **1,209 bytes / 929 ordinary differences**, with the
candidate assigning the same logical values to EBX/EDI in the opposite
roles. Register hints, declaration order, a separate input-offset local and
char-pointer arithmetic did not change that 929-byte frontier. Keeping the
short local but computing the ReplayInputState slot from full `sideIndex`
changed it to **1,215 bytes / 1,167 differences**; removing the local and
using `sideIndex` directly changed it to **1,211 bytes / 1,144 differences**.
A typed `&g_ReplayInputStates[0] + side` expression is byte-neutral at the
existing 1,210-byte / 235-difference baseline. Collapsing the two maintained
mode locals also regressed the owner (1,196/1,140 and 1,212/1,136 variants).
The existing baseline remains **1,210 bytes / 235 differences**, 70 fields,
58 ordered calls and 53 blocks. No variant was retained. The unresolved
problem is the target local/register lifetime, not a missing function tail.

`EnemyManagerView::OnUpdate` was also rebuilt from maintained source after
changing `NativeEnemyDrawHead` to take `EnemyView *`, moving its `drawHead`
local to function scope, and trying explicit x/y/z position-member writes.
The helper receiver and function-scope local both retained the original 13
residual offsets. The explicit memberwise copy changed literal relocation positions and
expanded the resolved ordinary difference count to 421, so it was rejected.
A `memcpy` position copy and a branch-local `register EffectManager *` alias
both retained the original 13 offsets. The restored baseline remains
**3,900 bytes, 97 fields, 69 calls, 170 blocks and 13 differences**. No source
changes from these probes were retained.

Only the affected Etama, EclManager and PlayerChargeAttack compiler carriers
were built and inspected. No full cold replay was run.

## 2026-11-13 GPT-web: EnemyManager OnUpdate side-category branch convergence

The maintained `EnemyManagerView::OnUpdate` source now expresses the side-category exception as `!(sideCategory == 1 || sideCategory == 2)`. This is behavior-equivalent to the previous conjunction, but pinned VC7.1 `/O2 /Ob1` emits the retail branch sequence: two independent `cmp eax,1/2` plus `je` instructions before the draw-group append path. The prior source form selected a range-style `jbe` sequence; the current form now matches the original comparison/control-flow instructions at that branch.

The focused `enemy-attached-effect-update` build and `inspect-enemy-manager-update.py --explain --json` report the same 3,900-byte target/candidate extent, all 97 relocation fields agreeing, all 69 ordered calls agreeing, 170 direct CFG blocks on both sides, and reviewed control flow agreeing. Thirteen linked instruction bytes remain different at `+0x19C,+0x1A3,+0x252,+0x263,+0x26D,+0x272,+0x294,+0x29A,+0x29C,+0x2A5,+0x2A9,+0x2B7,+0x2BD`: two bytes for draw-group index register selection and eleven bytes for side-state/threshold and descriptor-position register allocation/store scheduling. The function remains NONEXACT; no owner ledger promotion or Oracle exactness claim is made.

This source change is checkpointed as `4860f289` (`gpt-web: match enemy update side-category branches`). Continue from the current 13-byte diagnostic, not the earlier range-comparison source. Recent scalar-position interleaving and side-state pointer/reference experiments were temporary and restored; they did not replace the committed source. Keep the `/O2 /Ob1 /Oy-` profile and run only this owner/focused sibling guards while working this frontier.


## 2026-11-13 GPT-web: Enemy OnUpdate attached-effect register breakthrough

The maintained `EnemyManagerView::OnUpdate @ 0x00410730` now uses a
`volatile EnemySideStateView` pointee for the special-attack attached-effect
read. This is not a behavior guard or a new allocation: it preserves the
original side-state field access and causes VC7.1 `/O2 /Ob1` to retain the
retail EAX/EDX register schedule across the effect call. The eleven-byte
special-attack side-state, threshold and three-float descriptor window is now
byte-identical.

The complete 3,900-byte owner remains the same extent with all 97 original
relocation fields, 69 ordered calls and 170 direct CFG blocks agreeing. Its
residual is now exactly **two bytes**, at `+0x19C` and `+0x1A3`: the target
loads the draw-group byte into EAX and uses EAX as the LEA index, while the
candidate currently loads it into ECX before the same LEA. The owner remains
NONEXACT; no partial or Oracle exactness credit is claimed.

The focused `check-boss-frontiers.py --owner enemy` run passed the 77
same-EclManager exact siblings plus eight external exact callees (Enemy draw,
bullet descriptor, Etama pattern, Sound, ANM and ASCII marker owners). The
ECL local switch-label manifest spellings and ANM ExecuteScript local-label
spellings were refreshed only after resolving their COFF section-relative
addresses to the same original PE destinations; their logical/physical extents
and all nonlocal relocation identities remain unchanged. Continue from the
two-byte draw-group register frontier, not the former thirteen-byte frontier.

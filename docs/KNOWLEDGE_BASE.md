# TH09 reconstruction knowledge — durable findings

This file keeps **verified original layouts, compiler/COFF lessons,
and the unsolved source questions worth pursuing** for Japanese TH09 v1.50a.
It is not a commit-by-commit journal or a live exactness ledger.
The authoritative [current handoff](RE_HANDOFF.md) lists every open owner,
its current comparison and the right inspector; [generated progress](PROGRESS.md)
is derived from the canonical ledgers. Older omitted experiments, source
spellings and receipts are recoverable through Git history.

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

- **Current exactness/frontiers and safe rebuild commands:**
  [RE_HANDOFF.md](RE_HANDOFF.md); generated figures in
  [PROGRESS.md](PROGRESS.md).
- **ECL/Enemy:** [RunEcl opcode 4/155](#runecl-opcode-4-and-opcode-155),
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

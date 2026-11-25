# TH09 reconstruction knowledge — curated findings (2026-11-19)

This file retains durable target-backed native-layout, source-ownership and
VC7.1/COFF findings. It is not a session diary or exactness ledger. Current
owners and blockers are in RE_HANDOFF.md; counts are generated in PROGRESS.md.
Revalidate every finding against current source and the canonical ledgers
before applying it. Dated compiler observations do not prove current-source
exactness.

## Where to look

- Current exactness, nonexact owners and next work: RE_HANDOFF.md.
- Generated ledger totals: PROGRESS.md and scripts/report-reconstruction-status.py.
- Full acceptance boundary: ORACLES.md.
- Bounded reconstruction loop: RE_WORKFLOW.md.
- Original target and shared runtime: TOOLS.md and SHARED_TOOLS.md.
- Canonical exact units: config/match-units.toml and config/matches.csv.
- Nonexact source owners: config/frontiers.toml and scripts/check-frontier.py.

## Practical interpretation

- Only complete target-bound VC7.1 comparison of the full physical owner,
  tails/tables and relocation offsets/types/targets/addends establishes exactness.
- A nonexact frontier OK means only that its recorded monotone baseline was
  not worsened. A REGRESSED candidate must not be rebaselined to accept it.
- C++ array spelling is valid only when the real object extent and lifetime
  support it. Do not invent a convenient array, bounds guard, ABI projection,
  union or source-level type from one matching instruction.
- A changed .inl/header can renumber private $L symbols in every consumer.
  Rebuild all selected consumers and verify same-section COFF definitions
  against original PE destinations before changing manifest spellings.
- Preserve the fixed compiler profile. Do not use assembly, NOP/padding,
  register/variable-order hints, dummy liveness, volatile coaxing or profile
  roulette to force code generation.
- Ghidra decompilation and adjacent-game source are semantic hypotheses.
  Neither replaces TH09-local target, ABI, compiler or runtime evidence.
- Source naturalization is useful work, but does not add canonical exact
  credit unless the full original comparison passes.

## Verified native-source findings (recorded 2026-11-11)

These earlier target-backed source findings are retained as a compact
subsystem reference. They describe the source/layout observations made at
that time; check current definitions and exact-unit consumers before reuse.

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
fields** of `InitializeEffect`. At this historical 2026-11-11 snapshot,
the separately original-bound 51-byte `AddedCallback` had four bound fields,
both correct calls and eight nonexact register bytes. The current source
carrier was later improved in commit `2ebbf0cd`; it now has four residual
positions, as recorded in the current handoff. This source-semantic change
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

Compiler-wrapper provenance reconciliation (2026-11-16): The reviewed
`0x0043E170` body is a 33-byte i386 `__thiscall` scalar deleting destructor
(`ECX=this`, stack deletion flags, `RET 4`). The original calls the independent
`SoundManagerProcessView::~SoundManagerProcessView @ 0x0043F7A0`, conditionally
calls operator delete at `0x0047B249` when flag bit zero is set, and returns
the object pointer. Packet 116 pinned VC7.1 evidence independently produced
the same wrapper; a fresh selected-unit replay also verified the natural
`SoundPlayer::Release @ 0x0043EEB0` caller as **232/232 byte exact**. The
`functions.csv` row now records its compiler ABI, descriptive name and
**excluded** status, agreeing with the existing high-confidence
`compiler_generated/exclude` origin. It earns **no authored exact credit**;
the 23-byte DirectX SDK manager destructor remains separately third-party.
The original 33-byte body SHA-256 is
`fd55726971a8cccdd243225502ba1204e54b4b5a4797008a5f4c42770872aee7`;
the 15 following `CC` alignment bytes remain unowned. This status correction
does not change the authored denominator, 1,026 exact owners, or 35 genuinely
origin-unresolved candidates.

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
At this checkpoint, `TitleScreenView::OnUpdateResult` was also
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


## Current boss evidence (2026-11-19)

- EnemyManager::OnUpdate is a complete 3,900-byte physical owner with two
  differing immediate bytes. Retail uses EAX for the zero-extended byte
  draw-group index and the following LEA; current VC7.1 output uses ECX for
  the index. Array subscripting, pointer addition, pointer increment,
  byte-offset and index-local forms all retained the same two differences.
  Do not use volatile or register hints.
  Commit 7adfc808 aligns the C++ branch shape with the decompiler: test the
  side-state bit first, conditionally read the category byte, run normal
  enemy processing for bit-clear/category 1/category 2, and route the remaining
  early-draw case into the shared append. The draw-head local now spans the
  function, as does the decompiler v11 lifetime. The candidate remains at the
  same two register-selection differences. Its changed EclManager carrier
  passed scoped replay: 84 exact units, 45,420 physical bytes, 1,987
  relocation fields across two compiler profiles.
- SaveReplay is 1,238/1,238 bytes with two differences in the first native
  0x20-byte stage-header copy. Retail loads the destination base in EDI and
  forms EDI + stream offset - 0xC0; the candidate uses ECX for the base.
  Typed POD assignment is already retained.
- UpdateMovementAndOptions is 1,900/1,900 bytes with four residual bytes.
  The target loads the global GameConfiguration pointer into ECX before the
  Player side index into EAX; the maintained source reverses these loads.
  Pointer/reference/array-view source variants did not close the sequence.
- UpdateChargeAttack was 1,210/1,210 bytes with 183 differences. The target
  reads the 32-bit Player side index into EDI and derives the ReplayInputState
  pointer in EBX using the 0x8E stride. The short local put the
  sign-extended side in EBX and input pointer in EDI; simple int/pointer
  rewrites greatly regress the rest of the owner.
- CheckBulletCollision target facts: 0x34-byte local frame; EBX bullet, ESI
  active-region cursor, EDI Player owner. Region kinds 1/2/3 with delay <= 0
  are eligible. A nonzero radius selects circle geometry; zero radius and
  nonzero angle selects rotated rectangle; otherwise use AABB. Kind 1
  increments the region hit count and returns 2. Kinds 2/3 enter the cancel
  path only when a bullet exists and its word at +0x10C0 is zero. The target
  places the shared hit dispatcher at +0x8E before rectangle geometry at
  +0xBB. Current 849/862, 539-difference candidate is REGRESSED against the
  842-byte strict frontier baseline.
  Target CFG landmarks, relative to CheckBulletCollision at 0x0041DFF0: region
  filter +0x20; x87 radius compare +0x4B; circle calculation +0x5D; shared
  hit dispatcher +0x8E; zero-radius angle branch +0xBB; rotated rectangle
  +0xCD; AABB +0x14B; common cursor advance +0x1AE; cancellation reward
  +0x1B6; direct-hit count/return +0x336. Rectangle/AABB success returns to
  the +0x8E dispatcher. A nested-loop experiment still emitted the dispatcher
  at +0x17C (845/862 bytes, 581 differences) and was discarded; the maintained
  candidate remains 849/862 bytes, 539 differences, REGRESSED against the
  842-byte ratchet.
  Commit 9f223ed4 restores explicit unsigned DWORD wrap in reward-side
  arithmetic; the changed carrier scoped replay passed five exact units
  (1,220 physical bytes, 32 relocation fields). This is semantic fidelity,
  not exact-function credit for CheckBulletCollision.
- UpdateNetworkService target local frame is 0x14 bytes: final input pair at
  EBP-0x14, shared PopFrame output overlay at EBP-0x0C, return DWORD at EBP-4.
  The current source models these lifetimes, but its 1,645-byte candidate
  remains larger than the 1,633-byte target and is REGRESSED against the
  previous 1,629-byte candidate baseline (909 differing positions).
- BeginPlaybackStage has nine current differing positions, but is REGRESSED
  because their offsets moved relative to the 13-position frontier ratchet.
- Effect AddedCallback has four setup differences in global ANM-manager and
  callback-context register order. ScreenShake, Title replay drawing and
  result-update remain nonexact at the values in the current handoff.

## Further boss reconstruction probes (2026-11-19)

The following source experiments were run after the 7adfc808 EnemyManager CFG
checkpoint. All failed candidates were reverted; the strict frontier baselines
were not changed.

- CheckBulletCollision: a direct nested-loop reconstruction of the decompiler's
  scan/geometry/dispatch flow produced an 831/862-byte candidate with 660
  differing bytes. The maintained candidate remains 849/862 with 539 differing
  bytes against the 842-byte ratchet (REGRESSED). The nested candidate laid out
  rectangle geometry before the circle/dispatch path, unlike retail's circle
  at +0x4D, shared dispatcher at +0x7E, rotated rectangle at +0xBD and AABB at
  +0x13B. It was discarded. The next attempt must make the circle hit fall
  through into the dispatcher while rectangle/AABB successes branch back to
  that same dispatcher; do not merely move a common join label.
- UpdateChargeAttack: widening the side local from `short` to `int`, replacing
  the input reference with a pointer, and spelling the native 0x8E-byte stride
  explicitly all produced 1,209/1,210-byte candidates with 717 differences.
  Using the side-index member directly produced a 1,211-byte candidate with
  873 differences. The maintained source remains 1,210 bytes with 183
  differences. Retail holds the full side index in EDI and the replay-input
  pointer in EBX; these naive type/array rewrites continue to allocate them in
  the opposite registers and disturb the remainder of the owner. All were
  reverted.
- UpdateMovementAndOptions: local configuration pointers, references, byte
  views and a reference to the selected mode byte retained the same four-byte
  residual at +0x126. Retail loads `g_GameConfiguration` into ECX before the
  Player side index into EAX; the compiler continues to choose the reverse
  order. These aliases were reverted.
- EffectManager::AddedCallback: making the global ANM-manager pointer a
  volatile reference regressed the 51-byte owner from four to 25 differing
  bytes. The original natural helper-based source was restored.

## Follow-up source repair and boss probes (2026-11-19)

- BeginPlaybackStage: the HEAD candidate had nine differing bytes but moved
  seven offsets away from the 13-byte strict ratchet. The `volatile` reference
  to `g_GameManager.sides[0].runtime` was the cause of the new register-order
  differences. Replacing it with an ordinary typed pointer read into the
  existing `ReplayPlaybackScratch::side0Runtime` restores the recorded
  540/540-byte, 13-difference frontier. This is a frontier restoration, not an
  exact-function completion. Saved in `bb6c3c82` and simplified in
  `fee10d43`.
- CheckBulletCollision: additional explicit-label and `do`-loop variants still
  caused VC7.1 to schedule rectangle geometry before the shared dispatcher or
  to merge the dispatcher after both geometry paths. All variants were
  reverted. Maintained source remains 849/862 bytes with 539 differing bytes;
  do not rebaseline.
- UpdateNetworkService: local pointer and reference aliases, plus a local
  `localSide` value, did not reproduce retail's EDI network-state pointer and
  immediate field store. Candidate variants reached 1,645/1,633 bytes with
  909 differences or 1,649/1,633 with 986 differences; all were reverted.
  The maintained 1,645-byte candidate remains REGRESSED against the
  1,629-byte/966-difference ratchet. Do not rebaseline or use `volatile` to
  force EDI.
- ReplaySave: pointer-reference, direct aggregate, and `memcpy` spellings did
  not change the two EDI/ECX destination-base residuals. All experiments were
  reverted.
- EnemyManager::OnUpdate: scoping the side-category temporary, a shared
  draw-head helper, and explicit draw-group locals did not change the two
  EAX/ECX residuals. All experiments were reverted.
- EffectManager::AddedCallback: local manager/context ordering and ordinary
  reference aliases did not improve the four register-order differences. All
  experiments were reverted.

## Remaining register-order probes (2026-11-19)

Two small frontiers were revisited while keeping the large-function work in
view. The experiments below did not change their generated bytes and were
reverted:

- `PlayerMovement`'s last four-byte residual is the order of two independent
  loads at +0x126: retail loads `g_GameConfiguration` into ECX before loading
  `player->sideIndex` into EAX; the candidate does the reverse. A local
  configuration pointer plus a separately captured side index, and a pointer
  to the two-byte `sideFocusMode` array, both retained the same residual.
- `EnemyManager::OnUpdate`'s last two bytes are the early draw-group index
  temporary: retail zero-extends the byte into EAX, while the candidate uses
  ECX before forming the same EAX head pointer. `unsigned char`/`int` locals,
  explicit scaled pointer arithmetic, and an inline helper returning a
  reference to the selected head all compiled to the same ECX sequence.

These are compiler scheduling/register-allocation differences, not missing
logic. Do not keep extra helpers or aliases solely to perturb the allocator;
return to the two large owners (`CheckBulletCollision` and
`UpdateNetworkService`) for substantive reconstruction.

## Collision CFG reconstruction probes (2026-11-19)

IDA's native CFG clarifies the intended source structure more precisely than
our earlier nested geometry experiments:

- An outer scan loop contains an inner region-filter loop. The inner loop
  reloads `*slot`, rejects null/unsupported/delayed regions, and then tests
  `radius`.
- A nonzero radius breaks out of the inner loop into the circle test. A zero
  radius keeps execution in the inner loop and performs the rotated-rectangle
  or AABB test there. Rectangle misses and circle misses both jump to the
  inner-loop `next_region` label; rectangle hits jump forward to the common
  hit dispatcher.
- Circle hits fall through into that same dispatcher. The dispatcher sends
  direct hits to the shared hit-counter tail after the cancellation reward
  path; cancellation hits either break the outer loop into reward creation or
  advance to the next region.

The following source-only probes were compiled and then reverted:

- `if (radius == 0.0) goto rectangle_geometry` reduced raw differing bytes
  from 539 to 529, but changed the native `jnp` radius branch to `jp` and made
  the candidate 851 bytes instead of 862. It is not an acceptable frontier
  improvement.
- Moving direct-hit increment/return to a shared tail reproduced the native
  long branch to the counter tail, but grew the frame from 0x34 to 0x38 and
  produced 832/862 bytes with 595 differing bytes.
- Rebuilding the IDA inner/outer-loop shape literally produced 801/862 bytes,
  714 differing bytes, and changed the prologue/argument register allocation.
- Aliasing the local slot pointer to `scratch.cursor.slot` produced 830/862
  bytes with 711 differing bytes. The compiler stopped using the native ESI
  cursor and the candidate prologue no longer matched.

All probes are reverted. Maintained source remains 849/862 bytes with 539
raw differing bytes against the 842-byte/705-difference ratchet. The next
attempt should combine the IDA loop nesting and shared direct-hit tail while
preserving the native 0x34 frame, EBX bullet argument, ESI slot cursor and EDI
Player receiver. Do not use volatile/register hints or padding to pin those
registers or fake the frame.

Additional nested-scan ordering probes (2026-11-19):

- A three-loop source with the common hit dispatcher before the circle block
  and a backward edge from successful circle tests produced 831/862 bytes
  with 694 differences. A variant with a loop-local region pointer produced
  835/862 bytes with 693 differences. Both changed the entry allocation:
  retail keeps EBX=Bullet, EDI=this and ESI=slot, while these candidates
  kept this in EBX and the slot in ESI. The radius edge also remained `jp`
  rather than the native `jnp`; restoring the volatile radius read and
  double-zero comparison produced 831/862 with 695 differences. These are
  CFG evidence only, not accepted frontier improvements; all variants were
  reverted. Preserve the 0x34 frame and original argument/register roles.

## Boss and leaf native register-lifetime captures (2026-11-19)

The retained IDA project was queried directly for the original owners rather
than inferring their control flow from the current object files:

- `Supervisor::UpdateNetworkService` is `sub_430AA0`, native size `0x661`
  (1,633 bytes). The decompilation identifies a long-lived network-state
  pointer (`v2`) and a cached `timeGetTime` callback (`v3`); the native entry
  uses EDI for the state pointer and EBX for the timer callback. The source
  currently builds 1,645/1,633 bytes with 909 differences. A source-only
  pointer-lifetime rewrite that kept one `SupervisorNetworkState *` through
  the idle/join, retry and peer-ready branches, plus a local timer callback,
  produced 1,580/1,633 bytes with 1,106 differences. It was reverted. The
  earlier simpler pointer/reference/local-side probes also failed. Preserve
  the existing 20-byte `NetworkServiceStackScratch` overlay and do not
  rebaseline the regressed frontier.
- `PlayerLifecycleView::UpdateChargeAttack` is `sub_41F810`, native size
  `0x4BA` (1,210 bytes). Native entry loads the full 32-bit side index into
  EDI, multiplies a copy by the 0x8E-byte `ReplayInputState` stride, and keeps
  the input-record pointer in EBX for the input member calls. The first focus
  mode byte is then read through the global configuration pointer plus EDI;
  the second mode read is saved in the one-byte `[ebp-1]` local. The maintained
  source is 1,210/1,210 bytes with 183 differences. Widening the short side
  local, changing the input reference to a pointer, indexing the input array
  directly from `sideIndex`, reordering the declarations, and expressing the
  input base plus side as a pointer increment all failed: candidates were
  1,209/1,210 bytes with 717 differences or 1,211/1,210 bytes with 873
  differences. A further full-function side capture (`int side = sideIndex`
  reused for every later side-dependent sound/effect/configuration access,
  while the input reference still indexed the live member) produced
  1,200/1,210 bytes with 648 differences. It retained the wrong EBX/EDI
  allocation and shortened several later branches; it too was reverted. Keep
  the maintained 183-difference baseline. The next attempt must explain the
  natural lifetime that keeps the side index in EDI while the input object is
  in EBX, without register hints.
- `EffectManager::AddedCallback`: expanding `ResolveEffectAnm` into an
  explicit local manager pointer changed the callback from four to seven
  differences and reversed the native context/manager register roles. The
  helper-based source remains the best candidate.
- `ReplayManagerView::SaveReplay`: initializing the destination byte pointer
  first and advancing it in a separate statement left the two EDI/ECX
  destination-base residuals unchanged. The original direct typed aggregate
  copy remains the best candidate.
- `EnemyManagerView::OnUpdate`: a const unsigned draw-group local and direct
  typed pointer addition both preserved the two EAX/ECX residuals at the
  draw-head lookup. The maintained direct array expression remains unchanged;
  no register or volatile hints were added.
- `EffectManager::AddedCallback`: local context/manager pointer aliases, both
  declaration orders, and an explicit inline expansion of the ANM lookup all
  regressed the callback from four to seven differences. The helper-based
  implementation was restored.

## Recent accepted natural C++ recoveries

These commits improve native source fidelity while preserving their affected
exact owners; they do not add canonical exact-function credit:

- c231fc16 and cadfc828: direct ExAttack draw/history/VM array access.
- d1b29b50: Type4 source extra pointer is read from the typed +0x34 record
  member rather than an unknown-byte pointer overlay.
- ecd40217: Player Type9 polar helpers use the native position's first two
  floats as a Float2 view, with the 12-byte XYZ layout checked.
- bac91bf6: ANM 3D draw uses the actual 4x4 matrix values; D3DX pointer
  conversion is confined to the API boundary.
- c8d57624: Title music/SFX volumes are native configuration bytes with
  signed reads and direct member writes.
- 38a9f1e0: WinMain uses the executable-directory byte array and ordinary
  Win32 system metrics.
- ddcd49e1: DirectSound Lock uses LPVOID output buffers and DWORD lengths.
- 285e2339: Supervisor's two DirectInput capability records are treated as
  the actual DIDEVCAPS array.
- 57d3f852: viewport methods use the native Supervisor D3D device member.
- a2751ec4: the network retry path uses a direct fixed-value return; this
  naturalization did not improve the strict frontier and the current network
  owner remains REGRESSED.

## Private analysis retention

The active Ghidra project, factory runtime state, original target, toolchain,
reference checkout, two C2 projects and C2 allocator/register/codegen reports
are retained inputs. The small .analysis C2 heap/decompilation inputs, four
GDB recipes and three explicitly listed natural-C++ diagnostics are retained.
One-shot IDA call captures, Ghidra export folders, compiler OBJ/PDB files and
obsolete trial source files are disposable. Recreate selected objects from
maintained source and pinned profiles; do not use git clean -fdx.


## UpdateNetworkService: recovered entry and session CFG (2026-11-19)

IDA identifies `Supervisor::UpdateNetworkService` as `sub_430AA0` (native
extent `0x661`). The current source reconstruction now matches the first
`0x103` bytes byte-for-byte and restores the native `0x14` stack frame. The
local frontier is 1,645/1,633 bytes with 717 differing bytes, down from the
previous 966-difference baseline. This is a real partial recovery, not an
accepted exact match.

The source now follows the native receiver and branch lifetimes:

- Capture `g_SupervisorNetworkState` immediately after entering critical
  section 4. Use that receiver for the initial state/join reads, the join-side
  `localSide` store, the searching/host-wait state checks, and
  `GetRemotePeerId`.
- Keep methods whose native receiver is reloaded from the global as explicit
  `g_SupervisorNetworkState->...` calls. Reload the cached receiver after the
  searching text draw, after host-session setup, and after the successful
  remote-peer check before frame preparation.
- Move `HOST_SESSION` out of the initial idle branch. The initial idle-host
  path and the retry path jump to it; the join and non-idle paths pass through
  the searching check; a non-searching state skips the host body and reaches
  the host-wait check. The retry predicate now preserves the native
  short-circuit order: elapsed time below the jittered threshold OR join mode
  equal to one goes to wait; otherwise reset and retry hosting.
- Widen the service return local from `unsigned short` to the native 32-bit
  `SupervisorNetworkServiceResult`. This restores the `0x14` stack frame and
  lets the ordinary scratch return value remain distinct from reset/wait
  status.

Remaining work: the service status still spills to a stack home instead of
reusing EDI, the network-wait branch is 14 bytes later than the native label,
and the compiled body is 12 bytes longer than the native extent. Do not
rebaseline this frontier. Continue from the current natural-C++ control flow;
no register hints, inline assembly, padding, or synthetic bytes were added.


## UpdateNetworkService: timer and frame-local lifetime progress (2026-11-19)

The best current natural-C++ candidate is still 1,645/1,633 bytes and remains
REGRESSED against the maintained 1,629-byte ratchet, but the focused raw byte
difference count has improved from 717 to 695. The 20-byte native stack frame
and the recovered entry/session control flow remain intact. This is a useful
source checkpoint, not an exact-function claim or a frontier rebaseline.

Two source lifetime changes account for the new candidate:

- A `DWORD (WINAPI *getTime)()` local is scoped to the early connection-state
  machine and used by its searching/host-wait timer reads. This gives the
  compiler one ordinary function-pointer lifetime for the native timer
  callback instead of six independent `timeGetTime()` expressions. The
  pinned object retains the native EBX callback role on the relevant paths.
- The frame gate captures the 32-bit frame counter in one `int frame` local;
  the mutually exclusive initial-frame prefill loop reuses that same local
  for its negative signed counter. This reduces the focused difference count
  by 22 without changing the 0x14 stack frame.

The remaining prediction-path mismatch is now explicit. Retail loads the
frame counter into EBX at `0x430CFA`, keeps it through the first `GetSeed`,
and pushes that captured value to `InsertPredictedFrame`. The current source
reloads `networkTiming458.frameCounter` after `GetSeed`, so its object uses a
load from `[esi+0x458]` rather than preserving the original value. The
pre-fill branch also needs to reuse EBX naturally for its `-9998` counter.
Try C++ scope/lifetime forms that make these mutually exclusive values share
one ordinary register without `register`, `volatile`, asm, padding, or a
compiler-profile change. Keep the 1,629-byte ratchet and residual list
unchanged until a monotone frontier improvement is independently verified.


## Near-exact leaf register-order follow-up (2026-11-19)

Two small register-order owners were revisited while the network service boss
remained the primary source target. These are diagnostic results only; all
source probes were reverted and their frontiers remain unchanged.

- `PlayerLifecycleView::UpdateMovementAndOptions` remains 1,900/1,900 bytes
  with four differing bytes. Retail loads `g_GameConfiguration` into ECX at
  `0x41C296`, then reloads `Player::sideIndex` into EAX at `0x41C29C`, before
  comparing `sideFocusMode[sideIndex]`. The candidate does the same two loads
  in the opposite order. An object reference to the configuration, a reference
  to the global pointer, a const pointer alias, a reference to the focus-mode
  byte, a reference to the two-byte focus-mode array, and a small inlined
  accessor all retained the same EAX/ECX order and four-byte residual. Do not
  add volatile or register hints to force the order.
- `EnemyManagerView::OnUpdate` still has the two-byte draw-head register
  residual: retail uses `movzx eax, [ebx+0x3387]` followed by
  `lea eax, [esi+eax*4+0x2AC410]`; the candidate uses ECX for the byte index
  and EAX for the resulting head pointer. Hoisting the identical draw-head
  assignment out of both successful branches changed the CFG and regressed
  the focused candidate to 3,006 differences; it was reverted. Earlier
  unsigned/int index locals, explicit typed pointer addition, and an inline
  reference-returning accessor also did not change the register order. Keep
  the maintained shared append tail and do not rebaseline.

A separate `UpdateNetworkService` experiment retained the captured frame
value across `GetSeed` and used a distinct signed prefill index. It grew the
stack frame from 0x14 to 0x18 and reached 720 differences, so it was reverted
in favor of the committed 695-difference, 0x14-frame checkpoint. The first
predicted insertion still reloads the frame counter after `GetSeed`; this is
the next network-service lifetime mismatch to solve.


## CheckBulletCollision IDA loop ownership and dispatcher-order follow-up (2026-11-19)

A fresh native CFG/decompiler review confirms that the rectangle and circle
paths are owned by different loop exits, not merely opposite arms of one
geometry `if`:

- The inner loop reloads `*slot`, rejects null/unsupported/delayed regions,
  and leaves the valid region in the current cursor.
- A zero radius remains in the rectangle scan. The rotated rectangle and AABB
  tests execute inside that scan; misses advance the slot and continue the
  scan, while successful geometry branches backward to the hit dispatcher.
- A nonzero radius exits the rectangle scan. The circle test executes after
  that exit; a miss advances the same slot and resumes the scan. A circle hit
  falls through to the dispatcher at native `0x41E07E`.
- The dispatcher reloads the active region and distinguishes direct-hit kind 1
  from cancellation kinds 2/3. Direct hits branch to the common hit-count tail
  after the cancellation reward path; cancellation kind 2/3 checks the live
  bullet pointer/type, then either enters reward creation or advances the slot.

Source experiments on this CFG were compiled against the focused frontier and
reverted:

- A literal nested rectangle-scan loop with circle geometry after its break
  produced 828/862 bytes and 697 differences, but changed the entry register
  roles (it did not retain EDI=Player, ESI=slot, EBX=Bullet).
- A source-ordered circle/dispatcher/rectangle label graph retained the native
  entry roles, but VC7.1 still laid out rectangle geometry before the shared
  dispatcher. It produced 825/862 bytes and 583 differences, worse than the
  maintained 849/862-byte, 539-difference candidate.
- A direct-hit jump to the common post-reward hit-count tail did not by itself
  change the compiler's block ordering.

All collision source experiments are reverted. Keep the 0x34-byte frame,
entry registers, native `jnp` radius edge, and existing 842-byte ratchet. The
next attempt should change the lifetime/ownership of the scan cursor and
common dispatcher without introducing another live local or moving the
rectangle body ahead of the circle/dispatcher blocks. Do not rebaseline.

A two-case `switch (radius == 0.0)` gate was also tested after the nested-loop
review. It materialized the X87 comparison as an integer boolean (`set/jmp`
sequence) and expanded the owner to 872/862 bytes with 651 differences. This
was both larger and less faithful to the native `test ah,0x44` / `jnp` edge;
revert it and keep the original volatile X87 radius read.


## ReplaySave, UpdateNetworkService and movement-leaf register probes (2026-11-19)

The near-exact ReplaySave leaf was revisited at the two recorded bytes. The
remaining instruction pair is a destination-base allocation difference:
retail loads the temporary-buffer base into EDI at function offset +0xB6 and
forms `EDI = EDI + currentOffset - 0xC0` at +0xB9; the maintained candidate
loads the same stack home into ECX and forms `EDI = ECX + currentOffset -
0xC0`. The eight-DWORD POD frame-header copy and all surrounding bytes remain
unchanged. Direct typed assignment, an explicit byte cursor, the compiler
`memcpy` intrinsic, and a short-lived cursor advanced/restored around the
copy all retained the same two residual bytes. Extending the stage-buffer base
pointer lifetime across sizing, input-copy and FPS-copy loops regressed the
owner to 1,254/1,238 bytes and 926 differences; it was reverted. Do not add
volatile/register hints or retain an artificial helper solely to select EDI.

`UpdateNetworkService` was also tested with direct `timeGetTime()` imports
instead of the scoped callback pointer. The candidate remained 1,645/1,633
bytes with 695 differences and the same instruction sequence. Reusing the
captured frame-gate local for the first `InsertPredictedFrame` while using
direct timer imports still grew the stack from 0x14 to 0x18 and produced 720
differences; it was reverted. The maintained 695-difference source and the
1,629-byte ratchet remain unchanged. The next prediction experiment must
produce the retail EBX snapshot across `GetSeed` without creating a stack home
or changing the 20-byte local frame.

## 2026-11-20 Player movement focus loads

`PlayerLifecycleView::UpdateMovementAndOptions` at `0x0041C170` has one
instruction-order residual under the pinned VC7.1 profile in
`config/frontiers.toml`. At `0x0041C296`, retail loads the configuration
pointer into ECX and then the live Player side into EAX. The maintained
source emits those loads in reverse order. Both feed the same indexed
byte comparison. After relocation replay, the differing bytes are
`+0x127..+0x12E`.

### Extent and owned data

- The owner has 1,835 code bytes, one alignment NOP and two eight-entry
  switch tables. Its 1,900-byte physical extent ends at `0x0041C8DB`
  inclusive.
- The configuration slot at `0x004A7E78` is the native GameManager member
  `configurationE8`. Its DIR32 field uses the GameManager symbol plus
  addend `0xE8` and moves from retail `+0x128` to candidate `+0x12B`.
  All 66 effective destinations agree, including 23 calls and the
  compiler's floating-point zero. The other 65 fields retain their
  positions.
- The maintained focus-effect table is the 16 DWORDs at
  `0x004A1A10..0x004A1A4F` in retail `.data`, between the Player
  callback arrays at `0x004A19D0` and `0x004A1A50`. The defining COFF
  `.data` section matches all 64 bytes with zero relocations and SHA-256
  `16f82c6f1ce9daef50da8b2236fd0497bd8782926c6ad199a3b23d548d6b1714`.
  The zero-addend DIR32 field at `+0x1B5` binds to that definition.
  The rebuild preserves the raw owner bytes and field identities from
  the declaration-only control. The maintained symbol keeps its
  previous external linkage. Retail linkage remains unresolved.
- The cold caller replay passed all 21 canonical `PlayerRuntime.cpp`
  units, covering 5,391 physical bytes and 288 relocation fields.

### Replacement of the configuration load

At allocation driver entry `0x1077C08E`, the maintained carrier already
has a side MOV. At `0x1077EE6B`, C2 creates opcode `0x26A` for the
raw configuration slot immediately after that MOV. It unlinks the
temporary at `0x1078952D` in the maintained carrier and `0x10789246` in
the reference controls. The builder at `0x1075F004`, reached through
the request at `0x10776AE0`, reuses the freed record for an ordinary MOV.
The write at `0x1075F0A1` attaches the replacement after the side load.
Declaring a configuration alias before the side in source can still
take this path. Both maintained destinations are virtual at global
allocator exit.

### Post-charge lookup as a scheduler control

The exact post-charge lookup at `0x0041F914` has the same absolute slot
and Player side operands. Both its loads and the maintained movement
loads reach scheduling with one dependency per load, equal priority
`0xA000`, and side-first incoming ordinals. The tie-breaker at
`0x107B7396` keeps that order. A bounded scan of tracked extents found
twelve indexed accesses with independent absolute and base-relative
loads within the preceding ten instructions and within one straight-line
region. Only movement loaded the absolute pointer first.

Binding a reference to the selected mode byte and using its value as
the analog branch's true result creates a separate LEA. Both pointer
loads feed it, and the subsequent byte load writes CL. The register
family check at `0x107B73C7` adds a separate ECX-to-CL write dependency
and raises the configuration load's priority from `0xA800` to
`0xAC00`. That recovers the original order and register choices.
When the byte load directly uses the indexed operand, the existing
read edge absorbs the write dependency at `0x107B4124`, and the loads
tie again.

The mode-reference control retains the LEA and emits an indirect CMP.
The pair is two bytes longer than retail's indexed CMP, with alignment
absorbing the displacement before the option loop at `+0x680`.
Instruction alignment verified 485 other encodings, all 62 corresponding
direct branches and the 16 table destinations.

### Local allocation and compare cleanup

The pre-scheduler pass at `0x10786238` folds the analog threshold's
temporary WORD load without advancing the scratch cursor. After
scheduling, the peephole at `0x107B7DAD` replaces a register operand
in the CMP with the preceding byte MOV's source at `0x1075AC02`.
It unlinks the MOV and clears the compare's eligibility bit. The
indexed source stays indexed in the maintained, direction-join and
fresh-side controls. The mode-reference control retains its LEA.

Global substitution can remove the LEA earlier. The indexed-address
check at `0x1072E3CC` requires both terms to have allocation class 2.
With the original input leaf opaque, a direct live-side term has class 3.
The failure sets bit `0x400` at `0x10783EF6`, and the work-list skips
the copy at `0x10783CEA`. Giving both terms class 2 allows the LEA to be
unlinked at `0x107A4378` and the indexed source to replace the indirect
one. A fresh by-value side with a const-reference alias does this with
all nine input calls opaque. It leaves nine differences, the eight
load-order bytes and the opposite SIB byte. Its side gets EAX globally
with weight 16. The configuration load enters with weight 8 and drops to
-60 in the queue, leaving ECX to the local chooser.

A configuration-first snapshot can move the load before the side by
global allocator entry. A configuration reference with the side as a
live reference scores 12 for the pointer. In a by-value aggregate both
ordinary MOVs score 16, from two counts for the definition and two for
the address use, scaled by four at `0x10782919`. The backward ordinal
walk at `0x1077FAE3` and comparison at `0x10782B4E` give the earlier
configuration the tie. In both cases the pointer gets EAX, and its
shorter absolute-load encoding shifts the following code.

### 2026-11-21 Player movement direction join and scratch allocation

The retained `.analysis/movement-focus/focus-order-control.cpp` captures
the live side at eight jump predecessors and the neutral fallthrough.
The neutral and three upward exits capture configuration first. The
other exits use a common configuration assignment. C2 merges those
reads after the direction stores. Side scores 63 and gets EAX globally.
Configuration scores 36 and gets ECX globally. The load order, field at
`+0x128` and indexed CMP with SIB byte `0x08` all match retail. Capturing configuration
only in the neutral exit keeps the load order and reverses the SIB.
Using the movement-field reference for both position increments
restores the original x87 encodings in the retained control.

The complete 1,900-byte join comparison keeps all 66 relocation
positions. It differs in seven bytes at the flag update near `+0x156`
and seven in the first effect call near `+0x186`. The maintained
carrier makes two local scratch requests at the lookup. The join
has both registers assigned globally, so its first local request
is the flag delta. The cursor advances at `0x10776893` in the
EAX, ECX, EDX sequence. With the computed focus result occupying
EAX, the join chooses ECX for the delta and EDX for the first effect
receiver. The maintained carrier chooses EDX for the delta and
wraps to EAX for the receiver. The receiver's register affects
scheduling of its loads relative to the argument pushes.

### Lowering the two flag-word reads

ECX and EDX are both free when the delta request starts in the
maintained and join controls. The later XOR still has two memory
operands at global allocation exit. Local lowering inserts the
old-word MOV before the XOR at `0x1075F099` and binds the destination
at `0x1075A482`. The scheduler moves that MOV ahead of the delta
operations at `0x107B54CC`. The old word gets ECX in the maintained
carrier and EDX in the join.

A source snapshot of the old flags before the analog threshold or
after the digital query merges the reads into one load and a register
copy. The first effect receiver returns to EAX with its original
instruction order, while the flag region before the branch is five
bytes shorter. A direct two-bit XOR update, an entry-state reference
and an intrinsic copy of only the analog count all reproduce the
join's original two residual regions. Putting the unfocused branch
first with an explicit forward edge also preserves the residual.
Inverting the branch rearranges both switches and leaves the first
local request at the flag update.

### Keeping the focus side live for the first effect

Using the lookup's by-value side snapshot as the first effect argument
extends it through the flag update. The join assigns EDI to the side,
spills the input receiver and assigns EAX to the configuration.
The fresh-side control spills the side around `IsHeld` and reloads
ECX. A live reference instead lets C2 reload the side at the original
call site, leaving only the nine-byte lookup residual.

On the direction-join body, assigning the first effect's side from
the lookup before the analog threshold and refreshing it after the
digital query preserves the original lookup. EAX and ECX are both
occupied at the first bitfield request, so the delta gets EDX.
The cached side stays live in ECX. The old flag word then gets EDI
and the effect call pushes the cached side, while retail reloads it.
This control identifies the register pressure that recovers EDX
and the later effects of keeping that pressure alive.

### Co-visible position conversion and indexed addressing

The exact three-byte `PlayerPositionView::operator float *()` body at
`0x004343D0` changes the maintained comparison's SIB from `0x08` to
`0x01` when visible before or after movement. It still emits the
original call and also changes ten later x87 bytes. Using the native
movement-field reference for both position increments restores the
SIB and x87 encodings, leaving the original eight-byte residual.

The address walk at `0x1077302C` follows copy definitions and constructs
the first indexed operand at `0x1079FE1A`. The visible position body
makes it follow the configuration definition first. Both versions
already have the side MOV at that point, so the later replacement at
`0x10778417` preserves their respective SIB order with a late
configuration load. Truncating the probe after the clamps also
reverses the SIB. Keeping the option loop restores it.

Making the real `IsHeld` body visible gives both address terms class
2 and removes the LEA in the mode-reference control. The same nine
calls remain, but entry allocation, the live side register and the
configuration's assignment change. Six equivalent 18-byte input
leaves preserve that outcome. DWORD-mask and compound-assignment
controls can compile to a separate 16-byte leaf and change the caller.
The original 18-byte body also stayed exact in the later co-visible
position, Float3, ANM and timer controls.

Co-visible PlayerRuntime, PlayerAddedState, charge, death, effect-spawn,
animation-script and Float3 leaves preserve the lookup in the relevant
maintained and join carriers. The four exact PlayerAddedState units
replayed 665 bytes and 30 fields in their shared carriers. Charge and
death stayed exact across 1,210 bytes and 70 fields and 753 bytes and
38 fields, respectively. CirnoLab's complete player carrier at
`afebc8b` and ten compilable historical TH09 drafts also retain the
side-first residual under their tested contexts.

### Configuration slot access and carrier boundary

An earlier slot batch used the native member address, pointers
to members, a typed byte offset and a DWORD address. Ordinary
dereferences and typed pointer copies preserve the maintained load
order. A four-byte `memcpy` snapshot of the slot gives the pointer
EAX and the shorter absolute load. The direction-join slot variants
retain their lookup and the two later register residuals. Some also
change the x87 position operand order.

A configuration pointer reference paired with a side value, a mode array
reference paired with a side value, and a two-reference aggregate all
retain the same eight unmasked differences in the full owner. Pairing
the array reference with a side reference adds ten later x87 differences.
All four controls retain the 1,900-byte extent and 66 relocation fields.

The combined `ReplayInput.cpp` control compiles with the native
`g_Supervisor` declaration and a `SupervisorReplayView` cast at its replay
uses. It leaves the same eight differences and all 66 field positions in
movement. The compiler renumbers the two private switch-table labels.
Both still resolve to the same offsets within the owner.

### 2026-11-21 allocator-entry IL and source-shape batch

At allocator entry `0x1077C08E`, the maintained join block holds one
explicit MOV of the live side into a temporary. The compare's indexed
operand uses that temporary as base. It uses the configuration global
directly as a register-class variable reference for the index. The
helper at `0x1075F093` links the reload created during allocation
immediately before the using instruction, so it always follows the side
MOV. The scheduler tie at `0x107B7396` compares 16-bit ordinals at
record `+0x36`, not node addresses. The ignored
`.analysis/movement-focus/trace-ildump.py` prints that IL window
through `trace-run.py`.

These full-owner variants kept the eight-byte residual unchanged:
`this->` throughout, a ternary stored directly to the bitfield, commuted
index and pointer-sum spellings, byte-offset casts, unsigned side and
index types, char or int casts of the mode byte, and a plain inline mode
accessor. Using `this->sideIndex` only at the lookup flips the SIB byte.
Inverting the mode branch, a one-case switch, a row-end helper or
separate bitfield stores regress the owner. A diagnostic volatile
configuration load still emits the side first and flips the SIB, so an
explicit early configuration MOV is not sufficient by itself. Defining
`ReplayInputState::IsHeld` in the carrier inlines all nine calls and
grows the owner to 1,916 bytes.

### Retained compiler controls

The ignored `.analysis/movement-focus/` directory keeps the merged
direction control (`focus-order-control.cpp`), a copy of the maintained
source and the GDB trace recipes. The fresh-side and mode-reference
control sources were removed in the 2026-11-22 cleanup; the sections
above describe their shapes and results. Rebuild them from those notes
before tracing their allocation paths again. `probe.py` checks the target and
compiler inputs and replays complete function objects through their
COFF fields. `trace-run.py` compiles a selected source under the
pinned profile and supplies its object and trace. The checkout's
[handoff](RE_HANDOFF.md#player-movement-handoff) lists the shortest replay.

Read back the cold frontier object with the local relocation replay.

```bash
python3 -B - <<'PY'
from pathlib import Path
import sys

sys.path.insert(0, '.analysis/movement-focus')
from probe import SYMBOL, examine

body, fields, _, differences = examine(Path('build/frontier/player-movement.obj'), SYMBOL)
print(len(body), len(fields), [hex(offset) for offset in differences])
PY
```

Secondary trace recipes cover the slot MOV replacement in
`load-normalization`, compare folding in `peephole`, SIB terms in
`address-walk` and the post-charge control in `peer-scheduling`.
Debugger-built objects were compared with their cold counterparts over
each tested extent and all its fields.

## 2026-11-22 whole-image link findings

These conclusions come from linking every canonical carrier with the
pinned VC7.1 linker (`scripts/trial-link.py`). Each source change was
checked by a cold scoped replay of its carriers.

- One profile per carrier. Twelve multi-profile sources unify directly.
  AnmManagerLoad, CardAttack, PlayerRuntime, AnmManagerRuntime and
  ScreenEffect needed two profiles only because reconstruction-local
  accessor helpers relied on inline expansion. Writing those expressions
  directly lets one `/Ob0` profile reproduce every unit. Under `/Ob0`, the
  real D3DX8 inline vector helpers, `sinf`/`cosf` and member array
  constructors stay out-of-line calls, as in the original.
- Header inline bodies as COMDATs. `MidiOutput::PlayFile` (0x00423361),
  `InputView::IsPressedScrolling` (0x00423158) and
  `AnmManager::SpriteHasTexture` (0x00423469) are inline bodies that the
  `/O1` Title unit emitted out of line. A visible body still changes a
  caller's register allocation. For example, `SetInterruptArray` needs
  `SpriteHasTexture` visible but not expanded.
- Real classes reproduce compiler-generated members. The real
  `DummyMidiTimer`, with no user-declared destructor, emits its
  constructor, implicit destructor and scalar deleting destructor. They
  match 0x0042F3B0, 0x0042F3F0 and 0x00431680 byte for byte. The deleting
  destructor appears only in a `/Ob0` caller. A user-declared destructor
  adds a vtable store, and the original has none.
- D3DX8 linkage. The external entry points use the SDK's C linkage
  (`_D3DX...@N`). TH09 strides metadata slots by 0x14 bytes and the
  linked provider writes 0x1C bytes. [Provider evidence](#2026-11-23-d3dx8-provider-and-surface-output)
- Data identity. Reversing the labelled data blocks found globals that
  were really fields of larger objects: the score table's name column,
  the score record's date, GameWindow's power flags and the character
  resource table's ECL column. Splitting them works in the original
  layout and fails in the reversed one. Third-party LZSS code addresses
  the inside of its tables, so its operands must not mark object starts.
- Attract-mode RNG state is not reproducible across runs of the original.
  A debugger stop per frame also perturbs timing. A behavioural trace
  needs replay-driven input.

## ChargeAttack entry-register and timer-reuse probes (2026-11-19)

IDA's `UpdateChargeAttack` entry is precise: EDI holds the full DWORD
`sideIndex`, EAX forms `sideIndex * 0x8E`, and EBX receives the
`ReplayInputState` record pointer. EDI remains the side value for the two
configuration-row reads, then is reused for the `timer303A4` receiver after
those reads; EBX is reused for `timer303B0`. The candidate at this checkpoint
reversed the initial pair (EBX=side, EDI=input record) and was at
1,210/1,210 bytes with 183 differences.

New natural-C++ probes established both sides of the lifetime problem:

- A full-DWORD `int side` plus direct `g_ReplayInputStates[side]` call-site
  access, with the side value also used for later sound pan, reproduces the
  target entry assignment (EDI=side, EBX=input pointer). It reaches
  1,208/1,210 bytes but has 526 differences. The side local stays live across
  the timer branch, so VC7.1 keeps EDI for that value and uses EBX for
  `timer303A4`; retail reuses EDI for `timer303A4` and reloads `sideIndex` from
  the player for later sound pan. The first two configuration operand orders
  also differ by commutative address terms only.
- Removing later side-local uses makes the compiler return to EBX=side and
  EDI=input, with 816 differences. A local input pointer with a full-DWORD
  side spills the side to the stack and reaches 921 differences. Direct array
  indexing from the live `sideIndex` member recomputes the input address at
  every call and grows the owner to 1,270/1,210 bytes with 947 differences.
- Precomputing the later sound-pan value lets the side local die earlier, but
  introduces a new stack home and reaches 1,200/1,210 bytes with 895
  differences. Removing the redundant `modeSnapshot` byte changes the owner
  to 1,205/1,210 bytes with 864 differences, but does not recover the target
  entry registers.

These variants were reverted. The [later exact lookup](#2026-11-21-chargeattack-input-and-timer-allocation)
recovers the initial roles and the reuse of the timer receivers.

## Collision triple-loop and near-leaf follow-up (2026-11-19)

The IDA three-loop structure was tested again with both region-pointer forms.
A version that removed the region local and repeatedly dereferenced `*slot`
reached 798/862 bytes with 675 differences, but changed the entry allocation
and expanded the frame to 0x38 (EDI became the position argument). A version
that retained the region local and placed the circle, dispatcher and
rectangle labels in native order reached 813/862 bytes with 722 differences;
its entry became EBX=Player, EDI=slot and ESI=region instead of the required
EBX=Bullet, EDI=Player and ESI=slot. It also retained the wrong X87 radius
operand order/branch polarity. Both probes were reverted; keep the maintained
849/862-byte, 539-difference source and the 0x34-byte frame.

The ReplaySave stage-buffer-base experiment was extended to reuse one captured
base for both input and FPS payload loops. It retained the 1,238-byte extent
but regressed from two differences to six. The frame-header destination still
uses ECX as the LEA base instead of retail EDI. The source was reverted.

## ReplaySave stage-root lifetime and frame-row cursor probes (2026-11-19)

The retail SaveReplay setup keeps `&replayManager->stageBuffers[0]` alive
across the allocation call. The sizing loop advances a separate cursor, while
the later input and FPS loops reuse the saved base. A natural C++ split with a
function-scope `stageRoot`, a distinct sizing-only `stageCursor`, and later
`stageRoot[loopIndex]` accesses reproduces the native early stack-home store at
+0x36 and keeps the function at 1,238 bytes. It has six differences rather than
the maintained two: the row/cursor stack-home stores at +0xA3/+0xA6 are in the
opposite order, and the header-copy destination still loads ECX rather than
EDI at +0xB6/+0xB9. This probe was reverted.

Several attempts to expose the native per-stream frame-row cursor as an
explicit local or to reorder the `link` and frame-header declarations made
the compiler retain additional stack homes (1,254/1,238 bytes, 897 or more
differences; an advancing row pointer reached 1,302/1,238 bytes and 962
differences). Those were reverted too. Preserve the two-difference leaf;
next isolate the two stack-home stores without extending the frame or changing
the eight-DWORD POD assignment.


## ChargeAttack phase-local side and collision common-tail follow-up (2026-11-19)

A new `UpdateChargeAttack` register-lifetime experiment clarified the exact
boundary between the initial input phase and the later cooldown/attack phase.
The native entry needs EDI=the full DWORD `sideIndex` and EBX=the
`ReplayInputState` pointer. The native cooldown path then reuses EBX for
`timer303B0` and EDI for `timer303A4`, and reloads the live `sideIndex` from
`this+8` for the post-cooldown focus-mode test.

- A function-scope `int side = sideIndex` with direct input-row calls, while
  keeping the existing later member reload, compiled to 1,211/1,210 bytes and
  816 differences. VC7.1 still chose EBX=side and EDI=input.
- Scoping the full-DWORD side, volatile side-mode byte and direct input-row
  accesses around the initial input/charge branch, keeping the sound-pan uses
  on that local, and reloading `sideIndex` for the post-cooldown test restored
  the target entry roles and the EDI/EBX timer receiver roles. The candidate
  was 1,202/1,210 bytes with 503 differences. Moving the `resolveAttack` label
  inside or outside the side scope did not change the result. This substantially
  changed the later attack-branch layout and is worse than the maintained
  1,210-byte/183-difference frontier; it was reverted.
- Adding a short-lived mode-path pointer or sequenced opponent/enemy-manager
  pointer locals did not improve that 1,202/503 candidate. The side-scope
  experiment was not promoted, and no exact-function oracle replay was
  available for `UpdateChargeAttack`.

The [later exact lookup](#2026-11-21-chargeattack-input-and-timer-allocation)
preserves the retail layout and ends the side lifetime before cooldown.

`CheckBulletCollision` was also tested with a shared direct-hit/reward
hit-count tail:

- Removing the volatile dispatcher pointer local and reading `(*slot)->type38`
  directly, then sending direct hits to the common counter tail, produced
  818/862 bytes and 598 differences. It retained the native entry registers
  and 0x34 frame, but had more differences than the maintained 849/862-byte,
  539-difference source. A scoped replay of the source carrier passed five
  other exact functions; it did not exercise `CheckBulletCollision` itself.
- Retaining the volatile dispatcher pointer while sharing the counter tail
  produced 825/862 bytes and 583 differences, again with the dispatcher and
  rectangle blocks in the wrong order. Both variants were reverted.

Keep the maintained 849/862-byte, 539-difference collision source. The
remaining structural target is still the native circle -> dispatcher ->
rectangle order, with rectangle success returning backward to the dispatcher.
Do not rebaseline or claim the shared-tail variants as semantic validation.

`SaveReplay` was tested with the in-place destination expressed as a C++
reference rather than a pointer local. It remained 1,238/1,238 bytes with the
same two residual bytes at +0xB6/+0xB9 (`ECX` versus `EDI` as the destination
base). The reference form was reverted.

`UpdateNetworkService` was tested with a prediction-only frame snapshot that
survived `GetSeed`, followed by a `DWORD`/unsigned frame-local variant. Both
expanded the stack from 0x14 to 0x18: the signed candidate reached 1,645/1,633
bytes and 720 differences; the unsigned candidate reached 1,647/1,633 bytes
and 728 differences. Both were reverted. Keep the maintained 1,645-byte,
695-difference candidate and the 0x14-byte stack frame; the native prediction
path's EBX frame snapshot remains an unresolved register-lifetime problem.


## UpdateNetworkService native frame/prediction and packet-flow recovery (2026-11-19)

The maintained `Supervisor::UpdateNetworkService` source now has a substantially
better natural-C++ reconstruction of the native frame and prediction CFG. The
native owner is `sub_430AA0`, 1,633 bytes, with a 0x14-byte local frame. The
strict local frontier remains 1,629 bytes / 966 differences, so the improved
candidate is still reported REGRESSED and must not be rebaselined.

Two source commits carry this recovery:

- `230e3f4c` — `gpt-web: restore native network prediction frame lifetime`.
- `61fbf69e` — `gpt-web: recover network frame and packet flow`.

The source CFG now tests the nonzero, non-sync frame case as the leading
prediction arm. An unsigned `frame` local is retained across the first
`GetSeed` and `InsertPredictedFrame`; the second prediction reads the live
network-side pointer after the second `GetSeed`, then reloads the live
`frameCounter` before its queue call. The preparation arm uses a separate
signed `queuedFrame` induction variable starting at -9998. These two mutually
exclusive roles compile to the native EBX lifetime: EBX is the frame counter
on the prediction edge and the prefill loop counter on the preparation edge.
The stack remains 0x14 bytes; no additional predicted-frame stack slot is
introduced.

The packet path also preserves the target's receiver lifetimes in ordinary
C++:

- `handshakeNetwork` is captured immediately before filling the handshake
  packet and retained across `GetSeed` and `SendPacket`, matching the native
  EDI receiver lifetime.
- `packetNetwork` supplies the frame packet side and sync-rate reads and stays
  live through the seed/checksum/send sequence.
- The packet-wait return stores `NETWORK_SERVICE_RETRY_FRAME` in the existing
  `scratch.returnValue` home before leaving the critical section.
- The final pair of input words is kept in `packedInput` and `secondInput`
  locals across the three `ApplyNetworkInput` calls. This removed eight further
  byte differences without changing the 0x14 stack frame.

The current candidate is 1,633/1,633 bytes with 307 differences, compared with
the prior maintained 1,645/1,633-byte, 695-difference candidate. The native
prediction branch's first and second argument reloads, handshake sender, packet
sender, and final input lifetimes are materially closer. The strict frontier
still rejects it because its size is four bytes above the 1,629-byte ratchet.
Do not change that ratchet or claim exactness. No full cold replay or scoped
exact replay of this nonexact owner was run.

The explicit `HAVE_PACKET`/`WAIT_FOR_FRAME` source labels and a fresh local
pointer for the readiness test did not change the final codegen. The compiler
still places the shared wait-return block at the end of the function rather
than the target's earlier block. The next network pass should attack this
packet-ready/return layout and the remaining packet/queue code without losing
the recovered EBX lifetime, EDI network receiver, or 0x14 frame.

Other continuation probes were reverted:

- A separate `predictedFrame` local and a DWORD snapshot across `GetSeed`
  expanded the stack to 0x18 and produced 720/728 differences.
- Using `frameNetwork` for the packet-ready and input reads also expanded the
  stack to 0x18 and lost the recovered EBX frame role.
- `CheckBulletCollision` was tested with an explicit circle/dispatcher/rectangle
  label graph and a shared direct-hit/reward counter tail. The latest version
  reached 811/862 bytes and 726 differences, reversed the entry register roles,
  and was reverted. Keep the maintained 849/862-byte, 539-difference source.
- `SaveReplay` was tested with a mutable byte cursor, a typed cursor, and a
  direct frame-header pointer load. All retained the same two +0xB6/+0xB9
  ECX/EDI destination-base differences and were reverted.

The maintained frontiers for `CheckBulletCollision` and `SaveReplay` remain
unchanged. Keep all failed variants out of maintained source.


## TH09 natural-source continuation (2026-11-20)

The following five local source commits follow the 2026-11-19 network/charge
work. They improve the readability and native ownership model of already
exact carriers; they do not increase the authored exact-function count.

- `c7e8a8ac`: `AsciiManager::OnUpdate` now reads the existing typed
  `menuState97C8Active` / `menuStateA7A8Active` members, and walks the real
  `AsciiGameManagerSideView` array rather than incrementing a byte pointer
  between side flags. The changed source replay passed 10 exact units, 4,206
  bytes and 264 relocation fields. Rebuilding the carrier changed three
  private `$L` symbol spellings. The actual COFF definitions remained in the
  owning section at the same offsets, and their original PE destinations
  matched exactly before the manifest spellings were rebound.
- `ff31849d`: `FrontFactoryConstructView::SetSideEnemyIndicatorPosition`
  directly indexes the real `markerVms123F4` VM array and `g_GameManager.sides`
  array. This removes two byte-pointer stride helpers while preserving the
  original unsigned side index. The focused source replay passed the complete
  213-byte owner and 10 relocation fields.
- `d66e8533`: `FrontSide::OnDraw` accesses the one-off `vm3CBC` and `vm59C8`
  objects through their actual typed `AnmVm` members and passes their native
  VM addresses to the draw call.
- `2f753505`: the one-off `vm7430` HUD draw now reads its `pos2` values through
  the real VM member and passes `&frontSide->vm7430`; its existing `Float3`
  position cursor remains for the coordinate writes.
- `1c403728`: removes the obsolete `cursor` assignments before those two
  direct one-off VM accesses.

The maintained `FrontSide::OnDraw` source still compiles to its full 3,094-byte
owner and all 209 relocation fields. Its focused changed-source replay passed
1 unit / 3,094 bytes / 209 relocations. The two other changed carriers passed
1 unit / 213 bytes / 10 relocations (`FrontEnemyIndicator`) and 10 units /
4,206 bytes / 264 relocations (`AsciiManagerMenu`). No full cold replay was
run. The canonical authored exact count remains 1,026.

### Rejected probes from the same continuation

- A wholesale `Float3 *` to `AnmVm *` cursor conversion in `FrontSide::OnDraw`
  expanded the owner to 3,127 bytes and 2,001 structural differences. A
  per-loop typed cursor variant and a forced-inline owner-recovery helper
  were also rejected; the latter was not inlined under the pinned `/Ob0`
  profile and changed the function extent. Directly rewriting the `vm7430.pos`
  stores also changed 21 bytes. Only the smaller, exact typed member changes
  above were retained.
- `CheckBulletCollision` was re-expressed with an inner region scan, a
  rectangle branch, a circle exit and a dispatcher label inside the radius
  branch. The best such candidate was 845/862 bytes with 584 differences;
  other nonvolatile/direct-dispatch variants ranged from 817 to 845 bytes
  with 584--688 differences. All were worse than the maintained 849/862-byte,
  539-difference candidate and were reverted. Preserve the native 0x34 frame,
  EDI=Player, ESI=slot, EBX=Bullet and X87 radius branch.
- `UpdateNetworkService` packet-ready/wait-return splitting and local receiver
  reuse did not improve the maintained 1,633/1,633-byte, 307-difference
  candidate. Several pointer-alias forms changed residual offsets without
  reducing their count; no frontier or source was changed.
- `UpdateChargeAttack` unsigned DWORD side locals and pointer/reference input
  variants produced 1,209/1,210 bytes with 717 differences. They were
  reverted to the 1,210-byte candidate with 183 differences.

These are bounded compiler experiments, not proof that the natural C++ search
space is exhausted. Do not add register hints, volatile aliases, handwritten
assembly, padding or synthetic bytes to force target instructions.


## 2026-11-20 ScreenEffect shake-envelope native record

`ScreenEffect`'s three-DWORD shake-envelope parameter window is now named
`ScreenShakeEnvelopeView`. Its `fadeInFrames`, `holdFrames` and
`fadeOutFrames` members occupy +0x00/+0x04/+0x08 in a 0x0C-byte record;
the enclosing union remains at `ScreenEffect` +0x1C and preserves the
existing `rawParameter0/1/2` aliases. Compile-time size and member-offset
assertions keep the native layout explicit. This type recovery left
`CalcShakeEnvelope` with 26 register-encoding differences at the time.
See the compiler context below for their resolution.

The header change passed the focused changed-source replay across 21 compiler
carriers: 100 exact units, 40,016 physical bytes and 2,487 relocation fields.
The pinned compiler regenerated 79 local COFF label spellings in Ending,
FrontMessage, ScreenEffect and TitleScreen carriers. Each was rebound only
after confirming the relocation's original PE destination and the candidate
COFF symbol's same-section owner-relative location; all 100 units then passed
the canonical comparator. No full cold replay was run, and the authored exact
count remains 1,026.


## 2026-11-20 Shake envelope compiler context

`ScreenEffect::CalcShakeEnvelope` matches the complete 336-byte owner at
`0x004227A0..0x004228EF` under pinned VC7.1 /O2 /Ob1 /Oy-. Its canonical
comparison replays all 20 fields, including three calls to the signed timer
comparison, two timer float calls, one timer increment and two bounded RNG
calls. The 1.0f and 2^32 float definitions also match their original bytes.
The next independent owner starts at `0x004228F0`.

The visible `ZunTimer::operator<` body lets C2 analyze the effects of those
three calls. The repository shares its real signed comparison as a
noinline COMDAT, following the existing greater-or-equal operator pattern.
This preserves the 20-byte callee, the original call sites and one linkable
definition. The original source annotation and translation-unit grouping
are unknown. With just the external declaration, the exact source body
has 101 differing bytes in the full ScreenEffect carrier.

The affected replay covers all 11 source carriers with reviewed calls to
the comparison, across 17 compiler jobs. All 158 exact units pass,
including 73,568 physical bytes and 3,414 relocation fields. Each emitted
timer comparison COMDAT also matches the original 20 bytes. The direct
charge-attack caller and the Enemy update frontier retain their recorded
residuals. A standalone caller under /O1 or /O2 /Os emits an 18-byte
comparison. All 13 current size-optimized carrier builds that include
ZunTimer.hpp have zero definitions of this COMDAT.

The fade-in arm and the shared hold/fade-out arm each scale their ratio
by the amplitude. VC7.1 merges them into the one original x87 multiply.
This phase structure changes receiver weights upstream of the global
allocator. With one source multiply in the RNG block, C2 gives more weight
to the effect receiver. With the split amplitude, it assigns the timer
first and uses the original ESI for the timer, EDI for the effect and EBX
for the endpoint. The allocator queues the signed weights at record
`+0x0C` in C2 `0x10782B26`. Explicitly promoting the returned fade-in
timer value to double recovers FIDIV at `+0x3E`. Reusing the unsigned
hold-plus-fade duration in the endpoint comparison and numerator recovers
the field reads at `+0x5A` and `+0x5D` and keeps them unqualified.

The final ratio interprets the same native fade-out word as an unsigned
count after the timer float call. A four-byte memcpy into an unsigned
local expresses that representation read without aliasing through a
different pointer type. VC7.1 expands it inline and emits the original
MOV and field-based FILD. In an otherwise identical cast-only control the
336-byte body matches through `+0xBD`. C2's local scratch selection for
the sign load at `+0x91` then leaves the first animation pointer load
starting at ECX. Both axes use ECX, EDX and EAX in the control. Both use
EAX, ECX and EDX in the target. Four relocation fields move because the
EAX load has the shorter encoding.

The exact representation-read control already has EAX assigned to the
sign load when C2 returns from global allocation at `0x1077C0FA`. The
cursor at `0x108A7D0C` advances at `0x10776893` exactly six times, once
per animation pointer load. A debugger build reproduces the 336 bytes,
all 20 relocation fields and both float definitions. An explicit signed
float conversion with a conditional 2^32 adjustment also avoids consuming
the scratch register, but shares one signed integer temporary and emits an
extra three-byte store for a stack-based FILD.

The two scratch selectors are independent. While investigating the
older split-amplitude controls, legalizing a proposed ADD advanced the
per-block selector at `0x10785179` without necessarily emitting a load.
The cursor at `0x108A7D0C` chooses the later virtual sign and switch
operands. At the final encoding, an earlier LEA can become ADD when
physical register assignment makes its destination share an address
component. Emitted instruction shape alone can therefore hide the
allocator decisions responsible for differences later in the same owner.

## 2026-11-20 Player-shot trail segment count

The 16-bit `PlayerShotCtorView` member at +0x46A is now named
`trailSegmentCount46A`, replacing the generic `field46A` spelling in the
shared Player view, shot update callback and shot spawn callback. This is
backed by independent consumers: `PlayerShotSpawnCallbackType1` initializes
it to 15; `PlayerShotUpdateCallbackType2` iterates this count and addresses
two 0x0C-byte history samples per segment; `PlayerShotDrawCallbackType1`
uses the same +0x46A count and draws `2 * trailSegmentCount46A` samples. The
existing 0x484-byte shot extent and +0x46A offset assertion remain intact.
This is a native field-meaning recovery, not new function-exactness credit.


## 2026-11-20 Player-shot draw callback uses the native owner

`PlayerShotDrawCallbackType1` no longer carries its own sparse
`PlayerShotDraw1ShotView` layout for the +0x1F3 alpha byte, +0x208 VM
position, +0x2B0 history array and +0x46A trail count. Its callback ABI tag
remains opaque and unchanged, while the body recovers the actual
`PlayerShotCtorView` and uses its embedded `PlayerAnmVmView`, constructed
trail-vector array and `trailSegmentCount46A`. The function still matches all
204 original bytes and six relocation fields under its native VC7.1 profile.
This unifies the draw callback with the same 0x484-byte shot allocation used
by spawn, update, damage and initialization code; it adds no exact-function
credit because the callback was already exact.

## 2026-11-20 Player-shot draw callback type 2 native owner

`PlayerShotDrawCallbackType2` now uses the same full `PlayerShotCtorView` as
its Type1 sibling instead of maintaining a duplicate sparse layout. The IDA
body confirms that it reads the first four +0x2B0 history vectors, writes the
embedded VM scale at +0x18/+0x1C, and interpolates the four BGRA bytes at
+0x1F0. Those members are now addressed through `PlayerShotCtorView::vm` and
`trailVectors2B0`; the original opaque callback parameter tag is retained so
the decorated ABI symbol is unchanged. The 293-byte function remains exact,
including all five relocations and three compiler literal definitions. This
is a native ownership/type recovery, not new exact-function credit.

## 2026-11-20 Player-shot spawn/update callbacks share native shot and SHT views

The remaining sparse shot layouts in spawn Type2/Type6 and update Type5/
Type6/Type7 have been replaced with the existing `PlayerShotCtorView`.
Spawn Type2 now writes its native +0x43C velocity and +0x450 angle through
that view. Spawn Type6 also uses the real `PlayerShtDescriptorView` for its
+0x00 fire interval and +0x18 speed, and copies the fixed-slot effect's
position into the native +0x2A4 shot position. Update Type5 addresses the
native velocity, +0x454 timer, +0x462 state, +0x470 mode and +0x472 flag;
update Type6/Type7 share the native velocity, speed and angle members.

The multi-callback spawn translation unit also dropped its duplicate sparse
SHT descriptor and now uses the real descriptor's +0x00 fire interval and
+0x14 angle in Type2/Type4/Type5. All original callback parameter tags remain
forward-declared and unchanged, preserving decorated ABI names. The seven
affected callback bodies remain exact: spawn Type2/4/5/6 are 68/44/72/119
bytes; update Type5/6/7 are 86/98/98 bytes. Their 15 relocation fields were
validated by the four changed-source scoped replays; no cold replay was run.
This is shared native C++ ownership recovery, not new exact-function credit.

## 2026-11-20 Remove duplicate PlayerShot initialization layouts

`PlayerShotInitializeView.hpp` no longer defines a second 0x484-byte shot
record or a second 0x38-byte descriptor record. Its two legacy
`PlayerShotInitShotView`/`PlayerShotInitDescriptorView` tags remain forward
declared for the original `SpawnShotWhenFrameMatches` decorated ABI, while
the actual allocations are the shared `PlayerShotCtorView` and
`PlayerShtDescriptorView`. The 37-byte frame-match method now reads the
native `fireInterval` and passes the native shot/descriptor to
`InitializeShot`. This also removes the redundant VM/timer/vector/descriptor
field maps from the header without changing any exported callback tag.

The header's changed-source replay covered seven compiler carriers and nine
exact functions: 1,025 physical bytes and 22 relocation fields, including the
spawn callback family, `InitializeShot`, and the frame-match leaf. All passed;
no full cold replay was run and no exactness ledger count changed.

## 2026-11-20 PlayerShot mode and update flag fields

The native `PlayerShotCtorView` words at +0x470/+0x472 are now named
`mode470` and `flag472`. The exact shot initializer clears `mode470`; exact
update callback Type5 tests it for mode 1 and sets `flag472` every update.
The adjacent byte at +0x473 remains unknown. Compile-time offset checks pin
all three members and preserve the 0x484-byte allocation. The shot
initializer and update callback remain byte-exact. A selected replay of 14
compiler carriers and 17 exact units passed 2,886 physical bytes and 68
relocation fields, including all exact shot spawn/draw/update consumers plus
the shot producer and frame-match leaf. No whole-product cold replay ran.

## 2026-11-20 Player-shot collision callback uses native Player owner

The Player byte at +0x30354, incremented by exact
`PlayerShotCollisionCallbackType1`, is now named `shotCollisionCount30354`
in the shared `PlayerLifecycleView`. The former sparse callback-only
0x30355-byte Player prefix and the unused position layout were removed; the
original opaque callback parameter tags remain forward declared so both
collision callback symbols keep their exact decorated names. The +0x3033C
prefix and +0x30355 tail still preserve the original `movementDirection30358`
offset. Both collision callbacks remain exact (11 and 8 bytes); their focused
carrier replay passed 19 bytes and zero relocations. This is a native Player
field recovery, not additional exact-function credit.

## 2026-11-20 Player-shot effect spawn callbacks use native Player owner

`PlayerShotSpawnCallbacks.cpp` no longer carries its own sparse Player prefix
or a duplicate `PlayerShotSpawnSideView`. Type4 and Type5 now follow the
native `PlayerLifecycleView::sideState` pointer to the real
`PlayerSideStateView`, then borrow the original effect-manager method ABI
through its callback-specific view. Type4 uses the native Player position at
+0x1B88; Type5 uses the native side-state for its fixed-slot effect and shot
initialization. The callback parameter tags remain opaque and unchanged.
Type2/Type4/Type5 remain exact at 68/44/72 bytes with all four relocations;
their selected carrier replay passed 184 physical bytes. Type6 also uses the
full native `PlayerLifecycleView` after its fixed-slot accessor and returned
record were switched to the native `EffectManager`/`Effect` types; its
119-byte callback remains exact with the original opaque Player ABI tag.

## 2026-11-20 Player-shot spawn callbacks use native Effect records

The fixed-slot effect returned by the callback-specific manager ABI is now
consumed through the shared native `Effect` record. Type5 reads
`Effect::active` at +0xC4; Type6 copies the native `Effect::position` at
+0x0C into the shot's +0x2A4 position. The callback-specific manager and
return-type tags remain forward declared because their decorated method
symbols are distinct original aliases at the same fixed-slot accessor entry;
only the duplicate sparse Effect layouts were removed. Spawn Type2/4/5/6 all
remain exact (68/44/72/119 bytes), and the two changed-source carriers passed
303 physical bytes and six relocation fields.

## 2026-11-20 Effect fixed-slot accessor uses native Effect storage

EffectFixedSlotManagerView::GetFixedSlotEffect retains its original
0x00445560 decorated ABI and 26-byte body, but its +0x30 pool pointer is now
typed as the real Effect * rather than a pointer to a synthetic 0xD8-byte
record. The return tag remains forward-declared solely to preserve the
original symbol. Compile-time checks tie the native Effect size (0xD8),
position (+0x0C), active byte (+0xC4), and EffectManager::effects pool
(+0x30) to the accessor's physical arithmetic. The function remains exact,
and the selected carrier replay passed its 26-byte body with no relocations.

## 2026-11-20 Fixed-slot effect accessor uses native Effect pool

`EffectFixedSlotManagerView::GetFixedSlotEffect` now types its +0x30 pool as
`Effect *` and asserts the native 0xD8 record, +0x0C position and +0xC4 active
byte. It still returns the original opaque `EffectFixedSlotRecordView *`
ABI tag and computes `&effects[256 + slot]`, preserving all 26 bytes.
The callback-specific fixed-slot manager views in shot spawn Type2/4/5/6 now
also type their +0x30 pool pointer as `Effect *` while retaining the distinct
original callback method/return tags used by their relocations. All five
functions remain exact (329 physical bytes, six relocation fields) across
three changed-source carriers.

## 2026-11-20 Effect follow-player callback uses native Player owners

`EffectFollowPlayerUpdate` no longer carries private sparse Player and Side
records. Its `sideState` pointer is the shared `PlayerSideStateView`
(`AsciiGameManagerSideView`) and its +0x04 `player04` pointer is the native
`PlayerLifecycleView`. The followed position is the real Player +0x1B88
`position1B88` record; compile-time checks bind both offsets and the matching
three-float storage size. The callback remains byte-exact at 38/38 bytes with
no relocations. Its changed-source replay passed the one affected exact unit;
no cold replay ran.

## 2026-11-20 Effect callbacks share the native side-state record

The 38-byte `EffectFollowPlayerUpdate` callback now follows its owner through
`Effect::manager->sideState` as the native `PlayerSideStateView`, then reads
the real `PlayerLifecycleView::position1B88`. Its two private sparse owner
records are gone; offset assertions pin side +0x04 and Player +0x1B88, while
the callback remains 38/38 bytes exact.

`EffectUpdateCallbackType1Type2` also no longer carries a private 0x24-byte
side-character prefix. The random-axes bullet speed adjustment reads
`g_GameManager.sides[...].characterIndex20` from the canonical
`AsciiGameManagerSideView` at +0x20. Its 775-byte body remains exact with all
33 relocation fields. Both changed-source scoped replays and CI passed; no
full cold replay ran.


## 2026-11-20 ECL interpolation helpers use the shared Enemy owner

EclHelpers.cpp no longer defines a private EclHelperEnemyView spanning
the Enemy's ECL contexts, operand banks, movement state, bullet descriptor,
flags, animation selectors and child-block table. Those offsets are now
asserted directly against the canonical EnemyView in EnemyManager.hpp.
The unused HelperView reinterpret-cast adapter is removed; interpolation
callbacks already receive the native EnemyView pointer and resolve operands
through its methods. The two original interpolators remain exact: linear is
74/74 bytes with three relocation fields; Hermite is 188/188 bytes with nine
relocation fields. Changed-source scoped replay passed both functions (262
physical bytes, 12 relocations). This removes a duplicate 67-line partial
Enemy layout without changing either callback's ABI or exactness credit.


## 2026-11-20 Effect +0xC9 is the shared draw-enable flag

The Effect byte at +0xC9 is now named drawFlag0C9 in the canonical 0xD8
Effect owner. The evidence spans the complete ring/fan/strip callback family:
initializers and update callbacks set it, draw callbacks test it, and the
shrinking Type32 update clears it when its radius reaches zero. The VM-backed
Effect update also sets the same byte. The +0xC9 offset is compile-time
asserted between alternateDrawGroup at +0xC8 and backgroundDrawGroup at
+0xCA. All 12 affected Effect source files now use this single field name.

The changed EclManager translation unit regenerated compiler-private local
COFF label spellings in CompareOperands, EnemyPostEclUpdateMovement and
RunEcl. The 202 changed manifest bindings were updated only after checking
each actual label belongs to its function's COFF section and that its
owner-relative address equals the independently reviewed target address.
No relocation was ignored or masked differently. Selected replay of
EffectManager.cpp, EclManager.cpp, EffectDrawStrips.cpp and
EffectPlayerStripFamily.cpp passed five compiler carriers, 104 exact units,
49,253 physical bytes and 2,148 relocation fields. Basic CI passed. No
whole-product cold replay ran; exact-function credit remains unchanged.


## 2026-11-20 Boss frontier follow-up: cursor ownership and draw-head registers

The current EnemyManager::OnUpdate candidate was rebuilt after the Effect
header work: 3,900/3,900 bytes with only the two recorded draw-group register
differences at +0x19A/+0x1A1 (MOVZX into ECX rather than EAX, then LEA from
ECX rather than EAX). Branch-local slot references, pointer aliases, an
array reference and an explicit unsigned index were each compiled; all
retained the same two-byte residual. No register hints or volatile fields
were added.

CheckBulletCollision remains at its maintained 849/862-byte candidate with
539 differences and REGRESSED status against the strict 842-byte ratchet.
Two new cursor-home experiments were compiled and reverted: removing the
redundant entry cursor-home store produced 837 bytes/706 differences with
105 newly differing offsets; making the scratch-union cursor the sole scan
cursor produced 830 bytes/711 differences with 113 newly differing offsets.
Neither was promoted and the frontier manifest was not changed.

The three selected boss frontiers were rechecked independently. UpdateNetworkService
remains a 1,633-byte, 307-difference candidate and REGRESSED against the
1,629-byte ratchet. No full cold replay ran; no source experiment from this
follow-up remains in the worktree.

## 2026-11-20 Player collision region has one shared native record

The collision-region field at +0x40 is now named `collisionDelay40` in the
canonical `PlayerCollisionRegionCreateRegionView`. The name is supported by
its allocator argument/write and by both independent consumers: bullet-region
collision filtering and shot-to-enemy damage filtering. `PlayerDamage.cpp`,
`PlayerCollisionHelpers.cpp` and `PlayerCollisionRegionCreate.cpp` now use
that same member.

`PlayerShortLeaves.cpp` no longer declares a second sparse 0x44-byte
`PlayerCollisionRegionRuntimeView` with an opaque 0x3C-byte prefix and a
repeated delay field. The five-byte Deactivate leaf is declared and
defined directly on `PlayerCollisionRegionCreateRegionView`; the exact-unit manifest
uses that canonical member symbol. The shared owner asserts `active3C` at
+0x3C and `collisionDelay40` at +0x40.

Selected source replay passed 19 exact units across six compiler jobs: 2,799
physical bytes and 68 relocation fields. This includes all eight collision
region create functions, `CalcDamageToEnemy`, the three other exact collision
helpers and the four short leaves. `CheckBulletCollision` itself is a
non-exact boss frontier and was not credited by that replay. Its maintained
source remains 849/862 bytes with 539 differences; no frontier was rebaselined.
No whole-product cold replay ran.

## 2026-11-20 CheckBulletCollision boss CFG and register frontier

The read-only IDA reconstruction of the 0x362-byte sub_41DFF0 owner
confirms the native 0x34-byte frame and entry roles EDI=Player, ESI=active
region slot, EBX=Bullet. The scan first filters region kinds 1/2/3 and
nonpositive collisionDelay40. Zero-radius regions take the rotated or
axis-aligned rectangle paths; a nonzero radius exits the inner filter loop
to the circle test. All successful geometry paths join the shared
direct-hit/bullet-cancel dispatcher. Rectangle misses and circle misses
advance the slot and retry. Cancellation only exits to the reward path when
the bullet's sprite discriminator at +0x10C0 is zero.

Several natural-C++ layouts were rebuilt and byte-compared in this
continuation: explicit circle/dispatcher/rectangle-tail labels, a nested
filter loop with an outer radius/rectangle decision, direct-slot versus
pointer-snapshot dispatch, and scratch/field-based rectangle comparisons.
The best new nested-loop candidate was 845/862 bytes with 581 differences
(84 new, 208 closed relative to the current 539-difference source); other
variants were worse. Every probe was reverted. Keep the maintained
849/862-byte, 539-difference source and do not rebaseline it. No assembly,
padding, or synthetic bytes were used.

EnemyManager::OnUpdate was also rechecked at 3,900/3,900 bytes. The only
remaining differences are the two destination-register choices at +0x19C
and +0x1A3 (MOVZX/LEA use ECX rather than EAX). Pointer-arithmetic,
index-local, inline slot/reference helper, declaration-order and previous-head
temporary experiments did not change those two bytes; all were reverted.
Keep its current exact-size source and two-byte residual. Neither boss
received an exactness claim from these experiments.

## 2026-11-20 Player collision updater now uses the canonical region owner

UpdatePlayerCollisionRegions no longer declares a second sparse 0x44-byte
PlayerCollisionRegionRuntimeView or a parallel PlayerCollisionRuntimeView
prefix-and-array overlay. It now uses the actual PlayerLifecycleView members:
collisionRegionB0BC, activeB100, releasedB908, collisionRegionActiveCountC110
and collisionRegionReleasedCountC114. Each slot and the embedded region is
typed as PlayerCollisionRegionCreateRegionView, the same complete record
used by allocation, bullet collision and damage.

The updater's old +0x08/+0x0C/+0x10/+0x14/+0x18/+0x1C values now resolve to
radius08/radiusGrowth0C/halfWidth10/halfHeight14/widthGrowth18/heightGrowth1C.
The +0x24 lifetime, +0x3C active byte and +0x40 collision delay are also
shared fields. The active/released arrays and counts are direct, offset-asserted
PlayerLifecycleView members; compaction, signed lifetime expiry, and the
embedded-region early return retain the original control flow.

The 11 compiler-private local COFF labels in PlayerLifecycleView::UpdateBeforeState
were regenerated by rebuilding PlayerRuntime.cpp. Each changed spelling was
verified as an owner-local symbol with the exact same owner-relative destination;
only those manifest names changed. Selected replay passed 21 exact units across
five compiler jobs, 5,391 physical bytes and 288 relocation fields, including
the 273-byte updater. The updater's standalone comparator also returned exact.
No full cold replay ran.

## 2026-11-20 Player bomb input uses the canonical ReplayInputState

UpdatePlayerBombState no longer declares a duplicate 0x8E-byte
PlayerInputRuntimeView or a parallel g_PlayerInputStates array identity.
The input source is the already shared g_ReplayInputStates[sideIndex], and
the check is ReplayInputState::IsHeld(2). This is the same target function
address 0x00415CB0 previously named PlayerInputRuntimeView::Check; the
unsigned-short mask is a bit mask (bomb bit 0x0002), not a key-array index.
Both the global relocation and method relocation now use the canonical
ReplayInputState symbols.

The PlayerRuntime.cpp rebuild regenerated 11 owner-local compiler labels in
PlayerLifecycleView::UpdateBeforeState. Every replacement was checked to
retain its original owner-relative destination before changing the manifest.
Selected replay of all five PlayerRuntime.cpp carriers passed 21 exact units,
5,391 physical bytes and 288 relocation fields. This includes the 335-byte
UpdatePlayerBombState and 273-byte UpdatePlayerCollisionRegions. No full cold
replay ran.

## 2026-11-20 Player bomb state now calls the canonical EnemyManager and CardAttack owners

UpdatePlayerBombState no longer declares PlayerBombTargetRuntimeView or
PlayerBombOwnerRuntimeView, nor does it reinterpret the two side-state slots.
The +0x10 target is the already typed EnemyManagerView and its exact
FindActiveEnemyBySideCategory(3) method; the +0x14 owner is the existing
CardAttack and its exact StartAttack(row, class, value, path) method. The
same pair of methods is used directly by UpdateChargeAttack, PlayerDeath
and PlayerOwnerStateReward. The target query returns EnemyView*, so the
bomb's previous boolean test is now the explicit null test.

The four Configure call relocations and one CheckMode relocation in the
UpdatePlayerBombState exact unit now bind to the canonical existing method
symbols at the same target addresses. No wrapper methods or new leaf bodies
were added. The changed PlayerRuntime.cpp carrier replay passed all 21 exact
units across five compiler profiles/jobs: 5,391 bytes and 288 relocations.
The 335-byte UpdatePlayerBombState comparator is exact.

## 2026-11-20 Player collision query now uses the canonical ZunTimer comparison

PlayerCollisionQuery.cpp no longer defines a private 0x0C PlayerCollisionTimerView
or casts PlayerLifecycleView::timer1B74 through that duplicate. The field is
already PlayerTimerCtorView, which is an alias of the complete ZunTimer record.
FindCollision now expresses the native comparison as player->timer1B74 > 0.

The call target at 0x00403DE0 is the already exact ZunTimer::operator> leaf
(20 bytes, symbol ??OZunTimer@@QAEIH@Z). The source call's actual COFF relocation
was checked before updating the caller manifest; the duplicate
PlayerCollisionTimerView::IsAfter name is no longer needed. Selected replay of
PlayerCollisionQuery.cpp passed both exact functions in its carrier:
FindCollision 602/602 and CalcItemBoxCollision 121/121, 723 bytes total and
nine relocations. No full cold replay ran.

## 2026-11-20 Player state-4 timers now use the canonical ZunTimer getter

PlayerRuntime.cpp no longer declares PlayerState4TimerCurrentView or
reinterpret-casts Player-owned timer303C8 objects to that duplicate one-method
view. All six GetCurrent call sites now use the native ZunTimer conversion
operator (static_cast<int>), and the three exact-unit manifests
PlayerCheckState4, PlayerTransitionSpecial and PlayerUpdateCommon bind their
six call relocations to the already exact ZunTimer::operator int symbol
??BZunTimer@@QAEHXZ at 0x00435F00.

The PlayerRuntime.cpp selected replay rebuilt all five compiler carriers and
passed 21 exact units: 5,391 physical bytes and 288 relocation fields. The
eleven regenerated UpdateBeforeState local COFF labels were rebound by exact
relocation offset only after verifying each owner-local symbol retained its
original owner-relative address. No full cold replay ran.

## 2026-11-20 Player runtime now uses the canonical FrontSide owner

PlayerRuntime.cpp no longer declares PlayerFrontSideRuntimeView. The three
Player runtime call sites now use the real FrontSide pointer already owned by
PlayerSideStateView::frontSide18: UpdateMeterHundreds, ClearState and
SetPatternTiming. The former SetTimingValue wrapper was especially misleading:
its target is the existing int-returning FrontSide::SetPatternTiming method
at 0x0041A3E0, not a distinct void method.

The three caller relocations now use the exact FrontSide method symbols.
Selected replay of PlayerRuntime.cpp passed all 21 exact units across five
compiler carriers, 5,391 physical bytes and 288 relocation fields. The eleven
regenerated UpdateBeforeState local labels were checked by owner-relative
address before rebinding. No full cold replay ran.

## 2026-11-20 Player timing metric now uses the canonical ZunTimer owner

PlayerRuntime.cpp no longer declares PlayerMetricRuntimeView or reinterprets
the opponent's header24.timer5C. PlayerHeaderStateView already declares that
exact +0x5C member as ZunTimer. Player::OnUpdate now obtains a ZunTimer*
from the real member and reads it through the native int conversion operator.
The call relocation at +0x18A now binds to the canonical
ZunTimer::operator int symbol at 0x00435F00.

The first direct member-expression spelling changed three caller bytes because
VC7.1 selected a different address-calculation register sequence. Keeping the
natural ZunTimer* local restored the complete 522-byte Player::OnUpdate body
and all relocations. Selected replay passed 21 exact PlayerRuntime units over
five carriers, 5,391 bytes and 288 relocation fields. Eleven local COFF labels
were rebound only after their owner-relative destinations were verified.
No full cold replay ran.

## 2026-11-20 Player respawn blending uses the canonical PlayerAnmVmView

The sparse PlayerAnmVmBlendModeView (1F8-byte opaque prefix plus a flags word)
and the separate PlayerRespawnBlendView wrapper described the same Player
main VM. The unused PlayerRespawnVmView layout was also dead. The two blend
operations now belong to PlayerAnmVmView, whose complete 0x2A4-byte storage
inherits the already asserted AnmVm flagsWord at +0x1F8. PlayerTransitionSpecial
calls player->mainVm directly; the leaf definitions update flagsWord on that
canonical VM owner.

The two exact leaf symbols and the two PlayerTransitionSpecial call
relocations now use PlayerAnmVmView. Both leaves remain exact (19 and 8 bytes),
and the caller remains exact (293 bytes). Selected replay of PlayerRuntime.cpp
and PlayerShortLeaves.cpp passed 25 exact units across six compiler carriers,
5,442 physical bytes and 289 relocation fields. The eleven regenerated
UpdateBeforeState local labels were checked against unchanged owner-relative
destinations. No full cold replay ran.

## 2026-11-20 Player runtime uses the canonical GameManager owner

PlayerRuntime.cpp no longer declares PlayerGameManagerRuntimeView or a
second g_PlayerGameManagerRuntime global at the same address as g_GameManager.
All Player draw/update paths now use the canonical AsciiGameManagerView
object for popup transforms and playfield checks. The old CheckTimingState
wrapper is the already exact IsGameMode1 method (gameMode +0x118 == 1), and
the old suppressPlayerDraw13D byte is the existing GameManager +0x13D byte.
That semantic alias is now explicit in AsciiGameManagerView and has an offset
assertion.

All PlayerRuntime exact-unit relocations were rebound from the duplicate
global/method names to the canonical g_GameManager / AsciiGameManagerView
symbols at unchanged original addresses. Selected replay passed all 21 exact
PlayerRuntime units across five compiler carriers, 5,391 bytes and 288
relocation fields. Eleven regenerated UpdateBeforeState local labels were
checked against their unchanged owner-relative destinations. No full cold
replay ran.

## 2026-11-20 Player tail records now share the canonical constructed type

PlayerTailStateCtorView was previously an opaque 0x2C4-byte storage block,
while PlayerRuntime.cpp overlaid it with two separate views:
PlayerRewardTailStateView (type/position/velocity/active/ANM at +0x00..+0x20)
and PlayerTailDrawStateView (a pointer shifted by +4, then position/active/ANM
at +0x00/+0x18/+0x1C). Those are the same physical record viewed from
different starting addresses.

PlayerTailStateCtorView now owns the named fields directly:
type00, position04, velocity10, active1C and vm20. Size and every field offset
are asserted, with the existing 0x2C4 constructor ABI retained. UpdateBeforeState,
SpawnRewardTailState and DrawTailStates all use this same typed array; the
draw path no longer creates a second pointer view at storage[4].

Selected PlayerRuntime replay passed all 21 exact units across five compiler
carriers, 5,391 physical bytes and 288 relocation fields. The eleven regenerated
UpdateBeforeState local COFF labels were rebound only after verifying the
unchanged owner-relative target addresses. No full cold replay ran.

## RunEcl register coupling across handlers

Commit `5fc16212` recovered the final 30 instruction bytes of
`EclManager::RunEcl` at `0x004086C0` with pinned VC7.1
`/O2 /Ob1 /Oy-`. Opcode 4's jump and opcode 155's timeout flag both needed
ECX where the old build used EAX. Experiments limited to those two
handlers kept the residual or changed other instructions.

Temporarily omitting individual handlers in scratch showed that the
register choices in opcodes 155 and 156 moved together when opcodes 134,
150, 151 or 152 were omitted. Opcode 134 shared a tail with opcode 153.
Restoring direct timer installs, integer stores and jump statements
removed the hand-written tail merges and snapshots. VC7.1 still merged
the original tails. The plain opcode 4 assignment and opcode 155
bitfield store then reproduced the original instructions.

The canonical `ecl-run-ecl` unit covers 14,792 credited bytes and
15,564 physical bytes, with both switch tables and 598 relocation
fields. Rebuilt private labels were accepted after their same-section
definitions, recorded destinations and PE operands agreed.

## 2026-11-20 CirnoLab cross-check of the exact-codegen Oracle

The cold replay of 8286742c rebuilt all 422 exact compiler carriers.
CirnoLab's separate COFF parser and a PE reader reproduced all 1,026
physical extents, 283,400 bytes and 14,052 relocation fields. The audit
checked every actual relocation offset and type and independently verified
1,037 owner-local definition addresses. The canonical comparator also
passed all 1,026 units. A separate cold replay of 5b6491eb's changed
Player carriers and their source-local include consumers passed all 276
affected exact units across 91 compiler jobs.

Cirno-Labs/th09 at afebc8b and TH09's pinned executable share SHA-1
89c8fdef84d25b2ee8bbd2026f8a343d7ef57c03. Every one of the 1,026 exact
ledger starts appears in CirnoLab's symbols.csv. All 18 size disagreements
are one to three alignment bytes before inline tables that our physical
comparison already includes. None of CirnoLab's starts falls inside a
different exact body's physical range. All 212 manifest import operands
with __imp_ symbols agree with the original PE IAT. Across CirnoLab's 80
table extents, 309 original pointer slots name canonical exact entries.

Match by original address before comparing nominal type names. Our
MidiTimer special members at 0x0043D0A0 and 0x0043D320 are CirnoLab's
MmTimer members. Its MidiTimer special members at 0x0042F3B0 and 0x0042F3F0
are distinct target entries whose origin remains unresolved in our ledger.

CirnoLab's build reports differences in Stage::DrawLayer, EnemyScripts::RunEnemy,
GameManager::LoadingThread and DrawEffectType29. Our corresponding cold
COFF contributions reproduce the original 3,274, 15,564, 1,689 and 2,220
physical bytes, respectively. RunEcl includes both jump tables. Its
compiler-private label names changed at 195 relocation fields, with all
owner-relative definitions and original operands unchanged. The comparator
now accepts a private label's changed numeric suffix only when its actual
COFF definition resolves inside the same owner at the reviewed address.
Keep exact symbol spelling for externals and cross-section definitions.
Anonymous C++ namespace names can depend on the source path, so use the
original working-tree path for those carriers when reproducing the proof.

The audit hardened two independent inputs. For an aux-less COFF function,
the extent comes from the next function or the end of its code section.
The tracking validator now checks that the unit size equals the credited
size and that the physical extent stops before the next catalog entry.
The comparator also checks explicit manifest addends. The 1,292 recorded
addends in credited units agree with the original operands and their
reviewed symbol bases.

CirnoLab's complete EffectManager::AddedCallback reproduces all 51 bytes
and four relocations, including the referenced string payload, with its
documented per-file flags and the pinned RTM compiler. A control using the
same source and flags with just the header-inline GetAnm body replaced by
its declaration still emits a 51-byte callback, but has 25 structural
differences and five relocation fields. This offers a concrete codegen
reason to recover the original visibility of the accessor and preserve the
complete sibling carrier before experimenting with register ordering.

The frontier counts measure bytes after masking candidate COFF fields.
BeginPlaybackStage has 13 structural differences and 19 after relocation
replay because two GameManager operands moved. UpdateMovementAndOptions
has four structural differences and at least eight after replaying its
shifted configuration operand. The Effect callback had four structural
differences and eight after replaying its shifted manager operand before the
accessor visibility recovery below. At the time of this audit,
EnemyManager::OnUpdate and SaveReplay each had two structural differences.
CirnoLab's table contracts identify the three name tables and their extents.

Native link identity remains a separate open question for exact callers.
Nine g_GameManager COFF names account for 1,031 fields in the exact ledger
at the same original address. An ExAttack-specific name contributes six
more fields at that address. No inspected exact carrier defines any of
those GameManager symbols. SupervisorAudio.cpp declares the global with
an anonymous-namespace view type, so its linker symbol cannot resolve to
an AsciiGameManagerView definition. The pinned linker reports this
unresolved reference in a partial SupervisorAudio link. Its BGM track
table is also an external declaration. CirnoLab provides a single
GameManager owner and a 19-by-16-byte BGM table at the corresponding
original addresses. Consolidate those providers before treating a
per-function operand replay as evidence of native linkage.

## 2026-11-20 comparison with TH08 data and linkage checks

TH08 separates configured COFF code from initialized-data contracts and
normal-PE/map evidence. At TH08 checkout 37e0e198, all four declared data
contracts and the final Windows i386 owner verifier passed against the
retained local build.
The latter checks actual linked member offsets, callback slots and repaired
REL32 sites. The TH08 RT-002/005/006 cases show how a wrong field addend, a
separately allocated alias or a wrong callee can survive original-address
operand replay. Use TH09 target evidence to review each such identity.

The TH09 ECL interpolation callback table at 0x004A0AF4 has eight reviewed
DWORDs. The first seven select 0x00407450 and the last selects 0x004074A0.
The pinned EclHelpers COFF defines a complete 32-byte contribution with
eight DIR32 relocations and zero addends. Replacing the final relocation's
symbol index with the Linear callback in a private object copy formerly
left the installer and both helper bodies exact. The comparison of each
defining helper now rejects that object at table offset 0x1C. A fresh
EclHelpers replay checks both bodies and the whole table. The installer
still reports its separate 255-byte caller proof.

TH09's existing Effect and ExAttack coverage report reads callback prefixes
from the original. It checks catalog coverage at those targets, while the
new ECL check reads a real initializer and its actual relocation identities.
Replay difficulty, mode and character names are still external declarations
in TH09. Other named initialized data needs reviewed extents and defining
objects before it can receive the same check. The current exact REL32 ledger
has no repeated decorated call symbol assigned to conflicting bases.

TH08 exposes an explicit whole-ledger cold verifier and uses paired
PE/map checks, relocated link-order experiments and retail/native replay
traces for its product. TH09's regular exact replay is explicitly scoped.
Its PE build graph is open, so final linked call targets, alias storage
identity, moved-layout behavior and retail/native replay remain product
and runtime work.

## 2026-11-20 cross-check against TH06, TH07, TH08, and TH095

TH06's linked replay binds the retained objects, source inputs, compiler,
image and map before comparing linked bytes at original addresses. Its
boundary audit reviews inter-function gaps separately from exact bytes.
The local target and ledger were readable, but a linked replay refused the
retained receipt because its pinned compiler path was missing.

TH07 derives procedure extents from the PE-bound PDB and compares its
faithful link with a metadata link. It resolves imports and compares linked
globals and compiler exception graphs alongside function bytes. Read-only
replay of the retained maintained product matched all 694 named functions.
Its exception audit paired 18 native descriptors and flagged one unpaired
record. TH08's four data contracts passed against its retained build and map.
TH095 main's build graph validated with 88 sources and two profiles. Its
documented production link and runtime checkpoints are separate from exact
replay, and its ownership audit covers aliases that selected the same
original address but occupied different native storage.

TH09 previously accepted a private TitleScreen COFF copy whose setup-thread
relocation selected FindClose while its manifest gave that symbol Sleep's
original IAT slot. The substituted address reproduced the caller bytes.
Canonical comparison now checks each import's name and independent PE slot,
DIR32 type and zero addend. All 212 credited import fields agree with the
original IAT and direct operands. The fresh TitleScreen replay passed 41
units; the changed COFF and manifest pair now fails at caller offset 0x0F.

The exception check exposed a separate scope. The old Background::Create
comparison passed when a private COFF copy changed the cleanup funclet's
stack read from EBP-0x10 to EBP-0x14. A bounded replay of four handler roots
in three unmodified objects compared the full handler, FuncInfo, unwind map
and cleanup bytes using the original handler roots and verified CRT entries.
Background::Create, Ending::RegisterChain and the two Title handlers passed.
The subsequent unwind check expanded this proof to all credited handlers.

## 2026-11-20 TH09 C++ unwind contributions

Canonical VC7.1 comparisons now verify the complete EH contributions for
each credited caller with a `__ehhandler$` relocation. The caller's handler
offset and the original FuncInfo and unwind map fix the reviewed extents.
Both COFF sections must declare the caller as their associative COMDAT
owner. Each unwind action must point to the corresponding section-relative
cleanup entry. The comparison replays all nested fields and compares all
code and metadata bytes. The frame-handler and scalar-delete calls bind to
the pinned CRT entries. The ChainElem and TextHelper cleanup calls bind to
their unique canonical function entries.

A cold build of 18 objects passed all 21 credited EH callers and their 23
cleanup states. SupervisorStartup is the three-state case. Several Title
and text-rendering callers share an original handler address; the check
verifies each emitted contribution. Those proofs cover 457 code bytes,
772 metadata bytes and 109 nested relocation fields, counted per caller.
The authored ledger keeps its existing code extents.

The original Background::Create cleanup reads EBP-0x10. Changing the
private COFF copy to EBP-0x14 now fails at 0x0048D212. Redirecting the
unwind action to the handler entry also fails, while both mutations
leave the caller's code and relocation set intact.

## 2026-11-20 EffectManager::AddedCallback visibility

Pinned VC7.1 with `/O2 /Ob0` reproduces the Japanese v1.50a callback at
`0x0040D1C0` when the real inline `AnmManager::GetAnm` definition is
visible. The comparison covers all 51 bytes, both calls, the manager
global and the ten-byte `etama.anm` literal. The visible inline accessor
lets VC7.1 reuse the manager across calls while retaining a call to
`GetAnm`. The same object emits the exact 20-byte accessor at
`0x00401660`.

With just the accessor declaration, the direct callback reloads the manager
and emits five relocation fields. A visible non-inline definition reuses
ECX across calls and emits a 45-byte callback. With the inline definition
and `/Ob1`, the compiler expands the accessor and emits a 47-byte
callback. These controls isolate declaration, definition and inlining
effects independently of the callback's local variable spellings.

The Effect callback has a dedicated `/Ob0` carrier. The other EffectManager
units compare under `/Ob1`. All six existing callbacks that call the same
accessor compare exactly under `/Ob0`, as do the other fourteen FrontSide
units. Player's callback uses its constructor carrier because
`PlayerUpdateCommon` requires its `/Ob1` carrier. All emitted `GetAnm`
definitions in the selected objects are identical 20-byte COMDATs. These
comparisons establish function-level compiler contexts. The retail
translation-unit partition remains open.

## 2026-11-20 SaveReplay payload copy and USER tables

The shared inline payload copy reproduces the full Japanese v1.50a
`ReplayManagerView::SaveReplay` function at `0x00420C60` under the
existing VC7.1 `/O2 /Ob1` profile. A file offset includes the 0xC0-byte
uncompressed header. Stage headers, input chunks and FPS chunks use the
same conversion to an offset within the compression buffer. Inlining
this copy makes VC7.1 load the buffer base into EDI for the eight-DWORD
stage-header copy at +0xB6/+0xB9. This closes the two remaining bytes.

Direct aggregate assignment and direct intrinsic `memcpy` both load the
base into ECX. The enclosing stack frame and remaining code agree in
both controls. Moving the real replay constructors into the same
translation unit also retains the two differences. CirnoLab's replay
carrier has the same ECX copy setup with and without its precompiled
header. The shared copy accounts for the observed difference in the
maintained source and compiler profile.

The canonical comparison covers 1,238 bytes and 53 code relocations.
The object defines the USER format strings and the original 108-byte
block of character, mode and difficulty tables at `0x004A1C78`.
The oracle checks all 27 table relocations and the pointed-to strings,
including the shared Extra literal. A swapped pair of table targets, a
nonzero table addend and a changed format byte each fail comparison with
the caller's code and relocation set intact.

## 2026-11-20 Enemy update schedule storage

The original Enemy update reads a byte schedule at `0x004A7C88`.
The exact AddedCallback and gameplay reset routines fill those same 256
slots. All 256 original initial bytes are zero. The rebuilt ECL object
defines one 256-byte zero-fill contribution under the readers' and writers'
shared symbol. Update's two DIR32 fields at +0xB5 and +0xC5 select it with
zero addends. Cold replay of the ECL and both writer sources passed their
91 existing exact units.

After replaying all 97 code and table relocations and checking the five
literal references, the rebuilt 3,900-byte owner differs only at +0x19C
and +0x1A3. The four switch destinations and both three-byte folded callback
definitions agree with the original.

A reference to the side timer before the pool loop restores EAX for the
draw-group index and changes ten nearby register-dependent bytes.
CirnoLab's carrier retains its original EAX instructions with a shared
append tail and with its nearby scheduler and motion callees hidden.
Applying the target-backed bullet descriptor field order and speed to
that carrier leaves seven structural differences in the secondary ANM VM
loop, which allocates the cursor and count to the opposite registers.
Both carriers retain their draw-group register choice under their
same-source compiler profile controls.

Moving the real `ZunTimer::HasTickedEvery` definition at `0x00404920` into
the Enemy compilation context changes code emitted more than 200 bytes
before its call. VC7.1 emits the original EAX load and indexed address in
the early draw path. With all 97 relocation fields replayed, the 3,900-byte
candidate differs at four force-death vector argument bytes and 41 bullet
descriptor bytes. Removing the volatile qualifier on the effect receiver
produces the same result in this context. The visible constructor, effect
spawn function and Enemy cleanup definition each retain the old two-byte
residual when tested independently.

In temporary source copies, removing the periodic timer/effect check while
keeping the threshold and bullet spawn also restores the early EAX choice.
Removing the bullet descriptor and its spawn call while keeping the timer
and effect calls leaves the early ECX choice. Removing movement or the
secondary animation loop independently preserves the two carriers' original
register choices. These controls point to the timer/effect context.

In the timer-visible build, assigning `descriptor.aimMode = 0` before
`descriptor.bulletType = 1` restores the full bullet descriptor region.
The 3,900-byte owner then has four differences at +0x1BA, +0x1BF,
+0x1C1 and +0x1C3. All 97 fields and 69 ordered calls agree. The
force-death vector call still passes its offset pointer in ECX and
forms the return-storage address in EDX. The original uses EDX and
EAX respectively. Moving the timer definition next to the greater-than
operator or after the Enemy body gives the same four differences.
An ordinary effect receiver also gives the same result in this control.

The timer body matches all 33 bytes in that object. The four exact
co-visible ExAttack timer callers also pass complete comparison.
A standalone Enemy carrier that leaves RunEcl external reproduces
the two original differences with the opaque timer and the four
vector differences with the visible timer and changed descriptor order.

In the standalone pair, the side-state load at +0x16F always emits
`mov eax,[esi+0x320]`. With the timer opaque, its local allocation
advances the scratch-register list to ECX. The following draw index
takes ECX and leaves EDX for the force-death offset pointer. With the
timer visible, global allocation assigns EAX to the side-state load
before the scratch pass. The draw index takes EAX and leaves ECX for
the offset pointer. The compiler updates the list cursor at
`0x10776893`. This accounts for the differing argument registers
with the same vector operations at global allocator exit.
Taking a const reference to the side-state pointer slot restores
the original vector registers in the timer-visible control. That
build again uses ECX for the draw index and changes the descriptor.

## 2026-11-21 Enemy update exact

Pinned VC7.1 reproduces all 3,900 physical bytes of
`EnemyManagerView::OnUpdate` at `0x00410730`. The comparison includes
97 COFF relocations, the alignment byte and all four switch entries.
Independent replay also confirmed all 69 direct call identities, the five
read-only float definitions and both folded callback bodies. The canonical
comparison rejects separate mutations to the draw instruction, a schedule
relocation addend and the first switch destination's COFF definition.
The exact `ZunTimer::HasTickedEvery` definition now shares the ECL compilation
unit with its Enemy and ExAttack callers. Original translation-unit
ownership remains unresolved.

When the timer definition is visible, VC7.1 initially gives the side gate
and the later effect receiver one allocation record. The output reloads
the side pointer in the effect branch, but the gate's EAX is already
assigned before local allocation. The early draw index then takes EAX.
The CirnoLab control exposed a second local request before the vector
call. Its early append reads the old draw head into ECX before VC7.1
merges that append with the normal path. The scratch list advances to EDX
even though the separate early read disappears from the final code.

Writing the append in both source paths reproduces this sequence. The
retained force-death call uses EDX for the offset argument and EAX for the
hidden return buffer. Initializing the descriptor's aim mode before the
bullet type restores the remaining descriptor order. With the timer
visible, the effect receiver uses the shared side type directly. The
canonical comparison runs with
`python3 -B scripts/replay-scoped-exact.py --source src/EclManager.cpp`.

## 2026-11-20 Replay playback side-state load

These measurements use the playback source before the
[co-visible seed match](#2026-11-21-co-visible-replay-seed-and-exact-playback).

Original `BeginPlaybackStage` stores replay byte `+0x1E7` into GameManager
`+0x37C`, reads byte `+0x1E8`, and then loads the side-0 runtime pointer
from GameManager `+0x1C`. The maintained build moves the pointer load ahead
of that store. Explicit relocation replay covers all 540 bytes and finds
19 differences. CirnoLab's `PlayAddedCallback` independently reproduces the
same shifted operands under its documented VC7.1 profile.

Using an `AsciiGameManagerSideView &` for both `sides[0].runtime` and
`sides[0].value30` restores both original operand locations. The `DIR32`
fields at `+0x14D` and `+0x15D` use the original GameManager base `0x004A7D90`
with addends `0x37C` and `0x1C`. All 32 fields agree with the original.
Nine byte differences remain from `+0x152` to `+0x17A`. VC7.1 uses EDX to
hold the runtime pointer through the first score store and ESI for the
replay bytes and score value. The original uses the opposite assignment.
The earlier volatile probe also left nine differences. The new probe uses
an ordinary typed reference and remains outside the strict frontier.

Replacing the stage/runtime scratch union with distinct typed locals emits
the same 540-byte instruction stream and 32 COFF fields, including ESI
reuse for the stage index and the side runtime pointer.

`PlayerLifecycleView::InitializeAddedState` at `0x0041ED43` reads
`0x004A8108 + sideIndex * 4`, confirming that GameManager `+0x378` and
`+0x37C` form a side-indexed pair. Modelling the pair as an array in the
playback probe preserves the nine differences.

An isolated function with its real declarations reproduces both the 19-byte
baseline and the nine-byte side-reference result. A read-only trace of the
pinned C2 backend at `0x107833F2` and `0x10782A1D` exposes the register
decision in the side-reference build. Its additional scored live range has
candidate mask `0x88`, zero cost for EDX and ESI, and no affinity list.
C2 chooses descriptor 3 (EDX) ahead of descriptor 7 (ESI). The generated
pointer load at `+0x15B` and score store at `+0x179` use EDX. The baseline
has no corresponding scored range. Capturing a reference to the score word
instead of the whole runtime object restores the baseline's early pointer
load and 19-byte residual.

The early validation computes `header + 0x20 + stage * 0x28` and tests
that 32-bit address. Computing the address from the full header expresses
this check for the ten-slot stage domain, including indices beyond the three
input-stream rows. The address expression preserves all 540 candidate
bytes and 32 COFF fields in the baseline and in the side-reference probe.

The pinned compiler can also allocate the pointer to ESI with a later load.
Capture the pointer directly after publishing `+0x1E7`, then read and publish
`+0x1E8`, and snapshot the first frame's score before publishing `+0x1E9`.
C2 gives the score snapshot the `0x88` candidate mask and selects EDX. The
pointer load uses ESI at `+0x173`, after both side writes. This 540-byte
variant has 27 differences after relocation replay. Moving the score read
after the second side write or using its field directly removes the scored
range and restores the 19-byte baseline. An exact candidate must place the
load at `+0x15B` and allocate the pointer to ESI.

The tested statement orders reach either the original load offset with EDX
or ESI at a different offset. Clearing prior-stage markers, populating the
input cursors, and copying configuration through alternative source forms
preserve the early register choices in the candidates that match their
original instruction ranges. Flattening the replay layout or expressing its
header and payload as members also preserves the two baseline results.

## 2026-11-21 Replay playback scheduling across the restore block

Two natural source controls reproduce the entire original side-state
sequence at +0x14B through +0x17A, including the runtime pointer load into
ESI at +0x15B and all operand fields in that interval. The pinned VC7.1
baseline still emits 540 bytes with 19 differences after relocation
replay. The typed side-reference control still emits 540 bytes with nine
differences and allocates EDX to the runtime pointer.

The baseline assigns the side pointer to ESI before scheduling. Its
scheduler record has priority `0x19400`, above the `0x19000` priority
of the preceding GameManager +0x37C store. This lets C2 hoist the pointer
over that store. The dependency graph also connects the earlier side
pointer load to the final RNG seed call. The backend inserts that edge
from its memory access list at `0x107B8187`.

Representing the two score destinations with a loop over the two recorded
frame pointers removes that edge. The compiler expands the loop into
two stores, and the pointer and the +0x37C store both have priority
`0x18800`. The store wins by its earlier ordinal. The 545-byte control
retains a separate copy for the second frame pointer and a stack store
at +0x1A3. Taking the configuration destination before restoring the
right input cursor provides the second control. It keeps the original
470-byte prefix, then changes the tail schedule and emits a two-byte
copy to EDI. Ordering the cursor stores as left, FPS, right, then the
third stream yields the original 540-byte extent and 32 COFF fields
with 39 replayed byte differences starting at +0x1D6. All three
controls reproduce their results in the real ReplayInput.cpp compilation
unit.

The base and the 540-byte cursor-order control both retain the RNG call
at scheduler ordinal 81 and the side-load dependency on that call. In
the cursor-order control, EDX also receives the FPS pointer and the
configuration pointer. These two later writes and the single MOVZX seed
read give the +0x37C store one extra outgoing edge. Its priority rises
to `0x19400`, tying the side load. The baseline and the cursor-order
control each reach the same graph boundary. The 545-byte score loop
and 542-byte right-cursor binding instead place the call beyond that
boundary. Their earlier graph ends with the sentinel at ordinal 82 and
clears the pending reads at `0x107B4642`.

The maintained source has the original tail. The remaining
experiment is to preserve that tail while reproducing the earlier
side-state schedule. Capturing the destination slot by reference,
binding the replay configuration source separately, and giving the
lives conversion explicit integer locals each preserve the baseline
when the rest of the emitted code agrees.

In the score-loop control, both extra instructions are materialized before
scheduling. At the C2 global allocation entry `0x1077C0F5`, the second
score read has a separate address temporary. Its source resolves to the
right frame already in ECX and its destination resolves to EDX, producing
the two-byte copy at `+0x182`. The three-byte store at `+0x1A3` starts as
opcode `0x26B` for the right frame's stack slot. Global allocation turns it
into a move from ECX to `[ebp-8]`. The subsequent right-side float
conversion uses the same frame from ECX and the first conversion reuses
the stack slot. Populating the score-loop array from scalar frame pointers
removes both instructions and restores the baseline schedule.

A separate control passes the left replay byte through a `double` before
the float store. C2 inserts an opcode `0x269` conversion record in the
scheduler. It contributes to the region cutoff and emits zero bytes. The
RNG call still assembles at `+0x207` and falls outside the region containing
the side load. The initial-lives store and the side load each lose one
outgoing edge, leaving scheduling priorities `0x17800` and `0x17C00`
respectively. The control reproduces all 540 baseline bytes and 32 COFF
fields in both the isolated function and the full ReplayInput.cpp carrier.
The baseline residual is still 19 bytes after relocation replay. Relative
to this control, the score loop adds exactly one outgoing edge from the
initial-lives store, a register dependency on the ECX-to-EDX copy at
`+0x182`. The side load keeps the same outgoing dependencies. This accounts
for the tied priorities in the score loop and isolates the missing
scheduling change from the region cutoff.

### Priority calculation and right-side references

The pinned scheduler at `0x107B5084` calculates these priorities from the
longest weighted dependency path, the outgoing-edge count, and a bonus for
the side-pointer load. The path length and edge count each contribute
`0x400` per unit. The bonus contributes `0x8000`.

| Control | Lives store path + edges | Pointer load path + edges + bonus |
| --- | ---: | ---: |
| Maintained source | 70 + 30 | 55 + 14 + 32 |
| Left float conversion | 65 + 29 | 50 + 13 + 32 |
| Score loop | 68 + 30 | 53 + 13 + 32 |
| Early configuration destination and cursor order | 70 + 31 | 55 + 14 + 32 |

The maintained scheduler selects the pointer at cycle 51, ahead of the
lives store at cycle 52. With tied priorities, it selects the earlier
lives store at cycle 51 and uses the pointer load at cycle 56, between
the replay-byte load and its store.

Using one right-side reference for `character20` and `value30` gives the
same dependency graph, schedule, 540 bytes and 32 COFF relocations as the
left-side reference control. The right-side control uses the maintained
left-side source. In both controls C2 allocates the left runtime pointer
to EDX and adds a register dependency from the lives store to that load.
The full ReplayInput.cpp build confirms the nine remaining register
bytes and every original relocation position and destination. A linked
comparison of the remaining candidates still requires ESI at `+0x15B`.

The pinned frontend rejects `__restrict` with C4235 and
`__declspec(noalias)` with C2485. Reference and receiver forms of the
replay manager parameter preserve their respective baseline byte streams.

### Copy removal and memory ranges before scheduling

After global allocation, the baseline and the score-loop control both
contain integer copies whose source and destination use the same physical
register. C2 compares their descriptors at `0x10779F26` and unlinks the
move through `0x1075F13C` before building the scheduling region. The
observed ESI self-copy reaches that path in both controls. The score
loop's ECX-to-EDX copy keeps distinct descriptors and remains an emitted
instruction. Ordinary integer copies therefore provide no counterpart
to the non-emitting floating-point conversion in these builds.

The memory comparison at `0x107B0138` returns disjoint for the two
four-byte ranges at GameManager `+0x37C` and `+0x1C` in both the baseline
and the side-reference control. It reports possible overlap between
the `+0x1C` load and the indirect first score store. Using native
two-entry arrays for initial lives and recorded side options, or
accessing the same fields by pointer-to-member, does not produce a
variant with the complete original instruction stream.

## 2026-11-21 Co-visible replay seed and exact playback

`ReplayManagerView::BeginPlaybackStage` at `0x00420840` reproduces all
540 bytes and all 32 original relocation fields when C2 sees the
`RngRuntimeView::SetSeed` definition in the same translation unit.
The setter reads its unsigned-short parameter through a const reference
and reproduces its own 14 bytes at `0x00420160`. The parameter remains a
word passed by value in the existing thiscall interface. The original
translation-unit partition remains unproven.

The seed call stays at playback offset `+0x207` and scheduler ordinal 81.
When the setter is external to the compiled unit, the initial-lives store
and the side-pointer load both have outgoing dependencies on the call.
Giving C2 the direct value-assignment setter body removes both edges and
leaves the 19-byte replayed residual. Reading the seed argument through the
reference retains the store's `0x20` dependency on the call and removes
the side load's `0x40` dependency. This ties their priorities at `0x19000`.
The store emits at `+0x14B` and the pointer load emits into ESI at
`+0x15B`. The same result was independently reproduced by reading the
parameter through a typed pointer.

The scoped replay rebuilds `src/ReplayInput.cpp` and
`src/ReplayRuntimeLeaves.cpp` with their canonical profiles. The relevant
separate callers are `replay-stage-dispatch`, `win-main` and
`supervisor-network-message-handler`.

## 2026-11-21 Title replay caption tables

`TitleScreenView::DrawReplayMenu` references a continuous block of four
caption tables at `0x004A1CF8–0x004A1D67`. They begin at `0x004A1CF8`,
`0x004A1D3C`, `0x004A1D50`, and `0x004A1D58` and contain respectively
17 character names, five difficulties, two replay modes, and four match
labels. The preceding slot points to the name-entry alphabet and the
next table begins at `0x004A1D68`. The replay USER record uses its own
separate name arrays. Its character captions are Shift-JIS, while the
menu uses the original fixed-width display labels. The old declarations
used the USER record symbols for three menu roles and left the two mode
labels without definitions.

The menu source now defines its own caption arrays. The pinned build
reproduces the complete 112-byte initialized table section with one
zero-addend DIR32 field per slot. Each of the 28 string definitions agrees
with the original pointer destination. The entire draw body has 1,280
bytes and all 57 original operand fields, including 30 references to
16 code literals. The table change left 29 differing instruction bytes
at `+0x185` through `+0x321`. The original uses EDX for the selected index
and ECX for the animation timer and caption scratch. That build exchanged
the two registers and stored the replay pointer before the selected index.

### Selected row conversion

The register differences originate in VC7.1 global allocation.
In the 29-byte baseline, the selected-index range admits ECX, EDX, ESI,
and EDI at equal cost and takes ECX. The timer is allocated afterward and
takes EDX. The index and replay pointer are spilled in the opposite
order from the original. Rebuilding CirnoLab's complete `mainmenu.cpp`
at `afebc8b` with the pinned `/O1 /Ob1` compiler reproduces the same 29
differing positions and 57 operand coordinates and types. Precompiling
its `global.h` leaves both the body and relocation records unchanged.

Changing only the Y-coordinate expression to
`position.y = (float)selectedReplay * 13.0f + 90.0f;` reproduces the
original EDX index, ECX timer, and index-before-pointer stack stores.
The allocator now sees an address for `this+0xC908` and an indirect index
load. It assigns the timer to ECX globally and the index to EDX during
local allocation. The integer-to-float instruction at `+0x1B2` reads
`[ebx+0xC908]` in this control. The original reads the saved index at
`[ebp-0x1c]`. The encoding adds three bytes. Aligning all 376
instructions and resolving relative operands proves that the other
instructions, branch destinations, and 57 relocation identities agree.

Capturing that field as a float before the column test emits the original
saved-index operand and register assignment but moves the `fild` before
the column branch. The matching source binds one const reference to the
selected-slot field. Reading through that reference for the record
lookup, the integer snapshot, and the Y coordinate gives the original
stack-based `fild` at `+0x1B2`, after the two stack stores and the column
test. Both a typed pointer and a const reference reproduce the same
1,280-byte stream with all 57 relocation fields.

The canonical `title-screen-draw-replay-menu` unit compares the complete
body, its literal definitions, and the 112-byte caption section. The
Extra Mode reference uses the same table symbol as Story Mode with a
four-byte addend. The Shift-JIS USER tables retain their separate
`replay-save` proof.

## 2026-11-21 Title result cursor direction

In the 484-byte baseline for `TitleScreenView::OnUpdateResult`, the first
instruction difference after aligning branch destinations was at
`0x004267B2`. The candidate computed the table-element address in EAX
and sign-extended the byte from memory. The original reads the table byte
into CL, pushes stride 2, and sign-extends CL into EDI before pushing
the other arguments. The following cancel path and initialization block
agree after accounting for the one-byte shift.

The exact `UpdateScreen8Mode0` sibling uses the same split load and extension
near its direction-dependent cursor calls. Retaining those direction arms
with the inactive highlight in both reproduces the original result code.
VC7.1 merges the arms into one call and preserves the sign-extended value
for the following score-bank store. A single captured character and call
produces 483 bytes. Keeping distinct lookup sites on the two unlock-loop
exits recovers 485 bytes with ten structural differences. Hiding the real
horizontal and inactive cursor definitions, making the cancel callees visible,
and making the character table definition visible leave the respective
single-call instruction streams unchanged. The register and call schedule
establish this source shape as a matching reconstruction.

The `title-screen-update-result` unit covers `0x004266B5–0x00426899`,
including nine call and 25 data fields. Both result order operands reach the
16-byte definition at `0x004A1DAC`. Character selection independently reads
the equal-valued table at `0x004A1D7C`. The literal definition matches
`title/result00.png` including its terminator. Input references use replay
slot 2 word06 at root `0x004ACE18` plus `0x122`. The three PLST unlock rows
start at `0x004A8180` plus `0x4C`, `0x5C`, and `0x6C`. The unlock sequence
counter at `0x004AC8D4` lies in the PE's zero-filled data range and has eight
references inside this owner. These shared operands retain their original
bindings.

## 2026-11-21 ChargeAttack input and timer allocation

`PlayerLifecycleView::UpdateChargeAttack` matches all 1,210 bytes at
`0x0041F810–0x0041FCC9` under the pinned VC7.1 profile. The unit checks
58 call relocations, 12 data relocations, and all five referenced four-byte
float definitions. The input address uses the 0x8E-stride replay records
at `0x004ACE18`. The three configuration-pointer reads use `0x004A7E78`,
the GameManager `+0xE8` slot. The configuration row begins at `+0xB4`.

The exact source binds the one-past pointer of the current two-byte mode
row and the signed offset from that end at both early lookups. VC7.1 folds
these into `[edi+ecx+0xB4]` and `[edi+edx+0xB4]`. The DWORD side stays in
EDI and the single replay input receiver stays in EBX for all six input
queries. The fallback byte keeps its `EBP-1` home. The third configuration
read and both sound-side reloads use the live side member.

The earlier row-start lookup entered global allocation with a `0x252` LEA
for the row base and a zero-displacement `0x253` consumer. The substitution
at `0x1075AC02` folded the displacement into that operand while retaining
the configuration-first ordering. A 16-bit side snapshot put the side
first in both operands and shifted every relocation by one byte through
its entry MOVZX. A DWORD lookup of the whole configuration and a byte
offset also put the side first, but it selected EAX for the configuration
pointer. Commuting additive expressions and isolated late branch cuts in
the row-start control left both SIB differences.

In the byte-offset control, making both genuine `ZunTimer::operator==`
and `operator>` definitions visible recovered ECX for the first
configuration read. Either definition alone kept EAX. Omitting the cooldown
branch also recovered ECX in that control. Using the byte-offset lookup
only for the first test with both comparisons visible left a single SIB
byte at `+0x4E`. The fallback still needed its deferred configuration load
and the side-first address. Both row-end lookups reproduce that combination
with the existing timer declarations. The exact carrier keeps the original
comparison calls and the original saturation-timer address at `+0x2B3`.

The cold scoped replay passed 24 exact units in eight compiler jobs,
including the charge owner, its PlayerRuntime caller carrier, and both
replay input leaves.

## 2026-11-21 CheckBulletCollision reward storage and ordered bounds

Pinned VC7.1 reproduces all 862 physical bytes at 0x0041DFF0 through
0x0041E34D, including all three RET12 sites. The canonical
`player-check-bullet-collision` comparison verifies 26 relocation
fields, including 12 direct calls, and the four literal definitions
for double zero, float zero, 0.5f, and 8.0f.

The inner `for` advances the cursor until an eligible shape contains
the position. Its successful exit places the shared dispatcher at
+0x8E before the rectangle geometry at +0xBB. A positive nonempty-slot
guard leaves the empty-slot epilogue at +0x350. The rotated half-width
and AABB minimum X have separate scalar lifetimes. All four rotated
bounds require ordered inclusive comparisons. The AABB uses outside
comparisons with different unordered behavior.

The cursor stays live through cancellation and acquires its EBP-4
home in the compiler. Putting the hit-count update after the outer
loop lets VC7.1 duplicate the common source exit. The cancellation
exit reloads ESI after AddScore. The direct-hit exit stores ESI to its
stack home before incrementing the count. Duplicated source updates
left six different bytes even after the three exits were placed.

Cancellation reads the pointer at GameManager +0x7C (0x004A7E0C).
The setup thread stores the shared effect manager in the third side
record, and cleanup resets that pool. The collision COFF field binds
`g_GameManager` with addend +0x7C. The coordinate and difficulty
references use the same owner with addends +0x358 and +0x11C.
The original reloads the RNG receiver before both calls. Making the
full RNG implementation co-visible lets VC7.1 retain ECX across the
first call and drop the second receiver relocation. Rotation, popup
transforms, side selection, effect spawning, sound, respawn, reward
application and score publication controls preserved the candidate
within their respective builds.

The final scoped cold replay passed 36 exact units across eight compiler
jobs, covering 13,473 physical bytes and 732 relocation fields. This
includes the collision carrier, the GameManager setup and cleanup
owners, and the Etama update caller.

## 2026-11-21 Network service retry and input context

Pinned VC7.1 reproduces all 1,633 bytes and 106 relocations of
`Supervisor::UpdateNetworkService` at 0x00430AA0. The extent ends at
the RET at 0x00431100. The following 15 CC bytes are unowned.

```bash
python3 -B scripts/replay-scoped-exact.py --unit supervisor-network-service
```

The two PopFrame phases each use a block-local seed pair and prediction.
VC7.1 reuses EBP-0xC and EBP-0x8 when their roles exchange. The result
DWORD is at EBP-4. A standalone input array retains the word stores at
EBP-0x14 and EBP-0x12 and the DWORD reloads into EDI and EBX before
the EDX argument moves. The fastcall interface consumes the low word.
Both phase-local output objects are needed to recover the stack
displacements and preserve every relocation.

The readiness jump at `+0x2E7` targets `+0x3FE`. It passes over the
receiver reload at `+0x3F8` when the initial frames are already queued.
Reloading the receiver only when handshake completes, then using it for
both readiness and local input, reproduces that edge. Reloading it again
after publishing or predicting a frame lets the skipped phase enter the
queue check at `+0x516` with its existing receiver and clock. This
recovers the original 0x14 stack frame, the two direct timeGetTime calls
during publication, the receiver and timer reload order at `+0x50A`,
and the full PopFrame argument and store order.

Each retry branch sets the result before entering the shared cleanup.
VC7.1 emits the short JNE at `+0x406` after checking readiness and
merges the timeout at `+0x554` into a backward JBE to the early retry.
C2 presents the retry-result assignment in the block immediately
following the readiness branch at allocator entry.

Making the exact PopFrame body co-visible changed the later layout
and register scheduling without changing the call inventory. With
GetSeed visible, VC7.1 kept the outgoing frame value in EDX across the
call and removed the original checksum reload from 0x004B40A0.
Co-visible definitions of DrawTextCentered, GetInput, ApplyNetworkInput
and the lock and session helpers left the candidate bytes and references
unchanged. Reloading the receiver after
both publication paths alone gave the shared network pointer EDI in the
C2 allocator and assigned a timer-import partition across the prediction
interval to EBX. The resulting frame-counter spill at EBP-4 displaced the
scratch record and grew the frame to 0x18 bytes. Tying the readiness and
controller-side reads to the conditional receiver restored the original
allocation through the queue check.

A shared scratch record preserved the target stack offsets but changed
12 bytes in the two input reloads and copies. C2 inserts `0x26A` loads
for the record members inside `0x1077C367`, before global allocation.
The inserted load lands in EDX and is copied to EDI or EBX. An
independent input array gives a direct load into each saved register.
Leaving the other 12 bytes in a single aggregate puts the input array
at EBP-8 instead. The stack candidate sort at `0x10788672` compares
effective access spans. The two input reloads span six bytes, rounded
to eight by stack layout. Splitting the aggregate into two function-wide
4-byte output unions plus a separate result matched the final input
instructions but left eight incorrect stack displacements. Splitting
only the seeds or only the prediction by phase grew the frame. Giving
both output objects their own phase-local lifetime reproduces the
original 0x14-byte frame without an explicit storage overlay.

The two 12-byte outgoing packet slots at 0x004B409C and 0x004B40A8
and the 4-byte network pointer cell at 0x004B42D0 are in the zero-filled
portion of the original data section. All 18 packet references resolve
to the respective base plus the original wire-field offset. The
three message references point to distinct NUL-terminated CP932 strings
in file-backed read-only data. These remain external bindings in
the COFF carrier. The following sizes and SHA-256 digests include the
terminating NUL.

| Message | Target address | Bytes | SHA-256 |
| --- | --- | ---: | --- |
| Disconnect | 0x00490A88 | 31 | `e090f2f63c305d57e9f325cdf0664f68bd271d2669df717cf28032db956f2335` |
| Wait | 0x00490AA8 | 25 | `426343891901918c1bb6f3587faa04ee41eacc42f494991007312fc13c5d9017` |
| Search | 0x00490AC4 | 29 | `3c97c551d2d16a54d5eaa65473067a9f1a96e85b23eacfa7c5f422972b682ce9` |

The cold scoped replay passed nine exact units across eight compiler
jobs, covering 5,014 physical bytes and 371 relocations. This includes
the service, its OnUpdate caller, the input and frame-queue functions,
the packet sender, and the WinMain carrier that sets the network pointer.

## 2026-11-21 Etama paired trigonometric helper

Pinned VC7.1 reproduces all 33 bytes of `fsincos` at 0x00412780
with zero relocations or owned data. The helper executes a single
FSINCOS and stores cosine before sine. RET 4 ends at 0x004127A0.
The following 15 CC bytes are unowned. The sole direct call is at
0x00413C3B in EtamaController::OnDraw. It passes distinct output slots
and the laser angle. The PE contains no absolute pointer to the helper.

```bash
python3 -B scripts/build-match-unit.py --unit etama-fsincos
python3 -B scripts/compare-coff-function.py --unit etama-fsincos
```

The operator approved restoring the existing x87 implementation only
for this helper. The cold comparison of that carrier also passed all
three authored Etama units and the folded SDK pointer conversion alias.
OnDraw retains its 600 bytes, 27 relocation fields and seven checked
literals, including the reviewed call to 0x00412780.

The natural C++ float form emitted calls to the exact 12-byte
`@cosf@4` and `@sinf@4` wrappers at 0x00401060 and 0x00401070.
It reached 36/33 bytes with 25 differing bytes after relocation
masking and 33 after replay. Using double locals emitted separate
FCOS and FSIN in a 20-byte body without relocations. Those two C2
nodes were already distinct at allocation entry at 0x1077C08E and
survived through emission at 0x107B1490. In the historical x87 control,
C1xx returned the paired opcode 0xD5 from its mnemonic lookup at
0x1042DB2C. The paired instruction was already present outside the
ordinary C2 block list at allocator entry. The emitter wrote D9 FB
at helper offset 0xF. Changing definition visibility preserved the
complete caller and siblings.

The adjacent TH08 1.00d body at 0x00433880 has the same 33 bytes and
three direct calls. The reviewed TH095 binary has no identical body.
A scan of 250 installed VC7.1 and SDK archive/object files found no
complete occurrence. The pinned D3DX `sincosf` member has a distinct
19-byte cdecl body. Original source or library provenance for the TH09
helper remains unresolved. The `etama-fsincos` match unit records its
byte-exact code generation while the origin ledger retains its review
status. Authored progress is unchanged.

## 2026-11-23 review of unresolved function origins

Direct Ghidra analysis of the attested Japanese 1.50a image confirmed
the starts, inclusive ends, and byte counts for all 35 reviewed candidates.
The only origin change is the five-byte `DummyMidiTimer::~DummyMidiTimer`
at 0x0042F3F0. The original deleting destructor at 0x00431680 calls it
before optionally calling scalar `operator delete` at 0x0047B249. The
five-byte body tails to the base destructor at 0x0043D320. The pinned
`/Ob0` SupervisorDeletedCallback object emits both complete bodies
with those same relocation destinations. The user-declared destructor
control emits an additional vtable store. The constructor at 0x0042F3B0
also has a confirmed call and vtable identity, but a user-declared empty
constructor can express the same operations.

The 34 remaining origins have these evidence limits.

- 29 constructors or destructors perform member or base operations only.
  This includes the Player collision query array constructor at 0x0041CEB0
  and the Player tail record constructor at 0x0041F060. Their call contracts
  establish which storage is constructed. The machine code does not
  distinguish a user-declared empty body from implicit generation. The
  ExAttack constructor pair has both forms reproduced under VC7.1. The
  15-byte replay payload constructor and 18-byte replay record constructor
  also match with their original call relocations as explicit `/Ob1`
  definitions and as implicit constructors emitted for array creation under
  `/Ob0`. Under `/Ob1` the implicit record constructor inlines the payload
  constructor and calls `GameConfiguration` directly.
- Three bodies have multiple observed interfaces. The pointer identity at
  0x004343D0 is both a constructor callback and a pointer conversion. The
  field-zero getter at 0x00435EC0 has multiple callers with different
  receiver types. The one-byte return at 0x0043D2B0 has both call and vtable
  references, including the maintained MidiTimer callback and AnmManager
  destructor.
- At 0x00405700 the SDK inline and the maintained wrapper reproduce the
  same `atan2f` body. At 0x00412780 the laser-draw call establishes the
  `fsincos` contract. The local SDK archive scan and adjacent-game
  comparison leave the original provider open.

The replay control objects carry different COMDAT selections for explicit
and implicit constructors. The retail PE has no COFF symbol table or PE
debug directory to recover that distinction. The ambiguous origins remain
outside authored totals pending source-level or provider-specific evidence.

## 2026-11-23 position independence and product checks

The VC7.1 linker selected different `SpriteHasTexture` COMDATs when the full
object list was reversed. The normal link selected the 47-byte target-exact
body from TitleDrawCallback. The reversed link selected a 51-byte body from
AnmManagerRuntime. Both layouts now put the reviewed provider before the
competing carrier. The provider itself moves from 0x00401148 to 0x0044C550.

The earlier cold build produced 386 carrier objects. All 1,084 configured
units passed comparison with the original in their own profiles. Five of
the six units replaced in that trial's EclManager carrier differed from retail.
`ecl-move-random-biased`, `effect-draw-callback-ring`,
`effect-draw-type29`, `effect-draw-type31`, and
`player-position-callback-type11`.

The linked verifier checks 14,668 code relocations per PE, including
28 references to four private read-only input sections. For those fields it
checks the full section contents and the shared base implied by every access.
It also checks the generated data's 614 labels, storage extents, initialized
bytes and 552 pointer fields in each layout. All 1,084 code units and 613
generated data labels change address. A third check puts `ProductData.obj`
before the game carriers in a copy of the normal linker response file
with a separate output and map. It passed the same byte and relocation
checks and moved the remaining label, `th09_rdata_0048F4E8`, from
0x0048FDA8 to 0x0048EB68. The two regular trial PEs and the original have the
same 156 imports across nine DLLs and identical payloads for both resources.

The missing-resource and invalid-resource link checks return nonzero. The
layout verifier rejects pairing a PE with the other layout's map and rejects
a changed generated data pointer. The original and both trials reached the
attract-mode match under Wine. All three also opened Options, returned to
the title, and exited normally. The three Options captures were identical.
Production data ownership remains open.
[External replay evidence](#2026-11-23-replay-layout-divergence-and-cpu-table)

## 2026-11-23 replay layout divergence and CPU table

TH09's bundled `demorpy0.rpy` is repeatable when extracted with the
TH09 PBGZ key and selected in the Replay menu. Two independent retail
captures and the normal trial agreed across the first 600 calculations
of stage 9. Both runs of the reordered trial first differed at calculation
516. The recorded input words were identical. The reordered trial's
effective right input was 96 instead of 16, and its RNG generation count
was one smaller. A write watchpoint showed the normal trial's extra draw
in `PlayerUpdateSelectorState` after identical enemy reward draws.

Reversing only the writable initialized data reproduced the divergence.
Separate code, read-only data and BSS reversal checks agreed through
calculation 520. The CPU difficulty table begins at 0x004A0330 and its
71 rows of 0x1C bytes end at the ECL callback table at 0x004A0AF4.
`PlayerUpdateSelectorState` indexes records at the base. Player update
indexes the same stride from 0x004A0338, the third word in each record.
The old generator placed that timing label in a separate block, breaking
the base reader whenever those blocks moved apart. The declared extent now
keeps the timing label at +8. Both linked tables match all 1,988 original
bytes. The layout verifier rejects the old split and both repaired layouts
agree with retail across the captured 600 calculations.

The retail capture also reached all 7,996 input records. With the repaired
table, the trial and reordered trial agreed on every row up to shutdown
at replay frames 3,004 and 3,018, respectively. The reordered trial's
`Supervisor::OnUpdate` called `CheckVersion` at global calculation 3,999
and received -1. Both trials quit through the success exit from the same
callback. The archive's `0150a` record binds 685,056 bytes and checksum
2,884,521,103, which match the original PE. Both trial PEs are 700,416
bytes. The complete product playback gate remains open at this check.
The record was independently re-extracted from the SHA-256-attested game
archive. `scripts/build.py --runtime-version EXE [EXE ...]` checks that
archive digest and applies the game's DWORD sum to each named file before
Wine is needed. The stock PE passes and both current trial PEs fail.

Extract the fixture with the existing thtk `thdat` tool using version
`-x 9` and the member name `demorpy0.rpy`. The probe uses the generic
hardware-breakpoint driver from the read-only TH08 checkout. Run each
case under a separate Xvfb display, serially, with the checkout's Wine
prefix. The observer records input histories, RNG state, scores, and
raw float position bits at the priority-27 replay callback.

```bash
th08=/home/pentester/coding/codex_ida/th08-reconstruction/th08
fixture=.analysis/replay-fixture/demorpy0.rpy
mkdir -p .analysis/replay-fixture
thdat -x 9 -C "$PWD/.analysis/replay-fixture" "$PWD/resources/th09.dat" demorpy0.rpy
for layout in retail trial trial-reversed; do
    nice -n 10 xvfb-run -a -s '-screen 0 800x600x24' python3 -B scripts/probe-trial-replay.py \
        "$layout" "$fixture" ".analysis/replay-$layout" \
        --ptrace-runner "$th08/scripts/replay-ptrace.py"
done
cmp .analysis/replay-retail/trace.csv .analysis/replay-trial/trace.csv
cmp .analysis/replay-retail/trace.csv .analysis/replay-trial-reversed/trace.csv
```

## 2026-11-23 D3DX8 provider and surface output

The pinned VC7.1 Platform SDK archive matches all 12 D3DX entry points
referenced by the game match units, plus `CD3DXImage::Load` at 0x00454F63.
The 13 complete COFF bodies cover 1,753 bytes. All non-relocation bytes
match the attested TH09 executable. In both trial PEs, all 33 COFF
relocations resolve to their linked symbols with the recorded addends.
The alternate SDK archive in the TH08 checkout disagrees at 11 of the
12 game-facing entries.

The retail wrapper at 0x004523F6 passes its ninth argument directly to
`CD3DXImage::Load`. On success the helper stores the last two metadata
words at output offsets +0x14 and +0x18. Read-only hardware breakpoint
captures before and after those stores observed the same transition in
the original and both Wine trials. Surface slot 8 began with seven zero
DWORDs and received `[640, 480, 1, 1, 22, 3, 1]`. With the target's
0x14-byte slot stride, those last two words occupy the first eight
bytes of the next slot.

The trial link now verifies the SDK archive digest and names the archive
by explicit path. The data generator indexes the same pinned archive.
Both commands reject the alternate SDK, and the two relinked PEs match
their previous images byte for byte after normalizing the PE timestamp.

## 2026-11-23 DirectInput data format ownership

The original `SetupDirectInput` references the controller format at
0x00491854 and the keyboard format at 0x00491A5C. The trial links resolve
them to the Platform SDK `dinput8.lib` definitions in `dilib4.obj` and
`dilib2.obj`, respectively. Both layouts match the original headers and
all 164 controller and 256 keyboard object descriptors. The comparison
follows the descriptor array and GUID pointers at each layout's addresses
and checks object offsets, type and flags, GUID contents and GUID sharing.
The layout verifier now checks those library-owned tables with the existing
code relocations for `SetupDirectInput`. Mutations to the controller
header, descriptor offset and GUID bytes each fail the check.

## 2026-11-23 Effect and ExAttack linked callback tables

The linked layout verifier compares the 38 reviewed Effect rows and 27
reviewed ExAttack rows directly against the attested original. It uses
the table references from the two factory units and the linked addresses
of each original callback. The other fields and null slots retain their
original bytes. The Effect range ends at the ending-file path table
and the ExAttack range ends at the base-limit table.

The Effect rows move from 0x004A0C38 to 0x004A2AC8 and the ExAttack rows
from 0x004A1068 to 0x004A2878. All 79 Effect and 108 ExAttack callback
slots resolve to their expected code in both layouts. An in-memory
swap to a different linked Effect callback fails the new comparison,
as does a changed numeric field.

The replay probe now passes the attested original image to the linked
layout verifier before starting either trial. A 60-calculation smoke on
the original and both trials produced identical CSVs across all 13 fields.

## 2026-11-23 CRT startup provider and initializer order

The VC7.1 `libcmt.lib` entry point, `_cinit`, five C initializers
and security-cookie initializer match the non-relocation bytes in the
original and both trial PEs across eight bodies and 1,141 bytes. The
linked checks resolved all public operands to their map symbols and
matched the entry's full 12-byte SEH scope record and both handler
addresses. The unhandled-exception filter saves its previous value in
private BSS.

Retail calls onexit setup, stdio setup, SSE2 detection and multibyte
initialization in that order, then installs the C++ exception filter.
Both previous trial links swapped the SSE2 and multibyte callbacks.
The trial now pins the archive and passes the four `.CRT$XIC` members
as objects in retail order. The layout verifier also checks the
security-cookie initializer and the trial C++ registration entry.
The latter moves from 0x0048D800 to 0x0048D840 and its compiled tail jump
resolves to the respective linked `ProductStaticInit` constructor.
The trial still uses this single wrapper for game globals.

Both relinks keep the original 156 named imports and both resource
payloads. Their 600-calculation Wine traces agree with the original in
all 13 recorded fields, including the earlier divergence at calculation
516. The stock version preflight still rejects both relinked files.

## 2026-11-23 opponent and transition parameter table storage

The 45 five-DWORD opponent parameter rows span 0x004A1500 through
0x004A1883. `SelectOpponentConfiguration` indexes five words per stage
and nine stages per difficulty. `FrontCalcCallback` reads the same
rows through the `g_TransitionResultTable` and
`g_TransitionResultTableAlt` views at +4 and +0x10.

The previous reversed link separated all three labels. It read the first
row as `[5, 0, 4851936, 4852272, 4852656]`. The original row is
`[5, 4, 3, 2, 1]`. The trial data generator now
keeps the two views inside the declared 900-byte owner. Both relinks
match all 45 rows byte for byte and preserve the two relative offsets.
The layout verifier rejects the previous reversed link at this owner.

A read-only Wine capture at the front calculation callback reproduced the
effect in Story mode on Normal difficulty. The prior reversed build
stored 13,107,207 at GameManager +0x64 for the first opponent. Retail
and both repaired links stored 5 and exposed the same five words at row
index 9.

## 2026-11-23 independently linked opponent lists

The 14 pointers at 0x004A14C8 select individual lists of 0x10-byte
opponent entries. Each list has one negative-key record at its end.
All 307 records occupy 0x004A2300 through 0x004A362F. The adjacent BGM
table has 19 entries and ends at 0x004A2300. The data generator
previously grouped the opponent lists with the BGM table because only
the list pointers refer to the individual lists.

The generator now gives each reviewed pointer-only extent a linker
label. All 14 destinations move relative to BGM between the two
layouts. Both links preserve every list pointer, all 307 records,
and all 19 BGM records and path values. The linked check rejects a
pointer to a different list, a changed row, or an extent that omits
the negative-key record.

At the first Story/Normal front calculation under Wine, retail and both
trial builds selected one of the same two eligible entries from player
slot 0. The records were read through the live pointer at GameManager
+0xF0. Each build's list had a different address and retained both
candidate rows and the final negative-key record.

## 2026-11-23 score and configuration round trip

The 0x1FC-byte play-stats record at 0x004A8180 owns the BGM unlock view
at +0x2C and the three character unlock views at +0x4C, +0x5C and +0x6C.
Declaring the complete extent gives the linked verifier an ownership
check for these labels. Both regenerated data sources are byte-identical
to the inputs that produced the tested PEs. Moving just the BGM view by
four bytes fails the ownership check.

A retail Story/Normal match and graceful window close produced a score
file with one nonzero character statistic. The original and both trial
layouts loaded that same file and rewrote it on title exit. Retail then
loaded each trial's output. A read-only snapshot after initialization
matched all 400 score records, the last-name record and all play-stats
bytes outside the eight clock counters across those five loads. The
loaded header included a nonempty compressed payload in every case.
Every run also rewrote the same 204-byte configuration.

In a separate Options run, the reordered build changed configuration
byte +0xAC from 0 to 1. The original loaded 1 into Supervisor +0x434 and
wrote the complete modified configuration back without a byte difference.

## 2026-11-23 linked C++ unwind records

Both trial links pass the existing VC7.1 COFF exception-graph comparison
for the 22 handler references in the match units. Each handler reference
changes address, and identical-code folding leaves 20 distinct handler
addresses in each PE. Per layout the comparison covers 24 unwind states
and 114 relocations, including the cleanup destinations and CRT frame
handler. The linked verifier rejects swapped valid cleanup actions and
a cleanup call retargeted to a different linked function.

The product startup constructor has its own unwind contribution. An
independent COFF replay compared its 13 states, 271 code bytes, 132
metadata bytes and all 42 relocations in each current PE. Its handler
moves from 0x0048D265 to 0x0048D4C5.

## 2026-11-23 linked PE import bindings

Both trial PEs have the original 156 imports across nine DLLs, with
identities compared by DLL and export name or ordinal. Between the two
links, 90 IAT slots change address. The linked verifier reads 226 direct
IAT operands and follows six calls through their import thunks. Each
resolves to the original API independently of the map's import names.
The thunk check covers DirectSoundCreate8's DSOUND ordinal 11.

An in-memory DLL name change fails the import inventory check. Swapping
two import lookup slots keeps the same inventory and fails the operand
check, as does retargeting an import thunk to a different live IAT slot.

## 2026-11-23 disjoint image-base check

For each trial layout, the pinned VC7.1 linker used the same response
file inputs and produced isolated `/FIXED:NO` links at 0x00400000 and
0x00600000. The loadable sections of each 0x00400000 link agree byte for
byte with its fixed trial PE. The two bases have the same section
layout, entry RVA and 14,261 HIGHLOW field RVAs. Applying the 0x200000
delta to those fields reproduces every byte of the shifted sections,
including library contributions. Each layout has 12,755 fields in
`.text`, 533 in `.rdata` and 973 in `.data`.

All four PEs pass the linked trial verifier using their own maps. A
bounded linear x86 decode of each shifted `.text` section found no
operand in the original image's address ranges. The aligned data scan's
candidates were opponent-record halfwords, CRT character-type entries,
and ASCII or import-name bytes. The section comparison rejected a
retained old-base operand and a changed resource byte in private copies.

The reordered 0x00600000 link ran under Wine with the stock archive.
Its first 600 calculations of the bundled replay matched the retail
trace byte for byte across all 13 observed fields. The current linker
command reproduced that result in the built-in attract demo. The observer
attested the replay callback's live instruction bytes at the shifted map
address before it collected the trace.

The [handoff commands](RE_HANDOFF.md#whole-image-link-and-position-independence)
now produce the four `/FIXED:NO` PEs with their own maps and response
files. `verify-trial-layouts.py --verify-bases` runs the usual linked
checks against each map, compares the preferred-base payloads with the
fixed trial, then checks both HIGHLOW site sets and the entire
shifted section payloads. Both layouts reject a missing or unsupported
base relocation, a retained old-base operand, and a changed resource
byte in private copies.

The base comparison also checks section attributes and independently
decodes absolute memory operands in executable sections. It uses small
Capstone batches to keep memory bounded. In the current four relocatable
PEs, all 6,616 decoded unsegmented absolute memory operands into each
image have HIGHLOW entries. Neither shifted layout has such an operand
into the preferred image range. This linear decode covers the linked
libraries as well as game code. Immediate operands and pointer tables
need their own evidence. The score-save allocation size `0x640000` and
the D3DX8 PNG flag mask `0x602000` both fall inside the shifted image.

Removing the same HIGHLOW entry in both relocatable files and retaining
the preferred address in the shifted D3DX8 rotation initializer passed
the previous section-byte check. The independent operand check rejects
it in both layouts. The section attribute comparison also rejects a
shifted image with executable `.data1` constants.

## 2026-11-23 replay pause and popup teardown

The three bundled replay headers contain 7,996, 8,006 and 7,991
stage-9 input records per stream. These spans all exceed the 3,999
global updates before the trial's first failing version check.

The workspace configuration selects WAV music. In a private Wine run
without the supplemental WAV archive, the original faults on pause in
`SoundPlayer::ProcessQueues` at 0x0043F495. The pause command reads
+0x78 from a null active stream. The checked pause scenario uses a
private configuration with music disabled and the stock game archive.

`GameManagerSetupLayout::DeletedCallback` clears 0xAF0 DWORDs through
`g_DeletedScratch`. The original and both linked paths bind that name
to `AsciiManager` +0xB240, the start of both score-popup banks. The clear
ends at +0xDE00 inside the 0xE0AC-byte owner. The linked ownership check
rejects a displaced scratch label.

The Wine probe opened `demorpy0.rpy` from the replay menu and paused
after calculation 600. It sampled the replay counter across a 350 ms
pause and resumed via the regular menu. It then paused again, confirmed
return to title and checked the 11,200-byte popup range. The calculation
counter stayed fixed during pause and advanced on resume in retail
and both trial layouts. All three runs had 486 nonzero bytes before
teardown, cleared the complete range on return and closed normally.

## 2026-11-23 linked virtual tables and MIDI music room

The linked verifier now checks the six C++ vtables named by tracked
operands in the PBG file, sound manager and MIDI carriers. All six
tables move and all 21 linked slots match their COFF function symbols.
The attested retail slots select the same methods or generated
destructors. Substituting a different valid virtual method in
either the PBG file or MIDI output table fails the check.

Retail and both trials ran with MIDI enabled in private copies of the
configuration. All three parsed the opening `th09_00.mid` into slot 0
and the second Music Room selection `th09_01.mid` into slot 31. Their
in-memory track data matches the two archive members, including format
0, one track and 480 ticks per quarter note. The live MIDI vtable
points to each image's `MidiOutput::OnTimerElapsed`.

In the Music Room, Q toggled the menu pause flag and the MIDI clock
advanced during both states. All three returned to the title and
closed normally. The Wine MIDI device handle remained null, so
these observations cover parsing, timer dispatch and control state.
The retail demo at frame 600 had no active parsed MIDI track.

## 2026-11-23 windowed D3D restart and PE resources

In Wine, changing the color-depth option triggered a renderer restart
in the original and both trial layouts. Each run stayed in windowed
mode, destroyed its original window and returned to the title in a
new X11 window with a new Win32 handle. The supervisor calculation
counter advanced after the restart. Both visits used the same
640-by-480 D3D8 presentation settings and format 22. All three processes
closed normally and persisted the change from 0 to 1 at configuration
+0xAD as the sole file difference.

The linked verifier now walks the numeric PE resource directory and
compares resource type, ID, language, codepage and payload with retail.
Both images preserve the icon and icon group, with Japanese language
ID 1041 and payloads of 2,216 and 20 bytes. The resource generator
uses the same bounded tree reader and reproduces its previous
`ProductResources.res` byte for byte. Changed language, codepage,
payload and out-of-directory references failed the linked check.

## 2026-11-23 linked ECL dispatch tables

The EclHelpers interpolation table and the EnemyExAttackLeaves extra
instruction table retain all 20 retail callback identities in the normal
and reordered links. Their COFF contributions contain eight and twelve
DIR32 fields with zero addends. The interpolation table moves from
0x004A04B8 to 0x004A3CBC, and the extra instruction table moves from
0x004A04D8 to 0x004A3C8C. `verify-trial-layouts.py` reads the original
slots and checks the corresponding linked destinations. Pointing the
Hermite slot at Linear or redirecting extra instruction slot 0 to the
slot 1 callback is rejected in private copies of both layouts.

The selected EclManager `/Ob1` carrier retains the exact `RunEcl` body.
The five nonexact bodies have these retail and COFF physical extents in
bytes. Each also differs in relocation count.

| Unit | Retail | `/Ob1` COFF |
| --- | ---: | ---: |
| `ecl-move-random-biased` | 516 | 510 |
| `effect-draw-callback-ring` | 1028 | 1024 |
| `effect-draw-type29` | 2220 | 2380 |
| `effect-draw-type31` | 921 | 905 |
| `player-position-callback-type11` | 732 | 709 |

## 2026-11-23 file parameter table and marker adjacency

`FileSystem::TryDecryptFromTable` scans eight 12-byte parameter rows at
0x004A1E60. Its final iteration can read byte +0x60 before rejecting
index eight. In retail that byte is the first byte of `g_CryptSignature`
at 0x004A1EC0. The previous reordered link split the two symbols. The byte
at table +0x60 was 0x8C and the retail byte was 0x85.

The product data manifest now keeps the table and marker in one 0x63-byte
range. The normal and reordered PEs preserve all 99 original bytes and
the marker label at +0x60. The existing linked extent check rejects the
previous reordered placement and a displaced marker in either new map.
The selected VC7.1 object still matches the complete 220-byte caller and
all 12 relocations.

A read-only Wine startup probe checked the live range at the first eight
calls in retail and both linked PEs. Seven input buffers matched in
order, size, and digest of up to their first 4,096 bytes. The second read
in each run matched the size and sample digest of its running PE.
This probe covered ordinary startup reads. The terminal iteration was
checked by the linked range comparison.

## 2026-11-23 GameManager and replay input storage at startup

The linked data contracts now cover the 0x390-byte GameManager at
0x004A7D90 and the three 0x8E-byte replay input records at 0x004ACE18.
The first contains 69 generated labels and the second contains nine.
The replay inputs include both banks of controller bindings. Declaring
these ranges leaves the generated assembly byte for byte identical.

The linked verifier checks that each reviewed generated range fits inside
its COFF allocation and contains only labels originating within the retail
range. This also checks reviewed objects reached only by table pointers.
A shortened allocation and a foreign label were each rejected for the
GameManager, replay inputs, and first opponent list in both layouts.

In Wine, the 426 bytes after `Supervisor::LoadConfig` matched retail in
both trials. The two banks together held the six serialized bindings. The
912 bytes after `GameManagerResetView::ResetForSupervisorAdded` also
matched retail. The startup probe scopes those comparisons to the two
return sites.

## 2026-11-23 linked CardAttack callback rows

The nine `CardAttackCallbackRow` records occupy 0x004A00C0 through
0x004A014F immediately before `g_PlayerPatternGrid`. Their 36 slots reach
11 distinct cataloged entries. The table moves from 0x004A0580 to
0x004A3B90 in the two trial links. All begin, update, draw, and end
slots resolve to their retail callback identities in each PE.

The callback coverage report and linked layout verifier now include this
range. Both linked checks rejected a private PE copy in which row zero's
begin slot selected row one's valid begin callback. The selected VC7.1
`CardAttack::StartAttack` body still matches all 923 bytes and 29
relocations.

In the built-in attract demo, retail and both trial layouts first selected
row two with attack class two. The four live pointers agreed with the
checked rows, and the indirect call reached the begin callback at retail
identity 0x00441420. The callback received the same object as `StartAttack`
in each run and had return address `StartAttack` +0x10B. The runtime probe
covered the first dispatch.

## 2026-11-23 retail binding for generated pointers and link aliases

All 552 generated data relocations in each trial map back to the same
pointer slots in the attested executable. The layout verifier takes the
retail address of the preceding COFF label and adds the field's offset
within that contribution. It checks the original pointer word against
the target symbol and COFF addend before checking the linked value.
There are 549 zero addends and three nonzero addends in each layout.
Changing a `g_PlayerResourcePaths` destination or addend in a private
COFF object and matching PE formerly passed the linked byte check. The
retail comparison rejects both controls.

The linker response files use 137 aliases with retail address bindings
and two DirectInput library aliases. The verifier now checks the shared
retail address for each of the 137 and the selected library symbol for
both input formats. It rejects a missing linked provider, a provider at
another original address, and a swapped library format provider.

## 2026-11-23 live keyboard movement in local Match Mode

Retail and both fixed trial layouts entered a local 1P versus CPU match
on Normal through the title menus. The probe used private configuration
copies with music disabled. Player zero had the human selector and
player one had the CPU selector. Eight sampled Left updates selected
direction 3 and moved the human player by -4.0 horizontally per update.
Eight Shift+Right updates selected direction 4, set focus, populated
both focus-effect slots and moved that player by +2.0 per update. The
vertical displacement stayed zero. All recorded fields agreed across
the three runs at the return from `UpdateMovementAndOptions`.

The linked verifier now includes the source-present movement frontier.
Both layouts preserve its 1,900-byte selected COFF extent and all 66
relocation operands. The 64-byte focus ID table moves from 0x004A0478 to
0x004A3CE0 and matches retail. Changing a table entry or displacing a
movement operand was rejected in each layout. The pinned frontier build
still has its recorded four byte differences with relocations masked.

## 2026-11-23 DirectPlay GUID bindings and host entry

The layout verifier checks 13 original GUID values through all 36 linked
operands. The scope includes the DirectPlay peer, address, TCP/IP provider
and private session application GUID. Both layouts also preserve the
DirectInput, DirectSound and Shell identifiers in this set. The target
instruction supplies the field addend. For example, three `_GUID_NULL`
operands in `SoundPlayer::LoadBgm` read DWORDs at +4, +8 and +0xC.

Both layouts reject a substituted DirectPlay provider GUID, a changed
session application GUID and a displaced `_GUID_NULL` operand in private PE
copies. The four disjoint-base variants also pass the GUID checks.

The runtime comparison selected the host option from Match Mode in a
private network namespace with loopback and an isolated dummy adapter.
Retail and both trial layouts created the DirectPlay peer and device
address, loaded the same LAN settings and reached `HostSession`.
The selected Wine `dpnet` implementation logged `Host` as a stub and
returned `E_FAIL` (0x80004005) in every run. No player ID was assigned.
The game entered its wait state and returned to the mode menu on X in
all three runs. The reset cleared the device address, active flag and
connection state. Successful hosting and peer synchronization require a
DirectPlay implementation with a working `Host` call.

## 2026-11-23 selected carrier comparisons and right-side focus mode

All six EclManager units with the earlier `/Ob0` profiles matched
their complete retail extents and relocations in that object.
`player-type11-orientation-sign` also matched in the `/Ob1` object
selected by those trials. The other five differed in code length and
relocation count. `trial-link.py verify` reports
the selected-carrier failures alongside the canonical comparisons. A
canonical-profile failure returns a nonzero exit status.

The earlier `/Ob0` movement carrier had the same 1,900-byte extent and
66 relocation fields measured in the frontier. Its masked differences
span +0x127 through +0x12A. Resolving all fields to retail destinations
leaves eight differences across +0x127 through +0x12E.

The trial build now uses the recorded `/Ob1` frontier profile for movement.
The cold `/Ob1` object matches the earlier `/Ob0` object across the complete
1,900-byte extent and the offsets, types, symbols and addends of all 66
relocations. Both fixed layouts and all four base variants pass. The linked
verifier compares the selected object against the original frontier before
checking all of its linked relocations. It rejects the old `/Ob0` index and
an object with a new byte difference outside the recorded offsets.

The Wine probe selected a CPU versus 1P match on Normal and enabled
hold-to-focus in both private configuration slots. Match setup retained
it for the human on the right and cleared it for the CPU on the left.
Holding Z while moving left advanced the right input counter from one to
eight and kept it at eight through update twelve. Focus and both effect
slots became active at eight. The horizontal displacement changed from
-5.0 to -3.0 per update. Releasing Z restored the original movement and
cleared focus and both slots. Retail and both trial layouts agreed on
all observed fields.

## 2026-11-23 D3DX8 SIMD constant section

Six nonempty `.data1` contributions in the pinned Platform SDK
`d3dx8.lib` cover all 2,272 mapped bytes of the retail section. They
come from `d3dxmathsse.obj`, `d3dxquatsse.obj`, `d3dxmathsse2.obj`,
`d3dxquatsse2.obj`, `d3dxtrigsse.obj` and `d3dxtrigsse2.obj`, in that
order. None of the six sections has COFF relocations. Their payloads
match the retail bytes from section offset zero to +0x8DF.

The linked verifier now checks the complete section extent and bytes in
every trial PE. It rejects a changed constant or shortened extent. A
linear decode of each of the four `/FIXED:NO` PEs found 50 code operands
into `.data1`. Every operand has a HIGHLOW field. The section's live
addresses in the low and high base links are 0x004E5000 and 0x006E5000.

In Wine, the original and the reordered 0x00600000 link each entered the
SIMD implementation for two `D3DXMatrixRotationY` calls. The input angle
bits were 0x3FC90FDB. A breakpoint at the live operand showed the same
16-byte constant at `.data1` +0x210 in both images. The complete 64-byte
output matrices matched at the return to the caller.

The linked verifier also checks the six game-facing math thunks that jump
through `g_D3DXFastTable` in `d3dxmath.obj`. All six select their original
slot offsets in the retail PE and in both fixed links and all four base
variants. The complete 460-byte linked table is writable and contains the
112 initial function pointers recorded by COFF and three zero slots. The
table moves from 0x004A4B68 to 0x004A4BC8 in the reordered fixed link. Its
function targets move by three different deltas relative to retail.

Headless controls in the normal and high-base reordered layouts rejected
a thunk using a neighboring slot, a slot pointing to the wrong live
function, a modified zero slot and a read-only table.

## 2026-11-23 linked replay captions and short replay saves

The existing VC7.1 comparison attests the source-defined replay caption
sections and their literal definitions. The linked verifier now uses
that proof to follow every slot in the final PE. The writer has 27 slots
in three tables. Its section moves from 0x004A0508 to 0x004A3C20. The
replay menu has 28 slots in four tables and moves from 0x004A0408 to
0x004A3D20. All referenced literals and table offsets agree with retail
in both layouts and their four disjoint-base variants.

Both layouts reject a caption pointer redirected to a different valid
literal, changed literal bytes and a displaced table label. These controls
cover the text written to replay metadata and shown in the replay menu.

Retail and both trial layouts finished local 1P versus CPU matches on
Lunatic and wrote `replay/th9_01.rpy` from the results menu. The probe set
the human player's health setting to one, moved up through both rounds,
and advanced the closing dialogue with Z. Each file passed the wire
checksum and USER chunk extent check. The three stage-9 streams had
1,936 records in the retail recording, 2,201 in the normal trial and
1,523 in the reordered trial.

Retail's saved replay played in retail and both trial layouts. The
first 480 frames agreed in all 13 captured fields, including the three
input histories, RNG state and draw count, scores and both positions.
Decompressing the three recordings showed that the writer stores its
own executable size and checksum with `0150a`. The retail recording
stores `(685056, 2884521103)`. The normal and reordered trials store
`(696320, 887951917)` and `(700416, 3067938002)` respectively. The
stock version record accepts only the retail pair. Read-only hardware
breakpoints at `CheckVersion` and its return to `LoadReplayData`
showed 0 for the retail recording in retail and -1 for the normal
trial recording in retail. The reordered trial received -1 when
loading its own recording and produced zero playback frames. The
retail probe for the reordered recording timed out before capturing
the loader's return.

## 2026-11-23 linked product startup and exit callbacks

Normal title-menu exit under Wine reached the CRT onexit dispatcher in
retail and in the earlier normal and reordered 0x00600000 trials. All
eight game callbacks present in those trials took the same objects in
the same order as retail. They covered the three AsciiManager chains
in high-priority draw, low-priority draw and calculation order, the
main PbgArchive, ZunMemory, Chain, and the GameManager draw and
calculation chains. Retail also ran two callbacks omitted in that trial
build. The callback at 0x0048DA70 returns immediately. The callback at
0x0048DA90 destroyed the archives at 0x004B42E0. All 320 array bytes
were zero when the callback ran in a second retail exit.

The pinned VC7.1 vector iterators establish that the array has 20
elements of 0x10 bytes each. They use `[ebp+0x0C]` as the stride and
`[ebp+0x10]` as the count. The product glue now defines a typed array
between the loading VM and AsciiManager initializers. Its 36-byte
initializer and 20-byte terminator match retail with all nine relocations
replayed. The shared constructor also matches all 16 retail bytes.

The trial now registers cleanup for the real `GameErrorContext` immediately
after construction. VC7.1 emits the same one-byte body as the retail
terminator for the call to its trivial destructor. The VC7.1 linker folds
that body with the empty placement delete. The generated zero-fill object
is explicitly bounded to its 0x2008-byte native extent, with identical
generated assembly in both layouts before and after the extent declaration.
The four GameErrorContext methods and its constructor still match all 415
retail bytes and 17 relocation fields.

Both fixed links and all four base variants pass the startup checks.
They cover the CRT initializer and exit registration order, complete
zero-fill storage, and 22 startup and exit functions with 119 code
relocations and 45 unwind relocations across 13 states. The archive array moves
from 0x004A8008 to 0x004E2998 between fixed links. The checker
rejects a modified array count, shifted storage, a wrong destructor
receiver and swapped CRT initializers. The error-context controls also
reject a missing or reordered registration, a replaced callback body
and a one-byte-short storage allocation. Console links with the same
startup objects confirmed the context's initialized buffer pointer and all
ten game callbacks in the live CRT table in retail order. Both console
processes exited normally under Wine.

## 2026-11-23 live replay version rejection

`probe-trial-replay.py` now sends its menu input when the live title
menu reaches the selectable state. It verifies the mapped instructions
at the menu, replay callback and `CheckVersion` rejection path. The
failure observation is keyed to the call from `Supervisor::OnUpdate`
and carries the live version identifier, size, checksum, calculation
count and rejection result in `complete.json`. The probe exits nonzero
on this path and retains the captured trace.

Use `--base 0x600000` with `trial` or `trial-reversed` to exercise the
corresponding relocatable PE and map. Before launch the probe checks the
linked layout and the requested PE image base. The original runs with
its fixed address and accepts no `--base` argument.

The reordered 0x00600000 image recorded 3,566 frames before the
periodic check at global calculation 3,999. The check received `0150a`,
size 741,376 and checksum 269,289,566, the same identity as the static
version preflight. It took the -1 return path. All 13 trace fields
agreed with retail through frame 3,566. Retail recorded all 7,996
input frames. The fixed normal trial also matched the 600-frame
smoke. The replay check and stock archive were used without
modification.

PE inspection found 4,096-byte file alignment in the earlier trial links.
Retail uses 512 bytes on disk and 4,096 bytes in memory. Setting
`/FILEALIGN:512` gives the six trial PEs the retail alignment values, and
all six pass the linked layout checks. The fixed normal image shrinks
from 696,320 to 685,568 bytes and the reversed image from 700,416 to
686,592 bytes. Their remaining differences from the 685,056-byte retail
file come from 512 fewer bytes of `.text` and additional `.rdata` and
`.data` bytes. Both data sections add 512 bytes in the normal layout and
1,024 bytes in the reversed layout. For each layout, two links from the
same inputs differed only in the COFF timestamp. The checksum difference
equaled the timestamp difference.

Two console links using the selected game objects ran the actual
`ComputeExecutableChecksum`, `SupervisorAddedLoadDat` and `CheckVersion`
routines under Wine. The console directory supplied the attested
`th09.dat`, and the private working directory supplied each tested PE
as a read-only input to the checksum routine. Both consoles loaded the
104-byte stock version file. Their file sizes and checksums matched the
static preflight for retail, the two fixed trials and the reordered
0x00600000 trial. Both returned 0 for retail and -1 for all three trials.
The three routines and `FileSystem::OpenFile` match retail in the selected
objects across 891 bytes and 58 relocations.

## 2026-11-24 Silent Selene replay prefixes

The replay probe accepts public format-2 TH09 recordings with
`--replay-sha256 SHA256`. It checks the bounded wire lengths and replay
checksum before launch. The native loader still does the version check.
Frame numbering follows the live stage and restarts at 1 on stage changes.
The bundled demo still uses its pinned SHA-256 and stage 9.

| Silent Selene download | Difficulty and player character | SHA-256 |
| --- | --- | --- |
| [78651](https://www.silentselene.net/replays/th09/78651/download) | Easy Komachi | `cd9983c74f573ab9d0111acfc81b47a924a3f3fd69cd98988fb18df2ece96a65` |
| [83649](https://www.silentselene.net/replays/th09/83649/download) | Normal Reisen | `935416c6c40cdedc05c30b948f09ca5a905d2b90c2e504c3622bb0314e1dd64b` |
| [82579](https://www.silentselene.net/replays/th09/82579/download) | Hard Reisen | `4ea304a4ff72a86917a180e50812469a6d77cedecbf52f4fd9412e46ba4b523a` |
| [82831](https://www.silentselene.net/replays/th09/82831/download) | Lunatic Marisa | `04a625accc63108536a546a5d16b75f634705e6e545038bdf2965d3ec0d5972a` |
| [83683](https://www.silentselene.net/replays/th09/83683/download) | Lunatic Reisen | `94c950f07e1a550ea6930e937753420610f9e4733b65df588c9fd2ea12e8d816` |
| [88377](https://www.silentselene.net/replays/th09/88377/download) | Extra Reisen | `75b6c0b56e363d1936fd6d16c9dd731adbc91053ced9b18f858f25533adbe6bc` |

Use the [same observer invocation as the bundled demo](#2026-11-23-replay-layout-divergence-and-cpu-table)
with a downloaded fixture and its hash. Require `diagnosticComplete` and
the requested row count for both images before comparing `trace.csv`.

The Japanese 1.50a retail PE and the fixed normal trial each completed
1,500 frames in all six recordings. All 13 fields agree across the 9,000
paired frames in stage index 0. The fields cover the stage and frame,
three input histories, the RNG seed and draw count, both scores and
both raw position vectors. The trial was built with the working-tree
zero-fill placement update. Both images used the attested stock archive
and identical copies of `th09.cfg`. The reordered 0x00600000 trial also
matched retail for the first 1,500 frames of Lunatic Marisa (82831).

The longer Easy capture stopped in the fixed normal trial after 3,534
replay frames. The periodic version check at global calculation 3,999
returned -1 for `0150a`, size 685,056 and checksum 309,162,082. The
stock archive expects the same file size and checksum 2,884,521,103.
Retail completed the requested 5,000 frames. All 13 fields agree through
frame 3,534. The stock version check stops the trial at that point.
Replay acceptance uses gameplay trace agreement and PI acceptance uses
linked reference and runtime evidence. A rebuild with its own executable
size and checksum can pass both.

The TH08 native Windows `bugfix` build accepts a version name present
in the archive table without requiring the executable identity recorded
there. TH08 keeps the original size-and-checksum logic in its exact
build and validates long replays by trace and stage-end score agreement.
See `docs/WINDOWS_I386_RUNTIME.md` and `docs/POSITION_INDEPENDENCE.md` in
the read-only TH08 checkout.

`trial-link.py link --playable` applies that policy to TH09 by compiling
only `SupervisorVersion.cpp` with `TH09_PLAYABLE` and linking a separate
PE with the same VC7.1 carriers and data. The replay loader still checks
the payload checksum. The ordinary version owner preserves its full
retail comparison (344 bytes and 22 relocations). The playable build
matched the original through all 5,000 requested Easy Komachi frames
using the same stock archive and configuration. This covers the point
where the ordinary trial exited.

[Extra Komachi 78164](https://www.silentselene.net/replays/th09/78164/download)
has SHA-256 `7fc00509df98c7d7c4333e2b1fb4093bf644b221ed4719dc4c61cf8e656af272`.
Retail and the playable build both reached the title registration after
all nine stages and 33,661 input frames. Their 13-field traces are
byte-identical, including both scores at each stage boundary. Use
`--until-end --frames 35000` for this recording and require `ended` in
both reports before comparing the traces.

The playable build also matched the original trace through 1,500 frames
for 83649, 82579, 82831, 83683 and 88377. This brings the playable
comparison to 46,161 paired frames across seven public recordings. The
playable links used the working-tree zero-fill placement update.

## 2026-11-25 Lunatic replay handoff check

[Lunatic Youmu 82996](https://www.silentselene.net/replays/th09/82996/download)
has SHA-256 `845cf93e93608faa95f5248cc036fa6d0f7f7b178fb9ef67967dcbcf7d5d3a82`.
Retail and the same playable image used for the Extra check consumed all
43,972 input frames over six stages and reached title registration.
Their 13-field traces are byte-identical. The fifth stage alone
contains 17,967 frames. Both used the stock archive and the same
configuration. The probe uses `--until-end --frames 46000` for this case.

## 2026-11-25 Windows practice launcher

The fixed-base playable image binds the local P1 collision dispatch to
`PlayerUpdateStageC` at `0x00419256`. Practice mode follows the no-hit
path at `0x00419277` after the collision query. The P2, replay, attract
and network cases continue through the original decision. The launcher
installs and reads back the patch while its own child is suspended.

Native Windows preflight verified the deployed PE and rejected an altered
copy. The ordinary and practice launches reached a 640 by 480 window with
DirectSound and an active WAV stream from `thbgm.dat` under isolated Wine.
With a controlled P1 collision result, the ordinary game changed health
and update state from `(10, 0)` to `(8, 4)`. The practice game kept
`(10, 0)` and reached the no-hit path.

The practice dispatch uses the original collision path for story mode,
stage index 8 and round index 0. The round count advances in
`FrontFactoryConstructView::SelectOpponentSide`. Read-back of the updated
launcher under isolated Wine confirmed the opening-round exception,
later-round protection and the existing P2, replay, attract and network paths.

## 2026-11-23 native DirectPlay startup and path storage

The host Windows installation reports its DirectPlay feature enabled.
A standalone 32-bit Peer test used the system `dpnet.dll` to host and
join a local session, producing four `CREATE_PLAYER` callbacks across
two peers. The same provider test in the isolated Wine namespace
returned `E_FAIL` from the built-in provider's `Host`. The Windows DLL
loaded in a disposable Wine prefix but Peer activation returned
`0x80070005` there. Loading the DLL alone does not supply working Wine
hosting.

Native Windows runs reached the title in retail, the fixed normal trial
and the reordered 0x00600000 trial. All three instantiated a Peer whose
`Host` slot resolves to RVA 0xABC0 of the installed 32-bit `dpnet.dll`.
Before the layout change, the default startup title in the same UNC
directory left a truncated executable-directory prefix in retail and
the normal trial. They used port zero and joined that prefix to their
score and configuration filenames. The reordered trial kept the full
prefix and read the port from the same `IP.txt`.

The startup file check uses the shared path buffer before game
configuration is loaded. Retail keeps the executable directory at +0x108
from the resolved-path buffer. The trial data declaration now keeps the
two buffers and their aliases in a single 0x20C-byte range. The layout
verifier rejects the previous split and passes both fixed links and the
four base variants. The selected WinMain, ResolvePath and network config
COFF bodies still match retail across 2,120 bytes and 178 relocations.

After relinking, the default-title runs reproduced the same original
prefix and port behavior in all three images. With a short explicit
startup title, all three kept the full prefix and loaded port 34,872
from the private fixture. All six native startup runs closed normally.
The game window was visible with the short title, but the automated
Windows launch did not acquire foreground focus. The native game
session never reached Match Mode. The successful join above applies
to the standalone provider test.

## 2026-11-23 native retail Match Mode host

The inactive test desktop did not reach the selectable title menu.
A later native retail run used the existing Win32 keyboard option in
a private configuration. The test driver attached only to the launched
game's verified window thread and fed its keyboard-state table. It
entered Match Mode through the title, selected the network option and
reached host wait on port 34,872 with a nonzero local DirectPlay player
ID. No remote player had joined when the game closed normally.

Headless COFF comparison verified eight units in the selected carriers.
They cover mode selection, Peer initialization, device and host
addresses, Host, Connect, message handling and network service. All
3,906 physical bytes and 255 relocations match their original extents.
Native trial hosting and two-game synchronization remain open.

## 2026-11-23 PE entry and CRT scope bindings

Both fixed trial layouts and all four base variants enter the pinned
VC7.1 `wincrt0.obj` through their PE headers. The linked verifier checks
the GUI subsystem, the full 469-byte entry in executable file-backed
storage and each of its 37 COFF relocations. Startup calls the mapped
`_WinMain@16`. The import operands select the same IAT identities as
retail. Two more relocations in the private 12-byte SEH scope table
bind the entry's filter and handler in each image.

In-memory controls in the fixed normal and reordered 0x00600000 images
were rejected for a redirected PE entry, a different subsystem, a
non-executable entry section, a redirected WinMain call, a swapped
scope reference, a live but different import slot and a changed
instruction byte. All checks ran headlessly against the existing files.

## 2026-11-23 ECL carrier split and polar call context

The four public EclManager callbacks with selected-carrier differences
use no-duplicates COFF COMDATs in both profiles. `MoveRandomBiased`
has internal linkage and is called by `RunEcl` inside the same carrier.
The two complete objects therefore need a partition that accounts for
both the public definitions and the private calls.

Omitting the Type29 draw include from a private `/Ob1` carrier preserved
all 80 units assigned to that profile. Compiling Type29 in its own `/Ob0`
carrier with the existing vertex declaration produced 2,239 bytes.
The first added instruction reloads ECX before the `Float3::operator+=`
call after `SetPolar2D`. The set of call and literal symbols stayed the
same. Making the existing `SetPolar2D` definition visible again restored
all 2,220 original bytes and 129 relocation fields.

With `SetPolar2D` left as an external declaration in the `/Ob1` carrier,
55 of its 80 units remained exact. The moved helper was absent as expected.
Twenty-four other units changed code extents or relocation offsets,
including ECL post-movement, effect drawing, ExAttack and player position
callbacks.

VC7.1 reproduces the five previously differing bodies in a single `/Ob1`
carrier when `#pragma inline_depth(0)` applies only to their definitions.
Each scope restores the default before the next definition. This keeps
the existing `SetPolar2D` definition visible and preserves the compiler's
private calling conventions. A cold replay matches all 86 units, 49,353
physical bytes and 2,084 relocations, including the full RunEcl body.
The exact manifest and the trial link select the same compiler profile.
Both fixed layouts and the four base variants pass the linked code and
callback-table comparisons with that carrier. The links have no duplicate
or unresolved symbols.
The original translation-unit partition remains unknown.

## 2026-11-23 linked data permissions and bulk storage

The product data check now compares the linked PE section's read, write
and execute flags with the corresponding COFF input section. It also
checks declared extents for both 0x194-byte `JOYCAPSA` records, the
0x80-byte button buffer shared by the input clear and copy, and all
three 0x2A4-byte loading VMs. Regenerating both data objects left their
assembly byte for byte identical. The 12 canonical consumers of these
ranges and the enemy schedule still match through all 332 relocations
in the selected carriers.

The source-defined enemy schedule at retail 0x004A7C88 has 256 zero
bytes. Its selected EclManager COFF symbol owns a complete 256-byte
`.bss` section. The two fixed PEs place the schedule at 0x004A8010
and 0x004E2898 in writable zero-fill storage. The check verifies its
initial contents, full mapped extent and lack of overlapping symbols.

Both fixed layouts and all four base variants pass. The normal and
high-base reordered checks rejected cleared data write permission,
an executable read-only contribution, a shortened schedule object,
an overlapping schedule symbol and a one-byte short allocation for
each of the three declared buffer extents. All checks ran headlessly.

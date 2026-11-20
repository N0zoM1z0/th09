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
- UpdateChargeAttack is 1,210/1,210 bytes with 183 differences. The target
  reads the 32-bit Player side index into EDI and derives the ReplayInputState
  pointer in EBX using the 0x8E stride. The current short local puts the
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
  filter +0x20; X87 radius compare +0x3B; circle calculation +0x4D; shared
  hit dispatcher +0x7E; zero-radius angle branch +0xAB; rotated rectangle
  +0xBD; AABB +0x13B; next-region cursor advance +0x19E; cancellation exit
  +0x1A6; direct-hit count/return +0x326. Rectangle/AABB success returns to
  the +0x7E dispatcher. A nested-loop experiment still emitted the dispatcher
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

`PlayerLifecycleView::UpdateMovementAndOptions` was revisited at its four-byte
configuration/side-index load-order residual. A real inline
`GameConfiguration::GetSideFocusMode` member accessor and explicit pointer
addition for the two-byte `sideFocusMode` row both compiled to the same EAX/ECX
load order. Both source changes were reverted; the maintained 1,900-byte
candidate remains at four differences.


## ChargeAttack entry-register and timer-reuse probes (2026-11-19)

IDA's `UpdateChargeAttack` entry is precise: EDI holds the full DWORD
`sideIndex`, EAX forms `sideIndex * 0x8E`, and EBX receives the
`ReplayInputState` record pointer. EDI remains the side value for the two
configuration-row reads, then is reused for the `timer303A4` receiver after
those reads; EBX is reused for `timer303B0`. The maintained source currently
reverses the initial pair (EBX=side, EDI=input record) and remains at
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

All variants were reverted. The next useful attempt must retain the target
EDI/EBX assignment through the initial input/configuration checks while
naturally ending the side value's live range before the timer receivers reuse
EDI/EBX. Do not use register hints, volatile aliases, inline assembly or
padding.

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

A `GameConfiguration *configuration = g_GameConfiguration` snapshot in
`UpdateMovementAndOptions` did not sequence the configuration load ahead of
`player->sideIndex`: VC7.1 still emitted the side load first. The four-byte
residual at +0x127..+0x12A remains unchanged, and the local snapshot was
reverted.


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

The next charge attempt should preserve the baseline 1,210/183 control-flow
layout while finding a way to end the DWORD side local's live range before the
cooldown receiver reuse. Do not copy the 1,202/503 experiment into maintained
source.

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
  variants still produced 1,209/1,210 bytes with 717 differences. They were
  reverted; keep the 1,210/1,210-byte, 183-difference baseline.

These are bounded compiler experiments, not proof that the natural C++ search
space is exhausted. Do not add register hints, volatile aliases, handwritten
assembly, padding or synthetic bytes to force target instructions.


## 2026-11-20 ScreenEffect shake-envelope native record

`ScreenEffect`'s three-DWORD shake-envelope parameter window is now named
`ScreenShakeEnvelopeView`. Its `fadeInFrames`, `holdFrames` and
`fadeOutFrames` members occupy +0x00/+0x04/+0x08 in a 0x0C-byte record;
the enclosing union remains at `ScreenEffect` +0x1C and preserves the
existing `rawParameter0/1/2` aliases. Compile-time size and member-offset
assertions keep the native layout explicit. This is a source-type recovery,
not a new exact-function claim: `CalcShakeEnvelope` remains 336/336 bytes
with 26 differences. Typed timer/owner aliases were also probed against its
VC7.1 output and rejected when they expanded the function or changed many
more instructions.

The header change passed the focused changed-source replay across 21 compiler
carriers: 100 exact units, 40,016 physical bytes and 2,487 relocation fields.
The pinned compiler regenerated 79 local COFF label spellings in Ending,
FrontMessage, ScreenEffect and TitleScreen carriers. Each was rebound only
after confirming the relocation's original PE destination and the candidate
COFF symbol's same-section owner-relative location; all 100 units then passed
the canonical comparator. No full cold replay was run, and the authored exact
count remains 1,026.


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

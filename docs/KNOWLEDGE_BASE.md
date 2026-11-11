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
establishes exactness. Dated paragraphs below preserve the *observations
from that point in the investigation*; any statement describing a "current"
candidate is historical unless confirmed in the active handoff. In particular,
GameplaySetupThread and Title DrawResult are exact; the 12 active nonexact
owners and the unfinished product/runtime gates remain separate.

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

## 2026-11-10 native variable-length ECL argument views

The 0x0C-byte TH09 ECL instruction prefix ends in an actual variable-length
sequence of four-byte union arguments. `arguments[0]` preserves the zero-size
native prefix and target layout, but C++ zero-length array indexing has no
ordinary in-bounds object semantics. `RawInt`, `RawFloat`, `RawByte`, and
`RawShort` now form their lvalue addresses as native unsigned i386 words,
adding `offsetof(arguments)` and the byte/word operand displacement. Byte and
halfword accesses address that same four-byte sequence without an invented
second payload. No decoder bounds checks or changes to retail error handling
were added, and all four accessors remain lexical/inlined rather than invented
external owners.

Seven isolated and combined pinned VC7.1 source probes kept the whole 15,564
byte RunEcl owner, its 598 original fields and all tables at the same physical
locations, with the same 30 codegen differences in opcodes 4 and 155. The
maintained combined source was then cold-rebuilt through its six actually
affected compiler carriers. Internal VC7.1 `$L` labels were renumbered at 20
locations in `CompareOperands`/post-ECL movement and 291 more in the separately
compiled ECL resolver leaves. Each one was checked against the exact COFF
same-section definition, real owner-relative label destination, addend, and
original PE field before updating the name in the exact-unit manifest. Final
**scoped cold replay passed all 91 exact functions, 36,301 physical bytes and
1,783 original-bound relocation fields**. This is a source semantics improvement;
it does **not** promote the RunEcl boss to exact or change the 1,024-function
exact ledger. RunEcl opcode-155 reverse-parameter, bitfield, old-flag lifetime,
and mutating-helper trial variants were separately compiled; they were neutral
or caused extensive table/branch regressions, so were not installed.

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
positions, addends and original target operands. Exact ledger remains 1,024;
none of these useful source corrections is incorrectly promoted.

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

## 2026-11-10 CompareOperands signed ECL back-jump addresses

`Th09EclRunControl::CompareOperands` (`0x00407700`, full canonical extent
892 bytes) advances from its current instruction on a successful comparison
using a signed 32-bit wire displacement. Its previous `unsigned char * + int`
C++ spelling could perform out-of-object pointer arithmetic for backward
branches. The maintained source now adds the operand's modulo-32 bits to the
instruction's unsigned native address, then reconstructs the typed pointer;
no new branch or error policy is introduced. With pinned VC7.1 on both actual
`EclManager.cpp` profiles the canonical `CompareOperands` remains **892/892
exact**, and the complete two-carrier scoped cold replay passes **83 existing
exact owners, 29,856 physical bytes and 1,389 original-bound relocation
fields**. The complete RunEcl boss keeps 15,564 physical bytes, 598 fields,
all tables and the same 30 nonmatching code bytes. Enemy OnUpdate keeps its
3,900-byte/97-field/13-byte frontier. This is native-source semantics
progress, not a new exactness claim.

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

## 2026-11-10 Laser and shot wire payload address arithmetic

The remaining native ECL laser and shot decoders shared the same zero-length
`Th09EclRawInstructionHeaderView::arguments` byte tail as RunEcl's basic
operand readers. `EclRunBullet.inl` now builds laser float/int/color views
from an unsigned i386 instruction address plus the reviewed `0x0C` operand
offset and the requested byte displacement; the laser creation handler
uses the same tail address for its structured packet. `EclPostRuntime.inl`
reads the separate shot instruction descriptor from that native byte address.
The new names and types remain transparent views of the existing binary
payload, not new allocation/decoder owners or altered original error handling.

Source-cold full RunEcl (15,564 physical bytes, 598 independently bound fields
and complete dispatch tables) still differs only at opcode 4 (seven bytes)
and opcode 155 (23 bytes), with no new field/table differences. Enemy OnUpdate
preserves its 3,900-byte/97-field/13-byte frontier. The five affected real
compiler profiles cold-replayed **87 exact owners, 30,583 complete physical
bytes and 1,409 relocation fields**. The independent target-bound physical
proof found no changed local label destinations or byte differences. The
change is native-address semantic reconstruction, not newly exact code.

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

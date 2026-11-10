# TH09 useful findings

This file keeps conclusions and useful failed approaches for the original
Japanese TH09 v1.50a and pinned VC7.1 compiler. Current exactness comes from
`config/matches.csv` and complete replay, not an old experiment result.
Use [the handoff](RE_HANDOFF.md) for the active owner table and
[generated progress](PROGRESS.md) for totals. Git retains previous source and
investigations; obsolete local outputs and archives are no longer retained.

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

## 2026-11-10 ECL and Enemy native address-word arithmetic

Original TH09 advances ECL instructions through i386 32-bit address arithmetic:
`nextOffset06` is sign-extended from 16 bits, and the jump displacement is a
signed 32-bit word. In the maintained RunEcl source, three next-instruction
paths now add those bits to the instruction's unsigned 32-bit address before
reconstructing its typed pointer. `EnemyManagerView::OnUpdate` also forms the
native draw-list head address with the unsigned group byte times a four-byte
pointer stride, rather than indexing beyond a C++ array's nominal extent.
These are source-level Win32 semantics corrections, not claims of matching
original C++ spelling.

Current-source VC7.1 complete-owner checks retain RunEcl at 15,564 bytes,
598 independently bound fields and the same 30 ordinary differences (only
opcodes 4 and 155). Enemy OnUpdate retains 3,900 bytes, 97 bound fields, 69
calls and the same 13 differences. A selected two-carrier cold replay
preserved all **83 exact sibling units**, **29,856 physical bytes** and
**1,389 original-bound relocation fields**. Eight renamed compiler-private
`$L` labels were independently checked against their actual COFF section
positions and original PE destinations before updating their manifest names.
No new exact function was admitted.

Natural opcode-4 timer-lifetime, opcode-155 masked-flag, SaveReplay frame-copy,
and Enemy draw-index/descriptor-copy source trials failed to improve their
complete original-bound residuals, so none was installed. The later focused
Effect AddedCallback receiver-lifetime trials were also either byte-neutral
(eight original differences) or regressed to eleven.

The opcode-4 relative JUMP now likewise adds the signed instruction displacement
as an unsigned i386 address word; the prior `unsigned char *` addition could
leave the bounds of the instruction object for backward ECL branches. That
lexical ECL change was compiled in all six actual affected carriers, including
the standalone EclOperands/EnemyManagerClear leaves. **91** complete canonical
units cold-replayed exactly across **36,301** physical bytes and **1,783**
relocations. VC7.1 renumbered **299** same-section `$L` definitions in six
owners; each changed name was rebound only after checking actual COFF offset,
addend, owner-local destination and the corresponding original PE operand.
RunEcl still has the same 30 opcode-4/155 ordinary differences, including all
598 target fields and tables; Enemy OnUpdate still has 13 ordinary differences
and 97 fields. This is native integer-address semantic progress, not a new
exact owner or whole-product closure.

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

## 2026-11-10 Enemy OnUpdate native death-frame wraparound

The death branch in `EnemyManagerView::OnUpdate @ 0x00410730` computes the
signed seconds until its boss-timer callback from the threshold and the
current `ZunTimer` value. Retail i386 subtracts these two DWORD values
**modulo 2^32**, then performs signed division by 60. The former C++ signed
subtraction could overflow before division. The maintained source now
performs unsigned native-word subtraction and interprets its result as a
signed frame count before applying the same divisor. The independent timer
conversion, all lifecycle branches and original return behavior remain
unchanged; the `sideTimer2AC3C8` and `frameTimer2AC404` fields are **real
`ZunTimer` objects**, not scalar counters, and their exact overloads were
preserved rather than replaced by naive integer increments.

Four pinned-VC7.1 source-cold versions (inline, intermediate native word,
operand snapshots and signed intermediate) maintained the complete original
owner shape. The installed version preserves **3,900/3,900 physical bytes,
97 independent fields, 69 calls, complete flow/switch and 13 ordinary
nonmatching bytes**. Its shared RunEcl definition remains a full 15,564
byte/598-field owner with only its previous 30 nonmatching bytes and no
table changes. Two actual affected compiler carriers cold-pass **83 exact
functions, 29,856 physical bytes and 1,389 relocation fields**. VC7.1
renumbered two same-section `$L` symbols; their PE addresses and owner-local
COFF definitions were checked before updating their manifest names. No new
exact credit or whole-product build claim follows.

Separately compiled direct vector-copy/index variations in the Enemy
spawn/draw path kept the same 13 residuals when they preserved original
literal provenance; attempts to force individual component copies changed
constant field positions. Player charge `short`->`int` index tests and
mode-byte lifetime variations likewise regressed 1,210-byte owner codegen,
so those unrelated candidates were not installed.

## 2026-11-10 Player collision native side/reward/hit-count words

`PlayerLifecycleView::CheckBulletCollision @ 0x0041DFF0` uses ordinary i386
DWORD subtraction, multiply and increment instructions for three related
operations: choosing the opposite player side from `1 - sideIndex`, computing
a reward of `ownerState30410.state38 * 10` on the lower-score branch, and
incrementing `PlayerCollisionRegionCreateRegionView::hitCount2C` on either
successful hit path. The former C++ signed arithmetic could overflow even
though the original instructions wrap modulo 2^32. The maintained source
uses unsigned 32-bit word operations for those steps, with an explicit
signed interpretation at the original destination. No new collision, reward,
score or error-handling branch was introduced.

Seven independent and combined source-cold variants under pinned VC7.1
preserved the complete existing 849-byte candidate with all **26 native
relocations, 12 calls, 32 candidate CFG blocks and 799 overlapping byte
differences** against the independently SHA-attested 862-byte/33-block
original. The three actual affected compiler profiles then cold-passed all
**five existing exact siblings**, with **1,220 physical bytes and 32 complete
relocation fields**. This is semantic fidelity progress in a large nonexact
owner; its missing 13 bytes and 33-versus-32 target block issue remain open.

Original-to-candidate x86 analysis explains why generic source reordering
has not fixed that boundary: retail preserves `bullet` in EBX at the
prologue and places the shared collision-hit block after the circle test;
the present source holds `position` in EBX and schedules the hit block later.
Several real for/while/goto/early-hit source probes were rejected because
they kept the 849-byte shape or changed return/relocation inventory. An
early bullet-type snapshot expanded the body but changed when memory is
read, so it was not adopted as an original-source reconstruction.

## 2026-11-10 Player movement native focus-effect side word

`PlayerLifecycleView::UpdateMovementAndOptions @ 0x0041C170` chooses the
second focus-effect's side slot as `sideIndex + 2`, passed unchanged to the
existing exact `EffectManager::SpawnEffectInFixedSlot` callee. The original
Win32 `LEA/ADD` address-sized addition wraps modulo 2^32 even if the signed
source value overflows. The maintained source now forms the two-word sum as
native unsigned arithmetic before casting to the original signed call
argument. No focus condition, effect index, allocation, branch, or callee
signature was changed.

The pinned VC7.1 private probe and live complete owner rebuild retained all
**1,900/1,900 physical bytes, 1,835 code bytes, 66 original-bound fields,
23 ordered calls, 435 aligned instructions, and precisely 122 unlike
ordinary bytes**. This independently compiled PlayerMovement carrier has
no mapped exact units within the source itself; the official source-specific
cold-replay selector therefore correctly reports an empty exact scope. The
fully original-bound movement owner check supplies the relevant verification;
no whole-product cold replay or exactness credit is claimed.

Other new source-cold probes investigated the actual remaining register
transport rather than treating all 122 positions as independent: 24 focused
x/y coordinate clamp spellings were neutral or regressed, while fourteen
`Float3` result-copy representations increased unmatched full-owner bytes
from 122 to 318--547 despite preserving call/field identities; they were
not installed. Eleven different explicitly inline two-operand `ZunTimer`
assignment adapters for RunEcl opcode 4 also retained its full 15,564-byte
owner and original 30-byte residual, so none was adopted. A new natural
Player collision shape with an early shared hit block brought source length
from 849 to 854 against 862 retail bytes, but raised overlapping mismatches
from 799 to 817 and still had only 32 of 33 original blocks. It was rejected
rather than promoting an apparent length-only improvement.

## Player movement, charge and collision

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

## 2026-11-10 SaveReplay native serialized-byte arithmetic

`ReplayManagerView::SaveReplay @ 0x00420C60` now maintains allocation and
serialized payload positions as unsigned i386 words. Three `memcpy` destinations
are formed by modulo-32 address addition, rather than unchecked C++ pointer
subscripts outside an individual source object. The link's two-byte-per-frame
serialized length likewise multiplies native unsigned words, preserving the
original i386 shift/add results without C++ signed-overflow UB. Checksum and
obfuscation cursors cross adjacent wire fields in the 0x1EC-byte replay
record; native address-word advances preserve those exact locations instead
of incrementing a one-byte/one-word scalar subobject pointer out of bounds.
No error-handling or allocation behavior was inserted or removed.

Individual and combined signedness, buffer-expression, checksum and
obfuscation trials were genuinely compiled under the existing pinned VC7.1
profile. The installed final source retains **1,238/1,238** complete physical
bytes, **53/53** original-bound fields and **23** calls, with only the same
`+0xB7/+0xBB` destination-register differences. It remains **NONEXACT**.
`BeginPlaybackStage` retains its previous 538/540 candidate/target lengths,
32 bound fields and 39 overlap differences. Scoped two-carrier cold replay of
the final installed source passed **12 exact owners, 4,093 physical bytes,
232 relocation fields**; no new exact owner or whole-product claim follows.

## 2026-11-10 BeginPlaybackStage native frame-table row address

At `0x00420840`, the original `BeginPlaybackStage` computes the frame-row
address `replayData + 0x20 + stage*0x28` and tests that address, **not** the
pointer in `frameStart[0][stage]`. The maintained `frameStart[stage]`
expression happened to generate that same i386 `LEA`, but the true record
contains only **three C++ row arrays**, while replay stages are indexed 0..9.
That expression therefore has undefined out-of-bounds C++ array semantics
for stages 3..9. The new source forms the same native row address through
unsigned 32-bit arithmetic from `&frameStart[0][0]` and the actual row size.
It preserves the original (unusual) address test and adds no substituted
null-data-pointer check or defensive behavior.

Six independently compiled natural native-word formulations preserved the
complete owner. The installed version retains **538 candidate bytes against
540 original**, **32/32** independently original-bound fields, **five** calls,
twelve blocks, 39 overlapping differences and the two-byte missing tail.
SaveReplay retains its same-size two-byte frontier. Two actual ReplayInput
compiler carriers cold-pass **12 exact owners / 4,093 physical bytes / 232
relocations**. This is a meaningful native-source semantics correction;
`BeginPlaybackStage` is still **NONEXACT** and there is no new exact credit.

## 2026-11-10 LoadReplayData native linear-buffer addresses

`ReplayManagerView::LoadReplayData` at `0x004205E0` decodes a contiguous
replay file. The obfuscation pass starts at `compressedSize`, the checksum
starts at `obfuscationKey`, and both byte walks cross multiple serialized
header fields. Their previous C++ pointers were incremented past a single
scalar subobject; the maintained source now increments native unsigned 32-bit
addresses and reconstructs byte pointers only for each access. The LZSS
output append likewise uses the actual `randomPayloadByte` offset in the
allocated file buffer, rather than indexing past the one-byte struct member.
The saved `frameOffset[3][10]` and `fpsOffset[10]` words are reconstituted as
native base-plus-byte-offset pointers, not object-out-of-bounds `u8 *`
addition. All original signed file-size conditions, allocation and error
branches remain unchanged.

Four independent pinned-VC7.1 probes and their combined candidate matched
the **entire 591-byte original LoadReplayData** with **14/14 canonical
relocation fields**. The installed source then cold-passed both relevant
ReplayInput profiles: **12 existing exact units, 4,093 full physical bytes and
232 relocation fields**. The separate nonexact SaveReplay remains 1,238/1,238
with two differing bytes; BeginPlaybackStage remains 538/540 with 39
nonmatching overlaps and 32 bound fields. No new exact entry or whole-product
claim follows from the source-semantics correction.

## 2026-11-10 BeginPlaybackStage native input payload cursors

The genuine 0x20-byte `ReplayFrameDataStartView` header in each serialized
stream is followed immediately by 16-bit input samples. The original
`BeginPlaybackStage` stores pointers to those sample bytes, not to another
C++ frame-header element. The three maintained cursor assignments now form
the same native i386 32-bit `frame address + sizeof(*frame)` word rather
than using `frame + 1` outside a single header object and reinterpreting
that one-past pointer. The original order of the three cursor stores and
the FPS pointer store is unchanged.

Five full-source natural pointer forms and three independent single-stream
forms were source-cold compiled under the pinned no-EH VC7.1 profile. All
preserved the complete **538/540** nonexact `BeginPlaybackStage` extent,
**32** bound fields, **five** calls and **39** overlap differences. The
selected source then cold-passed both true ReplayInput carrier profiles:
**12 existing exact functions, 4,093 physical bytes and 232 relocation
fields**. The unrelated SaveReplay boss remains 1,238/1,238 with precisely
two differing ordinary bytes. No new exact ledger credit is claimed.

A separate set of 18 SaveReplay frame-header copy probes (typed structure
copy, explicit native pointer, inline serialization helpers, CRT copy
interfaces) did not eliminate its `+0xB7/+0xBB` EDI/ECX residual; `memmove`
changed original call/field inventory and was rejected. None of these
neutral or regressive alternatives was installed.

## 2026-11-10 Replay LZSS payload base and serialized payload copy

The `0xC0`-offset `ReplayDataView::randomPayloadByte` anchors a **variable
length contiguous replay payload**, not an independent one-byte C++ array.
`LoadReplayData` previously passed addresses of that scalar field directly
to `Lzss::Decode` with the complete packed/unpacked stream lengths.
`SaveReplay` likewise copied the entire `ReplayPayloadView` beginning at
that one-byte field. The maintained implementations now calculate the
actual payload addresses from each complete replay object base plus the
reviewed `offsetof(ReplayDataView, randomPayloadByte)` in native i386
unsigned address words, without changing the retail copy sizes, seed,
checksum, compressor, allocators or error paths.

Independent load, save and combined VC7.1 source-cold probes preserved the
entire canonical 591-byte LoadReplayData and its 14 relocation fields, and
the 1,238-byte SaveReplay boss with only the original two unlike register
bytes. The installed combined source cold-replayed both affected ReplayInput
compiler profiles: **12 exact functions, 4,093 physical bytes, 232 fields**.
The separate BeginPlaybackStage remains 538/540 bytes, 32 fields, 39
overlapping differences, five calls and the same CFG. No exact ledger
promotion or product-closure claim follows from this source-semantics fix.

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

## 2026-11-10 Title result native unlock and character cursor words

`TitleScreenView::OnUpdateResult @ 0x004266B5` updates the hidden-unlock
sequence in four separate arms and wraps a selected character index after
`MoveCursorHorizontal(14)`. The original VC7.1/i386 instructions perform
native modulo-32 integer arithmetic in both paths. The maintained C++ now
increments `g_TitleResultUnlockStep` via an unsigned native DWORD and applies
the same unsigned word add/subtract to `keyboardSelection` before signed
boundary comparisons. This preserves the original decision predicates,
byte-addressed character order and exact `SetCharacterCursorInactive` calls;
no input filtering, new branch or array-size assumption was introduced.

Independent unlock-only, selection-only and combined source-cold probes
retained **484 candidate bytes / 485 target bytes, all 34 original-bound
fields, nine ordered calls, full CFG and the baseline 218 overlapping
ordinary differences**. The installed source was then compiled across both
actual Title compiler profiles and the canonical source-specific cold replay
passed **41 exact units, 18,972 physical bytes and 953 original relocation
fields**. Nine compiler-private `$L` names in `OnUpdateStartMenu` were rebound
only after an independent full PE/COFF physical proof established unchanged
owner-local destinations, original operands, addends, and all other bytes.
The exact original-bound `OnUpdateStartMenu` (1,592 physical bytes, 107 fields)
remains intact. Title OnUpdateResult is still nonexact with one missing byte;
no new ledger exactness or native product closure is implied.

Additional signed-byte/volatile/reference experiments at result-selection
produced 483/485, 488/485 or 489/485 candidate extents or extra external
fields; they did not reproduce retail `mov cl` plus `movsx edi,cl` and were
not installed. The register-lifetime problem remains separate from this
native arithmetic correction.

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

## 2026-11-10 Gameplay setup native phase and round-count arithmetic

The gameplay setup worker and its independently exact `AdvanceTimedState`
leaf share native i386 **signed storage with modulo-32 arithmetic**. The
maintained source now advances the real `SetupStateBuffer::phase10` and
`GameManagerSetupLayout::counterFC` as unsigned address-sized DWORDs before
reinterpreting their results as signed counters. Stage-mode-one's
`500000u * (phase10 + 1)` likewise now performs the inner increment as a
native unsigned word, rather than permitting C++ signed overflow before its
already-unsigned multiply. The four fixed-mode round offsets (`counterFC +
1/3/6/10`) are likewise constructed as original i386 word sums. All mode,
threshold, score, allocation and failure branches remain unchanged.

Eleven individual/combined VC7.1 source-cold worker probes retained its
complete 1,682-byte candidate / 1,689-byte SHA-pinned target, 172 bound
fields, 35 ordered calls and 78 blocks, with 694 overlap differences and
seven missing bytes. Three separate phase-increment probes reproduced the
complete **88/88-byte `AdvanceTimedState` leaf with all eight original-bound
fields**. The installed combined code then passed its **two actual affected
compiler profiles and 11 exact units**, covering **2,991 complete physical
bytes and 243 relocations**. This preserves native arithmetic semantics
without new exactness credit; the worker's branch-layout/field scheduling
remains nonexact. Previous `switch`-sharing / `if/else` worker hypotheses
displaced code and did not resolve its remaining original branch layout.

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

## Gameplay setup, ScreenEffect, network and Effect callback

GameplaySetupThread remains 1,682/1,689 bytes, 172 fields and 694 differing
overlaps plus seven missing bytes. Original fixed-mode difficulty 2/3 paths
join difficulty 4's C8=22 publication; counter-derived paths bypass it.
The target field at +0x433 appears at candidate +0x524. Explicit goto sharing
and isolating the worker retained the baseline; nested conditions and post-switch
publication changed dispatch/fields. A helper left five extra calls absent
from the original and was rejected. Explore genuine case sharing and rate
chronology rather than a cosmetic rewrite.

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

# TH09 reconstruction session — 2026-11-09

## EnemyManagerView::OnUpdate: preserve the primary hitbox's third component

The 3,900-byte `EnemyManagerView::OnUpdate` owner was rechecked against the
original-bound IDA function at `0x00410730`. The current source still has
3,900 physical bytes, 97 relocation fields, 69 ordered calls and 170 direct
blocks. Its 13 ordinary differences remain confined to the previously
recorded draw-group index register and special-attack effect/descriptor
register and stack-home scheduling. No field or switch-table identity moved.

The primary hitbox at `EnemyView+0x2DBC` is a contiguous XYZ vector, not just
an XY pair: initialization writes all three components, trail collision
copies and scales the full `Float3`, and the damage call receives the same
three-float storage. Commit `05f3e3cb` records the field model and size guard.

Focused natural source controls for draw-head scope/reference spelling,
direct list insertion, descriptor pointer/member spelling, position copies,
and threshold aliases did not reduce the 13-byte frontier. Rejected
alternatives were restored; no register forcing, assembly, volatile or
literal/relocation guard relaxation was introduced.

## EnemyEclManagerView::RunEcl: opcode 4 and opcode 155

The original-bound `RunEcl` at `0x004086C0` is 14,792 code bytes and 15,564
physical bytes including its owned tables. The current linked carrier has
598 relocation fields and exactly 30 ordinary differences, with zero
field and zero table differences. The residuals are:

- Opcode 4 (`JUMP`): seven bytes in the `ZunTimer::operator=(int)` call-site
  argument/receiver register schedule. The target loads the raw integer into
  ECX and pushes it before loading the active timer receiver; the candidate
  loads the argument into EAX, loads the receiver, then pushes EAX.
- Opcode 155 (`SET_TIMEOUT_SPELL`): 23 bytes in the byte zero-extension and
  masked bit-24 update. The target uses `movzx ecx, byte [instruction+0x0C]`,
  keeps the old flags word in ESI, and computes the masked XOR through ECX
  and EAX. The current source uses a byte local and the same semantic masked
  XOR helper, but VC7.1 chooses a different register schedule.

Natural controls tested during this session included direct wire-member
expressions, int locals, shared switch locals, explicit timer operator calls,
timer pointer/reference aliases, byte and bitfield assignments, direct XOR
expressions, and helper parameter-width changes. Direct JUMP rewrites grew
the owner to 42 ordinary differences; several timeout rewrites changed
literal/field/table placement and grew the difference count above 2,000.
Those candidates were rejected and the maintained source restored.

At the previous committed checkpoint, opcode 155 represented the raw
four-byte argument as a Th09EclWireArgument local and consumed value.bytes[0].
That source form preserved the 15,564-byte physical owner, 598 fields, 30
ordinary differences, and zero field/table differences, but it widened the
source-level read beyond the byte consumed by the target. The follow-up
2026-11-09 probe below supersedes that source spelling with a one-byte copy;
neither form receives exactness credit.

## Focused validation

- `enemy-attached-effect-update`: 151/151 bytes exact.
- ECL main carrier: all 77 exact units passed; ECL Type11 carrier: all 6
  exact units passed.
- Enemy dependencies passed: draw implementation 1,758/1,758; bullet
  descriptor constructor 28/28; bullet pattern 203/203; positioned sound
  153/153; boss marker position 42/42; marker interrupt 32/32; marker state
  20/20.
- `check-boss-frontiers.py --owner enemy --skip-exact-siblings` passed with
  the unchanged 3,900/97/69/170 owner and the exact 82-byte collision leaf.

The broader enemy exact-sibling driver stops at the `anm-manager-execute-script`
comparison's local-label mapping output. This was reproduced with both
`EclRunState.inl` and `EnemyManager.hpp` temporarily restored to their
pre-session committed versions, so it is recorded as a pre-existing
comparison failure rather than attributed to the two source edits. All 77
EclManager exact units and all 6 Type11 units were independently compared
successfully on the current carrier.

No full-project cold replay was run. No partial-owner byte credit was
claimed for either large boss.


## Follow-up: byte-width and register-lifetime probes (2026-11-09)

The opcode-155 input is the byte at instruction +0x0C, not a four-byte
integer. The source now copies only that byte from the shared wire-argument
view before applying AssignTimeoutSpellBit to Enemy primary flags +0x337C:

    unsigned char value;
    memcpy(&value, &Th09EclRunState::RawByte(instruction, 0), sizeof(value));
    enemy->primaryFlags337C =
        Th09EclRunState::AssignTimeoutSpellBit(enemy->primaryFlags337C, value);

The focused pinned-VC7.1 build and inspect-ecl-complete.py report 15,564 bytes
/ 598 fields / 30 ordinary differences / zero field differences / zero table
differences. The non-crediting audit-ecl-timeout-bit.py independently decodes
both opcode windows: 12 instructions in each, eight instructions in each
flag-update prefix, and equal flag transitions for all 256 wire-byte values
across 12 representative 32-bit flag words (3,072 cases).

IDA's target-attested disassembly at 0x0040B770 shows the original order:
movzx ECX from byte [EBX+0x0C], load old flags into ESI, shift/XOR/AND bit 24,
copy old flags to EAX, XOR the changed bit, and store EAX. The current
candidate reads the correct byte and preserves the complete 30-difference
frontier, but its register/lifetime schedule is not exact: mov CL from the
wire byte, load old flags to EAX, then movzx ECX,CL. The original loads
directly into ECX and keeps old flags in ESI.

Several natural C++ alternatives were rebuilt and rejected by the complete
owner oracle. Direct RawByte locals/expressions, direct wire-byte locals,
pointer aliases, and direct XOR/bitfield assignments moved the owner to 2,048
ordinary differences, 242 field differences, and 26 switch-table differences.
The bitfield assignment has a superficially similar local sequence but a
different register allocation and instruction encoding. Changing helper
parameter order or forcing inlining did not improve it. The original four-byte
union snapshot remains rejected because it widens the wire read.

The remaining large-owner frontiers are unchanged: opcode 4 has seven ordinary
differences around 0x00408938; opcode 155 has 23 around 0x0040B770; and
EnemyManagerView::OnUpdate has 13 ordinary differences in its 3,900-byte
owner. Continue with focused carrier builds and complete diagnostics for only
the owner being changed; no full-project cold replay is needed.

## Follow-up: exact instruction evidence and three current boss frontiers

The opcode-155 audit now includes the exact bytes, mnemonics and operands for
both eight-instruction flag-update prefixes. The latest focused source-cold
build remains 15,564 physical bytes / 598 relocation fields / 30 ordinary
differences / zero field differences / zero table differences. Its target
prefix begins 0f b6 4b 0c (movzx ecx, byte [ebx+0x0C]) and loads the old
flags into ESI; the candidate begins 8a 4b 0c (mov cl, byte [ebx+0x0C]),
loads old flags into EAX, and then zero-extends CL. Both prefixes execute
identically for all 3,072 combinations in the independent decoded-instruction
audit. This is a byte-width/semantic result, not exactness credit.

A direct instruction->arguments[0].bytes[0] helper expression does generate
the target's first movzx ecx, byte [ebx+0x0C], but it changes the complete
RunEcl owner to 2,048 ordinary differences, 242 field differences and 26
switch-table differences. Union snapshots preserve the 30-difference owner
but generate a DWORD load followed by movzx ecx,cl. Direct member assignment,
explicit operator=, raw-pointer indexing and shared switch-local attempts
for opcode 4 grow the owner to 42 ordinary differences. Moving a union
snapshot to the JUMP displacement operand changes the receiver/argument
schedule again. None of these probes is adopted.

The seven-byte opcode-4 target/candidate schedule is now pinned more
specifically. At owner offset 0x277 (VA 0x00408937), the target loads
[ebx+0x0C] into ECX, pushes ECX, then loads [edi+0x2CE0] into ECX and
adds eight before calling the exact ZunTimer::operator=(int) at
0x00401500. The maintained union-snapshot source loads the operand into
EAX, computes the receiver, then pushes EAX. The called setter remains an
independently exact nine-byte leaf; only the caller's argument/receiver
register schedule is open.

A fresh EclManager carrier also rechecked the 3,900-byte
EnemyManagerView::OnUpdate boss: 97 bound fields, 69 ordered calls,
170 direct blocks, agreed reviewed control flow and switch destinations,
and 13 ordinary differences. The two draw-group-index bytes at offsets
0x19C/0x1A3 are ECX-vs-EAX allocation. The remaining 11 bytes at
0x252..0x2BD are side-effect receiver, threshold register and descriptor
XYZ-copy scheduling. Pointer-add/index locals, byte/int/register index
locals, the prior AsciiGameManagerSideView spelling, side references and
pointer aliases, explicit Float3 copy helpers, and position component/pointer/
reference/memcpy spellings were rebuilt. They did not reduce the 13-byte
frontier; the componentwise and explicit-copy alternatives instead changed
large parts of the owner and were rejected.

The current charge-boss source was also rebuilt from a clean object using the
reviewed player-on-update VC7.1 profile. Its fresh result is 1,210 target /
1,195 candidate bytes, 70/70 bound fields, 58 ordered calls, 53 blocks,
1,153 unlike overlapping bytes, 15 missing target bytes and no extra bytes.
The target's mode byte still has its three EBP-1 accesses at +0x55, +0x1C7
and +0x1DB; the candidate has none and keeps the value in BL. The target
keeps side index in EDI and replay-input receiver in EBX, while the candidate
uses the opposite register roles. The earlier 1,197/13 checkpoint is
superseded by this fresh 1,195/15 observation; no exact or fractional credit
is claimed. The 58 ordered calls and all 70 field bindings still agree.

Focused tests test_ecl_timeout_bit_semantics and
test_runecl_real_timer_opcode4 pass (18 tests). The timeout audit and only
the involved ECL/Enemy/charge carriers were run; no full-project cold replay
was run. The only accepted source change in this follow-up is the more
transparent, byte-exact timeout-prefix diagnostic; all exploratory C++
rewrites were restored.

## PlayerLifecycleView::UpdateChargeAttack — complete extent and CFG recovery

The 1,210-byte charge boss now emits its full target extent instead of the
previous 1,195-byte candidate. A fresh pinned-profile player-on-update
probe reports 1,210/1,210 physical bytes, all 70 relocation fields bound,
58 ordered calls, 350 instructions, 53 blocks, and matching direct control-flow
graphs. There are 235 ordinary byte differences, with zero missing or extra
bytes. The separate real 20-byte ZunTimer::operator> sibling remains exact.
This is still diagnostic-only; no partial or exactness credit is claimed.

The adopted source model uses a short-lived playable-side index and one
reused input receiver:

    short side = static_cast<short>(sideIndex);
    volatile u8 sideMode;
    u8 modeSnapshot;
    int previousLevel;
    ReplayInputState *input = g_ReplayInputStates + side;

The narrowing is justified by the actual creation path, not an assumed input
domain: GameManagerSetup.cpp creates players only inside the loop bounded by
GameplaySetupNativeSides::PlayableCount; that count is compile-time asserted
to be two, and the only PlayerRegisterChain call passes that loop index.
PlayerRegisterChain stores the same value in player->sideIndex. The charge
translation unit now carries the same two-playable-side compile-time guard,
and test_player_charge_native_boss_owner checks the creation path.

The residual mask now protects the 975 positions that match the target in
this complete-size candidate. Compared with the prior 1,195-byte candidate,
three previously matching byte positions (84, 128, 253) became residual during
the source-model rebase; this is explicitly recorded rather than silently
discarded. The new model gains the full 15 missing bytes and restores the
reviewed 53-block direct CFG. All future candidates must keep every byte
outside the 235-position residual mask equal to the original.

The remaining early-mode mismatch is tightly identified: target stores AL to
[ebp-1] at +0x55, while the candidate stores BL at +0x56; the subsequent
stack compare/read at +0x1C7/+0x1DB agree. Side/input register roles and the
initial branch encoding still account for most residual bytes. More natural
C++ register-lifetime and evaluation-order probes continue from this full
extent baseline; no assembly, forced register hints, or profile search was
introduced.

Focused validation:
- scripts/check-boss-frontiers.py --owner charge: passed; only the charge
  owner was rebuilt and inspected.
- test_player_charge_frontier.py: 5 tests passed, including protection of
  every currently equal byte and non-crediting residual shrink.
- test_player_charge_native_boss_owner.py: 2 tests passed, including the
  playable-side creation invariant.

## Fresh exact Title DrawResult and stale diagnostic guard cleanup

A new pinned-profile build of src/TitleScreenResultDraw.cpp into
build/matching/TitleResultDraw.obj was checked against the immutable original
at 0x00423D16. Its complete 1,939-byte physical owner and all 82 original
relocations are identical; the strict canonical comparator reports
matched_bytes = matched_compared_bytes = 1,939, result = exact. The standalone
original-bound diagnostic independently confirms 577/577 decoded instructions,
16 ordered calls, 92/92 CFG blocks, 15 literal definitions and zero difference
offsets. The DrawResult source had already converged before this work; it is
not a new exact-function promotion.

The previous noncrediting DrawResult frontier still allowed three historical
register-encoding differences at +0x1D, +0x23 and +0x39. That allowance was
stale relative to the current source and could mask future regressions. The
guard now requires zero differences and complete byte equality, all 577
aligned instructions and zero unpaired branch destinations. Tests include
the three former residual locations and other original bytes as regression
cases. The diagnostic still awards no credit by itself: the separately
invoked strict canonical full-owner comparison remains mandatory.

Focused validation: 64 tests in test_boss_frontiers.py passed;
check-boss-frontiers.py --owner resultdraw --skip-exact-siblings passed,
rebuilt only the selected DrawResult carrier and reported 0 differences.
No all-project replay was performed.

## Read-only result-score bank with unchanged exact machine body

The full ResultDraw owner uses the score-bank rows only as read inputs while
the score cursor advances across five rank groups. Its local now initializes a
const pointee directly from the existing native title bank index:

    const TitleScoreRecordView (*scores)[5] = g_TitleScoreTable[nameBankIndex];

This is a natural C++ const-correctness improvement; it does not establish
the original author's exact source spelling. A fresh fixed-profile build of
the updated TitleScreenResultDraw.cpp reproduces all 1,939 original physical
bytes and 82 source relocations. The canonical whole-function comparator
returns exact; the independent full-owner inspector reports zero differences,
577 instructions and matching direct graph. Both focused native Title owner
tests passed. Neither compile flags nor the original target changed.

## Replay SaveReplay: typed full-frame serialization extent

In the 1,238-byte SaveReplay boss, the first streamed frame-header copy now
takes its count from the pointed-to native ReplayFrameDataStartView object,
rather than repeating that type's name in a separate sizeof expression.
ReplayDataOriginalView.hpp independently guards the 0x20-byte frame layout,
and the link chain remains const/read-only throughout serialization. This is
a source correctness/readability improvement, not a reconstructed original
authoring claim.

The focused no-EH Replay carrier rebuilt cleanly: all 1,238 owner bytes
remain present, all 53 target-bound fields and 380 instructions remain
reviewed, and only the preexisting two register-allocation bytes +0xB7
and +0xBB differ. The separate 540-byte BeginPlaybackStage remains at
538 source bytes, 32 bound fields, 39 unlike overlaps, two missing bytes
and matching direct CFG. Its 1,323-byte BeginRecordingStage sibling
continues to pass a full strict canonical byte/field comparison. Several
natural destination-pointer, typed struct-copy, helper and loop-lifetime
alternatives for the remaining two SaveReplay register differences were
rejected after whole-owner comparison; none was incorporated.


## Replay SaveReplay: align typed copy and cursor advance

Following the committed pointer-sized frame copy, the stream cursor now uses
currentOffset += sizeof(*frameHeader) instead of a separately spelled
sizeof(ReplayFrameDataStartView). The actual native frame-start pointer is a
32-byte POD record, so both expressions are type- and behavior-equivalent.
Keeping the cursor step tied to the copied object prevents accidental drift if
a future layout reconstruction adjusts that type.

The same focused no-EH carrier rebuild gives 1,238/1,238 SaveReplay bytes,
53 independently bound relocations and the same two register-only residuals
(+0xB7, +0xBB). BeginPlaybackStage remains 538/540, 32 fields, 39 residuals
and a matching CFG. This change is a maintained C++ source-model refinement,
not an exactness claim for either boss.


## Replay BeginPlaybackStage: source-level byte snapshot chronology

Target-bound instruction review resolves the earlier ambiguous comment:
original +0x151/+0x154 loads the header 1E8 byte into EDX, +0x15B
reads the side-0 runtime owner into ESI, and +0x161 publishes 1E8
to GameManager. Source now expresses the byte load ahead of the native
volatile pointer-slot access as a const u8 snapshot. The separate side-0
runtime accesses remain independent, as required by the target evidence.

This C++ source-model change has not repaired the optimizer's register
assignment. A focused no-EH Replay build still has a 538/540-byte
BeginPlaybackStage owner, 32 independently bound fields, 39 unlike overlap
bytes, two missing bytes, 5 ordered calls and matching CFG. The same carrier's
SaveReplay remains 1,238 bytes/53 fields with two register-only residuals;
the 1,323-byte BeginRecordingStage passes the strict exact comparator.
A source-input regression test preserves the relative snapshot/pointer/store
chronology without pretending that source order proves emitted instruction
order. No full cold replay or new exactness credit.

## Newly recovered ECL timeline/submanager native constructor chain

The original 0x0040FCA0, 28-byte special-member-like body constructs exactly
eight 0x0C-byte vectors beginning at the timeline-state object's +0x100.
IDA and a fresh target-bound PE disassembly independently identify the real
VC7.1 vector-ctor iterator at 0x00401470 and the shared/folded empty
Float3 constructor callback at 0x004343D0. No explicit call to the compiler
iterator is necessary.

EnemyEclTimelineStateView now declares the native constructedPositions100[8]
array and its ordinary empty C++ constructor in EnemyManagerLifecycle.cpp.
VC7.1 /O2 /Ob0 emits the exact 28/28-byte body at 0x0040FCA0 with both
original DIR32/REL32 operands. It also naturally emits the 15/15-byte
EnemyEclManagerView constructor at 0x00412090, which invokes the former
on its +8 timeline-state member. Both target byte hashes and independently
verified relocation roles are pinned in
scripts/inspect-enemy-ecl-constructors.py, a diagnostic-only proof.

The real EnemyManagerView constructor remains canonical exact at 101/101 bytes
with all nine fields, and EnemyManagerView::Create remains exact at
328/328 with all 29 fields. Both compiled from the same updated source.
The 3,900-byte EnemyManager::OnUpdate frontier remains at 13 ordinary
differences and 97 fields. The 15,564-byte RunEcl frontier remains at 30
ordinary differences and 598 fields, with zero field/table differences.
Only those impacted carriers were rebuilt. The original authoring status
of 0x0040FCA0/0x00412090 is still unknown; these special members remain
unclassified and receive zero authored exactness credit.

## Follow-on: native ECL interpolation-slot and context constructor chain

The original 0x004120C0 special member is an ordinary thiscall constructor
that calls the real ZunTimer constructor at member +0x04; the object is the
current 0x30-byte Th09EclInterpolationSlotView.

The original 0x004120D0 context constructor calls the exact ZunTimer
constructor twice, for members +0x08/+0x94, then invokes the actual VC7.1
vector-construction helper over eight 0x30-byte interpolation slots starting
at +0xA0, with callback to the 0x004120C0 slot constructor.

Adding normal out-of-line C++ empty constructors for these two existing
native-sized structures makes the same compiler naturally generate the exact
15/15-byte slot ctor (1 REL32 field) and 47/47-byte context ctor (4 original
address fields). Neither the layout nor the compiler profile was altered.
The original target SHA-256 and each operand role are now independently
checked as part of the four-owner diagnostic constructor-chain script.

Focused regression: the 101-byte EnemyManager constructor and 328-byte
EnemyManager::Create remain strictly canonical exact; the 3,900-byte
OnUpdate stays 13 ordinary differences / 97 fields, and the 15,564-byte
RunEcl stays 30 ordinary differences / 598 fields with no field/table drift.
All three constructor-chain and 35 ECL native storage unit tests pass.
The two new constructor bodies at 0x4120C0 and 0x4120D0 remain origin-unknown
special members in functions.csv; matching their bytes does not establish
explicit authored source and grants no exactness credit.

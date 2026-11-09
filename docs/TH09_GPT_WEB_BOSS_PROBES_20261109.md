# TH09 boss reconstruction probes — 2026-11-09

This is a fresh focused compiler/PE investigation against the maintained
pinned-VC7.1 sources. It records both negative and positive observations; it
does not grant exactness, runtime, product, or Factory credit. No full-project
cold replay was run.

## `GameplaySetupThread` — 1,689-byte initialization worker

A fresh `game-manager-on-update` build and original-bound
`inspect-gameplay-setup.py` report reproduce the current frontier:

- target/candidate physical bytes: **1,689 / 1,682**;
- target/candidate instructions: **413 / 413**;
- original/bound address fields: **172 / 172**;
- ordered calls: **35 / 35**, all agreeing;
- direct blocks: **78 / 78**, graph not agreeing;
- unequal overlapping bytes: **694**, seven missing target bytes.

The one field-order anomaly remains the native `valueC8` word at
`0x004A7E58`: the target's decoded field is at owner `+0x433`, while the
candidate's is at `+0x524`. In the original, the fixed-mode value `22` is
published in the case-4 block at `+0x431`; the fixed-mode case-3 and case-2
paths branch back to that publication. The current switch lets VC7.1 merge the
three equal source assignments into a later common block. A natural explicit
label/goto model placed the common assignment immediately after case 4 and
sent the case-3/case-2 fixed paths to it, but a fresh build retained the same
1,682-byte/694-difference frontier and the same branch graph. This model was
not adopted. The exact original source-level sharing remains open.

## `EnemyManagerView::OnUpdate` — 3,900-byte manager callback

The fresh baseline remains **3,900 physical bytes / 3,883 code bytes**,
**97 fields**, **1,026 instructions**, **69 ordered calls**, and **170 direct
blocks**. All reviewed field identities, call identities, switch-table
destinations, and control-flow checks continue to agree. The 13 differing
bytes are exactly:

`+0x19C +0x1A3 +0x252 +0x263 +0x26D +0x272 +0x294 +0x29A +0x29C +0x2A5 +0x2A9 +0x2B7 +0x2BD`.

Fresh natural-source builds tried draw-group index locals and base-plus-index
forms; side-state pointer/reference aliases; actual `AsciiGameManagerSideView`
aliases; effect-manager pointer/reference aliases; and `Float3` position
pointer/reference/value forms. The ordinary aliases leave the same 13-byte
frontier. Moving the spawn-side reference ahead of the position copy changes
the scalar literal positions and expands the raw byte difference to 50; the
strict literal-owner check correctly rejects it. That rearrangement is not
adopted. A `RunEcl` timeout-bitfield assignment probe was also rejected: it
changes the complete 15,564-byte interpreter body from the baseline 30
ordinary differences to 2,140 differences. The existing masked-word source
remains authoritative.

## `ReplayManagerView::SaveReplay` — 1,238-byte replay writer

A fresh no-EH `ReplayInputNoEh.obj` build and the original-bound guarded audit
reproduce **1,238 bytes, 380 instructions, 53 fields, 23 ordered calls**, with
only two unequal ordinary bytes at `+0xB7/+0xBB`. The target uses
`mov edi,[ebp-0x10]; lea edi,[edi+ebx-0xC0]`; the candidate uses
`mov ecx,[ebp-0x10]; lea edi,[ecx+ebx-0xC0]`. All 53 relocation positions and
all 23 call positions agree.

New focused variants included local destination cursors, indexed and typed
header copies, signed/unsigned and literal offset arithmetic, mutable/const
header pointers, eight-word copies, and local declaration-order changes.
They all retain the two residual bytes. Volatile cursor variants are rejected
by the original-bound guard. No source model is adopted and no comparator
exception is added.

## `ReplayManagerView::BeginPlaybackStage` — 540-byte stage callback

Fresh focused builds retain **538 / 540 bytes**, **152 / 151 instructions**,
**32 / 32 bound fields**, **5 / 5 ordered calls**, and **12 / 12 direct blocks**;
there are 39 unequal overlapping bytes and two missing target bytes. Further
union, two-half struct, pointer/reference receiver, and zero-plus-`memcpy`
seed-local forms all retained that frontier. The target's
`xor edx,edx; mov dx,[eax+4]; push edx` seed schedule remains unresolved; no
register or ABI forcing was introduced.

## Scope and next experiments

All probes rebuilt only the touched compiler carrier and ran the matching
owner-specific diagnostic. Source files were restored after each rejected
variant. The repository is left with no uncommitted production-source model
from these probes. Current exactness totals and original-owner boundaries are
unchanged. The next useful attack is the setup worker's case-sharing/control
flow and the Enemy callback's live-register/source-expression schedule, not
another cosmetic pointer alias or a whole-project cold replay.

## `TitleScreenView::DrawReplayMenu` — 1,280-byte replay-menu boss

A fresh standalone build and original-bound
`inspect-title-replay-menu-draw.py` reproduce **1,280 bytes**, **376
instructions**, **57 bound fields**, **13 ordered calls**, and **72 direct
blocks**, with **29** unequal overlapping bytes and no missing/extra bytes.
The residual offsets are:

`+0x185 +0x18B +0x193 +0x19D +0x19E +0x1A0 +0x1A1 +0x1B6 +0x1BC +0x1D3 +0x25A +0x266 +0x26A +0x271 +0x272 +0x29D +0x2A3 +0x2C6 +0x2D7 +0x2F0 +0x2F5 +0x2FD +0x304 +0x308 +0x316 +0x319 +0x31C +0x31E +0x321`.

The decoded residuals form a consistent register-allocation family around the
selected replay row and state timer. The original carries the selected index
in EDX and the timer value in ECX; the current candidate carries the selected
index in ECX and the timer in EDX. This swaps the local-store order at
`+0x193..+0x19E` and propagates to row-coordinate, name, difficulty and mode
formatting. Fresh selected-local declaration order, function-scope local,
reference/pointer, const/mutable, signed-width, and timer-reference probes did
not reduce the 29-byte frontier. Some alternate lifetimes changed the owner to
1,271 bytes or 50/85/860 differences and were rejected. No source model was
adopted.

The first context probe was stale: it expected `TitleSetupThread`'s static
member declaration inside `TitleScreen.cpp`, while the real declaration now
lives in `TitleScreenReplayView.hpp`. The probe was repaired to use the current
header and to append the complete menu source (including its own native SDK,
layout and slot declarations). A fresh three-context run then compared the
standalone menu, the real TitleScreen declaration prefix, and the complete
TitleScreen native TU. All three independently produced the same raw function
and COFF fields: **1,280 bytes / 376 instructions / 57 fields / 13 calls /
72 blocks / 29 differences**. Translation-unit context therefore does not
explain the residual register schedule. The repaired probe still grants no
exactness credit to other functions emitted by the native TU.

## Second focused sweep — current owner receipts and source-order controls

### Replay menu: repair the context probe, then prove the residual is TU-independent

The old `probe-title-replay-menu-context.py` expected a `TitleSetupThread`
member declaration inside `TitleScreen.cpp`. That declaration actually lives
in `TitleScreenReplayView.hpp`; the stale marker made the probe reject before
it reached the compiler. The probe was repaired to use the real current
TitleScreen TU prefix/header and the complete replay-menu source. This is a
probe correction only, not a source-code match change.

A fresh `--context all` run now builds three independent carriers:

| Context | Bytes / instructions | Fields / calls / blocks | Differences | Raw + field identity |
| --- | ---: | ---: | ---: | --- |
| Standalone | 1280 / 376 | 57 / 13 / 72 | 29 | baseline |
| Current Title declarations | 1280 / 376 | 57 / 13 / 72 | 29 | identical |
| Current full native Title TU | 1280 / 376 | 57 / 13 / 72 | 29 | identical |

All three have the same candidate raw SHA and exact 57 COFF field identities.
The 29 offsets are unchanged. This rules out the current standalone-vs-Title
TU split as the cause of the selected-path register differences; it does not
prove the historical retail TU partition.

A second natural-C++ sweep tested array pointer addition, typed byte-offset
lookup with both `sizeof(ReplayDataView)` and the reviewed `0x1EC` stride,
unsigned index casts, `+0` index expressions, pointer aliases, uninitialized
pointer/index declaration and assignment order, and local `playerName` /
`playTime` aliases. The byte-offset and array forms retained the exact 29-byte
frontier. Reordering or replacing the selected-index snapshot often changed
the local homes and shrank the owner to 1,264 or 1,271 bytes (371/375
instructions), creating 858–860 unequal overlapping positions; those forms
were rejected. Explicit formatting aliases either retained the same 29 bytes
or changed the real literal-reference inventory and were rejected by the
strict COFF audit. No source variant was adopted.

Compiler controls were also checked without changing the maintained profile:
`/O1 /Ob2` and `/O1` without `/Gr` retain the same 1,280/376/57/13/72/29
frontier. `/O2`, `/Os`, `/Ob0`, and `/Oi` change the actual literal-reference
inventory or owner shape; none is evidence for changing the established
Title `/O1 /Ob1 /Oy-` source profile.

The focused canonical frontier command passed:

```bash
python3 -B scripts/check-boss-frontiers.py --owner replaymenu --skip-exact-siblings
```

It independently rechecked 1,280 target/candidate bytes, 57 fields, 16 actual
literal definitions, 30 literal references, 13 ordered calls, 72 blocks,
identical direct graphs, zero missing target bytes, and 29 residual bytes.
Exactness credit remains **none**.

### Gameplay setup: inspect the actual shared store and reject the wrong branch models

The original disassembly pins the fixed `valueC8 = 22` store at owner `+0x431`
inside the case-4 block. The fixed-mode branches from cases 3 and 2 jump back
to that store. The maintained `/Os` candidate instead merges the equivalent
fixed-22 publication into a later common block at `+0x524`; the case-4 path
therefore reaches that later block rather than leaving the switch from its
original block. This is the concrete branch-layout difference behind part of
the existing CFG mismatch, not an unbound address or missing call.

New source controls put a backward `goto` label immediately before the
case-4 store and sent the fixed-mode case-3/case-2 branches to it, first in the
switch and then in split condition arms. Both retained the 1,682-byte,
413-instruction, 172-field, 35-call, 78-block candidate with 694 ordinary
differences, seven missing bytes, and a nonmatching direct graph. Rewriting
the full mode switch as a natural `if/else if` chain with the same backward
label compiled to 1,672 bytes / 413 instructions / 172 fields / 35 calls /
78 blocks, but expanded the physical difference to 702 bytes and 17 missing
target bytes. That rewrite is rejected; the maintained switch is restored.

The focused canonical frontier command passed and rechecked both the boss and
its exact siblings:

```bash
python3 -B scripts/check-boss-frontiers.py --owner gamemgr --skip-exact-siblings
```

Current receipt: `GameplaySetupThread` 1,689 target / 1,682 candidate bytes,
172/172 fields, 35/35 calls, 78/78 blocks, 694 ordinary differences, seven
missing bytes, and a nonmatching graph. In the same run,
`GameManager::OnUpdate` remains 1,230/1,230 bytes exact with 87 relocations;
`GameManager::CleanupGameplayState` remains 425/425 bytes exact with 43
relocations. The new if-chain/goto variants were not adopted.

### Enemy manager: fresh owner check

The focused canonical command also passed:

```bash
python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
```

`EnemyManagerView::OnUpdate` remains 3,900/3,900 bytes, 97 bound fields,
69 ordered calls, matching direct graph and switch destinations, with 13
ordinary differences. `AppendPlayerCollisionBox` remains 82/82 bytes exact
with two bound fields and its two original calls. This is a fresh current
receipt; the extra pointer/reference/local aliases did not change the boss
frontier.

No full-project cold replay was run. All rejected source experiments restored
the maintained source before the next build. The canonical owner checks above
rebuild only their selected carriers and deliberately grant no exactness to
non-exact owners.

## Third focused sweep — mixed switch branches, playback seed codegen, Enemy register allocation

### GameplaySetupThread: mixed branch forms move the frontier by a byte, but do not close the shared-store seam

All candidates below were rebuilt only from `src/GameManagerSetup.cpp` with
the reviewed `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Os /Ob1 /Oy-`
profile. The original PE remains the sole target. Every candidate retained
172 address fields and 35 ordered calls; every candidate decoded the same
78 direct blocks, but none had an identical direct graph.

- Inverting only the `case 2` mode test (`if (gameMode != 2) normal; else fixed`) while retaining the case-4 shared label and case-3 forward spelling produces **1,688 bytes / 413 instructions / 684 unequal bytes / one missing target byte**. This is ten fewer unequal bytes and six more candidate bytes than the maintained 1,682/694/seven-missing frontier. It is a genuine natural C++ spelling and remains a non-exact probe, not yet adopted.
- Inverting both case-3 and case-2 tests gives 1,685/413/693/four-missing. Explicit normal-path labels for both cases give the same frontier. Putting only the explicit normal-path label on case 2 is equivalent to the 1,688/684/one-missing result above.
- A nested `switch (gameMode)` in case 2 produces 1,689 bytes, 414 instructions, 374 unequal bytes and no missing bytes. However, it changes the target's `cmp [gameMode], edi; jne normal` into `mov eax,[gameMode]; sub eax,edi; je ...`, adds one instruction, changes the direct graph, and still places the shared fixed-22 store in a later common block rather than the target's case-4 block. It is not accepted as a source improvement merely because its physical overlap count is smaller.
- Nested-switch forms in case 3 or both cases, `do/while(0)` and `if(1)` label wrappers, a constant `switch(0)` label wrapper, and volatile lvalue casts do not improve the maintained branch/store topology. The volatile forms are explicitly rejected as anti-optimization hypotheses, not natural source candidates.

The decisive original seam remains: the retail case-4 block stores `valueC8 = 22`
at `+0x431`, and the fixed-mode case-3/case-2 paths jump backward to that
store. The maintained compiler candidate factors the same store into a
later common block. The new mixed case-2 spelling is promising for raw extent
and residual count, but it does not yet reproduce the retail branch destination
or complete owner. No production source or strict inspector gate was changed.

### BeginPlaybackStage: more seed-local and receiver-order controls

A fresh no-EH `ReplayInputNoEh.obj` baseline reproduces the complete 540-byte
original-bound diagnostic: 538 candidate bytes, 151 instructions, 32 fields,
5 ordered calls, 12 direct blocks, 39 unequal overlapping bytes and two
missing target bytes. The complete field inventory, call order and direct
control-flow graph agree; all field-change spans remain empty.

The target seed call tail is:

```text
xor edx,edx
mov dx,[eax+4]
mov ecx,0x4ACE0C
push edx
call SetSeed
```

The maintained candidate uses `movzx eax,word ptr [eax+4]; push eax; mov ecx,...; call`.
New `u32` zero plus low-half aliasing, a byte-addressed low-half alias, local
pointer/reference copies of the seed, and function-scope or block-scope RNG
pointer/reference aliases all retain the 538/151/32/5/12/39/two-missing
baseline. Making the low-half alias volatile expands the owner to 545 bytes,
153 instructions and 42 differences, and changes the accepted field/call
order; it is rejected. These results do not justify assembly or ABI forcing.

### EnemyManagerView::OnUpdate: source aliases do not explain the thirteen register bytes

Fresh whole-owner probes retain 3,900 bytes, 97 fields, 1,026 instructions,
69 ordered calls, 170 direct blocks, matching reviewed control flow and switch
destinations, with the same 13 differences at
`+0x19C +0x1A3 +0x252 +0x263 +0x26D +0x272 +0x294 +0x29A +0x29C +0x2A5 +0x2A9 +0x2B7 +0x2BD`.

New variants used effect-side pointer/reference aliases, explicit enemy-position
pointer/reference aliases, both alias groups together, mask-before-shift side
category arithmetic, direct repeated side-category expressions, a narrowed
side-category scope, local declaration reordering, and a `register` draw-head
pointer. The strict audit retains the same 13-byte residual for all ordinary
forms. A byte-sized side-category changes the actual literal definition/
reference inventory and is rejected before byte comparison. No source change
is adopted and no literal guard is relaxed.

No whole-project cold replay was run. All rejected source probes restored the
maintained source and the canonical object was rebuilt after the experiments.
The current non-exact owner statuses and exactness ledger are unchanged.

## Fourth sweep — native IDA pseudocode cross-checks and the limits of decompiler structure

The original-bound `th09-ida` provider was used to decompile the retail
functions directly, rather than treating earlier reconstruction notes as
source truth.

### `GameplaySetupThread`

IDA pseudocode independently exposes two local integer temporaries (the
values used for the normal-path `limitB4` and `valueC8` publications), fixed
mode exits for 20/22/16, and the shared fixed-22 destination reached by cases
2, 3 and 4. The pseudocode presents case 0 as an outer `else` and cases 1–4 as
a nested switch. The raw target dispatch, however, begins with a single
`setupMode` load, subtracts the zero base, branches to the mode-0 block, and
then uses the decrement chain for the other mode values. Therefore the
pseudocode's `if/else` presentation is not sufficient evidence that the
original C++ used two separate constructs; it may be Hex-Rays structuring of
the switch CFG.

A candidate with the decompiler-shaped outer `if`, case-1..4 switch, normal
path locals and a separate case-0 path compiles to **1,682 bytes / 413
instructions / 171 fields / 35 calls / 77 blocks / 692 ordinary differences /
seven missing bytes**. The single-switch form with local `nextLimit` and
`nextValueC8` temporaries reaches the same frontier. Neither reproduces the
retail 172-field / 78-block graph or relocates the fixed-22 store to the retail
case-4 block. Both are rejected as source changes. This demonstrates that
recovering the decompiler's temporary variables alone does not explain the
remaining codegen difference.

### `DrawReplayMenu`

IDA pseudocode shows the selected record pointer being acquired before the
selected-index snapshot, consistent with the maintained `const ReplayDataView
&` model. The ten stage-detail rows are represented in the decompilation as a
pointer starting at the first frame-start row (`replay + 0x20`), advancing by
four bytes per iteration, and reading the second side at `+0x28`.

New source experiments reproduced that representation using a typed pointer
increment and a post-tested `do/while`, plus a raw-byte pointer stepping by
`sizeof(void*)`. The typed pointer forms expand the original 29-byte frontier
to 63 differences; an indexed `do/while` retains the original 29. This is
useful negative evidence: the decompiler's pointer walk is not, by itself,
proof of the original C++ loop idiom. No pointer-walk variant is adopted.

### `BeginPlaybackStage`

IDA pseudocode confirms the maintained high-level dataflow: frame-start row
pointers are selected for both sides and the shared stream, replay header
fields are published, configuration is copied, then the RNG seed is passed
from the low word at `frame0 + 4`. The remaining two-byte deficit is at the
call-site register/argument schedule, not an incorrect replay-field offset or
an omitted helper call. The fresh natural seed/receiver experiments recorded
above do not close it.

No production source change was accepted from this sweep. The target PE hash,
original-bound inventories, current exact siblings and canonical match units
remain unchanged. No full-project cold replay was run.

## Fifth focused sweep — separate branch-shape and register-lifetime causes

### `GameplaySetupThread`: isolate the remaining fixed-value block

The strongest natural source candidate in this sweep uses a fixed-22 label in
the mode-4 arm, a fall-through `if (gameMode == 2)` form in mode 3, and a
nested `switch (gameMode)` in mode 2. Its fresh pinned build is **1,689 / 1,689
physical bytes**, **414 / 413 instructions**, **172 / 172 bound fields**,
**374 unequal bytes**, and **zero missing target bytes**. The direct graph is
still not identical, and the extra instruction means this is not an exact
owner.

The complete disassembly alignment narrows the remaining structural issue to
three small regions, not a vague whole-function mismatch:

- target `+0x431` contains the fixed `valueC8 = 22` store in the case-4 block;
  the candidate moves that same store to `+0x545` in the shared mode-2 tail;
- target `+0x521` is `cmp [gameMode], edi; jne ...`; the nested-switch
  candidate uses `mov eax,[gameMode]; sub eax,edi; je ...` at `+0x513`;
- the candidate's corresponding shared-tail jump/store adds one instruction
  relative to the target.

New controls separated these causes. Replacing the mode-2 nested switch with
an `if (gameMode == 2)` plus `goto fixed_mode_value_c8` restores the target's
`cmp [gameMode],edi; jne` branch shape, but the compiler duplicates/moves the
fixed-22 store and returns to **1,682 bytes / 413 instructions / 694
unequal bytes / seven missing bytes**. Inverting that `if` keeps a memory
compare but reverses the conditional edge and yields **1,688 bytes / 413
instructions / 684 unequal bytes / one missing byte**. Comparing against a
local setup-mode snapshot or switching on `gameMode - 2` adds fields or
instructions and is worse. These results establish that the fixed-store
placement and conditional edge must be solved together; neither a local alias
nor a cosmetic condition inversion is sufficient. No setup source variant is
adopted.

### `EnemyManagerView::OnUpdate`: exact register and local-home schedule

The 13-byte owner frontier was re-decoded at instruction boundaries. The
residuals are register selection and three stack-home placements:

- `+0x19A`: target reads `drawGroup3387` into EAX and indexes the draw-head
  table with EAX; candidate reads it into ECX and indexes with ECX;
- `+0x251/+0x262`: target loads `sideState320` into EAX and fetches the effect
  receiver through `[eax+0x0C]`; candidate uses EDX and `[edx+0x0C]`;
- `+0x26C/+0x272`: target loads the special-attack threshold into EDX and
  pushes EDX; candidate uses EAX;
- `+0x293..+0x2BF`: target loads position x/y/z through EAX and stores them
  to the descriptor's three consecutive homes `-0x2A4/-0x2A0/-0x29C`.
  Candidate loads through ECX with a cyclic EDX/EAX/ECX assignment and writes
  the same values to `-0x29C/-0x2A4/-0x2A0`.

An explicit natural `descriptor.position.x/y/z` assignment in x/y/z order
reproduces the target's x/y/z register-to-stack-home ordering. However, it
changes the owner-bound literal-reference positions and is rejected by the
strict original-field guard; it is not integrated. Pointer/reference aliases
reused across both the periodic effect call and descriptor construction retain
the original 13-byte frontier. The canonical owner remains **3,900 / 3,900
bytes, 97 fields, 69 calls, 170 blocks, 13 differences**. No guard or literal
identity check is weakened.

### `BeginPlaybackStage`: lexical lifetime controls

Four natural nested-scope variants were compiled: the entire stage section,
frame pointer selection through seed publication, header publication through
seed publication, and cursor publication through seed publication. Each
retains **538 / 540 bytes, 151 / 152 instructions, 32 / 32 fields, 5 / 5
ordered calls, 12 / 12 blocks, 39 overlapping differences, and two missing
bytes**. The final `inputDelay = 0` store remains before the saved-register
pops, and the target's zero-extend-through-EDX seed argument remains
unexplained. The source was restored; no lifetime-only scope is adopted.

All probes compiled only the touched translation unit/carrier. The
maintained sources were restored after every experiment. No full-project cold
replay ran, and no exactness credit or target denominator changed.

## Sixth focused sweep — replay writer/return-expression controls and canonical receipts

### `ReplayManagerView::SaveReplay`: register hints and destination cursor mutations

The guarded original-bound 1,238-byte writer was rebuilt for each of six
`register` controls (`tempBuffer`, `currentOffset`, `frameHeader`, and their
combinations). Every build remains **1,238 bytes / 380 instructions / 53
fields / 23 ordered calls / two differences**, at `+0xB7/+0xBB`. Four natural
cursor spellings were also tested: mutate and restore `tempBuffer`, add then
subtract the `0xC0` header bias, a separate pointer cursor, and a pointer-to-
pointer cursor. The first three preserve the same two-byte frontier; the
pointer-to-pointer spelling is rejected by the original-bound guard. No
register hint or pointer mutation is retained.

### `BeginPlaybackStage`: final-store expression controls

The final `inputDelay = 0; return 0;` was tested as a returned assignment,
return-after-read, comma expression, typed zero assignment, local-zero
assignment, and an explicit empty scope. All six retain **538 / 540 bytes,
151 / 152 instructions, 32 fields, five calls, 12 blocks, 39 overlapping
differences and two missing bytes**. The target's `pop edi; pop esi; store
inputDelay; xor eax,eax` order is therefore not explained by a source-level
return-expression or simple lexical scope. No source change is adopted.

### Fresh canonical owner receipts after all probes

The three focused boss-frontier commands completed successfully, with no
whole-project cold replay:

- `check-boss-frontiers.py --owner gamemgr --skip-exact-siblings`: the setup
  worker remains **1,689 / 1,682 bytes**, 172/172 fields, 35 calls, 78/78
  blocks, 694 differences and seven missing bytes; `GameManager::OnUpdate`
  and `CleanupGameplayState` remain exact at 1,230 and 425 bytes.
- `check-boss-frontiers.py --owner enemy --skip-exact-siblings`:
  `EnemyManagerView::OnUpdate` remains **3,900 / 3,900 bytes**, 97 fields,
  69 calls, matching direct graph/switch destinations and 13 differences;
  `AppendPlayerCollisionBox` remains exact at 82 bytes.
- `check-boss-frontiers.py --owner replaymenu --skip-exact-siblings`:
  `DrawReplayMenu` remains **1,280 / 1,280 bytes**, 57 fields, 16 literal
  definitions, 30 literal references, 13 calls, 72 blocks, matching direct
  graph, zero missing bytes and 29 differences.

These are current diagnostic receipts, not exactness credit. All production
sources remain restored to the committed baseline, all target/original-field
guards remain enabled, and the repository is clean after the focused builds.

## Seventh focused sweep — setup case-order controls

The fixed-22 label plus the two natural `if (gameMode == 2) goto label`
branches was held constant while the five outer setup-mode cases were
permuted. Orders beginning with cases 4/3/2 in different permutations all
reproduced the **1,682-byte / 413-instruction / 172-field / 694-difference /
seven-missing-byte** baseline. Moving the 4/3/2 group behind cases 0/1
produced **1,691 bytes / 413 instructions / 172 fields / 691 differences /
two excess bytes**. Case order by itself therefore does not place the shared
fixed-22 store at target `+0x431`; this family of permutations is exhausted
for the maintained switch shape and should not be repeated unchanged.

No production source edit was retained. The two current checkpoint commits
preserve the compiler receipts and all canonical owner checks; the worktree
remains clean and no full-project cold replay was run.

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

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

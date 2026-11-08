# TH09 boss and leaf re-investigation — 2026-11-08

> Historical source-bound investigation snapshot from 2026-11-08. Its
> per-owner compiler observations remain useful, but its branch/HEAD and
> “latest” statements are not live status. Use
> [the refreshed next-agent guide](TH09_NEXT_AGENT_HANDOFF_20261108.md) and
> [live ledger](RE_HANDOFF.md) for current state. It does not claim that the
> large open owners below are exact. The target is the
> Japanese TH09 v1.50a PE with SHA-256
> `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.

## Focused source commits

| Commit | Change | Verification and scope |
| --- | --- | --- |
| `908fbb06` | Spell `ReplayPayloadView::configSnapshot()` initialization explicitly. Keep `ReplayDataView`'s payload base call implicit. | The 15-byte payload constructor and 18-byte data constructor both remain relocation-aware exact. |
| `d06b83f8` | Name the volatile pointer-slot type used by `BeginPlaybackStage` as `ReplaySideRuntimeSlotView`. | Fresh `ReplayInput.obj` remains 538/540 bytes, 32 bound fields, 5 calls, 39 unlike overlaps and 2 missing bytes. The `ReplayInputNoEh.obj` SaveReplay owner remains 1,238 bytes with only two unlike bytes. |

These are source clarity/reconstruction commits, not new authored exact-function
credit. In particular, the constructor origins (retail-authored versus
compiler-generated) remain unknown.

## Large owner: `EnemyManagerView::OnUpdate` at 0x00410730

Fresh pinned-VC7.1 build of `src/EclManager.cpp` and the original-bound
`inspect-enemy-manager-update.py` result:

- Original and candidate physical extent: **3,900 bytes**.
- Candidate relocations: **97**, all independently bound.
- Original and candidate instruction counts: **1,026**.
- Original and candidate ordered direct calls: **69**.
- Reviewed direct control-flow blocks: **170 / 170**; switch-table destinations agree.
- Remaining unlike bytes: **13**, exactly at
  `0x19C, 0x1A3, 0x252, 0x263, 0x26D, 0x272, 0x294, 0x29A, 0x29C, 0x2A5, 0x2A9, 0x2B7, 0x2BD`.
- The separate `AppendPlayerCollisionBox` leaf remains **82 / 82** bytes exact.

The 13 differences group into two local regions. The first is the draw-group
index load/LEA at `+0x19A..+0x1A8` (target uses EAX for the zero-extended byte;
candidate uses ECX). The second is the special-attack descriptor and native
side dispatch at `+0x251..+0x2BD`; the same data and calls are present, but
VC7.1 chooses different general-purpose registers and stack temporaries.

Fresh natural-source experiments included array subscript versus pointer
addition, local index widths, moving the draw-head local to function scope,
side pointer aliases, a unified native GameManager side pointer, direct and
memberwise position copies, Float3 references/copies, and `memcpy`. They did
not reduce the 13-byte residual. A full `EnemyFloat3` to `Float3` alias is not
a valid shortcut: the existing POD vector appears in unions, whereas `Float3`
has a user-provided constructor. A narrower standalone `Float3` field trial
compiled after its dependent ECL views were adjusted, but the same 13 bytes
remained. No volatile/register-forcing/assembly variant was adopted.

The current remaining task is compiler dataflow and variable-lifetime recovery
around the draw-group index and the special-attack descriptor. The 3,900-byte
boss is still non-exact; matching control flow and calls are not byte credit.

## Large owner: `ReplayManagerView::SaveReplay` at 0x00420C60

Fresh pinned no-EH VC7.1 build and original-bound `inspect-replay-save.py`:

- Original and candidate physical extent: **1,238 bytes**.
- Original and candidate instruction count: **380**.
- Original-bound fields: **53 / 53**; ordered direct calls: **23 / 23**.
- Remaining unlike bytes: exactly `+0xB7` and `+0xBB` (decimal 183 and 187).

The two residual bytes are register selection in the first 0x20-byte stage
header copy. The target loads the temporary buffer base into EDI and forms the
destination from EDI plus the running offset; the current source loads the
same base into ECX and forms the destination from ECX plus the same offset.
The payload bytes, header fields, call order and direct control flow agree.

Fresh source-cold trials included mutable versus const chain/frame pointers,
array-subscript destination, local destination pointer, payload-base pointer,
`char *` buffer plus typed cast at the codec call, `size_t`/unsigned allocation
and stream lengths, unsigned current offset, and typed frame-header copies.
They retained the two-byte residual or changed the physical owner and were
rejected. No target-byte patch, padding, assembly or compiler-profile steering
was introduced.

## Second Replay boss: `BeginPlaybackStage` at 0x00420840

Fresh `ReplayInput.obj` build and original-bound
`inspect-replay-begin-playback.py`:

- Original physical owner: **540 bytes**; current candidate: **538 bytes**.
- Original/candidate instruction counts: **152 / 151**.
- Original-bound relocation identities: **32 / 32**; ordered calls: **5 / 5**;
  direct control-flow blocks: **12 / 12**.
- Unlike overlapping bytes: **39**; missing target bytes: **2**; no extra source
  bytes. The first 30 field positions agree; the last two seed/epilogue fields
  trail by one and two bytes.

Two independently useful observations were rechecked from original bytes:

1. Around `+0x151..+0x163`, retail loads the replay `value1E8` byte, reads the
   side-0 runtime pointer, then publishes `value1E8`. A non-volatile pointer
   alias in the current pinned build hoists that pointer relocation ten bytes
   earlier and worsens the owner from **39** to **49** unlike overlaps. The
   retained volatile pointer-slot read preserves the original field order.
   Naming the slot type `ReplaySideRuntimeSlotView` leaves the 538-byte object
   and all 39 differences unchanged; it is a clarity improvement, not a fix.
2. At the seed tail, retail uses `xor edx,edx; mov dx,[eax+4]`, loads the RNG
   receiver into ECX, pushes EDX and calls `SetSeed`. The current candidate
   uses `movzx eax,word ptr [eax+4]`, pushes EAX, then loads the receiver.
   This accounts for the two-byte deficit and shifted epilogue. Local `u16`/
   `u32` seed variables, zero-initialized locals, `memcpy`/OR seed forms, and
   explicit RNG pointer/reference receivers all retained the 538/540,
   39-overlap, two-missing frontier.

The volatile slot is retained because the original-bound field chronology
supports it; the exact original C++ qualifier is not inferred. The seed
register/zero-extension schedule remains open. The function is not exact.

## Small Replay constructor leafs

`ReplayPayloadView::ReplayPayloadView` at `0x00420830` remains **15 / 15**
bytes exact with its `GameConfiguration` constructor relocation at `+0x07`.
`ReplayDataView::ReplayDataView` at `0x00420C40` remains **18 / 18** bytes
exact with its payload-constructor relocation at `+0x0A`.

The payload source now spells `: configSnapshot()` explicitly. A trial that
also explicitly initialized both `ReplayDataView` bases caused VC7.1 to inline
the payload constructor and emitted a 31-byte derived body; that trial was
rejected. The maintained derived constructor therefore retains its implicit
base initialization, matching the target's out-of-line 18-byte call to the
payload constructor. This byte evidence does not establish the retail
constructors' authored/compiler-generated origin.

## Follow-up probes after this snapshot (same date)

Additional natural C++ experiments were run against the complete owners after
this snapshot. For Enemy `OnUpdate`, index-local widths, pointer arithmetic,
inline helper returns, copied/reference/bitfield flag views, descriptor POD and
`D3DXVECTOR3` overlays, `Float3` copy operators, descriptor-field ordering and
split speed temporaries did not reduce the 13-byte residual. Moving the side
alias across the branch changed the physical extent and was rejected; custom
assignment and split-speed trials worsened codegen.

For `RunEcl`, 90 combinations of opcode-4 timer-operand forms and opcode-155
masked-bit expressions compiled. None improved the 30-difference baseline.
The current source retains the raw instruction-word snapshot for opcode 4 and
the masked XOR helper for opcode 155. Alternate local/union/bitfield result
forms changed register allocation or shifted subsequent code. No source
variant from these trials was adopted. Their scratch objects and copied source
trees were removed during the handoff cleanup; this summary is the durable
result. See the current next-agent guide for the exact maintained source state.

## Focused reproduction

These are bounded commands for the current owners; no full cold replay is
required:

```bash
python3 -B scripts/build-match-unit.py --unit enemy-attached-effect-update
python3 -B scripts/inspect-enemy-manager-update.py build/matching/EclManager.obj --json --explain
python3 -B scripts/inspect-enemy-collision-box.py build/matching/EclManager.obj --json

python3 -B scripts/build-match-unit.py --unit replay-input-update
python3 -B scripts/inspect-replay-begin-playback.py build/matching/ReplayInput.obj --json
python3 -B scripts/build-match-unit.py --unit replay-manager-create
python3 -B scripts/inspect-replay-save.py build/matching/ReplayInputNoEh.obj --json --guard

python3 -B scripts/build-match-unit.py --unit replay-payload-constructor
python3 -B scripts/compare-coff-function.py --unit replay-payload-constructor --json
python3 -B scripts/compare-coff-function.py --unit replay-data-constructor --json
```

After the two source commits, focused rebuilds and diagnostics passed. Target
attestation, `validate-tracking.py --require-target`, `validate-docs.py`, and
`git diff --check` also passed. The current tracking validator reports **2,248
candidates, 1,037 mappings, 1,024 exact**; the 13 open mappings remain open.
No full cold replay, full product link, runtime/port test, Factory acceptance
or remote push is claimed.

At the time of this snapshot, the branch was `main`, two commits ahead of
`origin/main`, and the worktree was clean. That branch relation is historical;
current Git state is recorded in the next-agent guide. The ignored `.analysis/`
experiments are local scratch evidence, not source inputs or accepted receipts.

## 2026-11-08 fresh RunEcl opcode-4 / opcode-155 register-lifetime probes

Rebuilt only `enemy-attached-effect-update` and re-ran the original-bound
complete RunEcl inspector. The current carrier remains **15,564 physical bytes /
598 bound fields / 30 ordinary differences / zero field or table differences**.
The independently attested original PE decompilation of `RunEcl @ 0x004086C0`
was read around both handlers, rather than relying only on earlier notes:

- Opcode 4 at `+0x0277` is a direct `ZunTimer::operator=(int)` call with the
  raw instruction DWORD at `instruction + 0x0C`; the target loads that value
  into ECX and pushes ECX before loading the timer receiver. The four natural
  direct-expression / integer-local / pointer / reference trials retain the
  seven opcode-4 differences. Reusing the existing `rhsInt` local also retains
  them. Removing the copied wire-word declaration moves twelve unrelated
  opcode-5 bytes, so the whole-owner score worsens from 30 to 42; that shape is
  rejected.
- Opcode 155's decompiler expression is the native primary-word XOR update:
  `flags ^= (flags ^ (byte << 24)) & 0x01000000`. The target zero-extends the
  wire byte into ECX, preserves the old flags in ESI, masks in ECX, copies ESI
  to EAX and stores the XOR result from EAX. This confirms the bit-level
  semantics, not the unique source declaration.

Fresh natural C++ trials distinguish three compiler shapes. Keeping the
current `memcpy` byte snapshot emits `mov cl`, loads the old word into EAX,
then zero-extends CL; the block stays 31 bytes but retains the 23 opcode-155
unlike bytes. Direct byte reads with the same XOR/helper or compound-XOR
semantics emit a 29-byte block, missing the target's two-byte `mov eax,esi`;
the resulting two-byte code shift changes later handler/table placement and
the complete inspector reports **2,290** differences (2,048 ordinary, 242
field, 26 table). Native bitfield and local bitfield-copy variants emit a
30-byte block with the shorter accumulator-specific AND encoding; the
complete inspector reports **2,140** differences (1,938 ordinary, 202 field,
26 table). All these are rejected as whole-owner regressions. No inline
assembly, target-byte patch, volatile local, compiler-profile change, or
relocation/table masking is used or accepted.

Scratch objects, compile logs and complete reports are retained under
`.analysis/gpt-web-th09-boss-leaf-20261108/runecl-probes/`. The 30-byte
baseline remains the maintained source frontier; these experiments award no
partial exactness credit. The next useful search is for a natural source form
that preserves the target's two-register live range in opcode 155 while
retaining the original 31-byte handler, and independently selects ECX for the
opcode-4 stack argument without perturbing opcode 5.

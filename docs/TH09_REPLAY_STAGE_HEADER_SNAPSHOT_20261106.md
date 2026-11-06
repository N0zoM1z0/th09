# Replay stage-header snapshot and complete Save review, 2026-11-06

SaveReplay now names a read-only snapshot of each live stage-header pointer.
The same pointer is tested and copied before the shared frameStart/frameOffset
word is replaced with a serialized file offset. This makes the live-to-wire
transition explicit in the maintained nonexact source. No interface, layout,
compiler profile, original branch or error behavior changes.

Both existing source/profile carriers retain the whole1238-byte/53-field owner,
including all1236 previously equal positions. Original differences remain only
+0xB7/+0xBB. This is natural source progress without new exact credit.

## Fresh original observations

Direct Ghidra rechecks SaveReplay420C60, record constructor420C40 and payload
constructor420830. Disk/Program SHA-256, x86 little-endian32, base400000,
entry47D45F and all685056 mapped file bytes are attested. Both constructor
origins remain unknown; their33 bytes/two fields do not receive authored credit.

The differing instructions belong to the first32-byte stage-header copy inside
the three-stream loop, not the earlier complete0x1EC-byte record copy. At original
+0xB6, destination base loads into EDI; +0xB9 forms EDI+EBX-C0 in EDI. Current
code loads the base into ECX and forms ECX+EBX-C0 in EDI. Only the two register
encoding bytes differ. The original and maintained owners have380 instructions,
23 identical original-position call operands,53 independently bound fields and
49 matching direct control-flow blocks. Their original0x618 frame and terminal
RET4 also agree; ten followingCC bytes are unowned.

The original constructor initializes the payload through its actual constructor
chain but leaves the header tables uninitialized before the allocation-size
scan. The maintained source preserves that observed scan and cleanup behavior.
No zero initialization, null handling or allocation-failure behavior is added.
Ghidra decompilation does not prove unique C++ names, ownership or qualifiers.

## Bounded current-native source models

All models use the existing /O2 /Ob1 /Oy- profile and the current native inputs.
No assembly, barriers, register/variable-order hints, new volatile, dummy clobbers,
fake returns or profile searches are introduced.

| Model | Bytes/fields | Unlike overlaps | Original-relative extra bytes |
| --- | --- | --- | --- |
| Frame-header pointer snapshot; adopted |1238/53|2|0|
| Snapshot plus typed frame assignment |1238/53|2|0|
| Publish wire offset before copying captured source |1238/53|13|0|
| Payload write cursor instead of file-relative integer |1238/53|773|0|
| Named stream table slice |1270/53|1082|32|
| Genuine standard typed copy algorithm |1286/53|1079|48|
| Stage-header early continue |1238/53|2|0|
| Stage and FPS early continues |1238/53|2|0|
| Read-only chain inputs |1238/53|2|0|
| Coherent const manager/chain input signature |1238/53|2|0|
| Actual allocator definitions visible |1238/53|2|0|
| Complete actual codec visible |1238/53|2|0|
| Actual record/payload constructors visible |1238/53|2|0|

Every complete model retains53 referents, their multiplicities, both literal
payloads and all23 ordered call identities. Shifted models do not retain their
physical positions or the complete graph and are not accepted. The original
380-instruction/49-block graph remains independently decoded from original bytes.
The const-manager model has a coherent real declaration and its actual new
COFF symbol; it changes no physical ECX/EDX/stack-name/RET4 transport. It is not
adopted and does not establish original nominal qualifiers or ownership.

All13 contexts preserve130 repeated complete exact sibling comparisons:
31317 bytes/1586 fields. Visibility models also reproduce15 actual producer
comparisons2013 bytes/71 fields: four ZunMemory bodies, all nine codec bodies,
and both constructor bodies. Actual definitions are included unchanged and
compared separately; a caller observation does not substitute for their proof.
Neither visibility nor source guard order recovers the two remaining bytes in
these fixed contexts. This does not exhaust natural source or TU hypotheses.

## Affected source validation

Only ReplayInput.cpp changes game source. Its two existing profiles are freshly
built; both complete objects preserve all28 regular function/profile bodies,
two EH graphs, auxiliary code and private cleanup definitions. The other
nonexact BeginPlaybackStage remains538 bytes against540,39 unlike overlaps
and two missing, through full raw-body/referent conservation.

Five fresh groups and three verified unchanged source/profile/object proofs
cover31 existing complete units6407 physical bytes/311 fields. They include
all12 same-source canonical siblings, the real direct callees and nested
constructor/configuration dependencies. The nine codec units retain excluded
origin and both replay constructors remain origin-unknown. The strict public
comparison uses current actual objects through a private full manifest; no
canonical symbol, destination, addend, extent or comparison rule changes.

All49 focused Replay tests pass. Tracking with the original required, progress,
documentation and whitespace checks pass. Shared validators are unchanged;
full public CI is not repeated for this bounded source/ledger/documentation edit.
Totals remain1034 source-present/1021 exact/13 nonexact. Product/runtime,
semantic and portability gates remain open; local receipts are not Factory
acceptance or remote-publication evidence.

The ignored packet is `.analysis/replay-header-copy-20261106/`. It retains the
original-bound diagnostics, current/frozen sources, all models and objects,
complete actual producer comparisons and fresh mapped-byte Ghidra reports.
The initial current-build driver failed before compilation on a Python import
path; its setup log is retained separately from all successful experiments.

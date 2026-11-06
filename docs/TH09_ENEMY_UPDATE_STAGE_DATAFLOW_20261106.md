# Enemy update source and residual dataflow, 2026-11-06

The maintained complete Enemy update remains nonexact:3883 code bytes,
3900 physical bytes including NOP/table,97 fields and13 differing bytes.
The source now uses its typed manager parameter directly and decodes the
two-bit side category once within the side-state gate. Both actual ECL
profiles preserve every emitted function body and field identity. No type,
ABI, layout, compiler profile or exact-function status changes.

## Original-bound observations

Fresh direct Ghidra analysis of410730 attests the original disk/Program SHA,
x86 little-endian32, base400000, entry47D45F and all685056 mapped PE bytes.
Its disjoint3881-byte flow body is not the complete3883-byte code extent.
The complete physical target SHA-256 remains
`a7f01c2ff0bda79d1fd17ac9170dc3560d4089457455d7928776df8556f1bc75`.
The current source retains1026 instructions,69 ordered calls,170 matching
blocks, all four switch entries and the actual five scalar literal definitions.

The residual offsets are+19C,+1A3,+252,+263,+26D,+272,+294,+29A,+29C,
+2A5,+2A9,+2B7,+2BD. Their bounded transfer review distinguishes four paths:

- Draw lookup uses a different temporary index register. Both states converge
  after the first footer instruction at+EF3 loads ECX from the selected head.
- The side-effect path reaches SpawnEffect40CC00 with the same ECX receiver,
  four stack arguments and callee-preserved inputs. The comparison stops before
  the call; it does not invent volatile outputs or treat them as ABI inputs.
- The threshold path passes the same value in a different register. Executing
  the actual same-carrier20-byte Timer comparison403DE0 gives identical states,
  including its ECX/EDX writes, EAX result and RET4 stack transport.
- The position copy loads and stores XYZ in a different register/order sequence.
  All modeled registers, memory, flags and ordered x87 expressions converge
  after XOR EDX at+2D3, before+2D5.

This private static transfer audit assumes valid disjoint object/private-stack
storage and normal non-trapping completion with the same FP control state.
It is neither runtime validation nor an all-input/intermediate-state proof.
Four IR-only wrong-scale/argument/threshold/coordinate controls are rejected;
no target, source or object bytes are patched. It grants no exactness or later
semantic-phase credit.

## Natural source models

Ten models use the existing fixed main profile. Each protects77 complete exact
same-source units; all retain full RunEcl15564/598/30. No assembly, barriers,
register hints, variable-order hints, forced inline or profile search is used.

| Source model | Physical bytes | Unlike overlaps | Extra bytes |
| --- | ---: | ---: | ---: |
| Ordinary draw-head lookup |3900|13|0|
| POD-to-vector converting constructor |3916|3274|16|
| POD base plus converting constructor |3916|3274|16|
| Assignment through actual POD base |3900|13|0|
| Individual position components |3900|469|0|
| Value snapshot then components |3916|3284|16|
| Nested gate with named side category |3900|13|0|
| Loop-local enemy index |3900|13|0|
| Direct manager parameter |3900|13|0|
| Direct manager and named category |3900|13|0|

The last model is adopted for source clarity. Header/type/conversion models
remain private hypotheses. Each of the three changed-header models also freshly
rebuilds the actual28-byte descriptor constructor and reproduces its complete
original body. The converting models retain temporary coordinate stores and
grow the stack frame by12 bytes; they do not emit an extra conversion call.
The actual SDK vector declarations suggested these models, but provide no TH09
layout or ownership evidence.

The public Enemy diagnostic correctly rejects shifted literal positions in
four models. A separate bounded private diagnostic reads all actual bytes and
97 actual fields, derives source-local destinations from source definitions,
validates external identities/addends, actual literals, code/NOP/table and calls,
and counts every missing/extra byte. The public gate is not weakened. The first
four model reports retain an initial reader-API error; subsequent read-only
replay uses the correct full RunEcl reader. This was not a compiler rejection.

## Affected replay and limits

Two fresh current ECL builds pass83 complete existing exact units29856/1389.
All272 regular body/profile payloads and private/auxiliary definitions agree
with their source-bound baselines. The actual69 calls resolve40 distinct exact
direct callees:seven are already in those83;33 receive additional complete
proofs,19 reused after source/profile/object verification and14 using11 fresh
support objects. The separate nonexact RunEcl remains fully diagnosed. Actual
folded callbacks at44AB20 remain verified three-byte definitions.

Strict public replay of116 distinct existing units passes44752 physical bytes
and1896 fields. Only two actual owner-local compiler label spellings refresh;
positions, destinations, kinds, addends, extents and profiles stay unchanged.
The focused suite passes96 tests/282 subtests. Tracking/progress/documentation
and whitespace checks remain required for this checkpoint.

The ignored packet is `.analysis/enemy-update-stage-dataflow-20261106/`.
Totals remain1034 source-present/1021 exact/13 nonexact. The reconstruction goal,
complete Enemy exactness and later product/runtime/semantic/port gates stay open.
Local commits and compiler proofs do not establish Factory or remote acceptance.

## Immutable source and control binding

Source commit `a34affb3fdd0aac570f424f5b5fcc72d9065083f` binds134 tracked
inputs/Git blobs,38 selected compiler/SDK files and46 current/model/producer/
baseline objects. The frozen87-input hypothesis epoch remains bound to its
originaldcf780ea source, separately from the adopted current source. The
source/control proof SHA-256 is:

`aefdbed4bd645db99f91bad34c5f01f1bc93a19c354f4b96de56ea357a9a78a0`

The ignored `verify.py` independently replays116 strict current comparisons,
272 preserved body/profile payloads, all ten complete models/770 exact sibling
comparisons, three actual constructors, bounded static transfer controls and
all mapped Ghidra ranges. Historical model receipts retain their original
private-label spellings; the current public replay uses the refreshed manifest.
Reused callees require the old proof's exact input and object memberships.

Cleanup removes854 SHA-identical staged source copies and26 disposable PDBs,
880 files/8474638 bytes. Every removed source resolves to its hashed retained
frozen input. All26 new compiler objects, changed source models, original-bound
baselines and reused objects remain. No original input or analysis object is
removed. Target-required tracking/progress, documentation and whitespace pass;
totals stay1021 exact/13 nonexact. This is a local source-scoped checkpoint.

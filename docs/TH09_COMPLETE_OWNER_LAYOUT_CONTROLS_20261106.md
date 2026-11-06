# Complete Replay layout and Enemy descriptor controls, 2026-11-06

This review starts from clean `806e0fe1`. The original Japanese 1.50a target,
tracking and reconstruction-status checks pass. No production source, compiler
profile, relocation destination or exact ledger is changed by these controls.
Totals remain1035 source-present /1022 exact /13 nonexact.

## Replay composition is not identified by the constructor chain

Fresh direct Ghidra covers SaveReplay420C60, record constructor420C40 and
payload constructor420830. Disk/Program SHA-256, i386, base400000, entry47D45F
and all685056 mapped file bytes agree with the original. Ghidra's Save body
set is1227 bytes; the PE/COFF comparison covers the entire1238-byte owner.

Three complete current-source models replace the maintained pair of base-class
projections with: header base plus payload member; header/payload members;
or expanded header fields plus payload member. Every model guards the same
C0-byte header,12C-byte payload,1EC-byte record and existing field locations.
All ReplayInput field consumers are adapted coherently. The actual out-of-line
constructor source is compiled separately with each same staged header.

All three Save bodies remain1238 bytes/53 fields/two differences at+B7/+BB.
Each actual constructor pair reproduces all18/15 original bytes and its two
REL32 fields. That is nine complete comparisons3813 bytes/165 fields, with
no authored exact credit. These concrete counterexamples leave composition,
inheritance, original nominal classes and constructor authorship unresolved.
No layout model is adopted.

One initial member-layout renderer missed a source-local `offsetof` use and
failed compilation. Its recipe, rejection and input hashes are retained; the
corrected complete member model compiles and independently reproduces the
same two-byte frontier. A renderer error is not a rejected C++ layout hypothesis.

The ignored `.analysis/replay-object-shape-20261106/` freezes thirteen inputs,
all changed controls, recipes, actual objects/COFF rows and complete reports.
`verify-controls.py` rechecks all nine comparisons.33 SHA-identical per-case
copies are pruned. `replay-one.py` restores eleven unchanged inputs and cold
rebuilds the complete member-layout Save and both constructors; their entire
raw bodies/actual COFF rows reproduce before those eleven copies are pruned
again. Control seal SHA-256:
`1820e05da6ada7703a294481b3fb93c812375776b76282439ba7b8eda1c93300`.

## Enemy descriptor selection remains a complete-owner question

Fresh direct Ghidra attests the same full mapped target for Enemy OnUpdate.
The preserved bfa170c9 ECL object, existing profile and all86 quoted source
inputs still match their source-bound receipt. All four new controls compile
complete EclManager carriers with the existing /O2 /Ob1 /EHsc /Oy- profile,
serially at lowered priority with a180-second timeout.

The pinned Cirno-Labs Enemy update supplies a hypothesis for count selection
and assignment order. The TH09-observed speed constant remains+1.0; the
reference's different+2.0 value is not imported. The complete comparisons are:

| Descriptor source model | Physical bytes / fields | Unlike overlaps / absent bytes |
| --- | --- | --- |
| Arithmetic count1 expression | 3900 /97 | 13 /0 |
| Reference-style late count1 assignment | 3900 /97 | 214 /0 |
| Late arithmetic count1 | 3852 /96 | 2954 /48 |
| Direct side accesses with arithmetic count1 | 3900 /97 | 13 /0 |

The late assignment repairs old byte+2A9 but introduces202 new differing
positions. Its real Player-state call moves+36B to+36D, so the existing strict
inspector rejects it. The arithmetic combination also changes the whole
physical extent/census. Neither earns partial credit or adoption.

A separate noncrediting comparison binds every actual candidate COFF row:
external destinations retain independently reviewed baseline identities;
owner-local labels use their actual same-section definitions. All five literal
payloads are checked against actual object data and original PE values. It
compares the complete overlap and records every absent/extra byte, including
all table fields. The strict public inspector remains unchanged. All four
complete RunEcl companions retain15564/598/30 with zero field/table differences.

The ignored `.analysis/enemy-loop-lifetimes-20261106/` retains these controls,
actual fields, strict rejection reports and full negative comparisons.
340 SHA-identical staged copies are pruned. A restored late-count carrier
cold-reproduces the entire Enemy3900/97 and RunEcl15564/598 raw bodies/fields;
85 restored duplicates are then pruned again. Control seal SHA-256:
`bba170578758b77452a045a88fcca830c3630c780ea52ac4da6ec98dcb494de9`.

The PE Rich records were also rechecked. The build4035 entries are the same
aggregate caveat already recorded in KNOWLEDGE_BASE RESUME-TOOLS-20261022-001;
they do not identify an optimizer or build for an individual function. Their
raw tuples and this limitation are retained privately. Compiler/profile pins
remain unchanged.

These are bounded target/compiler observations. Existing exact-source receipts
remain bound to their recorded commits; no new exact function, Factory verdict,
product/runtime, semantic-stage or portability result is claimed. Tracking,
progress, documentation and whitespace checks pass.

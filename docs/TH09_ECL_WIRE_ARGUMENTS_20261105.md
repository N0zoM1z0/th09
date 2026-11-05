# ECL wire operands and complete-owner controls, 2026-11-05

Started from clean `3780ab89`. The original Japanese 1.50a target, tracking
and status checks pass. Fresh direct Ghidra exports of RunEcl and biased
movement attest disk/Program SHA, i386, image base, entry and all685056
file-backed mapped bytes. Ghidra decompilation is semantic evidence only.

## Maintained operand layout

The instruction prefix now names its trailing four-byte operands through
`Th09EclWireArgument`. Integer and floating interpretations occupy the same
word; the byte array exposes the same storage without variable resolution.
`RawInt` and `RawFloat` return references to those typed members, replacing
repeated byte arithmetic and scalar pointer casts. The existing parameter-mask
checks and resolver calls are retained.

VC7.1 compile assertions check the operand size4, prefix size0x0C and operand
offset0x0C. The zero-length trailing array is a compiler-supported variable
payload view; it does not increase the prefix size or imply an operand count
for every opcode. The raw instruction header type and external ABI stay the
same. Original nominal C++ types and declaration spelling remain unknown.

Seven actual source/profile builds cover90 canonical units:81 in the two
EclManager profiles, four operand resolvers, two interpolation helpers, and
one each for subroutine initialization, non-boss cleanup and ECL loading.
All33719 physical bytes and1631 fields compare completely exact. The311
changed private-label operand fields are checked against actual same-section COFF
definitions and immutable original operand encodings before the manifest is
updated; no destination, addend or ordinary byte is adjusted.

RunEcl remains15564 bytes/598 fields/30 ordinary differences. Its entire
linked difference list equals the fresh pre-change baseline, including the
same candidate values at every unequal position. All field/table bytes agree.
The complete Enemy guard stays3900/97/13; biased movement stays517 candidate
versus516 target bytes,28 fields/14 calls/148 unequal overlapping bytes.
This checkpoint adds no exact-function credit.

## Fresh source controls

The frozen inputs are bound to the starting commit and per-file SHA-256.
Every trial uses the existing compiler profile, a serial lowered-priority
bounded compile and a complete original-linked owner comparison.

| Control | Observation |
| --- | --- |
| Native timeout bitfield; direct byte address; byte-return accessor | RunEcl1938 ordinary/202 field/26 table differences |
| Typed trailing operands, retained scalar copies | Complete RunEcl frontier unchanged; adopted after canonical replay |
| Typed operands with direct timer/timeout assignments | RunEcl1950 ordinary/202 field/26 table differences |
| Single switch scope; one Enemy alias | Retained assignments stay30; native assignments still regress |
| Biased-movement Y pointer, array reference or named-member reference | Each emits518 bytes; complete-extent guard rejects |

The native timeout block emits44 bytes against the target45, shifting later
handlers; its masked-XOR semantics alone do not make the owner exact. In biased
movement, explicit Y aliases emit `mov` plus `add` where the target uses `lea`.
These controls are bounded negatives, not an exclusion of other natural source.
None uses assembly, register hints, padding, volatile forcing or profile search.

The existing raw-context probe recognizes both its historical scalar-pointer
accessor and the maintained typed accessor. Its macro control preserves the
selected representation. Unknown/duplicate accessors and changed typed storage
are rejected before rewriting. This preserves the rendered representation without
turning a source-spelling test into game exactness acceptance.

The real probe replay also exposed a separate staging defect: only the lexical
fragments were copied, so a transitive live `EnemyManager.hpp` included the
live flags header beside its staged duplicate. VC7.1 rejected the duplicate
types. The shared variant stager now copies and hashes the complete quoted
project-header closure. Escaping and absolute includes are rejected before
writing the carrier. The raw runner compares every staged input hash to its
observed inputs before compilation and verifies all copied inputs afterward.
The real typed-integer macro trial now compiles and completes the original-bound
15564-byte/598-field comparison with the same30 ordinary differences and no
field/table differences. The original rejected receipt is retained separately.

## Reproduction and retained evidence

The ignored `.analysis/ecl-arguments-20261105/` holds frozen inputs/hashes,
Ghidra attestations, changed trial sources, compiler logs, full owner reports,
definition-backed label proof and the current90-unit receipt. Objects remain
under `build/analysis/ecl-arguments-20261105/` and `build/matching/`.

`verify-current.py --build` rebuilds the seven actual profiles and checks all90
owners, the complete RunEcl difference list and the Enemy/movement guards.
`current-proof.json` binds source/header/manifest/profile inputs and object
hashes. Trial recipes reconstruct their inputs from the preserved frozen set.
Only hash-identical per-trial source/header duplicates are cleaned, with a
per-file receipt. Unique controls, original inputs and unreplayed evidence stay.

Target-independent CI passes1339 tests with29 skips. Target/tracking/status,
documentation, generated progress and whitespace checks pass. Whole-product,
runtime, portability, Factory
acceptance and remote publication remain separate open claims.

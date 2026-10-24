# Fresh TH09 boss-function experiment checklist

This is the independent working checklist requested on October 24, 2026.
It starts from observed current source and fresh target/compiler evidence, not
from the truth of historical repository conclusions. Historical notes may point
to an artifact, but only its actual source, inputs and results establish what
was tested. A failed concrete model never rules out an entire source family.

## Rules and baseline

- [x] Read live Git state: source baseline 95300f57, initially clean.
- [x] Verify original JP 1.50a disk image and Factory IDA entry/mapped samples.
  SHA-256: 10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822.
  IDA get_metadata attestation passed at 2026-10-24 03:19 UTC, six mapped samples.
- [x] Cold-compile current RunEcl through the pinned production frontier gate.
  Fresh result: 15564 physical bytes; 30 ordinary differences; zero field/table
  differences; 146/148 handler shapes. Opcode 4 contributes 7, opcode 155 23.
  Evidence: .analysis/gpt-dots-20261024-resume/runecl-baseline.log and .exit.
- [x] Diagnose Enemy from that same freshly built carrier, without rebuilding.
  Result: 3900 physical bytes, 97 matching fields, 13 ordinary differences,
  69 ordered calls and 170 blocks. Evidence: resume/enemy-baseline.json.
- [ ] Preserve a fresh source/profile/object-bound baseline for each experiment.

Completion of a checklist item means the stated investigation was performed,
not that an owner became exact. Accepted progress requires full owner bytes,
actual relocation destinations/tables and affected exact callees. Keep existing
matching regions protected. No target-byte injection, assembly, fake ABI,
dummy liveness, register/declaration-order hints or compiler-profile roulette.
Compile serially at lowered priority. Do not overwrite prior evidence.

## H01: coherent shared Enemy storage across State and Movement

Status: preparing an isolated model; not compiled.

Observation from current source: the same Enemy allocation is exposed through
separate State and Movement structures. Geometry and timer storage differ in
nominal types, despite describing the same addresses. The actual previously
inspected shared-owner producers did not cover Movement.

Hypothesis: a coherent shared storage owner changes whole-function alias/dataflow
or compiler allocation, including code distant from the changed declarations.

- [ ] Enumerate overlapping fields, offsets, widths, object size and real ABI.
- [ ] Build one consistent shared layout; preserve every read/write/call order.
- [ ] Compile full current EclManager carrier with unchanged pinned profile.
- [ ] Compare complete RunEcl, Enemy and every affected emitted exact owner.
- [ ] Explain changed allocation/fields/calls, even if the result worsens.
- [ ] Record result, reproducibility command and next evidence-based decision.

Do not add unsupported inheritance, lifetime consumers or new runtime effects.
A neutral result tests only this concrete model, not all shared-layout designs.

## H02: genuine Timer predicate definitions and caller context

Status: checking actual prior recipes and preparing at most two coherent models.

Observation: current Timer predicates use compact expressions. Same emitted
callee bytes alone may not establish identical compiler knowledge at callers.
This is separate from simply moving an existing definition into a translation unit.

Hypothesis: natural guarded-return implementations of the actual predicates
preserve their semantics and exact standalone bodies but change caller lowering.

- [ ] Audit current predicate contracts, including integer and float comparisons.
- [ ] Inspect actual prior predicate-form sources, not historical summary labels.
- [ ] Test guarded return definitions for existing visible greater comparisons.
- [ ] Separately test the real periodic predicate with an early no-tick return.
- [ ] Require exact original predicate bodies; diagnose complete caller and cohort.
- [ ] Record result and whether any observed change is a real improvement.

No invented return value, altered bool/integer ABI or post-call flags assumption.

## H03: instruction/control-flow ownership across the complete interpreter

Status: independent source investigation; no candidate or compilation yet.

Hypothesis: the maintained decomposition into instruction views, shared transfer
variables and lexical families misses an original whole-interpreter dataflow
relationship. Local opcode spelling alone may not capture it.

- [ ] Map actual instruction rebindings, context changes and shared live values.
- [ ] Check target memory/call effects across joins, not only opcode 4/155.
- [ ] Distinguish genuine value ownership from artificial lifetime extension.
- [ ] Select a concrete model only when its semantics and evidence are explicit.
- [ ] Compile and validate all affected owners if a justified model is found.

## H04: consistent author conventions across independently checked functions

Status: cross-function evidence collection, not an established original style.

Hypothesis: one consistent source convention should explain several independently
observed TH09 code patterns, rather than special syntax for each troublesome
opcode. This is a search prior, not proof that every original function used the
same idiom or that reconstructed source is the original author's source.

- [ ] Select repeated original byte/dataflow patterns in multiple real owners.
- [ ] Infer a coherent candidate convention for types, predicates, operand
      access or helper boundaries, with explicit competing interpretations.
- [ ] Check actual original semantics and call contracts in each selected owner.
- [ ] Test the convention across those owners under their existing profiles.
- [ ] Prefer explanations that preserve or improve multiple complete owners;
      retain contradictory results rather than tailoring one exceptional opcode.

## Results and next queue

No new exact owner or byte credit has been established by this checklist.
Items may be added, split or rejected when fresh evidence warrants it. Each
completed trial must retain source/provider hashes, exact compile command,
object and complete comparisons, plus a concise conclusion here.

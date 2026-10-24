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
- [x] Preserve fresh source/profile/object-bound inputs for the four completed
  H01/H02/H03 concrete trials; later trials need their own bindings.

Completion of a checklist item means the stated investigation was performed,
not that an owner became exact. Accepted progress requires full owner bytes,
actual relocation destinations/tables and affected exact callees. Keep existing
matching regions protected. No target-byte injection, assembly, fake ABI,
dummy liveness, register/declaration-order hints or compiler-profile roulette.
Compile serially at lowered priority. Do not overwrite prior evidence.

## H01: coherent shared Enemy storage across State and Movement

Status: compiled and independently verified; both complete owners are
byte-identical to the fresh baseline. No adoption.

Observation from current source: the same Enemy allocation is exposed through
separate State and Movement structures. Geometry and timer storage differ in
nominal types, despite describing the same addresses. The actual previously
inspected shared-owner producers did not cover Movement.

Hypothesis: a coherent shared storage owner changes whole-function alias/dataflow
or compiler allocation, including code distant from the changed declarations.

- [x] Enumerate overlapping fields, offsets, widths, object size and real ABI.
- [x] Build one consistent shared layout; preserve every read/write/call order.
- [x] Compile full current EclManager carrier with unchanged pinned profile.
- [x] Compare complete RunEcl, Enemy and every affected emitted exact owner.
- [x] Explain changed allocation/fields/calls, even if the result worsens.
- [x] Record result, reproducibility command and next evidence-based decision.

Trial packet: .analysis/th09-runecl-movement-state-owner-20261024-v01/.
The shared layout covers 70 State and 42 Movement records with 96 merged fields,
extent 0x5430. RunEcl is still 15564/598/30, zero field/table differences and
complete raw SHA-256 bc9a1a71fbc3010338ef7f3de66ca3e0f3c3ee3bb9eb51f1588c6e53dea8c01b.
All75 canonical bodies pass against original (22463 bytes / 990 fields).
All139 emitted functions equal the fresh baseline (50104 bytes / 2097 fields).
The sole renamed Movement::View is the same 3-byte identity, zero fields and
zero emitted callers; no new original binding is claimed. verify.py and
collateral.json retain the independent comparison. Next: assess a coherent
public/Core extension before a separately identified H01b trial.
No adoption or exactness credit.

Do not add unsupported inheritance, lifetime consumers or new runtime effects.
A neutral result tests only this concrete model, not all shared-layout designs.

## H02: genuine Timer predicate definitions and caller context

Status: completed concrete two-model test; no improvement, no adoption.

Observation: current Timer predicates use compact expressions. Same emitted
callee bytes alone may not establish identical compiler knowledge at callers.
This is separate from simply moving an existing definition into a translation unit.

Hypothesis: natural guarded-return implementations of the actual predicates
preserve their semantics and exact standalone bodies but change caller lowering.

- [x] Audit the changed integer predicate contracts and division short circuit.
- [x] Inspect bounded prior predicate-form sources, not summary labels; this
      is not an exhaustive historical search.
- [x] Test guarded return definitions for existing visible greater comparisons.
- [x] Separately test the real periodic predicate with an early no-tick return.
- [x] Require exact original predicate bodies; diagnose complete caller and cohort.
- [x] Record result and whether any observed change is a real improvement.

Trial packet: .analysis/th09-timer-predicate-controlflow-20261024-v01/.
comparison-guards: RunEcl30 / Enemy13, both native comparisons exact (20 bytes
each), all75 same-profile canonical bodies exact. periodic-guards: RunEcl30 /
Enemy45, all75 canonical bodies exact, but the new periodic definition fails
its own original-body proof: 31 bytes instead of original33, using a
NEG/SBB/INC result tail instead of the original branch/MOV-one pattern. Enemy
raw bytes equal the separately verified historical native-periodic class,
not a new improvement. verify.py replays all150 canonical proofs and owner
reports without compiling. Preserve the rejection; no adoption.

No invented return value, altered bool/integer ABI or post-call flags assumption.

## H03: instruction/control-flow ownership across the complete interpreter

Status: a coherent shared header/scalar-slot prefix was compiled across all
four raw-reader families; complete owner bytes remain unchanged.

Hypothesis: the maintained decomposition into instruction views, shared transfer
variables and lexical families misses an original whole-interpreter dataflow
relationship. Local opcode spelling alone may not capture it.

- [ ] Map actual instruction rebindings, context changes and shared live values.
- [ ] Check target memory/call effects across joins, not only opcode 4/155.
- [ ] Distinguish genuine value ownership from artificial lifetime extension.
- [ ] Select a concrete model only when its semantics and evidence are explicit.
- [ ] Compile and validate all affected owners if a justified model is found.

Concrete H03a evidence:
.analysis/th09-global-source-boundary-audit-20261024-v01/.
The common first-scalar-slot address replaces all four raw-reader roots without
widening accesses or moving captures; union values are never read. Current
RunEcl remains raw bc9a1a71..., 15564/598/30; Enemy remains13; all75 canonical
same-profile bodies pass. This establishes neutrality only for this address-root
model. It does not settle instruction nominal types or object-lifetime theory.

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

## H05: structured whole-interpreter control flow

Status: auditing actual prior coverage before staging.

Observation: the maintained instruction-loop label precedes a for(;;) wrapper
whose outer iteration exits through breaks or jumps to that label. A natural
flat dispatch representation may preserve the real graph while changing the
compiler's source-level lowering context.

- [ ] Verify every outer-loop exit and context-restart path.
- [ ] Check whether this exact graph-preserving whole-owner form was already tested.
- [ ] If justified, remove only the redundant wrapper, preserving scopes required
      by C++ and the complete memory/call order.
- [ ] Compile full owner and all affected native callees; explain any changes.

H04 observed limit from original Timer bodies: all five simple comparisons
share a 20-byte SETcc shape, yet H02's expressions and guards can emit identical
bytes. The compound periodic predicate distinguishes the tested 31/33-byte
forms. This supports rejecting that concrete guard form, not unique syntax.

## Results and next queue

No new exact owner or byte credit has been established by this checklist.
Items may be added, split or rejected when fresh evidence warrants it. Each
completed trial must retain source/provider hashes, exact compile command,
object and complete comparisons, plus a concise conclusion here.

Checkpoint CI passed 1016 tests with 29 skips; live tracking, documentation,
build graph, progress SVG and whitespace gates passed. Receipt:
.analysis/gpt-dots-20261024-resume/checkpoint-ci.log and .exit.

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

### H01b: one actual public Enemy owner, including Core

Status: main carrier and two external boundaries compiled and privately
verified; both bosses retain30/13 differences. No adoption.

Hypothesis: H01 unifies two internal views but leaves public EnemyView and Core
nominally distinct. One actual public owner, with existing methods and all four
views as aliases, is a broader coherent source model.

- [x] Reconcile public/State/Movement/Core offsets and widths (107 fields).
- [x] Preserve existing public method ABI and primary/opposing manager receivers.
- [x] Prove Movement's 8-byte hitbox prefixes use only x/y where Core exposes
      12-byte Float3; do not introduce extra copying or reads.
- [x] Stage one owner without artificial inheritance or dummy fields.
- [x] Compile complete carrier and verify all changed emitted functions.

Packet: .analysis/th09-runecl-four-view-owner-20261024-v01/.
Full affected closure is117 inputs /31 sources /142 canonical units. Only the
main EclManager carrier plus child-release and attached-effect external
boundaries were compiled. All139 main-carrier bodies match the fresh baseline,
including6 proven definition renames and2 explicit external-boundary renames.
All75 canonical equivalents pass their complete original proofs; complete Enemy
has97 matching fields,69 calls,170 blocks and13 differences. Raw RunEcl is
unchanged. Production comparator/config were not relaxed. verify.py replays the
private identity proofs. The full31-source adoption scope is NOT all replayed.

This is not an arbitrary combination sweep. It tests the real shared object
identity omitted by H01 while preserving each original access.

Independent source review found no concrete executed behavior drift in this
candidate. It does not prove original nominal types or portable alias/lifetime
legality. EnemyView's constructor is still declaration-only: a future definition
would need separate scrutiny because exposed ZunTimer members default-construct.
Do not treat a main-carrier comparison as constructor or full-product proof.

### H01c: native timeout bitfield on the coherent public owner

Status: compiled; complete RunEcl regresses and this candidate is rejected.

Hypothesis: a genuine bitfield assignment may interact with the now-unified
owner differently from the previous fragmented source. Original ANM supplies
an independent native masked-RMW precedent, but does not prove source identity.

- [x] Preserve all H01b inputs immutably.
- [x] Replace only opcode155's synthetic pure-value update with assignment to
      the actual shared one-bit field; preserve the full wire-byte read and
      low-bit truncation, never Boolean-normalize.
- [ ] Compile and compare the complete owner and affected native identities.
- [ ] Compare actual output with current H01b and retained fragmented-model
      observations; reject damage even if the local handler looks closer.

Actual H01c output:15564 physical bytes /598 fields, but1938 ordinary and202
field-byte differences,26 table-byte differences; only144/148 handler shapes.
Raw SHA-256:82e081e31c21f83618c00f795d43f4191fe1be1d73dd444e1152eb1dd7e41ea3.
Opcode4 remains wrong; opcode155 is44 rather than45 bytes and shifts subsequent
handlers. This independently reproduces a prior native-bitfield output class
on the new full-owner model; it is not a new improvement or universal exclusion.
Evidence: .analysis/th09-runecl-four-view-native-timeout-20261024-v01/.
Private identity/collateral verification now passes:138 emitted functions
(the unused timeout helper disappears), other137 bodies/fields unchanged vs
H01b;75 canonical equivalents pass; Enemy remains13 with97 matching fields.
All138 raw bodies match the retained native-direct class, but two historical
initializer symbol identities differ from the current selected-manager model
and were recorded rather than blindly aliased. verify.py/collateral.json retain
this scoped proof. No production adoption.

### H01d: direct access on the unified public owner

Status: bounded actual adapter audit; no candidate yet.

Hypothesis: after genuine owner unification, reconstruction-only identity View
adapters can be removed consistently rather than changing one opcode expression.
Any test must prove each argument already has the actual public owner type,
preserve evaluation count/timing and avoid arbitrary register/declaration hints.

- [ ] Audit all relevant adapter definitions and real argument types.
- [ ] Inspect actual prior whole-adapter transformations.
- [ ] Stage only a coherent distinct source model, preserving prior candidates.
- [ ] Compile and fully compare if justified.

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

Status: bounded original Timer/descriptor/bitfield census completed; no
unique original style or additional justified local spelling trial established.

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

Bounded H04 findings:
- Enemy/Effect share the original28-byte descriptor constructor and three-DWORD
  vector copy. ECL dispatch updates persistent storage after each resolver,
  so a shared reset/configure aggregate is not justified.
- Native ANM two-bit assignment at0x4374EF has masked-XOR RMW; adjacent Boolean
  normalization at0x4374C7 differs. Do not replace low-bit truncation by bool.
- CardAttack |= and &= lower differently; no universal XOR helper follows.
- Original bytes/current source hashes are retained in the predicate packet's
  original-bitfield-census.json. Its reconstructed ANM/Card full-body witnesses
  were not freshly replayed; absent objects do not establish exactness.

## H05: structured whole-interpreter control flow

Status: scoped flat-dispatch model compiled; no owner-byte change.

Observation: the maintained instruction-loop label precedes a for(;;) wrapper
whose outer iteration exits through breaks or jumps to that label. A natural
flat dispatch representation may preserve the real graph while changing the
compiler's source-level lowering context.

- [x] Verify every outer-loop exit and context-restart path.
- [x] Inspect bounded actual prior recipes: complementary-else form found,
      not this early-goto form; historical coverage is not claimed exhaustive.
- [x] Remove only the redundant wrapper, preserving scopes required
      by C++ and the complete memory/call order.
- [x] Compile full owner and all affected native callees; explain any changes.
  RunEcl retains15564/598/30 and raw bc9a1a71..., Enemy13; all75 canonical
  bodies pass. Evidence: .analysis/th09-flat-dispatch-audit-20261024-v01/.
  Removing the wrapper while preserving its compound scope does not change
  code generation in this concrete context. No adoption.

H04 observed limit from original Timer bodies: all five simple comparisons
share a 20-byte SETcc shape, yet H02's expressions and guards can emit identical
bytes. The compound periodic predicate distinguishes the tested 31/33-byte
forms. This supports rejecting that concrete guard form, not unique syntax.

## H06: shared Float3 copy-assignment contract

Status: auditing actual prior source recipes; not yet a new trial.

Fresh Enemy lifetime review finds the descriptor constants already match from
owner+0x2C1 onward. The residual copy region is register/store permutation, not
a missing long-lived manager, side-state or index value. Current Float3 has an
implicit copy assignment and a genuine arithmetic API.

Hypothesis to assess: a coherent explicitly defined ordinary-inline copy
assignment across all Float3 consumers may differ from isolated descriptor
helpers. This must preserve the natural three scalar stores and return contract,
without forced inlining, register hints, declaration reordering or fake uses.

- [x] Actual global ordinary-inline Float3 assignment found in Oct22
      probe_copy_family.py, not merely descriptor-local helpers. Historical
      source binding/results are being checked before any duplicate trial.
- [ ] Check original copy patterns and current class ABI across several owners.
- [ ] Stage only if a distinct, semantic model is justified.
- [ ] If compiled, compare the complete carrier and all affected exact functions.

## Results and next queue

No new exact owner or byte credit has been established by this checklist.
Items may be added, split or rejected when fresh evidence warrants it. Each
completed trial must retain source/provider hashes, exact compile command,
object and complete comparisons, plus a concise conclusion here.

Checkpoint CI passed 1016 tests with 29 skips; live tracking, documentation,
build graph, progress SVG and whitespace gates passed. Receipt:
.analysis/gpt-dots-20261024-resume/checkpoint-ci.log and .exit.

H03 and H05 each have an executed verify_no_compile.py:479 checks, fresh
complete COFF/target observations and75 canonical proofs reproduce saved results.

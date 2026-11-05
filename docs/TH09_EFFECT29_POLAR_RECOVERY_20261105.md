# Effect29 polar investigation and Type6 source recovery — 2026-11-05

The first adopted checkpoint restores `ExAttackUpdateCallbackType6 @00446060`
to a reproducible current-header build: all 698 bytes and 30 fields match.
This repairs an existing exact unit; totals remain 1021/1035 functions and
259829/291883 reviewed-authored bytes (89.0182%), with14 nonexact functions.
The second adopted checkpoint recovers the complete Type29 draw extent:
2220 bytes, 129 original-position fields, 72 calls and 41 matching physical
blocks, with 92 ordinary unlike bytes. Type29 remains nonexact. All 130 affected
canonical units retain exact codegen after the shared polar API migration.

## Target and interrupted source evidence

The session starts from clean `30a2b755`. The original Japanese v1.50a target
passes SHA-256 verification:
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Fresh Ghidra exports of Type29 and Type6 attest the disk/Program identity,
i386 language, base00400000, entry0047D45F and all685056 mapped file-backed
bytes. Ghidra names/prototypes remain provisional semantic evidence.

Following the shared polar callers exposed a real pre-existing failure:
unchanged Type6 source fails current-header compilation. It redeclares the
shared GameManager with an incompatible private type, then accesses the retired
Player `anmFileBC` spelling. A frozen, unmodified source/header control produces
the same errors before any polar-interface migration. The failure is not an
ABI-migration regression and is not silently excluded from the caller cohort.

## Adopted Type6 interfaces

The source removes its conflicting GameManager/side/Player declarations and
uses the already shared GameManager and Player storage. Player's ANM resource
is `header24.anmFile98`, physically Player+BC; a compile assertion checks the
sum. The native `BulletSpawnDescriptorCtorView` constructs the offset-zero
real descriptor, preserving the original28-byte constructor call. Merely
constructing the POD descriptor loses that call and gives687/29, so that
intermediate control is rejected.

The callback now uses five C++ identities that independently predate its own
canonical entry:

| Interface | Original destination |
| --- | --- |
| BulletSpawnDescriptorCtorView constructor | 0040D500 |
| PlayerLifecycleView::CheckBulletCollision | 0041DFF0 |
| AnmVm::SetInterrupt | 00406790 |
| Genuine fastcall AddNormalizeAngle | 0042AED0 |
| Shared GameManager global | 004A7D90 |

All original field offsets, kinds, addends and destinations remain. Full
698-byte/30-field comparison is zero-difference, with19 calls and18 blocks.
The diagnostic rejects the retired private aliases even at the same numeric
addresses. Its shared decoder import is deferred until the private CLI verifies
the target, so the native identity test runs without optional site packages.
No allocation-failure branches or defensive game behavior are added.

The caller, descriptor constructor, seven actual ANM-runtime units across two
profiles, and both angle-runtime units compare exactly:11 units,1359 physical
bytes and60 fields across five actual object carriers. Seven stale private
label spellings in the ANM draw sibling are refreshed only after their actual
same-section COFF definitions and immutable original PE destinations agree.
The entire owned switch-table tail is included. These are existing exact units,
not new exact-function credit.

## Adopted Type29 source and physical polar interface

The initial source reproduces 2205 of 2220 bytes, 129 fields and 72 ordered
calls, with 1170 overlap differences plus 15 missing bytes. Original target
instructions reload the local center receiver six times between polar conversion
and accumulation; vertex receivers are retained. The old Float3 member API
emits no such local reloads.

Cirno-Labs/th09 `d7653152becabb8c86f2f43d49015ecc11bd0652` provides a source
hypothesis in `pl09.cpp` using SDK local vectors and a two-dimensional free
polar function. The reference alone establishes no TH09 fact. Full local
original-bound comparisons support an explicit `Float2 *` receiver in ECX and
two stack floats, with a physical `ret 8`. The maintained `SetPolar2D` API
expresses that contract. Original nominal types/names, static/global/member
ownership and original translation-unit boundaries remain unknown.

The migration reuses the sole existing FSINCOS primitive: only its source
signature and the assembler operand naming its receiver change. No primitive
instructions, assembly blocks, clobbers, register hints, inert padding or
compiler profiles are added. Its complete 32 bytes at441890 remain original-
equal. The unused old declaration in Float3.hpp has no callers or definition;
removing that shared-header declaration is outside this bounded replay scope.

Type29 uses the existing 20-byte native fan vertex and actual SDK D3DXVECTOR3
local centers. An ABI assertion checks both XYZ layouts and 12-byte size.
The selected object is the existing EclManager Type11 `/O2 /Ob0` carrier,
`build/matching/EclManager-Type11.obj`. Its main `/Ob1` copy remains uncredited.
Complete comparison yields 2220 bytes, 129 correctly positioned fields,
675 instructions, 72 original ordered calls, 41 identical physical blocks and
92 unlike ordinary bytes. All six local-center ECX reloads and zero vertex
reloads now agree. Actual same-object SDK constructor4010B0 (26 bytes) and
accumulator405730 (36 bytes), plus the native polar primitive (32 bytes),
match their complete original bodies with zero fields. SDK alias binding
requires those actual definitions, never just a name or address. These aliases
add no authored/origin or exact-function credit.

This is a reviewed frontier transition: 1089 formerly unlike positions become
equal, 11 formerly equal positions change, and all 15 missing tail bytes return.
The new guard protects all 2128 equal positions and limits differences to the
92 observed positions. It also requires the full extent, original field
positions, ordered calls, physical graph, receiver observations and three
same-object callee proofs. The changed 11 old matches are explicit rather than
hidden by an enlarged old mask. Remaining differences chiefly concern stack
homes, a store-order pair and floating-add operand order. Natural narrower
angle scope, a const step, mixed native/SDK types and reversed step accumulation
do not improve the 92;
no declaration-order or register forcing is adopted.

## Affected callers and bounded behavior countercheck

All 156 polar call sites are migrated. Serial lowered-priority canonical builds
cover 29 affected source/profile groups. Identical BulletManager profiles share
one compiled object copied to their existing canonical artifact paths; every
comparison still reads its declared actual object. All 130 canonical units pass
47047 physical bytes and 2371 fields. Thirty private label names are refreshed
only after actual same-section definitions and original PE operand bytes agree;
field offsets, kinds, addends and destinations remain fixed. No full cold
repository replay is performed. The separate ten unchanged Type6 dependencies
reuse their source/profile/object proof and pass the complete byte checks too.

Main RunEcl retains identical raw and linked hashes and the full prior list of
30 differences at15564 bytes/598 fields, with zero field/table differences.
Enemy update retains all13 original difference offsets at3900 bytes/97 fields.
The focused driver passes Type34 draw, Type26 update, Type19 update, both large
owners, Type29 and the accepted biased-movement leaf with its actual callees.

The existing bounded draw emulator's 26 fixtures all agree on helper-call
chronology, fan payloads, fixture RAM, return and preserved nonvolatile registers;
it covers all672 reachable original instructions. Arithmetic executes the
read-only original helper bodies; only rendering is intercepted. Candidate raw
hash binds this private countercheck to the adopted canonical Type29 body.
This is a scoped reconstruction countercheck, not a native-product, complete
runtime, dual-Oracle semantic, Factory or byte-exactness gate.

## Evidence and reproduction

Ignored `.analysis/effect29-recovery-20261105/` retains frozen inputs,
Ghidra attestations, source/profile/object hashes, the pre-existing Type6 failure,
rejected controls, complete reports and reproduction recipes. `build-current.py`
builds each affected source/profile once; `verify-current-exact.py` compares all
130 declared canonical units and validates any private-label refresh. The latter
fails if the frozen source closure or any field contract changes.
`replay-frontiers.py` reads the already rebuilt canonical ECL profiles and runs
the focused complete-owner driver. `verify-type6-current.py` preserves its
immutable first-checkpoint receipt and writes later replays separately.

```bash
python3 -B scripts/build-match-unit.py --unit ecl-move-random-biased
python3 -B scripts/inspect-effect-type29.py build/matching/EclManager-Type11.obj --json
python3 -B scripts/check-boss-frontiers.py --owner effect29
python3 -B scripts/compare-coff-function.py --unit exattack-type6-update --json
```

The first Type6 source checkpoint is `ba92c53d`, with its immutable receipt
recorded in [source-bound receipts](RE_ACCEPTED_RECEIPTS.md). The later polar
migration source checkpoint is `4586b675`, with its own immutable
source-bound proof in that same receipt index; neither is Factory acceptance.
Totals stay 1021 exact /14 nonexact and 259829 reviewed-authored exact bytes
(89.0182%). Whole Windows product/runtime and portability remain open.

The second checkpoint passes full target-independent CI: 1351 tests with
29 skips, plus target/tracking, documentation, progress and whitespace checks.

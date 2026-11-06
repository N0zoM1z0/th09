# RunEcl native wire snapshot, 2026-11-06

The jump handler now snapshots the existing four-byte `Th09EclWireArgument`
and assigns its integer member to the active context timer. This replaces the
scalar `memcpy` transport and removes code-generation commentary from the game
source. The raw operand is not resolved through the parameter mask. The real
`ZunTimer::operator=(int)` interface remains in use.

Fresh direct Ghidra analysis covers RunEcl4086C0 and Timer assignment401500;
disk and Program SHA-256,i386,base400000,entry47D45F and all685056 mapped bytes
agree with the original Japanese1.50a. This is semantic evidence only. The
original setter is4014D0/25 bytes;401500/9 is its tail wrapper.

The maintained complete RunEcl remains15564 bytes/598 fields with30 ordinary
unlike bytes, zero field differences and zero table differences. Every raw
byte and actual external/local field identity agrees with the preceding
398b69e4 ECL object; private local label spellings may change, and their actual
same-owner positions are checked. Opcode4 still contributes7 unlike positions;
opcode155 contributes23. Enemy OnUpdate remains3900/97/13. No exact credit is
added and neither residual is claimed solved.

Six affected source/profile builds pass91 complete exact comparisons36301
bytes/1783 fields. These include both existing ECL profiles and the independent
EclHelpers,EclOperands,EclInitializeSubroutine,EnemyManagerClear owners.
Twenty-nine unchanged callee/alias proofs2027/71 are reused after verifying all
transitive inputs, existing object hashes, profiles and SDK/provider hashes.
No cold replay or whole-product/runtime validation is performed.

Under the alternative /Ob0 exact-leaf carrier, the same natural snapshot
removes one noninlined RawInt call. RunEcl there is11980 bytes/649 fields,
versus11980/650 before; it is not a target-complete RunEcl reconstruction.
Its six canonical exact leaves and complete Enemy owner are preserved. This
alternative carrier is not misreported as unchanged original-bound RunEcl.

## Bounded current-source controls

Eight experiments use the existing /O2 /Ob1 /Oy- profile without hints, added
qualifiers, forced inlining, assembly, barriers or compiler-option searches.
All eight preserve77 exact siblings each:616 repeated complete comparisons
194944 bytes/8768 fields. They are not616 distinct functions or new credit.

| Natural source model | Complete RunEcl ordinary/field/table differences |
| --- | --- |
| Four-byte wire-argument snapshot |30/0/0; adopted|
| Word-valued flag helper with retained byte copy |30/0/0|
| Word-valued helper and widened wire byte |2048/242/26|
| Existing generic masked assignment |1938/202/26|
| Native timeout bitfield from widened wire byte |1938/202/26|
| Direct word mask expression |1938/202/26|
| Genuine Timer assignment definition visible |30/0/0|
| Genuine setter and assignment definitions visible |15560/598;6230 unlike overlaps plus4 missing|

The first seven complete reports independently audit calls,data,literals and
both physical tables. Table differences are a subset of field differences.
For the setter-visible control, the standard diagnostic rejects the changed
physical shape. A separate private reader preserves that rejection and binds
every actual field by the reviewed external name/address identities and
actual same-owner positions. It accounts for all15560 candidate bytes, all598
fields and the four missing original bytes; it does not relax public gates.

The genuine visible Timer bodies remain original-equal: one9-byte wrapper in
the first context, plus a9-byte wrapper and25-byte setter in the second,
43 bytes/two fields across three comparisons. Their visibility still does not
recover the original caller. The original C++ TU boundaries remain unknown.
All eight complete Enemy byte/field identities equal the maintained owner;
the setter-visible standard Enemy diagnostic rejects its co-visible RunEcl
inventory, so that raw identity is checked separately without changing it.

The diagnostic source recipes now accept the native wire snapshot as input
while retaining their historical scalar-copy experiments. They reject changed
wire width, mixed/duplicate snapshots or a changed timer consumer. These are
probe-integrity checks, not game-source spelling acceptance tests. Staging
coverage uses an explicit fixture edit instead of depending on a game local's
spelling. The game source is never rewritten by these probes.

The ignored packet is `.analysis/runecl-native-expression-20261106/`; fourteen
new objects are retained in `build/analysis/runecl-native-expression-20261106/`.
Frozen inputs, full COFF rows, original-bound diagnostics and reproducible
recipes are retained. Current source/receipt bindings and cleanup are recorded
in the follow-up checkpoint. Authored totals remain1034 source-present/1021
exact/13 nonexact; product/runtime, semantic and port gates remain open.

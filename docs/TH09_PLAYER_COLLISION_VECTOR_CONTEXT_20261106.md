# Player collision geometry and native vector contexts

`CheckBulletCollision` now computes one `intersects` result for its three
geometry branches. Three `goto hit` transfers and their label are removed.
It uses the actual `this` receiver and `bullet` parameter directly; the unused
second parameter remains an unnamed ABI stack word. One cancellation label
and its two explicit type branches remain. The complete849 raw bytes and all
26 actual COFF rows reproduce the preceding source exactly. This is natural
source cleanup within a still non-exact owner,not new exactness credit.

The sole target remains original Japanese TH09v1.50a,SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Fresh direct Ghidra exports cover collision41DFF0 and rotation42AF40,with
disk/Program SHA,i386 language,base400000,entry47D45F and all685056 mapped bytes
attested. Rotation reads input X/Y and writes output X/Y; no Z access appears.
That does not establish the original nominal vector type or caller-local size.

The original collision body remains862 bytes/277 instructions/26 fields/12
ordered calls/33 blocks/three RET12 sites. Its full SHA-256 is
`9dcfe1aca4ac06526072014ee6f956120d9c1d84cc1bba044638556b9b82445c`.
The maintained candidate stays849/266/26/12/32,with799 unequal overlapping
positions and13 missing bytes. The four actual read-only scalar definitions,
including double zero,remain checked. The new conjunction of negated AABB
comparisons preserves the existing floating-point predicates; it does not
replace them with ordered inequalities that would change NaN handling.

Ten fixed-profile whole-source controls produce these results:

| Model | Complete bytes/fields | Unequal overlaps / missing | Raw baseline equal |
| --- | --- | --- | --- |
| Existing Float3 geometry locals |849/26|799/13|yes|
| Existing Float3 geometry and effect locals |849/26|799/13|yes|
| Float3 workspace reused by geometry and effects |849/26|799/13|no|
| Response nested in successful circle branch |849/26|799/13|yes|
| Positive eligible-region scope |849/26|799/13|yes|
| Both positive scopes |849/26|799/13|yes|
| Direct receiver/input and unnamed unused argument |849/26|799/13|yes|
| One geometry result |849/26|799/13|yes|
| Geometry result plus compound type early return |849/26|804/13|no|
| Geometry result plus direct receiver/input,adopted |849/26|799/13|yes|

The ordinary empty Float3 constructor and scalar type changes do not explain
the target loop layout. Moving the source response block and changing its
positive scopes also canonicalize to the maintained body. The workspace still
recovers previously observed scratch homes without recovering the whole owner.
It is not adopted. The compound type guard loses five previously equal target
positions and gains none; the two explicit type branches remain. No register,
declaration-order,barrier,assembly,padding,new volatile,forced-inline or compiler
profile tactic is introduced. These controls do not exhaust natural source
models or identify unique original declarations.

All50 repeated exact-sibling comparisons pass. Only this source is rebuilt for
the adopted version. Five current exact siblings1220/32 and eleven direct
callees2260/106 pass16 strict public comparisons3480/138. Nine unchanged callee
carriers reuse their source/profile/object-bound proofs. No canonical match
recipe or match row changes. The collision ledger is refreshed to distinguish
the current structured source from historical goto/receiver-alias experiments.

The old source-connection test incorrectly required the spelling `player` for
the receiver alias. It now resolves `this` or a declared local alias and the
cursor name while preserving the actual owner/pool/field storage checks. An
additional negative test rejects another Player receiver. Nineteen focused
collision tests pass. Source spelling remains independent of compiler equality.

The ignored packet `.analysis/player-collision-vector-context-20261106/` freezes
the22-input8f8da4c0 epoch and the verified preceding object/source proof.
`current-proof.json` retains all ten complete controls,the fresh current object,
all actual COFF rows,16 strict unit comparisons and unchanged dependency proofs.
Objects remain below `build/analysis/player-collision-vector-context-20261106/`.
The reconstruction goal and later product/runtime/semantic/port gates remain
open. Totals remain1034 source-present,1021 exact and13 non-exact. No Factory
acceptance or independent remote-publication claim is made.

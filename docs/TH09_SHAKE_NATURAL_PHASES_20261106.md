# Shake envelope natural phase models, 2026-11-06

This bounded investigation starts at source epoch74457145, game source7e293460.
All16 current ScreenEffect inputs still match the immutable398b69e4 native-RNG
proof. The maintained owner remains336 bytes/20 fields/26 unlike positions;
no game source, ledger or acceptance guard changes.

Fresh direct Ghidra exports cover CalcShakeEnvelope4227A0 and the actual
RegisterChain entry422D20. Disk/Program SHA-256,i386,base400000,entry47D45F and
all685056 mapped bytes agree. An initial request incorrectly selected422DB0
inside RegisterChain; that export fails closed. Its failure/request remain in
the packet, separately from the successful reviewed-entry run. No target bytes
or function metadata are patched.

Original phase arithmetic fetches hold+20 and fade+24 independently, adds them,
then extends the signed comparison endpoint. The finalized endpoint is
converted as unsigned and rounded/spilled to float before Timer's float
conversion; the unsigned fade divisor is read after that call. These are TH09
instruction observations, not proof of original C++ volatility or declarations.
In particular, the old source comment about original volatile reads is not an
established original qualifier fact.

Three natural models remove the existing volatile reference construction:

| Model | Complete bytes/fields | Unlike overlaps | Missing/extra bytes |
| --- | --- | --- | --- |
| Ordinary hold+fade sum extending the phase endpoint |334/20|249|2/0|
| Separate unsigned final-phase endpoint |339/20|249|0/3|
| Function-local attack/hold/fade parameter interpretation |334/20|249|2/0|

Every candidate binds all20 actual fields to independently reviewed Timer/RNG/
global/literal identities, decodes the complete physical body and retains all
eight ordered calls and26 direct control-flow blocks. All missing/extra bytes
remain explicit. Matching call/graph counts do not establish runtime equivalence
or exactness. The ordinary and typed-parameter bodies have identical complete
raw bytes and field identities; the typed view does not recover extra code.

The ordinary expression folds the hold load into an ADD memory operand. It
uses one fewer instruction than the original separate two loads plus ADD and
shifts the following physical code. The unsigned endpoint model instead has
an extra instruction. None preserves the maintained310 equal positions, and
none is adopted. No declaration-order, register, volatile-qualifier, forced-inline,
assembly, barrier or compiler-profile tactic is introduced. These three
rejections do not exhaust natural source models or determine original types.

Each control preserves all16 canonical siblings:48 repeated complete
comparisons4665 bytes/231 fields. The unchanged three canonical ScreenEffect
profile objects are reused for18 comparisons2562/164 after source/profile/
object/provider checks. Actual Timer increment,float conversion,less-than and
RNG range owners pass four comparisons66/2; Timer less-than is freshly built
under its existing /Ob0 profile, the other three reuse unchanged source proofs.
The total70 comparisons7293/397 are not70 distinct functions or new credit.

The packet is `.analysis/shake-native-envelope-20261106/`. It binds19 current
source/Git-blob inputs including callees,eight provider/SDK files, three new
control objects and the new Timer object. Control seal SHA-256:

`2bda1cc0aa0f4cca878046dfb8ee1c3eae5a6c80bdcd75461394be12a9a36165`

`python3 -B .analysis/shake-native-envelope-20261106/seal.py` rechecks the
complete packet read-only. `replay-one.py native-phase-parameters` independently
rebuilds the frozen context, reproduces complete raw bytes, all actual COFF
rows,16 exact siblings and the full original-bound diagnostics, then removes
its disposable build.45 SHA-identical staged source copies and four PDBs524288
bytes are pruned; all four new objects, changed sources and frozen inputs remain.

The preceding game-source checkpoint passes full public CI1377/29. This
private experiment/documentation batch runs focused validation.
Tracking,progress,documentation and35 focused status tests pass. Authored
counts remain1034 source-present/1021 exact/13 nonexact. The reconstruction goal,
product/runtime, semantic and port gates stay open. No Factory acceptance or
independent remote-publication claim is made.

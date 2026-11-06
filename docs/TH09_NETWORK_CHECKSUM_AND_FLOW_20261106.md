# Network frame checksum and complete flow review, 2026-11-06

The maintained SupervisorServiceUpdate checksum now names the packet's16-bit
input field. It previously added the32-bit input/checksum union and discarded
the final sum's upper word. For any side/frame/seed sum S,input I and old
checksum H, the stored value satisfies:

`(S + I + 65536*H) mod65536 = (S + I) mod65536`.

The sum remains unsigned32 arithmetic followed by the existing unsigned-short
store. The natural field expression does not depend on an old checksum value.
The compiler still selects the original-style DWORD read; that machine width
does not identify the unique original source expression.

A fresh current-source build preserves every1627 raw byte and all106 actual
field identities of the source398b69e4 service object. Anonymous namespace salt
spellings are normalized only after binding their reviewed packet consumers;
no address,addend,field kind or offset is ignored. Complete original1633-byte
comparison remains1381 unlike overlaps plus six missing bytes. It is nonexact,
with no new function credit or changed ABI/profile.

Fresh direct Ghidra rechecks service430AA0 and receiver432B70 with disk/Program
SHA-256,i386,base400000,entry47D45F and all685056 mapped bytes attested. The
receiver's type0 branch copies the12 wire bytes, then queues input+8 and seed+2.
This branch contains no checksum comparison. This bounded observation does not
claim that no other game code ever reads the copied checksum, and no receive
behavior is added.

## Current-source control-flow observations

The first25 physical direct calls already agree in the maintained service.
Ordered mismatches are confined to sequence positions25..30: the original wait
LeaveCriticalSection call precedes Controller input and frame transmission,
while the maintained compiler moves that return block behind synchronization.
All41 direct-callee multiplicities agree. The original20-byte local frame also
retains result at EBP-4, unlike the maintained12-byte frame.

Source branch order alone does not determine those physical blocks. Swapping
the predicted/synchronous source arms reproduces every maintained raw byte and
referent. Two separate structured early-wait returns also reproduce them.
Moving the successful-frame source block earlier changes the generated owner
and worsens the complete original boundary. No source-placement inference is
accepted from Ghidra statement order alone; packet-side publication was checked
against instructions and left intact.

Ten successful bounded controls use the existing /O2 /Ob1 /Oy- profile:

| Model | Bytes/fields | Unlike overlaps | Missing/extra original-relative bytes |
| --- | --- | --- | --- |
| Synchronous source arm first |1627/106|1381|6/0|
| Successful queue path first |1615/106|1376|18/0|
| Both positive source paths first |1615/106|1376|18/0|
| Complete per-frame input/output/status aggregate |1645/106|1325|0/12|
| Aggregate with successful path first |1638/106|1327|0/5|
| Two structured early-wait returns |1627/106|1381|6/0|
| Input-word checksum term |1627/106|1381|6/0; adopted|
| Early-wait returns with complete aggregate |1657/107|1323|0/24|
| Actual GetSeed definition visible |1622/105|1500|11/0|
| Actual GetSeed and range definitions visible |1622/105|1500|11/0|

The aggregate is a single coherent grouping of actual inputs,disjoint phase
outputs and status, with no padding or field-order search. It retains a result
home at EBP-8 and fails to recover the original EBP-4 home. The combined early-
wait aggregate emits an extra real unlock call:42 versus41. Its additional
canonical destination is bound, retained and rejected, not hidden or assigned
a fabricated target. A first combined-control setup missed one rewritten
result name and exited2; its source/log remain separately from the recovered
complete model. That setup failure is not a compiler counterexample.

Genuine visible GetSeed4 and range34 bodies remain original-equal under the
caller profile: three repeated producer comparisons42 bytes/one field. All41
calls remain. The visible getter lets C2 remove one packet-frame+4 reload,
reducing106 fields to105 and making the caller less faithful. Current source
keeps the genuine separate owners. This constrains this fixed-profile context;
it does not uniquely establish original TU boundaries or field qualifiers.
No assembly,barriers,new volatile,forced-inline,register/variable-order hints,
fake callee bodies or compiler-profile searches are introduced.

## Focused verification and reproduction

Fourteen fresh source/profile builds cover the maintained caller and16 exact
callee units. Four unchanged source/profile/object proofs are reused: RNG seed,
RNG range,Controller input and the complete808-byte receiver with its physical
tables/tails. The20 complete exact comparisons total3767 bytes/131 fields:
19 direct-callee units2959/85 plus the receiver808/46. These are existing exact
units, not20 new matches. All37 transitive source inputs and13 selected compiler/
SDK files, including DirectPlay and Windows headers, are hashed.

The ignored packet is `.analysis/network-sync-flow-20261106/`. It retains all
24 new objects, frozen/changed sources, both fresh Ghidra exports and complete
PE/COFF reports. Source/receipt bindings, cleanup and independent read-only
verification are completed in the follow-up checkpoint.57 focused network/
input/tracking/progress/origin/documentation tests pass. Target-required
tracking,progress and whitespace pass. Full public CI is not repeated for this
bounded source/metadata change; no shared validator changes.

Authored totals stay1034 source-present/1021 exact/13 nonexact. Product/runtime,
semantic and port gates remain open. Local checkpoints do not establish Factory
acceptance or independently verified remote publication.

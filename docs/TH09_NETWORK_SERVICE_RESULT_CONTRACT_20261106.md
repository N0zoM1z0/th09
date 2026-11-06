# Network service result contract and complete controls, 2026-11-06

The service430AA0 and its996-byte caller431110 now share one declaration and
named result values in `SupervisorNetworkService.hpp`. The physical int/fastcall
interface stays unchanged. The names describe independently observed caller
actions, not recovered original enum spelling or nominal return type.

| Value | Shared source name | Observed caller action |
| --- | --- | --- |
|0|FRAME_READY|Leaves the retry switch and continues the update|
|1|RETRY_FRAME|Calls the service again|
|2|WAIT|Returns3 from the Supervisor callback|
|3|RESET|Resets/inactivates the session, clears connection state and selects state1|
|4|STOP|Resets the session and returns4 from the Supervisor callback|

The complete nonexact service remains1627 bytes/106 fields against original
1633:1381 unlike overlaps plus six missing bytes. Its entire raw body and every
actual referent are unchanged, and its one emitted regular body/auxiliary/EH
payload matches the preceding source-bound baseline. No API-width, status-
lifetime, workspace or return-layout experiment is adopted.

## Fresh target and source observations

Direct Ghidra rechecks service430AA0, actual PopFrame42EB80 and caller431110.
Both exports attest original disk/Program SHA-256, x86 little-endian32,
base400000, entry47D45F and all685056 mapped bytes. Ghidra remains semantic
evidence. The whole caller also passes complete original-bound VC7.1 replay.

The original service allocates20 local bytes. Its result is initialized at
EBP-4, set to1 on the wait path and reloaded after the wait/normal unlocks.
Prefill output addresses are seeds at EBP-C/-A and predicted at EBP-8;
normal-frame outputs are seeds at EBP-8/-6 and predicted at EBP-C.
Normal input stores write AX at EBP-14/-12, then later read DWORDs from those
same locations. These are actual instruction widths and positions. They do
not uniquely identify the original input array element or callback C++ types.
No runtime reads, defensive changes or invented high-word values are added.

The existing paired unsigned-short input/result producer is independently
rechecked over all131 bytes/one field. Its actual source/header hashes, profile,
object and immutable prior seal are reused; no producer recompilation is needed.
Its symbol is associated with42ACE0 only after full actual producer equality.
The maintained int/int producer remains unchanged. This countermodel does not
establish a unique original nominal API.

## Twelve complete fixed-profile controls

All controls use the service's existing profile, serial low-priority bounded
compilation and14 frozen inputs. Every actual code byte and relocation is
bound before comparison; missing/extra bytes remain explicit. The private word
observer adds only the proven producer symbol/role and leaves the public gate
unchanged. No assembly, barriers, volatile forcing, register/variable-order
hints, fake bodies, padding or compiler-profile search is used.

| Natural source model | Bytes/fields | Unlike overlaps | Missing/extra |
| --- | --- | --- | --- |
| One shared result local |1627/106|1381|6/0|
| Shared wait/normal unlock-return |1639/106|1326|0/6|
| One unlock-return for all results |1629/104|1172|4/0|
| Word API and shared frame return |1641/106|1326|0/8|
| Word API and single unlock-return |1631/104|1175|2/0|
| Two scalar side inputs |1625/106|1387|8/0|
| Word API and scalar inputs |1619/106|1378|14/0|
| Word API, scalar inputs and frame return |1631/106|1322|2/0|
| Single-pass service loop/frame cleanup |1639/106|1326|0/6|
| Word API and single-pass service loop |1641/106|1326|0/8|
| Word API and integer input snapshot |1621/106|1377|12/0|
| Word API, integer inputs and frame return |1633/106|1279|0/0|

The two single-unlock models have39 physical direct calls instead of41; their
two missing unlock call fields are retained as real differences. Other models
retain all41 callee multiplicities, but none recovers the original call order.
In particular, matching total1633-byte length in the final model is not a match:
its frame is16 bytes, result home is EBP-C, and1279 overlapping bytes differ.

Merging only the two status locals reproduces every maintained raw byte.
Shared frame returns keep the result in memory but at EBP-C rather than EBP-4.
The single-pass loop reproduces its corresponding explicit cleanup model;
that source control structure does not explain the original wait-block layout.
Scalar/int input representations likewise fail to recover the target. These
negative measurements constrain the tested contexts, not all natural source.

## Current-source validation

Only the two affected source owners rebuild. Twenty unchanged exact direct-
callee/receiver proofs reuse verified transitive inputs, profiles and objects.
Strict public replay passes21 existing units4763 physical bytes/223 fields,
including caller996/92 and receiver808/46. Forty caller-local compiler label
spellings refresh after complete equality at their actual owner-relative
positions; no destination, addend, field kind, offset or extent changes.

The focused network/native-owner/queue/caller suites pass43 tests/173 subtests.
Target-required tracking/progress, documentation and whitespace checks accompany
the checkpoint. No new test requires a particular enum spelling or formatting.
The ignored packet is `.analysis/network-service-status-flow-20261106/`.
All12 full controls, actual fields/calls/instruction widths, current builds,
unchanged proofs and Ghidra attestations remain reproducible.

Counts remain1034 source-present/1021 exact/13 nonexact. There is no new exact,
product/runtime, semantic, portability, Factory or remote-publication credit.
The reconstruction goal and those later gates remain open.

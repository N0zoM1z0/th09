# Complete network service frame contexts, 2026-11-06

## Complete pinned-reference countercheck after 885cc874

The complete d7653152 `app_network.cpp`, built with the pinned local compiler
and its recorded `/O2 /Ob0 /Oy- /Gr /GX` profile, produces 1610 bytes against
the original 1633. All 106 actual fields are bound; every external destination
also exists in the independent native baseline. The three actual CP932 string
definitions match their original payloads. Complete comparison has 1478 unlike
overlaps and 23 missing bytes. Its 41 call multiplicities agree, while their
order and the flow remain nonexact. It does not recover the original stack or
the two observed input-selector preparations and is not adopted.

Fresh original review also confirms partial-word input stores followed by
dword loads at 4310C5/4310D1. A paired unsigned-short input/result producer
again passes all 131 bytes and its field, but its native service caller is
1629 bytes / 106 fields / 1388 unlike overlaps / four missing bytes. This is a
current countercheck of the October 31 width ambiguity already recorded in the
ledger, not new proof of unique source types. The canonical int/int interface
and 1627/106/1381 service baseline remain unchanged.

Fresh Ghidra covers service430AA0, producer42ACE0 and queue42EB80 with all
685056 mapped bytes attested. The ignored packet
`.analysis/network-reference-whole-20261106/` retains three objects, the 17-input
native control, complete comparisons and recipes. 79 SHA-identical reference
copies are pruned against the retained pinned mirror. Independent seal replay
passes `adf4581b2d28eef2eb80643a7453e213019ef8b4c679952abcd28b880fe323b5`.
No source adoption or exact credit; authored totals are 1034 present / 1021
exact / 13 nonexact. The remainder below describes its earlier source epoch.

## Earlier native frame contexts

The restored observed input selectors in source `f350f750` are the baseline.
Fresh target-bound Ghidra and direct original-byte decoding show a 20-byte
local frame, a result home at EBP-4, and different output homes in the two
PopFrame phases: prefill uses seeds at EBP-C/-A and predicted at EBP-8;
the successful frame uses seeds at EBP-8/-6 and predicted at EBP-C. The current
source compiles with 12 local bytes and optimizes the result out of memory.
These observations motivate lifetime/output-storage questions; they do not
prove an original aggregate, union, declaration order or volatile qualifier.

Five natural complete-source controls use the same existing service profile,
serial lowered-priority bounded compilation, and a frozen 19-file source
closure. Every comparison binds all 106 actual COFF fields against the full
1633-byte original owner. None is accepted into production.

| Complete source context | Candidate bytes | Unequal overlaps | Local bytes |
| --- | --- | --- | --- |
| Maintained baseline, preserved separately | 1627 | 1381 | 12 |
| Adjacent seed arrays in each phase | 1627 | 1381 | 8 |
| Phase-specific output union with shared result | 1639 | 1326 | 16 |
| Four actual queue definitions co-visible | 1627 | 1381 | 8 |
| Phase workspace and actual queue definitions | 1634 | 1321 | 16 |
| Phase workspace, successful frame before timeout work | 1627 | 1321 | 16 |

All five retain the original 41 direct-callee destination multiplicities;
none retains the complete ordered call sequence. The union keeps a memory
result but places it at EBP-8 and does not recover the original frame.
Smaller overlap counts in differently laid-out owners are not exactness
percentages or semantic-equivalence proofs. The caller remains unresolved.

Both co-visible contexts independently reproduce the actual four queue bodies:
AreFrameQueuesSynchronized 49/0, PopFrame 119/1, InsertPredictedFrame 191/0,
and InsertReceivedFrame 213/0, expressed as complete bytes/relocation fields.
That is **572 bytes / 1 field** per context, **eight comparisons / 1144 bytes /
2 fields** in total. All ten service calls to those owners refer to actual
definitions in the same object. Matching callees do not establish the caller's
source shape or original translation-unit membership.

The pinned Cirno-Labs `App::UpdateNetPlay` was consulted as source-family
hypothesis material. TH09-local bytes, field identities and compiler results
establish every measurement here. Its spelling does not override the original
ECX selector preparations recovered in
[the input correction](TH09_NETWORK_INPUT_SELECTORS_20261106.md).

Ignored `.analysis/network-current-frame-20261106/frame-context/` retains
frozen inputs, five changed sources, commands/logs, full comparisons, actual
callee receipts, `case-index.json` and an independent `verify.py`. The latter
rechecks source commit/blobs, all source/object/report hashes, every full body,
all actual fields and the co-visible callees. `probe.py CASE` restores missing
SHA-verified unchanged inputs and writes a separate current replay rather than
overwriting the indexed evidence. Ninety identical per-case copies are pruned;
frozen/changed sources and all objects remain. One pruned co-visible context is
rebuilt to exercise that recovery path and reproduces its complete raw body,
actual fields and four exact callees.

The separate sealed baseline receipt is bound to source
`f350f750c5fb24f75a125e2053813f75cd533644`, with SHA-256
`dcb6b7b906a738f7444fbc7bd30d4a54cb7f00e0950fed079fad50a6e82b31da`.
It retains the seven canonical exact input units, the complete nonexact
service report, 33 tracked inputs/Git blobs, six actual objects/profiles,
eight compiler/SDK hashes, full Ghidra attestations and public CI log.
`../source-receipt.py` independently verifies that immutable receipt without
rebuilding unchanged canonical units or overwriting its reports. The earlier
unaccepted receipt/recipe with an absolute-versus-relative diagnostic path
check failure are preserved separately; the byte comparison was unaffected.

No further production source, compiler profile, matching row or exact counter
changes. Totals remain 1035 source-present / 1022 exact / 13 nonexact.
Target-required tracking, progress, documentation and whitespace checks pass;
the source checkpoint's full public CI passes 1364 tests with 29 skips.
Product/runtime, semantic reconstruction, portability and Factory acceptance
remain independent open gates.

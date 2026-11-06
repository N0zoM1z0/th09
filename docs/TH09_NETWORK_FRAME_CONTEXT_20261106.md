# Complete network service frame contexts, 2026-11-06

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

# Network input call contract, 2026-11-05

This source continuation follows `06f89ba1`, the native Supervisor input owner.
Fresh target-attested Ghidra covers the full service430AA0 and input42BE40
owners; disk/Program, i386, base/entry and all685056 mapped bytes agree.

The maintained GetInput implementation never consumes its index. Original
service calls at430EC8 and431046 also do not prepare an input index. A complete
physical E8 scan of.text finds these two candidates only; no absolute42BE40
DWORD occurs in the original file. That census does not exclude computed
indirect calls, and the original C++ arity is not uniquely recoverable from an
unused register parameter.

The maintained interface now expresses the observed minimal contract:
`unsigned short __fastcall Controller::GetInput()`. Both service sites call
it directly, removing the unused side/index expressions. No overload, adapter
or invented argument value is added. The native keyboard/controller behavior,
return payload and other input/queue interfaces are unchanged.

All seven exact functions whose source includes Controller.hpp are cold-built
under their existing recorded profiles and strictly compared in full:
**3597 physical bytes/73 fields**, all exact. The819-byte GetInput retains its
nine actual fields and original operand values with the new decorated symbol.
No compiler-private label update is needed. Thirteen focused tests pass.

The entire service is also freshly rebuilt and linked through all106 actual
COFF fields. It now has1616 candidate bytes against1633 original bytes,1366
unequal overlaps and17 absent bytes. All41 original direct-callee destination
multiplicities agree, while ordered calls/control flow remain nonexact. The
prior1627-byte source had1381 unequal overlaps and six absent bytes. The full
physical-position delta is retained separately; neither aggregate counts nor
the minimal API correction authorize exact credit or runtime-equivalence claims.

The earlier real-body visibility and member/workspace controls do not close
the service; no synthetic clobber contract or owner switch is adopted.
Only the observed input interface is changed. Totals remain1019 exact/16
nonexact. Original nominal signature, whole Windows product/runtime and
Factory acceptance remain independent open questions.

Ignored `.analysis/input-contract-20261105/` retains before/current input hashes,
the seven canonical receipts, complete service comparison, physical delta and
serial compiler logs. `verify.py` rebuilds the actual affected sources, compares
each full unit and binds the service; `delta.py` independently compares both
full original-linked candidates. Objects are isolated under
`build/analysis/input-contract-20261105/`. Earlier controls and the physical
xref census remain in `.analysis/service-flow-20261105/`.
Tracking, progress, documentation and whitespace checks pass.

Only SHA-identical per-case inputs in the service-flow packet are removed,
with path/hash/recovery entries in duplicate-cleanup.json. Frozen inputs,
changed source controls, full receipts, logs and objects are retained; the
recipes recreate those copies.

# Observed network input selectors, 2026-11-06

Fresh original-byte review corrects the previous claim that the two service
callers do not prepare an input index. The original prepares ECX immediately
before **both** calls to `Controller::GetInput @ 0x0042BE40`:

| Original producer | Following call | Observed value |
| --- | --- | --- |
| `0x00430EC2: mov ecx,[edi+0xAC]` | `0x00430EC8` | Current network state's local side |
| `0x00431041: mov ecx,2` | `0x00431046` | Literal 2 |

On the first path, `0x00430E98` loads EDI from `g_SupervisorNetworkState`
at `0x004B42D0`; the packet-ready branch at `0x00430EA6` reaches the selector
load directly. Neither producer has an intervening instruction before its
call. The full original input body does not consume the incoming selector.
These facts support preserving the observed caller values; they do not
uniquely recover original C++ arity, parameter names or authorship.

The maintained declaration/definition again take `int inputIndex`, and the
service passes the current local side and literal 2. The input body gains no
selector-dependent behavior. Its configured symbol is
`?GetInput@Controller@@YIGH@Z`. This supersedes the target-interpretation and
current-interface claims in
[the November 5 note](TH09_NETWORK_INPUT_CONTRACT_20261105.md), whose receipts
remain evidence only for their historical source checkpoint.

Both Ghidra exports attest original disk and Program SHA-256, x86 little-endian
32-bit language, image base `0x00400000`, entry `0x0047D45F`, and all **685056**
file-backed mapped bytes. The full service and input extents are independently
checked against the original file. Ghidra is semantic evidence, not the byte
comparison Oracle.

Serial lowered-priority builds use the five affected sources' existing pinned
VC7.1 profiles. All seven configured complete functions pass **3597 bytes /
73 relocation fields**, including all **819 bytes / 9 fields** of GetInput.
Actual operands retain their original destinations; no private-label refresh
is needed. Source inputs are hashed before and after the builds.

The full service is rebuilt separately under its unchanged profile and bound
through all **106** actual fields. It has **1627 candidate bytes / 1633 target
bytes**, **1381 unequal overlaps**, and **six missing bytes**. All **41** direct
callee destination multiplicities agree; ordered calls/control flow still
differ. The prior no-argument source had 1616/1633 bytes, 1366 unequal overlaps
and 17 missing bytes. This correction follows the observed caller values;
the differing aggregate byte counts do not constitute an exactness advance.
The service remains nonexact and receives no new credit.

`scripts/audit-controller-input-calls.py` fails closed on the original target,
complete owner/callee hashes, both producer/call edges, and the physical E8
census. That census finds the two known direct candidates and does not exclude
computed indirect calls. Nine synthetic byte-contract tests reject changed
selectors, state field/root, ready edge, call target/opcode and owner extent.
They assert target observations rather than source spelling.

Ignored `.analysis/network-current-frame-20261106/` retains the attested exports,
`original-input-contract.json`, seven canonical receipts, full service report,
frozen controls, and serial logs. `verify-input-restore.py` reproduces the actual
affected builds and complete comparisons; objects are under `build/matching/`
and `build/analysis/network-current-frame-20261106/`. Earlier array, phase-union
and loop controls remain nonexact and are not adopted. Exact totals remain
**1022 functions / 262049 bytes**; 13 source-present functions remain nonexact.
Windows product/runtime, semantic reconstruction and Factory acceptance remain
separate open gates.

Full public CI passes **1364 tests, 29 skips**, including the nine new contract
tests. Target-required tracking, progress, documentation and whitespace checks
pass. These checks do not promote the incomplete service comparison.

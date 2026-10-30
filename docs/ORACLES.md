# Oracle policy

An Oracle is a replayable mechanism that can falsify a bounded claim. No single
view supplies every kind of truth.

During the exact phase, require target identity, reviewed ownership and complete
VC7.1 byte/relocation comparison for the affected functions. Native-product,
runtime and port rows below apply in their later phases. Allocation/null fault
injection and generic defensive-runtime coverage are not exactness requirements.
Documentation/test-only changes do not need target or compiler replay.

| Claim | Minimum useful Oracle | Acceptance boundary |
| --- | --- | --- |
| Target identity | size, SHA-256/MD5, PE structure | exact manifest agreement |
| Ghidra binding | disk/Program SHA-256, i386 language, image base, entry and all file-backed mapped PE bytes | exact target agreement before analysis and each export |
| Boundary/ownership | target control flow, references, tables, adjacent bytes | complete reviewed extent; unknown if ambiguous |
| Compiler generation | PE/Rich evidence plus pinned compiler probe | observed generation/build, not flags |
| Source presence | maintained mapping and source file | no exactness credit |
| Codegen exactness | cold VC7.1 object build, complete bytes and relocations | zero difference for full accepted extent |
| Native product closure | cold compile/link graph, imports/resources/data owners | no missing required production owner |
| Runtime semantics | deterministic scenarios against original and native products | both Oracles agree on declared observables |
| Port behavior | native baseline plus port scenario | separate portability claim |

Direct local Ghidra is the semantic-analysis tool, not an independent exactness Oracle.
Decompiler text, names, types, and candidate extents remain provisional. A thunk
can inherit its callee name and produce expanded callee pseudocode: at47B249,
Ghidra reports a5-byte body named _free,while the fixed CRT member proves scalar
operator delete jumping to the separate _free entry47B163. Check actual body
bytes,entry and provider identity before using such output as a call contract. A
successful build or visually plausible runtime also cannot waive missing bytes,
relocations, data owners, or scenario observables.

If a required input, extent, tool profile, or observable cannot be established,
record `unknown` or an open candidate. Accuracy takes precedence over apparent
completion.

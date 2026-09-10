# Oracle policy

An Oracle is a replayable mechanism that can falsify a bounded claim. No single
view supplies every kind of truth.

| Claim | Minimum useful Oracle | Acceptance boundary |
| --- | --- | --- |
| Target identity | size, SHA-256/MD5, PE structure | exact manifest agreement |
| IDA routing | IDA metadata, entry point, distributed mapped bytes | exact target agreement before analysis |
| Boundary/ownership | target control flow, references, tables, adjacent bytes | complete reviewed extent; unknown if ambiguous |
| Compiler generation | PE/Rich evidence plus pinned compiler probe | observed generation/build, not flags |
| Source presence | maintained mapping and source file | no exactness credit |
| Codegen exactness | cold VC7.1 object build, complete bytes and relocations | zero difference for full accepted extent |
| Native product closure | cold compile/link graph, imports/resources/data owners | no missing required production owner |
| Runtime semantics | deterministic scenarios against original and native products | both Oracles agree on declared observables |
| Port behavior | native baseline plus port scenario | separate portability claim |

IDA is the semantic-analysis provider, not an independent exactness Oracle.
Decompiler text, names, types, and candidate extents remain provisional. A
successful build or visually plausible runtime also cannot waive missing bytes,
relocations, data owners, or scenario observables.

If a required input, extent, tool profile, or observable cannot be established,
record `unknown` or an open candidate. Accuracy takes precedence over apparent
completion.

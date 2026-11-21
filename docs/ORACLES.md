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
| Owned initialized data | compare the defining COFF section's bytes and pointer identities with the reviewed target range | complete match for each explicitly checked initializer |
| C++ unwind contributions | replay the caller-associated COFF code and metadata against the original handler and unwind graph | exact handler, cleanup, state records and relocation identities |
| Native product closure | cold compile/link graph, imports/resources/data owners | no missing required production owner |
| Runtime semantics | deterministic scenarios against original and native products | both Oracles agree on declared observables |
| Port behavior | native baseline plus port scenario | separate portability claim |

The comparator reports checked compiler literals in `literal_definitions`
and checked named initializers in `data_initializers`. The EclHelpers
comparison checks all eight slots of the interpolation callback table.
The SaveReplay comparison checks the complete character, mode and
difficulty tables, including all 27 pointers and their Shift-JIS strings.
The DrawReplayMenu comparison checks the separate 112-byte title caption
section and all 28 string pointers. Its two mode references select the
same table at addends 0 and 4.
An operand targeting an external table establishes its address in the
original image. A table-content claim requires the separate defining-object
comparison. Native linkage requires a matched rebuilt PE and map.

Each canonical import relocation must select the uniquely named slot in the
original PE import address table, using DIR32 and a zero COFF addend.
Canonical units that reference a VC7.1 `__ehhandler$` also check the
associated `.text$x` and `.xdata$x` sections. The comparison follows the
original handler to FuncInfo and the unwind map, checks each cleanup offset
and resolves cleanup calls through independent symbol anchors. The JSON
`exception_handlers` field records this proof separately from authored
function coverage. The checked TH09 shape covers one- and three-state
unwind maps with no catch records.

Direct local Ghidra is the semantic-analysis tool, not an independent exactness
Oracle. Decompiler text, names, types, and candidate extents remain provisional.
A thunk can inherit its callee name and produce expanded callee pseudocode: at
`0x0047B249`, Ghidra reports a 5-byte body named `_free`, while the fixed CRT
member proves scalar `operator delete` jumping to the separate `_free` entry at
`0x0047B163`. Check actual body bytes, entry and provider identity before using
such output as a call contract. A successful build or visually plausible
runtime also cannot waive missing bytes, relocations, data owners, or scenario
observables.

If a required input, extent, tool profile, or observable cannot be established,
record `unknown` or an open candidate. Accuracy takes precedence over apparent
completion.

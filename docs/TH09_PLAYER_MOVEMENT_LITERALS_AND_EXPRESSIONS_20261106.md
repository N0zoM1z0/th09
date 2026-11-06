# Player movement literal definitions and source models, 2026-11-06

The complete Player movement diagnostic now checks the actual defined read-only
zero scalar used by its floating-point comparisons. Previously a private OBJ
fixture with0.5 under the unchanged `__real@00000000` name produced exactly the
same1900-byte/66-field/122-difference report, and the complete frontier guard
accepted it. The new check rejects that same fixture. No game source or target
bytes are changed, and no new exact function is claimed.

The valid object's public result format and every value remain unchanged.
The existing literal-definition utility reads actual section/value coordinates
from the independently parsed COFF symbol definitions. It verifies initialized
read-only source storage, payload width, reference addend and the independently
reviewed original zero at48E314. Undefined, writable, truncated or counterfeit
definitions fail. Omitting the required movement zero reference also fails.
Every code byte,66 actual fields, both tables and all calls remain checked.

The malformed fixture changes one data byte in a copied OBJ, turning the four
zero bytes into the IEEE single-precision0.5 representation. All1900 owned code/
table bytes and all actual COFF fields remain identical. It is an Oracle
regression fixture, not game-input fuzzing, source candidate or target patch.
The old checker is retained as its immutable Git blob for independent replay.

## Fresh target review and seven natural source models

Direct Ghidra rechecks movement41C170 with original disk/Program SHA-256,
x86 little-endian32, base400000, entry47D45F and all685056 mapped PE bytes
attested. The required owner remains1835 code bytes, one alignment byte and
two eight-entry tables:1900 physical bytes. Ghidra's disjoint1832-byte body is
separate evidence. The target owner SHA-256 remains
`64ea64884e978540c718d1691a4ae1b9866aaa8ec2296bafb158c4d8b1c628ca`.

Original and maintained scalar homes already agree: horizontal speed at EBP-8
and vertical at EBP-4. Many one-byte residuals are general-register choices,
not swapped scalar homes. The X-minimum clamp uses EAX in the original and EDX
in current source, changing the absolute-load encoding length; the opposite
Y-minimum choice later compensates. The first bounds-result copy also has a
different temporary-register/store schedule. These observations do not recover
unique original C++ locals, types or translation-unit boundaries.

Seven coherent models use the existing fixed movement profile, serial bounded
low-priority compilation and15 frozen quoted inputs:

| Natural source model | Physical bytes/fields | Unlike overlaps | Extra bytes |
| --- | --- | --- | --- |
| POD two-component velocity |1900/66|156|0|
| Actual SDK D3DXVECTOR2 velocity |1900/66|156|0|
| Native two-bit focus field |1900/66|122|0|
| One shared SHT snapshot variable |1900/66|131|0|
| Direct member receiver |1900/66|133|0|
| Borrowed SHT object references |1900/66|122|0|
| Ordinary explicit Float3 copy assignment |1916/66|520|16|

All retain23 ordered direct calls and both complete tables. The vector models
move the local homes and regress. The native focus and borrowed-SHT models do
not improve code generation. The assignment model retains extra storage/code;
its extra16 bytes remain counted. No model, header, type, ABI or profile change
is adopted. No assembly, barriers, forcing attributes, register/variable-order
hints, padding or compiler-option search is introduced. New literal validation
independently replays all seven reports without changing their measurements.

The two changed-header contexts also build genuine consumers: native-focus
Player OnUpdate522/40 and assignment-visible Float3 constructor26/0,
addition54/1 and subtraction54/1. All four complete comparisons656/42 pass.
These are repeated existing producers, not new exact functions or proof that
the complete corresponding headers are suitable for global replacement.

## Current complete-owner and dependency scope

The maintained movement object reuses its unchanged15-input/profile/object
proof from the native-side source epoch. It remains1900/66/122 with all1778
equal physical positions protected; the actual zero definition is now checked
too. There is no fresh game-source compile claim for that unchanged baseline.

Its23 calls resolve nine distinct exact direct callees. Their full bodies,
nested Float3 constructor and522-byte Player caller pass11 strict existing
unit comparisons10605 physical bytes/414 fields. Eight unit proofs reuse
unchanged transitive source/profile/object receipts; three missing current
carriers rebuild serially. No canonical recipe, private label, destination,
extent, profile or matching ledger changes.

Five new target-independent tests cover actual/original zero identity, corrupt
payload, missing references and invalid source provenance. The focused suite
passes42 tests/111 subtests. Full public CI passes1386 tests/29 skips; final
target-required tracking/progress, documentation and whitespace checks accompany
the checkpoint. The full CI is required here because a shared validator changes.

The ignored packet is `.analysis/player-movement-expression-flow-20261106/`.
It retains all seven complete models, the genuine producer builds, actual zero
definitions, both malformed-fixture reports, restored legacy checker, current
dependency proofs and mapped-byte Ghidra evidence. Counts remain1034 source-
present/1021 exact/13 nonexact; the goal and later product/runtime/semantic/port
gates stay open. Local Oracle checks do not establish Factory or remote acceptance.

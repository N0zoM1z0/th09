# TH09 ECL native wire byte indexing and complete RunEcl proof — 2026-11-07

The attested retail target is the original Japanese TH09 v1.50a PE with SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint continues the still-open complete 15,564-byte `EclManager::RunEcl`
boss rather than treating an inline accessor as its own independent exact function.

## One real native operand representation

`Th09EclRawInstructionHeaderView` owns the genuine 0x0C-byte prefix, followed
by the zero-length `arguments[0]` array of four-byte `Th09EclWireArgument`
union words. Integer operand readers use `.integer`, floating operand readers
use `.real`, and the real opcode byte reader now accesses
`arguments[byteOffset / sizeof(Th09EclWireArgument)].bytes[
byteOffset % sizeof(Th09EclWireArgument)]` instead of a reinterpret-cast
through raw bytes at the payload base. This retains byte addressing within
and across the packed four-byte wire words. Original observed consumers
currently request byte offset zero; other nonnegative offsets remain
expressible by the same indexed wire layout under the pinned VC7.1 extension.
This is not a claim of standard-C++ flexible-array portability or of
runtime behavior beyond the checked opcode consumers. The wire type and header sizes
are compiler-asserted in `EclOpcodes.hpp`.

The original timer-assignment and timeout-flag handlers are untouched. The
strict canonical comparison still audits the entire ECL interpreter, including
all 598 relocation fields, six interpolation table entries, all 187 main
opcode destinations, 146 of 148 matching handler shapes and both remaining
nonexact handler blocks. No original PE byte, compiler profile, foreign source
or manually generated machine code was altered.

The historical raw-context probe had two alternate source models: the raw
instruction prefix and typed base-pointer access. Its local macro-expansion
branch now also recognizes the reviewed native union-word byte form; it may
no longer silently replace the new accessor with an old raw-base expression.
Focused `test_runecl_raw_context.py` and `test_ecl_native_wire_byte_indexing.py`
cover this extension and the native wire layouts. This remains source-facing
model evidence, not a runtime test or a claim about unique original spelling.

## Current source-cold proof and still-open residual

Pinned VC7.1 builds of **five affected C++ compiler carriers** pass **87
complete exact existing functions**, totaling **30,583 complete physical
bytes** and **1,409 actual original-bound relocation fields**. The
15,564-byte RunEcl boss remains exactly **30 ordinary differing bytes**:
seven in opcode 4 and 23 in opcode 155, with **zero field or jump-table
differences**. The companion **3,900-byte EnemyManager::OnUpdate** still has
97 fields / 69 calls / 13 ordinary differences; its exact 82-byte collision
append leaf remains unchanged. Neither open boss receives exactness credit.

The source-cold ECL carrier refreshed **20 local `$L` compiler symbol names**
in `ecl-post-update-movement` and `ecl-compare-operands`. Before any manifest
edit, an independent audit of **77 complete main-profile ECL units / 24,368
physical bytes** checked every candidate field/section and compared each
changed label's actual same-section symbol offset to its existing fixed
original PE VA, including field type/addend. All 20 were name-only, with no
unreviewed destination or target changes; the strict canonical validator was
not weakened. The source-scoped replay succeeds with the corrected names.
Private receipt: `.analysis/gpt-web-ecl-native-wire-20261107/ecl-label-proof.json`.

No whole-game cold replay, full Windows product link, runtime Oracle,
semantic/port phase, Factory acceptance or remote publication follows from
these complete source comparisons. Counts remain **1,034 source-present /
1,021 exact / 13 nonexact**.

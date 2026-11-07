# RunEcl native four-byte wire operand and byte access — 2026-11-07

Against the pinned SHA-256 Japanese 1.50a TH09 original
(`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`).
This is a C++ source-identity checkpoint, not original-exact RunEcl or a
whole-product/Factory acceptance claim.

## Reconstruct the real operand storage, not a second raw pointer

`Th09EclRawInstructionHeaderView` has an independently checked 0x0C-byte
prefix and an inline variable-length `Th09EclWireArgument arguments[0]`
with genuine four-byte entries. That one native union carries ordinary
integer, floating-point and four byte fields; its members share the same
physical source locations. The existing `RawInt`/`RawFloat` accessors use
its first two C++ views directly.

`Th09EclRunState::RawByte` now also expresses the actual physical relation
as ordinary C++ indexing:

```cpp
return instruction->arguments[byteOffset / sizeof(Th09EclWireArgument)]
    .bytes[byteOffset % sizeof(Th09EclWireArgument)];
```

This replaces the separate `reinterpret_cast<unsigned char *>` expression.
The available current handlers, including death mode, sprite rotation,
callstack status, timeout spell and special interaction, request byte offset
zero. The byte offset is a *flattened payload offset* across consecutive
four-byte wire operands; it is never the instruction-prefix offset. The
separate signed 16-bit `RawShort` width has not been silently reclassified.

Opcode 4 continues assigning the genuine four-byte wire integer to the
real Timer through `Th09EclWireArgument` by-value snapshot. Opcode 155
continues its one-byte memcpy snapshot before the masked XOR of flag bit
24. Although a direct-looking `unsigned char` read is tempting, its VC7.1
output is a validated **counterexample** to source-code equivalence:
changing that snapshot alone produces 2,048 unlike ordinary positions,
242 differing relocation fields and 26 differing table positions. It is
not adopted merely for shorter C++ syntax.

## Complete owner and leaf validation

The new byte-array accessor compiles to **the exact same complete linked
RunEcl bytes as the baseline**: 15,564 physical bytes, 598 independently
bound relocations, identical complete residual positions, exactly **30
ordinary differences / zero field differences / zero table differences**.
Of the 148 ECL handlers, opcode 4 still accounts for seven differences
and opcode 155 for 23. Enemy's distinct **3,900-byte OnUpdate** remains
13 unlike bytes with all 97 fields and its full graph unchanged.
Neither boss becomes exact.

Fresh scoped fixed-profile VC7.1 builds of all **five affected ECL/helper
carriers** pass **87 previously exact whole functions / 30,583 physical
bytes / 1,409 relocations**. The complete owner comparison includes both
physical switch tables; no field masking, auto label redirection or
relaxed verifier was used. Natural-source experiments tested 13 independent
opcode4/opcode155/value/flag/typed-byte shapes under the same compiler:
three integer-timer snapshots increased the residual from 30 to 42;
most typed expression rewrites preserved the 30, and direct byte reads
invalidated far more than the 30 baseline. The full stored diagnostics
are under `.analysis/gpt-web-runecl-natural-20261107/` and are diagnostic
only, not new exactness credit.

The typed byte accessor changed VC7.1 **20 local `$L` symbol names** in the
complete ECL exact carrier, even though the linked bytes and original
addresses did not move. Each of the 20 changes was independently checked
against the corresponding same-owner COFF source definition, matching
original PE operand, original VA, relocation type/addend/site, and fixed
canonical owner. Only compiler-private names in `match-units.toml` were
updated; destinations and strict comparison logic were not. See
`.analysis/gpt-web-runecl-natural-20261107/native-byte-label-proof.json`.
The earlier distinct two-name ECL proof is preserved in
`.analysis/gpt-web-enemy-natural-20261107/ecl-label-proof.json`.

The historical `probe-runecl-raw-context.py` remains usable: its strict
review now recognizes the newer native wire-array form in addition to both
old synthetic and typed-flat forms, and generates a matching type-safe macro
only for a reviewed accessor. Altering the native word stride is rejected.
**37 focused ECL/RunEcl tests** pass, including strict negative probes.

Ledger remains **1,034 authored source functions, 1,021 exact, 13 nonexact**.
No full-all cold replay, runtime portability or complete Windows link is
claimed.

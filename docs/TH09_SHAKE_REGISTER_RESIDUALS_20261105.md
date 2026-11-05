# Shake envelope: complete register-operand residual review

Fresh original-bound analysis and a fresh pinned VC7.1 main-carrier build
retain the complete `ScreenEffect::CalcShakeEnvelope` frontier at
`0x004227A0`: 336 bytes, 20 relocation fields, 132 instructions, eight ordered
calls, 26 agreeing physical blocks and 26 unlike bytes. No source change or
new exact-function credit results. The inspected source is the unchanged
`1dd4a12758f6a213eacb2ab85a13f3e2d5579d1d` source epoch.

## What the fresh comparison establishes

The original owner SHA-256 remains
`1d33957d51f8cc170471b9673a938899281cfa07fbed746eb0843e62e2597472`.
The direct Ghidra run attests the original disk and Program hashes, i386
language, image base, entry point and all 685,056 file-backed bytes. Ghidra
decompilation supplies provisional data flow; the full PE/COFF comparison
supplies the independent byte result.

After the maintained strict binding of all 20 actual fields, all 132
instruction positions, lengths, mnemonics, operand widths, immediate values
and memory displacements agree. Every unlike byte selects a general-purpose
register. The diagnostic preserves EBP/ESP, x87 stack operands and all other
operands; it does not hide stack-layout, floating-point, branch-target or
relocation differences. There are 26 changed instructions, each differing in
one byte.

The original carries the effect owner in EDI and its timer receiver in ESI;
the candidate carries them in ESI and EDI. Hold/fade addition temporaries and
the unsigned fade-divisor test also use different registers. The x87 arithmetic
and conversion sequence agrees, including the signed initial-ramp divisor,
the final-endpoint float spill before timer conversion and the final unsigned
fade-divisor conversion. This classifies the existing residuals; it does not
prove semantic equivalence or satisfy the exact-byte Oracle.

The legacy source comment about volatile aliases and declaration-order
sensitivity is compiler-experiment history. Neither separate machine loads nor
the decompilation establishes original C++ volatility. Original field
qualifiers, local declarations and source spelling remain unknown. This review
introduces no volatile, declaration-order, register or compiler-profile tactic.

## Complete source and proof scope

The fresh main `ScreenEffect.obj` also passes all 16 canonical units belonging
to that actual object: 1,555 physical bytes and 77 relocation fields. The
separate RegisterChain and DrawSquare objects are outside this fresh-build
claim. The eight original call identities remain protected by the maintained
complete inspector; this review does not claim a new callee reconstruction or
product/runtime validation.

The ignored packet `.analysis/shake-complete-20261105/` contains the build log,
complete comparison, original/candidate disassemblies, operand classification,
16-unit comparison, Ghidra attestation and recipes. The sealed local
`source-proof.json` binds all 16 quoted source inputs, actual main object,
profile, tools and eight compiler/SDK files. Its SHA-256 is
`3072145fd451799cac027a6db3b96b66d0130b16d9b47d0136a8d64546747aa8`.

`python3 -B .analysis/shake-complete-20261105/replay.py` independently recompiles
the main carrier into a disposable directory and compares complete raw bodies
and actual COFF rows for all 16 exact siblings and the full shake envelope.
It preserves the sealed receipt and canonical object and writes
`source-replay.json`. No matching ledger or acceptance rule changes; totals
remain 1,035 source-present / 1,022 exact / 13 nonexact.

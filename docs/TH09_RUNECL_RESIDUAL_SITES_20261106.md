# RunEcl complete residual-site review, 2026-11-06

This review starts from clean source `5db810b8`. Fresh direct Ghidra exports
cover RunEcl and the real Timer assignment wrapper at `401500`. Disk and
Program SHA-256, i386 language, base `400000`, entry `47D45F` and all 685056
file-backed mapped bytes agree with the original Japanese 1.50a target.
Ghidra observations give no exactness credit.

The preserved `bfa170c9` ECL object and all 86 quoted source inputs still match
their sealed source/profile proof. Its complete physical RunEcl remains 15564
bytes, 598 relocations and 30 ordinary differences, with no relocation-field
or table differences. The 83 exact sibling proofs are reused; rejected private
controls do not replace their source or objects.

## Physical tables and actual residual blocks

There are two consecutive tables: six interpolation entries at `40C088`, then
187 main-dispatch entries at `40C0A0`. The main dispatch indexes `opcode - 1`.
Selecting a handler from all 193 trailing words as one table gives the wrong
block. Every control below locates its handler from its actual COFF table field
and same-owner symbol definition, independently of the original destination.

Opcode 4 begins at `408937`. The original loads the integer into ECX and pushes
it before loading the active context into ECX. The maintained object loads EAX,
loads the context, then pushes EAX. Both call the real Timer assignment at
`401500`. Exactly seven differing physical positions lie in this block.

Opcode 155 begins at `40B770`. The original zero-extends the wire byte into ECX,
loads the old primary flags into ESI, computes the bit-24 masked XOR in ECX,
then copies ESI to EAX and stores the XOR result through EAX. The maintained
word helper uses a byte load followed by zero-extension and another register
schedule. Exactly 23 differing physical positions lie in this block.

These are target instruction observations. They do not identify the unique
original C++ declarations, register variables, nominal types or TU boundaries.

## Fixed-profile complete controls

Nine fresh controls freeze all 86 current quoted inputs and use the existing
`/O2 /Ob1 /Oy- /EHsc` ECL profile, serially at lowered priority with a timeout.
They contain no register hints, assembly, barriers, volatile forcing or profile
search. Each is compared over both complete owners, RunEcl and Enemy OnUpdate.

| Source hypothesis | RunEcl ordinary / field / table differences |
| --- | --- |
| Explicit Timer operator call, retaining the copied integer | 30 / 0 / 0 |
| Direct typed wire lvalue inside the retained integer copy | 30 / 0 / 0 |
| Named native timeout field, retaining the copied byte | 1938 / 202 / 26 |
| Flat anonymous native bitfield, direct or copied byte | 1938 / 202 / 26 in both |
| Native field from wire byte, byte pointer, or narrowed wire integer | 1938 / 202 / 26 in all three |
| Native field directly from the wire integer | 2065 / 242 / 26 |

The six byte-valued native-bitfield controls produce the same local instruction
shape as the earlier native-field control: EAX carries the shifted operand and
ECX the result. Its compact EAX AND encoding is one byte shorter than the
original ECX encoding. Following instruction addresses and later table
**destinations** change, although the complete physical extent remains 15564.
All nine Enemy owners remain 3900 bytes / 97 fields / 13 differences.
No source model is adopted and no exact count increases.

An older timer control which removes the integer copy has 42 differences over
the complete function. Reinspection confirms that opcode 4 itself still has
seven; the extra twelve are in the neighboring opcode-5 block. The complete
comparison prevents treating local similarity as a whole-owner improvement.

## Reproduction and retained evidence

The ignored packet `.analysis/runecl-remaining-20261106/` retains the two probe
recipes, frozen inputs and Git-blob hashes, all changed control sources, actual
objects/logs, every COFF field, original-bound complete reports and the fresh
Ghidra exports. `verify-controls.py` independently rechecks all nine RunEcl and
nine Enemy comparisons and decodes the physical table-to-block mapping.

763 SHA-identical per-case source copies are pruned after inspection; frozen
inputs and changed sources remain. `replay-one.py timeout-flat-copied` restores
84 unchanged inputs, recompiles the same complete carrier, reproduces its full
raw bytes and all actual COFF rows, and prunes those 84 duplicates again.
The control seal is SHA-256
`5935e5541e9c8a9af07fdefec3b79b3e8451cde06566ad2854d761d3642219ae`.
These bounded negative results leave other natural source hypotheses open.

The separate Effect investigation also found that the cached Ghidra Program
had no callback function at `40D1C0`. After a complete mapped-byte precheck,
a private script creates only reviewed function metadata for `40D1C0..40D1F2`,
reads back its 51-byte range and physical fastcall convention, and exports it
with GetAnm20 and PreloadAnm90. The post-export mapped-byte check still covers
all 685056 bytes. No target bytes change. The packet
`.analysis/effect-added-current-20261106/` retains the initial failed export,
metadata recipe/readback and successful exports. This repairs analysis working
state and does not close the existing eight-byte callback frontier.

Tracking and reconstruction totals remain 1035 source-present / 1022 exact /
13 nonexact. Whole-product, runtime, semantic and port gates remain open.

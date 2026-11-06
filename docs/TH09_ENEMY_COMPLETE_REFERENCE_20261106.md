# Complete Enemy reference replay, 2026-11-06

The complete pinned Cirno-Labs Enemy translation unit does not resolve the
maintained OnUpdate frontier. Both its unmodified source and a target-correct
speed-constant control produce 3,872 physical bytes and 96 fields, compared
with the original owner's 3,900 bytes and 97 fields. Each complete comparison
has 3,297 unlike overlapping positions and 28 missing original bytes. Neither
source is adopted and neither receives exact credit.

The reference is the clean, immutable d7653152 checkout, with tools edf50486.
All 79 CP932 source inputs are checked against the previously pinned mirror.
Two serial builds use the pinned local VC7.1 compiler and the reference's
recorded `/O2 /Ob0 /Oy- /Gr /GX` profile. This does not identify the original
compiler profile or validate the reference's claimed matching status.

The reference writes `rank * 0.1f + 2.0f`; the original operand and maintained
source require `+ 1.0f`. The second control changes only that constant. All
96 actual fields are bound without fitting candidate operands to target
operands. Owner-local destinations retain their actual COFF definitions;
external named bindings are cross-checked against the current canonical
Enemy identities. The extra `2.0f` scalar uses an existing canonical literal
identity. Each object's five actual read-only literal definitions pass.
The four-entry physical table is retained at candidate offsets 3856..3871.

Both reference contexts emit 68 direct calls. They merge the movement branches
into one shared vector-add call, while the original and maintained source
retain 69 calls and three vector additions. This independently reproduces the
whole-owner consequence observed in the earlier local count experiments.
Register transport and descriptor initialization also differ. Correcting the
speed constant cannot repair these other differences. Instruction alignment
is explanatory only; full comparisons retain actual positions, displacements
and every table word.

Fresh direct Ghidra attests disk/Program SHA-256, i386, base `0x400000`, entry
`0x47D45F` and all 685,056 mapped file bytes. Its disjoint 3,881-byte flow body
is distinct from the 3,883-byte code contribution and complete 3,900-byte
physical comparison. The unchanged 86-source-input/native-object proof and
SDK/provider hashes are checked again. The strict maintained Enemy inspector
replays 3,900 bytes / 97 agreeing fields / 13 differences; the full RunEcl
companion remains 15,564 / 598 / 30, with no field or table differences.

The ignored packet `.analysis/enemy-reference-context-20261106/` retains both
objects under `build/analysis/enemy-reference-context-20261106/`, recipes,
complete COFF rows/comparisons, the changed control and fresh Ghidra evidence.
157 per-file SHA-identical staged copies are pruned against the retained
79-file reference mirror. Independent replay of the seal passes:
`b9f2730ab6f6f93bdbd6562c47451991d286da4fa5057e259bbf7ff220cb23cd`.
Code epoch is `7008a947`; authored totals remain 1,034 present / 1,021 exact /
13 nonexact. Product/runtime closure and the reconstruction goal remain open.

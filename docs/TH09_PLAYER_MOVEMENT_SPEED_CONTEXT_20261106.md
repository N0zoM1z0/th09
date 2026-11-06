# Complete Player movement speed contexts, 2026-11-06

The source baseline is `b9e1a95a`, after the six native math-provider repairs.
Three fixed-profile source contexts recheck TH09's four adjacent SHT floats
at +14/+18/+1C/+20. A storage union exposes both a four-float array and two
Float2 pairs. Existing scalar fields remain available, letting the first
control distinguish changed storage representation from changed reads.
Compile-time guards retain the16-byte span and original member offsets.

| Source context | Complete movement observation | Direction-offset leaf |
| --- | --- | --- |
| Union storage; existing scalar reads | 1900 bytes /66 fields /122 differences | 412 bytes /18 fields, exact |
| Four constant-index float array reads | 1900/66/122 | 412/18, exact |
| Axis/diagonal Float2 pair reads | 1900/66/122 | 412/18, exact |

Every movement raw byte, effective field and residual position remains equal
to the maintained baseline; all23 original calls remain in order. These
representations do not explain its unresolved scheduling/register choices.
No storage or caller model is adopted, and no original array/class identity
is inferred.

Each actual PlayerGameplayState source is also built under its existing
profile. ResolvePatternOffset reproduces the complete346-byte function,
two alignment bytes and64 switch-table bytes, **412 physical bytes /18 fields**.
The three comparisons total1236 bytes/54 fields. Eighteen private labels per
object retain their actual same-owner definitions and original destinations;
the anonymous owner spelling is resolved from the actual COFF definition.
No private mapping is written to production metadata.

Committed-only TH08 `adfdc969` and TH095 `72409b7` Player headers corroborate
the broad four-scalar-speed source family. Their offsets/layouts and TH095's
provisional semantic names are not TH09 evidence. TH09-local target operands
and canonical speed consumers establish this experiment's offsets and roles.
No adjacent variable-order pragma or register hint is used.

The earlier seven controls in the parent packet bind the pre-repair `b60b5f65`
source; these three bind `b9e1a95a`. Their epochs are kept distinct. The
independent `verify-experiments.py` checks **ten full1900-byte owners** and
**twelve complete callee comparisons /1638 bytes /60 fields**. The latter
comprise three real Float3-core contexts and the three direction-offset
comparisons. These are diagnostic experiments, not new canonical credit.

Ignored `.analysis/player-movement-current-20261106/speed-context/` retains
frozen15-file inputs, three changed header/read contexts, full comparisons,
private definition reviews, commands/logs and source hashes. The parent retains
the earlier frozen17-file context and unique controls. **149** SHA-identical
staged copies are pruned; no objects or unique sources are removed. Both
recipes restore missing unchanged inputs from their frozen copies and write
separate current reports. A restored native-math/core context reproduces its
entire caller, actual fields and three exact providers; the16 restored copies
are pruned again after their hashes agree.

The separate immutable source receipt binds
`b9e1a95a72ad033ba722aa8734fedb1ce1dbd541`, SHA-256
`bc4d06b3b4f4940fb40095dc71378349ae7ed8ef9060741d41917f0fed4f9112`.
It checks104 tracked inputs/Git blobs, four actual objects/profiles, eight
compiler/SDK files and five canonical comparisons674 bytes/42 fields.
`source-receipt.py` replays it into a separate current report. See
[the adopted source review](TH09_PLAYER_MOVEMENT_NATIVE_MATH_20261106.md).
Tracking, progress, documentation and whitespace checks pass. Totals remain
1022 exact/13 nonexact; no product/runtime or Factory verdict follows.

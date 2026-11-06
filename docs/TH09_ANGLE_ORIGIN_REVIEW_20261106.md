# Angle helper origin review, 2026-11-06

The original15-byte function at405700 remains completely reproduced by
`Th09EclRunControl::VectorAngle`. Its original SHA-256 is
`727ef3908f51e11e70cbf309e0f044724453ac04c491ad87a827b5d21a5dfc28`.
The existing native source and runtime-vector-angle recipe are preserved.

A fresh analysis-only translation unit takes the address of the real SDK
`atan2f`, exposing its inline implementation under the existing pinned
RuntimeAngleMath profile. The resulting `@atan2f@8` has exactly the same15
original bytes and no relocations. The preserved complete reference ECL
object independently contains that same15-byte implementation. Actual SDK
math.h defines the float wrapper through double-precision atan2 and a float
conversion; its SHA-256 is
`776b7b9c45c5aed0e355109ab7f3435971d833d2228dd4f2b16aad4fe6bf96c8`.
Fresh target-attested Ghidra covers the15-byte function and all685056 mapped
file bytes. The following single CC alignment byte remains unowned.

This is evidence of two byte-identical source producers. It establishes
neither the unique original namespace/calling-convention spelling nor
SDK-versus-game authorship. Folding of equivalent implementations also stays
unknown. The earlier high-confidence authored classification cited a compiler
byte comparison, which cannot distinguish these origins. Adjacent-game source
similarity does not resolve TH09 authorship.

The origin is therefore returned to unknown/review, with attribution confidence
low. The function keeps its matching100 codegen status and maintained source;
its canonical recipe still replays. Only authored mappings, implemented-name
credit and the authored match-ledger row are withdrawn. No library exclusion
is asserted. A fresh native source object protects both original-equal units:
VectorAngle15/0 and AddNormalizeAngle111/6,126 complete bytes in total.

Current totals become1034 source-present authored mappings /1021 authored
exact functions /13 nonexact functions, with38 origin-unresolved candidates.
Exact authored bytes are262034 of291868,89.7783%. The98% gate still requires
23997 additional exact bytes. This attribution correction changes no game
source, target bytes, function extent, compiler profile or exact-codegen proof.

The ignored packet `.analysis/math-angle-origin-20261106/` retains the two
isolated build recipes/logs/objects, actual SDK header hash, complete native
comparisons and the Ghidra report. Objects remain below
`build/analysis/math-angle-origin-20261106/`. The source-code epoch is
`17bb8e86e72fbf5c16f872467a13e97eefe01418`; the later ledger review is
independent of that unchanged implementation. Factory acceptance and remote
publication are not claimed.

Independent `seal.py` replay passes. The ignored source-proof.json SHA-256 is
`9bc0390a0c531971bf7b78cdb4d7daf2f68bd233dbc3452e564c9802a616ddb5`.
It separates the immutable code epoch and two objects from the current five
classification-ledger hashes, and pins the actual SDK header, eight compiler/
SDK files and Ghidra report. Thirty-five focused tracking/progress/origin/docs
tests pass. The full unchanged RunEcl guard still passes15564/598/30 after the
origin review; authored attribution is not a prerequisite for its real callee
byte proof.

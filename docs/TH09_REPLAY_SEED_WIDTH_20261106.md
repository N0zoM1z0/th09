# Replay seed parameter and frame-header controls, 2026-11-06

SetSeed's original 14 bytes do not uniquely recover its original C++ parameter
width. A coherent unsigned-int declaration/definition, assigning to the same
16-bit seed member, independently reproduces every original byte under both
existing Replay profiles. The maintained unsigned-short interface also matches
and remains unchanged. Ghidra's two-byte parameter projection reflects the
machine access; it does not identify a unique formal source declaration.

That ambiguity does not solve BeginPlaybackStage. Two complete source models
are compiled under each of the existing EH-enabled and EH-disabled profiles:

| Source model | Complete caller | Full original comparison |
| --- | --- | --- |
| Wider setter declaration plus real wider setter definition | 538 bytes / 32 fields in both profiles | 39 unlike overlaps; two missing bytes |
| Read-only frame-0 header view, preserving the mutable stream cursor | 538 bytes / 32 fields in both profiles | 39 unlike overlaps; two missing bytes |

All four callers retain every baseline raw byte and effective field identity.
Only the wider setter's C++ symbol spelling changes, at the same independently
known original address. Both actual wider setter bodies match 14/0. Six serial,
lowered-priority bounded compiler jobs use the two existing profiles; no flag
combination is searched and no source model is adopted.

Both wider caller profiles and the actual runtime leaves pass 16 existing
complete comparisons, 4132 bytes / 232 fields. The read-only-header profiles
pass 12, 4093 / 232. That is 28 comparisons / 8225 bytes / 464 fields; no
compiler-private label refresh is needed. These repeat existing exact credit.

Fresh direct Ghidra covers playback420840, setter420160 and creator421670,
attesting disk/Program identity and all 685056 mapped bytes. Original creator
operand4216F5 contains420840, independently matching the configured callback
relocation at creator+85. This is an actual installed callback as well as the
stage dispatcher's tail-jump target; that registration contract is preserved.
Original nominal C++ classes, formal widths and TU identity remain separate
unknowns.

The ignored `.analysis/replay-seed-width-20261106/` retains thirteen frozen
inputs, changed sources, six objects, whole caller/callee reports and complete
unit recipes. The baseline code/object proofs are scoped to source398b69e4.
23 per-file SHA-identical copies are pruned against frozen inputs. Independent
read-only replay passes; immutable control seal SHA-256:
`04cd6f55ae6aafe0f9971de6f2e088d45774a79932791ae179ae829b015f19f2`.

An initial observer reused the previous case's alternate setter-symbol map and
rejected the already-compiled read-only-header case. A fresh normal observer
recovered that same retained object, and each later case resets the map. The
compiler job was not restarted and no public guard was weakened.

No production source, canonical interface, target/profile, exact counter,
Factory result or product/runtime status changes. Current authored totals
remain 1034 present / 1021 exact / 13 nonexact.

Tracking, generated progress, documentation and whitespace validation pass,
as do 35 focused tracking/progress/origin/documentation tests. Nine disposable
probe PDBs across this and the Title caller/callee packet are reviewed by
path/SHA and removed; all nine new COFF objects remain. The separate cleanup
manifest is `debug-cleanup.json` in this packet.

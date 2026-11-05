# SaveReplay restores the checksum address byte

SaveReplay now reuses one loop index across its seven stage/byte loops,
following the pinned reference's source-lifetime hypothesis. Both actual
ReplayInput profiles retain the complete 1,238-byte owner and restore byte
`+0x3F6`, reducing three differences to two. The remaining offsets are
`+0xB7/+0xBB`, selecting the frame-header copy destination register. This is
still nonexact; it gains no partial or exact-function credit.

## Complete original and compiler result

The target and fresh mapped-byte Ghidra evidence from the immediately preceding
[native counter review](TH09_REPLAY_NATIVE_COUNTERS_20261105.md) are unchanged.
The original owner SHA-256 remains
`a225c9a1aae3db79d6e31b22e77ccba892b38c44fe9572233cd6df24edb1cef3`.
The complete original-bound PE/COFF inspector, rather than Ghidra's body set,
covers every one of the 1,238 bytes through the terminal `RET 4`.

Against source checkpoint `92ddf9b3afdf0ad1219ea01b6ad3aba118196b44`, both
new actual objects change exactly one raw byte, `+0x3F6: 10 -> 02`. All 53
actual COFF rows remain identical. The original load uses `[edx+eax]`; the old
candidate used the equal-address scale-one encoding `[eax+edx]`. The new byte
reproduces the original SIB encoding. All 1,235 previously equal resolved
positions survive. The complete function still has 380 instructions, 23
original-position ordered calls and 49 agreeing control-flow blocks.

The shared variable is declared at its first use and reset by each original
loop initializer. Source expressions, input-chain acquisition, copy order,
file/error branches, the original pre-copy uninitialized header scan and the
`0x618` frame are preserved. There are no declaration-order permutations,
register hints, volatile additions, barriers, assembly or profile changes.
The pinned Cirno-Labs reference at
`d7653152becabb8c86f2f43d49015ecc11bd0652` uses one index across these phases;
it is hypothesis material, not proof of unique original declarations.

## Causal controls

Six complete current-interface controls use frozen source from `92ddf9b3`
and the existing `/O2 /Ob1 /EHsc /Oy-` profile:

| Index/cursor model | Complete result |
| --- | --- |
| Share only the three stage-loop indices | Identical full raw body/COFF; three differences |
| Share only the four byte-loop indices | Identical full raw body/COFF; three differences |
| Share only the two checksum-loop indices | Identical full raw body/COFF; three differences |
| Share one index across all seven loops | One changed raw byte; two differences; adopted |
| Also share the chain cursor | Same one-byte improvement; no additional benefit; not adopted |
| Also share the input/FPS copy length | Same one-byte improvement; no additional benefit; not adopted |

The older parser/replay archive contains both an interrupted shared-stage
experiment and a corrected neighboring carrier using superseded interfaces.
Those historical outputs do not replace these current-source comparisons.
Sharing just the smaller loop groups fails to recover the byte; this is a
whole-function lifetime observation, not a local indexing-spelling result.

## Protection and validation scope

The maintained guard now protects all 1,236 equal bytes and permits differences
only at `+0xB7/+0xBB`. A new test explicitly rejects reintroducing `+0x3F6`,
with a self-consistent three-entry diagnostic, so a return to the previous
frontier cannot silently pass. Existing equal-count mismatch tests now use the
two-byte frontier and continue to test real unexpected offsets.

Both affected actual profiles are rebuilt serially at lowered priority. All
twelve existing canonical units retain 4,093 physical bytes and 232 fields.
No canonical label or relocation metadata changes are needed. Seven callee
proofs from the preceding native-counter checkpoint are reused after checking
their actual object hashes, profiles, unit records and complete quoted source
closures; no unchanged callee is rebuilt again. BeginPlaybackStage retains
538/540 bytes with 39 unlike overlaps and two missing bytes.

The ignored packet `.analysis/replay-shared-index-20261105/` retains frozen
inputs, six changed source controls, actual objects/logs, full comparisons and
the parent guard. `verify-current.py` checks the one-byte transition, both
complete owner contexts, all twelve canonical units and the reused callee
bindings. `replay.py` independently recompiles all six frozen controls in
disposable directories: the three old-byte controls are rejected by the new
guard and the three two-byte controls pass. This cold control replay passed.
Only 66 SHA-identical duplicate source copies were pruned; the frozen closure,
changed bodies and object/report evidence remain.

The sealed `canonical-source-proof.json` is SHA-256
`18ec28e7115b5efe18563559c2816f246c4dd5e537192b2778d49c64fe10b38b`,
bound to source `7d51024348ded9bce92444288c06fb97db5ed6f9`. It records all 30
tracked source/tool inputs and normalized Git blobs, eight compiler/SDK files,
both actual objects/profiles, complete owner diagnostics, six controls, the
independently reproduced parent no-EH baseline and the unchanged callee proof
bindings. `python3 -B .analysis/replay-shared-index-20261105/proof.py` verifies
that committed source and retained evidence and writes a separate
`canonical-source-replay.json`, leaving the sealed receipt intact.

This is local exact-phase evidence; source/TU uniqueness, product/runtime
closure and Factory acceptance remain open. All 48 Replay tests pass; full public CI passes
1,354 tests with 29 skips. Tracking, progress, documentation and whitespace
checks pass. Totals stay 1,035 source-present / 1,022 exact / 13 nonexact.

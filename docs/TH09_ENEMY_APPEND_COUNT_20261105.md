# Enemy update: independent append and count chronology review

This is a negative exact-phase investigation, not a new match. The maintained
source at `1dd4a12758f6a213eacb2ab85a13f3e2d5579d1d` remains unchanged.
`EnemyManagerView::OnUpdate` retains 13 differences across its complete
3,900-byte physical owner at `0x00410730`.

## Current original and compiler evidence

The original Japanese executable was verified again. Fresh direct Ghidra
analysis attests the disk and Program SHA-256, x86 LE32 language, `0x400000`
image base, `0x47D45F` entry point and all 685,056 file-backed bytes. Its
decompilation is analysis evidence only. The complete PE/COFF comparison
separately covers 3,883 code bytes, one alignment byte and the four-entry table.

The maintained object has 97 agreeing relocation fields, 69 ordered calls and
170 agreeing physical control-flow blocks. The 13 unlike bytes remain:

`19C 1A3 252 263 26D 272 294 29A 29C 2A5 2A9 2B7 2BD`.

Two select the first draw-list index register; four select the effect-manager
receiver and threshold registers; seven encode the descriptor position-copy
registers and component-store scheduling. The original copies x/y/z through
ECX/EDX/EAX and stores x/y/z, while the candidate uses EDX/EAX/ECX and stores
z/x/y. These observations do not establish original local declarations,
nominal vector types or a unique source spelling.

## Bounded natural-source hypotheses

The pinned Cirno-Labs `enemy.cpp` reference at
`d7653152becabb8c86f2f43d49015ecc11bd0652` supplies two hypotheses: independently
written list insertions and the arithmetic count expression
`(character != 13) * 2 + 1` assigned near the end of descriptor initialization.
Its different speed constant is not imported; TH09's original `1.0f` remains
in every control. The reference is hypothesis material, not TH09 acceptance
evidence.

All controls use the existing EclManager `/O2 /Ob1` profile and the full current
85-file quoted source closure. No declarations are reordered as register
hints, and no assembly, barriers, volatile additions or profile changes occur.

| Control | Physical bytes | Fields / calls | Complete outcome |
| --- | ---: | ---: | --- |
| Independent early list append, replacing the shared goto | 3,912 | 97 / 69 | 1,992 unlike overlaps, 12 excess bytes; 169 blocks; rejected |
| Same append with the redundant native-enemy alias removed | 3,912 | 97 / 69 | Same complete raw body and resolved bytes; rejected |
| Reference count expression and its later assignment point | 3,852 | 96 / 68 | 2,954 unlike overlaps, 48 missing bytes; 169 blocks; rejected |
| Arithmetic count expression at the maintained assignment point | 3,900 | 97 / 69 | Identical raw body; same 13 unlike resolved bytes and 170-block graph |

The last control's five private COFF label spellings change, but their actual
same-section values, relocation offsets, types and addends remain identical;
every external symbol identity also remains identical. This is not a raw
COFF-row identity claim.

The later-count control also changes code far beyond descriptor setup:
the compiler merges the two movement-position addition paths, reducing the
complete owner's calls to `Float3::operator+` at `0x00401100` from three to two.
Both source branches remain present. Thus a local descriptor experiment must
still verify the complete owner, fields and call multiplicities; preserving
the local calculation alone is insufficient. The strict maintained inspector
rejects all three regressed controls without being weakened.

## Reproduction and retained evidence

The ignored packet is `.analysis/enemy-complete-20261105/`. It retains the
frozen 85-file input closure, four changed source bodies, actual objects under
`build/analysis/enemy-complete-20261105/`, compiler logs, full COFF rows,
resolved comparisons, disassemblies, Ghidra attestation and recipes. Only 336
SHA-identical duplicate source copies were pruned; the frozen input closure
and unresolved evidence remain. The sealed receipt is `source-proof.json`,
SHA-256 `6519ae29ebe266939c8fc0d28d710521f63aa1603e55914438143fc61cd304a5`.

`python3 -B .analysis/enemy-complete-20261105/replay.py` freshly recompiles all
four controls into disposable independent directories. It reproduces the
complete Enemy and RunEcl raw bodies and actual COFF rows, preserves the sealed
receipt and writes `source-replay.json`. This replay passed after duplicate
input pruning. RunEcl remains 15,564 physical bytes / 598 fields / 30 differences
with unchanged raw and linked hashes in every control.

The existing Type29 receipt
`9382a5f090a48c90fa74b43dc7dc72941bd0257debfad0ac0e189eb74e3275c6`
is reused after checking all 144 live inputs and both canonical ECL object
hashes. Its 83 exact-unit proofs are reused, not reported as fresh canonical
compiles. No game source, canonical ECL object, ledger, acceptance rule or
exact credit changes. Totals remain 1,035 source-present / 1,022 exact / 13
nonexact. Original source context, product closure and runtime remain open.

# Native Setup rank publication, 2026-11-06

The maintained GameplaySetupThread now uses native GameManager fields for its
runtime setup and groups the rank writes in their original order. Its complete
candidate is1682 bytes/172 fields, compared against all1689 original bytes:
694 unlike overlaps plus seven missing bytes. It remains nonexact. The earlier
1689/171/904 candidate is retained as source-scoped historical evidence.

The changes come from the current nonexact source and original TH09 accesses.
Global native side fields recover the original three root reloads and the
flag-word loop anchor. Moving the rank parameter write before the consecutive
limit/rate writes lets the compiler share the original publication tails.
The difficulty0 mode branch uses a normal if/else. The adopted source introduces
no helper, assembly, barrier, qualifier, padding, register hint or profile.
Experiments with explicit shared local values/labels were not adopted.

PlayerRegisterChain now takes int in its shared declaration and genuine
PlayerLifecycle.cpp definition. Both this and the preceding unsigned-char pair
reproduce the complete328-byte/29-field original producer. Both consume the
low byte in InitializeType. The int pair also reproduces original Setup's full
ECX zeroing. This is a supported coherent source model, not proof of the unique
original formal type. The actual private EH definition and recursive data/code
contribution remain equal; only its source-bound compiler name changes.

## Complete physical and structural comparison

All172 effective address operands now agree with the original referent
multiset, including every destination's multiplicity. All35 ordered calls
agree. Both original and candidate contain413 instructions and78 direct blocks;
412 instruction correspondences agree. The remaining instruction placement
moves the shared C8=22 write from original+431 to candidate+522. Its incident
jump encodings and later physical offsets still differ. Instruction alignment
is diagnostic, not a byte-exact verdict.

The original has785 equal physical positions in the preceding maintained
candidate; the new candidate has988. It retains536 of those earlier positions,
loses249 and gains452. The full original error count falls from904 to701,
including all seven missing bytes. This is a reviewed new nonexact source
baseline, not a claim that all old equal offsets survived. Its new strict
residual mask protects all988 equal positions. The old source/object proofs
retain their original epochs; they are not transferred to changed code.

The public diagnostic explicitly reports the1682-byte candidate,1689-byte
original,694 overlap differences,seven missing and zero extra bytes. The
noncrediting guard requires both extent counts,all172 fields and empty
unmatched-referent inventories. It rejects a fabricated complete match even
if overlap differences are zero while seven original bytes remain missing.
No matches.csv row or exact-function count changes.

## Validation scope

Three fresh serial pinned-VC7.1 builds cover both existing Setup profiles and
the actual Player lifecycle producer. All14 affected canonical units pass:
3480 complete physical bytes/286 fields. The ten /Os Setup siblings, separate
214-byte no-/Os loading sibling and three Player units all remain exact.
No compiler profile or target destination changes. Twenty-five unchanged
regular body/profile contributions retain6742 raw bytes/546 field identities;
the actual compiler EH graph is checked separately. The alternate Setup
worker is recorded as a complete nonexact carrier, not credited as exact.

Fresh session Ghidra evidence for41AF2D,41A7C3 and41F170 attests the original
disk/Program SHA, i386,base400000,entry47D45F and all685056 mapped bytes.
The target is the original Japanese1.50a SHA-256
10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822.

A specific remaining control-flow question is checked using ten x86-emulated
rank slices: all five supported difficulties, mode0/2,round count6. Original
and candidate preserve every ordered write to rank limit/parameters and both
players' counters. This confirms those bounded branch traces around the moved
shared write. It is not whole-worker, Windows product or native runtime
validation; no allocator or callee is stubbed into this slice.

Focused guards pass26 tests;35 tracking/progress/origin/documentation tests
also pass. Full public CI passes1376 tests with29 skips for the changed shared
diagnostics. Its initial three failures were obsolete caller-spelling assertions,
which are removed while preserving layout and actual COFF acceptance checks.
The immutable source/object binding follows in the proof checkpoint. Three obsolete tests stop requiring a local pointer
spelling for calls/field accesses. Their layout contracts remain, and actual
COFF identities/counts and full original comparisons enforce the call boundary.

The ignored packet is `.analysis/setup-rank-publication-20261106/`. It retains
five complete native-source experiments, three fresh current objects, full
comparisons and exact-unit recipes. `current-verify.py` independently replays
all14 units and the whole nonexact diagnostic; `preservation.py` checks the
25 unchanged contributions; `rank-traces.py` repeats the ten bounded traces.
Current authored totals remain1034 source-present/1021 exact/13 nonexact.
Native Windows i386 product/runtime and later semantic/port gates remain open.


## Immutable source binding

Source checkpoint `657e9699dabdf962d96f5b193ae5da4fa594dd0a`; private source-proof SHA-256:

`8023ad887bc478521658b77fa3e516a401214626e6d228e3796a1016c2940f93`

The proof binds60 tracked inputs/Git blobs,eight compiler/SDK files,three
current objects and five experimental objects. Independent read-only replay
passes14 current canonical comparisons3480/286 plus50 source-scoped experimental
sibling comparisons13885/1130. These64 comparisons17365/1416 do not represent64
distinct functions. The25 unchanged body/profile comparisons and ten rank
traces remain separately identified. The alternate worker is1828/189 with1592
unlike overlaps plus139 extra candidate bytes; it is not original-exact credit.

Run `python3 -B .analysis/setup-rank-publication-20261106/source-seal.py` for
the complete read-only verification.195 per-file SHA-identical source copies
and eight disposable PDBs totaling1409024 bytes are removed; all eight
actual new objects remain. Five private PDBs are covered by the sealed packet;
three lowercase canonical output names use supplemental path/SHA receipt
`b4c784c2294230e538243fabc928a2ed04cc8d2232d57735dad920c2ee6fe411`
(`.analysis/setup-rank-canonical-debug-cleanup-20261106.json`). Final public CI passes1376 tests/29 skips. Factory
acceptance and remote publication are not claimed.

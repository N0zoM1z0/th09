# SaveReplay offset and source-context recheck, 2026-11-06

The bounded continuation starts at `0d06206a`, with production source unchanged
from the prior native Player checkpoints. Fresh local Ghidra exports attest
SaveReplay and the record constructor against the original disk/Program SHA,
i386, base/entry and all685056 file-backed mapped PE bytes.

The maintained complete SaveReplay owner remains1238 bytes with53 independently
bound fields and two ordinary differences at+B7/+BB. The original loads the
record destination base into EDI, then forms the final EDI address from that
base and EBX. The maintained code uses ECX for that intermediate base. This
is an address-transport difference; no missing record-copy operation is found.

Nine natural source controls use the same fixed caller profile and complete
original-bound inspector:

| Source model | Complete observation |
| --- | --- |
| Unsigned32 file-offset counter | 1238 bytes/53 fields/two differences |
| Named record destination pointer | 1238/53/two |
| Actual allocation/free source co-visible | 1238/53/two |
| Actual RNG source co-visible | 1238/53/two |
| Both actual memory and RNG sources co-visible | 1238/53/two |
| Header and payload both represented as members | 1238/53/two |
| Backpatch file offset before copying the record | 1238/53/13; protected positions reject |
| Positive outer guard instead of the existing early return | 1247/53/76 overlapping differences; complete guard rejects |
| Initialize both chain cursors after record-offset publication | 1238/53/148 overlapping differences; complete guard rejects |

The delayed-chain control follows a reference-source timing hypothesis; its
rejection is a TH09 compiler observation, not proof of a unique original
source spelling. The header/payload member model preserves all physical
offset/size checks and independently reproduces the15/18-byte constructors.
This does not distinguish original inheritance from composition. An initial
private compile omitted one nested offsetof projection; it is corrected before
the reported complete compile, with the initial diagnostic retained separately.

All four real memory bodies remain original-equal,92 bytes total. All eight
real RNG bodies remain original-equal,244 bytes, including when combined with
the memory bodies. These RNG observations use the unchanged caller profile
with `/Oi`; their canonical producer profile omits that option and is not
changed. The twelve combined co-visible bodies therefore supply complete
callee evidence, not a new canonical producer-profile receipt or caller credit.

No source model is adopted. The two baseline bytes and all protected positions
remain the current frontier. Original class/TU identity and the rest of the
natural-source search remain open; no compiler-profile sweep, assembly,
register hint, barrier, dummy effect or fabricated return is introduced.

The ignored `.analysis/replay-current-offset-20261106/` packet retains15 frozen
source inputs, changed model files, compiler commands/logs, actual destination
instructions, complete reports, constructor/callee witnesses, `case-index.json`
and attested Ghidra exports. `probe.py CASE` restores verified frozen input
copies before recompiling a selected existing model. Objects stay below
`build/analysis/replay-current-offset-20261106/`.

Across this and the preceding Enemy packet,1094 SHA-identical source copies
are removed after indexing; frozen originals, all changed inputs, reports and
objects remain. Each packet records the source-copy recovery paths and hashes.
No original input, canonical source, toolchain or database is removed.

The current112 source/tool input hashes,14 actual object hashes in their recorded profiles and
eight compiler/SDK hashes still match the prior sealed source receipt. Unchanged
proofs are reused; no full cold replay is run. Totals stay1035 source-present/
1022 exact/13 nonexact. No new Factory, product/runtime or later-phase credit.

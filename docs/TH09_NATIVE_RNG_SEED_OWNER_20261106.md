# Native RNG seed methods, 2026-11-06

Replay, Network and startup now call the seed operations on the same
`RngRuntimeView` used by the generator. The duplicate `ReplayRngView` layout
and the Replay/Network reinterpret casts are removed. GetSeed, SetSeed and
ResetGenerationCount retain their existing source translation unit and
compiler profile. Their shared state is eight bytes: seed at +0, the neutral
word at +2 and generationCount at +4. Original nominal class and TU identity
remain unproved.

The original callers bind both RNG global spellings to 004ACE0C. Fresh direct
Ghidra covers GetSeed41A89E, SetSeed420160 and ResetGenerationCount420170,
attesting disk/Program identity and all 685056 mapped file bytes. Their full
original bodies remain 4, 14 and 8 bytes with no relocation fields. This is
physical storage and method evidence, separate from an original class name.
The two global spellings remain reconstruction aliases; native-product data
ownership and linking are later work.

Only the affected include closure is rebuilt: 58 source/profile groups,
covering 241 configured exact comparisons, plus the standalone network
service. All 241 pass 91667 complete bytes / 4878 fields. A second independent
canonical ReplayRuntimeLeaves build passes all four configured leaves,
39 bytes / zero fields. This repeats existing exact credit; totals remain
1034 authored present / 1021 exact / 13 nonexact.

The compiler renumbers 533 private relocation-field spellings, representing
512 per-unit labels in 19 units. Every refresh uses the actual same-owner
COFF definition and checks its unchanged original destination and operand.
Targets, addends, extents and profiles remain fixed. An initial cascading
text substitution produced an invalid transient manifest and was rejected by
the strict Oracle/CI. The corrected one-pass field mapping passes all complete
comparisons; no acceptance guard is relaxed.

The full affected nonexact owners retain their previous frontiers:

| Owner | Candidate bytes / fields | Full original comparison |
| --- | ---: | --- |
| RunEcl | 15564 / 598 | 30 ordinary differences; zero field/table differences |
| Enemy update | 3900 / 97 | 13 differences |
| Gameplay setup | 1689 / 171 | 904 differences |
| Bullet collision | 849 / 26 | 799 unlike overlaps; 13 missing bytes |
| Replay begin playback, both profiles | 538 / 32 | 39 unlike overlaps; two missing bytes |
| SaveReplay, both profiles | 1238 / 53 | Two differences |
| Shake envelope | 336 / 20 | 26 differences |
| Network service | 1627 / 106 | 1381 unlike overlaps; six missing bytes |

512 prior body/profile contributions retain full raw bytes and semantic COFF
identities. Five compiler-private cleanup definitions and five complete EH
handlers/data graphs also retain their actual contents after label renumbering.
Two old canonical Setup caches predate source 1d3a23c1's configuration-pointer
repair; the sealed current carriers replace those stale aliases for this
comparison. Both current Setup profiles retain their complete raw bodies and
fields. This accounts for all 524 reviewed prior contributions without
asserting original EH data or product closure.

The ignored packet `.analysis/rng-native-seed-20261106/` retains 169 frozen
before-inputs, current source hashes, all objects, exact/whole-owner reports,
label proofs, Ghidra exports and recipes. The 59 per-probe PDB debug databases
are reviewed by path/SHA and removed; all COFF objects remain. The separate
canonical leaf object also remains. Existing source-commit receipts describe
their own epochs and are not silently relabelled as current proofs.

Full target-independent CI passes 1376 tests with 29 skips; 21 focused tests,
target-required tracking, progress and whitespace checks pass. No new authored
exact function, Factory acceptance, remote publication or product/runtime
result is claimed. Current source proof is routed through RE_HANDOFF.md.

Source checkpoint is `398b69e443c994201e515396656770d2b3efba58`. The immutable
`source-proof.json` binds 196 tracked inputs and their Git blobs, eight compiler/
SDK files, all 60 new objects, existing Setup source carriers and every report
and recipe. Its SHA-256 is
`5e15c80d2e1ce618e6916f01915bb2c834c3e3505282eb301a6cbd6b43e3b443`.
Independent complete read-only replay passes with no compiler jobs:

```bash
python3 -B .analysis/rng-native-seed-20261106/seal.py --replay
```

The canonical four-leaf carrier can also be rebuilt separately with
`python3 -B scripts/build-match-unit.py --unit replay-rng-get-seed`, followed
by the configured complete comparison for each ReplayRuntimeLeaves unit.
The private packet supplies the broader scoped replay; a fresh clone uses the
maintained per-unit recipes rather than assuming ignored objects are present.

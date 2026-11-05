# Screen and player source review, 2026-11-05

The batch starts at main `75e021fe`, with 1019 exact and 16 nonexact functions.
Target identity and live ledgers pass before compilation. Fresh Ghidra exports
verify disk/Program SHA-256, i386, base/entry and all 685056 file-backed bytes.
The analyzed owners are the 336-byte shake envelope, 862-byte player collision,
485-byte result update and 139-byte screen clear. Ghidra remains provisional.

## Adopted native device access

Screen clear and rectangle drawing now use the established
Supervisor::d3dDevice08 directly. This removes twelve repeated device casts
and the duplicated device field from the local view. The remaining private
presentation-parameter window describes only the independently known +0x350
SDK record; the shared Supervisor layout is unchanged.

All three existing ScreenEffect compiler profiles rebuild serially. All 18
canonical units remain exact: 2562 physical bytes and 164 relocation fields.
The 527-byte rectangle renderer and 139-byte clear are included. Nine private
RegisterChain label spellings change; actual same-section COFF definitions
prove every unchanged target. No field offset, kind or destination changes.
All 28 focused screen tests pass. The complete shake envelope remains 336
bytes, 20 fields, eight calls, 26 blocks and 26 unequal positions.

## Fresh rejected controls

| Source control | Complete observation |
| --- | --- |
| Current shake baseline | 336 bytes; 26 unequal bytes |
| Ordinary hold/fade expression, without volatile references | 334 bytes; 249 unequal overlaps; two missing |
| Named envelope-tail expression | Identical 334-byte result |
| Ordinary expression with actual Timer definitions | 330 bytes; 248 unequal overlaps; six missing |
| Ordinary expression with actual RNG definitions | Same 334-byte result |
| Native unsigned hold/fall aliases and explicit timer comparator | Same 334-byte result |
| Current player baseline | 849/862 bytes; 799 unequal overlaps; 13 missing |
| Reference-shaped hit loop and reused collision/effect vectors | 830/862 bytes; 785 unequal overlaps; 32 missing |
| Same with cancellation inside the hit branch | Identical raw body |
| SDK local vectors, in either loop shape | Identical raw body |
| SDK pointer signature and checked ABI bridges | Identical raw body |

All five player models bind the same 26 independently reviewed fields and
actual scalar initializers. They emit two ret12 exits; the original has three.
The strict inspector rejects that changed exit census. The smaller overlap
count does not compensate for missing bytes or lost previously equal positions.
No player source/signature change is adopted. These are fresh full-profile
compiler observations, not claims that other natural models cannot work.
The unsigned shake control needs an explicit signed comparator because our
shared timer offers both integer and floating conversions; its first ambiguous
overload build provides no codegen evidence.

## Reproduction and evidence

Build only the affected source profiles, then use the existing focused guard:

```bash
python3 -B scripts/check-boss-frontiers.py --owner screen
python3 -B scripts/inspect-player-bullet-collision.py build/matching/PlayerCollisionHelpers.obj --json
```

Original-bound JSON, compiler logs, source controls and Ghidra exports are kept
in `.analysis/natural-effects-retry-20261105/`, with generated objects below
`build/analysis/natural-effects-retry/`. `probe.py`, `screen-natural.py`,
`screen-unsigned.py` and the `player-*.py` recipes retain the fresh controls.
Canonical ScreenEffect objects are under `build/matching/`. The earlier
RegisterChain label-mismatch log is superseded by `screen-native-canonical.json`:
all 18 strict comparisons pass after definition-backed name refresh.

[Cirno-Labs/th09](https://github.com/Cirno-Labs/th09) revision
`d7653152becabb8c86f2f43d49015ecc11bd0652` supplies source hypotheses.
No reference player body is adopted and no new exact credit is claimed here.
Whole-product and runtime closure remain separate.

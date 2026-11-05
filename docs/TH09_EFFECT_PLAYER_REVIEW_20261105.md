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

## Native result storage and stricter original-byte protection

The result updater now reads ReplayInputState slot 2 word06 and the shared
PlayStatsRecord characterUnlocked rows. Compiler assertions fix input stride
0x8E, word offset 6, unlock offset 0x4C and 16-byte rows. Five input and five unlock
operands retain their original effective addresses: input root 4ACE18+122;
statistics root 4A8180+4C/5C/6C. These roots were independently established by
existing TH09 exact owners. No target is derived from candidate operand bytes.
The already-exact 40+1 same-source units are replayed in both actual profiles.

The complete updater remains 484/485 bytes, 147 instructions, 34 fields, nine calls,
40 original-equal blocks and 218 unequal bytes. Its 266 already-equal positions
are now individually guarded, closing a count-only regression gap. The Oracle
also checks the actual read-only 19-byte result PNG literal against the original;
an unchanged symbol name cannot authenticate changed initializer bytes.
Old aliases remain readable for historical controls. New tests reject bad native
addends, changed literal payloads, wrong owner hashes and same-count exchanges
that damage any previously equal position. The source stays nonexact.

Fresh result character/loop models produce 483 bytes/224 unequal overlaps/two
missing; native data fields alone preserve 484/218/one missing. Actual player
Rotate2D visibility grows the maintained collision candidate to 865 bytes/826
unequal overlaps/three extra, so it is also rejected.

## Fresh Etama complete-pair retries

The reference-shaped draw with the real three-byte Float3 pointer conversion
body visible again reproduces the entire 600-byte/27-field/13-call parent.
Direct-index helper expressions, int/void source returns and three genuine
body placements still change the existing 232-byte/16-field bullet helper:
133 fully bound bytes differ. All controls use the current source and fixed
profile; actual local COFF definitions resolve tables without target fitting.
The source caller does not consume the helper result; changing the C++ return
spelling alone changes neither outcome. The current maintained parent/helper
pair is retained. Parent equality alone cannot authorize this integration.
No new folded-body authorship or unique original nominal type is inferred.

Final retained source passes all 41 Title canonical comparisons plus the new
complete result guard. Nine Title private label renames are proved against
actual COFF definitions. Full local CI passes 1333 tests with 29 skips;
tracking, documentation, generated progress and whitespace checks pass.
Totals remain 1019 exact and 16 nonexact.

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

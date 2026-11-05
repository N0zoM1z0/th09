# Replay state uses the physical GameManager members

Recording, playback, input gating and save metadata now use the GameManager
members already selected by the original instructions. Eighteen separate
global aliases are retired from ReplayInput; two of those aliases described
the same configuration-pointer cell. The shared native view gains the observed
32-bit side fields at +2C/+30, signed i32 at +138, and a GameConfiguration pointer
projection at +E8. Existing opaque byte views and all physical offsets remain.

## Original-bound state paths

The GameManager base is `0x4A7D90`, with two 0x38-byte sides. Fresh attested
Ghidra exports cover SaveReplay, BeginPlaybackStage and BeginRecordingStage.
Each run verifies disk/Program SHA-256, i386, image base, entry and all 685056
file-backed mapped PE bytes. Ghidra supplies provisional semantic observations;
the pinned compiler and complete PE/COFF comparison supply exactness evidence.

| Native member | Original address or offset | Replay use |
| --- | --- | --- |
| sides[0/1].character20 | 4A7DB0 / 4A7DE8 | frame +06 |
| sides[0/1].replaySelector28 | 4A7DB8 / 4A7DF0 | frame +07 |
| sides[0/1].value2C | 4A7DBC / 4A7DF4 | frame +0A u16 |
| sides[0/1].value30 | 4A7DC0 / 4A7DF8 | info +1E8/+1E9 bytes |
| clearD8 | 4A7E68 | previously seen character bytes |
| configurationE8 | 4A7E78 | complete CC-byte configuration and input modes |
| valueF4 | 4A7E84 | frame +0C byte |
| field0FC | 4A7E8C | current replay stage index |
| replayMode114 / gameMode | 4A7EA4 / 4A7EA8 | info +1E5/+1E4 bytes |
| difficulty | 4A7EAC | info +D7 byte |
| value138 | 4A7EC8 | signed frame +09 widened to i32 |
| seedCopy346 | 4A80D6 | three captured stage seeds |

These are physical reconstruction views, not unique original declarations or
new high-level names for the opaque fields. The configuration pointer's new
union member selects the same word as the existing gate projection. The side
words and signed field retain their old byte-array union views. No original
allocation, initialization, input limit, file branch or copy order changes.
The native +378/+37C counters from the earlier checkpoint are retained.

Twenty canonical relocation entries now name the native GameManager root and
use their actual compiler-emitted member addends. Every effective destination
was checked independently against the original operand; targets were not
changed to fit a candidate. The complete playback inspector now requires those
native identities. Tests reject equal-address legacy aliases and wrong member
addends for both sides, the signed field and configuration pointer.

## Bounded source investigation

Eight private compiler controls test the remaining replay questions:

- Typed frame assignment and payload-relative indexing reproduce the entire
  maintained SaveReplay raw body and COFF rows. Neither closes its two bytes.
- An explicit mutable frame-table slot keeps 1238 bytes/53 fields but introduces
  four additional differences. The complete-owner guard rejects it.
- Existing-member and complete-member migrations with the retained single
  pointer access preserve playback's complete diagnostic. Ordinary local or
  direct side pointers instead produce 49 unlike overlaps and reorder a field.
- A u32 seed-argument hypothesis preserves the setter's complete 14 bytes,
  but leaves playback at 538/540 with 39 unlike overlaps and two missing bytes.
  It does not establish a different original prototype and is not adopted.

The source adopts the independently observed member ownership only. Copy
abstractions, argument-width changes and pointer-order regressions remain
experiments. The existing volatile projection is not a recovered original
qualifier or a recommended codegen technique; no new one is introduced.
Negative controls are bounded counterevidence, not an exhaustion claim.

## Complete affected-owner replay

Quoted-include traversal identifies 112 source users and 348 configured units
in 118 existing source/profile groups. These groups are compiled serially at
lowered priority with a 180-second bound. Identical source/profile output aliases
share the same actual compiled object; all configured paths and hashes are
recorded. Movement and Charge add two affected diagnostic builds using their
existing Player profile. There is no unrelated full cold replay.

The builds expose seven older source declaration conflicts. The independently
committed [enemy lifecycle repair](TH09_ENEMY_LIFECYCLE_NATIVE_MANAGER_20261105.md)
restores four complete owners; the [callback repair](TH09_CALLBACK_NATIVE_MANAGER_REBUILD_20261105.md)
restores six. Each old source/header closure reproduces the failure, and each
fixed source also matches with its old header. Their source checkpoints stand
independently of this expanded header. Successful earlier groups are reused
only after checking their unchanged dependencies, profiles and actual objects.

All 348 configured units now match 115410 complete compared bytes and 6134
relocation fields. Nineteen units require reviewed compiler-private spelling
updates across 466 field references, including one generated suffix for the
existing source label `title`. Each actual same-owner definition, root/addend
and original encoded operand is checked before the manifest name changes;
every complete function, physical tail and table then passes the normal
strict comparator. No public acceptance rule is relaxed.

The eight affected nonexact dependent bodies retain their full raw hashes and
actual external identities/local definition positions: RunEcl, Enemy update,
gameplay setup, Effect AddedCallback, shake, Player collision, movement and
charge. RunEcl's twenty references to the existing reconstruction label
`th09_ecl_advance_instruction` retain their actual owner offset despite its
generated suffix changing. A fresh full original-bound RunEcl diagnostic
retains 15564 bytes/598 fields, 30 ordinary differences and no field/table
differences. The full Enemy diagnostic retains 3900 bytes/97 fields and 13
differences. These are preserved nonexact results, not partial credit.

Both Replay profiles retain twelve canonical units/4093 bytes/232 fields
within the larger cohort. SaveReplay remains 1238 bytes/53 fields, with only
+B7/+BB unlike. Playback remains 538/540 bytes/32 fields, 39 unlike overlaps
and two missing bytes; all original field identities and the direct graph are
preserved. The pre-copy uninitialized header scan and original cleanup remain.

The ignored `.analysis/replay-frame-copy-20261105/` packet retains attested
exports, frozen inputs, eight controls, old-source failures, independent repair
contexts, all build logs, actual objects, complete comparisons and the 235
current source input hashes. The compiler/SDK inputs are pinned separately.
`replay-native-cohort.py` records the focused serial builds and reviewed resume
points; `finish-native-cohort.py` checks complete equality and unchanged owners
before applying only the proven symbol spellings.

No new exact-function or authored-origin credit is added: totals remain 1035
source-present / 1022 exact / 13 nonexact. Whole Windows product closure,
runtime validation, dual-Oracle semantics and portability remain open.

Full public CI passes 1355 tests with 29 skips. All 49 focused Replay tests,
91 Enemy tests and six Card tests pass. The focused ExAttack invocation with
the repository scripts on its import path passes 63 tests with four skips.
Obsolete configuration-access and union-spelling assertions are removed;
their physical offset guards and complete original-byte checks
remain. Target/tracking, progress, documentation and whitespace checks pass.

## Immutable source receipts

The native-state source checkpoint is
`c8a0bc15c07520b95abba2e218d3a5ee2a70054e`. The ignored
`canonical-source-proof.json` is SHA-256
`194c65dc2bb227ea5a360ca163dde3ad5796c783d55ce265d06ab56f58b8e540`.
It binds 276 tracked source/tool/test inputs and normalized Git blobs, 132
actual objects/profiles, eight pinned compiler/SDK files, all complete unit
results, unchanged nonexact reports, private controls, successful attested
exports and public CI. Six unchanged allocator/RNG/replay-constructor/LZSS
callee proofs are reused only after source/profile/object checks and complete
original-byte replay. The affected GameManager mode helpers are already
freshly compiled among the 348 units.

The packet also retains two independent source receipts: lifecycle source
`a7cbf13fdc3ccb23fab6e780b97ea5fd5aeb3247`, receipt SHA-256
`5079d12805e100aa6c511f83fc00a8b35117cd5dabd4f88a5bee655bb0df7ae9`;
callback source `533bf0267c1e3ac646265fb51688e2d0825eb23d`, receipt SHA-256
`a2825a29ef2b7053ada10f885282bcd02eebff6543ba43826908b552952a30d9`.
These bind the fixed sources to their old-header compiler contexts as well as
the separately recorded matching new-header builds.

`python3 -B .analysis/replay-frame-copy-20261105/verify-native-receipts.py`
checks immutable/current inputs and retained reports/objects, then independently
replays all 348 affected units, six unchanged callees and ten historical
recovery comparisons. It writes a separate `canonical-source-replay.json`
without replacing any sealed receipt. These are local source-bound compiler
receipts, not Factory verdicts or remote-publication evidence.

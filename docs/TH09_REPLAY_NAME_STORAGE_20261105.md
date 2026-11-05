# Replay player-name storage observed in the original

The reconstruction view now gives `playerName` nine bytes at record offsets
`+0xCE..+0xD6`: eight title-entry characters and their copied zero terminator.
The formerly separate `unknown0D6` byte belongs to this observed string span.
The difficulty byte remains at `+0xD7`, the configuration at `+0xDC`, and the
complete record remains `0x1EC` bytes. The original copy, input limit and wire
behavior are unchanged. This establishes a useful view of the accessed bytes;
it does not uniquely recover the original C++ member declaration.

## Fresh TH09 observations

The local Ghidra runner re-examined SaveReplay `0x420C60`, the title save update
`0x429662`, and the title constructor `0x429D83`. Each run attested the original
disk and Program SHA-256, i386 language, image base, entry point and all 685,056
file-backed mapped bytes. Ghidra's function bodies and pseudocode are semantic
observations, not the compiler Oracle or ownership boundaries. SaveReplay's
reviewed complete comparison remains 1,238 bytes, rather than Ghidra's shorter
body inventory.

The title constructor clears its `0x1B388`-byte object. The title name begins
at `this+0x8C`; keyboard edits address positions zero through seven, with the
cursor increment conditional on `<7`. The save action passes this name to
SaveReplay. The keyboard global at `0x4A1CF4` is a pointer, whose original
file-backed value selects the character table at `0x48F470`; key zero is `A`.
It is not an inline alphabet at the global's address. The saved-name global
at `0x4A8174` is outside file-backed data, so it supplies no PE string literal
evidence.

The original SaveReplay constructor setup selects the local record at
`EBP-0x218`. Its string copy at `0x420F53..0x420F70` selects `EBP-0x14A`, hence
record offset `+0xCE`, and copies each source byte before testing for zero.
A bounded interpreter of those actual, hash-verified PE instructions traced
the valid eight-`A` name plus zero. Fifty original instructions write exactly
nine name bytes at `+0xCE..+0xD6`; the last write is zero and `+0xD7` is
untouched. This answers the field-span question without a whole-game runtime
claim or extra malformed-input tests.

## Compiler investigation and retained source

The pinned Cirno-Labs reference suggested a nine-byte name and a smaller
constructed info subobject. It remains hypothesis material. Six private,
serial VC7.1 controls tested the destination helper, a copy-and-append helper,
whole-record representation copying, info-member composition, composition
with a nine-byte name, and the nine-byte name alone.

Five retain the complete 1,238-byte / 53-field SaveReplay diagnostic and its
two differences at `+0xB7/+0xBB`. The copy-and-append helper instead emits 1,254
bytes with 1,027 unlike overlap bytes and fails the existing complete-owner
guard. The composition controls also preserve both 15/18-byte constructor
bodies. These results do not distinguish the original inheritance/composition
or constructed-subobject extent. Those changes and the destination helpers
are not adopted. Only the name span, independently supported by the TH09
copy and title-entry observations, changes in production source.

## Focused affected-owner replay

Quoted-include traversal found seven source users and 68 configured units in
nine existing source/profile groups. All nine were compiled serially at
lowered priority with the pinned compiler and bounded duration. The two
GameManager profiles use their independently reviewed output paths; see
[the cache checkpoint](TH09_GAME_MANAGER_PROFILE_OBJECTS_20261105.md).

All 68 units pass complete original-byte and actual relocation comparison:
26,948 physical bytes and 1,474 relocation fields. This includes complete
physical tails and tables, not selected equal blocks. Constructor exactness
does not promote their still-unknown origins to authored-source credit.

The header change shifts nine compiler-private `$L` spellings in the title
start-menu unit. The complete 1,592 raw bytes, all addends and actual COFF
owner-relative symbol positions are identical before/after. Each renamed
label still resolves to its unchanged original destination, including the
physical jump table. Only these reviewed names change in the manifest;
field offsets, types, targets, extents and acceptance rules are unchanged.

The affected nonexact owners retain their entire raw code and actual COFF
rows, with these complete diagnostics:

| Owner | Candidate / original bytes | Fields | Unlike overlap bytes |
| --- | ---: | ---: | ---: |
| SaveReplay, both profiles | 1,238 / 1,238 | 53 | 2 |
| BeginPlaybackStage, both profiles | 538 / 540 | 32 | 39, plus 2 missing |
| GameplaySetupThread, `/Os` | 1,689 / 1,689 | 171 | 904 |
| Title OnUpdateResult | 484 / 485 | 34 | 218, plus 1 missing |
| Title DrawReplayMenu, three contexts | 1,280 / 1,280 | 57 | 29 |

The menu's standalone, declarations and native-source contexts were each
rebuilt with the existing title profile. Their full raw hashes, actual fields
and complete diagnostics are identical to the fresh eight-byte-view controls.
There is no new function-exact credit: totals remain 1,035 source-present,
1,022 exact and 13 nonexact. Windows product closure, game runtime validation,
dual-Oracle semantics and portability remain separate open phases.

The ignored `.analysis/replay-frame-destination-20261105/` packet retains the
three attested Ghidra runs, target-instruction trace, six private controls,
before/after objects, compiler logs, dependency inventory, label review and
complete comparison reports. `replay-name-storage.py`, `verify-name-labels.py`
and `verify-name-storage.py` record the focused replay, with 53 source inputs,
nine actual profiles/objects and eight pinned compiler/SDK inputs.
The five unchanged allocator/mode/RNG/compression callee proofs are reused only
after checking their source, profile and actual object hashes against the
earlier immutable receipt. Both affected replay constructors are freshly
compiled and compared among the 68 units.

All six shared-layout tests pass. The obsolete assertion requiring the literal
eight-byte declaration is removed; the physical-neighbor and original target
checks remain. Full public CI passes 1,354 tests with 29 skips. Target/tracking,
progress, documentation and whitespace checks pass.

## Immutable source binding

The source checkpoint is `4adec272cd857a39d36432078e5e666c27c80b18`.
The ignored `canonical-source-proof.json` is SHA-256
`4e4b1c873b127832aa0cf2d53d7c46636e4e0a340501e8d3cc04acbd02a90826`.
It binds 76 tracked source/tool/test inputs and their normalized Git blobs,
13 actual objects/profiles, eight compiler/SDK inputs, all 68 complete unit
results, unchanged nonexact diagnostics, reused callees, three attested Ghidra
reports, private controls, original-copy trace and public CI log to that
immutable source commit. It also binds the separate historical cache receipt.

`python3 -B .analysis/replay-frame-destination-20261105/verify-source-proofs.py`
checks the committed and current inputs, retained objects and report hashes,
then independently replays all 68 current comparisons and the eleven
historical cache comparisons against the original PE. It writes a separate
`canonical-source-replay.json`; neither sealed receipt is overwritten.
The receipt registry records these as local source-bound evidence. Later
documentation commits, original source uniqueness and Factory acceptance are
not automatically covered by a source checkpoint.

# Player movement uses implemented native math, 2026-11-06

Recovery starts at clean `b60b5f65`; target, tracking and status checks pass.
Fresh local Ghidra attests original disk/Program SHA-256, x86 little-endian
32-bit language, base `0x00400000`, entry `0x0047D45F`, and all685056 file-backed
mapped bytes. The successful export covers movement, both54-byte math providers
and the three-byte pointer identity body. An initial request mistakenly selects
interior address `0x004010F0`; that batch fails closed and is retained separately,
without crediting its partial output.

The independently checked full owner is **1835 code bytes + 1 alignment byte +
64 switch-table bytes = 1900 physical bytes**. Ghidra's provisional disjoint
ranges report1832 bytes and do not replace that complete comparison. The six
original bounds operations call subtraction401140 and addition401100; both
read three floats through ECX/reference operands and construct the returned
12-byte value through the exact26-byte Float3 constructor.

The movement caller formerly named declaration-only PlayerPositionView math
interfaces. It now projects its existing three-float position, half sizes and
six destination members to **Float3**, resolving to the implemented native
providers in the ECL carrier. A storage guard checks size and x/y/z offsets.
Player's header and construction are unchanged. No wrapper/helper body is
introduced, and original class names/TU membership remain unproved.

| Actual caller field offset | Native provider | Original target |
| --- | --- | --- |
| `+0x59E`, `+0x5E8`, `+0x632` | `Float3::operator-` | `0x00401140` |
| `+0x5C0`, `+0x60A`, `+0x654` | `Float3::operator+` | `0x00401100` |

Before/private/adopted builds use the same pinned VC7.1 movement profile.
All **1900 raw bytes**, **66 effective field identities** and **122 mismatch
positions** are unchanged, including both tables and **23 ordered calls**.
All **1778 already-equal physical bytes** remain protected. Owner-local field
spellings are checked through their actual same-section coordinates when names
change. No partial exactness is awarded. Obsolete129-byte ledger prose is
corrected to the current122 frontier.

The fresh522-byte Player OnUpdate and18-byte AnmVm SetInterrupt remain exact.
The three native math providers replay **134 bytes / 2 fields** from unchanged
`bfa170c9` source/object evidence: all86 ECL source inputs, original Git blobs,
object hash, three unit recipes/profiles and eight compiler/SDK hashes are
independently checked. There is no ECL rebuild. Combined, these are **five
complete canonical comparisons / 674 bytes / 42 fields**. The preserved
provider receipt SHA-256 is
`e52571a93f3b68573aee8cae83b021fe078549efa8133663b5801a93563e5ada`.

Seven isolated source controls use frozen17-file inputs. Removing all16 SHT
aliases yields1900/66/252 and fails the122-position guard; its combinations
with a native-vector header and actual math-body visibility also fail. The
native-vector header alone or with actual definitions retains1900/66/122,
which establishes only that caller's codegen. Local math projection and its
real-definition control likewise retain1900/66/122. All three actual-core
contexts reproduce the26/54/54-byte providers in full. None proves global
Player type substitution. Only the local provider correction is adopted.

The public inspector recognizes native symbols. The movement runner requires
all six native field identities and original edges; calls through former
private names are rejected even with identical numerical destinations. Six
synthetic relocation-contract tests reject changed owners, targets, offsets,
kinds, addends, missing/duplicate fields and extra math references, without
source-spelling assertions. Fifty movement-focused tests pass with two skips.
Full public CI passes1370 tests with29 skips; target-required tracking,
documentation, progress and whitespace checks pass.

Ignored `.analysis/player-movement-current-20261106/` retains frozen sources,
seven full controls, before/adopted reports, actual fields, Ghidra and compiler
logs. `verify-current.py` checks the complete caller, six native edges and five
canonical units, including unchanged provider provenance. Builds stay below
`build/`. Totals remain1035 source-present /1022 exact /13 nonexact. Windows
product/runtime, semantic reconstruction, portability and Factory acceptance
remain open.

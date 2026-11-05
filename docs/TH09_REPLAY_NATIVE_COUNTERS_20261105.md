# Replay uses the native GameManager counter pair

Replay recording, playback and save metadata now use the existing
`g_GameManager.value378` and `value37C` members directly. Four independent
global declarations are removed from `ReplayInput.cpp`. This is a physical
owner correction, not a new exact-function match: all twelve existing canonical
ReplayInput units remain exact, and both open whole-function frontiers retain
their previous resolved bytes.

## Original-bound ownership evidence

The Japanese TH09 original is unchanged. Fresh direct Ghidra analysis attests
disk/Program SHA-256, x86 LE32 language, image base `0x400000`, entry point
`0x47D45F` and all 685,056 mapped file bytes before exporting SaveReplay,
BeginPlaybackStage and the independent Supervisor reset helper. Ghidra's body
sets and decompilation do not define the accepted complete comparison extents.

The existing native GameManager root is `0x004A7D90`; its checked members at
`+0x378/+0x37C` are the original dwords at `0x004A8108/0x004A810C`. The original
reset at `0x0041BA4F` writes 10 to both. The Replay paths use those same words:

| Path | Observed use of the two native fields |
| --- | --- |
| BeginRecordingStage | Captures their low bytes in replay payload `+0x1E6/+0x1E7` |
| BeginPlaybackStage | Zero-extends those saved bytes and restores both dwords |
| SaveReplay | Converts the signed dwords to floating point and multiplies each by `0.5f` for the initial-health metadata line |

The removed names were `g_ReplayHeaderValue1E6`, `g_ReplayHeaderValue1E7`,
`g_ReplayInitialHealth0` and `g_ReplayInitialHealth1`. They described two
addresses through four global interfaces. The shared layout, original operands,
recording comparison and complete playback/save inspectors establish the
physical connection. This does not establish a unique original C++ field name
or every gameplay role; the existing neutral native member names are retained.

The canonical recording unit changes only the two relocation roots/addends at
`+0xE1/+0xEC`. Their effective original destinations remain unchanged. The
playback inspector likewise requires the native root with exactly `0x378/0x37C`.
The save inspector removes both old aliases and permits these two native
addends while retaining its exact original destination/order checks. New tests
reject the former equal-address aliases and swapped side addends. Original
target hashes, physical field destinations, complete extents and frontier
acceptance rules are unchanged.

## Complete compiler results and source controls

Both actual ReplayInput profiles are freshly rebuilt serially at lowered
priority: the existing `/O2 /Ob1 /EHsc /Oy-` profile and its existing no-EH
counterpart. All twelve canonical units pass 4,093 physical bytes and 232
relocation fields. No private table-label updates are required. Seven unchanged
callee bodies are also rebuilt and compared: memory allocation/free, mode2,
RNG range, the two replay constructors and LZSS encoding, totaling 698 bytes
and 13 fields. Constructor origin dispositions remain unchanged; these checks
do not promote unknown-origin bodies to authored code.

In both actual profiles, SaveReplay retains all 1,238 bytes, 53 fields,
380 instructions, 23 ordered calls and 49 agreeing blocks. Its complete
resolved stream is identical to the pre-change baseline; only the existing
three differences at `+0xB7/+0xBB/+0x3F6` remain. BeginPlaybackStage retains
538/540 bytes, 32 field identities, five calls, twelve agreeing blocks,
39 unlike overlaps and two missing bytes.

Fresh bounded controls also test two reference hypotheses before adoption:

- Acquiring the input chain after copying the frame header, as in the pinned
  Cirno-Labs reference, retains the extent/calls/graph but introduces 146 new
  unlike positions, leaving 149 total. The strict guard rejects it.
- The reference's signed checksum accumulator and unsigned fixed-loop bounds
  reproduce the complete maintained raw body and every actual COFF row. They
  do not close any of the three residuals and are not adopted.

The reference remains hypothesis material at
`d7653152becabb8c86f2f43d49015ecc11bd0652`; it supplies no TH09 ownership or
exactness proof. No register hints, declaration-order tactics, barriers,
assembly, padding or profile search are introduced.

## Reproduction and scope

The ignored packet `.analysis/replay-save-complete-20261105/` preserves the
original twelve-file closure and original metadata, four changed controls,
actual objects/logs, complete comparisons, Ghidra attestations and recipes.
`scripts/check-boss-frontiers.py --owner replaysave` remains the maintained
focused replay. Packet `callees.py` rebuilds the five callee carriers;
`proof.py` checks the complete source/object/profile evidence and binds it to
the committed source. Its immutable receipt binding is recorded separately
after the source checkpoint.

All 47 Replay tests pass. Full public CI passes 1,353 tests with 29 skips.
Tracking, progress, documentation and whitespace checks pass. Totals remain
1,035 source-present / 1,022 exact / 13 nonexact. This is local source and
compiler evidence; original TU ownership, faithful Windows product closure,
runtime validation, Factory acceptance and portability remain open.

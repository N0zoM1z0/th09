# Player movement native options/history arrays — 2026-11-07

The original Japanese TH09 1.50a executable was target-verified as SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint edits the maintained **large 1,900-byte physical** Player
`UpdateMovementAndOptions`, not just a small callback wrapper.

The native `PlayerLifecycleView` already physically owns `optionStates[4]`
(records of 0x2F4 bytes with timer at +0x2E0 and callback +0x2EC) and
`positions1BA0[16]` (16 original 0x0C-byte history vectors). The movement
loop formerly counted down from a disconnected literal `4`, while history
shift started from literal `15`. An ordinary in-function C++ enum now derives
both `OptionCount` and `PositionHistoryLast` from their actual source arrays.
No separate buffer, callback, transient allocation, or inferred original C++
name was introduced. The enum constants remain ordinary signed integral
loop bounds rather than letting `sizeof` silently change comparison signedness.

Eight isolated natural C++ candidates were compiled serially under the same
pinned VC7.1 movement profile; every candidate was measured over the full
code + alignment + both jump tables (1,900 physical bytes) against the
original PE/COFF. The private probe and source/object SHA evidence are at
`.analysis/gptweb-player-native-bounds-20261107/`.

| Source model | Physical bytes | Actual fields | Calls | Unlike original bytes |
| --- | ---: | ---: | ---: | ---: |
| Maintained baseline | 1,900 | 66 | 23 | 122 |
| Direct array-derived loop sizes | 1,900 | 66 | 23 | 122 |
| In-function enum array sizes **adopted** | 1,900 | 66 | 23 | 122 |
| Direct `PlayerPositionView &` clamp | 1,884 | 65 | 22 | 631 |
| Direct `Float3 &` clamp | 1,884 | 65 | 22 | 631 |
| Float element references | 1,900 | 66 | 23 | 122 |
| Preloaded coordinate minimums | 1,884 | 62 | 23 | 608 |
| Array-derived sizes plus element references | 1,900 | 66 | 23 | 122 |

In particular, an apparently more natural direct position-object reference
**removes a true Player-position-to-float-pointer conversion call** and cannot
replace the source's existing conversion expression. The short versions are
rejected, not counted as any partial exactness win. The adopted enum version
has identical raw emitted bytes to the baseline, with **all 1,778 existing
equal positions preserved** and no new table/field deviation.

A source-cold, original-bound `--owner movement` replay after the real source
edit validates exactly 1,900/1,900 physical bytes, 66 original-bound fields,
23 ordered calls, 435 normalized aligned instructions, both tables and a
**122-byte residual**. Its two affected exact native providers replayed
**540 complete bytes and 40 relocations**. This change increases source
semantic fidelity, not canonical exact-function credit: 1,034 source-present,
1,021 exact, 13 nonexact remain. No entire-game cold replay, product build,
runtime equivalence, Factory acceptance or remote push is claimed.

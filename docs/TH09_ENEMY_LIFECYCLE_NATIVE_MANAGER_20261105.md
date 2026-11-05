# Enemy lifecycle source builds with the native GameManager

EnemyManagerLifecycle now uses the GameManager type already declared through
its shared headers. Its separate `extern unsigned char g_GameManager[]`
conflicted with that native declaration and prevented the current source from
compiling. Removing the conflict and using the actual side records and stage
member restores all four configured lifecycle comparisons without changing
their existing compiler profiles or original extents.

## Reproduced source failure

The failure was not attributed to the pending replay-field work. A private
nineteen-file closure reproduced the source and headers from committed
`24a08fb8`, including the old GameManager header. Every input was checked
against its normalized Git blob. The unchanged pinned compiler profile still
rejects the two incompatible GameManager declarations with C2040, followed by
errors from the attempted byte-array arithmetic. Historical object receipts
do not prove that this later source snapshot remains buildable.

## Native source and complete comparisons

Create now selects `g_GameManager.sides[sideIndex]` and
`g_GameManager.sides[1-sideIndex]`. Their addresses are projected into the
existing independently sized 0x38-byte EnemySideStateView. AddedCallback reads
the existing native `valueF4` member. The global remains based at `0x4A7D90`;
the original side and stage operands independently bind addends 0, 0x38 and
0xF4. No separate global storage, ABI change or defensive branch is added.

| Complete owner | Bytes | Relocation fields |
| --- | ---: | ---: |
| Create | 328 | 29 |
| AddedCallback | 372 | 24 |
| Initialize | 554 | 5 |
| Constructor | 101 | 9 |
| Total | 1355 | 67 |

Both original configured profiles are used: `/O2 /Ob1` for the first three
and `/O2 /Ob0` for the constructor. Only three manifest symbols change from
the obsolete byte-array declaration to the native GameManager identity.
Field offsets, types, target roots, extents and acceptance rules are unchanged.
The actual COFF addends and every original byte pass complete comparison.

The fixed source was also built independently with the committed old header.
All four raw bodies and actual COFF rows are identical to the builds using
the pending expanded header. Thus this source checkpoint stands independently
of that later shared-header change. Origin classifications and exact-function
counts are unchanged; the result restores current source evidence for these
existing units rather than adding new credit.

## Ghidra and evidence scope

Direct Ghidra attests disk/Program identity, architecture, base, entry and all
685056 file-backed mapped bytes. Create, Initialize and the constructor export
completely. An earlier four-entry request reports no function at AddedCallback
`0x411DB0`, so it is not a successful four-function export. Create's original
callback operand identifies that entry; the pinned original PE/COFF Oracle
independently covers its complete 372 bytes, including the stage read. No
Ghidra boundary or naming change is inferred from compiler equality.

The ignored `.analysis/replay-frame-copy-20261105/` packet retains the original
nineteen-file closure, failure log, fixed old-header inputs, both actual
profiles/objects and complete comparisons. `old-lifecycle.json`,
`native-lifecycle-verified.json` and `fixed-lifecycle-old-header-verified.json`
separate the reproduced failure from both successful source contexts. Native
product/link/runtime closure, original source uniqueness and Factory
acceptance remain independent open claims. The broader affected-header replay
continues separately.

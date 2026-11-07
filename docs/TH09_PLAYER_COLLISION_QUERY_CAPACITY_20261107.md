# Player collision-query native 128-record allocation across three append paths

Original: Japanese TH09 1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The source-facing name of the native query type remains provisional.

## Real physical record pool, not three independently chosen limits

`PlayerCollisionQueryStateView` in the Player allocation starts with the
owner at +0x00, followed by **128 records of 0x30 bytes** beginning at
+0x04; the live signed `count1804` follows at +0x1804, with the entire
query object occupying 0x1808 bytes. The collision query is embedded in
the Player allocation at +0x36C. Original append paths stop once the
count reaches 128. The FindCollision reader traverses the live count and
is not gratuitously clamped to a different behavior.

`PlayerCollisionQueryCapacity::RecordCount` is now derived from the actual
`records` array instead of repeating `128` independently. Three original
append paths use it: **205-byte AppendBoxRecord** compiled in the ECL/Enemy
source carrier, **245-byte AppendLaserRecord** in the Player carrier, and
**95-byte AppendCircleRecord** in its separate original-facing method
carrier. The circle view's physical 0x1808 size and count coordinate are
compared with the native Player query allocation, rather than describing
an independent differently sized pool. The original ABI/function ownership
and write order are not altered in this checkpoint.

The 3,900-byte EnemyManager::OnUpdate also calls the exact 82-byte collision
wrapper, which in turn calls the exact 205-byte AppendBoxRecord. This
source model therefore protects a **complete big boss and the leaf chain**,
not a new isolated helper. The present big boss remains nonexact.

## Fresh complete original comparisons

Pinned VC7.1 source-cold comparison of the changed ECL carrier passes **83
complete existing exact owners / 29,856 physical bytes / 1,389 fields**.
The two independent Player append leaves pass **245 + 95 = 340 bytes / 2
fields**. Across these affected carrier scopes, **85 exact functions /
30,196 physical bytes / 1,391 fields** were checked. Separately, complete
Enemy OnUpdate remains 3,900 bytes / 97 fields / 69 calls with **13
ordinary byte differences**; its 82-byte collision wrapper remains exact.
This grants no new authored exact credit.

Two ECL compiler-local names refreshed in the main exact object:
`ecl-post-update-movement` field +0x139 became `$L76200`, still resolving
0x00408548; `ecl-compare-operands` field +0x14 became `$L75381`, still
resolving 0x00407A4C. Both source label definitions, owner sections,
field offsets/types/addends, and original PE destinations were checked
before changing only the names in the canonical manifest. All 77 main
profile ECL exact owners were independently audited; no other relocations
changed. Private proof:
`.analysis/gpt-web-player-query-capacity-20261107/ecl-label-proof.json`.

These are original-byte PE/COFF and compiler observations, not runtime
acceptance, portability, or whole-product closure. No full cold replay was
run. Counts remain **1,034 source-present / 1,021 exact / 13 nonexact**.

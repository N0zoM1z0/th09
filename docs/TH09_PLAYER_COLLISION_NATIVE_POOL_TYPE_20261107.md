# TH09 bullet collision — native region pool and region-type snapshot (2026-11-07)

The Japanese v1.50a `PlayerLifecycleView::CheckBulletCollision` remains a
nonexact **862-byte** original owner. The maintained 849-byte candidate scans
real collision regions belonging to its Player allocation, not a separately
allocated bullet array. The existing `PlayerCollisionRegionCreateView` already
identifies the active pointer array at native Player `+0xB100`, with actual
`PlayerCollisionRegionCreateRegionView` entries of `0x44` bytes and type
field `+0x38`.

The revised C++ now borrows one `PlayerCollisionRegionCreateView& pool` over
the Player's real storage, gets its `pool.activeB100` array and snapshots the
current region's `type38` through a typed pointer before the original
three-way type eligibility check. Later reads from `*slot` remain live: we do
not invent a new region allocation or assume that helpers can never mutate
the slot chain. Geometry, cancel response and the actual reward/effect callees
are unchanged. No register forcing, artificial padding, volatile barrier,
compiler-profile substitution or target byte patch is used.

## Actual focused experiments and non-regression

Ten natural C++ whole-owner loop/type variants were tried with the pinned
VC7.1 source profile, including typed pool locals, region/type snapshots,
indexed traversal and alternate null-exit/for-loop models. Two more combined
pool/type variants were tested. The chosen model retains every previously
equal target byte: **849 candidate / 862 original physical bytes, 26 actual
bound fields, 12 ordered calls, 799 unlike overlapping bytes, 13 missing
bytes, 32 vs 33 control-flow blocks**. A C++ type range test emitted 859
bytes but damaged 49 previously equal positions. An infinite-for/null-break
variant altered the original three-`RET 12` contract. Both were rejected.

`python3 -B scripts/check-boss-frontiers.py --owner player` passes with this
source and independently verifies all **seven related exact original-bound
units** (1,943 physical bytes / 41 fields). This remains a local diagnostic
checkpoint, **not a newly exact function**, complete source ownership proof,
Factory approval, full cold replay or native runtime acceptance.

The ignored control sources/objects and accepted diagnostic are preserved at
`.analysis/gpt-web-player-collision-loop-20261107/`.
Original target SHA-256:
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.

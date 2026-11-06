# Exact Player selector: shared Enemy allocation ownership, 2026-11-06

This is a **gameplay source** change, not a ledger-only rearrangement.
`PlayerUpdateSelectorState` at `0x004049A0` is a sizeable 3,238-byte exact
owner. Its opponent-target and pattern-history path previously walked the
same EnemyManager and Enemy allocations through repeatedly cast `void *`
pointers and numeric offsets. The native structures already existed and were
independently used by the complete Enemy update and ECL compiler owner.

The live selector now uses `PlayerSideStateView::enemyManager10` (the one real
`AsciiGameManagerSideView` +0x10 slot), typed `EnemyManagerView` enemy-counts,
reward count, priority and first-active pointers, `EnemyView::position2D74`
and `secondaryFlags3380`, and `PlayerLifecycleView::targetOverride30F64`.
The one remaining boundary reinterpretation copies the Enemy's XYZ bytes into
the existing PlayerPosition workspace; compile-time assertions require both
native three-float views' sizes and field offsets to match. No private
stand-in enemy pool or duplicate manager allocation is introduced. These
replacement types describe concrete shared storage, not proof of retail
nominal class names.

A follow-on source checkpoint has also replaced the remaining raw +0x3044C
and +0xC110 paths, using the independently existing
`PlayerLifecycleView::ownerState30410.activeFrameCounter3C` and
`PlayerCollisionRegionCreateView::activeCountC110`. Compile-time offset
assertions pin both storage paths. This supersedes the two-unknown-fields
statement in the initial checkpoint, without claiming original class names
or any newly matched authored bytes.

## Actual pinned compiler comparisons

Two independent compilation profiles for this same source were built (no
full-game cold replay):

| Owner | Target size | Complete compared bytes | Original fields | Result |
| --- | ---: | ---: | ---: | --- |
| `player-update-selector-state` | 3,238 | 3,238 | 136 | Complete original-bound exact |
| `player-resolve-pattern-offset` | 346 code, 412 with both tables | 412 | 18 | Complete original-bound exact |

The new source include also changes VC7's automatic `$L` label sequence in
the separate resolver profile. **All 18** actual COFF fields were first read
with their real symbol-section and owner-relative values; every one resolved
to the **same independently retained original target address** as before.
`config/match-units.toml` updates only these 18 compiler-generated symbol
spellings, not their offsets, relocation types or target destinations. Two further local table roots are renumbered by the second native header and
verified to land on the same two original destinations. This
is not a field mask, target fitted alias, or a new exactness claim. No owner
changed code bytes and no newly matched authored byte is counted; both were
already exact in the 1,021-unit baseline.

Rebuild and check only these two owners after source changes:

```bash
python3 -B scripts/build-match-unit.py --unit player-update-selector-state
python3 -B scripts/compare-coff-function.py --unit player-update-selector-state --json
python3 -B scripts/build-match-unit.py --unit player-resolve-pattern-offset
python3 -B scripts/compare-coff-function.py --unit player-resolve-pattern-offset --json
python3 -B -m unittest discover -s tests -p 'test_player_gameplay_native*.py'
```

`result: exact` above means a complete compiled-source comparison to this
pinned TH09 target, not fresh Factory acceptance, runtime validation, original
translation-unit ownership, or an executable product build.

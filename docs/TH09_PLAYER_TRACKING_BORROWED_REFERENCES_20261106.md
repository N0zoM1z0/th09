# TH09 Player shot callbacks: one borrowed tracking state (2026-11-06)

The original Japanese TH09 v1.50a Player object stores the tracked nearest
Enemy position and the homing Enemy pointer inside its existing opaque
`+0x30364` region. The shared `PlayerEnemyTrackingStateView` already pins
its actual three-float position at `+0x00` and homing pointer at `+0x18`
(absolute `+0x3037C`). The **exact** Player shot callbacks use the same
record; they do not construct another `Float3` object on top of Player's
lifecycle allocation.

The maintained code previously repeated a
`reinterpret_cast<PlayerEnemyTrackingStateView *>(player->unknown30364)`
in every branch and component access. It now takes **one ordinary borrowed
C++ reference** in each relevant callback, then reads its named native
position fields:

- `PlayerShotUpdateCallbackType1` uses a *const* borrowed reference after the
  original timer/active checks, before deciding whether the target is present.
- `PlayerShotUpdateCallbackType3` uses a *const* borrowed reference for the
  original target position and position-delta calculations.
- `PlayerShotSpawnCallbackType7` uses a borrowed mutable reference; its
  original exact angle callee accepts the pointer to that same tracked
  vector without `const` qualification.

The original call order, target thresholds (-900/-144), positioning math,
shot speed and original `AngleFromPlayerToPoint` owner are unchanged. No
new tracking allocation, fake helper call, nontrivial embedded constructor,
compiler switch, volatile, forced inline or register hint was introduced.

## Bounded natural source and exactness probes

The fixed original-bound VC7.1 builds verified four natural local pointer/
reference forms of the two update callbacks and the mutable pointer/reference
forms of the spawn callback. The adopted forms reproduce each **complete
original byte and relocation identity**:

| Current exact original owner | Complete physical bytes | Result |
| --- | ---: | --- |
| `player-shot-update-callback-type1` | 471 | Exact |
| `player-shot-update-callback-type3` | 248 | Exact |
| `player-shot-spawn-callback-type7` | 139 | Exact |
| **Total** | **858** | **All exact** |

The competing common `inline` accessor was compiled too, but changed all
three leaf extents and was **not adopted**. A local reference in the exact
184-byte `EnemyView::CleanupAfterDeactivation` adds four bytes; that function
retains its previous repeated-access source. This is a measurement about the
fixed compiler/code shape, not an assertion that the other natural models are
impossible or that any nonexact boss has become exact.

Reproduction uses only the three affected complete match units:

```bash
for unit in player-shot-update-callback-type1 \
  player-shot-update-callback-type3 player-shot-spawn-callback-type7; do
  python3 -B scripts/build-match-unit.py --unit "$unit"
  python3 -B scripts/compare-coff-function.py --unit "$unit"
done
```

Original target SHA-256 is
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Private failed-model/current-source results are retained in the ignored
`.analysis/gpt-web-player-tracking-20261106/`. Existing exact-function
count remains 1,021; source progress does not imply native product closure,
Factory acceptance or remote publication.

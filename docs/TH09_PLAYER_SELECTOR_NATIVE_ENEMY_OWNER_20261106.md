# Player selector native Enemy owner, 2026-11-06

The maintained Player selector at **0x004049A0** now uses the same typed
`EnemyManagerView` and `EnemyView` allocations already present in the Enemy
update/ECL sources. It no longer fabricates raw `void* + 0x2AC3B8`,
`+ 0x2AC444`, `+ 0x2AC448`, `+ 0x2AC3AC`, or `enemy + 0x2D74/0x3380`
member reads. The Player's `targetOverride30F64` also uses the existing native
`PlayerLifecycleView` member. Native layout assertions check each source
member's original i386 offset and the copied XYZ three-float window.

The source change preserves the original selection order, repeated Enemy
state reads, fallback target Y coordinate, and event frequency choice. It
does **not** claim the original C++ nominal class/TU identity, runtime
product closure, or new function-count credit: both affected functions were
already exact. This is natural source/layout reconstruction, not a register
hint, binary patch, padding, or fake replacement function.

## Target-bound focused Oracle

The canonical Japanese 1.50a TH09 PE SHA-256 is
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Pinned VC7.1 `/O2 /Ob0` compiles the entire 3,238-byte
`PlayerUpdateSelectorState` body and reproduces the 136 previously reviewed
original operand fields. The related `/O2 /Ob1` source TU reproduces the
346-byte `PlayerGameplayMethods::ResolvePatternOffset` and its complete
412-byte physical span, including its two eight-entry switch tables and
18 original-bound relocation fields. Neither comparison skips changed bytes.

The added native header initially renumbered some private compiler labels.
Moving that header after the resolver definition preserves its 16 original
internal switch labels. Two remaining outer table labels are now `$L5015`
and `$L5016`, at the **same** fields `+0x31`, `+0xC1`, targeting original
`0x0040486C`, `0x0040488C`. Every one of the 18 actual COFF label
coordinates was checked independently of the original field value, rather
than fitting a target address to a guessed label. No addresses, addends,
field types, offsets, body bytes or table bytes were changed in the match unit.

Local reproducer (affected units only):

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/build-match-unit.py --unit player-update-selector-state
python3 -B scripts/compare-coff-function.py --unit player-update-selector-state
python3 -B scripts/build-match-unit.py --unit player-resolve-pattern-offset
python3 -B scripts/compare-coff-function.py --unit player-resolve-pattern-offset
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
```

The source and generated COFF evidence is local. The `gpt-web:` checkpoint is
a source/recipe checkpoint, not independent Factory or remote acceptance.

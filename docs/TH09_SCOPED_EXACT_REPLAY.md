# TH09 bounded canonical exact regression replay

This workflow does **not** run the all-owner cold replay. It expands an
explicit canonical `--unit` or source path into only the **exact** functions in
that one source, including all distinct compiler profiles and physical COFF
carriers. It compiles each selected carrier **once**, with the pinned VC7.1
source/profile recorded in `config/match-units.toml`, then strictly checks every
matched owner against the original Japanese 1.50a PE, its complete physical
code/tail/table and every reviewed relocation. The target and tracking ledgers
are reverified before replay. A nonexact boss is **not** silently accepted.

```sh
# No compiler or original executable required for planning.
python3 scripts/replay-scoped-exact.py --unit effect-draw-callback-type34 --dry-run
# The 80 exact units associated with EclManager.cpp span two distinct objects.
python3 scripts/replay-scoped-exact.py --source src/EclManager.cpp
# The 12 exact ReplayInput.cpp units span /EHsc and no-EH profiles.
python3 scripts/replay-scoped-exact.py --source src/ReplayInput.cpp
# The 15 exact EffectManager.cpp units use one physical carrier.
python3 scripts/replay-scoped-exact.py --source src/EffectManager.cpp
```

Specify multiple `--source` or `--unit` arguments only when the edit affects
those source files. For `.inl` and `.hpp` edits, `--changed-file` follows **actual
quoted local `#include` edges** (including nested include chains) to find the
involved `.cpp` sources, then expands all canonical exact carrier profiles:

```sh
# The ECL state fragment affects four source files across five actual profiles.
python3 scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl --dry-run
python3 scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl
# A narrower Effect34 lexical fragment affects only EclManager.cpp.
python3 scripts/replay-scoped-exact.py --changed-file src/EffectType34Family.inl
```

This is a **conservative textual** dependency traversal, not an authenticated
preprocessor/build graph. If the project uses macro, generated, or external
includes, supply those affected `.cpp` files with `--source` explicitly. If
a changed file also affects `.cpp` files with no canonical exact match, the
tool refuses the ambiguous selection rather than silently declaring success.
Only true **exact** witnesses are replayed; every nonexact owner still needs its
own original-bound boss inspector. A default cap of **8** physical compiler
jobs prevents a broad header from silently launching an enormous cold sweep;
`--max-carriers N` requires an explicit operator override (maximum 64).
It intentionally provides no `--all` and refuses a missing selection. It is not a nonexact-boss inspector:
for a changed ECL, Enemy, Title, or Effect boss, also run its separate
`check-boss-frontiers.py --owner NAME --skip-exact-siblings` or other complete
original-bound diagnostic. Do not count equal fragments or “near” percentages as
exact. A canonical manifest row by itself does not establish the full Windows
product link/runtime or persistent Factory registry acceptance.

Fresh 2026-10-31 demonstration (same target and original verifier): selecting
`src/EffectManager.cpp` compiled **one** carrier and verified **15 exact units,
2,530 complete physical bytes and 100 original-linked relocation fields**.
The independent focused EffectManager::AddedCallback remains nonexact at
51/51 physical bytes, four fields and eight differing bytes. Dry-run selection
for ECL found **two** carrier profiles/80 exact units; ReplayInput found **two**
carrier profiles/12 exact units. No full cold replay was run. All five new
no-target isolation/fail-closed unit tests and full public CI (**1085 tests,
29 optional skips**) passed. Subsequent full original-bound
`--changed-file src/EclRunState.inl` replay verified **five** compiler carriers,
**84 exact owners, 26,888 physical bytes and 1,199 original-bound relocation
fields** (EclHelpers 2, EclInitializeSubroutine 1, EclManager /Ob0 4,
EclManager /Ob1 76, EnemyManagerClear 1). This counts **existing** exact
functions, not 84 newly solved functions; RunEcl and EnemyManager::OnUpdate
remain nonexact as separately verified.

### Current strict include-scoped smoke (2026-10-31)

A new independent current-worktree replay of
`python3 scripts/replay-scoped-exact.py --changed-file src/EffectType34Family.inl`
confirmed exactly **2** affected physical carrier compiles, **80** canonical
exact original-PE/VC7.1 owners, **26,161** complete physical bytes, and
**1,179** relocations. The 80 existing exact units are not new match credit.
The original `EffectDrawCallbackType34` is included by the actual ECL carrier
and retains its separate strict original-bound comparator proof. Receipt:
`.analysis/gpt-web-changed-file-effect34-replay-20261031.json`. No all-source,
whole-image, product/link/runtime, or semantic closure was run or inferred.

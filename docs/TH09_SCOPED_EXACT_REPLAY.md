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
those source files. For an `.inl` or header change, **you must identify its
actual including translation units yourself**: this tool is not an automatically
inferred dependency/translation-unit graph. It intentionally provides no
`--all` and refuses a missing selection. It is not a nonexact-boss inspector:
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
29 optional skips**) passed.

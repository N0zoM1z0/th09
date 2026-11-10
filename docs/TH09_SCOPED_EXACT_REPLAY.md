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
# Expand only the selected source and its configured profiles.
python3 scripts/replay-scoped-exact.py --source src/EclManager.cpp
# ReplayInput.cpp uses both EH and no-EH profiles.
python3 scripts/replay-scoped-exact.py --source src/ReplayInput.cpp
# Select another source when it is affected.
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

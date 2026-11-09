# 東方花映塚 ～ Phantasmagoria of Flower View

Agent workflow: follow the highest-priority requirements in [AGENTS.md](AGENTS.md). Use affected exact checks and searchable Markdown knowledge; ordinary notes do not require full CI or replay.

<p align="center">
  <img
    src="resources/title-screen.png"
    width="640"
    alt="Original Japanese TH09 1.50a title screen">
</p>

<p align="center">
  <img src="resources/progress.svg" alt="TH09 reconstruction progress">
</p>

This project reconstructs the original Japanese TH09 version 1.50a
executable. It starts from an independently versioned, hash-attested target and
tracks boundary, origin, source presence, codegen exactness, native-product
closure, runtime semantics, and portability as separate facts.

## Exact target

Place your own legal copy at the ignored canonical path
`resources/th09.exe`. Repository tools use that path by default. An explicit
path or `TH09_TARGET_PATH` is supported only for deliberate local overrides;
normal reconstruction work uses the canonical path.

| Property | Required value |
| --- | --- |
| Version | original Japanese 1.50a |
| Size | `685,056` bytes |
| SHA-256 | `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822` |
| MD5 | `cf634df46e05552e104fa97a971aaac0` |
| Image base | `0x00400000` |
| Entry point | `0x0047D45F` |

The pinned [thcrap version database](https://github.com/thpatch/thcrap-tsa/blob/f08b582ce57bce800955dd371fc9a68dbad5b324/base_tsa/versions.js)
classifies this exact SHA-256 and size as `th09`, `v1.50a`, `(original)` and
lists the English-patched executable under a different digest. The
[official 1.50a release post](https://kourindou.exblog.jp/1327589/) is dated
2005-10-08 JST, consistent with the target's PE timestamp.

```bash
python3 scripts/verify-target.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
```

Analysis uses direct local Ghidra through `scripts/ghidra-analysis.py`.
The runner verifies disk/Program identity, i386 language, entry and every
file-backed mapped PE byte before analysis or export, then reuses the saved
project. See [`AGENTS.md`](AGENTS.md) and [`docs/TOOLS.md`](docs/TOOLS.md).

Original executables, game data, analysis databases, decompiler output, and
downloaded tools are private and never committed.

## Current status

At the November 9 handoff, the canonical ledger still records **2,248
candidates, 1,037 source-present authored mappings, 1,024 complete exact
functions and 13 nonexact functions** (29,834 physical original bytes).
Whole-original-byte exactness is **262,097 / 291,931 = 89.7805%**; the
98% authored-byte milestone needs another **23,996 fully exact bytes** at
the current denominator. The inventory is not proven complete and the
game is not a linked, runtime-tested native Windows product.

Start with the [November 9 next-agent guide](docs/TH09_NEXT_AGENT_HANDOFF_20261109.md)
and [live frontier/ledger](docs/RE_HANDOFF.md). These are current routing
references; the [November 8 guide](docs/TH09_NEXT_AGENT_HANDOFF_20261108.md),
[November 7 guide](docs/TH09_NEXT_AGENT_HANDOFF_20261107.md)
and [November 8 history snapshot](docs/RE_HANDOFF_HISTORY_20261108.md)
contain historical source experiments, not the current HEAD.

Additional exact-byte Enemy/ECL constructor-shaped members were recovered
on November 9 (28, 15, 15, 47 and 207 bytes). Those five remain
origin-unclassified, so no authored exact ledger credit is added.
The three native Player constructors previously recovered (28, 23, 12 bytes)
remain tracked exact. The 309-byte `PlayerLifecycleView::InitializeShot` now belongs
to the real Player and all seven exact shot callers retain its original
`0x41F350` target. See the next-agent guide for current big-boss residuals.

Only rebuild and compare affected exact owners and their dependencies;
never do routine full cold replay. Use `gpt-web: <English summary>` for
coherent local commits and preserve dirty/untracked work rather than
silently discarding it.

The PE and Rich header identify Microsoft Visual C++ .NET 2003 build 3077.
Some runtime-library contributions and compiler-generated helpers are proven,
but the complete translation-unit graph, resources, data owners, compiler
profiles, and link order remain open. `config/build.toml` records that state
instead of pretending the project is buildable.

Run `python3 scripts/progress.py` to regenerate the status card,
`python3 scripts/validate-docs.py` to catch stale handoff totals, percentage,
98% gap, non-exact routing and accepted-owner manifest/receipt entries, and
`python3 scripts/ci.py` for the full target-independent validation set when
changing shared validation/CI tooling. Ordinary exact checkpoints use affected
complete-owner comparisons and checks relevant to the diff; see
the validation scope in `docs/RE_WORKFLOW.md`.
Generated compiler caches can reappear after focused builds. Preview cleanup
with `python3 scripts/clean-generated-artifacts.py`; its default policy removes
only ignored `build/` outputs and disposable debug/bytecode caches under
`.analysis/`. Analysis `.obj` files are preserved unless a separate reviewed
path/SHA-256 list explicitly authorizes their removal. Do not delete `.analysis/`
wholesale: it also contains original-bound evidence, saved Ghidra projects and
lossless historical archives. Use `scripts/restore-analysis-archive.py` for
selective recovery. The dated cleanup history records exact scopes, hashes and
restore instructions in `docs/RE_ARTIFACT_CLEANUP_HISTORY.md`.

## Required phase order

1. Preserve the reviewed boundary/origin inventory and recover remaining exact
   VC7.1 authored units.
2. Close and exercise the faithful Windows i386 product, including static data
   ownership, initializers, resources, libraries, and runtime scenarios.
3. Perform semantic reconstruction with two independent runtime Oracles: the
   original target and the reconstructed native i386 product.
4. Start a portability branch only after the native semantic baseline is
   trustworthy.

Exact functions do not imply a complete or runnable product. A modern port can
hide missing original data owners, ABI behavior, and initialization, so it is
not an acceptable shortcut around the native-product stage.

## Project map

- [`AGENTS.md`](AGENTS.md) — mandatory session, evidence, and checkpoint rules.
- [`config/target.toml`](config/target.toml) — exact target and observed PE/toolchain facts.
- [`config/build.toml`](config/build.toml) — whole-product graph and explicit unknowns.
- [`docs/RE_WORKFLOW.md`](docs/RE_WORKFLOW.md) — bounded reconstruction loop and stage gates.
- [`docs/ORACLES.md`](docs/ORACLES.md) — what can falsify or accept each claim.
- [`docs/TH09_NEXT_AGENT_HANDOFF_20261109.md`](docs/TH09_NEXT_AGENT_HANDOFF_20261109.md) — **current** next-agent restart, corrected source facts, 13 boss frontiers and verified lossless archive cleanup.
- [`docs/TH09_NEXT_AGENT_HANDOFF_20261108.md`](docs/TH09_NEXT_AGENT_HANDOFF_20261108.md) — historical November 8 source and storage snapshot.
- [`docs/TH09_NEXT_AGENT_HANDOFF_20261107.md`](docs/TH09_NEXT_AGENT_HANDOFF_20261107.md) — historical November 7 guide, not the current worktree.
- [`docs/TH09_NEXT_AGENT_HANDOFF_20261106.md`](docs/TH09_NEXT_AGENT_HANDOFF_20261106.md) — historical November 6 cleanup snapshot, not the current HEAD.
- [`docs/RE_HANDOFF.md`](docs/RE_HANDOFF.md) — concise live ledger, all 13 nonexact frontiers and evidence routing.
- [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) — target facts and maintained source/verification map.
- [`docs/RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md`](docs/RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md) — frozen historical checkpoints; use the active handoff, not this archive, for current facts.
- [`docs/KNOWLEDGE_BASE.md`](docs/KNOWLEDGE_BASE.md) — durable TH09-only facts.
- [`docs/TOOLS.md`](docs/TOOLS.md) — direct Ghidra and pinned compiler routing.
- [`docs/PROGRESS.md`](docs/PROGRESS.md) — generated ledger totals.
- [`docs/SMALL_FUNCTION_FRONTIER.md`](docs/SMALL_FUNCTION_FRONTIER.md) — short open leaf owners.
- [`docs/RE_ARTIFACT_CLEANUP_HISTORY.md`](docs/RE_ARTIFACT_CLEANUP_HISTORY.md) — verified generated-cache deletion audits and preservation policy.
- [Touhou Reconstruction Factory](https://github.com/N0zoM1z0/touhou-reconstruction-factory) — shared ontology, workflow, and cross-game knowledge.

## Credits

Thanks to [Cirno-Labs/th09](https://github.com/Cirno-Labs/th09) for the reference
implementations adapted into fourteen exact functions here. File-level credits
and the pinned upstream revision are listed in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## License

Original project code and documentation retain their [MIT license](LICENSE).
Code adapted from Cirno-Labs/th09 is [GPL-3.0-only](LICENSES/GPL-3.0-only.txt),
so the combined reconstructed program is distributed under GPL-3.0-only.
These licenses do not cover the original game or its assets.

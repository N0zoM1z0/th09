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

Current ledger-derived counts and exact bytes are in
[docs/PROGRESS.md](docs/PROGRESS.md). Generate them with
`python3 -B scripts/progress.py`, or inspect the live report:

```bash
python3 -B scripts/report-reconstruction-status.py
```

[docs/RE_HANDOFF.md](docs/RE_HANDOFF.md) is the single restart guide: current
source work, all nonexact owners and focused comparison commands.
[docs/KNOWLEDGE_BASE.md](docs/KNOWLEDGE_BASE.md) keeps verified findings,
current blockers and useful failed approaches. The candidate inventory is not
proven complete; the native Windows product is not yet linked/runtime-validated.

Only rebuild and compare affected complete owners and their dependencies.
Use `gpt-web: <English summary>` for coherent local checkpoints and preserve
dirty/untracked operator work. `config/build.toml` records open whole-product
questions; exact functions do not establish that graph.

Retain the original executable, toolchain/game-specific Wine runtime, saved
Ghidra projects, maintained source/ledgers/replay scripts and concise knowledge.
Remove obsolete archives, duplicate evidence and old trial outputs without
creating another archive or cleanup report. A document reference alone is not
a reason to retain a file. `build/` may be absent; recreate only the selected
unit's object. A small experimental context is worth keeping only when an
unresolved question depends on it and it is difficult to rebuild.

Use `python3 -B scripts/validate-docs.py` for links and frontier consistency,
`python3 -B scripts/ci.py` for fast basic syntax/public-file, ledger,
link/progress and whitespace checks. This is also the GitHub Actions command.
Run affected tool tests separately; `python3 -B scripts/ci.py --full` adds the
optional complete unit suite. Routine commits do not require that full suite.

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

- [AGENTS.md](AGENTS.md) — session, evidence, retention and checkpoint rules.
- [config/target.toml](config/target.toml) — original target identity.
- [config/match-units.toml](config/match-units.toml) — rebuild/comparison recipes.
- [config/matches.csv](config/matches.csv) — canonical exact ledger.
- [config/build.toml](config/build.toml) — whole-product graph and unknowns.
- [docs/RE_HANDOFF.md](docs/RE_HANDOFF.md) — current restart guide and frontiers.
- [docs/KNOWLEDGE_BASE.md](docs/KNOWLEDGE_BASE.md) — useful conclusions and failures.
- [docs/RE_WORKFLOW.md](docs/RE_WORKFLOW.md) and [docs/ORACLES.md](docs/ORACLES.md) — bounded loop and acceptance boundaries.
- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) — observed target and source map.
- [docs/TOOLS.md](docs/TOOLS.md) and [docs/SHARED_TOOLS.md](docs/SHARED_TOOLS.md) — compiler/Ghidra/runtime routing.
- [docs/PROGRESS.md](docs/PROGRESS.md) — generated ledger totals.
- [docs/TH09_SCOPED_EXACT_REPLAY.md](docs/TH09_SCOPED_EXACT_REPLAY.md) — affected-source replay selection.
- [Touhou Reconstruction Factory](https://github.com/N0zoM1z0/touhou-reconstruction-factory) — shared workflow and cross-game knowledge.

## Credits

Thanks to [Cirno-Labs/th09](https://github.com/Cirno-Labs/th09) for the reference
implementations adapted into fourteen exact functions here. File-level credits
and the pinned upstream revision are listed in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## License

Original project code and documentation retain their [MIT license](LICENSE).
Code adapted from Cirno-Labs/th09 is [GPL-3.0-only](LICENSES/GPL-3.0-only.txt),
so the combined reconstructed program is distributed under GPL-3.0-only.
These licenses do not cover the original game or its assets.

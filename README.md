# 東方花映塚 ～ Phantasmagoria of Flower View

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

The operator has resumed exact-source reconstruction after the November 6 cleanup.
Reconstruction remains unfinished. The canonical ledger reports **2,248
candidates, 1,034 authored mappings, 1,021 exact functions and 13 non-exact
functions /29,834 authored bytes**. Exact authored-byte coverage is
**262,034 /291,868 (89.7783%)**; the 98% milestone still needs23,997 exact
authored bytes at the present denominator. Whole-product closure and runtime
validation remain open; semantic reconstruction and portability have not started.

Use [the November 6 restart guide](docs/TH09_NEXT_AGENT_HANDOFF_20261106.md)
and [the live handoff](docs/RE_HANDOFF.md), then check actual Git status/log and
ledgers. The latter lists all13 nonexact owners and current source-bound
receipts. RunEcl remains30 unlike bytes, Enemy update13, Player movement122,
SaveReplay two; Setup is1682/1689 bytes with694 unequal overlaps plus seven
missing. Historical handoff prose was moved to
[the dated archive](docs/RE_HANDOFF_HISTORY_20261106.md). The latest interrupted
RunEcl prototype experiment was recovered as unaccepted evidence; no gameplay
source or exactness credit changed during cleanup.

Use focused complete-owner builds/replays for the changed units only. Do not
run a full cold replay for routine iteration. Commits use
`gpt-web: <concise English summary>` after each substantive checkpoint; keep
code and documentation commits coherent and make multiple checkpoints when a
session produces multiple reviewable advances.

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
Canonical build caches may be absent after reviewed cleanup; replay a function
by running `python3 scripts/build-match-unit.py --unit NAME` before its
corresponding comparison or non-crediting inspector. Preview disposable-cache
cleanup with `python3 scripts/clean-generated-artifacts.py`. Analysis objects
are preserved by default; `--reviewed-objects` accepts an explicit path/SHA-256
list for obsolete objects. Changed files invalidate the reviewed plan.
Git-backed old source/config/doc copies can be restored with
`scripts/restore-analysis-snapshots.py`; the handoff names the recovery
manifest. The 2026-11-03 handoff includes a **locally retained, verified lossless
archive of unreferenced historical experiments** and reviewed `build/`
cache cleanup; about **590.5 MiB of net logical file bytes** were reclaimed.
See `docs/RE_ARTIFACT_CLEANUP_HISTORY.md` for archive SHA-256, protected
scopes and selective restoration. The archive is ignored and not cloned.

The November6 cleanup reclaimed approximately **402.85 MiB** more logical file
bytes. Build objects and selected large JSON/TOML evidence are losslessly saved
in a SHA-verified local archive; `build/` is absent. Use the current handoff for
`restore-analysis-archive.py --bundle ... --prefix ...` recovery commands and
restore archived inputs before running a dated private verifier.

Use `docs/RE_ARTIFACT_CLEANUP_HISTORY.md` for the actual cleaned
scopes and retained evidence.

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
- [`docs/TH09_NEXT_AGENT_HANDOFF_20261106.md`](docs/TH09_NEXT_AGENT_HANDOFF_20261106.md) — current restart scope, verified owner boundaries, and cleanup guidance.
- [`docs/RE_HANDOFF.md`](docs/RE_HANDOFF.md) — concise live ledger, all13 nonexact frontiers and evidence routing.
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

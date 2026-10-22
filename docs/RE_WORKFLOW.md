# Reconstruction workflow

The project milestone is at least **98% of reviewed authored bytes exact**,
using pinned VC7.1 Oracles and the verified Japanese TH09 1.50a executable.
The operator explicitly resumed reconstruction through Factory MCP on
October 22, 2026. Prioritize key large functions and test fresh source/type/
lifetime hypotheses against TH09 evidence; old notes are not conclusive.
The RunEcl, EnemyManager::OnUpdate, Title DrawResult and ExAttack Type3 fronts
and the milestone remain unfinished. Compiles stay serial and at lowered
priority; only affected proofs need replay. Clean only reviewed, reproducible
artifacts after their producers terminate.
See [the current handoff](RE_HANDOFF.md) and refresh the ledgers for live totals;
dated experiments and prior stop/resume records are historical.
Exact code, the reviewed-authored denominator, the native Windows product and
later runtime/port phases are independent gates.

## Session entry

First recover the current worktree instead of assuming it is clean. Read the
handoff, inspect status and diffs (including untracked work), then attest the
disk target, IDA target, ledgers, and live status using the commands in
`AGENTS.md`. Stop if any identity differs.

## Bounded exact-reconstruction loop

1. Select one candidate from the ledger; never select only by an attractive
   decompiler name.
2. Reconcile its entry, exits, tails, tables, padding, references, and physical
   ownership against target bytes.
3. Classify origin independently: authored game code, compiler-generated,
   library, import thunk, data, padding, or still unknown.
4. For authored code, recover ABI, types, object layout, globals, constants,
   side effects, and error behavior using bounded IDA queries and raw target
   evidence. Keep hypotheses explicitly falsifiable. When useful, inspect the
   read-only TH08 reconstruction at
   `/home/pentester/coding/codex_ida/th08-reconstruction/th08` or TH095 at
   `/home/pentester/coding/codex_ida/th095-reconstruction/th095` for an
   analogous implementation, naming pattern, or compiler experiment. TH095's
   ongoing semantic work is provisional. Treat everything learned from either
   repository as a candidate hypothesis until independent TH09 evidence
   confirms it.
5. Place natural C/C++ in a candidate translation unit. Mark source presence
   without claiming exactness.
6. Compile the smallest useful VC7.1 probe with an explicit candidate profile.
   Compare the complete owned extent and every relocation; distinguish source,
   profile, boundary, ownership, and library mismatches.
7. Add a canonical match unit and exact ledger row only after repeatable zero
   difference against the verified TH09 target. Similar adjacent-game source,
   even when it compiles plausibly, never substitutes for this comparison.
8. Replay changed functions only, regenerate progress, update the handoff and
   per-game knowledge, inspect the final diff, and commit one coherent local
   checkpoint. Reuse hash-matched evidence for unchanged functions; do not run
   the whole historical cohort or repeat cold builds after every edit. Each new
   exact owner still needs complete byte/relocation/source proof and a
   reproducible canonical local build/replay. Historical Factory receipts remain
   scoped historical evidence. The resumed Factory/Web campaign uses the
   attested `th09-ida` provider and target-bound pinned compiler Oracles. This
   focused replay scope supersedes older broad replay wording.

### What to read at a fresh handoff

Use [RE_HANDOFF.md](RE_HANDOFF.md) for the **current** ledger, unresolved
large/small function fronts, exact-sibling scopes, and restart/cleanup gates.
The separately preserved [accepted Factory receipts](RE_ACCEPTED_RECEIPTS.md)
are a **historical source-commit-bound archive**, not a new acceptance claim
for the current HEAD; [artifact cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md)
records what was actually deleted and what remained untouched. The current
standalone 1,280-byte Title DrawReplayMenu PE/COFF auditor is
`scripts/inspect-title-replay-menu-draw.py`; the focused
`python3 scripts/check-boss-frontiers.py --owner title` command now
cold-compiles and verifies it automatically, in addition to the other
original-bound Title owners.

### Public CI

`python3 scripts/ci.py` runs target-independent tests with Python `-S`, so a
prepared local machine cannot hide undeclared third-party import dependencies.
Optional decoders belong inside private diagnostic entrypoints. This local gate
uses the same command as GitHub Actions; a local pass does not prove a remote
workflow run. **Commit verified local checkpoints** using the current operator
prefix and identity in `AGENTS.md`. This Factory campaign has no network or
Git push. A local commit is never evidence of a successful remote push.

### Current-state documentation hygiene

`docs/RE_HANDOFF.md` is replace-in-place current state, not an append-only packet
journal. Keep only the live ledger snapshot, active frontiers, restart commands,
and short routing notes there. Historical experiments and superseded byte-count
plateaus belong in Git history or the historical sections of
`docs/KNOWLEDGE_BASE.md`.

Do not use a knowledge-base `Recorded state` cell or old packet wording as a
current exactness ledger. When a function changes exactness, update or remove
any active handoff or routing claim in the same checkpoint. Prefer a
reproducible command over copying transient whole-owner counts into long-lived
prose.

## Product and semantic gates

Function exactness is followed by a separate faithful-product phase. Populate
`config/build.toml` as evidence establishes translation units, compiler
profiles, libraries, resources, initializers, data owners, and link order. A
successful link is necessary but insufficient: exercise representative
Windows i386 runtime scenarios and audit production data ownership.

Only after that native product is trustworthy should semantic reconstruction
begin. Each semantic change then has two runtime Oracles:

1. the original hash-attested TH09 target;
2. the reconstructed Windows i386 product built from the exact baseline.

Agreement between both substantially reduces false positives that a single
runtime or a premature modern port can hide. Portability work starts after this
dual-Oracle semantic baseline, in a distinct product configuration.

## Artifact lifecycle

`.analysis/` is non-authoritative working state, not a journal. Keep only compact
manifests, commands, disassembly snippets, and comparison outputs that are still
the sole useful record of unresolved evidence. Once a result is accepted, move
the durable fact to tracked source/ledgers/docs and delete its probe copy,
compiler object, PDB, duplicate header, decompilation dump, and stale log.

Clear generated object/PDB/bytecode caches routinely, but distinguish them
from unique, still-reproducible experimental C++ sources, analysis tables and
JSON audit evidence. A source-copy tree is **not automatically safe to delete**
merely because its files look generated: first verify how to reproduce the
original trial and whether its negative result was recorded. The handoff must
state exactly what was removed and what large scratch archives remain.
`python3 scripts/clean-generated-artifacts.py` is dry-run by default and
cleans the ignored `build/` tree plus compiler binaries under `.analysis/`
only when called with `--apply`. Ambiguous `.idb` files are preserved because
they may be IDA databases. It intentionally leaves copied experiment sources,
JSON evidence, and original/IDA inputs intact until separately audited. Classify deletion by reproducibility, tracked
references, and whether the useful fact has been promoted. Do not blindly
delete a recovered scratch tree before that audit. Once the remaining scratch
has no tracked consumers and every still-useful unresolved fact is recorded in
tracked source/ledgers/docs, remove only individually reviewed scratch paths.
Never bulk-delete `.analysis/` or remove unclassified legacy artifacts, provider
databases, toolchains, or Wine prefixes. Record the paths, classification,
reproducibility and retained evidence in `docs/RE_HANDOFF.md` so future agents
can distinguish deliberately cleaned outputs from missing canonical evidence.

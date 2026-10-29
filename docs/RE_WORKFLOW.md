# Reconstruction workflow

The project milestone is at least **98% of reviewed authored bytes exact**,
using pinned VC7.1 Oracles and the verified Japanese TH09 1.50a executable.
The operator explicitly resumed reconstruction after the October30 cleanup.
Audit earlier dependency/interface details as well as big-function bodies. The
October29 session explicitly
selected direct local Ghidra, without IDA, REA or Factory MCP. Use repository
Bash, `scripts/ghidra-analysis.py` and pinned VC7.1 tools.
New English commits use `gpt-6.1-sol:` and the AGENTS.md identity/trailer. Compile
serially at lowered priority and preserve complete affected-owner/callee proofs.
See RE_HANDOFF.md for current source map, preserved archives and baseline.

Historical negative results are bounded measurements, not proof that all natural
source models are exhausted. Exact code, the authored denominator, the Windows
i386 product and later runtime/port phases remain independent gates.

## Session entry

First recover the current worktree instead of assuming it is clean. Read the
current handoff checkpoint and inspect status and diffs, including untracked
work. Before target-dependent work, verify the disk target and affected ledgers;
bind Ghidra to that original before using its results. The direct runner checks
the Program identity and every file-backed mapped PE byte before analysis and
each export. Documentation/test-only changes need no compiler or Ghidra replay.
See `AGENTS.md` for commands and target identity.

Run `python3 -B scripts/ghidra-analysis.py --function 0x004086c0` for a bounded
export. The first invocation imports the original into an ignored local project;
subsequent invocations use `-noanalysis -readOnly` after rechecking the target
and every file-backed memory range. Supply `--ghidra-dir` or the ignored
`.tools/ghidra` symlink to an existing installation. Outputs and the reusable
project stay below `.analysis/ghidra-direct/`; no MCP service is needed.

## Bounded exact-reconstruction loop

1. Select one candidate from the ledger; never select only by an attractive
   decompiler name.
2. Reconcile its entry, exits, tails, tables, padding, references, and physical
   ownership against target bytes.
3. Classify origin independently: authored game code, compiler-generated,
   library, import thunk, data, padding, or still unknown.
4. For authored code, recover ABI, types, object layout, globals, constants,
   side effects, and original branches using bounded Ghidra queries and raw target
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
   scoped historical evidence. The resumed local campaign uses direct local
   Ghidra with mapped-byte attestation and target-bound pinned compiler Oracles. This
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
Optional decoders belong inside private diagnostic entrypoints. Run full CI for
shared validator/CI changes; ordinary exact checkpoints need only affected-owner
replay and checks relevant to the diff. The command is also used by GitHub
Actions. Commit using the current operator prefix and identity; report local
checks separately from remote publication.

### Checks during the exact phase

The acceptance question is complete original-bound byte and relocation equality
under the pinned compiler. Preserve affected exact siblings/callees and reuse
unchanged source/profile-bound proofs. Tests for wrong targets, missing extents,
incorrect relocations and stale evidence support that question.

Do not add routine allocation-failure injection, null-pointer matrices, malformed
game-input campaigns, fuzzing or general runtime hardening. Original error
branches still belong in the source. Additional behavioral tests should answer
a concrete reconstruction question or an explicit operator request. Avoid
source-text snapshots that require a particular spelling, include count or
equivalent C++ expression; let the compiler Oracle judge the result.

Select checkpoint checks from the diff:

- Reconstruction: focused complete-owner Oracle and affected exact siblings.
- Ledgers or status claims: tracking, progress and documentation validators.
- Tooling/tests: affected unit tests; full public CI for shared validation/CI.
- Every checkpoint: review the diff and run `git diff --check`.

Whole-product build/runtime, dual-Oracle semantic and portability checks belong
to their later phases. No unrelated test suite is required for each exact edit.
This scope supersedes historical full-check lists in handoff/experiment notes.
Use `python3 scripts/build.py --check` when working on the product build graph;
it is no longer part of exact-phase public CI.

### Current-state documentation hygiene

Keep the current handoff concise and sufficient to resume: current state, changed
claims, unresolved work and useful commands. Put lengthy experiment details in
Git history or `docs/KNOWLEDGE_BASE.md`; headings, dates and line counts are not
validation gates.

Do not use a knowledge-base `Recorded state` cell or old packet wording as a
current exactness ledger. When a function changes exactness, update or remove
any active handoff or routing claim in the same checkpoint. Prefer a
reproducible command over copying transient whole-owner counts into long-lived
prose.

## Product and semantic gates

These are later phases, not extra gates on current exact-source checkpoints.
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

`.analysis/` contains local working state and retained evidence. Promote durable
facts into tracked source/ledgers/docs, and keep the smallest source-bound packet
needed to reproduce an unresolved or accepted claim. Verified archives may
replace loose duplicates. Remove compiler objects, PDBs and copied inputs only
after checking that the retained recipe and evidence reproduce their useful
claims; the adopted selected-ECL-manager objects are retained proof, not caches.

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

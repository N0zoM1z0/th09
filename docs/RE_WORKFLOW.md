# Reconstruction workflow

> Current workflow: follow the working rules in [AGENTS.md](../AGENTS.md).
> Prefer affected exact comparisons and useful Markdown knowledge. Ordinary
> notes do not require full CI, native replay or document-hash/pin refreshes.
> Older broad validation instructions apply only when their impact is relevant.


The **future milestone** is at least **98% of reviewed authored bytes
exact**. It is **not the current state**: the 2026-11-13 canonical ledger has
**263,786 / 291,931 authored bytes exact (90.3590%)**, although 1,025 / 1,037
source-present function owners are exact by *count*. Use
[`PROGRESS.md`](PROGRESS.md) or the live status script for later changes.
Both measures are distinct from native Windows build/runtime completion.
Use pinned VC7.1 Oracles and the verified Japanese TH09 1.50a executable.
For the next reconstruction session, work seriously
on complete large bosses as well as leaf functions. Re-investigate old claims
against current source/target evidence. Use focused complete-owner builds and
replays only; no full cold replay for routine iteration.

Use the live registered repository worktree for source and Git operations,
`scripts/ghidra-analysis.py` for target-attested local Ghidra analysis, and the
pinned VC7.1 compiler for byte-exact comparisons. These are separate evidence
sources. New English commits use `gpt-web: <concise summary>`; checkpoint every
substantive advance and create multiple commits when a session has multiple
reviewable advances. Compile serially at lowered priority and preserve complete
affected-owner/callee proofs. See [the current restart guide](RE_HANDOFF.md)
for current source boundaries/residuals and useful commands. Generate totals
from the canonical ledgers.

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
   reproducible canonical local build/replay. Use direct local Ghidra with
   mapped-byte attestation and target-bound pinned compiler Oracles.

### What to read at a fresh handoff

Use [RE_HANDOFF.md](RE_HANDOFF.md) for the current owner frontiers,
[PROGRESS.md](PROGRESS.md) for generated totals and
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) for findings relevant to the chosen owner.

### Public CI

`python3 -B scripts/ci.py` is the normal local and GitHub Actions check.
It parses public Python/JSON/TOML/shell inputs without executing them, rejects
private binaries, and checks ledgers, links/frontiers, generated progress and
whitespace. It requires neither the original game nor the compiler and writes
no build cache.

Run tests relevant to a tool change separately. `python3 -B scripts/ci.py --full`
adds the optional complete target-independent unit suite using Python `-S`.
It is not a per-commit requirement, including for CI/tooling changes. Run it
only when explicitly requested or a concrete impact cannot be checked narrowly.
Source edits still need their affected complete original-bound comparisons.

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
- Tooling/tests: basic CI and affected unit tests. Full suite is optional.
- Every checkpoint: review the diff and run `git diff --check`.

Whole-product build/runtime, dual-Oracle semantic and portability checks belong
to their later phases. No unrelated test suite is required for each exact edit.
This scope supersedes historical full-check lists in handoff/experiment notes.
Use `python3 scripts/build.py --check` when working on the product build graph;
it is no longer part of exact-phase public CI.

### Current-state documentation hygiene

Keep the current handoff concise and sufficient to resume: current state, changed
claims, unresolved work and useful commands. Keep findings and useful failed
approaches in `docs/KNOWLEDGE_BASE.md`; Git retains the source history.

Do not use old knowledge wording as a current exactness ledger. When a function changes exactness, update or remove
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

Retain necessary private inputs, maintained source/ledgers/replay scripts, and
short useful knowledge. The original executable, toolchain/Wine runtime and
saved Ghidra projects are inputs. `build/` and analysis exports, objects and
old trials are disposable outputs. A document reference does not justify
retention. Do not archive obsolete outputs or produce cleanup audit journals.

If an unresolved question really needs a compiler context that cannot readily
be rebuilt, keep that small context and explain the dependency in the handoff.
Otherwise record the conclusion and useful failed approach in the knowledge
base, then remove the scratch. Preserve dirty/untracked operator work and
private inputs when cleaning. Exactness comes from complete reproducible
comparison, not from retaining historical receipts.

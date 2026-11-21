# Reconstruction workflow

> Current workflow: follow the working rules in [AGENTS.md](../AGENTS.md).
> Prefer affected exact comparisons and useful Markdown knowledge. Ordinary
> notes do not require full CI, native replay or document-hash/pin refreshes.
> Older broad validation instructions apply only when their impact is relevant.


The **future milestone** is at least **98% of reviewed authored bytes
exact**. It has **not** been reached. Consult generated
[`PROGRESS.md`](PROGRESS.md) or
`scripts/report-reconstruction-status.py` for the current authored-byte
denominator, exact count and remaining functions; copied dated percentages
are not authoritative. Authored-function/byte exactness does not establish
native Windows build or runtime completion.
Use pinned VC7.1 Oracles and the verified Japanese TH09 1.50a executable.
For the next reconstruction session, work seriously
on complete large bosses as well as leaf functions. Re-investigate old claims
against current source/target evidence. Use focused complete-owner builds and
replays only; no full cold replay for routine iteration.

Use the live registered repository worktree for source and Git operations,
`scripts/ghidra-analysis.py` for target-attested local Ghidra analysis, and the
pinned VC7.1 compiler for byte-exact comparisons. These are separate evidence
sources. Use the commit prefix in AGENTS.md or the one requested by the
operator. Checkpoint substantive advances in concise English commits.
Compile serially at lowered priority and preserve complete
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

### Trace a difference through its compiler context

1. Start with the complete maintained translation unit and its pinned
   profile. Align the original and rebuilt instruction streams by control
   flow and call destinations when an encoding changes length. Separate the
   first real divergence from the resulting shifts in branches, table
   positions and relocations. Keep the full physical comparison for acceptance.
2. Check the reference at every changed operand. Use the original PE to
   establish the destination, then inspect the actual COFF symbol, offset,
   type and addend. A reference to a global proves where the original reads
   or writes. Compare the defining object's section, neighboring data,
   pointer slots and actual literal bytes to establish the provider. Check
   relevant readers and writers as well. The same printed name or a zero
   masked byte count can hide a wrong definition. [Oracle policy](ORACLES.md)
3. Compare the full carrier with a controlled build that hides one genuine
   callee definition behind its declaration. Hold the caller, ABI and flags
   fixed. A retained call can still change alias analysis, register allocation
   or scheduling elsewhere in the function. When the residual is in a switch
   or shared tail, select another branch or handler to isolate in scratch and
   watch which earlier choices change. Use that information to recover the
   natural source and original live ranges.
4. Test isolated hypotheses against the full carrier again. If a one-line
   control recovers the register and stack-store order but changes an x87
   operand or moves a branch, use each result to constrain the next C++
   expression. Read-only C2 allocation and scheduling traces are useful once
   the candidate is small enough to name the competing live ranges or memory
   dependencies. Rebuild a comparable reference carrier at its recorded
   revision when it can distinguish a source hypothesis from a tool or
   comparison problem. Match routines by original address.
5. Replay the selected owner and every affected exact callee, sibling and
   consumer, including readers and writers of a moved definition. Keep the
   original call and data identities. For regenerated private labels, verify
   their same-section COFF destinations and original PE operands. Report
   compiler context demonstrated by the experiment and leave retail
   translation-unit ownership open unless independent evidence establishes it.

The following controls give concrete starting points.

- **RunEcl.** Varying only the two visibly wrong handlers kept the residual.
  Temporarily omitting individual neighboring handlers exposed which shared
  tails affected register choice. Restoring ordinary statements there let
  VC7.1 form the target merges from the source. [RunEcl controls](KNOWLEDGE_BASE.md#runecl-register-coupling-across-handlers)
- **Replay playback and Enemy update.** The exact seed setter body changed
  a memory dependency at a retained call and reordered loads and stores
  earlier in playback. Making the exact timer comparison visible changed
  an Enemy scratch-register choice more than 200 bytes before its call.
  The source recovery then reproduced the draw-list flow and descriptor
  stores. [Seed proof](KNOWLEDGE_BASE.md#2026-11-21-co-visible-replay-seed-and-exact-playback)
  [Enemy proof](KNOWLEDGE_BASE.md#2026-11-21-enemy-update-exact)
- **Replay menu.** The menu and saved USER record have separate caption
  arrays. Correcting the table definitions left all 29 instruction differences
  intact. The live-field control restored the register choices and stack
  stores, leaving one widened `fild` operand after alignment. Sharing a typed
  slot reference between the lookup and the coordinate recovered the full
  owner. [Caption and conversion proof](KNOWLEDGE_BASE.md#2026-11-21-title-replay-caption-tables)

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

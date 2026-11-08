# TH09 reconstruction agent rules

## Highest priority: lightweight exact reconstruction and useful knowledge

These are the user's current workflow requirements. They override older
instructions that mandate broad CI, repeated preflights, document/source-pin
refreshes, per-checkpoint audits or extra workflow machinery. Preserve the
actual exactness, target identity, natural-source and state-isolation rules.

- Prioritize exact reconstruction and reusable knowledge. Use the shortest
  useful loop: inspect relevant state, analyze/edit, compile and compare the
  affected scope, retain useful learning, and create a coherent checkpoint.
- Use existing repository scripts and composable Bash. Add only the smallest
  change needed for an observed problem. Do not introduce a framework, schema,
  registry, state machine, generalized validator, test family or reporting
  process merely to make a local reconstruction task look more engineered.
- Documentation, knowledge and handoff edits require no compiler, analyzer,
  full CI, cold replay or receipt refresh. Inspect the diff. Check syntax only
  for machine-readable inputs actually changed.
- Source changes need the complete affected exact comparison, including its
  owned extents and relocations. Recheck known consumers of changed shared
  inputs. Tool changes need relevant positive/negative cases and a bounded
  real smoke. Run a broad suite only for an explicit request or a demonstrated
  impact that cannot be narrowed; state that reason first.
- Do not add tests or live freshness gates for prose hashes, prompt wording,
  progress totals, handoff source-SHA tables or ordinary source spelling.
  Bind exact evidence to executable inputs and relevant verification settings.
  Keep Git metadata as provenance. A document genuinely consumed by a build
  remains a build input. Never edit historical receipts to manufacture a pass.
- Plain Markdown is sufficient for knowledge: record the finding, applicable
  target/compiler/scope, evidence links, useful failed attempts and open
  questions. No mandatory JSON conversion, promotion workflow or test per note.
  Search relevant sections; do not read an entire large knowledge journal.
- Write documentation and knowledge in natural, fluent English. Lead with the
  concrete finding and connect the evidence and next action clearly. Remove
  filler, circular restatements, repeated conclusions and unnecessary jargon.
  Do not add defensive-claim boilerplate. State a real uncertainty or scope
  limit once when it changes a decision; do not repeat disclaimers about what
  a note does not prove, certify or authorize. Prefer a small useful note over
  an exhaustive report that is difficult to read.
- Keep handoff focused on current work, evidence references, blockers and the
  next useful action. Generate progress from canonical ledgers; avoid manual
  duplicate counts, recurring full reports and unrelated documentation chores.
- Preserve dirty/untracked work, targets, useful evidence, game-specific Wine
  prefixes and analyzer projects. Exactness still requires a complete strict
  comparison; knowledge and successful commands alone confer no exact credit.


## Current operator status

At the **2026-11-08 handoff**, the operator asked for clean source/docs and
safe storage reduction so another agent can continue. For reconstruction,
work on both big nonexact bosses and exact leaves. Challenge previous
hypotheses against the original target and pinned compiler; retain and
finish dirty/untracked work instead of resetting it. Commit coherent advances
using `gpt-web: <English subject>`. Use only affected complete source-cold
PE/COFF comparisons and bounded boss diagnostics, not routine all-owner replay.

The **current execution guide** is
`docs/TH09_NEXT_AGENT_HANDOFF_20261108.md`, with the live ledger and all 13
open frontiers in `docs/RE_HANDOFF.md`. The source baseline for this refresh
is `6d3a3618`; `main` was clean and in sync with `origin/main` before the
handoff-only documentation commits. The latest source-affecting commit is
`f40b2755`, while `6d3a3618` changes only the boss diagnostic and its
regression test. The November 7 guide and
`docs/RE_HANDOFF_HISTORY_20261108.md` are **historical snapshots**, not current
statements about HEAD, remaining cache or active next steps. Always inspect
`git status`, `git log` and canonical ledgers again: documentation commits can
advance HEAD without source changes. Git commits are not Factory receipt,
byte-exact, full product, or remote-push acceptance proofs.

Use the live registered repository worktree exposed to the current agent for
Git, source and build operations; do not switch to an arbitrary checkout or
assume a previous session's working directory. For static analysis use the
target-attested local Ghidra runner and pinned VC7.1 compiler Oracles. Direct
Ghidra analysis is distinct from the compiler/byte-exact Oracle. Compile
serially at lowered priority, bound resources and reuse unchanged
source/profile/object proofs. Recover dirty work rather than discarding or
hiding it.

This repository targets only the original Japanese TH09 version 1.50a
executable identified by SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Never substitute a localized, modified, Steam, trial, or earlier executable.

The canonical private game file is resources/th09.exe from the repository root.
It is read-only, ignored operator input and must never be committed. Repository
tools use this path by default. Do not search /mnt, add a Windows game-directory
mount, or set TH09_TARGET_PATH during normal work; that variable is only an
explicit local override.

## Mandatory session recovery

Before changing anything:

1. Read docs/RE_HANDOFF.md and the instructions relevant to the change. For
   reconstruction also read docs/RE_WORKFLOW.md, docs/ORACLES.md and affected
   source/ledger rows.
2. Run git status --short --branch. Inspect tracked, staged, dirty and untracked
   work; review diffs and recover/supersede valuable interrupted work rather
   than silently working around it.
3. Before target-dependent work, run python3 scripts/verify-target.py,
   python3 scripts/validate-tracking.py --require-target and
   python3 scripts/report-reconstruction-status.py. Before Ghidra analysis,
   attest disk and Program SHA-256, i386 language, image base, entry point and
   every file-backed PE byte mapped into the Program. Metadata hashes alone do
   not prove the mapped database is unchanged. Ghidra is not an exactness
   Oracle. Documentation-only work does not need compiler/Ghidra replay.
4. Work on one bounded owner or coherent maintenance batch and leave a
   reviewable local checkpoint. Rebuild/replay affected owners only; no full
   cold replay for routine iteration.

Consult the Factory ontology, semantic-reconstruction, agent-autonomy and
worktree-recovery guidance when those documents are available in the current
agent environment. Adjacent TH08/TH095 repositories are read-only hypothesis
sources only; they never provide TH09 byte/layout/ownership evidence.

## Adjacent-game hypothesis sources

The Factory mounts two adjacent reconstructions read-only for TH09 work:

- `/home/pentester/coding/codex_ida/th08-reconstruction/th08`
- `/home/pentester/coding/codex_ida/th095-reconstruction/th095`

Inspect their source, history, scripts, and game-local knowledge when a TH09
subsystem has a plausible analogue. They may suggest source shape, names, engine
structure, compiler idioms, and useful probes, but they are hypothesis material
only. TH095 is itself undergoing semantic reconstruction, so its semantic names,
owners, layouts, and abstractions are especially provisional. Neither adjacent
game's addresses, bytes, layouts, ownership, behavior, or exactness claims are
TH09 evidence. Accept every TH09 conclusion only from TH09-local target, Ghidra,
xref, ABI, compiler, exact-byte, build, or runtime evidence. When the games
differ or the comparison is inconclusive, keep the TH09 fact unknown.

## Evidence and state

- Keep target observations, Ghidra observations, compiler observations, runtime
  observations, cross-game corroboration, inference, and unknowns distinct.
- Ghidra names, types, boundaries, and decompilation are provisional semantic
  evidence. They do not establish authorship, object ownership, or exactness.
- Keep boundary review, origin, source presence, exact codegen, native build
  closure, runtime validation, and port status independent.
- Only a target-bound, complete, reproducible VC7.1 comparison may enter
  `config/matches.csv`. Near matches remain candidates.
- Use natural source. Never copy target bytes, manufacture fake returns, add
  inert padding, lie about the ABI, or patch target code to force equality.
  Do not introduce assembly, register/variable-order hints, dummy clobbers or
  compiler-profile roulette to force a match.
- The former no-code _ReadWriteBarrier() exception in
  TitleScreenResultDraw.cpp was removed after the Cirno-Labs reference
  integration reproduced the complete 1939-byte/82-field owner using natural
  source. Do not restore it or add new barriers/assembly/register hints as a
  generic codegen tactic. See docs/CIRNO_LABS_INTEGRATION_20261105.md.
- Record durable TH09 facts in `docs/KNOWLEDGE_BASE.md`. Cross-game promotion
  belongs to a later Factory review and is not performed by a game session.

## Phase gates

The required order is exact reconstruction, faithful Windows i386 product
closure, semantic reconstruction with target-plus-native dual Oracles, and only
then portability. These later phases are not prerequisites for an exact-source
checkpoint. Do not treat exact units as proof that the executable builds or runs.

## Exact-phase validation scope

- Verify the affected complete functions, physical tails/tables and relocations
  with the pinned VC7.1 Oracle. Include affected exact siblings and callees;
  reuse unchanged source/profile-bound proofs.
- Do not add allocation-failure injection, null-pointer matrices, malformed game
  input, fuzzing or general defensive-runtime tests as routine exact-phase work.
  Recover branches present in the original; do not add defensive game behavior.
  Extra behavioral tests need a specific reconstruction question or operator request.
- Keep tests that detect wrong targets, incomplete comparisons, changed bytes,
  relocation identities or stale proof inputs. Source spelling, whitespace,
  include-count and handoff-format snapshots are not acceptance criteria.
- Run checks relevant to the changed files. Documentation/test-only edits need
  no compiler replay or Ghidra attestation. Full public CI is optional for ordinary
  exact checkpoints; run it when changing shared validation or CI tooling.
- Whole-product builds, runtime scenarios, dual-Oracle semantics and portability
  validation belong to their later phases, not each exact iteration.

This scope supersedes historical full-check lists in handoff/experiment notes.

## Autonomy and tools

Agents may use broad repository Bash and compose small Ghidra, compiler, Wine,
comparison, and Git tools as needed. The framework exists to improve feedback,
accuracy, and resumability, not to restrict competent investigation. Each
important operation must still fail closed on the wrong target.

Ghidra metadata writes for reviewed names, types, prototypes, and comments are
allowed when useful and must be read back. Never patch target bytes. The
analysis database is working state; mirror durable conclusions into the repo.

## Checkpoints and artifacts

- Use concise, coherent local commits prefixed **gpt-web: ...**. Commit after
  each substantive advance and create multiple commits in a session when there
  are multiple reviewable checkpoints. Report local checkpoints separately
  from independently verified remote state.
- Route current work through `docs/RE_HANDOFF.md`; original-bound
  accepted receipts now live in `docs/RE_ACCEPTED_RECEIPTS.md` with
  immutable source commit bindings. Superseded long-form checkpoint details
  are preserved in `docs/RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md`, not
  appended to the active handoff; previous file deletions are recorded in
  `docs/RE_ARTIFACT_CLEANUP_HISTORY.md`. Archival receipts are **not**
  current-source exactness credit; clean handoffs may have no `build/`.
- Keep decompiler dumps, logs, experiments, screenshots, and generated reports
  below `.analysis/`; keep builds below `build/` and downloaded tools below
  `.tools/`.
- Do not accumulate scratch indefinitely. At the end of a bounded unit, retain
  only evidence needed to reproduce an unresolved claim, turn reusable work
  into a script or durable note, and remove superseded disposable artifacts.
- `scripts/clean-generated-artifacts.py` previews by default; `--apply`
  removes ignored `build/` and disposable debug/bytecode caches under `.analysis/`.
  Analysis OBJ files are preserved unless `--reviewed-objects` supplies an explicit
  path/SHA-256 list. A file or build-tree change after inspection rejects cleanup.
  **After a clean handoff, `build/` may be absent**. Build each exact unit and
  diagnostic object afresh before comparing; do not treat the absence of an
  old `.obj`/`.pdb` as lost canonical evidence. Preserve original executable,
  tracked source and unreproduced analysis/experiment evidence.
- The 2026-11-03 local archive of unreferenced old experiments is a
  **lossless, per-file SHA-verified ignored tar.zst**, not a deletion of
  original evidence; see `docs/RE_ARTIFACT_CLEANUP_HISTORY.md`. Referenced
  analysis directories and both Ghidra projects remain unpacked. Restore
  selected old packets when required before trying historical scripts.
- The 2026-11-04 local cleanup additionally archived 1,258 older large
  generated JSON snapshots with **per-file SHA-256** into a separate ignored
  `tar.zst`. To restore one original JSON without unpacking the whole archive,
  preview `python3 -B scripts/restore-analysis-archive.py --path FILE`, then
  repeat with `--apply`. The helper refuses divergent files, symlinks and
  unknown paths. `build/` was intentionally cleared, and Git objects were
  repacked with `git gc --prune=never` while preserving all live refs; neither
  storage action changes accepted source-exact evidence. See the newest
  2026-11-04 audit and handoff before deleting any other evidence.
- The 2026-11-06 cleanup losslessly archived 1,208 build OBJ plus 209 large
  analysis JSON and 96 private TOML files, with per-member SHA-256 validation.
  `build/` was absent at that historical checkpoint and may reappear after
  focused compiles or disappear again after a reviewed cache clean. Read the current handoff before replay; use
  `scripts/restore-analysis-archive.py --bundle .analysis/handoff-archive-20261106`
  with `--path` or `--prefix` (preview, then `--apply`) to recover named inputs.
  The interrupted RunEcl reference-parameter object is recovered but unaccepted;
  its postcompile harness failed. Never promote it from object existence alone.
- The 2026-11-07 next-agent handoff also reviewed and removed 183 newly
  generated cache files, then repacked Git with `gc --prune=never` and
  reflog expiry disabled. It retained all refs/reflogs, three archives,
  original/unique analysis objects, both Ghidra projects and the canonical
  private target. `build/` is now absent again; focused compiles recreate it.
  Precise logical-byte measurements and local proof receipts are in
  `docs/RE_ARTIFACT_CLEANUP_HISTORY.md`.
- Never commit the original executable, game archives/data, IDA databases,
  Ghidra projects, toolchains, generated decompiler text, credentials, or private endpoints.
- Finish with `git diff --check` and the checks required by the change: the
  focused Oracle for reconstruction, tracking/progress for ledger changes,
  documentation validation for status claims, and relevant tooling tests.
  `python3 scripts/ci.py` remains available for full public checks. Accepted
  receipts are source-commit scoped; never report a pending replay as accepted.

## Shared Factory runtime

Follow [the shared tools instructions](docs/SHARED_TOOLS.md). Ghidra/JDK and
Wine executables are shared through Factory; repository scripts also work
without MCP. Use `scripts/shared_runtime.py` and the existing compiler/analysis
frontends. Keep this game's prefix, analyzer project, Java home/cache, target
and compiler profile isolated. Do not install duplicate tools or inherit
another game's `WINEPREFIX`. This shared-runtime procedure supersedes older
per-repository installation instructions; pinned versions/hashes still apply.

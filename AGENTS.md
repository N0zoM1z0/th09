# TH09 reconstruction agent rules

## Current operator status

TH09 reconstruction is active on explicit operator request. The operator
requires serious work on both complete large bosses and leaf functions,
source/target re-investigation rather than trusting old notes, and English
commit subjects of the form `gpt-web: <summary>` after each substantive checkpoint. Make multiple coherent
commits when a session produces multiple reviewable advances; do not stop after
the first useful commit. Routine iteration must rebuild/replay only affected
owners and dependencies; do not run a full cold replay unless explicitly
requested.

The **2026-11-04 current restart guide** is
`docs/TH09_NEXT_AGENT_HANDOFF_20261104.md`. Earlier source checkpoints
(`e4ea703b`, `80313a4f` and October/early-November handoffs) are
historical, not statements about current HEAD. Re-read `git log -1
--oneline`, the real Git status and the ledgers for each session: a
handoff/cleanup commit may advance HEAD without modifying source.
`docs/RE_HANDOFF.md` and `docs/TH09_NEXT_AGENT_HANDOFF_20261102.md`
retain dated evidence, including superseded "current" claims. Commit hashes are not Factory
acceptance, original-byte, or remote-publication proofs.

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
- One current source-specific exception is the no-code VC7.1
  _ReadWriteBarrier() in TitleScreenResultDraw.cpp: it preserves the reviewed
  ordering of the scoreGroup store and subsequent currentScreen snapshot and
  emits no runtime instruction. It remains a non-exact, source-specific
  compiler scheduling constraint, not semantic evidence or a reusable
  matching trick. Re-evaluate it only with a complete-owner proof; do not add
  new barriers/assembly/register hints as a generic codegen tactic.
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
- Never commit the original executable, game archives/data, IDA databases,
  Ghidra projects, toolchains, generated decompiler text, credentials, or private endpoints.
- Finish with `git diff --check` and the checks required by the change: the
  focused Oracle for reconstruction, tracking/progress for ledger changes,
  documentation validation for status claims, and relevant tooling tests.
  `python3 scripts/ci.py` remains available for full public checks. Accepted
  receipts are source-commit scoped; never report a pending replay as accepted.

# TH09 reconstruction agent rules

## Current operator status

The operator cleared the reconstruction goal on October31 and requested code,
documentation and storage housekeeping. Current work is maintenance; the
unfinished98% milestone does not authorize automatic reconstruction. Latest
game-source checkpoint is a3e4aaee (unified LZSS codec). The earlier71246812
maintenance checkpoint, d332bec4 stop, and intervening reconstruction remain
historical evidence. Preserve documentation, research records, current game
source, target/toolchains and the working Ghidra project. Delete unused old code
copies and compiler artifacts instead of archiving them.

On October 29 the operator resumed reconstruction with direct local tools.
The current explicit routing is **Ghidra directly, without IDA, REA or Factory
MCP**. Use `scripts/ghidra-analysis.py` and the local `analyzeHeadless` executable;
reuse the verified project instead of importing the target for every query.
The October 24 stop and earlier tool routing are historical checkpoints.

When reconstruction is explicitly resumed, prioritize complete RunEcl and EnemyManager::OnUpdate
exactness toward the unfinished 98% reviewed-authored-byte milestone. Explore
whole-function dataflow, shared types and real translation-unit boundaries as
well as local source models; historical assumptions are not conclusive.
Preserve existing exact matches and validate complete owners and affected callees.

Use repository Bash, target-bound local Ghidra analysis and pinned VC7.1 Oracles.
Compile serially at lowered priority, bound resources and reuse unchanged proofs.
Recover dirty work rather than discarding or hiding it.
Use English commits prefixed `gpt-6.1-sol:`, author
`N0zoM1z0 <1705004398@qq.com>`, and trailer
`Co-authored-by: gensokyo-dev <342580529+gensokyo-dev@users.noreply.github.com>`.
Create local checkpoints; do not infer remote publication from local state.
Read docs/RE_HANDOFF.md for source and evidence routing.

This repository targets only the original Japanese TH09 version 1.50a
executable identified by SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Never substitute a localized, modified, Steam, trial, or earlier executable.

The canonical private game file is
`/home/pentester/coding/codex_ida/th09-reconstruction/th09/resources/th09.exe`
(`resources/th09.exe` from the repository root). It is a read-only, ignored
operator input copied into WSL from the Windows game directory; it must never be
committed. Repository tools use this path by default. Do not search `/mnt`, add
a Windows game-directory mount, or set `TH09_TARGET_PATH` during normal reconstruction
work. That environment variable is only an explicit local override.

## Mandatory session recovery

Before changing anything:

1. Read the current `docs/RE_HANDOFF.md` checkpoint and the instructions relevant
   to the change. For reconstruction, also read `docs/RE_WORKFLOW.md`,
   `docs/ORACLES.md`, and the affected source or ledger rows.
2. Run `git status --short --branch`, inspect tracked and untracked work, and
   review the diff. A previous Web session may have disconnected while leaving
   valuable dirty work. Understand, recover, finish, or deliberately supersede
   it; do not silently work around it.
3. In the repository shell, run `python3 scripts/verify-target.py`,
   `python3 scripts/validate-tracking.py --require-target`, and
   `python3 scripts/report-reconstruction-status.py` before target-dependent
   work. Before Ghidra analysis, verify the disk and Program SHA-256, i386
   language, image base and entry, and compare every file-backed PE header and
   section against mapped Ghidra memory. The direct runner checks these before
   analysis and every export. Metadata hashes alone do not prove an unmodified
   mapped database. Ghidra is not an exactness Oracle.
4. Work on one bounded unit or one coherent infrastructure batch and leave a
   reviewable local checkpoint.

Consult the Factory guidance when the task needs it:
`/home/pentester/coding/codex_ida/touhou-reconstruction-factory/docs/ontology.md`,
`docs/semantic-reconstruction.md`, `docs/agent-autonomy.md`, and
`docs/worktree-recovery-and-analysis-artifacts.md` below that Factory root.
These explicit paths are authoritative even when a client does not inject a
plugin skill.

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

- For the current local campaign, use concise, coherent local commits
  prefixed **`gpt-6.1-sol: ...`**, with the author and co-author trailer above.
  Report local checkpoints separately from independently verified remote state.
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
  removes ignored `build/` and disposable compiler binaries under `.analysis/`.
  **After a clean handoff, `build/` may be absent**. Build each exact unit and
  diagnostic object afresh before comparing; do not treat the absence of an
  old `.obj`/`.pdb` as lost canonical evidence. Preserve original executable,
  tracked source and unreproduced analysis/experiment evidence.
- Never commit the original executable, game archives/data, IDA databases,
  Ghidra projects, toolchains, generated decompiler text, credentials, or private endpoints.
- Finish with `git diff --check` and the checks required by the change: the
  focused Oracle for reconstruction, tracking/progress for ledger changes,
  documentation validation for status claims, and relevant tooling tests.
  `python3 scripts/ci.py` remains available for full public checks. Accepted
  receipts are source-commit scoped; never report a pending replay as accepted.

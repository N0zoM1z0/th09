# TH09 reconstruction agent rules

## Current operator status

The current operator requests continued **TH09 original v1.50a** exact
reconstruction across large boss functions and small leaves, with the live
roadmap prioritizing RunEcl, Enemy/Etama, TitleScreen and ExAttack. Work from
the real Git tree, not a historical 979/876 scoreboard or unverified document.
**Use `gpt-web: ...` for every new commit.** Commit tested substantive progress,
including bounded accepted source corrections and reproducible codegen evidence.
Never skip, hide, discard or avoid existing dirty work: examine and finish it.
Factory's live repository shell cannot push over the network; report local Git
commit versus remote push distinctly. The earlier `gpt-6.1-sol` prefix and
push language reflect a superseded campaign, not this operator's instructions.
RunEcl and the 95% reviewed-authored-byte milestone remain open. Use the
registered `th09-ida` provider in Factory/Web or direct local IDA, the live
repository shell and the pinned VC7.1 compiler/comparison Oracles. See
`docs/RE_HANDOFF.md` for the current ledger and verified frontiers.

This repository targets only the original Japanese TH09 version 1.50a
executable identified by SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Never substitute a localized, modified, Steam, trial, or earlier executable.

The canonical private game file is
`/home/pentester/coding/codex_ida/th09-reconstruction/th09/resources/th09.exe`
(`resources/th09.exe` from the repository root). It is a read-only, ignored
operator input copied into WSL from the Windows game directory; it must never be
committed. Repository tools use this path by default. Do not search `/mnt`, add
a Windows game-directory mount, or set `TH09_TARGET_PATH` during normal Factory
work. That environment variable is only an explicit local override.

## Mandatory session recovery

Before changing anything:

1. Read `docs/RE_HANDOFF.md`, `docs/RE_WORKFLOW.md`, `docs/ORACLES.md`, and the
   relevant source or ledger rows.
2. Run `git status --short --branch`, inspect tracked and untracked work, and
   review the diff. A previous Web session may have disconnected while leaving
   valuable dirty work. Understand, recover, finish, or deliberately supersede
   it; do not silently work around it.
3. In the repository shell, run `python3 scripts/verify-target.py`,
   `python3 scripts/validate-tracking.py --require-target`, and
   `python3 scripts/report-reconstruction-status.py` before target-dependent
   work. For this local session, call direct IDA Pro MCP `get_metadata` with
   exactly `{}` and run `python3 scripts/check-ida-mcp.py` to check the entry
   point and distributed mapped bytes against the verified private target.
   Metadata hashes alone do not prove the mapped database is unmodified.
   When using Factory/Web, call `th09-ida` `get_metadata` with `{}` via the
   attested provider; its passed target identity is not an exactness Oracle.
4. Work on one bounded unit or one coherent infrastructure batch and leave a
   reviewable local checkpoint.

When available, also read the Factory guidance at
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
TH09 evidence. Accept every TH09 conclusion only from TH09-local target, IDA,
xref, ABI, compiler, exact-byte, build, or runtime evidence. When the games
differ or the comparison is inconclusive, keep the TH09 fact unknown.

## Evidence and state

- Keep target observations, IDA observations, compiler observations, runtime
  observations, cross-game corroboration, inference, and unknowns distinct.
- IDA names, types, boundaries, and decompilation are provisional semantic
  evidence. They do not establish authorship, object ownership, or exactness.
- Keep boundary review, origin, source presence, exact codegen, native build
  closure, runtime validation, and port status independent.
- Only a target-bound, complete, reproducible VC7.1 comparison may enter
  `config/matches.csv`. Near matches remain candidates.
- Use natural source. Never copy target bytes, manufacture fake returns, add
  inert padding, lie about the ABI, or patch target code to force equality.
- Record durable TH09 facts in `docs/KNOWLEDGE_BASE.md`. Cross-game promotion
  belongs to a later Factory review and is not performed by a game session.

## Phase gates

The required order is exact reconstruction, faithful Windows i386 product
closure, semantic reconstruction with target-plus-native dual Oracles, and only
then portability. Do not treat exact units as proof that the executable builds
or runs. Audit data owners, static initialization, resources, library objects,
link order, and exercised runtime paths before opening the semantic gate.

## Autonomy and tools

Agents may use broad repository Bash and compose small IDA, compiler, Wine,
comparison, and Git tools as needed. The framework exists to improve feedback,
accuracy, and resumability, not to restrict competent investigation. Each
important operation must still fail closed on the wrong target.

IDA metadata writes for reviewed names, types, prototypes, and comments are
allowed when useful and must be read back. Never patch target bytes. The
analysis database is working state; mirror durable conclusions into the repo.

## Checkpoints and artifacts

- For the current operator campaign, use concise, coherent local commits
  prefixed **`gpt-web: ...`** as explicitly requested. Factory repository Bash
  has no network transport for a remote push; never claim a local commit was
  automatically pushed.
- Route current work through `docs/RE_HANDOFF.md`; original-bound
  accepted receipts now live in `docs/RE_ACCEPTED_RECEIPTS.md` with
  immutable source commit bindings, and previous file deletions are recorded
  in `docs/RE_ARTIFACT_CLEANUP_HISTORY.md`. Archival receipts are **not**
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
  toolchains, generated decompiler text, credentials, or private endpoints.
- Finish with the focused Oracle, `python3 scripts/validate-tracking.py`,
  `python3 scripts/validate-docs.py`, `python3 scripts/progress.py --check`,
  `python3 scripts/ci.py`, and `git diff --check`. Accepted receipts are
  source-commit scoped; document the actual accepted claim and receipt, never
  an outstanding replay as already accepted.

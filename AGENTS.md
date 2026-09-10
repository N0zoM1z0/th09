# TH09 reconstruction agent rules

This repository targets only the original Japanese TH09 version 1.50a
executable identified by SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Never substitute a localized, modified, Steam, trial, or earlier executable.

## Mandatory session recovery

Before changing anything:

1. Read `docs/RE_HANDOFF.md`, `docs/RE_WORKFLOW.md`, `docs/ORACLES.md`, and the
   relevant source or ledger rows.
2. Run `git status --short --branch`, inspect tracked and untracked work, and
   review the diff. A previous Web session may have disconnected while leaving
   valuable dirty work. Understand, recover, finish, or deliberately supersede
   it; do not silently work around it.
3. Run `python3 scripts/verify-target.py`, `python3 scripts/check-ida-mcp.py`,
   `python3 scripts/validate-tracking.py --require-target`, and
   `python3 scripts/report-reconstruction-status.py` before target-dependent
   work. Stop on identity or provider mismatch.
4. Work on one bounded unit or one coherent infrastructure batch and leave a
   reviewable local checkpoint.

When available, also read the Factory guidance at
`/home/pentester/coding/codex_ida/touhou-reconstruction-factory/docs/ontology.md`,
`docs/semantic-reconstruction.md`, `docs/agent-autonomy.md`, and
`docs/worktree-recovery-and-analysis-artifacts.md` below that Factory root.
These explicit paths are authoritative even when a client does not inject a
plugin skill.

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

- Use concise, coherent local commits, with `gpt-web: ...` for Web-authored
  checkpoints. GPT-web may commit in this repository but must not push.
- Keep decompiler dumps, logs, experiments, screenshots, and generated reports
  below `.analysis/`; keep builds below `build/` and downloaded tools below
  `.tools/`.
- Do not accumulate scratch indefinitely. At the end of a bounded unit, retain
  only evidence needed to reproduce an unresolved claim, turn reusable work
  into a script or durable note, and remove superseded disposable artifacts.
- Never commit the original executable, game archives/data, IDA databases,
  toolchains, generated decompiler text, credentials, or private endpoints.
- Finish with the focused Oracle, `python3 scripts/validate-tracking.py`,
  `python3 scripts/progress.py`, `python3 scripts/ci.py`, and `git diff --check`.

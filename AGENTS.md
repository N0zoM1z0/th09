# TH09 reconstruction agent rules

## Working loop

Prioritize exact reconstruction and useful knowledge. Use the shortest useful
loop: inspect relevant state, analyze/edit, compile and compare the affected
scope, retain useful learning, and create a coherent checkpoint.

Use existing scripts and composable Bash. Add only the smallest change needed
for an observed problem. Do not add frameworks, registries, state machines or
reporting processes for routine reconstruction or cleanup.

Documentation and knowledge edits need diff review, not compiler/Ghidra replay.
Check syntax for machine-readable inputs actually changed. Source changes need
complete affected exact comparisons, owned extents and relocations, including
known consumers of changed shared inputs. Tool changes need relevant positive
and negative cases and a bounded real smoke. Use a broad suite only for an
explicit request or an impact that cannot be narrowed; state the reason first.
Routine commits use basic CI and tests relevant to the change. The full
unit suite is optional, even for CI/tooling edits; run it only when requested
or when a specific impact cannot be checked more narrowly.

Do not add tests or freshness gates for prose hashes, wording, progress copies,
handoff source-SHA tables or ordinary source spelling. Bind exact evidence to
executable inputs and relevant comparison settings; Git supplies provenance.
Never edit a historical result to manufacture a pass.

## Session recovery

Before changing anything:

1. Read [docs/RE_HANDOFF.md](docs/RE_HANDOFF.md) and instructions relevant to
   the change. For reconstruction also read [workflow](docs/RE_WORKFLOW.md),
   [Oracles](docs/ORACLES.md), affected source and canonical ledger rows.
2. Run `git status --short --branch`. Inspect staged, dirty and untracked
   work. Recover useful interrupted work instead of silently bypassing it.
3. Before target-dependent work, run:

   ```bash
   python3 -B scripts/verify-target.py
   python3 -B scripts/validate-tracking.py --require-target
   python3 -B scripts/report-reconstruction-status.py
   ```

   Before Ghidra analysis, attest disk and Program SHA-256, i386 language,
   image base, entry point and every file-backed PE byte mapped into the
   Program. Metadata hashes alone are insufficient. Ghidra is not an
   exactness Oracle. Documentation-only work needs no compiler/Ghidra replay.
4. Work on one bounded owner or coherent maintenance batch. Replay affected
   owners only and leave a reviewable local checkpoint. Do not run a routine
   project-wide cold replay.

Consult Factory ontology, semantic-reconstruction, agent-autonomy and
worktree-recovery guidance when available and relevant. Current operator
retention rules below supersede older archive and cleanup-audit requirements.

## Original target and evidence

The only verified target is Japanese TH09 v1.50a at ignored
`resources/th09.exe`, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Use the pinned VC7.1 compiler/profile. Never patch target bytes.

Keep target, Ghidra, compiler, runtime and adjacent-game observations distinct
from inference and unknowns. Ghidra names, types, boundaries and decompilation
are provisional semantic evidence. They do not establish original authorship,
object ownership or exactness.

Keep boundary review, origin, source presence, exact codegen, native build
closure, runtime validation and port status independent. Only a complete,
reproducible, target-bound VC7.1 comparison may enter `config/matches.csv`.
Near matches remain candidates. A comparison must cover all original code,
physical tails/tables and relocation offsets, types, targets, addends and call
identities. Compiler-private label names may change; verify actual same-section
COFF definitions against original PE operands before rebinding their spelling.

Use natural source. Do not copy target bytes, manufacture fake returns, add
inert padding, misrepresent the ABI, introduce assembly/register hints,
variable-order hints, dummy clobbers or compiler-profile roulette to force a
match. Do not add barriers as a codegen tactic. The old `_ReadWriteBarrier()`
in Title result drawing was removed after natural source reproduced the full
1,939-byte/82-field owner; do not restore it.

Ghidra metadata writes for reviewed names, types, prototypes and comments are
allowed when useful and must be read back. Preserve analysis databases; mirror
durable conclusions into the repository.

## Adjacent-game hypothesis sources

These Factory mounts are read-only:

- `/home/pentester/coding/codex_ida/th08-reconstruction/th08`
- `/home/pentester/coding/codex_ida/th095-reconstruction/th095`

Inspect analogous source, history and local knowledge when useful. Their
names, source shapes and compiler idioms are hypotheses, never TH09 address,
byte, layout, ownership or behavioral evidence. TH095 semantic work is itself
provisional. Accept TH09 conclusions only from TH09-local target, xref, ABI,
compiler, build or runtime evidence. Keep inconclusive facts unknown.

## Phase and validation scope

The required order is exact reconstruction, faithful Windows i386 product
closure, semantic reconstruction against target-plus-native runtime Oracles,
and then portability. Later phases are not prerequisites for an exact-source
checkpoint. Exact functions alone do not prove that the executable builds/runs.

Recover original branches without adding defensive game behavior. Routine
exact work does not need allocation-failure injection, null-pointer matrices,
malformed-input campaigns, fuzzing or general runtime hardening. Extra
behavioral tests need a specific reconstruction question or operator request.
Keep checks that detect wrong targets, incomplete extents, changed bytes,
relocation identities or stale executable proof inputs.

Finish with `git diff --check` and checks relevant to the change: focused
Oracles for reconstruction, tracking/progress for ledger changes, documentation
validation for routing/status claims, and relevant tooling tests. Use
`python3 -B scripts/ci.py` for fast syntax/public-file, ledger, link, progress
and whitespace checks;
`python3 -B scripts/ci.py --full` explicitly adds the optional unit suite.
Neither command compiles the game or replaces affected exact comparisons.

## Knowledge and retention

Keep only:

- Necessary inputs: the original program, toolchain/game-specific Wine runtime,
  shared-runtime selectors and saved Ghidra projects.
- Current results: maintained source, canonical exact ledgers and scripts that
  can rebuild and completely compare the affected scope.
- Useful knowledge: current blockers, verified conclusions and failed approaches
  worth avoiding, recorded briefly in [docs/KNOWLEDGE_BASE.md](docs/KNOWLEDGE_BASE.md).

A document reference alone does not justify keeping a file. Delete obsolete
archives, duplicate evidence, old trial outputs and cleanup reports, removing
their obsolete references too. Do not create replacement archives, recovery
manifests or another audit process just to delete old evidence.

Keep a small experimental compiler context only if an unresolved problem
actually depends on it and it is difficult to reconstruct. Explain that
specific dependency in the handoff. Otherwise keep the useful conclusion and
rebuild from maintained source when needed. Exactness comes from reproducible
complete comparison, not historical file accumulation.

Preserve dirty/untracked operator work, targets, toolchains, game-specific
prefixes and analyzer projects. `.analysis/` mixes protected Ghidra state and
disposable output; identify those inputs before deleting scratch. Builds belong
under `build/`, scratch under `.analysis/`, downloaded tools under `.tools/`.
`build/` may be absent after cleanup; recreate selected objects before comparing.

Use fluent English for repository documentation. Lead with the finding, identify
the applicable target/compiler/scope, and explain a real uncertainty once when
it matters. Plain Markdown is sufficient. No append-only experiment journal,
mandatory JSON promotion or test per note is needed. Search relevant knowledge
sections instead of reading an entire large journal. Cross-game knowledge
promotion belongs to a later Factory review.

Route current work through [docs/RE_HANDOFF.md](docs/RE_HANDOFF.md). Keep it
focused on current work, blockers and the next useful command. Generate
progress from canonical ledgers rather than manually copying totals into
multiple guides. Git retains source history; old receipts do not establish
current-source exactness.

## Tools and checkpoints

Use broad repository Bash and small Ghidra/compiler/Wine/comparison/Git commands
as needed. Every important target-dependent operation must fail closed on the
wrong target. Follow [docs/SHARED_TOOLS.md](docs/SHARED_TOOLS.md): Ghidra/JDK
and Wine executables are shared through Factory; use `scripts/shared_runtime.py`
and the existing compiler/analysis frontends. Keep this game's prefix, project,
Java home/cache, target and compiler profile isolated. Do not install duplicate
tools or inherit another game's `WINEPREFIX`.

Create concise coherent local commits prefixed `gpt-web: ...` after substantive
advances. Report local checkpoints separately from independently verified remote
state. Never commit original executables/game data, analyzer databases,
toolchains, generated decompiler output, credentials or private endpoints.

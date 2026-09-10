# Reconstruction workflow

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
8. Replay affected accepted units, regenerate progress, update the handoff and
   per-game knowledge, inspect the final diff, and commit one coherent local
   checkpoint.

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

`.analysis/` is disposable working state, not a journal. Keep small manifests,
commands, and outputs needed for unresolved evidence; convert reusable logic to
scripts and accepted facts to tracked ledgers/docs. Remove superseded dumps,
duplicate decompilations, stale logs, and obsolete build artifacts at each
checkpoint. Never delete unexplained artifacts from a recovered dirty session.

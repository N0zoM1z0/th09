> **Historical snapshot (2026-11-06), not the current handoff.** The
> [November 7 next-agent guide](TH09_NEXT_AGENT_HANDOFF_20261107.md)
> supersedes this page for source commits, current filesystem/cache state,
> and storage cleanup. Historical private recovery notes below are retained.

# TH09 next-agent handoff — 2026-11-06

Read [AGENTS.md](../AGENTS.md), [the live handoff](RE_HANDOFF.md),
[RE_WORKFLOW.md](RE_WORKFLOW.md) and [ORACLES.md](ORACLES.md) first.
This is a routing snapshot, not Factory acceptance or remote-publication proof.

**Operator update:** reconstruction has resumed after this cleanup snapshot.
For current intent and the `gpt-web:` commit convention, use AGENTS.md and
RE_WORKFLOW.md; the cleanup request below describes a completed earlier task.

## Operator task and current state

The operator ended the running reconstruction goal and requested code/document
cleanup, correction of stale claims and storage cleanup before another agent
takes over. This maintenance session makes no gameplay source changes. Resume
reconstruction according to the next operator request. The intended source style
is natural C++: learn structure and reasoning from references, then investigate
our nonexact code; repeatedly copying a reference is not the objective.

Before cleanup, local HEAD was `89058d69` (Replay method/stage-flow seal), with
clean tracked source. Handoff/tooling commits now advance HEAD; read the actual
log. The live ledger has 1034 authored mappings, 1021 exact and 13 nonexact;
262034/291868 authored bytes (89.7783%). The 98% milestone is unfinished,
with 23997 additional exact authored bytes needed at the current denominator.
There are 38 reviewed origin-unresolved functions, including the byte-proven
15-byte VectorAngle recipe. That recipe contributes no authored exact credit.
Product linking/closure and runtime validation remain open; semantic
reconstruction and portability have not started.

## Recover the actual worktree

```bash
git status --short --branch
git diff --check
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/validate-docs.py
```

Canonical private input is `resources/th09.exe`, Japanese v1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
No alternate target, game-directory mount or normal TH09_TARGET_PATH override.
Direct Ghidra requires full disk/Program/mapped-byte attestation, not metadata
alone. The saved databases under `.analysis/ghidra-direct/` are working state;
never patch target bytes. Compiler Oracle and Ghidra evidence are separate.

## Useful next reconstruction boundaries

[RE_HANDOFF.md](RE_HANDOFF.md) contains the complete 13-owner table. Review the
actual source and ledger before choosing one bounded owner. Recent natural
source checkpoints did not add exactness:

| Owner | Complete current residual | Current review |
| --- | --- | --- |
| RunEcl | 15564 physical bytes, 598 fields, 30 ordinary differences; opcode4 timer transport7 and opcode155 byte/word scheduling23 | [Scope/setter context](TH09_RUNECL_SCOPE_AND_SETTER_CONTEXT_20261106.md) |
| Enemy update | 3900 physical bytes, 97 fields, 13 differences | [Stage source flow](TH09_ENEMY_UPDATE_STAGE_DATAFLOW_20261106.md) |
| Player movement | 1900 physical bytes, 66 fields, 122 differences; the old129 claim is superseded | [Actual literals/expressions](TH09_PLAYER_MOVEMENT_LITERALS_AND_EXPRESSIONS_20261106.md) |
| Gameplay setup | 1682 candidate /1689 original bytes, 172 fields, 694 unequal overlaps plus7 missing; the old1689/171/904 claim is superseded | [Shared rate context](TH09_SETUP_SHARED_RATE_CONTEXT_20261106.md) |
| Player collision | 849/862 bytes, 26 fields, 799 unequal overlaps plus13 missing | [Vector source context](TH09_PLAYER_COLLISION_VECTOR_CONTEXT_20261106.md) |
| Replay Save | 1238 bytes, 53 fields, two differences at+B7/+BB; the old three-byte claim is superseded | [Stage/method flow](TH09_REPLAY_SAVE_STAGE_FLOW_20261106.md) |

No further profile roulette, barriers, assembly, volatile hints or artificial
register/variable ordering. Preserve full physical owners, tails/tables, ordered
calls, actual referents and affected exact siblings/callees. Use the real native
interfaces; reference and adjacent-game sources supply hypotheses, not TH09
ownership, ABI or byte evidence. Original class/TU/lifetime hypotheses remain
unknown unless TH09 evidence resolves them. The old shake source's volatile
construction remains unresolved; rejected natural models are not authorization
to add qualifiers elsewhere.

## Interrupted packet: recovered, unaccepted

`.analysis/runecl-timeout-storage-20261106/RECOVERY.json` is the recovery entry.
SHA-256: `2808163673521fb9c4c6279f31a8b4b233c7610bea6596ff564de50db18a0ccd`.
Its scripts, frozen inputs, modified private sources, reports and logs remain
loose. Nothing was adopted into tracked gameplay source.

Seventeen completed attempt receipts cover16 compiled comparisons and one
compile failure. `explicit-timer-operator` retains30 differences;
`direct-timer-operator` and `shared-jump-operand` retain42. Other coherent
storage models regress; `wire-byte-zero` changes extent and is rejected by the
complete-owner guard. The corrected qualified-word case is separate from the
initial compile failure. These are bounded model observations, not general
proof that natural source cannot resolve the owner.

A further `borrowed-enemy` reference-parameter model compiled an i386 object
(SHA-256 `98f93ee0922b4cf13187c9b215acebc6ad336bacf8b436a9506c037585556f09`),
then the postcompile harness failed at the old RunEcl alias lookup with KeyError.
There is no completed owner comparison/acceptance for it. `borrowed-native-enemy`
never started. The private prototype adapter remains unvalidated: inspect the
recovery inventory and driver log before modifying or rerunning it. All17 build
objects are retained losslessly in the dated archive, not loose build cache.

## Evidence routes and historical material

- Current RunEcl proof: `.analysis/runecl-schedule-source-20261106/source-proof.json`,
  SHA-256 `948bf487b6c0a92ab53f8779296ea8722d4d0320db7eedd50f9a95e0b0caa49a`.
- Current collision proof: `.analysis/player-collision-vector-context-20261106/source-proof.json`,
  SHA-256 `59bf0d33fc4cc4fcf8bf80bf792d00a6de4139915e531ffa65f0bf72205994b0`;
  source `0df06fa5`, seal `3d1ad1c9`.
- Replay controls: `.analysis/replay-save-stage-flow-20261106/source-proof.json`,
  SHA-256 `237f057e56d2c680532f29576434c3737785e50b732b06f8afd935554498639c`;
  source `1be9fee4`, seal `89058d69`. A coherent instance-method witness leaves
  the same residuals and does not establish the original method shape.
- [Accepted receipts](RE_ACCEPTED_RECEIPTS.md) retain immutable source-commit
  bindings. Source proof reuse requires unchanged source/profile/provider inputs;
  old whole-bundle claims do not establish current-source exactness.
- [Handoff history](RE_HANDOFF_HISTORY_20261106.md) preserves the previous active
  file byte-for-byte after an archive notice. Payload SHA-256:
  `3b2ce353995b88a947c5538f39575473d0c9b76e0dd6b0981ca85a3b02ba3e0f`.
  [Knowledge base](KNOWLEDGE_BASE.md) remains a dated evidence index. Earlier
  "goal active", counts, private layouts and register explanations are historical.

Rebuild/replay affected owners only. No full cold replay was requested.
Documentation cleanup requires no new compiler or Ghidra replay. Shared restore
tooling receives public CI; final cleanup measurements and selective restore
commands are recorded in the live handoff and
[artifact cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md).

## Local storage and recovery

The reviewed cleanup reclaimed approximately **402.85 MiB** of logical file
bytes. `build/` is absent. All1208 build objects,209 large JSON snapshots and96
private TOML manifests were saved in `.analysis/handoff-archive-20261106/`, with
every member/original checked by SHA-256 before loose copies were removed.
Analysis OBJ, Ghidra databases, original target and unresolved trial sources
remain. See [the live cleanup record](RE_HANDOFF.md) for exact measurements,
hashes and local audits. The archive is ignored and is not present in a fresh
clone; it is a lossless local evidence store, not an acceptance receipt.

```bash
python3 -B scripts/restore-analysis-archive.py --bundle .analysis/handoff-archive-20261106 --prefix build/analysis/runecl-timeout-storage-20261106
# Add --apply after preview. --path selects a single archived JSON/TOML/OBJ.
```

For another packet, select its analysis/build prefix from the manifest. A
source-proof entrypoint may remain loose while a large diagnostic/private
manifest it references is archived. Restore the named inputs before running an
old verifier; for new work, rebuild only the affected canonical source/profile.
A real19-file restore was verified and its temporary duplicates removed.

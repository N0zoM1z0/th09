---
name: th09-reconstruction
description: Reconstruct bounded code and data from the original Japanese TH09 v1.50a executable using direct target-bound Ghidra, the VC7.1 compiler Oracle, conservative ledgers, native i386 product gate, and later dual-Oracle semantic workflow.
---

# TH09 reconstruction

Read `AGENTS.md` and the current `docs/RE_HANDOFF.md` checkpoint. For
reconstruction, also read `docs/RE_WORKFLOW.md`, `docs/ORACLES.md`, and affected
source/ledger rows. Those files remain the standalone authority.

Recover the current dirty worktree first. Attest the disk target before
target-dependent work and the local Ghidra Program before analysis, then work
on one reviewed extent or coherent infrastructure batch. Documentation/test-only
changes need neither compiler replay nor Ghidra attestation.
The operator explicitly requires direct Ghidra: do not use IDA, REA or Factory
MCP. Use `scripts/ghidra-analysis.py`; its cached exports verify Program identity,
entry, architecture and every file-backed mapped PE byte against the original.
Keep boundary, origin, source presence, exact codegen, whole-product closure,
runtime semantics, and port status separate. Use natural source and the pinned
VC7.1 provider; accept exactness only for a complete zero-difference byte and
relocation replay.

Preserve the phase order: exact reconstruction, faithful Windows i386
compile/link/runtime closure, semantic reconstruction against both original and
native-product Oracles, then portability. Record TH09 facts locally and leave
cross-game Factory promotion for later reviewed analysis.

Keep exact-phase checks focused on complete bytes, relocations, affected owners
and proof inputs. Do not require general runtime hardening, allocation/null
fault injection, source-text snapshots, whole-product closure or full CI for
each exact checkpoint. Later-phase validation stays in its own phase; run full
public CI when shared validation/CI tooling changes.

Use broad repository Bash and composable Ghidra/compiler/Wine/Git tools where they
accelerate correct feedback. Keep scratch below `.analysis/`, inspect it before
cleanup, and create a coherent local checkpoint before handoff.

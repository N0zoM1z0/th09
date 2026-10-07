# TH09 final handoff

## Stopped by user

The user ended TH09 reconstruction on **2026-10-07 at 05:11 UTC** and requested
final code/document cleanup. The former 95% milestone is **not achieved**.
No reconstruction, trial compilation, exact replay, or monitoring is pending.
Resume only after a new explicit user request; this document is not a campaign
continuation prompt.

Cleanup began from clean local checkpoint
02346b74325b7576fe9148dd6e9943481870a365 (five commits ahead of origin/main).
It changes documentation only. Canonical C/C++ source, headers, profiles, ABIs,
ledgers, match units and accepted evidence are preserved. No push was performed.

## Final ledger snapshot

These totals are checked against the live ledgers by scripts/validate-docs.py.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,192 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 980 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 980 |
| Canonical exact functions | 928 |
| Source-present non-exact functions | 52 |
| Source-present non-exact bytes | 57,269 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 218,501 |

The fixed reviewed-authored denominator is **275,770 bytes**. Exact coverage is
**79.2331%**; reaching 95% would have required another **43,481 bytes**.
The 35 unresolved origins are separate. These numbers do not measure the entire
executable or establish a complete game. The faithful Windows i386 product and
runtime gates remain open; semantic reconstruction and portability have not
started. No new exactness credit or acceptance receipt is claimed by cleanup.

## Retained evidence and organization

- [PROGRESS.md](PROGRESS.md) is the generated ledger summary. The exactness
  authorities remain config/functions.csv, config/matches.csv and
  config/match-units.toml; historical prose cannot override them.
- [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) retains the investigation trail,
  source-model negatives, accepted-receipt references and evidence limitations.
- [SMALL_FUNCTION_FRONTIER.md](SMALL_FUNCTION_FRONTIER.md) remains a checked
  inventory, not an active work queue.
- The detailed pre-stop handoff is preserved verbatim in Git at
  02346b74325b7576fe9148dd6e9943481870a365:docs/RE_HANDOFF.md. Its experiment
  summaries were removed from this current-state note to avoid competing
  histories. Inspect that snapshot with git show rather than restoring it.
- Inherited ignored best candidates, full proofs, source/field manifests,
  raw errors, receipts, private target, toolchain and provider state were left
  untouched. Historical hashes are not retrospective producer attestations.

Retained nonexact frontiers include RunEcl's six-handler model (14,792 logical /
15,564 physical bytes, frame 0x168, 598 fields, 375 direct calls), Enemy draw's
four complete differences, Enemy OnUpdate's 39 differences, AddedState's 22
differences, and the isolated 794/800-byte replay-save renderer. These are
diagnostics only. No partial exact credit follows from them.

## Final task-owned cleanup

One fresh DrawReplaySave interpolation-workspace proposal was prepared but not
observed to compile. The initial compile response was not captured by the output
wrapper. Stop-state reconciliation found its manifest still prepared, with no
compile log, object or task build directory; the compile was not retried.
It is now explicitly **cancelled-by-user-stop**, not a measured negative.

Only its reproducible 6,345-byte C++ scratch copy was deleted. A complete forward
and inverse patch check proved recovery from its hash-pinned retained parent.
Its patch, provenance, cancellation, cleanup inventory, observed denial and
successful cleanup-only reconciliation remain in
.analysis/gpt-dots-replay-interpolation-owner-20261007/.
The prior ignored continuation was preserved there as historical evidence, then
replaced with an explicit stopped-state notice. No inherited artifact was deleted.

## Safety and validation

The previously parked SaveReplay second-row serialization, cancelled Type18/24
probes, denied narrow input ABI, and denied EnemyManager new-accessor work remain
closed. The separate UI renderer and BeginPlaybackStage were not serialization
authorization. Cleanup grants no permission to reopen any of them.

Final checks passed: target identity, target-required tracking, generated progress,
all 57 target-independent tests, documentation links/totals, the explicitly open
whole-build graph, workflow Python compilation and whitespace validation.
The maintenance command is repository-command:a66a74dedecf4ffd970c9bc485f99694;
its complete output is retained in the cleanup packet.

Factory reported zero queued, leased, running or cancel-requested TH09 replays.
This task has no child worker or background operation. All its repository commands
are terminal. No new candidate build or routine cold/cohort replay was performed.
No automatic continuation or push is authorized by this stopped-state handoff.

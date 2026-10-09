# TH09 reconstruction handoff

## Active objective

The user explicitly resumed work on **2026-10-09**: reach **95% exact reviewed
authored bytes**, work seriously on large functions, use direct IDA Pro MCP
and local Bash/compiler Oracles without Factory MCP, commit locally as
`gpt-6.1-sol: ...`, and periodically clean reproducible intermediates. Do not push.
The goal remains **active-incomplete**. No new exactness credit was obtained in
this checkpoint. Three additional RunEcl operand controls are measured: two
are neutral and one increases the physical extent. Enemy update's real
GameManager +0xB4 member correction remains in the previous checkpoint.
The October 7 stopped-state checkpoint is preserved in Git at
7347cb3; its cleanup and evidence limits are historical, not current stop orders.

## Live ledger snapshot

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
**79.2331%**; reaching 95% requires another **43,481 bytes**.
The 35 unresolved origins are separate. These numbers do not measure the entire
executable or establish a complete game. The faithful Windows i386 product and
runtime gates remain open; semantic reconstruction and portability have not
started. No new exactness credit or acceptance receipt is claimed by this checkpoint.

## Active frontiers

- RunEcl: 14,792 authored /15,564 physical bytes. Complete replay still has
  2,161 differences, with handler frontiers 4/7/86/155/156/157. Use
  `scripts/inspect-ecl-complete.py`; normalized shape agreement is diagnostic.
- Enemy OnUpdate: 3,883 authored /3,900 physical bytes, 97 independently bound
  fields and 39 full differences. Four residual windows concern early draw
  index, special descriptor/effect scheduling, trail copy and homing registers.
- Player movement: 1,835 authored /1,900 physical bytes, 66 fields and 144 full
  differences. The maintained inspector now reports complete physical bytes and
  independently decoded operands/table entries.
- ExAttack type18/type24: 1,436 target bytes; corrected candidate 1,441/44 fields.
  All 31 ordered calls and 23 direct blocks agree; full replay still has 1,051
  overlap differences plus five excess bytes. Target stack homes remain open.
- DirectPlay message handler: 768 authored /808 physical bytes, 46 fields and
  125 complete differences. Its 34-block switch graph is independently checked.
  Word-call, helper visibility and value/loop contexts are rejected; ABI and
  maintained source stay unchanged.
- PauseMenu update: 1,734 authored /1,776 physical bytes; complete replay has
  1,492 overlap differences plus 44 missing bytes. Use inspect-pause-menu.py.
- Enemy draw: 1,758 bytes and four complete differences around the second
  subtraction/Abs argument. Other nonexact UI, gameplay, network and callbacks
  remain in config/functions.csv. Consult docs/KNOWLEDGE_BASE.md before probes.

## Latest reviewed investigation

RunEcl's SET_FLOAT control borrows the two observed float32 operand slots
through a private eight-byte payload view. The compiler carries its address
in ESI before the source branch: opcode7 grows from 75 to 77 bytes; alignment
raises the physical owner from 15,564 to 15,568 bytes, with 598 fields unchanged.
The strict complete identity gate rejects it. No shifted-byte target score,
handler correspondence, exactness credit or original payload type is claimed.

Explicit invocation of the existing timer assignment operator in opcode 4 and
an explicit low-bit Boolean in opcode 155 are wholly neutral: all 67 owners,
28,838 physical bytes and 1,215 fields match the bound baseline. The low-bit
expression preserves the field update for all 256 input bytes; nonzero Boolean conversion
would not be equivalent. Both complete diagnostics still report 2,161 differences.
No production source changes were retained, so no unchanged canonical cold
compile was added. These three controls were compiled once each and inspected
as one batch. The other 66 owners and nondebug data are intact.

Previous checkpoint 312ca23 retains the following Enemy member correction:

Enemy OnUpdate's speed operand at 0x4109BD reads 0x4A7E44. Fresh GameManager
producer review at 0x41AC86/8C/94 identifies the same mutable signed-int slot as
GameManager root 0x4A7D90 +0xB4, separate from difficulty +0x11C. The private
playfield receiver view now exposes valueB4 with an offset assertion; the
separate g_EnemyCoreDifficultyValue alias is removed. Original names and larger
data/type/native ownership remain open.

Four focused source models were compiled together before one batch-end
canonical-path compile. Separate tracking/homing phases with a shared real
world pointer and a trail destination pointer are neutral. Explicit descriptor
components yield 499 complete differences; direct draw insertion arms yield
2018 overlap differences plus 12 excess bytes. None is retained. Consult the
knowledge base before repeating these contexts.

The canonical correction retains 3,900 physical bytes /97 fields and all 39
complete differences. Raw SHA256 is
a9ca56df7ef5a09effcd22605048b318f19f41d13cf7f1f7740ba2bafbf2b960;
fully resolved SHA256 stays
a204aa32d16f533f4fed2d917aa26f46038a26b0e6dc9bb774a1e8d5c47ab854.
Only raw byte +0x28F changes with the real member addend. Five private names
rename at unchanged actual local destinations. All five bodies and seven
nondebug sections are checked with independent COFF parsing. Attached-effect
update replays 151/151 with two fields. No new exactness credit.

Previous PauseMenu StopAudio dependency repair, complete inspector and rejected
continuation contexts remain documented in Git and the knowledge base. ECL's
2161 complete differences and six-handler frontier remain open. Continue with
large-owner target/local evidence rather than repeating neutral contexts.

## Evidence and artifact lifecycle

Entry at 312ca23 is clean; private target, direct IDA metadata, entry and five
mapped-byte samples pass. No Factory MCP, IDA writes, target patch or delegation.
Current evidence is below .analysis/gpt-6.1-sol-ecl-operand-schedule-20261009/:
source patches/recipe, actual include logs, complete diagnostics, opcode7 dump
and independent 67-owner COFF cross-check. Each carrier has 36 distinct logged
includes; all hashes match preobserved inputs. The unchanged source-bound
baseline is reused from the biased-movement/scalar-storage packets, with all 43
dependency hashes and the main source rechecked. The earlier Enemy evidence
remains below .analysis/gpt-6.1-sol-enemy-copy-context-20261009/.

Three current-batch compiles are terminal. Cleanup removes 27 copied
sources/objects/PDBs totaling 1,391,573 bytes. Source patches and reconstruct.py
remain; reconstruction does not overwrite the terminal manifest. The unchanged
source-bound baseline, canonical caches and inherited evidence remain.
Earlier Enemy and paired-PDB cleanup are not rerun.

The user requests batching changes before cold replay: accumulate one coherent
batch, use focused probes between changes, then close affected Oracles together.
Reuse unchanged source-bound evidence; no unchanged baseline was recompiled in
this batch.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 .analysis/gpt-6.1-sol-enemy-copy-context-20261009/audit.py canonical=build/matching/EnemyManagerCore.obj
python3 scripts/compare-coff-function.py --unit enemy-attached-effect-update
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
python3 scripts/report-ecl-handler-shapes.py build/matching/EclManager.obj
```

Call direct IDA `get_metadata` with exactly `{}` during entry attestation.
ECL's complete inspector returns1 for the measured nonexact result; Player's
inspector exits successfully after a completed diagnostic. Neither compiles
or proves arbitrary supplied-object source provenance. New exact promotion
requires bound source/includes, a fresh pinned canonical build and complete
zero-difference replay.

Validation: ecl-pop-context 147/147, two complete RunEcl diagnostics,
independent 67-owner/nondebug inventory, tracking, progress and
whitespace pass. Isolated CI runs 67 tests (65 pass, two optional Capstone tests
skipped). The worktree checkpoint remains local and is not pushed.

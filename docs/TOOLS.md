# Tool routing

Analysis uses **direct local Ghidra**, repository Bash and the pinned VC7.1
compiler. The operator requires no IDA, REA or Factory MCP. The active goal was
cleared again on October31;current work is maintenance. See
[RE_HANDOFF.md](RE_HANDOFF.md) for current status and later restart scope.

| Need | Command | Claim established |
| --- | --- | --- |
| Verify private target | `python3 scripts/verify-target.py` | exact identity and PE structure |
| Analyze/export with Ghidra | `python3 -B scripts/ghidra-analysis.py --function 0x004086c0` | disk/Program identity, i386 language, base/entry and every file-backed mapped PE byte agree before export |
| Validate tracking | `python3 scripts/validate-tracking.py --require-target` | ledger consistency only |
| Report current state | `python3 scripts/report-reconstruction-status.py` | ledger-derived source/exact totals |
| Regenerate progress | `python3 scripts/progress.py` | generated Markdown/SVG |
| Replay one match | `python3 scripts/build-match-unit.py --unit NAME`, then `python3 scripts/compare-coff-function.py --unit NAME --json` | complete original-bound VC7.1 bytes and relocations |
| Inspect complete RunEcl | `python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj` | every physical byte and independently bound field; diagnostic only |
| Locate RunEcl handler drift | `python3 scripts/report-ecl-handler-shapes.py --explain all build/matching/EclManager.obj` | normalized instruction shapes; diagnostic only |
| Replay an affected owner | `nice -n 10 python3 scripts/check-boss-frontiers.py --owner enemy` | complete diagnostic plus the selected affected exact-owner scope |
| Audit runtime origins | `python3 scripts/audit-runtime-origins.py` | pinned library/import evidence |
| Check product graph | `python3 scripts/build.py --check` | explicit open/closed graph state |
| Validate documentation | `python3 scripts/validate-docs.py` | ledger-derived totals, links and frontier consistency |
| Run public checks | `python3 scripts/ci.py` | target-independent repository consistency; required for shared validation/CI changes |
| Preview cache cleanup | `python3 scripts/clean-generated-artifacts.py` | deletion proposal only; review paths before applying |

The installed Ghidra12.1.4 is available via the ignored `.tools/ghidra` symlink;
`--ghidra-dir` accepts another explicit installation. The runner imports once
and reuses its project below `.analysis/ghidra-direct/` with
`-noanalysis -readOnly`. It checks all685056 file-backed PE header/section bytes
on every export. Metadata hashes alone are insufficient. Ghidra's inferred
names, types and boundaries remain provisional and do not establish exactness
or unique source ownership. Keep reviewed physical tails/tables for comparison.

The unused IDA/MCP clients,inventory initializer and Python MCP bootstrap were
removed during maintenance. Historical code is recoverable from Git history;
historical receipts remain records,not current commands. The Ghidra runner has
no dependency on those clients or their MCP Python package. Preserve the working
Ghidra project and operator-managed analysis databases during cache cleanup.

Rebuild the relevant canonical unit before comparing an object. For the main
ECL/Enemy profile, `exattack-type21-update` builds `build/matching/EclManager.obj`;
`ecl-pop-context` uses that same source/profile. Other ECL units may use a
different profile or object. Use the unit's own configured build for canonical
replay. A nonexact inspector may return1 after a complete comparison; distinguish
ordinary differences from a failed/incomplete identity audit. Exact comparators
must return0. Missing build caches after cleanup are expected.

Canonical build/comparison parses PE/COFF directly and does not depend on
Capstone. Several decoded diagnostic inspectors use the system Python Capstone;
that diagnostic dependency does not alter the compiler profile or exact bytes.
`compile-probe.sh` checks pinned `cl.exe`, `c1.dll`, `c1xx.dll` and `c2.dll`
hashes from `config/tools.lock.toml`, and clears ambient `CL`/`_CL_` options.
Those pins do not prove original retail switches or complete SDK/link ownership.

Use the ignored `resources/th09.exe`. Do not search `/mnt`, mount a Windows
game directory, or commit original game files, databases, tools or decompiler
exports. Keep generated analysis below `.analysis/`, builds below `build/` and
downloaded tools below `.tools/`. The generic cleaner's apply scope includes
`.analysis/` compiler objects, which can be unique proof: use the reviewed
scope and retained-evidence notes in
[RE_ARTIFACT_CLEANUP_HISTORY.md](RE_ARTIFACT_CLEANUP_HISTORY.md).

TH08 and TH095 remain read-only hypothesis sources. Only TH09-local target,
ABI, compiler, linker or runtime evidence can accept a TH09 claim.

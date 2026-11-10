# Tool routing

Use the registered live worktree, pinned VC7.1 compiler and direct local
Ghidra. A Factory repository shell can run these commands but does not itself
establish target identity or exactness. Start with [RE_HANDOFF.md](RE_HANDOFF.md).

| Need | Command |
| --- | --- |
| Verify original | `python3 -B scripts/verify-target.py` |
| Validate ledgers | `python3 -B scripts/validate-tracking.py --require-target` |
| Report current status | `python3 -B scripts/report-reconstruction-status.py` |
| Regenerate progress | `python3 -B scripts/progress.py` |
| Analyze one function | `python3 -B scripts/ghidra-analysis.py --function 0x004086c0` |
| Build exact unit | `python3 -B scripts/build-match-unit.py --unit NAME` |
| Compare complete unit | `python3 -B scripts/compare-coff-function.py --unit NAME --json` |
| Select affected exact consumers | `python3 -B scripts/replay-scoped-exact.py --changed-file PATH --dry-run` |
| Diagnose one nonexact owner | `python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings` |
| Audit runtime origins | `python3 -B scripts/audit-runtime-origins.py` |
| Inspect open product graph | `python3 -B scripts/build.py --check` |
| Check documentation links/frontiers | `python3 -B scripts/validate-docs.py` |
| Check changed machine-input syntax | `python3 -B scripts/ci.py` |
| Full public target-independent checks | `python3 -B scripts/ci.py --full` |

The Ghidra runner checks disk/Program identity, i386 language, base/entry and
every file-backed mapped PE byte before analysis/export. It imports once and
reuses the saved project under `.analysis/ghidra-direct/` with
`-noanalysis -readOnly`. Preserve the saved project roots when removing exports.
Ghidra names/types/body ranges are provisional; compare complete physical
extents, tails/tables, actual fields and calls through the compiler Oracle.

Use [shared Factory tools](SHARED_TOOLS.md). Do not duplicate Ghidra/JDK/Wine
installations or borrow another game's mutable runtime. `compile-probe.sh`
checks pinned compiler DLL hashes and clears ambient `CL`/`_CL_` options.
Canonical build/compare parses PE/COFF without Capstone; some decoded
inspectors use system Capstone. Rebuild a selected unit after cache cleanup.
A complete nonexact diagnostic may return 1; an exact comparator must return 0.

Keep necessary private inputs and reusable scripts. Old archives, trial
objects, dumps and cleanup journals are disposable; a document reference does
not make them inputs. Record useful conclusions in the knowledge base. See
[AGENTS.md](../AGENTS.md) for retention and exactness requirements.

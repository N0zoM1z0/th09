# Tool routing

| Need | Command or provider | Claim established |
| --- | --- | --- |
| Verify private target | `python3 scripts/verify-target.py` | exact identity and PE structure |
| Attest local-Codex IDA | direct `ida-pro-mcp` `get_metadata` with `{}`, then `python3 scripts/check-ida-mcp.py` | target metadata, entry point and distributed mapped-byte agreement |
| Attest Factory/Web IDA | discover `th09-ida`, then call `get_metadata` with exactly `{}` | passed `target:th09-main` attestation over `factory-native-stdio` |
| Call host IDA from shell | `python3 scripts/ida-mcp-call.py --search ...` / `--call ...` | bounded target analysis; metadata writes must be read back |
| Validate tracking | `python3 scripts/validate-tracking.py --require-target` | ledger consistency only |
| Report current state | `python3 scripts/report-reconstruction-status.py` | ledger-derived totals and exact/source frontier |
| Regenerate progress | `python3 scripts/progress.py` | generated Markdown/SVG |
| Replay one match | `python3 scripts/build-match-unit.py --unit NAME`, then `python3 scripts/compare-coff-function.py --unit NAME --json` | bounded VC7.1 codegen comparison |
| Inspect complete RunEcl | `python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj` | every physical byte and independently bound field; diagnostic only |
| Locate RunEcl handler drift | `python3 scripts/report-ecl-handler-shapes.py --explain all build/matching/EclManager.obj` | normalized instruction shapes; diagnostic only |
| Audit runtime origins | `python3 scripts/audit-runtime-origins.py` | pinned archive/import evidence |
| Check product graph | `python3 scripts/build.py --check` | explicit open/closed graph state |
| Run public checks | `python3 scripts/ci.py` | target-independent repository consistency |

The local IDA plugin is operator-managed and reached through the registered
stdio provider. Always compare its metadata with `config/target.toml` before
using semantic results. IDA names, types, decompilation, and function extents do
not independently establish exactness or source ownership.

Rebuild the RunEcl carrier with `python3 scripts/build-match-unit.py --unit
exattack-type21-update` before trusting a cached object. A complete inspector
exit status of 1 is expected for a non-exact owner; inspect its output to
distinguish measured differences from an incomplete or failed identity audit.
Canonical exact-unit comparisons must return 0.

The current TH09 canonical function replay does not import or invoke Capstone.
`build-match-unit.py` drives the pinned VC7.1 compiler, and
`compare-coff-function.py` parses the target PE and candidate COFF directly.
The system Python's Capstone version is therefore not a replay dependency. Do
not copy another repository's Capstone wrapper or lock into TH09 unless a real
consumer is added; that consumer must then fail closed on its pinned identity.

`compile-probe.sh` checks the pinned driver (`cl.exe`), C/C++ frontends
(`c1.dll`, `c1xx.dll`) and optimizer backend (`c2.dll`) before invoking Wine.
All four hashes come from the exact tool-provider commit in `tools.lock.toml`.
The compiler invocation clears ambient `CL` and `_CL_` option variables so the
explicit match-unit/probe profile determines the command-line flags. These
core pins do not establish the retail game's original switches or close SDK,
library, linker, Wine or whole-product ownership.

Factory/Web clients must use their attested `th09-ida` route. Local Codex does
not need Factory when the direct provider is available. Do not run the host-only
`check-ida-mcp.py` from inside a Factory repository shell. Provider discovery
must report `attestation.status=passed`, target `target:th09-main`, and transport
`factory-native-stdio` before any IDA result is used.

The canonical executable is the ignored `resources/th09.exe`. Normal work must
not search `/mnt`, mount a Windows game directory, or commit the target, IDA
database, game data, compiler tools, or generated decompiler output.

TH08 and TH095 checkouts are read-only hypothesis sources. Their names and
source shapes can guide a probe, but only TH09-local target, ABI, compiler,
linker, or runtime evidence can accept a TH09 claim.

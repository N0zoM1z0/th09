# Tool routing

| Need | Command or provider | Authority |
| --- | --- | --- |
| Verify the private target | `python3 scripts/verify-target.py` | target identity and PE structure |
| Attest live IDA | `python3 scripts/check-ida-mcp.py` | provisional semantic provider |
| Discover/call IDA tools | `python3 scripts/ida-mcp-call.py --search ...` / `--call ...` | target-attested; metadata writes explicit |
| Initialize IDA ledger | `python3 scripts/export-ida-inventory.py --initialize` | one-time provisional inventory only |
| Validate ledger graph | `python3 scripts/validate-tracking.py` | consistency, never exactness |
| Inspect status | `python3 scripts/report-reconstruction-status.py` | derived from ledgers |
| Regenerate progress | `python3 scripts/progress.py` | derived documentation/SVG |
| Check build skeleton | `python3 scripts/build.py --check` | proves only explicit open/closed state |
| Public validation | `python3 scripts/ci.py` | target-independent consistency |

The operator-managed IDA plugin listens on Windows localhost port `13337`.
Local Codex reaches it through the registered `ida-pro-mcp` stdio process. The
Factory Web service uses a separate loopback bridge and re-attests this same
target before forwarding semantic reads.

Build tools are shared provider installations where practical; target
selection, source graph, Wine prefix, runtime scenarios, IDA database, and
receipts remain per-game. Do not copy a tool installation into tracked source.

# Tool routing

| Need | Command or provider | Authority |
| --- | --- | --- |
| Verify the private target | `python3 scripts/verify-target.py` | target identity and PE structure |
| Attest live IDA from GPT-web | Discover `th09-ida`, then call Factory-native `get_metadata` | target-bound provisional semantic provider; require passed attestation |
| Attest live IDA from local Codex | `python3 scripts/check-ida-mcp.py` | host-only preflight; do not run inside the Factory shell |
| Discover/call IDA tools | `python3 scripts/ida-mcp-call.py --search ...` / `--call ...` | target-attested; metadata writes explicit |
| Inspect adjacent source | `git -C /home/pentester/coding/codex_ida/th08-reconstruction/th08 log ...`; likewise `/home/pentester/coding/codex_ida/th095-reconstruction/th095` | read-only hypotheses; TH095 semantic state is provisional; never TH09 proof |
| Initialize IDA ledger | `python3 scripts/export-ida-inventory.py --initialize` | one-time provisional inventory only |
| Validate ledger graph | `python3 scripts/validate-tracking.py` | consistency, never exactness |
| Inspect status | `python3 scripts/report-reconstruction-status.py` | derived from ledgers |
| Regenerate progress | `python3 scripts/progress.py` | derived documentation/SVG |
| Check build skeleton | `python3 scripts/build.py --check` | proves only explicit open/closed state |
| Public validation | `python3 scripts/ci.py` | target-independent consistency |

The operator-managed IDA plugin listens on Windows localhost port `13337`.
Local Codex reaches it through the registered `ida-pro-mcp` stdio process. The
shared Factory MCP also owns an `ida-pro-mcp` stdio client directly; TH09 does
not run a second MCP server or use `mcp_for_gptweb`. Before every Web analysis
operation, the Factory re-attests the repository target, private executable,
active IDA metadata and entry point, and distributed mapped `.text` bytes.
GPT-web keeps using the one Factory URL and selects repository `th09` plus
provider `th09-ida`.

The Factory repository runner exposes both adjacent checkouts above as
immutable reference roots. Use focused reads rather than copying their trees or
generated artifacts into TH09. Record the adjacent repository HEAD when a
comparison materially shapes a hypothesis. Any proposed reuse must be
reconciled against TH09-local boundaries, xrefs, ABI, data ownership, compiler
output, and target bytes.

Build tools are shared provider installations where practical; target
selection, source graph, Wine prefix, runtime scenarios, IDA database, and
receipts remain per-game. Do not copy a tool installation into tracked source.

The canonical target used by repository tools is the ignored, operator-supplied
`resources/th09.exe`. It is copied once from the Windows installation into WSL,
verified against `config/target.toml`, and kept out of Git. Normal Factory work
does not mount or inspect the Windows game directory and does not need
`TH09_TARGET_PATH`.

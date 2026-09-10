# TH09 reconstruction handoff

## Current phase

Bootstrap: exact target and IDA provider attested; provisional function
inventory initialized; boundary/origin review has not started. No authored
source, exact function, closed Windows i386 product, runtime semantic result, or
port is claimed.

## Restart commands

```bash
git status --short --branch
git diff --stat
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
```

If `TH09_TARGET_PATH` is not already set, point it at the user's legal original
1.50a executable. Stop if the disk target or active IDA database differs.

## Verified baseline

- Exact target identity and PE layout are recorded in `config/target.toml`.
- VC7.1 build 3077 is observed; all detailed build-shape fields remain unknown.
- The IDA MCP at Windows localhost `13337` is reachable through the registered
  local stdio client and is bound to the exact target.
- Every imported function origin is `unknown`, every disposition is `review`,
  and exact ledgers are empty.
- `config/build.toml` exists from day one but correctly reports an open graph.

## Next bounded work

Review high-information architecture roots and target ownership before writing
source: entry/startup, main loop, supervisor chains, archive loading, ANM/ECL
dispatch, player/opponent state, networking, audio, and shutdown. Select work by
references and subsystem closure, not just small candidate size. Commit stable
local checkpoints; do not push from GPT-web.

# TH09 reconstruction handoff

## Current phase

Exact reconstruction: the target and native IDA provider are attested, the
provisional function inventory is initialized, and boundary/origin review has
started with two CRT/library exclusions. No authored source, exact function,
closed Windows i386 product, runtime semantic result, or port is claimed.

## Latest exact-phase checkpoint

The first reviewed packet covers the CRT startup error helpers at
`0x0047D416-0x0047D45E`.

- `__amsg_exit` is retained at `0x0047D416-0x0047D43A` (37 bytes).
- `_fast_error_exit` is corrected from the imported 33-byte IDA candidate to
  `0x0047D43B-0x0047D45E` (36 bytes). Attested target bytes show the omitted
  tail as `pop ecx; pop ecx; ret`, immediately before the PE entry point at
  `0x0047D45F`.
- Both functions structurally match the corresponding VC7.1 `libcmt.lib`
  `wincrt0.obj` functions across every non-relocation byte, so their origin is
  reviewed as `library` / `CRT` and excluded from authored reconstruction.
- This is boundary/origin evidence only. No source-presence, codegen-exact,
  ownership, whole-build, runtime, or Factory-acceptance claim is made.
- The native `th09-ida` provider remained strongly attested to
  `target:th09-main`; an IDA comment at `0x0047D43B` records the corrected tail
  and was read back successfully.
- Repository-local target-dependent preflights remain unavailable because the
  private `th09.exe` is not mounted at `resources/th09.exe` and
  `TH09_TARGET_PATH` is unset in the repository runner. Target-independent
  `validate-tracking.py`, `build.py --check`, `ci.py`, progress regeneration,
  status reporting, and `git diff --check` pass.

The next connected packet is the PE entry-point CRT startup function at
`0x0047D45F-0x0047D633`. Reconcile its complete `wincrt0.obj` contribution and
origin before moving into the authored `_WinMain@16` chain.

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

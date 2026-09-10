# TH09 reconstruction handoff

## Current phase

Exact reconstruction: the target and native IDA provider are attested, the
provisional function inventory is initialized, and boundary/origin review has
started with three CRT/library exclusions. No authored source, exact function,
closed Windows i386 product, runtime semantic result, or port is claimed.

## Latest exact-phase checkpoints

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
- At the time of the first checkpoint, repository-local target-dependent
  preflights were unavailable because the private target had not yet been
  provisioned in the repository runner. Target-independent validation passed.

The second reviewed packet covers the PE entry-point CRT startup contribution at
`0x0047D45F-0x0047D633`.

- The target PE entry point has no callers and is exactly `0x1D5` bytes, ending
  in `ret` at `0x0047D633`; the next candidate starts at `0x0047D634`.
- VC7.1 `libcmt.lib` `wincrt0.obj` defines `_WinMainCRTStartup` with the same
  `0x1D5`-byte extent. A repository-local, target-verified structural comparison
  masks its 37 explicit i386 COFF relocation fields and matches all 321
  remaining bytes with zero differences. Both normalized bodies hash to
  `30849bbbd7773bbe50c5988f45b09d15a2b2793a7c097ab3481cd3cd624b0e47`.
- The function is therefore reviewed as `library` / `CRT` and excluded from the
  authored denominator. This does not claim reconstructed source or canonical
  codegen exactness.
- The operator has now provisioned the ignored, read-only `resources/th09.exe`.
  `verify-target.py` and `validate-tracking.py --require-target` pass against
  the manifest SHA-256/MD5/PE identity. Factory-native `th09-ida` `get_metadata`
  independently passes attestation for `target:th09-main` over
  `factory-native-stdio`.
- Concurrent operator edits to `.gitignore`, `AGENTS.md`, and `docs/TOOLS.md`
  appeared during this packet. They are preserved unstaged and are not part of
  the reconstruction checkpoint.

The next connected packet is the adjacent CRT floating-point pair
`__finite` / `__fpclass` at `0x0047D634-0x0047D6E4`, including the transition
from the startup contribution and the gap before `_longjmp`.

## Restart commands

```bash
git status --short --branch
git diff --stat
python3 scripts/verify-target.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
```

From GPT-web, discover `th09-ida` through the Factory and call native
`get_metadata`; require `attestation.status=passed`, target
`target:th09-main`, and transport `factory-native-stdio`. The normal Factory
repository target is the ignored, operator-supplied `resources/th09.exe`; do
not set `TH09_TARGET_PATH` for ordinary Factory work.

## Verified baseline

- Exact target identity and PE layout are recorded in `config/target.toml`.
- VC7.1 build 3077 is observed; all detailed build-shape fields remain unknown.
- Factory-native `th09-ida` is strongly attested to the exact target over
  `factory-native-stdio`; its semantic output remains provisional evidence.
- Three startup CRT/library candidates are reviewed as exclusions; all other
  imported origins remain pending and the exact ledgers are empty.
- `config/build.toml` exists from day one but correctly reports an open graph.

## Next bounded work

Review high-information architecture roots and target ownership before writing
source: entry/startup, main loop, supervisor chains, archive loading, ANM/ECL
dispatch, player/opponent state, networking, audio, and shutdown. Select work by
references and subsystem closure, not just small candidate size. Commit stable
local checkpoints; do not push from GPT-web.

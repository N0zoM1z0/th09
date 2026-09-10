# TH09 reconstruction handoff

## Current phase

Exact reconstruction: the target and native IDA provider are attested, the
provisional function inventory is initialized, and boundary/origin review has
started with seven CRT/library exclusions and two exact authored
GameErrorContext functions. The faithful Windows i386 whole-build graph is
still open; runtime semantic
validation and portability work have not started.

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

The third reviewed packet covers the adjacent CRT floating-point helpers at
`0x0047D634-0x0047D6E4` and the following alignment gap.

- VC7.1 `libcmt.lib` `ieeemisc.obj` defines `__finite` at exactly `0x15` bytes
  with no relocations. The verified TH09 target matches all 21 bytes exactly.
- The same object defines `__fpclass` at exactly `0x9C` bytes with two COFF
  relocation fields. After masking offsets `+0x1D` and `+0x6E`, all 148
  remaining target bytes match. The normalized target/object SHA-256 is
  `f7fcb0d2114c273ccfdbf3584b173e2c90f2d2b0aa2f5c7ad67a5736d82d188b`.
- The three target bytes at `0x0047D6E5-0x0047D6E7` are `CC CC CC` and are
  outside both reviewed function extents; `_longjmp` starts at `0x0047D6E8`.
  Their physical owner is intentionally left unknown.
- IDA comments at `0x0047D634` and `0x0047D649` record these origin/boundary
  conclusions and were read back successfully. No target bytes were modified.
- The VC7.1 archive was accessed through the read-only TH095 tool root. TH08
  committed HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was clean when consulted;
  TH095 committed HEAD `0e6f0ef251c0a222737ce2729b22d0fabaac80e7` had unrelated pre-existing
  source/runtime scratch changes. Adjacent-game state remains hypothesis/tooling
  material only; all classifications above are grounded in the TH09 target.

The fourth reviewed packet covers `_longjmp`, the imported `nullsub_10`
split, and `__setjmp3` at `0x0047D6E8-0x0047D7DE`.

- VC7.1 `libcmt.lib` `longjmp.obj` defines `_longjmp` at `0x79` bytes. The
  verified TH09 target matches all 101 non-relocation bytes after masking five
  COFF relocation fields. The correct target extent is therefore
  `0x0047D6E8-0x0047D760`, not the imported `0x78`-byte IDA extent.
- The target byte at `0x0047D760` is the object function's final `ret`; the
  imported `nullsub_10` candidate at that address was removed from the durable
  function inventory rather than retained as an overlapping false function.
- `0x0047D761-0x0047D763` is `CC CC CC` padding with physical ownership left
  unknown. `__setjmp3` begins at `0x0047D764`.
- VC7.1 `libcmt.lib` `setjmp3.obj` defines `__setjmp3` at exactly `0x7B` bytes;
  the verified target matches all 119 non-relocation bytes after masking its
  single COFF relocation at `+0x28`.
- IDA comments at `0x0047D6E8`, `0x0047D760`, and `0x0047D764` document the
  correction and were read back. The native provider exposes no safe atomic
  function-extent delete/resize operation, so the IDA function split itself was
  not mutated. No target bytes were modified.


The fifth reviewed packet enters the authored `_WinMain@16` chain through the
GameErrorContext lifecycle helpers at `0x0042D230-0x0042D28F`.

- `GameErrorContext::ResetContext @ 0x0042D230-0x0042D239` has one TH09 caller,
  `_WinMain@16 +0x456`, and ends in `ret`; six following `CC` bytes are kept out
  of the function and retain unknown physical ownership.
- `GameErrorContext::Flush @ 0x0042D240-0x0042D28F` has one TH09 caller,
  `_WinMain@16 +0x4F6`. Target-local field accesses establish an 8 KiB buffer,
  a buffer-end pointer at `+0x2000`, and a message-box flag byte at `+0x2004`.
  Its separator, `"log"`, `"./log.txt"`, logging call, MessageBox import, and
  file-write call are all target-local references.
- Committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` and TH095
  HEAD `619624ff99fe9d71360b9d9797066bec0d8b5f7a` supplied only a naming/source-shape
  hypothesis for the same error-context layout. TH08 was clean; TH095 had
  unrelated pre-existing EnemyManager/runtime scratch changes. Only committed
  adjacent content was consulted.
- A focused pinned-VC7.1 matrix rejected the adjacent TH095 `/Od /Ob1` code
  shape for TH09. `/O1` reaches the 10-byte Reset body but leaves Flush two
  bytes short; tested `/O2`/`/Ox` variants produce both target extents. The
  canonical TH09 units use one exact-producing `/O2` profile without claiming
  that `/O2` was the original project-wide flag.
- Repository-local `game-error-context-reset` replays 10/10 bytes with no
  relocations. `game-error-context-flush` replays 80/80 bytes with six explicit
  target-bound relocations: three strings, `GameErrorContext::Log @ 0x0042C5C0`,
  imported `MessageBoxA`, and `FileSystem::WriteDataToFile @ 0x0042C4E0`.
- Source presence and canonical exactness are established only for these two
  functions. `config/build.toml` remains honestly open because TU partition,
  global flags, libraries, resources, and link order are still unknown. No
  whole-build or runtime claim follows from the two exact match units.
- `0x0042D290` begins code that has a code xref from `0x0048D8E5`, but the
  current IDA database does not define a function there. It is deliberately not
  merged into `Flush` and remains an ownership/boundary follow-up.

The next evidence-connected packet should continue through the error-context
call seam: recover `GameErrorContext::Log @ 0x0042C5C0` and the neighboring
`0x0042C660` candidate, then validate their ABI, complete extents, relocations,
and optimized VC7.1 source shape before any exact promotion.

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
- Seven CRT/library candidates are reviewed as exclusions. Two authored
  GameErrorContext functions are source-present and canonical exact; all other
  imported origins remain pending.
- `config/build.toml` exists from day one but correctly reports an open graph.

## Next bounded work

Continue the error-context seam from `0x0042C5C0`/`0x0042C660` before
broadening to other architecture roots. Use the exact Reset/Flush relocation
graph to recover the logging ABI and object lifecycle, then follow the file-write
dependency at `0x0042C4E0` if it remains evidence-connected. Preserve unknown
ownership at `0x0042D290` until target-local boundary evidence resolves it.
Commit stable local checkpoints; do not push from GPT-web.

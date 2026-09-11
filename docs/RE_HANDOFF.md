# TH09 reconstruction handoff

## Current phase

Exact reconstruction: the target and native IDA provider are attested, the
provisional function inventory is initialized, and boundary/origin review has advanced through reviewed CRT/library and compiler-generated exclusions plus 55 repository-canonical exact authored functions across the currently reconstructed subsystems. The faithful Windows i386 whole-build graph is still open; runtime
semantic validation and portability work have not started.

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
  canonical GameErrorContext source now uses one exact-producing `/O2 /Oy-`
  profile without claiming either switch as an original project-wide flag.
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

The sixth reviewed packet closes the variadic error-context logging pair at
`0x0042C5C0-0x0042C6F8`.

- `GameErrorContext::Log @ 0x0042C5C0-0x0042C658` and
  `GameErrorContext::Fatal @ 0x0042C660-0x0042C6F8` are variadic member
  functions whose reconstructed object pointer is stack-passed and caller
  cleaned. Their semantic names are reconstruction names, not recovered target
  symbol strings.
- Both call the TH09-local supervisor wrappers at `0x0042B130` and
  `0x0042B160` around formatting and bounded append logic. TH09's wrappers
  themselves maintain the lock-active count, so the committed adjacent TH095
  source's separate counter increments were not copied into TH09 source.
- Log uses an 8 KiB temporary buffer and replays 153/153 bytes with six
  relocations: `__chkstk`, two supervisor-storage references, both wrapper calls,
  and `vsprintf`. Fatal uses a 512-byte temporary buffer, sets the object byte at
  `+0x2004`, and replays 153/153 bytes with five relocations.
- A focused compiler probe found the source shape is not exact under tested
  `/O2` with frame-pointer omission (0xA9/0xA0 bytes). `/O2 /Oy-` and
  `/Ox /Oy-` both produce the target 0x99/0x99 functions exactly; `/O1 /Oy-`
  remains short. The canonical unit uses `/O2 /Oy-`, but the original global
  optimization flags remain unknown.
- Seven `CC` bytes follow each reviewed extent at `0x0042C659-0x0042C65F` and
  `0x0042C6F9-0x0042C6FF`; their physical ownership remains unknown.
- IDA evidence comments were written at `0x0042C5C0`, `0x0042C660`,
  `0x0042D230`, and `0x0042D240` and read back through the attested native
  provider. Function names were deliberately not rewritten because no original
  target symbol-name evidence exists. No target bytes were modified.

The seventh reviewed packet follows the exact Flush relocation into the file
write helper at `0x0042C4E0-0x0042C5B1`.

- The target ABI is fastcall-shaped: path enters in ECX, data in EDX, the 32-bit
  size is the sole stack argument, and each return executes `ret 4`.
- The function first calls the unresolved TH09 path helper at `0x0042ADC0`, then
  acquires supervisor lock 2 and calls `CreateFileA` with `GENERIC_WRITE`,
  `FILE_SHARE_READ`, `CREATE_ALWAYS`, and `FILE_ATTRIBUTE_NORMAL`.
- Open failure formats and frees the Win32 error string, releases lock 2, and
  returns `-1`. A short write closes the handle, releases the lock, and returns
  `-2`; a complete write closes/releases and returns `0`. Unlike the committed
  TH08/TH095 hypotheses, the TH09 target has no debug-print calls in these
  paths, so those adjacent calls were not copied.
- A first natural probe with the error-message local hoisted to function scope
  produced a 0xD4-byte body. Limiting that local to the open-failure branch lets
  VC7.1 reuse the dead incoming size stack slot and produces the target 0xD2
  body. The canonical unit then replays all 210 bytes with sixteen explicit
  target-bound relocations.
- TH08 committed HEAD `a45e99fb1942714e6edded20847e32a654d56f97` and TH095
  committed HEAD `619624ff99fe9d71360b9d9797066bec0d8b5f7a` were consulted only
  for source-shape hypotheses; their debug logging conflicts with TH09 and was
  rejected. Previously recorded adjacent dirty-state classification is
  unchanged, and no adjacent uncommitted content was used.
- Four GameErrorContext units were replayed after shared FileSystem/Supervisor
  declarations were factored into headers and remain exact. The semantic name
  `ResolvePath` is only a reconstructed declaration for the relocation to
  `0x0042ADC0`; that callee is not source-present or exact yet.
- Fourteen `CC` bytes at `0x0042C5B2-0x0042C5BF` remain outside the reviewed
  function with physical ownership unassigned. An evidence comment at
  `0x0042C4E0` was written and read back through attested `th09-ida`; no target
  bytes or function names were modified.

The eighth reviewed packet closes the direct path helper at
`0x0042ADC0-0x0042AE19` while deliberately leaving its data ownership open.

- Target bytes and the exact object show a `__stdcall` single-argument helper:
  it copies the NUL-terminated directory string at `0x004ACD08` into shared
  scratch at `0x004ACC00`, appends the supplied path, returns the scratch
  address, and executes `ret 4`.
- TH09-local xrefs show `0x004ACD08` is also used by `_WinMain@16` startup.
  WinMain fills it with `GetModuleFileNameA(..., 0x104)`, trims the executable
  name to a slash-terminated directory, and falls back to `"./"` on failure.
  `0x004ACC00` has only the three direct references inside this helper in the
  current IDA database.
- The committed source therefore declares both storage objects as unresolved
  `extern` dependencies instead of defining or assigning them to this source
  file. The reconstructed names `g_ExecutableDirectory` and `g_ResolvedPath`
  are source names only; no original target data-symbol names are claimed.
- The natural `strcpy` + `strcat` implementation under the existing exact-
  producing VC7.1 `/O2 /Oy-` profile emits exactly 0x5A bytes. Canonical replay
  matches all 90 bytes with four DIR32 relocations: one to `0x004ACD08` and
  three to `0x004ACC00`.
- Callers at `0x0042C480`, exact `WriteDataToFile @ 0x0042C4E0`, and
  `0x0042C970` establish that the helper participates in multiple FileSystem
  paths. No caller is promoted by this result.
- Six `CC` bytes at `0x0042AE1A-0x0042AE1F` remain outside the reviewed extent
  with physical ownership unassigned. An IDA evidence comment at `0x0042ADC0`
  was written and read back through attested `th09-ida`; no target bytes or
  function names were modified.
- Factory imported the first five repository codegen-exact claims, but cold
  replay is currently unavailable: submitting the `0x0042C4E0` claim reports
  zero registered replay drivers. This is an acceptance-infrastructure boundary,
  not a codegen mismatch. No Factory Truth Kernel acceptance is claimed for any
  repository exact row.

The ninth reviewed packet closes the small file-presence helper at
`0x0042C480-0x0042C4DB`.

- Target-local ABI is the `/Gr` free-function shape: the path enters in ECX and
  no stack arguments are present. IDA's provisional `this` label is therefore
  not used as type evidence.
- The function acquires supervisor lock 2, evaluates `ResolvePath(path)` as the
  first argument of `CreateFileA`, requests `GENERIC_READ | FILE_SHARE_READ`
  with `OPEN_EXISTING` and `FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL`,
  then returns 1 after closing a valid handle or 0 on `INVALID_HANDLE_VALUE`.
- Committed TH08 source at HEAD
  `a45e99fb1942714e6edded20847e32a654d56f97` supplied the semantic-name
  hypothesis `FileSystem::CheckIfFileAlreadyExists`. TH09 independently differs
  by resolving the path inside the CreateFileA argument evaluation; the TH08
  body was not copied verbatim. TH095 committed source was also searched only
  as hypothesis material; its pre-existing dirty work was not consulted.
- The natural TH09-specific expression `CreateFileA(ResolvePath(path), ...)`
  under the existing exact-producing VC7.1 profile emits exactly 0x5C bytes.
  Canonical replay matches all 92 bytes with nine relocations to ResolvePath,
  supervisor storage/wrappers, CreateFileA, and CloseHandle.
- Four target callers (`0x00429662`, `0x0042D900`, `0x0042DB50`, and
  `0x004324C0` function bodies) use this candidate, but none is promoted by this
  result. Four `CC` bytes at `0x0042C4DC-0x0042C4DF` retain unknown physical
  ownership.
- An IDA evidence comment at `0x0042C480` was written and read back through the
  attested native provider. No target bytes or function names were modified.

The tenth reviewed packet closes the Supervisor lock-wrapper pair at
`0x0042B130-0x0042B187` while keeping the rest of the Supervisor layout opaque.

- `Supervisor::EnterCriticalSectionWrapper @ 0x0042B130-0x0042B157` and
  `Supervisor::LeaveCriticalSectionWrapper @ 0x0042B160-0x0042B187` are normal
  `__thiscall` members with ECX carrying `this`, one stack `int id`, and `ret 4`.
- TH09 target arithmetic establishes `CRITICAL_SECTION` storage at object offset
  `+0x6C0` with 0x18-byte stride. Five such slots end at `+0x738`, where the
  wrappers index a byte counter by the same id; Enter increments it after the
  Win32 call and Leave decrements it after the Win32 call. Direct target caller
  sites exercise ids 0, 2, 3, and 4; dynamic-id callers are not used to infer
  any wider array.
- Committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied the
  same wrapper source-shape hypothesis including lock counts. TH095 committed
  HEAD `8c79bc63280d4ea496e52cbf4ad6549f3e8a94d1` currently omits those count
  updates, so TH09 target instructions decide the result. TH08 remained clean;
  TH095 had unrelated pre-existing EclExtended/runtime scratch changes and only
  committed content was consulted.
- The maintained source uses a private `SupervisorLockLayout` overlay with an
  explicitly unknown prefix rather than claiming unrelated Supervisor fields.
  Both canonical units replay 40/40 target bytes twice under pinned VC7.1; each
  has one DIR32 relocation, to `EnterCriticalSection` or `LeaveCriticalSection`.
- Eight `CC` bytes at `0x0042B158-0x0042B15F` and another eight at
  `0x0042B188-0x0042B18F` remain outside the reviewed extents with physical
  ownership unassigned. The next IDA candidate begins at `0x0042B190`.
- IDA evidence comments at `0x0042B130` and `0x0042B160` were written and read
  back through the attested native provider. Function names were not rewritten;
  no target bytes were modified.
- The production build skeleton still validates as target-bound/open. Running the
  actual whole-build command returns rc 2 with compile flags, TU partition,
  libraries, resources, and link order explicitly unresolved; no whole-build or
  runtime closure is claimed from these exact units.

The eleventh reviewed packet closes the paired Chain insertion helpers at
`0x0042B1E0-0x0042B2FE`.

- `Chain::AddToCalcChain @ 0x0042B1E0-0x0042B26E` and
  `Chain::AddToDrawChain @ 0x0042B270-0x0042B2FE` are normal `__thiscall`
  members with a stack `ChainElem *` and 32-bit priority. Each returns the
  32-bit result from a pending added callback, or zero when none exists.
- TH09 target accesses establish the relevant 0x20-byte ChainElem layout:
  signed 16-bit priority `+0x0`, callback `+0x4`, added/deleted callbacks
  `+0x8/+0xC`, prev/next `+0x10/+0x14`, release-target storage `+0x18`, and
  callback argument `+0x1C`. Attested TH09 constructor/destructor candidates at
  `0x0042ABE0`/`0x0042AC00` independently corroborate these offsets but remain
  `unknown/review` and are not promoted by this packet.
- Both methods invoke and clear `addedCallback` before locking, then insert by
  ascending signed priority under exact Supervisor lock 0. AddToCalcChain uses
  the first embedded root; AddToDrawChain uses the second root at `this+0x20`.
- Committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied the
  Chain/ChainElem naming and source-shape hypothesis. TH095 committed HEAD
  `8c79bc63280d4ea496e52cbf4ad6549f3e8a94d1` has similar insertion code but
  separately mutates lock counts; those extra mutations conflict with TH09's
  exact wrapper semantics and were rejected. Only committed adjacent content
  was consulted.
- Natural TH09 source without `#pragma var_order` emits exactly 0x8F bytes for
  each method under the existing pinned VC7.1 profile. Canonical replay matches
  all 143 bytes for each function with four target-bound relocations to
  `g_Supervisor` and the exact Enter/Leave wrappers.
- `0x0042B26F` and `0x0042B2FF` are single-byte `CC` gaps outside the reviewed
  extents; physical ownership remains unknown. `0x0042B300` begins a separate
  candidate.
- IDA evidence comments at `0x0042B1E0` and `0x0042B270` were written and read
  back through attested `th09-ida` after transient Factory transport failures.
  No target bytes or function names were modified.

The twelfth reviewed packet closes the ChainElem lifecycle pair at
`0x0042ABE0-0x0042AC23`.

- `ChainElem::ChainElem @ 0x0042ABE0-0x0042ABFD` is a 30-byte `__thiscall`
  constructor. It clears bit 0 at `+0x2`, prev/next at `+0x10/+0x14`, callback
  fields at `+0x4/+0x8/+0xC`, sets releaseTarget at `+0x18` to `this`, and
  clears the signed 16-bit priority at `+0x0`.
- `ChainElem::~ChainElem @ 0x0042AC00-0x0042AC23` is a 36-byte `__thiscall`
  destructor. If the deleted callback at `+0xC` is non-null, it receives the
  argument at `+0x1C` through the `/Gr` callback ABI; the destructor then clears
  prev/next and callback/added/deleted callback fields.
- The natural lifecycle source matches the committed TH08 hypothesis but was
  validated entirely against TH09: both pinned-VC7.1 units replay twice with
  complete zero differences and no relocations. TH095 committed layout remains
  hypothesis-only and its unrelated dirty work was not consulted.
- The constructor is called by the still-unreviewed Chain constructor candidate
  at `0x0042AC30`, by `0x0042B300`, and by other TH09 allocation paths. The
  destructor is called by still-unreviewed `0x0042AC50` and Chain destructor
  candidate `0x0042B190`; none of those callers is promoted here.
- `0x0042ABFE-0x0042ABFF` and `0x0042AC24-0x0042AC2F` remain unowned `CC`
  gaps. IDA evidence comments at both lifecycle entries were written and read
  back through attested `th09-ida`; no target bytes or function names changed.

The thirteenth reviewed packet tests the Chain container special members at
`0x0042AC30` and `0x0042B190` but deliberately does not promote their origin.

- `0x0042AC30-0x0042AC41` has the exact 18-byte code shape of an empty
  `Chain::Chain()`: construct the first exact ChainElem, construct the second at
  `this+0x20`, return `this`. `0x0042B190-0x0042B1DA` has the exact 75-byte
  primary code shape of an empty `Chain::~Chain()`, destroying the second member
  then the first under VC7.1 EH registration.
- The destructor's non-contiguous compiler artifacts were reconciled instead of
  being silently ignored. The associated `.text$x` at `0x0048D340` replays
  18/18 bytes with relocations to exact `ChainElem::~ChainElem`, xdata at
  `0x0049DC10`, and `___CxxFrameHandler @ 0x0047B2E5`. The associated
  `.xdata$x` at `0x0049DC08` replays 36/36 bytes with relocations to the cleanup
  funclet and its own frame data. These are compiler EH artifacts, not separate
  authored-function extents; `.sxdata`/whole-link placement is not claimed.
- Source-shape probes explain why the constructor is unusually small. With the
  exact ChainElem definitions visible, `/O2 /Ob0 /Oy-` and `/O2 /Ob1 /Oy-`
  both emit the target 0x12-byte constructor and 0x4B-byte destructor. Tested
  `/O2 /Ob2 /Oy-` inlines the members and mismatches at 0x3A/0x72;
  `/O1 /Ob1 /Oy-` leaves the destructor short at 0x35. The configured candidate
  units use one exact-producing `/O2 /Ob1 /Oy-` profile without claiming it was
  the original TU-wide setting.
- TH08 committed HEAD `a45e99fb1942714e6edded20847e32a654d56f97` and TH095
  committed HEAD `f22604da05d5f6c91850be475568b5e8a8575e5b` both contain
  explicit empty Chain constructor/destructor definitions, which strengthens
  the source-shape hypothesis but cannot prove TH09 authorship. TH08 was clean;
  TH095 was ahead of origin and had unrelated untracked reconstruction/runtime
  files. Only committed adjacent content was consulted.
- Because stripped TH09 evidence cannot currently distinguish explicitly authored
  empty special members from implicitly compiler-generated ones, both candidates
  remain `origin=unknown, disposition=review`. Their natural source and replayable
  match units are present, but `matches.csv`, `implemented.csv`, canonical exact
  totals, and authored totals are intentionally unchanged.
- Fourteen `CC` bytes at `0x0042AC42-0x0042AC4F` and five at
  `0x0042B1DB-0x0042B1DF` remain physically unowned. IDA comments at both
  entries were written and read back; no target bytes or function names changed.

The next evidence-connected packet should inspect `0x0042AC50`, which directly
calls the exact ChainElem destructor and conditionally frees the object, and the
paired allocator-side candidate `0x0042B300` only if target-local evidence shows
that they form a bounded compiler-helper cohort. Keep origin classification
separate from codegen identity.

The fourteenth reviewed packet separates authored Chain creation helpers from a compiler-generated deleting destructor.

- `ChainElem::SetCallback @ 0x0041A889-0x0041A89D` is a normal `__thiscall` member. TH09 stores the supplied callback at `+0x4`, clears added/deleted callbacks at `+0x8/+0xC`, and returns with `ret 4`. The separate 4-byte candidate at `0x0041A89E` begins immediately, so no padding is assigned to SetCallback.
- `Chain::CreateElem @ 0x0042B300-0x0042B346` is authored game logic rather than a generic compiler helper: target-local code allocates 0x20 bytes, invokes the exact ChainElem constructor, passes the pointer through the release-build registry hook at `0x00401380` with the application literal `"funcChainInf"`, calls exact SetCallback, sets the heap-allocation bit at `+0x2`, and returns the node. A nine-byte `CC` gap through `0x0042B34F` remains physically unowned.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied the `SetCallback`/`CreateElem` naming and natural source-shape hypothesis. TH09 bytes, ABI, app-specific literal, callees, compiler output, and relocations independently decide both results. TH095 changed concurrently during this packet and is treated as volatile hypothesis material; no uncommitted TH095 content was used.
- SetCallback replays 21/21 bytes with no relocations under one exact-producing `/O1 /Ob1 /Oy-` profile. Tested `/O2`/`/Ox` variants preserve the 0x15-byte extent but select different instructions, so no global optimization claim follows.
- CreateElem replays 71/71 bytes with six relocations under a separate-TU probe in which SetCallback's definition is not visible and C++ EH is disabled with `/GX-`. With EH enabled the same natural source grows substantially; with SetCallback visible the no-EH body is four bytes shorter. These observations constrain this match unit only. In particular, the exact Chain destructor requires EH code generation, so `/GX-` is not promoted to a Chain-TU or project-wide fact.
- A normal C++ `delete ChainElem *` probe causes VC7.1 to emit the auxiliary COMDAT `??_GChainElem@@QAEPAXI@Z`. Its 33-byte body matches `0x0042AC50-0x0042AC70` across all 25 non-relocation bytes, and the two relocations resolve to the exact ordinary destructor and operator delete. The target candidate is therefore classified `compiler_generated` / `exclude`, not authored; no canonical exact row is claimed for it. Fifteen following `CC` bytes through `0x0042AC7F` remain physically unowned before the separate candidate at `0x0042AC80`.
- `scripts/compare-coff-function.py` now passes an explicit expected size through its already-guarded aux-less COFF-symbol path, allowing bounded comparison of compiler-generated COMDAT helpers without weakening target or section validation.

The next evidence-connected packet should inspect `0x0042B350`, one of the two direct callers of the scalar deleting destructor and the natural Chain removal seam. Reconcile it with the exact lock wrappers and ChainElem layout before deciding whether it is `Chain::CutImpl`, `Chain::Cut`, or another target-local routine.

The fifteenth reviewed packet closes the Chain node-removal seam at `0x0042B350` and its public wrapper at `0x0042C8C0`.

- `Chain::CutImpl @ 0x0042B350-0x0042B40B` is entered with Supervisor lock 0 already held. It searches the calc root followed by the draw root, requires a non-null `prev` before unlinking, clears the callback and links, then either deletes a heap-owned node or invokes its deleted callback. Both external actions occur with lock 0 temporarily released and are followed by reacquisition.
- All three TH09 callers satisfy that precondition: the small `0x0042C8C0` wrapper explicitly acquires/releases lock 0, while the run-chain paths at `0x0042C700` and `0x0042C7E0` hold lock 0 across traversal and call CutImpl after saving the next pointer. This target-local contract establishes the internal-helper role independently from adjacent names.
- `Chain::Cut @ 0x0042C8C0-0x0042C8ED` is exactly the public acquire-CutImpl-release wrapper. Two following `CC` bytes through `0x0042C8EF` remain physically unowned; four `CC` bytes at `0x0042B40C-0x0042B40F` similarly separate CutImpl from `0x0042B410`.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied the Cut/CutImpl naming and source-shape hypothesis. Its release source contains a `ZunMemory::RemoveFromRegistry` call before heap deletion, but the TH09 target has no observable registry call. The reconstruction omits an unobservable operation; whether original TH09 source contained an empty release-only inline cleanup remains unknown.
- Natural CutImpl source under `/O2 /Ob0 /Oy-` replays all 188 target bytes with nine relocations. `/Ob0` is compiler-sensitive here because it preserves the scalar deleting-destructor call; tested `/Ob1`/`/Ob2` expand destructor plus operator delete and produce 195-byte bodies. This is a match-unit fact, not a Chain-TU or project-wide inlining claim.
- The same source/TU and profile reproduce Cut at 46/46 bytes with five relocations.

The next evidence-connected packet should inspect the run-chain pair at `0x0042C700` and `0x0042C7E0`, which are the remaining direct callers of exact CutImpl and already expose callback return-state handling. Keep the two loops separate until TH09 proves their calc/draw ownership and exact source shape.

The sixteenth reviewed packet closes the calc/draw run-chain pair while correcting their physical owned extents beyond the IDA logical function bodies.

- `Chain::RunCalcChain` executes a 182-byte logical body at `0x0042C700-0x0042C7B5`. It acquires Supervisor lock 0, invokes each non-null callback with the lock released, reacquires it to interpret the result, removes result-0 nodes through exact CutImpl after first saving `next`, repeats result 2, maps results 3/4/5 to `1/0/-1`, and uniquely treats result 6 as a restart from the calc root.
- `Chain::RunDrawChain` executes a 186-byte body at `0x0042C7E0-0x0042C899` against the draw root at `this+0x20`. Results 0-5 mirror the calc path, but result 6 is outside its switch and therefore follows the default continue/count path rather than restarting.
- Initial compile feedback appeared to make `/O2` too large (`0xD4` versus IDA sizes `0xB6/0xBA`). Physical reconciliation showed that VC7.1 includes each switch table in the function COMDAT: calc owns an exact tail `0x0042C7B6-0x0042C7D3` (two-byte alignment plus seven 32-bit case targets), and draw owns `0x0042C89A-0x0042C8B3` (two-byte alignment plus six targets). The table-base xref for each tail comes only from its corresponding Run function.
- The canonical `/O2 /Ob1 /Oy-` units therefore compare the complete 0xD4 owned extents. Calc replays 212/212 bytes with 21 relocations; draw replays 212/212 with 20. Canonical authored-byte progress still counts only the logical C++ function bodies (182 and 186 bytes); the alignment/table tails are associated compiler-generated codegen, analogous to already separated compiler artifacts.
- The true padding gaps are `0x0042C7D4-0x0042C7DF` and `0x0042C8B4-0x0042C8BF`, twelve `CC` bytes each, and retain unknown physical ownership.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied the RunCalcChain/RunDrawChain naming and callback-result source-shape hypothesis. TH09 independently establishes root offsets, lock behavior, result handling, CutImpl calls, code bytes, COMDAT sizes, and switch-table targets. No volatile TH095 content was used.

The next evidence-connected packet should inspect `0x0042C8F0`, immediately following the now-exact run/cut cohort, before broadening. Determine whether its `0x78` body is a Chain release helper or an unrelated seam from TH09-local callers/callees and only then select source hypotheses.

The seventeenth reviewed packet establishes that the immediate post-Chain candidate is a separate Controller/WinMM startup helper, not Chain release logic.

- `Controller::GetJoystickCaps @ 0x0042C8F0-0x0042C967` has one TH09 caller, `_WinMain@16 +0x178`. It initializes a 0x34-byte `JOYINFOEX` with `JOY_RETURNALL`, probes joystick 0 and only if that fails probes joystick 1. If both probes fail it calls exact `GameErrorContext::Log` with external message storage at `0x0048FB94` and returns 1. Otherwise it queries capabilities for both IDs and returns 0.
- Two capability destinations at `0x004AC8D8` and `0x004ACA6C` are exactly 0x194 bytes apart. The exact object expresses them as one `JOYCAPSA[2]` relocation base plus addend `0x194`, independently establishing the contiguous function-level layout. The committed source leaves the array definition, GameErrorContext object definition, and message data as extern dependencies because their TU/physical ownership is unresolved.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied the `Controller::GetJoystickCaps` naming and single-joystick source-shape hypothesis. TH09 independently differs by checking both joystick IDs and filling two JOYCAPSA records; TH08 source was not copied verbatim. No volatile TH095 content was used.
- Canonical pinned-VC7.1 replay matches all 120 bytes with seven relocations. Tested `/O2 /Ob0`, `/O2 /Ob1`, and `/Ox /Ob1` variants are all exact, while `/O1 /Ob1` emits only 0x71 bytes; no project-wide profile claim follows.
- Eight `CC` bytes at `0x0042C968-0x0042C96F` remain outside the reviewed function with physical ownership unassigned. The native provider does not expose `read_string` in its allowlist, so the external message contents were not inferred from remembered or adjacent output.

The next evidence-connected packet should inspect `0x0042C970`, which directly follows this Controller helper and already references the same GameErrorContext object. Determine from TH09-local Win32/DirectInput calls whether it is another Controller routine before consulting adjacent source.

The eighteenth reviewed packet closes the high-fanout FileSystem read entry at `0x0042C970-0x0042CAD2`.

- `FileSystem::OpenFile` is a `/Gr` free function: path enters in ECX, the optional output-size pointer in EDX, and `isExternalResource` is the sole stack argument; every return executes `ret 4`. The function holds Supervisor lock 2 across both read modes and releases it on all success/error exits.
- Archive mode strips the final backslash and then slash component using two CRT `strrchr` calls, queries the archive object at `0x004AD008`, optionally stores decompressed size, calls exact `GameErrorContext::Fatal` when the entry is absent, allocates through the ZunMemory object, reads/decompresses the entry, then calls the common post-process. TH09 target contains no debug-print calls present in the committed TH08 hypothesis.
- External-resource mode first calls exact `FileSystem::ResolvePath`, opens with `GENERIC_READ | FILE_SHARE_READ | OPEN_EXISTING | FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL`, allocates using the resolved path as debug text, reads and optionally reports the byte count, closes the handle, and reaches the same post-process.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied the `FileSystem::OpenFile` naming and broad branch/source-shape hypothesis. TH09 independently establishes the `/Gr` ABI, removal of debug logging, ResolvePath use, branch order, exact call destinations, error flow, and final bytes. No volatile TH095 content was used.
- The natural TH09-adjusted source under `/O2 /Ob1 /Oy-` replays 355/355 bytes with 28 relocations. `/O2 /Ob0` and `/Ox /Ob1` are also exact, while `/O1 /Ob1` emits 0x12F bytes; no project-wide profile claim follows.
- Four direct dependencies remain declaration-only and unpromoted: `ZunMemory::Alloc @ 0x00401340`, `FileSystem::TryDecryptFromTable @ 0x0042C290`, `PbgArchive::ReadDecompressEntry @ 0x004331C0`, and `PbgArchive::GetEntryDecompressedSize @ 0x00433290`. The exact call sites constrain their ABI but do not establish implementation, source presence, origin, or exactness.
- Thirteen `CC` bytes at `0x0042CAD3-0x0042CADF` remain outside the reviewed function with physical ownership unassigned.

The next evidence-connected packet should inspect the common post-process `0x0042C290`, because exact OpenFile calls it from both archive and external modes with `(data, fileSize, size)` and its current decompiler omits the unused EDX parameter. Recover its signature/table dependencies before attempting its decrypt source.

The nineteenth reviewed packet closes the lower-level FileSystem decrypt transform while deliberately leaving its table-driven wrapper unpromoted.

- `FileSystem::Decrypt @ 0x0042C180-0x0042C282` is a `/Gr` free function with `inData` in ECX, `size` in EDX, and `xorValue`, `xorValueInc`, `chunkSize`, and `maxBytes` on the stack; all returns use `ret 0x10`. The target allocates an output buffer through the ZunMemory object, reverses alternating output positions within each bounded chunk while advancing the XOR byte, copies any undecrypted tail, and returns either the new allocation or the original input when allocation fails.
- The target-only allocation debug string is `"./system\\global.h"` at `0x0048E474`. Natural maintained source replays 259/259 bytes with three relocations under the configured `/O2 /Ob1 /Oy-` profile; `/O2 /Ob0` and `/Ox /Ob1` are also exact, while `/O1` emits 244 bytes and `/Od` 446 bytes. No project-wide optimization claim follows.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied the broad Decrypt algorithm hypothesis. TH09 independently establishes the ABI, exact control flow, allocator call, string, and output behavior. The maintained TH09 source omits TH08's unused reconstruction local because a focused probe proves its removal leaves the target-exact optimized body unchanged.
- The adjacent crypt data is explicitly not copied. TH09 has eight 12-byte records at `0x004A1E60` followed by signature bytes `85 A4 DA` at `0x004A1EC0`; current committed TH08 differs in at least its third and fourth record values. TH09 wins, and the TH09 data definition/TU owner remains unknown.
- `TryDecryptFromTable @ 0x0042C290-0x0042C36B` is now boundary- and ABI-reviewed but remains `unknown/review`. A natural source with the correct TH09 table view, call to exact Decrypt, and ZunMemory free path produces the same 220-byte extent under `/O2`, but its optimized register allocation differs. `/GB` and `/G6` do not change that mismatch, `/G7` emits 222 bytes, `/O1` is shorter, and `/Od` is longer. The repository compiler invocation does not load TH08's nonstandard `var_order` C1XX wrapper; no such wrapper was introduced merely to force equality.
- Thirteen `CC` bytes at `0x0042C283-0x0042C28F` and four at `0x0042C36C-0x0042C36F` remain outside the reviewed logical functions with physical ownership unassigned.

The next evidence-connected packet should inspect `FileSystem::Encrypt @ 0x0042C370` and its relationship to exact Decrypt before returning to the unresolved TryDecryptFromTable register-allocation shape. This can establish whether the encrypt/decrypt pair shares a natural optimized source/TU profile without importing TH08 data ownership.

The twentieth reviewed packet closes the inverse FileSystem encryption transform at `0x0042C370`.

- `FileSystem::Encrypt @ 0x0042C370-0x0042C47A` has the same `/Gr` six-argument ABI as exact Decrypt: ECX carries the input pointer, EDX the size, and four encryption/chunk parameters are stack arguments consumed by `ret 0x10`. Its transform is the natural inverse: each bounded chunk is read from alternating backward positions and written sequentially while the XOR value advances; any unencrypted tail is copied unchanged.
- The only target callee is the ZunMemory allocator at `0x00401340`. The same `"./system\\global.h"` literal at `0x0048E474` is passed as allocation debug text. One TH09 caller at `0x004221C6` uses the candidate; the caller's broader ownership remains outside this packet.
- Natural maintained source replays 267/267 bytes with three relocations under `/O2 /Ob1 /Oy-`; `/O2 /Ob0` and `/Ox /Ob1` are also exact, while `/O1` produces 256 bytes and `/Od` 439. Exact Encrypt and Decrypt therefore share an observed optimized profile family, but original TU co-location and global flags remain unknown.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied the broad inverse-transform source hypothesis only. TH09 independently establishes the ABI, boundary, allocator/string dependencies, and exact codegen.
- Five `CC` bytes at `0x0042C47B-0x0042C47F` remain outside Encrypt with physical ownership unassigned before exact CheckIfFileAlreadyExists.

The next evidence-connected packet should inspect the release-build ZunMemory wrapper cohort at `0x00401340`, `0x00401360`, and `0x00401380`. Exact OpenFile/Decrypt/Encrypt call the allocator/free wrappers, while exact Chain::CreateElem calls the registry wrapper, so their extents, release semantics, origins, and natural VC7.1 shapes can be evaluated together without broadening into unrelated memory code.

The twenty-first reviewed packet closes the release-build ZunMemory wrapper cohort at `0x00401340`, `0x00401360`, and `0x00401380`.

- `ZunMemory::Alloc @ 0x00401340-0x00401352` is a 19-byte release façade over CRT `malloc`. Target callers place the shared ZunMemory object in ECX and pass `size` plus debug text on the stack; the body uses only size and returns with `ret 8`. `ZunMemory::Free @ 0x00401360-0x00401372` analogously forwards its sole pointer to CRT `free` and returns with `ret 4`.
- `ZunMemory::AddToRegistry @ 0x00401380-0x00401389` is the release no-op registry path: it returns the first explicit `ptr` argument and consumes `(ptr, size, name)` with `ret 0x0C`. Exact Chain::CreateElem independently supplies the app-specific `"funcChainInf"` label, tying this façade to game code rather than CRT implementation. Nothing here reconstructs or claims a DEBUG registry implementation.
- Natural bodies replay 19/19, 19/19, and 10/10 bytes under `/O2 /Ob1 /Oy-`; `/O2 /Ob0` and `/Ox /Ob1` are also exact. `/O1` shortens Alloc/Free to 16 bytes while AddToRegistry remains 10/10, and `/Od` lengthens all three. No global profile claim follows.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied the ZunMemory API/name hypothesis. TH09 independently establishes each release body, ABI, CRT destination, broad caller use of `g_ZunMemory`, and the registry passthrough. The maintained reconstruction deliberately places these definitions out of line to avoid changing already-exact caller inlining. Original TH09 inline/header form, emitting translation unit, and source ownership remain unknown.
- Thirteen-byte `CC` gaps at `0x00401353-0x0040135F` and `0x00401373-0x0040137F`, plus six bytes at `0x0040138A-0x0040138F`, remain physically unassigned.

The next evidence-connected packet should inspect `PbgArchive::GetEntryDecompressedSize @ 0x00433290`, an ABI-only dependency already called by exact FileSystem::OpenFile. Reconcile its 29-byte extent and the entry-lookup callee before deciding whether the simple archive query can be promoted independently of the larger decompression path.

The twenty-second reviewed packet closes the archive entry lookup and decompressed-size query while correcting the prior accessor extent estimate.

- `PbgArchive::FindEntry @ 0x00433050-0x00433099` is a 74-byte `__thiscall` search. TH09 itself proves the state prefix used by this routine: entry-array pointer at `this+0`, signed entry count at `this+4`, 0x10-byte record stride, and filename pointer at record `+0`. It returns the first case-insensitive `_stricmp` match or NULL.
- `PbgArchive::GetEntryDecompressedSize @ 0x00433290-0x004332AC` is 29 bytes, not the earlier handoff's 27-byte estimate. It preserves `this`, calls FindEntry with the filename, and returns the record DWORD at `+0x8` or zero. Exact FileSystem::OpenFile is its sole current target caller.
- The maintained implementation deliberately uses a TU-local `PbgArchiveStatePrefix` instead of declaring a complete PbgArchive object layout. The 0x10-byte `PbgArchiveEntry` view names only filename and decompressedSize; the `+4` and `+0xC` fields stay generic. No total class size, backend type, data-definition ownership, or original TU is claimed.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied `FindEntry`/`GetEntryDecompressedSize` naming and broad source-shape hypotheses. TH09 independently establishes the state prefix, record stride/field offset, branch behavior, `_stricmp` destination, and both exact bodies.
- Natural source replays 74/74 and 29/29 bytes under `/O2 /Ob1 /Oy-`; `/O2 /Ob0` and `/Ox /Ob1` are also exact, while `/O1` and `/Od` produce different sizes. No project-wide flag claim follows.
- Six `CC` bytes at `0x0043309A-0x0043309F` and three at `0x004332AD-0x004332AF` remain physically unassigned.

The next evidence-connected packet should inspect `PbgArchive::ReadDecompressEntry @ 0x004331C0`, already called by exact FileSystem::OpenFile and now connected to exact FindEntry. Recover only the file-backend ABI, entry offsets, temporary allocation/free path, and LZSS call needed for this 193-byte routine; keep backend implementation and full archive layout independent.

The twenty-third reviewed packet closes `PbgArchive::ReadDecompressEntry @ 0x004331C0-0x00433280` without claiming the complete archive or file-backend layouts.

- TH09 extends the archive prefix needed by this routine to `archiveFilename @ this+8` and a polymorphic file backend at `this+0xC`. Combined with the exact query packet, the currently proven prefix is entries `+0`, entryCount `+4`, archiveFilename `+8`, backend `+0xC`; this is still not a total-class-size or source-ownership claim.
- The backend calls actually observed here are vtable Open at `+0`, Read at `+8`, and Seek at `+0x18`. The maintained TU uses `UnknownSlot*` declarations for the intervening virtual positions rather than importing TH08 names that TH09 has not independently established. Open receives the archive filename plus open-mode element 0; Seek receives entry `dataOffset` plus seek-mode element 0.
- Target data establishes open-mode storage at `0x004A1EC8` with its first pointer targeting `"r" @ 0x00490F1C`; seek-mode storage begins at `0x00490F08` with first value zero. The entry `+4` field is passed to Seek and `entry[1].dataOffset - entry.dataOffset` determines the temporary compressed byte count; entry `+8` supplies decompressed size.
- Temporary compressed storage uses `GlobalAlloc(0, compressedSize)`. Only after allocation do Seek/Read failures flow through `GlobalFree`; earlier file/entry/open/allocation failures return through a shared null block without freeing a NULL placeholder. This two-level error layout is required for the target CFG. Successful reads call `Lzss::Decode @ 0x00433AA0` with compressed data in ECX, compressed size in EDX, and `(outBuffer, decompressedSize)` on the stack, then GlobalFree the temporary data. At this packet Decode remained declaration-only/unpromoted; the later twenty-fourth packet establishes exact codegen while retaining unknown origin.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied API and broad decompression-flow hypotheses. Its current `compressedData = NULL` plus common error-cleanup shape compiles to 196 bytes under the tested TH09 profile, whereas the TH09-shaped two-level labels replay the target at 193/193 with seven relocations. TH09 source shape therefore wins.
- Natural TH09-shaped source is exact under `/O2 /Ob1 /Oy-`, `/O2 /Ob0 /Oy-`, and `/Ox /Ob1 /Oy-`; `/O1` produces 174 bytes and `/Od` 270. No project-wide compiler flag or original TU claim follows.
- Fifteen `CC` bytes at `0x00433281-0x0043328F` remain outside the reviewed function with physical ownership unassigned.

The next evidence-connected packet should inspect `Lzss::Decode @ 0x00433AA0`, now a direct declaration-only dependency of exact ReadDecompressEntry. Recover its true four-argument `/Gr` ABI, 0x2000-byte dictionary storage at `0x004CC430`, output-allocation behavior when `out == NULL`, and complete boundary before considering source exactness. The unresolved `FileSystem::TryDecryptFromTable @ 0x0042C290` register-allocation mismatch also remains a separate blocker and must not be promoted from semantic similarity.

The twenty-fourth reviewed packet establishes exact code generation for the LZSS decoder while deliberately leaving its origin and dictionary owner unresolved.

- `0x00433AA0-0x00433C75` has the target-exact source shape reconstructed as `Lzss::Decode`. The `/Gr` ABI carries compressed input in ECX and input size in EDX; `out` and `outSize` are stack arguments consumed by `ret 8`. If `out` is NULL, TH09 calls `GlobalAlloc(0, outSize)` and returns NULL on allocation failure.
- The decoder uses a 0x2000-byte circular dictionary at `0x004CC430` with a `0x1FFF` mask. It decodes MSB-first literal or 13-bit-offset/4-bit-length tokens, stops on zero offset, writes every output byte back into the ring, and drains residual bits through the normal fetch/checksum path. The checksum is maintained but has no returned observable.
- The ring is not assigned to this source unit. Target xrefs also reach `0x004CC430` from `0x00433CB0`, `0x00433E00`, and `0x00434020`, so data definition, source owner, object owner, and complete compression-state layout remain unknown. The maintained source declares only an external 8192-byte dictionary dependency.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` and committed TH095 HEAD `58ab994b157057a454bd236bb5b9bd3e692557bf` supplied the LZSS source-shape hypothesis. TH08 was clean. TH095 was ahead of origin with four unrelated/unknown untracked files that were preserved and not consulted. TH095's exact decoder uses target-specific identifier-bucket aliases; TH09 needs none of those techniques.
- Natural TH09 source replays 470/470 bytes with four relocations under `/O2 /Ob1 /Oy-`; tested `/O2 /Ob0`, `/O2 /Ob2`, and `/Ox /Ob1` are also exact, while `/O1` emits 436 bytes and `/Od` 852. A discarded 461-byte probe was traced to missing braces around a multi-statement macro in the trailing-bit loop; it produced a self-loop and was not retained as source evidence.
- Exact code generation does not settle origin. There is currently no TH09-local evidence distinguishing authored game code from incorporated third-party LZSS code, so `function-origins.csv` remains `unknown/review`, no canonical `matches.csv` or `implemented.csv` row is added, and authored exact totals remain 29 functions / 3,259 bytes.
- Ten `CC` bytes at `0x00433C76-0x00433C7F` remain outside the reviewed decoder extent with physical ownership unassigned.

The next evidence-connected packet should inspect `0x00433C80` and `0x00433CB0` as the small initialization cohort that also references the shared LZSS state. Reconcile their exact extents, the apparent 0x2001-node 12-byte tree view rooted around `0x004B4420`, and dictionary-clearing behavior before selecting names or source. Keep LZSS origin and all compression-state data ownership independent from codegen identity.

The twenty-fifth reviewed packet establishes exact code generation for the two small LZSS initialization helpers without promoting LZSS origin or state ownership.

- `0x00433C80-0x00433CA4` has the exact source shape reconstructed as `Lzss::InitTree(int root)`. Root arrives in ECX under `/Gr`; the routine stores it into the sentinel node's `right` field, sets the selected node's parent to 0x2000, and clears that node's left/right links. Four relocation addends all solve to one neutral tree base at `0x004B4420`.
- `0x00433CB0-0x00433CD9` has the exact source shape reconstructed as `Lzss::InitEncoderState()`. VC7.1 emits a `rep stosd` that clears exactly 0x2000 dictionary bytes at `0x004CC430`, followed by a 12-byte-stride loop clearing 0x2001 tree records. The comparison relocation `tree + 0x18010` lands exactly on the dictionary address, while the tree array itself occupies 0x1800C bytes; this geometry is target-observed but does not assign the four-byte intervening storage or either array definition to a source owner.
- Both helpers are called from `0x00434020`, the larger encoder-side candidate: InitEncoderState at `0x00434066` and InitTree at `0x004340AA`. This connects the helpers to the same compression subsystem but does not settle whether the LZSS implementation is game-authored or incorporated code. Both candidates remain `unknown/review` with source-present exact codegen only.
- Natural source replays 37/37 and 42/42 bytes under `/O2 /Ob1 /Oy-`; `/O2 /Ob0` and `/Ox /Ob1` are also exact. `/O1` changes InitTree while leaving InitEncoderState exact; `/Od` expands both.
- Eleven `CC` bytes at `0x00433CA5-0x00433CAF` and six at `0x00433CDA-0x00433CDF` remain physically unassigned. The bytes starting at `0x00433CE0` are code associated by native IDA with another function chunk and are explicitly not absorbed into the InitEncoderState gap.
- No `matches.csv`, `implemented.csv`, or authored-total changes follow from these codegen results. Canonical authored exact totals remain 29 functions / 3,259 bytes.

The next evidence-connected packet should map the tree-manipulation helpers around `0x00433CE0`, `0x00433D40`, `0x00433DC0`, `0x00433E00`, and `0x00433FC0` before source promotion. Reconcile IDA function chunks first, because `0x00433CE0` is already associated with the candidate named from `0x00433FC0`; then use callers and tree-field dataflow to distinguish ContractNode, DeleteString, FindNextNode, AddString, and ReplaceNode hypotheses.

The twenty-sixth reviewed packet reconstructs the LZSS tree-manipulation cohort and corrects one missed function boundary from the initial IDA inventory.

- `0x00433CE0-0x00433D32` is a distinct 83-byte routine reconstructed as `Lzss::ContractNode(oldNode, newNode)`. Native IDA had attached this range as a noncontiguous chunk of `sub_433FC0`, because DeleteString reaches it only by tail jump. Natural VC7.1 source compiled as a separate ContractNode symbol reproduces all 83 target bytes with eight tree relocations; separately compiled DeleteString has a REL32 tail-call relocation whose solved destination is exactly `0x00433CE0`. That combined codegen/relocation evidence overrides the initial chunk grouping.
- Because the imported inventory omitted `0x00433CE0` as a standalone start, `functions.csv` and `function-origins.csv` now add one boundary-corrected candidate. The tracked candidate denominator therefore rises from 2,158 to 2,159 and pending origin/boundary review from 2,121 to 2,122. This is inventory correction, not reconstruction regression or completion. ContractNode remains `origin=unknown, disposition=review` and is not a canonical authored match.
- `Lzss::ReplaceNode @ 0x00433D40-0x00433DB2` reassigns the old node's parent link, copies the complete 12-byte node record into the replacement, fixes both child parent links, and detaches the old node. `Lzss::FindNextNode @ 0x00433DC0-0x00433DF0` walks from `node.left` down repeated right links. Natural source replays 115/115 and 49/49 target bytes.
- `Lzss::AddString @ 0x00433E00-0x00433FB5` searches the tree rooted at the sentinel's right child, compares up to 18 circular dictionary bytes, tracks the best match through the caller's `int *`, replaces a complete match, or inserts a new leaf. VC7.1's optimized body is 438/438 exact with nineteen relocations, including the call to ReplaceNode.
- `Lzss::DeleteString @ 0x00433FC0-0x00434012` is independently 83/83 exact. It ignores nodes whose parent is zero; one-child paths tail-call ContractNode, while the two-child path calls exact-codegen FindNextNode, recursively calls DeleteString, and tail-calls ReplaceNode. Its logical/source extent does not include ContractNode despite the current IDA chunk association.
- All five helpers are exact under tested `/O2 /Ob0 /Oy-`, `/O2 /Ob1 /Oy-`, and `/Ox /Ob1 /Oy-`; `/O1` and `/Od` produce different sizes for every helper. The maintained source needs no byte embedding, assembly exception, artificial padding, or identifier-bucket trick.
- LZSS origin is still unresolved as a subsystem. ContractNode, ReplaceNode, FindNextNode, AddString, DeleteString, Decode, InitTree, and InitEncoderState therefore remain source-present/replayable codegen candidates outside `matches.csv`, `implemented.csv`, and authored exact totals. Canonical authored exact remains 29 functions / 3,259 bytes.
- Unowned CC gaps are 13 bytes at `0x00433D33-0x00433D3F`, 13 at `0x00433DB3-0x00433DBF`, 15 at `0x00433DF1-0x00433DFF`, 10 at `0x00433FB6-0x00433FBF`, and 13 at `0x00434013-0x0043401F`. Tree/dictionary definitions and source/TU ownership remain unknown.

The next evidence-connected packet should inspect `0x00434020-0x0043425A`, the 571-byte encoder-side caller of InitEncoderState, InitTree, AddString, and DeleteString. Recover its `/Gr` ABI, allocation/output contract, bit-packing state, checksum behavior, and full boundary first; use the now-exact-codegen helper call graph to test the natural `Lzss::Encode` hypothesis without promoting LZSS origin.

The twenty-seventh reviewed packet recovers the LZSS encoder semantics and maintained source, but stops short of exact codegen because the remaining compiler shape is not justified by target-local evidence.

- `0x00434020-0x0043425A` is a `/Gr` candidate reconstructed as `Lzss::Encode(unsigned char *in, int inSize, int *outSize)`. TH09 callers pass input in ECX, size in EDX, and an output-size pointer on the stack; the target consumes that pointer with `ret 4`. It allocates `inSize * 2` bytes through `GlobalAlloc(0, ...)`, returns NULL on allocation failure, zeroes `*outSize`, and invokes the now replayable InitEncoderState, InitTree, DeleteString, and AddString helper cohort.
- The target preloads up to 18 bytes into the dictionary from index 1, then emits MSB-first bit fields. Match lengths of two bytes or less become literal tokens (`1` plus eight data bits); longer matches become `0` plus a 13-bit offset and four-bit `(length - 3)`. The sliding dictionary/tree window advances through DeleteString/AddString, and the stream terminates with `0` plus thirteen zero offset bits. The final byte count is `outCursor - out` stored through `outSize`.
- Stable committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied the broad Encode source-shape hypothesis. The maintained TH09 source removes the adjacent dead checksum accumulation because a focused probe shows it has no target-observable effect and no codegen effect. Target callers/callees, ABI, dictionary accesses, token packing, allocation path, and final size write independently establish the maintained semantics.
- Exact code generation remains blocked. Natural stock-VC7.1 source is 570 bytes under `/O2 /Ob0`, `/O2 /Ob1`, `/O2 /Ob2`, and `/Ox /Ob1`, while the target is 571. `/O1` emits 469 and `/Od` 965. The optimized relocation graph contains the expected eight dependencies, but register assignment differs broadly: the candidate keeps `outBits` in EBX and spills `outBitMask`, while the target keeps `outBitMask` in BL and spills the 32-bit accumulator at `[ebp-4]`; input-size and cursor homes differ correspondingly.
- Reordering actual C++ declarations into the order listed by TH08's `#pragma var_order` does not alter stock VC7.1 output. Inspection of committed TH08 tooling confirms that `var_order` is implemented by a reconstruction-specific wrapper that replaces `C1XX.DLL`; it is not an observed original TH09 compiler feature. No such wrapper, identifier-bucket search, artificial volatile, padding, inline assembly, or register-directed source is introduced here. The candidate remains non-exact and no match unit is configured.
- LZSS origin remains `unknown/review`; maintained source presence does not change canonical authored totals. Five `CC` bytes at `0x0043425B-0x0043425F` remain physically unassigned.

This is a concrete source/compiler-evidence boundary for the LZSS packet. Revisit Encode only if new TH09-local source-identifier, TU, or compiler-front-end evidence appears. Otherwise the next bounded seam can begin at `0x00434260`/`0x00434280`, immediately after the LZSS encoder, first classifying whether those short candidates belong to the same subsystem before selecting any adjacent-game hypothesis. The independent `FileSystem::TryDecryptFromTable @ 0x0042C290` optimized-register-allocation blocker also remains unresolved.

The twenty-eighth reviewed packet classifies and canonically reconstructs the two short AsciiManager lifecycle helpers immediately after the LZSS encoder.

- `AsciiManager::DeletedCallback @ 0x00434260-0x0043427C` is the calc ChainElem deleted callback. TH09 registration at `0x00435CA0` stores this address in the deleted-callback field of the ChainElem at `0x004DC504`; the function releases AnmManager slots 1 and 3 through `g_AnmManager @ 0x004DC550` and returns zero. The TH09 added callback at `0x004344E0` independently binds slot 1 to `"ascii.anm"` and slot 3 to `"capture.anm"`, closing the resource identity without relying on adjacent addresses.
- `AsciiManager::CutChain @ 0x00434280-0x0043429E` cuts the calc ChainElem at `0x004DC504` and low-priority draw ChainElem at `0x004CE438` through exact `Chain::Cut @ 0x0042C8C0`; it intentionally has no reference to the high-priority draw ChainElem at `0x004DC524`. The broad shutdown routine at `0x00431B70` calls this helper after other Ascii/ANM cleanup.
- TH09 target layout further constrains the AsciiManager state: all three registered ChainElem `arg` fields point to `0x004CE458`; the added callback clears exactly `0xE0AC` bytes there; and `0x004CE458 + 0xE0AC == 0x004DC504`, immediately reaching the calc ChainElem. Registration uses calc priority 1 and draw priorities 34 and 23. Complete field layout, definitions, translation-unit partition, and static-data physical ownership remain unknown.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` contains the analogous `AsciiManager::DeletedCallback` and `AsciiManager::CutChain`, with resource slots 1/3 and the same omission of the high-priority draw chain in CutChain. It was used only after TH09 established the callback/resource seam, as naming/source-shape corroboration. TH08 priorities differ, so TH09 target values remain authoritative. No TH095 uncommitted content was consulted.
- Natural maintained source replays DeletedCallback 29/29 bytes with four relocations and CutChain 31/31 with six relocations, twice from the tracked source. Tested `/O2 /Ob0`, `/O2 /Ob1`, `/O2 /Ob2`, and `/Ox /Ob1` are exact for both; `/O1` makes CutChain 32 bytes and `/Od` expands both. This does not establish project-wide flags or original TU colocation.
- Both functions are classified `authored_game/AsciiManager` independently from codegen because they are directly wired into the game-owned Chain scheduler and TH09-specific `ascii.anm`/`capture.anm` resource lifecycle; after repeatable exact replay they are canonical. Authored exact totals rise from 29 functions / 3,259 bytes to 31 functions / 3,319 bytes. Factory Truth Kernel acceptance is still a separate state.
- Three `CC` bytes at `0x0043427D-0x0043427F` and one `CC` byte at `0x0043429F` remain physically unassigned. Shared IDA comments at both function entries record exact replay, lifecycle identity, and the fact that reconstructed names are not original-symbol claims.

The next evidence-connected packet should inspect `0x004342A0-0x00434321` and its immediate caller/callee/data neighborhood. It is adjacent to AsciiManager lifecycle code but must not inherit AsciiManager ownership merely by address. First reconcile its complete boundary, ABI, callers, strings/globals, and scheduler/resource links; only then consult committed adjacent source if TH09 evidence points to a recognizable AsciiManager method. Preserve the LZSS Encode and FileSystem::TryDecryptFromTable register-allocation blockers as independent open issues.

The twenty-ninth reviewed packet canonically reconstructs the AsciiManager text queue append and formatted-text wrapper.

- `AsciiManager::AddString @ 0x004342A0-0x00434321` is a normal non-variadic member (`this` in ECX, position/string on the stack, `ret 8`). It bounds the queue at 256 entries and indexes a 0x60-byte record array rooted at `this+0x2264`; the algebraic end of that array is exactly `this+0x8264`, the queue count. Records hold 64 bytes of text, three position dwords at `+0x40`, color/scales at `+0x4C/+0x50/+0x54`, selected state at `+0x58`, and GUI state at `+0x5C`. Current manager values come from `+0x8268/+0x826C/+0x8270/+0x8274`. TH09 unconditionally writes selected state zero.
- `AsciiManager::AddFormatText @ 0x00434330-0x00434366` is a variadic member and therefore lowers with `this` at stack `+8`, position at `+0xC`, and format string at `+0x10`. It passes the varargs to `vsprintf @ 0x0047C335` using a 512-byte local buffer, then calls exact AddString. Native IDA finds 35 call sites across gameplay/HUD code.
- Stable clean TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was consulted only after TH09 established the queue layout and ABI. It corroborates the AddString/AddFormatText names and 0x60 record shape, but its AddString conditionally propagates selected state based on software texturing. TH09 has no such branch and instead always clears the record selected field, so TH09 target behavior is authoritative. No TH095 uncommitted content was consulted.
- The maintained `AsciiManager` declaration exposes only target-proven layout: unknown bytes to `+0x2264`, the 256-entry 0x60 queue, count/style fields through `+0x8274`, and an unknown tail to the lifecycle-proven object extent `0xE0AC`. Those unknown byte ranges are not constituent-field ownership claims.
- Natural tracked source replays AddString 130/130 bytes with zero relocations and AddFormatText 55/55 with two relocations, twice. Tested `/O2 /Ob0`, `/O2 /Ob1`, `/O2 /Ob2`, and `/Ox /Ob1` are exact for both; `/O1` and `/Od` differ. No `var_order` wrapper is needed. Both functions are canonical `authored_game/AsciiManager`. Totals rise from 31 functions / 3,319 bytes to 33 functions / 3,504 bytes.
- Fourteen `CC` bytes at `0x00434322-0x0043432F` and nine at `0x00434367-0x0043436F` remain physically unassigned. Shared IDA comments at both entries record the reconstructed identities and exact replay without claiming original symbols.

The next evidence-connected packet should inspect `0x00434370` and the immediately following AsciiManager-adjacent candidates, beginning with target-local callers/field accesses rather than inheriting ownership by address.

The thirtieth reviewed packet canonically reconstructs two AsciiManager setup helpers while deliberately leaving neighboring generic candidates and Reset unpromoted.

- `AsciiManager::SetSpaceWidth @ 0x004343C0-0x004343CF` stores its sole stack argument at manager `+0x8280`; `Reset @ 0x004343E0` calls it with TH09 value 9. It has no relocation and no owned padding after the function; `0x004343D0` is a separate candidate.
- `AsciiManager::AddedCallback @ 0x004344E0-0x00434538` clears the lifecycle-proven 0xE0AC-byte manager state, preloads `"ascii.anm"` slot 1 into `+0x8288` and `"capture.anm"` slot 3 into `+0x828C`, returns -1 on either null result, otherwise calls Reset and returns zero. Seven relocations bind both target strings, `g_AnmManager`, candidate `0x0043C7A0`, and Reset. Registration independently installs this function as the calc ChainElem added callback.
- `0x0043C7A0` behaves like an asynchronous ANM preload wrapper and the exact caller compiles naturally when declared `AnmManager::PreloadAnm(int, const char *)`, matching clean committed TH08 naming. This is still only a callee hypothesis: its own extent, origin, source, and exactness were not promoted.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` corroborates AddedCallback's two preload operations and failure behavior, but TH09 target code calls only Reset on success and does not call TH08's separate InitializeVms. TH09 target control flow wins. No TH095 uncommitted content was consulted.
- Natural tracked source replays SetSpaceWidth 16/16 and AddedCallback 89/89 twice without disturbing the four previously exact AsciiManager functions. Tested optimized O2/Ox profiles are exact for both; `/O1` shrinks AddedCallback to 88 bytes and `/Od` expands both. Both are canonical `authored_game/AsciiManager`, taking totals from 33 functions / 3,504 bytes to 35 functions / 3,609 bytes.
- Shared IDA comments at both entries record the canonical replay and limitations. `0x00434539-0x0043453F` remains seven bytes of unassigned `CC` padding.
- The candidates at `0x00434370`, `0x00434380`, `0x00434390`, `0x004343B0`, and `0x004343D0` were not assigned to AsciiManager by address. In particular, `0x00434370/80` operate on an object flag at `+0x1F8`; Reset's second and third 0x2A4-byte VM blocks place the same offset at manager `+0x49C/+0x740`, supporting a generic VM-method hypothesis rather than AsciiManager ownership. They remain `unknown/review`.

The next evidence-connected packet is `AsciiManager::Reset @ 0x004343E0-0x004344DA`. Its TH09-local state ranges, constants, callers, SetSpaceWidth call, and two calls to candidate `0x00412800` are now bounded; recover only the fields/source shape required for a natural VC7.1 replay, retaining unknown array/subobject ownership where the target does not decide it.

The thirty-first reviewed packet canonically reconstructs `AsciiManager::Reset @ 0x004343E0-0x004344DA` without assigning unobserved manager regions to adjacent-game types.

- Reset clears exactly three 0x2A4-byte VM records at manager offsets `+0`, `+0x2A4`, and `+0x548`; the 0x6000-byte text queue at `+0x2264`; and three additional opaque spans at `+0x82A0` size 0x1528, `+0x97C8` size 0x0FE0, and `+0xB240` size 0x15E0. The intervals not touched by target code remain opaque in the maintained layout.
- It clears the observed queue count, GUI/selected state, fields `+0x8290/+0x8294/+0x829C`, sets color to `0xFFFFFFFF` and both scales to 1.0f, while target field `+0x8298` is not written and therefore remains explicitly unknown.
- The second and third VM flag words at manager `+0x49C/+0x740` receive anchor value 3 (`|=0x1800`); the VM-local flag word offset is `+0x1F8`. The second VM receives sprite 97 and the first sprite 32 through candidate `0x00412800`; second-VM `pos.z` at manager `+0x4B4` is set to 0.1f. Reset finishes with exact `SetSpaceWidth(9)`.
- `0x00412800` independently has ECX as a loaded-ANM object, VM pointer and sprite index as two stack arguments, and `ret 8`. Declaring it `AnmLoaded::InitializeAndSetSprite(AnmVm *, int)` produces both exact Reset relocations and matches committed TH08 naming, but this remains only a callee hypothesis. No origin, source, or exactness was promoted for `0x00412800`.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` corroborates the first-three-VM roles and the VM anchor/position field names after TH09 fixed their sizes and offsets. TH09 values differ materially: sprite 97 replaces TH08's 136 popup setup path, the target initializes only the observed two VMs here, and TH09 default space width is 9 rather than 13. Target behavior is authoritative. No TH095 uncommitted content was consulted.
- Natural maintained source replays all 251 bytes with three relocations twice; it remains exact under the tested O2/Ox family, while `/O1` emits 226 and `/Od` 379 bytes. No compiler wrapper, padding, target bytes, assembly, volatile trick, or register-directed source is used. Reset is canonical `authored_game/AsciiManager`, taking totals from 35 functions / 3,609 bytes to 36 functions / 3,860 bytes.
- The maintained layout deliberately leaves manager `+0x7EC-0x2264`, the three later memset region types, other gaps, and `+0x8298` semantically unknown. Shared IDA comment at Reset records the same constraints. `0x004344DB-0x004344DF` is five bytes of physically unassigned `CC`.

The next evidence-connected packet should inspect `0x00434540-0x00434694` and its immediate callers/callees/field accesses. It is adjacent to Reset/AddedCallback but must earn AsciiManager ownership from TH09-local object and call evidence before any adjacent-game name is adopted.

The thirty-second reviewed packet canonically reconstructs `AsciiManager::DrawStrings @ 0x00434540-0x00434694` and adds only the VM/render declarations required by target-local evidence.

- TH09 callers independently establish AsciiManager ownership: `0x00431540` first queues `"Press Shot Button"` through exact AddFormatText on `g_AsciiManager @ 0x004CE458` and then invokes `0x00434540`; `0x00435C00` calls the same method while operating the same manager-sized state. The method directly consumes the already exact queue/count/spaceWidth/asciiAnm offsets.
- The loop sets `largeText` visible plus anchor 3, copies queue position and scale, computes `spaceWidth * scaleX`, flushes and selects gameplay viewport mode 0 for GUI records or 2 for non-GUI records when mode changes, and restores mode 2 at exit when necessary. Newline increments Y by TH09-specific 14.0f and resets X; space advances X only; other unsigned bytes use sprite index `byte - 0x20`, copy queue color, draw the VM, then advance X.
- DrawStrings proves only the extra AnmVm fields it touches: two-float scale at `+0x18`, 32-bit color at `+0x1F0`, visible bit 0 in the existing `+0x1F8` flag word, and loaded-sprite pointer at `+0x224`. Other VM bytes remain opaque.
- Four callee declarations are source hypotheses backed by TH09 behavior and exact caller relocations, not callee exactness: `Supervisor::ConfigureGameplayViewport(int) @ 0x00401390`, `AnmLoaded::GetSprite(int) @ 0x00434390`, `AnmManager::FlushVertexBuffer() @ 0x004396A0`, and `AnmManager::DrawNoRotation(AnmVm *) @ 0x0043A950`. All four ledger rows remain `unknown/review`. Shared IDA comments record the same distinction.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` corroborates the older low-priority text renderer shape only. Committed-only TH095 HEAD `6103aff975e3f416ccce318c16c4bcdd56f78747` corroborates the `DrawStrings` and `ConfigureGameplayViewport` naming and the unselected glyph path, but TH095 is still semantically provisional and its dirty worktree was not read. TH09 differs from both where observed: line height is 14.0f and glyph mapping is exactly unsigned byte minus 0x20.
- Natural source replays 341/341 bytes with twelve relocations and is exact under tested O2/Ox profiles; `/O1` emits 328 and `/Od` 500 bytes. No target bytes, padding, assembly, volatile/register trick, `var_order`, or identifier-bucket wrapper is used. DrawStrings is canonical `authored_game/AsciiManager`, taking totals from 36 functions / 3,860 bytes to 37 functions / 4,201 bytes.
- Adding the target-supported Supervisor member declaration changes no existing ChainRun code or solved relocation destination, but VC7.1 renumbers compiler-internal COFF `$L...` labels by +2. The calc/draw match manifests were updated only to the observed new local symbol names; both functions remain 212/212 exact, and all 27 configured Supervisor/Ascii header consumers replayed exact before DrawStrings promotion.
- `0x00434695-0x0043469F` is eleven bytes of physically unassigned `CC`.

The next evidence-connected packet should classify `0x004346A0` and its immediate candidate/caller/data neighborhood. It is address-adjacent to DrawStrings but must independently prove AsciiManager ownership and method role; do not transfer TH08/TH095 names until TH09-local state and call evidence agree.

The thirty-third reviewed packet canonically reconstructs `AsciiManager::CreateScorePopup @ 0x004346A0-0x00434739` while keeping its timer callee and unobserved manager tail independent.

- TH09 callers fix AsciiManager ownership and score-popup semantics. `0x0041D150` calls the method while updating/clamping score and derives the first argument from the active player-side state; the same state participates in nearby `1 - index` player selection. `0x0040F640` independently calls it for point-like values and colors.
- The target uses a shared ring index at manager `+0x8290`, wrapping at 100, and computes `(playerIndex * 100 + ringIndex) * 0x38` from base `+0xB240`. This establishes two 100-record player banks for observed callers. The 0x38 record contains text[12], Float3 position, color, a 0x0C timer, two-float scale, in-use byte, character-count byte, and otherwise unknown tail bytes. Manager bytes from `+0xDE00` onward remain opaque.
- Natural source encodes positive numbers as reversed decimal digits, zero as digit 0, and negatives as sentinel 10, then stores character count/color, executes `popup->timer = 0`, copies position, and increments the ring index. The exact Reset source was updated only from opaque storage to `scorePopups[0]`; it still clears exactly one 0x15E0 bank and remains 251/251 exact.
- `popup->timer = 0` produces the sole relocation to candidate `ZunTimer::operator=(int) @ 0x00401500`. That 9-byte wrapper tail-jumps candidate `0x004014D0`, whose stores support `SetCurrent(int)` and the 0x0C timer layout. Both timer candidates remain `unknown/review`; caller exactness does not promote them.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` corroborates the popup record and ZunTimer naming/source shape only. TH095 changed concurrently throughout this packet and remained dirty; only explicit committed `HEAD` reads were used, and no uncommitted TH095 content was consulted.
- The maintained CreateScorePopup source replays 154/154 bytes with one relocation twice alongside all eight prior AsciiManager units. O2/Ox profiles tested exact; O1/Od are size mismatches. It is canonical `authored_game/AsciiManager`, taking totals from 37 functions / 4,201 bytes to 38 functions / 4,355 bytes.
- Shared IDA comment at `0x004346A0` records the reconstruction and independent callee limitation. `0x0043473A-0x0043473F` remains six bytes of unassigned `CC`.

The next evidence-connected packet is the timer callee pair `0x004014D0` and `0x00401500`. Recover their exact extents, xrefs, origin, natural `ZunTimer::SetCurrent(int)` / assignment source shape, and compiler profile independently before converting the current declaration-only timer type into definitions.

The thirty-fourth reviewed packet canonically reconstructs `ZunTimer::SetCurrent @ 0x004014D0-0x004014E8` and `ZunTimer::operator=(int) @ 0x00401500-0x00401508`, while preserving the two intervening/adjacent timer candidates as unknown.

- TH09 itself fixes the SetCurrent source shape: current `+8` receives the integer argument, subFrame `+4` receives `(float)value`, and previous `+0` receives `-999999`. A first probe using TH08's `subFrame=0/-999` semantics emitted 26 bytes rather than the 25-byte target and was rejected.
- A committed-only TH095 ZunTimer at observed HEAD `b9937d8194520f645ff0b6f81140b78025b19988` proposed the TH09-compatible `subFrame=(float)value/-999999` form after target evidence already showed the differing stores. TH095's worktree remained dirty and uncommitted content was not read; this is corroboration only. Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` is explicitly conflicting evidence for this version and was not transferred.
- Natural maintained SetCurrent replays 25/25 with no relocation; natural assignment replays 9/9 with one REL32 tail jump to exact SetCurrent. The exact CreateScorePopup caller remains 154/154 with the now-defined assignment method. Two focused passes across all ZunTimer/AsciiManager header consumers stayed exact before promotion.
- Profile probes show SetCurrent remains exact under tested optimized O1/O2/Ox profiles. Assignment requires an out-of-line call shape: `/Ob0` and `/Ob1` reproduce the 9-byte wrapper, while `/O2 /Ob2` inlines SetCurrent to 25 bytes; `/Od` mismatches both. These are local facts, not project-wide flag claims.
- `0x004014F0-0x004014F3` is a separate candidate that only loads timer float field `+4`; `0x00401510-0x00401517` is another separate candidate calling `0x004014B0`. Both remain `unknown/review`. The three separating CC gaps remain physically unassigned.
- Shared IDA comments were written at SetCurrent and assignment with reconstructed identity and boundary limitations; target-byte metadata was never writable or requested. The two canonical functions take totals from 38 functions / 4,355 bytes to 40 functions / 4,389 bytes.

The next evidence-connected packet is the remaining small ZunTimer-adjacent cohort `0x004014B0`, `0x004014F0`, and `0x00401510`. Start from target-local xrefs and ABI rather than assuming TH095 method names; promote only the subset whose exact natural source and origin can be independently demonstrated.

The thirty-fifth reviewed packet canonically reconstructs the remaining small ZunTimer cohort: `Tick @ 0x004014B0-0x004014CB`, float conversion `0x004014F0-0x004014F3`, and postfix increment `0x00401510-0x00401517`.

- TH09 Tick copies current to previous, passes `&current` and `&subFrame` to `g_Supervisor` candidate `0x0042F4F0`, then returns current. Declaring that callee `Supervisor::TickTimer(int *, float *)` produces exact relocations to `g_Supervisor @ 0x004B3100` and `0x0042F4F0`. The callee itself remains `unknown/review`; exactness is not inherited.
- TH09 float conversion is exactly `return subFrame`: the entire function is `fld [ecx+4]; ret`. This explicitly rejects TH08's `current + subFrame` implementation. Committed-only TH095 HEAD `b9937d8194520f645ff0b6f81140b78025b19988` corroborates the subFrame-only conversion, but its dirty worktree was not read.
- Postfix increment is an eight-byte wrapper that calls Tick and returns with `ret 4` for the dummy postfix argument. Natural `void ZunTimer::operator++(int) { Tick(); }` replays exactly.
- Focused profile probes keep Tick 28 bytes under tested O2/Ox but emit 27 under O1; float conversion is stable under optimized profiles; `/O2 /Ob2` inlines postfix increment into 26 bytes while `/Ob0`/`/Ob1` keep the target 8-byte wrapper. These remain local profile facts only.
- Adding `Supervisor::TickTimer` to the shared class declaration renumbers compiler-internal ChainRun COFF local labels by +3 without changing any code byte, relocation offset, or solved target destination. After updating only those internal manifest names, both ChainRun functions replay 212/212 and all 25 affected Supervisor/ZunTimer consumers replay exact in two passes.
- Shared IDA comments at all three reconstructed timer entries record exactness/boundaries; a separate comment at `0x0042F4F0` records its provisional callee role. No target-byte patching was requested or possible. The three functions add 40 canonical authored bytes, taking totals from 40 functions / 4,389 bytes to 43 functions / 4,429 bytes.
- Physical gaps remain unowned: `0x004014CC-0x004014CF` (4 CC), `0x004014F4-0x004014FF` (12 CC), and `0x00401518-0x0040151F` (8 CC).

The next evidence-connected packet should inspect the adjacent small candidates beginning at `0x00401520` and `0x00401540`, which operate on the same timer object but must independently earn operator names and exactness from TH09-local ABI/xrefs before adjacent-game source is consulted. `Supervisor::TickTimer @ 0x0042F4F0` remains a separate larger callee packet.

The thirty-sixth reviewed packet recovers an interrupted dirty worktree and canonically reconstructs the next two ZunTimer comparison operators: `operator>= @ 0x00401520-0x00401533` and `operator< @ 0x00401540-0x00401553`.

- Session recovery found eleven pre-existing unstaged tracked paths at unchanged HEAD `0x0f611e876bf7f32d7290dfb8cceb0039c7768c72`. The complete diff was confined to these two comparison operators, their canonical ledgers/progress, and a correction marking the preliminary ZUNTIMER-001 TH08-shaped field interpretation as superseded. The committed handoff already named `0x00401520/0x00401540` as the next packet, so the paths were classified `recoverable-current-work` and finished before any unrelated edits.
- TH09 target code independently fixes both operations and ABI. Each is a 20-byte `__thiscall` member with one stack `int` consumed by `ret 4`; both load signed `current @ +8`. `0x00401520` uses `setnl` after the signed comparison, establishing `current >= value`; `0x00401540` uses `setl`, establishing `current < value`. Reconstructed operator names are not original-symbol claims.
- Natural maintained source replays both functions 20/20 bytes with no relocations. The recovered match units were replayed directly, then all 55 configured units were replayed twice from the dirty source with zero mismatches. Target identity, tracking, progress, public CI, and `git diff --check` all pass.
- Exactness and origin remain separate: both methods are classified `authored_game/ZunTimer` from the already-established game timer type, widespread target use, target-local comparison semantics, and natural VC7.1 generation. Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` and committed-only TH095 HEAD `2060b0bcfb54b4d850471e33161a31454d671db7` corroborate the operator names/source shape only; no adjacent address, extent, or exactness was transferred. TH095 had four untracked scratch files and no uncommitted content was consulted.
- Profile evidence retained by the recovered packet shows both methods exact under tested `/O2 /Ob0`, `/O2 /Ob1`, `/O2 /Ob2`, and `/Ox /Ob1` with `/Oy-`; `/O1 /Ob1` emits 18 bytes and `/Od /Ob0` 27. These remain local compiler facts only.
- Shared IDA comments at both entries record signed semantics, exact replay, reconstructed-name limits, and boundaries; both comments were read back under passed `target:th09-main` / `factory-native-stdio` attestation. `0x00401534-0x0040153F` and `0x00401554-0x0040155F` are twelve-byte unassigned `CC` gaps.
- The actual whole-build command still returns rc 2 because compile flags, TU partition, libraries, resources, and link order remain unknown. No runtime, whole-image, semantic-stage, or Factory-acceptance claim follows. The two functions add 40 canonical authored bytes, taking totals from 43 functions / 4,429 bytes to 45 functions / 4,469 bytes.

The next evidence-connected packet should inspect `0x00401560` and its immediate timer-adjacent neighborhood from TH09-local callers, callees, ABI, and complete boundary before selecting an operator/helper name. Do not infer ownership solely from address adjacency.

The thirty-seventh reviewed packet recovers the interrupted Supervisor timer callee and canonically reconstructs `Supervisor::TickTimer @ 0x0042F4F0-0x0042F536`.

- Session recovery found eight unstaged tracked paths plus untracked `src/SupervisorTimer.cpp` at unchanged HEAD `1d1d34e0dd99b709eb8222c56c508d7dde533594`. The complete diff was confined to TickTimer source, canonical match/origin ledgers, and generated progress. The prior handoff already named TickTimer as an independent larger callee frontier, so all nine paths were classified `recoverable-current-work` and completed before a new packet.
- Native `th09-ida` independently reports a 0x47-byte function at `0x0042F4F0`. Its only direct caller is exact `ZunTimer::Tick @ 0x004014B0`; its only callee is `__ftol2 @ 0x0047B1D4`. Attested target bytes show nine `CC` bytes at `0x0042F537-0x0042F53F`, so those bytes remain outside the reviewed function with physical ownership unresolved.
- The target ABI is normal `__thiscall`: ECX carries the Supervisor object, two stack pointer arguments are consumed by `ret 8`, and Supervisor `+0x5B8` is the observed speed float. The function compares it with 0.99f, accumulates fractional time and converts through `__ftol2` on the slow path, or increments integer/fractional time by one on the fast path. Public Supervisor layout outside this field remains opaque.
- Natural maintained source replays 71/71 bytes twice under the configured pinned VC7.1 `/O2 /Ob1 /Oy-` unit, with three relocations to 0.99f, `__ftol2`, and 1.0f. This establishes repository-canonical codegen exactness for the function only; original translation-unit partition and project-wide flags remain unknown.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` corroborates the `Supervisor::TickTimer` naming/source-shape hypothesis. TH095 HEAD `2060b0bcfb54b4d850471e33161a31454d671db7` had unrelated pre-existing semantic/runtime work; only committed TickTimer references were consulted and no uncommitted TH095 content was read. TH09 target evidence and replay decide the result.
- The shared IDA comment at `0x0042F4F0` was updated from the earlier provisional-callee state to the reviewed boundary/exactness limitations, and the function prototype was corrected to `void __thiscall sub_42F4F0(void *this, int *current, float *subFrame)`. Decompilation readback confirmed both metadata edits under passed `target:th09-main` / `factory-native-stdio` attestation. No function rename or target-byte edit was performed.
- The production skeleton remains deliberately open. `python3 scripts/build.py` returns rc 2 because compile flags, TU partition, libraries, resources, and link order are unresolved; no whole-build or runtime closure follows from this exact unit. The new function raises canonical authored totals from 45 functions / 4,469 bytes to 46 functions / 4,540 bytes. Factory Truth Kernel acceptance remains separate and unclaimed.

The next evidence-connected packet should return to the timer-adjacent candidate at `0x00401560`, which was already queued by the prior handoff. Establish its complete extent, signed/unsigned semantics, callers, and ABI from TH09 before adopting an operator/helper name. The just-finished TickTimer packet is the required harder frontier after the two small comparison operators; the next selection is therefore continuity-driven rather than a smallest-function queue.

The thirty-eighth reviewed packet corrects the queued `0x00401560` ownership hypothesis, reconstructs the central ANM helper there, and independently closes its target-local Float3 constructor dependency.

- Address adjacency was misleading: `0x00401560-0x004015E5` is not a ZunTimer comparison/helper. Native TH09 evidence shows a 134-byte `__thiscall` member with two explicit arguments, broad gameplay/rendering callers, owner `+0x10` used as a script-pointer table, and a final call to `0x004395A0`. Exact/provisional `AnmLoaded::GetSprite @ 0x00434390` independently observes the same owner family with sprite storage at `+0x0C`, while the callee writes the owner into VM `+0x204`. Together these target-local relationships support `AnmLoaded::ExecuteAnmIdx(AnmVm *, int)`.
- The function writes a 16-bit script index at VM `+0x21A`, constructs zero three-float values at VM `+0x208` and `+0x288`, sets VM bytes `+0x299/+0x298` to 15, and invokes candidate `AnmLoaded::SetAndExecuteScript @ 0x004395A0` with `scripts[scriptIndex]`. The public AnmLoaded/AnmVm layouts remain intentionally opaque; `src/AnmLoadedExecute.cpp` uses packet-local layout views rather than claiming unobserved fields.
- Dependency `0x004010B0-0x004010C9` is independently reconstructed as `Float3::Float3(float,float,float)`: ECX is the 12-byte destination, x/y/z are stored at `+0/+4/+8`, EAX returns `this`, and `ret 0x0C` consumes the three float arguments. Target-local componentwise add/subtract/scalar-multiply helpers at `0x00401100/0x00401140/0x00401180` all create their result through this constructor.
- Natural source for the constructor replays 26/26 bytes with no relocations. Natural `ExecuteAnmIdx` source replays 134/134 bytes, including REL32 relocations at `+0x22/+0x46` to the exact constructor and `+0x79` to `0x004395A0`. Both new units, plus all nine pre-existing exact units that directly consume the modified shared ANM/ASCII headers, were cold-built and replayed exact in two complete passes.
- TH08 committed HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was clean when consulted and corroborates the reconstructed `Float3` constructor and `AnmLoaded::ExecuteAnmIdx` source shape only. TH095 committed HEAD `df437233d8a713960d28f04a483a70e8d3bc9b55` had unrelated untracked runtime/semantic files; only committed ANM source was read. Its `InitializeVm` includes extra initialization not present in TH09 and was explicitly not transferred. TH09 target evidence and replay decide both results.
- Shared IDA comments and conservative prototypes were written and read back at `0x004010B0` and `0x00401560` under passed `target:th09-main` / `factory-native-stdio` attestation. Neither function was renamed in IDA, and no target bytes were edited. Six `CC` bytes at `0x004010CA-0x004010CF` and ten at `0x004015E6-0x004015EF` remain physically unassigned.
- `0x004395A0` remains a connected `AnmLoaded::SetAndExecuteScript` candidate only: its target behavior strongly supports the role, but its origin/source/exactness are still `unknown/review`. The two new canonical functions raise authored exact totals from 46 functions / 4,540 bytes to 48 functions / 4,700 bytes. Whole-build closure, runtime validation, and Factory Truth Kernel acceptance remain separate and unclaimed.
- Checkpoint validation reran both new match units, target-bound tracking, generated-progress checks, `scripts/ci.py`, and `git diff --check` successfully. `python3 scripts/build.py` still exits 2 with the explicit open-graph diagnostic because compile flags, TU partition, libraries, resources, and link order remain unknown. `.analysis/` grew from approximately 17 MiB at session entry to 18 MiB; only four small current-session ANM/Float3 probe artifacts were added under the inherited `20260910-th09-exact-startup` campaign, while the older campaign contents and stale inherited manifest were retained rather than rewritten or deleted.

The next evidence-connected packet should exploit the newly established target-local Float3 seam as one bounded arithmetic cohort: inspect `0x00401100`, `0x00401140`, and `0x00401180` together, recover hidden-return ABI and exact source signatures rather than assuming C++ operators from TH08. This is not a smallest-function queue: the preceding packet attacked a 134-byte central broad-fanout ANM owner boundary and disproved the queued timer-adjacency hypothesis. After at most this one compact Float3 cohort, return to a harder frontier such as the 543-byte candidate `0x0042F540` immediately after exact Supervisor::TickTimer, or another evidence-connected central boundary if TH09 data redirects the work.

The thirty-ninth reviewed packet closes the connected Float3 arithmetic cohort at `0x00401100/0x00401140/0x00401180` without inferring the operators from address order or adjacent-game source.

- TH09 first fixed the machine-level contract independently. All three functions are value-returning `__thiscall` helpers: ECX carries the left/source Float3, stack slot 1 is a hidden 12-byte return-object pointer, stack slot 2 is either a right-operand pointer/reference or a float scalar, and `ret 8` consumes the hidden/result operand pair. `0x00401100` adds x/y/z, `0x00401140` subtracts x/y/z, and `0x00401180` multiplies x/y/z by the scalar; each constructs the hidden result through exact `Float3::Float3 @ 0x004010B0`. Broad TH09 caller sets establish their generic math-helper role.
- Only after those target-local operations and ABI were established, committed adjacent source was consulted. Clean TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` and committed TH095 HEAD `27eee6327f1c65dee113a09cadc30e22ec03ca78` both suggest the signatures `Float3::operator+(const Float3 &) const`, `operator-(const Float3 &) const`, and `operator*(float) const`. TH095 also had four unrelated untracked runtime/semantic files; none were read. Adjacent source supplied a naming/signature hypothesis only.
- A pinned VC7.1 natural-source probe produced the expected decorated symbols and exact target sizes: plus `??HFloat3@@QBE?AU0@ABU0@@Z` at 54 bytes, minus `??GFloat3@@QBE?AU0@ABU0@@Z` at 54 bytes, and multiply `??DFloat3@@QBE?AU0@M@Z` at 52 bytes. Structural comparison matched every non-relocation byte and solved the sole REL32 relocation in each function to exact constructor `0x004010B0`.
- The maintained `src/Float3.cpp` and `src/AsciiManager.hpp` forms then replayed all three functions canonically exact. Two complete tracked-source passes also rebuilt the exact constructor, exact `AnmLoaded::ExecuteAnmIdx`, and all nine existing exact direct AsciiManager users of the shared Float3 header; every affected unit remained exact. The cohort contributes 160 exact authored bytes, raising totals from 48 functions / 4,700 bytes to 51 functions / 4,860 bytes.
- Logical boundaries are `0x00401100-0x00401135`, `0x00401140-0x00401175`, and `0x00401180-0x004011B3`. The following `CC` runs (`0x00401136-0x0040113F`, `0x00401176-0x0040117F`, and `0x004011B4-0x004011BF`) remain physically unassigned.
- Shared IDA comments and conservative hidden-result prototypes were written at all three entry points and read back under passed `target:th09-main` / `factory-native-stdio` attestation. No IDA function rename and no target-byte edit was performed.
- Final checkpoint validation reran the three new match units, target-bound tracking, generated-progress checks, `scripts/ci.py`, and `git diff --check` successfully. `python3 scripts/build.py` remains deliberately unavailable with rc 2 because compile flags, TU partition, libraries, resources, and link order are unresolved. `.analysis/` remains approximately 18 MiB; this packet added only reproducible `Float3ArithmeticProbe.cpp` (627 bytes) and `Float3ArithmeticProbe.obj` (3,026 bytes) under the inherited campaign. Older scratch and the inherited stale manifest were retained, with no bulk deletion.

Packet-selection balance now requires a hard frontier: packet 38 was the 134-byte central ANM owner/boundary correction, and packet 39 intentionally used the one permitted compact connected Float3 cohort. The next packet must not be chosen for size/ease. Prefer the immediately connected 543-byte candidate `0x0042F540` after exact `Supervisor::TickTimer`, reconciling its full boundary, owner/call graph, control flow, data dependencies, and source shape before any exactness claim. A documented unknown or non-exact source-present result is acceptable if the target evidence does not close.

The fortieth reviewed packet satisfies the scheduled hard-frontier requirement by reconstructing the 543-byte Supervisor snapshot path at `0x0042F540-0x0042F75E`.

- Packet selection was structural rather than size/ease driven: after the compact Float3 cohort, `0x0042F540` was the required materially larger frontier immediately adjacent to exact `Supervisor::TickTimer`. It has D3D virtual calls, two allocation/error branches, a nested 640x480 conversion loop, Win32 file I/O, 23 relocations, and a Supervisor/data-layout boundary. An honest non-exact outcome was permitted; the target evidence happened to close exactly.
- TH09 target-local ownership came first. The function has one direct caller inside `0x0042DB50`; that caller checks the snapshot input, creates `snapshot`, searches `snapshot/th%.3d.bmp`, and invokes `0x0042F540` with `&dword_4B3100`, the same Supervisor object family used by exact Supervisor methods. The callee is `__thiscall`, consumes one `const char *filePath` argument with `ret 4`, reads a D3D-device-like pointer at Supervisor `+0x08`, and reads a D3DFORMAT-like value at `+0x358`.
- The target calls Direct3D8-style `GetBackBuffer`, rejects `D3DFMT_R5G6B5`, and captures `D3DFMT_X8R8G8B8`. It allocates a 0x2C-byte bitmap-info block and a 0xE1000-byte pixel buffer through exact `g_ZunMemory` wrappers, locks the backbuffer, converts 640x480 X8R8G8B8 rows into bottom-up 24-bit BMP bytes, unlocks, writes a 14-byte file header, 40-byte bitmap-info header, and 0xE1000 pixel payload through Win32 I/O, releases the surface, frees both buffers, and returns 0. Unknown formats log an error and return 1 before cleanup.
- Attested target bytes at `0x004907E8-0x0049083F` recovered the exact Shift-JIS message bytes used by the capture/allocation branches. Maintained source records those bytes with `\xNN` escapes; this avoids inventing translated text. Function exactness binds its relocations to the target strings but does not by itself assign physical data ownership for those string objects.
- Only after TH09 fixed the role and owner was adjacent source consulted. Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` contains `Supervisor::TakeSnapshot` with the same broad BMP algorithm, but its debug prints, allocator choice, and some message text differ and were not transferred. Committed TH095 was consulted only as a later screenshot-family hypothesis source; its threaded screenshot implementation is materially different, and unrelated untracked TH095 runtime/semantic files were preserved and not read.
- Source-shape recovery was compiler-guided rather than padded: an initial natural probe was 525 bytes, a row-pointer form was 533, and explicit negative-pitch state reached the 543-byte target size while retaining 20 comparable-byte differences. Those differences showed one extra four-byte local and three negative-stack `WriteFile` output pointers. The target decompiler independently exposes `(LPDWORD)&filePath` for all three calls; removing the separate `bytesWritten` local and using that exact source shape closed all differences. No inert padding, assembly, target bytes, fake returns, or oracle-specific branches were introduced.
- `src/SupervisorSnapshot.cpp` keeps the public `Supervisor` layout opaque. A packet-local layout view records only the two fields proven here (`+0x08` device, `+0x358` format). The tracked source compiles to the target symbol `?TakeSnapshot@Supervisor@@QAEHPBD@Z`; two consecutive canonical cold passes each replay 543/543 bytes and all 23 relocations exactly. Canonical totals therefore rise from 51 functions / 4,860 bytes to 52 functions / 5,403 bytes.
- The logical extent ends at `0x0042F75E` with `ret 4`. Attested bytes show exactly one unassigned `CC` byte at `0x0042F75F`; independent candidate `0x0042F760` begins at the next byte. Shared IDA received an English evidence comment and conservative `int __thiscall sub_42F540(void *this, const char *filePath)` prototype, both read back under passed `target:th09-main` / `factory-native-stdio` attestation. No IDA rename and no target-byte edit was performed.
- Checkpoint validation reran the focused match unit, `validate-tracking.py --require-target`, generated-progress checks, `scripts/ci.py`, and `git diff --check` successfully. Tracking reports 2,159 candidates, 52 mappings, and 52 exact functions. `python3 scripts/build.py` remains rc 2 with the explicit open-graph diagnostic because original compile flags, TU partition, libraries, resources, and link order are unresolved. Runtime validation and Factory Truth Kernel acceptance remain separate and unclaimed.
- `.analysis/` remains approximately 18 MiB versus approximately 17 MiB at session entry. This hard packet retained small reproducible `SupervisorTakeSnapshotProbe.cpp` (3,798 bytes), its object (13,481 bytes), and a tracked-source probe object (13,398 bytes) inside the inherited campaign. Older campaign scratch and its stale inherited manifest were retained; no legacy/unknown analysis state was deleted.

Packet-selection balance is reset by this hard-frontier result. The next evidence-connected packet should stay in the same Supervisor neighborhood but must not become a one-function smallest queue: inspect `0x0042F760` (15 bytes), `0x0042F770` (74 bytes), and the materially larger `0x0042F7C0` (1,054 bytes) together as one post-snapshot cohort. Establish boundaries, caller/callee relationships, owner/data seams, and whether the small entries are initialization/wrapper pieces of the larger subsystem before choosing maintained source. The 1,054-byte member is the hard anchor; a small exact helper may be included only if it is genuinely part of that connected cohort.

The forty-first reviewed packet closes the post-snapshot Supervisor cohort while correcting two address-adjacent ownership assumptions.

- `Supervisor::LoadConfig @ 0x0042F7C0-0x0042FBDD` is repository-canonical exact at 1,054 bytes. `_WinMain@16` supplies `g_Supervisor @ 0x004B3100` as ECX and `"th09.cfg"`; two no-pragma cold replays match 1,054/1,054 bytes with 131 relocations.
- The exact source shape is target-driven: whole-struct controller copies produced a 702-byte body, while field-by-field copies produced the target body. The inherited unsupported `#pragma var_order` was removed without affecting exactness.
- `0x0042F760` and `0x0042F770` are not Supervisor methods merely because they are adjacent. The sole external call to `0x0042F770` passes `g_AnmManager @ 0x004DC550` as ECX; both rows are corrected only to the AnmManager subsystem with origin/name/source/exactness left `unknown/review`.
- Adding the new Supervisor declarations shifted only compiler-internal ChainRun local COFF names. Relocation offset/type/target maps remained identical; after refreshing only those replay names, all 53 then-canonical exact units rebuilt and replayed exact.
- Shared IDA received evidence comments at all three cohort addresses and a conservative LoadConfig prototype; no IDA rename or target-byte edit was performed. Local checkpoint `c06b723c3f19e64b80efa72ad93df497c08d484e` is `gpt-web: reconstruct Supervisor LoadConfig`; GPT-web did not push it.

The forty-second reviewed packet follows LoadConfig into the 0xCC GameConfiguration data/constructor seam and corrects the controller-data ownership model.

- `GameConfiguration::Initialize @ 0x0041A8A2-0x0041A98A` is exact at 233 bytes with six relocations. `GameConfiguration::GameConfiguration @ 0x0041A98B-0x0041A996` is exact at 12 bytes with one relocation to Initialize; independent function `0x0041A997` begins immediately afterward.
- TH09-local callers establish both an actual 0xCC allocation path and an embedded-config path; every observed caller ignores the initializer return value. Maintained source therefore uses `void Initialize()` without claiming the original return contract, and the exact ledger intentionally leaves that signature field blank.
- The two temporary 0x36 globals from the first LoadConfig source are replaced by one target-supported 0xC4 controller layout rooted at `0x004ACE70`: three 0x12 bindings, an opaque 0x58-byte middle span, then three more 0x12 bindings at `+0x8E`. Refactored LoadConfig remains exact 1,054/1,054 and all affected relocations solve to the single base plus object addends. The original target identifier and opaque-middle semantics remain unknown.
- Natural Initialize is 444 bytes under `/O2 /Ob1 /Oy-`, but exactly 233 bytes under both `/O1 /Ob1 /Oy-` and `/O2 /Os /Ob1 /Oy-`; the latter is canonical. Exact LoadConfig instead becomes 957 bytes under `/O2 /Os`, so project-wide flags, original TU partition, and any local optimization pragma remain unknown.
- Canonical authored totals rise to 55 functions / 6,702 bytes. Whole-build closure remains open; runtime validation and Factory Truth Kernel acceptance remain separate and unclaimed.
- Checkpoint validation passes target-bound tracking (2,159 candidates / 55 mappings / 55 exact), generated-progress checks, public CI, `git diff --check`, and a complete cold rebuild/replay of all 55 canonical exact units. `scripts/build.py` remains rc 2 with compile flags, TU partition, libraries, resources, and link order explicitly unresolved.

Packet-selection balance now favors a harder connected frontier rather than the 21-byte neighbor at `0x0041A997`: inspect the 1,689-byte direct caller `0x0041AF2D-0x0041B5C5` next. It allocates a 0xCC GameConfiguration, calls the exact initializer, copies live Supervisor configuration, and coordinates several central subsystems. The measurable stop condition is to establish its full extent/owner/control flow and enough ABI/data/callee context for an honest maintained-source or documented non-exact/unknown result.

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
- Seven CRT/library candidates plus one compiler-generated ChainElem scalar
  deleting destructor are reviewed as exclusions. Fifty-five authored functions
  are source-present and repository-canonical exact: four GameErrorContext
  methods, six FileSystem helpers, five Supervisor methods (the two lock wrappers, TickTimer, TakeSnapshot, and LoadConfig), seven Chain
  methods, three ChainElem lifecycle/callback methods, Controller::GetJoystickCaps,
  three ZunMemory release wrappers, two PbgArchive query methods, PbgArchive::ReadDecompressEntry, nine AsciiManager lifecycle/text/setup/reset/draw/popup methods, seven ZunTimer methods, four Float3 methods (constructor plus three arithmetic operators), two GameConfiguration methods, and `AnmLoaded::ExecuteAnmIdx`. All other imported origins remain pending.
  Factory acceptance remains
  unavailable because the current TH09 adapter has no codegen-exact replay
  driver; no Truth Kernel acceptance is claimed.
- `config/build.toml` exists from day one but correctly reports an open graph.

## Next bounded work

Treat `Lzss::Encode @ 0x00434020` as source-present but codegen-blocked until new TH09-local compiler/source-identifier evidence appears. `Supervisor::TakeSnapshot @ 0x0042F540` is now independently canonical exact after the required hard-frontier pass. The post-snapshot Supervisor cohort is now closed: LoadConfig is exact and the two smaller address-adjacent entries were corrected to an AnmManager ownership seam without false promotion. Continue with the connected hard caller `0x0041AF2D` rather than the tiny `0x0041A997` neighbor. Preserve `0x004395A0`, LZSS origin/state ownership, and TryDecryptFromTable as unresolved; do not push from GPT-web.

The forty-third reviewed packet attacks the required hard frontier at `0x0041AF2D-0x0041B5C5` and deliberately stops at source-present/non-exact rather than manufacturing equality.

- Packet selection was structural rather than size/ease driven: the 1,689-byte routine is the direct gameplay-setup caller connected to exact `GameConfiguration::Initialize` and `GameConfiguration::GameConfiguration`, owns a large success/failure control-flow surface, allocates and initializes game-side state, registers multiple central subsystems, and crosses the GameManager/Supervisor ownership seam. The previous handoff explicitly selected it instead of the adjacent 21-byte candidate.
- TH09 target evidence closes the entry mechanism and boundary. `0x0041B76F` places `g_Supervisor @ 0x004B3100` in ECX, pushes NULL plus `0x0041AF2D`, and calls `0x00431AE0`; that wrapper is a Supervisor `CreateThread` path. The setup entry ends with a plain `ret` at `0x0041B5C5`, and independent `0x0041B5C6` begins immediately. Native IDA reports the registration as a data xref rather than a normal caller, explaining the otherwise empty caller list.
- The reconstructed source uses a packet-local GameManager layout rooted at `0x004A7D90` rather than claiming a complete public class. TH09 proves two 0x38-byte side records, their seven subsystem pointers, state pointer/selector/flags fields, the setup state/mode/flags seam, the stage-object pointer, and the success/failure cleanup paths. The Supervisor overlay names only target-observed configuration/thread-state fields, including the embedded 0xCC `GameConfiguration @ +0x388`.
- Source-shape recovery was compiler-guided. Early natural probes ranged from 1,852 bytes down to 1,685 bytes. Target-driven corrections included the missing GameManager `+0xD0` field, explicit allocation-failure assignment after `operator new(0xCC)`, equality-shaped game-mode branches, branch fall-through orientation, and block-local state reloads. The maintained source then reaches the exact 1,689-byte target size under both `/O2 /Os /Ob1 /Oy-` and `/O1 /Ob1 /Oy-`.
- Equal size is not treated as equality. Relocation-masked comparison still matches only 402/1005 directly comparable bytes, with the first mismatch at `+0x1DB`. The main observed codegen blocker is stock VC7.1 register scheduling in the non-reload setup path: the target reloads the manager into ECX at repeated calls and uses ESI as the side-record flag-base induction pointer, while the candidate hoists the manager address into ESI and chooses a different side-loop base. A direct-global source variant emits 1,688 bytes without solving this; disabling `/Og` expands the body to 2,564 bytes. No padding, volatile target-address forcing, assembly, or replacement compiler behavior is used.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` supplied only the `GameplaySetupThread` lifecycle/name hypothesis after TH09 established the thread role. TH095 HEAD `66c58a63c7dd91659095501f4ec6645f5dd40c6c` had unrelated tracked and untracked work; none of that uncommitted content was consulted for this packet. TH09 determines all addresses, layouts, control flow, and codegen conclusions.
- `config/functions.csv` now records maintained `src/GameManagerSetup.cpp` with proposed name `GameplaySetupThread`, but status remains `unclassified` at 0% and `function-origins.csv` remains `unknown/review`. No `matches.csv` or `implemented.csv` row is added, and canonical authored exact totals remain 55 functions / 6,702 bytes.
- Shared IDA received an English evidence comment and conservative `void __fastcall sub_41AF2D(void *unused)` prototype, both read back under passed `target:th09-main` / `factory-native-stdio` attestation. The function was not renamed and no target bytes were edited.
- Source presence is therefore established; exactness is not. Whole-build closure remains open, runtime validation is not established, and Factory Truth Kernel acceptance remains unavailable/unclaimed.

Packet-selection balance is reset by this hard-frontier result. The next bounded packet should stay evidence-connected by inspecting `0x0041AA07`, the 88-byte helper called twice by the gameplay setup timing path. Recover its state-pointer ABI, threshold behavior, boundary, and natural VC7.1 shape without assuming the decompiler's return type. Because this is one compact helper immediately after a hard packet, the following packet should return to a materially larger connected frontier such as the teardown/lifecycle routine beginning at `0x0041B5C6` rather than continuing a smallest-function queue.
- Checkpoint validation reran `validate-tracking.py --require-target`, reconstruction status, public `scripts/ci.py`, and `git diff --check` successfully. `scripts/build.py` remains intentionally unavailable with rc 2 because compile flags, TU partition, libraries, resources, and link order are unresolved. `.analysis/` is approximately 23 MiB versus approximately 20 MiB at session entry; this packet retained only reproducible current-session VC7.1 probe sources, objects, PDBs, and comparison JSON under `.analysis/gpt-web/20260911-th09-exact-41af2d/`, with no file above 64 MiB and no legacy/unknown analysis state removed. The existing campaign manifest was retained; two attempted in-session manifest rewrites were unavailable at the execution layer and did not modify tracked state.

The forty-fourth reviewed packet follows the hard gameplay-setup frontier with one compact, directly connected helper at `0x0041AA07-0x0041AA5E`.

- Four TH09 callers lie in the connected GameManager setup cohort: two in `0x0041AA5F` and two in `GameplaySetupThread @ 0x0041AF2D`. They pass the GameManager base in ECX and ignore EAX; the callee does not consume ECX, so the maintained ABI is conservatively `void __thiscall` rather than adopting IDA's provisional pointer return.
- Target bytes fix the behavior: side-0 state comes from `g_GameManager + 0x1C`; `value00 < 7.0f` causes `+1.0f` and an event-object call at `0x004DC698` with `(0x1C, 0)`; state `+0x10` increments; game mode `+0x118 == 1` uses threshold 8 and other modes threshold 5; threshold completion stores 99999.
- The first natural source was 84 bytes because it reused the loaded state pointer. Re-reading the global pointer produced an 88-byte structural match. The first tracked canonical replay then exposed a real type error: provisional unsigned `phase10` emitted `JB`, while target offset `+0x4E` is signed `JL`. Correcting the field to `int` is target-evidenced.
- Two consecutive canonical `game-manager-advance-timed-state` replays reproduce 88/88 bytes and all eight relocations, including GameManager base/addends, the 7.0f and 1.0f constants, event object `0x004DC698`, and call target `0x0043E2F0`.
- Authored-game classification is TH09-local: this helper is only called from the GameManager setup cohort, mutates GameManager-owned mode/state, and invokes a game event object. The committed TH08 search supplied no direct implementation and no adjacent code was transferred.
- `GameplaySetupThread` remains source-present/non-exact after the signed-field correction: its tested `/O2 /Os` body remains 1,689 bytes and 402/1005 directly comparable bytes match. This packet does not promote the hard-frontier routine.
- Canonical totals become 56 authored exact functions / 6,790 bytes. Whole Windows i386 build closure remains open; runtime validation and Factory Truth Kernel acceptance remain unclaimed.

Packet balance since the last hard attempt is hard/non-exact `0x0041AF2D` followed by one compact exact helper `0x0041AA07`. The next bounded packet must therefore be the materially larger connected lifecycle/teardown routine `0x0041B5C6-0x0041B76E` (425 bytes), not another smallest-function candidate.
- Shared IDA received and read back an English exactness/ABI comment at `0x0041AA07` under passed `target:th09-main` / `factory-native-stdio` attestation. A conservative `__thiscall` prototype write was attempted, but the native provider rejected that prototype string at parse time; no substitute calling convention was forced and no target bytes changed. Tracking validation passes at 56 mappings / 56 exact. Public CI initially reported only stale generated progress after the new exact row; `python3 scripts/progress.py` regenerated `docs/PROGRESS.md` and `resources/progress.svg`. Whole-build `scripts/build.py` still returns rc 2 for the explicitly open graph.

The forty-fifth reviewed packet returns to the required hard frontier at `0x0041B5C6-0x0041B76E` (425 bytes), immediately after the gameplay setup thread.

- This packet was selected for lifecycle/owner importance rather than ease: it is called from two central routines (`0x00416A69` and `0x00434F70`), releases the gameplay subsystem graph rooted in `g_GameManager @ 0x004A7D90`, and then recomputes GameManager setup parameters. It follows the previous hard/non-exact `0x0041AF2D` packet and one compact exact helper, satisfying the mixed-frontier rule.
- Native TH09 evidence closes the boundary and return usage. The routine ends with a plain `ret` at `0x0041B76E`; independent `0x0041B76F` begins at the next byte. At both call sites the next instruction overwrites/ignores EAX, and the routine has no intentional return-producing tail, so maintained source uses a no-argument `void` static method. Zero-argument cdecl/fastcall spelling remains observationally unresolved and is not forced into the ledger.
- The target first calls the already-observed GameManager reset helper at `0x0041A7C3`, exact `AsciiManager::Reset @ 0x004343E0`, and two global gameplay release helpers. It then walks two 0x38-byte side records using a pointer to each record's flags field, clears bit 0, and invokes seven release functions on pointers at side offsets `+0x00..+0x18`; the shared pointer at manager `+0x7C` is released through the same helper used for side offset `+0x0C`.
- The parameter-rebalance half is target-derived rather than copied from adjacency. `gameMode == 2` subtracts 8 from `limitB4` and clamps it to `value100`. Other modes switch on `setupMode`: cases 4/3/2/1/0 select limits from constants or `counterFC`, compute `valueC8` with the target's signed divide-by-two/divide-by-three idioms, and compute `valueC4` as 600-scaled offsets from `value100`; the final normal-path limit subtracts `2 * value100` and clamps to one. The routine finally clears `valueB0/value348` and subtracts 1000 from `valueCC`, clamping negative results to zero.
- Natural maintained source in `src/GameManagerSetup.cpp` emits 399 bytes under both tested `/O2 /Os /Ob1 /Oy-` and `/O1 /Ob1 /Oy-`, versus 425 target bytes. The entry, release calls, two-side induction loop, shared release, and arithmetic operations are source-shaped, but stock VC7.1 tail-merges the special-mode clamp with the normal clamp. The target instead keeps the special `JGE / store / JMP cleanup` block separate, which also pushes several switch branches over short-jump range. An explicit `goto cleanup` is optimized back to the same 399-byte form; an early-return duplicate-cleanup probe reaches 408 bytes but wrongly saves EBX. Neither artificial/non-evidenced probe shape was retained, and no padding/assembly/volatile-address forcing is used.
- TH08 committed HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was consulted only for a lifecycle hypothesis. Its committed `GameManager::DeletedCallback` has a materially different audio/chain/replay cleanup sequence, so it was rejected as an implementation template. No TH095 content was needed for this packet.
- Origin is independently classified `authored_game` with high confidence from TH09-local GameManager state and game-subsystem lifecycle behavior, even though exactness remains false. `config/functions.csv` maps maintained source at 0%, while `config/matches.csv`, `config/implemented.csv`, and `config/reccmp-functions.csv` deliberately remain unchanged for this address.
- Shared IDA received an English source-present/non-exact comment at `0x0041B5C6` under passed `target:th09-main` / `factory-native-stdio` attestation. No prototype or name was forced, and no target bytes were modified.
- Source presence is established; exactness is not. Canonical totals therefore remain 56 authored exact functions / 6,790 bytes. Whole-build closure remains open; runtime validation and Factory Truth Kernel acceptance remain unclaimed.

Packet balance is now hard/non-exact `0x0041AF2D`, compact/exact `0x0041AA07`, hard/non-exact `0x0041B5C6`. One compact connected packet is now permissible. The next evidence-connected candidate is `0x0041B76F`, the Supervisor-facing lifecycle routine that directly registers `GameplaySetupThread`; inspect its full boundary/callees and thread-state contract before choosing a source owner, rather than switching to an unrelated small function.
- Checkpoint validation preserved `GameManagerSetupLayout::AdvanceTimedState` at canonical 88/88 exact, passed target-bound tracking and public CI after regenerating progress, and left canonical exact totals at 56 / 6,790 bytes. Authored-origin inventory becomes 57 with 2,094 pending because this non-exact cleanup routine is independently classified authored-game. `scripts/build.py` remains rc 2/open for unresolved whole-build inputs. `.analysis/` is approximately 24 MiB versus approximately 20 MiB at session entry, with no file above 64 MiB; current-session cleanup probes/objects/comparison JSON are retained under the existing campaign and no legacy/unknown analysis state was removed.

The forty-sixth reviewed packet closes the compact GameManager added-callback seam at `0x0041B76F-0x0041B829`.

- Packet selection stays mixed rather than size-driven. The preceding sequence was hard/non-exact `GameplaySetupThread @ 0x0041AF2D`, compact/exact `AdvanceTimedState @ 0x0041AA07`, and hard/non-exact `CleanupGameplayState @ 0x0041B5C6`; this one compact packet was therefore permissible because it is directly connected to the same lifecycle seam. The next packet must return to the hard frontier at the 422-byte deleted callback `0x0041B82A`, not another small unrelated candidate.
- TH09 target-local ownership is explicit. `RegisterChain` candidate `0x0041B9D0` stores `0x0041B76F` in `ChainElem::addedCallback`, stores `g_GameManager @ 0x004A7D90` in `ChainElem::arg`, and adds that element to the calc chain. The callback itself uses the incoming ECX argument as GameManager state at `+0x134`; it ends with a plain `ret` at `0x0041B829`, and independent candidate `0x0041B82A` starts at the next byte.
- Behavior is target-derived. Supervisor state `+0x590` controls bit `0x200` at `+0x5D4`; GameManager `gameplaySetupState @ +0x124` becomes 1; Supervisor state `+0x594` selects loading positions `(500, 440, 0)` plus effect 0 or `(280, 430, 0)` without the effect; GameManager transition bits `0x60` at `+0x134` are normalized from the >=2 state to 1; the callback then starts `GameplaySetupThread @ 0x0041AF2D` through the Supervisor thread wrapper `0x00431AE0`, calls target-observed `UpdateGameTime @ 0x00430060`, then `UpdatePlayTime @ 0x0042FF70`. The latter pair independently refine Supervisor timer baselines at `+0x79C` and `+0x798`; their own origin/exactness remains unreviewed.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was consulted only after TH09 established the callback role. Its committed `GameManager::AddedCallback` corroborates the name, state exclusions, loading positions, effect call, transition normalization, and thread start. TH09 wins where it differs: TH09 additionally calls the two time-update routines. TH095 was searched only in committed content and did not materially shape this implementation; its observed HEAD was `cb9727db70f11136c3a1056e5916e5ab2ee5fd41`, with unrelated local untracked work left untouched.
- Natural maintained source in `src/GameManagerSetup.cpp` is canonical exact under the packet-local pinned VC7.1 `/O2 /Os /Ob1 /Oy- /Gr` profile. A tracked-source diagnostic first matched all 135 non-relocation bytes. The canonical manifest binds thirteen relocations to the Supervisor/GameManager globals, loading/effect/thread/time callees, and `GameplaySetupThread`; two consecutive builds/replays return 187/187 exact and produce byte-identical JSON reports. No padding, inline assembly, target bytes, volatile address forcing, or custom compiler behavior was introduced.
- Canonical ledgers now record `GameManagerSetupLayout::AddedCallback` as authored-game/high-confidence and exact. Totals rise to 57 authored exact functions / 6,977 bytes, with 2,093 origin/boundary candidates pending and 58 authored / 8 excluded origins. The source also refines only the packet-local Supervisor timer baseline field at `+0x798`; it does not claim complete data ownership or original TU partition.
- Shared IDA metadata was updated only after target-local ownership and canonical equality were established: function `0x0041B76F` is named `GameManager_AddedCallback`, has conservative prototype `int __fastcall GameManager_AddedCallback(void *gameManager)`, and carries an English evidence comment. Name/prototype/comment were read back under passed `target:th09-main` / `factory-native-stdio` attestation. No target bytes were edited.
- Checkpoint validation preserved both exact functions in the affected `GameManagerSetup.cpp` object (`AddedCallback` 187/187 and `AdvanceTimedState` 88/88), passed target verification, `validate-tracking.py --require-target`, status reporting, public `scripts/ci.py`, generated progress, and `git diff --check`. Whole-build `scripts/build.py` remains intentionally rc 2 because compile flags, TU partition, libraries, resources, and link order are unresolved. Runtime validation was not performed, and Factory Truth Kernel acceptance remains separate and unclaimed.
- Session recovery started from clean `main` HEAD `4b60b7deac3245eddc16f209473d4f8404e83169`; no pre-existing tracked/untracked dirty paths required recovery. Legacy ignored `.analysis/` and build state were preserved. `.analysis/` grew from 24,424 KiB at entry to 24,648 KiB before this checkpoint; the current campaign retains only reproducible probe source/object/PDB files, two canonical JSON reports, and its manifest, with no file over 64 MiB. A request to delete the reproducible build object before the second replay was blocked by the execution safety layer, so the compiler was rerun in-place instead; the resulting independent replay report is byte-identical. Two transient Factory transport failures occurred on commands that returned no command id; live repository status was rechecked before continuing. No legacy/unknown analysis state was removed.

Packet balance is now hard/non-exact, compact/exact, hard/non-exact, compact/exact across the connected GameManager setup/lifecycle seam. The next bounded packet is the 422-byte `0x0041B82A-0x0041B9CF` routine already installed by `0x0041B9D0` as `ChainElem::deletedCallback`. It is a hard, owner-central lifecycle frontier rather than an ease-driven choice. The measurable stop condition is to reconcile its full boundary/callers/callees, teardown side effects and ABI, then either establish repeatable complete target-bound VC7.1 equality or retain a source-present/non-exact/unknown result with the exact compiler or evidence blocker.

The forty-seventh reviewed packet closes the hard GameManager deleted-callback frontier at `0x0041B82A-0x0041B9CF`.

- Packet selection returned to the required hard frontier after packet 46's compact exact callback. The recent balance is hard/non-exact `GameplaySetupThread`, compact/exact `AdvanceTimedState`, hard/non-exact `CleanupGameplayState`, compact/exact `AddedCallback`, then this hard/exact 422-byte lifecycle routine. It was selected because `0x0041B9D0` installs it as the same GameManager ChainElem's deleted callback and because it owns the central teardown seam, not because of low risk or small size.
- TH09 target-local ownership and boundary are explicit. `0x0041B9D0` data-references `0x0041B82A` into `ChainElem::deletedCallback` with `g_GameManager @ 0x004A7D90` as `arg`; no ordinary code caller is required. The routine owns `0x0041B82A-0x0041B9CF`, and independent candidate `0x0041B9D0` starts at the next byte with no padding.
- Target behavior snapshots both 0x38-byte side records' `+0x20` values into `+0x24`, copies GameManager `+0xF4` to `+0xF8`, clears flag bit 2 at `+0x134`, and derives Supervisor `+0x598` from state `+0x590` excluding states 3/12/10. It clears 0x2BC0 bytes at `0x004D9698`, then conditionally fills the observed 0x2C-byte record at `0x004AC858` with `HSCR` magic, duplicate 0x2C size fields, version 2, runtime marker 0, GameManager/setup bytes, state `+0x08/+0x14`, and zero float/marker fields.
- When GameManager flag bit 3 and Supervisor `+0x598` allow it, the callback releases and clears the stage object. It then releases/nulls seven subsystem pointers for each of two 0x38-byte sides through target callees `0x00402530`, `0x0041CED0`, `0x00414180`, `0x0040D280`, `0x00411F80`, `0x00404570`, and `0x0041A1D0`; releases game/secondary/shared objects through `0x004182A0`, `0x004156A0`, and `0x0040D280`; frees GameManager `+0xE8` through CRT `free @ 0x0047B249`; drains the object at `0x004DC698` through `0x0043F1F0` until zero; and clears `0x004A80D8`.
- The `0x004DC698` dependency is now target-supported as SoundPlayer-compatible rather than the earlier provisional event-manager label. TH09 `0x0043E2F0` scans twelve SFX queue slots at object `+0x620`, maintains per-slot counts, and stores up to 128 pan values. Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` independently exposes the same SoundPlayer queue layout and `PlaySoundByIdx` / `ProcessQueues` roles. TH08's own GameManager deleted callback differs substantially and was not copied as TH09 implementation evidence. TH095 committed HEAD `cb9727db70f11136c3a1056e5916e5ab2ee5fd41` did not materially shape this packet; its unrelated local untracked work remained untouched.
- Natural VC7.1 source converged without tricks. The first probe was 407 bytes because a boolean assignment deferred the Supervisor store and `strcpy` selected a different copy shape. Explicit natural if/else state assignment plus direct semantic score-header fields produced 424 bytes. Replacing an unnecessary explicit loop-result local with natural `while (g_SoundPlayer.ProcessQueues()) {}` produced the target 422-byte extent, leaving ten non-relocation byte differences limited to score-record store scheduling. Moving the independent `stateValue2B` assignment before the zero-float store yielded 258/258 non-relocation-byte equality. No inert padding, target-byte embedding, inline assembly, volatile address forcing, fake return, or custom compiler behavior was used.
- Tracked `src/GameManagerSetup.cpp` reproduces the routine structurally and canonically. The match manifest binds forty-one relocations covering GameManager/Supervisor fields, the scratch/HSCR data dependencies, all release/free callees, SoundPlayer global/method, and `0x004A80D8`. Two consecutive pinned-VC7.1 canonical builds/replays are byte-identical and each returns 422/422 exact. The same source revision also keeps `AdvanceTimedState` 88/88 exact after correcting its SoundPlayer symbols and keeps `AddedCallback` 187/187 exact.
- Canonical ledgers now record `GameManagerSetupLayout::DeletedCallback` as authored-game/high-confidence and exact. Totals are 58 source-present / 58 exact mappings and 7,399 exact authored bytes, with 59 authored origins, 8 excluded origins, and 2,092 candidates still pending origin/boundary review. The data locations `0x004AC858` and `0x004D9698` are packet dependencies only; their physical/data ownership and original TU partition remain unknown.
- Shared IDA metadata was updated under passed `target:th09-main` / `factory-native-stdio` attestation: `0x0041B82A` is `GameManager_DeletedCallback` with conservative `int __fastcall GameManager_DeletedCallback(void *gameManager)` prototype and English exact-evidence comment; `0x0043E2F0` and `0x0043F1F0` are named `SoundPlayer_PlaySoundByIdx` and `SoundPlayer_ProcessQueues` with comments explicitly retaining their origin/exactness as unreviewed. No target bytes were modified.
- Validation passes the three affected GameManagerSetup match units, match-unit graph, target verification, `validate-tracking.py --require-target`, status reporting, public `scripts/ci.py`, generated progress, and `git diff --check`. Whole `scripts/build.py` remains rc 2 because compile flags, TU partition, libraries, resources, and link order are unresolved. Runtime validation was not performed. Factory/Truth Kernel acceptance remains separate and unclaimed.
- Packet 47 started from clean local checkpoint `37d92b4715418357471fdd40462ed8d4c4d9068b`. During the session the shared `origin/main` ref was observed to advance from behind that local checkpoint to the same commit without any `git push` command being issued by this session; no attribution is made for that external/shared-ref change. The private ignored `resources/th09.exe`, legacy ignored analysis/build state, and adjacent repositories were not modified.
- `.analysis/` was 24,424 KiB at session entry and is 24,928 KiB before this checkpoint. The single reused campaign `.analysis/gpt-web/20260911-th09-exact-41b76f/` is 504 KiB and retains nineteen reproducible current-session probe/object/PDB/JSON artifacts plus its manifest; no campaign file exceeds 64 MiB. No legacy/unknown analysis artifact was removed.

The next connected bounded packet is `0x0041B9D0-0x0041BA4E` (127 bytes), the registration routine that physically binds the now-exact added/deleted callbacks and GameManager argument into the calc ChainElem. This compact packet is permitted immediately after the hard 422-byte frontier and is selected for evidence connectivity, not ease. Its stop condition is to reconcile both ChainElem callback slots/argument, calc-chain insertion ABI and return behavior, then either establish repeatable complete target-bound VC7.1 equality or retain source-present/non-exact/unknown with the exact blocker.

The forty-eighth reviewed packet closes the connected GameManager chain-registration seam at `0x0041B9D0-0x0041BA4E`.

- Packet selection remains mixed. Packet 47 was the required hard 422-byte GameManager deleted-callback frontier; this 127-byte packet is compact but directly closes the physical registration edge between that exact callback, the exact added callback, the exact Chain insertion APIs, and the unresolved GameManager calc/draw callbacks. It was selected for evidence connectivity rather than size. The next packet must return to a hard frontier, and `OnUpdate @ 0x0041AA5F-0x0041AF2C` is the strongest connected candidate because this packet proves it occupies the calc callback slot and it is a materially larger 1230-byte central dispatcher.
- TH09 target-local ownership is explicit. The routine starts by clearing `g_GameManager + 0x348` at `0x004A80D8`; calc ChainElem `0x004A8120` receives callback `0x0041AA5F`, exact `AddedCallback @ 0x0041B76F`, exact `DeletedCallback @ 0x0041B82A`, and `g_GameManager @ 0x004A7D90` as its argument. `Chain::AddToCalcChain @ 0x0042B1E0` is called at priority 2; any nonzero callback/insertion result produces `-1` and skips draw registration.
- On calc success, draw ChainElem `0x004A8140` has added/deleted lifetime callbacks explicitly cleared, receives callback `0x0041A75F` and the same GameManager argument, and is passed to exact `Chain::AddToDrawChain @ 0x0042B270` at priority 5. The draw insertion return is intentionally ignored and the routine returns 0. The current packet establishes only that `0x0041AA5F` and `0x0041A75F` occupy the GameManager calc/draw callback slots; their own origin, boundaries beyond existing inventory, names, implementation, and exactness remain review pending.
- Three TH09 calls from the central `0x00431110` Supervisor/state-machine candidate use this routine during mode transitions and treat nonzero return as failure. The already-exact Chain insertion functions independently establish the 0x20-byte ChainElem offsets and the fact that AddToCalcChain invokes/clears the added callback before insertion and returns that result. This closes the callback/registration ABI without relying on adjacent-game symbols.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was consulted only after TH09 established the field addresses, callbacks, priorities, and Chain API contract. Its committed `GameManager::RegisterChain` corroborates the broad natural source shape (calc callback plus lifetime callbacks, calc failure return, draw callback with null lifetime callbacks), but no TH08 address, extent, priority constant identity, ownership, or exactness was transferred. TH095 did not materially shape this packet and its unrelated untracked state remained untouched.
- An independent natural VC7.1 probe immediately produced a 127-byte function with all 51 non-relocation bytes equal and nineteen relocation destinations resolving to the TH09 target. The same source shape in tracked `src/GameManagerSetup.cpp` reproduced the same structural equality. The canonical manifest then bound the GameManager/Chain globals, both ChainElem objects and field addends, calc/draw callbacks, exact added/deleted callbacks, and exact Chain insertion functions; two consecutive pinned-VC7.1 builds/replays are byte-identical and each returns 127/127 exact. No padding, target-byte embedding, inline assembly, volatile address forcing, fake return, or custom compiler behavior was used.
- Boundary ownership is closed locally: `RegisterChain` ends with `ret` at `0x0041BA4E`; attested target bytes show `0x0041BA4F` immediately begins independent 47-byte candidate `sub_41BA4F`, with no padding gap.
- Canonical ledgers now record `GameManagerSetupLayout::RegisterChain` as authored-game/high-confidence and exact. Totals are 59 source-present / 59 exact mappings and 7,526 exact authored bytes, with 60 authored origins, 8 excluded origins, and 2,091 candidates still pending origin/boundary review.
- Shared IDA metadata was updated only after canonical equality: `0x0041B9D0` is named `GameManager_RegisterChain`, has conservative `int __fastcall GameManager_RegisterChain()` prototype, and carries an English evidence comment explicitly leaving OnUpdate/OnDraw origin/exactness pending. Name/prototype/comment were read back under passed `target:th09-main` / `factory-native-stdio` attestation. No target bytes were modified.
- The affected `GameManagerSetup.cpp` source revision replays all four canonical units exactly: `AdvanceTimedState` 88/88, `AddedCallback` 187/187, `DeletedCallback` 422/422, and `RegisterChain` 127/127. Match-unit graph and `git diff --check` also pass before final checkpoint validation.
- Packet 48 started from clean local checkpoint `13a8ea62d27c99643b452e3a053e228a54526cbd`. At packet start the shared `origin/main` ref was observed equal to that commit even though this session issued no `git push`; as with the prior shared-ref changes, no attribution is made. The private ignored `resources/th09.exe`, legacy ignored analysis/build state, and adjacent repositories were not modified.
- `.analysis/` is 25,124 KiB before this checkpoint, versus 24,424 KiB at session entry. The single reused campaign `.analysis/gpt-web/20260911-th09-exact-41b76f/` is 700 KiB and retains only small reproducible current-session probe/object/PDB/JSON artifacts plus its manifest; no campaign file exceeds 64 MiB. No legacy/unknown analysis artifact was removed.

The next evidence-connected packet is the hard `GameManager OnUpdate` candidate `0x0041AA5F-0x0041AF2C` (1230 bytes). It is selected because `RegisterChain` now proves its central calc-callback ownership, not because it is easy. The measurable stop condition is to reconcile the full 1230-byte boundary/control-flow graph, GameManager field contract, pause/state transitions, audio/time dependencies, and callback return states, then either establish natural target-bound VC7.1 equality or checkpoint a source-present/non-exact/unknown result with the exact compiler/evidence blocker. `OnDraw @ 0x0041A75F` is intentionally not selected next despite its 55-byte size.

The forty-ninth reviewed packet attacks the hard GameManager calc-dispatch frontier at `0x0041AA5F-0x0041AF2C` (1,230 bytes).

- Recovery started from clean `main` HEAD `4281c1ec221701249c0fd7d308d6e6e3ca5e75e4`, with staged / unstaged / untracked counts all zero and `origin/main` equal to the same commit. No recoverable pre-existing tracked work existed. The ignored operator input `resources/th09.exe`, ignored build output, legacy/unknown `.analysis/` content, and adjacent repositories were preserved. No reset, stash, deletion, target relocation, target patch, or push was performed.
- The hard packet remains the evidence-connected choice named by packet 48 rather than an ease-driven candidate. Exact `GameManagerSetupLayout::RegisterChain @ 0x0041B9D0` installs `0x0041AA5F` in the calc ChainElem callback slot with `g_GameManager @ 0x004A7D90` as the argument and priority 2. Native TH09 reports no ordinary code caller; the registration data xref explains the entry. The reviewed logical extent is `0x0041AA5F-0x0041AF2C`, and independent `GameplaySetupThread @ 0x0041AF2D` starts at the next byte with no padding gap.
- TH09 target-local behavior closes the callback protocol without relying on adjacent-game symbols. The function returns Chain states 1, 3, and 4; increments the GameManager frame counter; conditionally snapshots five 16-bit input words into two 10-byte destinations; waits for gameplay setup; performs stage-startup resource/audio work; handles pause and demo transitions including the 6,000/6,120-frame path; updates periodic GameManager counters; grows paired side-state fields; performs lane-based two-side object spawning and SFX/auxiliary calls; smooths both side scores with the target unsigned arithmetic and 578,910 cap; invokes exact `AdvanceTimedState` under the two target thresholds; increments GameManager `+0x348`; and returns the continue state.
- Origin is now independently `authored_game` / GameManager / high confidence because exact target-local registration binds the routine as the GameManager calc callback and the body operates the established GameManager gameplay state. This classification is independent from exactness. `config/functions.csv` now records maintained source and the conservative fastcall callback signature, but deliberately leaves status `unclassified` / 0% and does not add `config/matches.csv`, `config/implemented.csv`, `config/reccmp-functions.csv`, or a canonical match unit.
- Maintained natural source in `src/GameManagerSetup.cpp` preserves the existing 0x38 side stride and refines only target-observed packet fields/types needed by this routine: typed 0xA0 side-state pointers; five-word input snapshots; GameManager setup-wait/frame/pause/menu/demo/stage-startup fields; Supervisor loading-state seam; packet-local audio/RNG/spawn/auxiliary call views; and unsigned score/counter fields where target `DIV/JAE/JB` behavior requires unsigned arithmetic. The source uses repeated direct side-state expressions so VC7.1 naturally strength-reduces the score-smoothing loop to the target-like ECX `+0x38` induction and signed pointer-bound comparison.
- Exactness remains false. The first compiling natural source emitted 1,128 bytes. Focused source-shape work progressed through 1,201, 1,216, and 1,231 bytes before the retained target-faithful tail/source organization settled at 1,236 bytes versus the 1,230-byte target under pinned stock VC7.1 `/O2 /Os /Ob1 /Oy- /Gr`. The remaining mismatch is compiler register allocation / input-copy scheduling rather than a known missing behavior block: the target keeps the incoming GameManager pointer in EBX and clears/reuses ESI as zero after the setup branch, while stock compilation retains constant 2 in EBX and keeps GameManager in ESI, producing small instruction/branch-offset drift. `/G6`, `/G7`, and `/GB` probes all preserve the same 1,236-byte result. No `volatile` forcing, TH08 reconstruction-only `var_order`, inert padding, inline assembly, target-byte embedding, fake return, or register-directed source is retained.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was consulted only as lifecycle/source-shape corroboration after TH09 fixed ownership and control flow. TH08 corroborates setup-wait, stage-startup, pause/audio, demo thresholds, and score-smoothing concepts, but its data model and reconstruction-only `#pragma var_order` are not transferred. TH095 did not materially shape this packet.
- Shared IDA metadata was updated under passed `target:th09-main` / `factory-native-stdio` attestation: `0x0041AA5F` is named `GameManager_OnUpdate`, has conservative prototype `int __fastcall GameManager_OnUpdate(void *gameManager)`, and carries an English comment that explicitly records source presence, the 1,236/1,230 mismatch, and unclaimed exactness. The name/prototype/comment were read back. IDA/decompiler observations remain provisional and no target bytes were modified.
- Focused validation compiles the maintained source successfully. The final OnUpdate comparison is `size-mismatch`, object 1,236 bytes versus target 1,230. The four pre-existing canonical functions in the same `GameManagerSetup.cpp` revision remain exact: `AdvanceTimedState` 88/88 with 8 relocations, `AddedCallback` 187/187 with 13, `DeletedCallback` 422/422 with 41, and `RegisterChain` 127/127 with 19. An attempted auxiliary `scripts/check-match-units.py` check is unavailable because that script does not exist in this repository; it is not treated as a passing gate and is superseded by the repository-defined tracking/public-CI checks below.
- Canonical exact totals remain 59 functions / 7,526 authored bytes. Origin inventory becomes 61 authored / 8 excluded / 2,090 pending, and confirmed authored bytes become 9,181. The generated repository status continues to report `source-present / exact: 59 / 59` under its own counting rule; this packet does not alter or reinterpret that metric. Whole Windows i386 closure remains open, runtime validation remains unperformed, and Factory Truth Kernel acceptance remains separate and unclaimed.
- `.analysis/` was 25,124 KiB at this campaign entry and is 26,880 KiB at checkpoint validation. The reused campaign `.analysis/gpt-web/20260911-th09-exact-41b76f/` is 2,456 KiB with 72 manifested reproducible artifacts after the OnUpdate probes and final checkpoint replay; no campaign file exceeds 64 MiB. Current-session object/PDB/JSON/objdump/probe artifacts are retained, and no legacy/unknown analysis state was removed.
- Several Factory transport requests failed before receiving a command id; after each potentially ambiguous operation, live repository status was rechecked and no unverified filesystem effect was assumed. One transient repository-ownership error also cleared on the next status/read attempt. These transport events did not produce a target/provider mismatch; mandatory IDA metadata attestation was re-run after the first target-analysis network failure and remained passed.

Packet balance now ends on a hard, central, source-present/non-exact dispatcher rather than an easy exact win. The next evidence-connected bounded packet may therefore be the compact `GameManager OnDraw @ 0x0041A75F-0x0041A795` (55 bytes), because exact `RegisterChain` proves it occupies the paired draw callback slot. It is selected for direct owner/protocol connectivity after a hard-frontier attempt, not for its size. The measurable stop condition is to reconcile its complete boundary, draw callback return states, GameManager field reads, Supervisor state dependency, and exact Chain callback ABI, then either establish repeatable complete target-bound VC7.1 equality or retain an explicit non-exact/unknown blocker. A later packet should return to a hard frontier rather than accumulate only compact wins.


The fiftieth reviewed packet closes the compact GameManager draw-callback seam at `0x0041A75F-0x0041A795` after the preceding hard `OnUpdate` attempt.

- Recovery began from clean `main` HEAD `4af589af42eee542c685b96d29c3fcd1298d82bf`. At session entry the branch was one commit ahead of `origin/main`; during the packet the shared upstream ref advanced externally to the same commit. This GPT-web session issued no `git push`, and no attribution is made for that shared-ref change. The only tracked edits made by this packet are the OnDraw source, its canonical ledgers/match unit, generated progress, and this handoff. The ignored operator target `resources/th09.exe`, ignored build/tool state, legacy/unknown analysis content, and both adjacent repositories were preserved.
- Packet selection is evidence-connected rather than size-driven. Packet 49 ended on the 1,230-byte hard central `GameManagerSetupLayout::OnUpdate` dispatcher with maintained source but a 1,236/1,230 stock-VC7.1 mismatch. Exact `GameManagerSetupLayout::RegisterChain @ 0x0041B9D0` independently stores `0x0041A75F` in the paired draw ChainElem with `g_GameManager @ 0x004A7D90` as its argument and priority 5, so one compact draw-callback closure was permitted immediately after that hard attempt.
- TH09 target-local evidence closes ownership, ABI, and boundary. Native IDA reports a 0x37-byte function at `0x0041A75F`; it has no ordinary callers or callees, and its sole xref is the data reference from exact RegisterChain. The function ends with `ret` at `0x0041A795`, while independent candidate `0x0041A796` starts at the next byte, leaving no padding or shared tail. The registered callback contract plus the established GameManager layout independently establish authored GameManager ownership rather than relying on address adjacency.
- Behavior is target-derived. Nonzero menu state `+0x13C` is advanced to 2. If Supervisor `+0x590` at `g_Supervisor @ 0x004B3100` is not 2, the callback returns Chain BREAK (3). It also returns BREAK when GameManager transition bits satisfy `(+0x134 & 0x60) == 0x20`. Otherwise `gameplaySetupState @ +0x124` selects BREAK (3) when nonzero or CONTINUE (1) when zero. The target implementation has no external calls.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was consulted only after TH09 fixed the owner and control flow. Its committed `GameManager::OnDraw` corroborates the broad menu/Supervisor/transition/setup-state source shape, but TH09 target values and bit tests decide this reconstruction. TH095 was not used as a source hypothesis; its observed HEAD was `39d4b4253f02f39fa27cc50c13ddc5e8ef6f36bd` with unrelated pre-existing untracked runtime/reconstruction files left untouched.
- Natural maintained source in `src/GameManagerSetup.cpp` is target-exact under the same packet-local pinned VC7.1 `/O2 /Os /Ob1 /Oy- /Gr` profile used by the existing exact GameManager lifecycle/registration units. The first focused compile matched all 51 non-relocation bytes and solved the sole relocation: DIR32 at function `+0x10`, symbol `g_Supervisor`, target base `0x004B3100`, addend `+0x590`. Two consecutive canonical rebuild/replay passes then return 55/55 exact with that same relocation manifest. No padding, target-byte embedding, inline assembly, volatile forcing, fake return, register-directed source, or reconstruction-only compiler wrapper is used.
- Canonical ledgers now record `GameManagerSetupLayout::OnDraw` as authored-game/high-confidence and exact. Tracking becomes 2,159 candidates / 60 source mappings / 60 canonical exact functions; origin inventory is 62 authored / 8 excluded / 2,089 pending. Canonical authored exact bytes rise from 7,526 to 7,581. The same `GameManagerSetup.cpp` revision preserves `AdvanceTimedState` 88/88, `AddedCallback` 187/187, `DeletedCallback` 422/422, and `RegisterChain` 127/127 exact. `OnUpdate` remains explicitly non-exact at 1,236 object bytes versus the 1,230-byte target and is not promoted by this packet.
- Shared IDA metadata was updated after canonical equality: `0x0041A75F` is named `GameManager_OnDraw`, has conservative prototype `int __fastcall GameManager_OnDraw(void *gameManager)`, and carries an English exact-evidence comment. Name/prototype/comment were read back under passed `target:th09-main` / `factory-native-stdio` attestation. IDA metadata remains provisional semantic-analysis state; exactness comes from the canonical replay. No target bytes were modified.
- Repository validation passes `verify-target.py`, `validate-tracking.py --require-target`, reconstruction status, all five affected GameManager canonical match units, generated progress, public `scripts/ci.py`, and `git diff --check`. The faithful Windows i386 whole-build command still returns rc 2 because compile flags, TU partition, libraries, resources, and link order remain unresolved. Runtime validation was not performed, and Factory Truth Kernel acceptance remains separate and unclaimed.
- Three transient Factory requests failed before returning a command id/result: the first OnDraw xref query, an attempted match-unit write/read sequence, and the first IDA metadata readback. After each ambiguity the live worktree and/or target attestation was rechecked before retrying; no unverified filesystem or analysis result was assumed. The external `origin/main` advance also triggered a full recovery review before canonical replay.
- `.analysis/` was 26,880 KiB at session entry and is 26,888 KiB after packet cleanup. The reused campaign `.analysis/gpt-web/20260911-th09-exact-41b76f/` retains the two 512-byte `ondraw-canonical-pass1.json` / `ondraw-canonical-pass2.json` reports and its manifest. The current-session focused OnDraw object, PDB, and preliminary diagnostic JSON were deleted only after their producer commands completed and the canonical reports superseded them. No campaign artifact exceeds 64 MiB, and no legacy/unknown analysis state was removed.

Packet balance now requires another hard frontier: the next evidence-connected candidate is `0x00431110`, whose current native-IDA candidate size is 0x34C (844 bytes). It is a central Supervisor/state-machine routine that directly calls exact `GameManager_RegisterChain` plus thirteen other internal dependencies spanning ANM, input/audio, and state-transition seams. Its owner, ABI, complete physical extent, state machine, data dependencies, and maintained source are still unreviewed. The next packet should first reconcile its entry/exits, adjacent bytes, callers/callees, Supervisor/GameManager state transitions, and the three observed GameManager registration call sites, then attempt natural VC7.1 source only when those target-local facts close. An honest source-present/non-exact or documented-unknown outcome is acceptable; it is selected because it is structurally central and materially larger, not because it is an easy remaining function.

The fifty-first reviewed packet attacks the hard central Supervisor calc-dispatch frontier at `0x00431110-0x0043145B` and expands the candidate denominator when its physical tail exposes previously untracked code.

- Packet 51 began from the clean packet-50 checkpoint `564c48e444c4329401768d3304479d97995c5663` on `main`, with no staged, unstaged, or untracked tracked-work residue. Immediately after packet 50 the local branch had been one commit ahead of `origin/main`; during this packet the shared upstream ref was observed to advance externally to the same checkpoint. This GPT-web session issued no `git push`, and no attribution is made for the shared-ref change. The ignored operator target `resources/th09.exe`, ignored build/tool state, legacy/unknown analysis content, and adjacent repositories were preserved.
- The hard packet was selected because native TH09 evidence makes `0x00431110` a central state machine rather than because of size convenience. Registration at `0x00432180` initializes Supervisor state fields, stores `0x00431110` into a calc ChainElem, and stores `g_Supervisor @ 0x004B3100` as the callback argument. The 844-byte body directly depends on exact `GameManager_RegisterChain`, the GameManager cut seam, ANM/loading/audio services, startup/subthread state, scene-transition registration helpers, and the periodic version check.
- Ownership and ABI are TH09-local. `0x00431110-0x0043145B` is reviewed as the callback's logical code extent. The body writes Supervisor `+0x478`, uses startup/subthread fields around `+0x6AC/+0x6B8/+0x6BC`, compares `wantedState +0x58C` with `curState +0x590`, writes transition state `+0x594`, increments `calcCount +0x454`, and invokes the version check against global Supervisor checksum/size fields `+0x7A0/+0x7A4`. Its return states are 1/3/4/5.
- Physical boundary review is intentionally wider than IDA's function object. The target code ends at `0x0043145B`. Five compiler-associated switch artifacts occupy `0x0043145C-0x004314F3`: 10 dwords, 4 dwords, 6 dwords, an 11-byte compressed case-index table plus one alignment byte, and 15 dwords. Twelve `CC` bytes occupy `0x004314F4-0x004314FF`. These switch artifacts are not added to authored-function byte totals; their physical TU/data ownership remains distinct from the reviewed 844-byte code extent.
- That boundary audit expanded the denominator. `0x00431500-0x0043153D` is real executable i386 code omitted from the initial IDA candidate inventory. IDA does not currently define it as a function, but an attested code xref at `0x0048D8F0` loads `g_Supervisor @ 0x004B3100` into ECX and tail-jumps to `0x00431500`. The bounded entry invokes a helper, constructs the embedded `GameConfiguration @ +0x388`, clears a Supervisor object span, sets bits at `+0x5D4`, returns the input object in EAX, and ends with `ret` at `0x0043153D`; two `CC` bytes follow before independent candidate `0x00431540`. A new 62-byte ledger row was inserted as `unknown/review`. Constructor-like shape, exact ABI/name, authored origin, maintained source, and exactness remain unresolved.
- Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was consulted only after TH09 established registration, fields, state values, and switch topology. It corroborates the likely `Supervisor::OnUpdate` role and broad state-machine organization. TH09 differs materially in transition cases and dependencies, so unresolved scene meanings in maintained source use numeric `STATE_5...STATE_13` and generic registration labels/helpers rather than TH08 semantic names. TH095 did not materially shape this packet.
- Maintained natural source is now present in `src/SupervisorUpdate.cpp`, with only the target-supported `static int OnUpdate(Supervisor *supervisor)` declaration added to `Supervisor.hpp`. Focused source-shape feedback uses the Supervisor-local pinned VC7.1 `/O2 /Ob1 /Oy- /Gr` profile. Target evidence drove the important source corrections: the loading-VM helper is expressed as an AnmManager member call; six absolute zero stores are represented as three neutral float pairs with 0xF0 stride after TH09 xrefs proved that layout; D3D and version-check accesses use global `g_Supervisor`; and nested switch/goto topology follows target state transitions.
- Exactness remains false. The final object `.text` product is 0x3C4 bytes and reaches its first switch artifact at function `+0x32C`; the target reaches its first switch artifact at `+0x34C`, so the maintained source's code is 32 bytes shorter. From those table starts onward, both products contain exactly 0x98 bytes of five switch-artifact segments with identical segment sizes, and the 11-byte compressed case-index sequence `00 05 05 01 05 02 05 03 05 05 04` is byte-identical. One explicit residual is compiler CFG folding: the source contains a separate previous-state 10/13 GameManager cut/re-register branch, but stock VC7.1 folds it into the semantically identical reinit branch; the target retains a second physical block. No artificial side effect, volatile/register forcing, inert padding, inline assembly, target-byte embedding, fake return, or reconstruction-only `var_order` wrapper was introduced. `Supervisor::OnUpdate` remains `unclassified / 0%` and is absent from canonical exact ledgers.
- Shared IDA metadata was updated under passed `target:th09-main` / `factory-native-stdio` attestation. `0x00431110` is named `Supervisor_OnUpdate`, has conservative prototype `int __fastcall Supervisor_OnUpdate(void *supervisor)`, and carries an English comment recording registration, boundary, switch artifacts, source presence, the 32-byte code deficit, and unclaimed exactness. The prototype write had one ambiguous transport failure; re-attestation and decompiler readback showed it had not applied, so it was retried once and then read back successfully. `0x00431500` received only a conservative English comment that it is real code rather than padding and that identity/origin/ABI remain unresolved. No target bytes were modified.
- Adding the `Supervisor::OnUpdate` declaration changed only compiler-internal ChainRun relocation label names. In a 35-unit cold regression covering every configured exact unit whose source directly includes `Supervisor.hpp`, 33 units replayed exact immediately. `chain-run-calc` and `chain-run-draw` failed only their manifest local-symbol-name precheck: relocation offsets/types, external symbols, targets, function bytes, and 212-byte extents were unchanged while the internal labels uniformly advanced by two. An initial broad string replacement over-adjusted four labels and was correctly rejected by replay; an offset-specific correction restored both units to 212/212 exact with zero byte differences.
- Final repository gates pass `build-match-unit.py --check` (70 units), `verify-target.py`, `validate-tracking.py --require-target`, reconstruction status, public `scripts/ci.py`, and `git diff --check`. Tracking is now 2,160 candidates / 60 source mappings / 60 canonical exact functions, with 63 authored origins, 8 exclusions, 2,089 pending candidates, 10,080 confirmed authored code bytes, and 7,581 canonical exact authored bytes. Whole Windows i386 build closure remains open: `scripts/build.py` returns rc 2 because compile flags, TU partition, libraries, resources, and link order remain unresolved. Runtime validation was not performed, and Factory Truth Kernel acceptance remains separate and unclaimed.
- Several Factory requests failed before returning a command id/result. After ambiguous writes or target-dependent reads, live repository status and, where relevant, `get_metadata` attestation were rechecked before retrying. One attempted ChainRun manifest refresh used cascading string replacements, produced an incorrect local-label manifest, and was rejected by canonical replay; it was replaced with an offset-specific correction that replayed exact. No reset, stash, target patch, or push was used.
- `.analysis/` was 26,888 KiB at packet-51 entry and is 26,916 KiB after artifact cleanup. The reused campaign retains a final 17,563-byte `SupervisorUpdateTracked.obj` and a 2,134-byte `supervisor-onupdate-physical-summary.json`. Superseded packet-51 PDB, old objdumps, and preliminary diagnostic output were deleted only after the final summary captured their durable conclusions. No campaign artifact exceeds 64 MiB, and no legacy/unknown analysis state was removed.

The next evidence-connected packet is the newly discovered `0x00431500-0x0043153D` code entry. It is compact but is selected because this hard packet itself exposed it, because the `0x0048D8F0` thunk supplies `g_Supervisor` in ECX, and because it sits directly on the Supervisor construction/layout seam while expanding the reviewed denominator. The stop condition is to close its code-entry boundary and xrefs, recover only target-supported construction/initialization semantics and ABI, determine authored origin independently from the constructor-shaped hypothesis, and then attempt natural VC7.1 source/replay if justified. If TH09 cannot prove the constructor identity or exact source shape, retain `unknown/review` rather than importing TH08's familiar implementation. A later packet should return to another materially large or central frontier rather than accumulating only compact candidates.

The fifty-second reviewed packet closes the newly discovered Supervisor construction entry at `0x00431500-0x0043153D` as authored/source-present while deliberately retaining a one-byte codegen blocker.

- Recovery started from clean shared `main` / `origin/main` HEAD `22a07f486833149ec11578e8c07eaa8e22fe61dc`, with staged / unstaged / untracked counts all zero. This HEAD already contained later shared packets beyond the prior browser handoff: `564c48e` had reconstructed the paired GameManager draw callback and `22a07f4` had added the hard Supervisor update source, so neither was repeated. No recoverable pre-existing tracked work existed. The operator-supplied ignored `resources/th09.exe`, ignored build output, legacy/unknown `.analysis/`, and the adjacent repositories were preserved. A first oversized recovery-shell request failed before returning a command id; status was rechecked before continuing and no filesystem effect was assumed.
- All required repository and Factory v5/recovery/bootstrap/artifact guidance paths were readable. Mandatory native IDA discovery returned complete schemas, and `get_metadata {}` passed for `target:th09-main` with SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822` and `provider_transport=factory-native-stdio`. Repository target/tracking/status/public-CI preflights passed at entry. The early whole-build diagnostic remains explicitly open with rc=2 because compile flags, translation-unit partition, libraries, resources, and link order are unresolved.
- This packet follows the hard 844-byte `Supervisor::OnUpdate` packet, so the compact 62-byte entry is permitted because it was discovered by that hard packet's physical-boundary audit and directly expands the authored denominator. Native TH09 has no IDA function object at `0x00431500`, but a code xref at `0x0048D8F5` belongs to a static-initializer thunk that loads `g_Supervisor @ 0x004B3100` into ECX and tail-jumps to the entry. The target owns `0x00431500-0x0043153D`; `0x0043153E-0x0043153F` are two unassigned `CC` bytes before independent candidate `0x00431540`.
- Target-local construction semantics are now strong enough to classify the entry independently of exactness. The function invokes the VC7.1 four-argument vector-constructor helper `0x00401470` for three 0xF0-byte members rooted at Supervisor `+0x78`, using `0x004343D0` as the constructor callback; it then invokes exact `GameConfiguration::GameConfiguration @ 0x0041A98B` on the member at `+0x388`, clears exactly 0x7B0 bytes from the Supervisor base, sets mask `0x140` in the 32-bit flag storage at `+0x5D4`, returns the input object in EAX, and ends in a plain `ret`. An implicit default constructor cannot account for the explicit full-object clear plus flag initialization, so `0x00431500` is now `authored_game / Supervisor / high` and proposed as `Supervisor::Supervisor()`.
- `0x004343D0` itself remains `unknown/review`. Its target body is exactly `mov eax, ecx; ret`, and a neutral standalone constructor-shaped VC7.1 source reproduces 3/3 bytes twice, but native xrefs show this address is reused broadly across many subsystems. The maintained neutral `SupervisorCtorArrayElement` name is therefore reconstruction-only and does not transfer an original type name, owner, authored origin, or canonical exact claim to `0x004343D0`.
- Adjacent games were consulted only after TH09 fixed the constructor ABI and layout. Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` has a `Supervisor::Supervisor` body that clears the object and initializes two flag bits, corroborating only that broad source shape. Committed TH095 HEAD `58c892a2cdc4945328041d86d52e367e21ab9077` shows a similar lifecycle pattern, but its live worktree has unrelated modified/untracked state; only committed content was read and TH095 names/layout were treated as provisional. TH09 decides the 3×0xF0 member array, `+0x388` configuration, 0x7B0 size, `+0x5D4` flags, boundary, origin, and codegen result.
- Maintained source is in `src/SupervisorLifecycle.cpp`, `src/SupervisorCtorArrayElement.cpp`, and the target-supported constructor layout in `src/Supervisor.hpp`. Defining the 0xF0 element constructor in the same probe TU lets VC7.1 prove it empty and drops the array construction, yielding only 41 bytes; leaving that constructor as an external declaration in the Supervisor constructor TU naturally preserves member construction. Speed-oriented `/O2` emits an explicit 3-element loop and 77 bytes. Size-oriented `/O2 /Os` and `/O1` forms use the target-style vector-constructor helper and emit 61 bytes.
- Exactness remains false for `Supervisor::Supervisor`. The 61-byte maintained candidate has the target-aligned first 47 bytes and three relocation destinations solving exactly to constructor callback `0x004343D0`, vector-constructor helper `0x00401470`, and exact `GameConfiguration::GameConfiguration @ 0x0041A98B`. The sole source/codegen blocker is the final flags update: stock size-oriented VC7.1 narrows `|= 0x140` to `or word ptr [esi+0x5D4], 0x140`, while the target uses `or dword ptr [esi+0x5D4], 0x140`, making the maintained function 61 bytes versus target 62. `/Ob0`, `/Ob2`, `/G6`, `/G7`, `/GB`, `/Oy`, `/Ox /Os`, signed bitfields, and unsigned 32-bit bitfields all preserve the 61-byte form. No volatile forcing, padding, inline assembly, hand-written compiler-helper call, target-byte embedding, or register-directed trick is retained. Two final maintained-source builds repeat the same 61/62 result.
- Expanding `Supervisor.hpp` is a shared-header change, so all 35 configured canonical units whose source directly includes it were cold-replayed. Thirty-three remained exact immediately. `chain-run-calc` and `chain-run-draw` initially failed only because compiler-internal local `$L...` / restart relocation names advanced by 99; every relocation offset/type/external dependency/target address was unchanged. Section-local, offset-specific manifest refresh restored both 212/212 exact. A final 35-unit replay returned zero failures. Canonical exact totals therefore remain 60 functions / 7,581 authored bytes; no match/exact row or match unit was added for `0x00431500` or `0x004343D0`.
- Tracking now reports 2,160 candidates, 64 confirmed authored functions, 8 exclusions, 2,088 pending origin/boundary reviews, and 10,142 confirmed authored code bytes. Repository-generated source-present/exact remains 60/60 under its existing counting rule. Target verification, tracking validation, match-unit graph, public CI, and `git diff --check` pass. Whole Windows i386 build remains open (rc=2), runtime validation was not performed, and no Factory/Truth Kernel acceptance is claimed. No target bytes were modified. No new IDA rename/prototype was forced because `0x00431500` is still absent from the provider function inventory and the widely reused `0x004343D0` original type identity remains unknown.
- `.analysis/` was 27,084 KiB at packet entry and is 28,220 KiB before checkpoint review. The reused `.analysis/gpt-web/20260911-th09-exact-41b76f/` campaign is 3,796 KiB with 138 manifested reproducible artifacts and no file above 64 MiB. Current-session constructor/profile/relocation/regression artifacts are retained; no legacy/unknown analysis artifact, IDB, toolchain, Wine prefix, target, or adjacent-worktree content was removed.

Packet balance now requires a hard frontier. The next evidence-connected packet is `0x00431D10-0x00432081` (0x372 / 882 bytes), not the smaller adjacent `0x00431540` candidate. TH09 `0x00432090` registers `0x00431D10` as a Supervisor startup-thread entry by passing it to the reviewed thread wrapper at `0x00431AE0`, and the body touches established Supervisor state/flag storage. The next packet should reconcile that full thread-entry boundary, startup ownership/lifetime, caller parameter contract, state/error paths, and target-local dependencies, then pursue natural VC7.1 feedback without assuming adjacent-game implementation or exactness.

The fifty-third reviewed packet closes the hard Supervisor startup-thread frontier at `0x00431D10-0x00432081` (882 bytes) canonically while retaining its DirectInput helper as a separate non-exact dependency.

- Recovery started from clean local `main` HEAD `4a6b908386ec949ea7a7e848a93e7f63b54275a0`, initially one commit ahead of `origin/main`. During the packet the shared `origin/main` ref independently advanced to the same checkpoint; this session issued no `git push`, and a full status/diff/untracked recovery review confirmed no concurrent tracked work. The operator-supplied ignored `resources/th09.exe`, ignored build output, legacy/unknown `.analysis/`, and adjacent repositories were preserved. Several Factory calls failed before returning command ids/results; after every ambiguous operation, live status and where relevant native IDA attestation were rechecked before retrying.
- Packet selection satisfies the mixed-frontier rule. Packet 52 was the compact 62-byte Supervisor constructor seam exposed by the preceding hard OnUpdate boundary audit; this packet returns to the required hard frontier. TH09 `0x00432090` passes `0x00431D10` as a thread entry to the reviewed Supervisor `CreateThread` wrapper `0x00431AE0`, establishing startup ownership independently from adjacent games. The supplied thread parameter is never read and the target ends with a plain `ret`, so the original callback calling convention is intentionally left unresolved even though the exact maintained source is compiled under `/Gr`.
- Target-local startup behavior covers DirectInput, MIDI, SoundPlayer, `text.anm`, a heap-owned 0x2A4 startup VM, Ascii/etama/front registration/preload seams, BGM format and volume state, BGM file selection, score initialization, optional dummy MIDI timer creation, and success/error thread-state transitions. Supervisor offsets include `+0x384` dummy timer, config volume bytes `+0x442/+0x443`, startup VM `+0x584`, transient fields `+0x5A4/+0x5A8`, MIDI pointer `+0x5BC`, text ANM `+0x5CC`, flags `+0x5D4`, close request `+0x6B4`, startup state `+0x6BC`, and total-play-time baseline `+0x798`. Successful completion sets GameManager side values at `+0x20` to 0/1 and clears Supervisor flag `0x100`; common failure sets startup state 2 and flag `0x80`.
- A critical TU-local ABI seam was resolved without tricks. Target `0x0042EEF0` receives the Supervisor pointer in EAX and returns EAX. A stock-VC7.1 experiment independently showed `/O2` may rewrite internal-linkage helper ABIs when all callers are visible. Expressing the real TH09 DirectInput setup as TU-local natural C++ makes VC7.1 choose that EAX convention and closes the final StartupThread caller differences. No assembly, naked function, custom wrapper, volatile forcing, fake return, target bytes, or register-directed code is used.
- `SupervisorSetupDInputInternal @ 0x0042EEF0-0x0042F0F4` is independently classified `authored_game / Supervisor / high` and source-present, but remains non-exact at 510 object bytes versus target 517. Its observed target contract covers the DirectInput interface, keyboard, two controller pointers, two 0x2C `DIDEVCAPS` records and the game HWND. Original member/static spelling and the physical/data owner of DirectInput GUID/data-format/string dependencies remain unknown; exact StartupThread does not promote this helper.
- Natural tracked `src/SupervisorStartup.cpp` under pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src` produces the exact 882-byte StartupThread body. The canonical manifest contains 106 reviewed relocations (78 DIR32 and 28 REL32), all with solved target destinations. Two complete canonical rebuild/replay passes are byte-identical and each returns 882/882 exact.
- Physical EH ownership was reconciled rather than hidden. The same object emits a 43-byte `.text$x` with three operator-delete cleanup funclets and the CxxFrameHandler thunk; relocation normalization reproduces `0x0048D380-0x0048D3AA` exactly. Its 52-byte `.xdata$x` likewise reproduces `0x0049DC50-0x0049DC83` exactly with cleanup pointers to `0x0048D380/8B/96` and self frame data. These compiler artifacts are not added to the 882 authored-function-byte total.
- Adding `Supervisor::StartupThread` to `Supervisor.hpp` triggered a cold replay of all 35 pre-existing canonical units whose source directly includes that header. Thirty-three remained exact immediately. `chain-run-calc` and `chain-run-draw` changed only compiler-internal COFF `$L...`/restart symbol names by +2; relocation offsets/types/external symbols/target destinations and function bytes were unchanged. Offset-specific manifest refresh restored both to 212/212 exact, and the new StartupThread unit remained 882/882 exact. The configured match graph is now 71 units.
- Adjacent source was used only after TH09 fixed the owner and control flow. Clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` corroborates the broad StartupThread resource lifecycle but differs in TH09's startup VM and etama/front sequence. Committed TH095 HEAD `58c892a2cdc4945328041d86d52e367e21ab9077` is materially simpler; its live worktree had unrelated modified/untracked semantic/runtime files, so only committed content was consulted. No adjacent address, layout, exactness, or dirty content was transferred.
- Shared IDA metadata was updated under passed `target:th09-main` / `factory-native-stdio` attestation: `0x00431D10` is named `Supervisor_StartupThread` and carries an English canonical-evidence comment. No function prototype was forced because the unused thread parameter leaves the original callback calling convention observationally unresolved. `0x0042EEF0` retains its neutral IDA function name but now carries a source-present/non-exact comment recording the compiler-private EAX ABI and 510/517 maintained-source result. Both comments/name were read back under passed attestation; no metadata write contributes exactness credit and no target bytes were modified.
- Tracking after repository promotion is 2,160 candidates / 66 authored / 8 excluded / 2,086 pending, with 11,541 confirmed authored code bytes. Canonical mappings/exact become 61 / 61 and canonical authored exact bytes 8,463. Whole Windows i386 product closure remains open; runtime validation was not performed and Factory Truth Kernel acceptance remains a separate unclaimed plane.
- Final shared-header validation rebuilt all 35 pre-existing canonical `Supervisor.hpp` consumers and each printed exact. The first aggregate wrapper returned rc=1 only because its report-writer referenced an undefined Python variable after all 35 comparisons had completed; a subsequent compare-only pass wrote `supervisor-startup-final-header-regression.json` and confirmed 35/35 exact with zero failures. The new StartupThread final replay remained 882/882 exact. This scripting/reporting error is not treated as a failed Oracle and is recorded rather than hidden.
- `.analysis/` was 28,220 KiB at packet entry and is 29,588 KiB after final manifest refresh; the reused campaign is 5,164 KiB and the refreshed manifest records the retained reproducible packet artifacts. No file exceeds 64 MiB. Current-session startup/DirectInput/compiler-ABI/EH/canonical reports are retained because they reproduce the exact and unresolved claims; no legacy/unknown analysis state, IDB, target, toolchain, Wine prefix, or adjacent worktree content was removed.

Packet balance is reset by this hard exact result. The next evidence-connected packet should inspect `0x00432090-0x0043217D` (238 bytes), the Supervisor startup-orchestration routine that directly registers exact `Supervisor::StartupThread` through `0x00431AE0`, loads `title/th09logo.jpg` and `nowloading.anm`, coordinates startup state, and passes its own object as the thread parameter. It is selected for direct lifecycle/protocol connectivity after an 882-byte hard frontier rather than because of its smaller size. First reconcile its full boundary, object owner/ABI, resource/error paths, and relation to the independent `0x00432180` Supervisor chain-registration candidate before selecting maintained source.

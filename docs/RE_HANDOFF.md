# TH09 reconstruction handoff

## Current phase

Exact reconstruction: the target and native IDA provider are attested, the
provisional function inventory is initialized, and boundary/origin review has
advanced through seven CRT/library exclusions and nine repository-canonical
exact authored functions in the GameErrorContext, FileSystem, and Supervisor
cohort. The faithful Windows i386 whole-build graph is still open; runtime
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
  deleting destructor are reviewed as exclusions. Nineteen authored functions
  are source-present and repository-canonical exact: four GameErrorContext
  methods, three FileSystem helpers, two Supervisor lock wrappers, seven Chain
  methods, and three ChainElem lifecycle/callback methods. All other imported
  origins remain pending. Factory acceptance remains
  unavailable because the current TH09 adapter has no codegen-exact replay
  driver; no Truth Kernel acceptance is claimed.
- `config/build.toml` exists from day one but correctly reports an open graph.

## Next bounded work

Inspect `0x0042C8F0` next as the immediate post-Chain-run candidate. Establish its TH09-local callers/callees and boundary before deciding whether it belongs to Chain release logic or another subsystem. Preserve all reviewed `CC` gaps and unrelated ownership unknowns. Commit stable local checkpoints; do not push from GPT-web.

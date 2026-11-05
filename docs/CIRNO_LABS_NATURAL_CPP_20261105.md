# Natural C++ source models after the Cirno-Labs integration

PR [#1](https://github.com/N0zoM1z0/th09/pull/1) merged at `1015aa9a` after
remote CI passed. Credits precede License at the end of the README. New
commits use `gpt-6.1-sol:`. Main continues the exact phase at **1019 exact,
16 nonexact**, with no new exact-function credit from this investigation.

## What the compiler comparisons establish

The reference gave us complete alternative source models: indexed engine
loops, ordinary collision objects and real helper implementations. Earlier
models sometimes encoded extra pointer aliases, narrower object scopes or
opaque callees. Those choices can preserve the intended calculation while
changing VC7.1's complete output. A small remaining byte difference does not
prove that the source model is already correct except for one instruction.

The following controls change one source feature at a time. They use complete
current translation units, recursive project-header copies and the existing
manifest profiles; there is no flag search, assembly, barrier or invented
clobber contract. Baseline source is pinned to `ba483304`, whose game-source
Git blobs are unchanged in the merge commit. All observations are compiler/PE
evidence, not recovery of the unique original C++ spelling.

| Owner and control | Whole-owner observation |
| --- | --- |
| Options: retain the real scrolling-helper body | 2045/2045 bytes, 135 fields, 51 calls, 121 blocks, zero differences |
| Options: replace only that body with its existing declaration | 2042/2045 bytes, 135 fields, the same 51 ordered calls and matching direct graph; 1210 unequal overlapping bytes and three missing |
| Type3: retain function-scope collision positions | 780/780 bytes, 38 fields, 24 calls, 17 blocks, zero differences |
| Type3: move just those three declarations into case 0 | 780/780 bytes and the same fields/calls/graph; the old 15 unequal bytes return |
| Front random: replace the first indexed loop with a pointer iterator | Rejected by the existing literal-definition/reference guard; no relaxed comparison or approximate credit |
| SaveReplay: initialize the input-chain cursor in its consuming loop | 1238/1238 bytes, 53 fields and 49 matching blocks; differences worsen from three to 149, so the candidate is rejected |

Options still calls the scrolling helper twice; exposing the body does not
inline it here. The emitted helper independently equals all 42 original bytes
at `0x00423158`, with no relocations. Its original instructions preserve ECX
and EDX and only change EAX among the volatile integer registers. Knowledge of
real register and memory effects is a plausible explanation for the caller's
changed allocation and scheduling. We did not inspect VC7.1's internal passes,
so that mechanism is an inference; the visibility experiment itself is direct
evidence. Raw unequal-byte counts include code-placement cascades and are not
counts of behavioral errors.

The Type3 control is more specific. Both sources keep the same Y, Z and tail-X
statements. With function-scope objects, the middle-Y call is at `+0x18E`,
Z store at `+0x196` and tail-X load at `+0x19D`, matching the original. Moving
the objects into the case makes VC7.1 emit tail-X at `+0x196` and Z at `+0x1A2`.
This reproduces the old scheduling residual without changing the callback ABI
or its 24 calls. It supports the complete collision-object source model; it
does not justify variable-order hints or a general rule to widen every scope.

SaveReplay provides the counterexample: narrowing a cursor's source lifetime
can make an already close function worse. Its upstream-style cursor placement
is not adopted. Likewise, the reference explicitly marks Effect AddedCallback
as still unmatched; it is not an exact replacement for our eight-byte frontier.

## A useful change to an already exact owner

Options now stores its VM pointers and 57 embedded VMs as the shared native
`AnmVm`. Alpha uses `color1Script.a`, drawing uses `drawEnabled`, and the eight
SetSprite calls receive native pointers directly. The redundant padded
`AnmVmView` definition and those eight conversion casts are removed. Native
size and field offsets remain compiler-checked. The unreconstructed help-text
prototype keeps only an opaque argument tag at its boundary.

The full **2045-byte /135-field Options owner remains exact** under the same
profile. This is a source-model improvement, not new function credit.

Whole-carrier review also found a previous integration omission: the
34-byte `TitleScreenView::PlayMenuSound` definition at the end of the file had
been dropped while its canonical unit remained registered. Its earlier body
is restored from `d72fb54b`, and both **34/34 bytes, three fields** and the
Options owner replay exactly in the same fresh object. The earlier ledger
count was not a complete current-source replay of this carrier. The new
focused `--owner options` gate inventories and checks both canonical functions
and verifies the actual visible 42-byte scrolling body. Full public CI checks
that inventory; it does not substitute for the target-bound compiler Oracle.

Final verification also reuses the retained objects for unchanged source/profile
inputs and checks every canonical unit in the twelve reference-adapted source
files: **135/135 comparisons are exact**, with no other missing definitions.
The changed Options carrier is compiled afresh. Full local public CI passes
**1330 tests (29 skipped)**, together with tracking, documentation, generated
progress and whitespace checks. These are bounded source/codegen checks.

## Continue from evidence

Use real native members where TH09 has established their layout and contracts.
Keep object scopes, loop structure and helper definitions in the source model
instead of trying to reproduce isolated register choices. For remaining owners,
try a small, falsifiable source hypothesis and compare the complete owner plus
its exact siblings. Keep a good current source when a cleaner-looking rewrite
regresses or fails a field guard. An upstream percentage is routing evidence.

Reproduce the accepted Options carrier with:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/check-boss-frontiers.py --owner options
```

The local control recipe, copied source/header hashes, complete reports,
compiler logs and object paths are retained below
`.analysis/cirno-natural-cpp-20261105/` (`natural-probes.py` and
`natural-probes.json`). Objects are below `build/analysis/cirno-natural/`.
Fresh direct Ghidra exports attest disk/Program hashes, i386 language, image
base, entry point and all **685056 mapped file bytes** for Options, Type3,
Front random and SaveReplay. Ghidra's discontiguous 1227-byte SaveReplay body
is provisional: the independent physical comparison still includes all 1238
bytes. The separate Effect request found no function at its reviewed entry;
that failed export is preserved and is not a successful analysis receipt.

Reference: [Cirno-Labs/th09 at d7653152](https://github.com/Cirno-Labs/th09/tree/d7653152becabb8c86f2f43d49015ecc11bd0652).
See [the integration record](CIRNO_LABS_INTEGRATION_20261105.md) for the fourteen
adopted owners and the README's final Credits / License sections for attribution.

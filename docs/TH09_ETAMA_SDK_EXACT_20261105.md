# Complete Etama renderer, 2026-11-05

`EtamaController::OnDraw @0x00413BE0` now reproduces all600 original bytes,
all27 address fields,13 ordered calls,157 instructions and17 direct blocks.
The same object retains DrawSingleBullet's232 physical bytes/16 fields and
AddedCallback's832 physical bytes/30 fields, including their owned switch
tails. The actual SDK conversion in that object independently reproduces the
shared three-byte identity body at4343D0. This is one new authored exact
function: totals1020 exact/15 nonexact,259313 of291883 reviewed authored bytes
(88.8414%). Product linking, runtime validation and origin closure stay open.
The local Oracle receipt binds source commit
`9507e12a3400c7936617ed7fb7f3e4fd3b2454ec`; see
[source-bound receipts](RE_ACCEPTED_RECEIPTS.md). It is not a Factory verdict.

## Compiler and source evidence

The session starts at clean `e026e240`. Fresh direct Ghidra attests the original
Japanese1.50a SHA, Program identity, i386, base/entry and all685056 mapped file
bytes before exporting both renderer and helper. The independently pinned
compiler remains VC7.1 build13.10.3077; original PE/Rich build3077 agrees.

The missing distinction was the conversion's inline linkage. A visible
ordinary noninline Float3 conversion makes the reference-shaped parent exact
but changes133 bytes in the already-exact helper. Its first result stays in
EDX across the second conversion. The SDK-style inline definition, with calls
retained under /Ob0, makes both complete owners exact. The COFF section
selection is1 (NODUPLICATES) for the noninline definition and2 (ANY) for the
inline definition and real SDK method. These are compiler observations; the
explanation that replaceable COMDAT calls retain the full caller-saved register
contract is an inference, not a recovered original project setting.

| Controlled model | Complete observation |
| --- | --- |
| Opaque reference-shaped vector calls | Parent680 bytes/34 fields; helper232 exact |
| Visible noninline conversion, /Ob1 or /Ob0 | Parent600 exact; helper232/16 with133 unequal bytes |
| Visible inline conversion, /Ob1 | Parent562 bytes/24 fields; required calls disappear |
| Visible inline conversion, /Ob0 | Parent600 and helper232 both exact |
| Actual SDK D3DXVECTOR3, /Ob0, direct loadedSprite reads | Parent600, helper232 and Added832 all exact |

The final source uses the real toolchain SDK type and its inline implementation;
it adds no duplicate Float3 definition or fabricated callable. Compile assertions
check the SDK/native vector size12 and all XYZ offsets, plus Laser's body VM
position at+208. Both existing exact units and the new renderer use the same
whole-source /O2 /Ob0 profile. The source-only LoadedSprite wrapper is replaced
by its actual native field reads, preserving the complete initialization owner.
There are no register directives, dummy clobbers, assembly, barriers, padding,
per-function flags or target patches.

The SDK method's three-byte body equals the original at4343D0 with no fields.
The complete inspector requires this same-object definition whenever it binds
the SDK symbol. It remains an uncredited logical alias: unique original class,
source ownership and authored/library/compiler origin are still unknown. The
two existing Float3/PlayerPosition aliases and their other callers are untouched.

OnDraw adapts Cirno-Labs/th09 `d7653152`'s loop/expressions with TH09-local ABI,
native layout, callee and original-byte checks. Attribution is in the source
and THIRD_PARTY_NOTICES. The earlier rejected integration and private-ABI
reviews remain dated evidence; this complete replay supersedes their nonexact
Etama conclusions.

## Validation and reproduction

All four canonical units in the renderer's actual object compare exactly.
Fifteen private-label operand spellings retain actual definition/original-PE
destinations. Rebuilding the Bullet dependency with current headers exposed
stale private names; every affected owner and tail is compared before updating
those names. Original targets, offsets, kinds and addends are unchanged.
The Bullet refresh covers16 private operand fields. All24 renderer/Bullet/math
units compare8504 physical bytes/338 fields exactly across five actual object
carriers. The focused Etama replay also passes its complete dependency gate.

```bash
python3 -B scripts/build-match-unit.py --unit etama-controller-on-draw
python3 -B scripts/compare-coff-function.py --unit etama-controller-on-draw --json
python3 -B scripts/compare-coff-function.py --unit bullet-draw-single --json
python3 -B scripts/compare-coff-function.py --unit etama-controller-added-callback --json
python3 -B scripts/compare-coff-function.py --unit etama-sdk-vector-pointer-identity-view --json
python3 -B scripts/check-boss-frontiers.py --owner etama
```

The ignored `.analysis/etama-const-20261105/` retains the frozen initial inputs,
mapped-byte Ghidra attestations, trial recipes, full original-bound reports,
COMDAT selections, both definition-backed label receipts and focused replay.
`verify-current.py` compares all24 canonical units across the renderer,
Bullet source and runtime-angle source. `canonical-source-proof.json` records
their input/profile/object hashes and the relevant SDK-header hashes. Trial
objects are isolated under `build/analysis/etama-const-20261105/`. Only
hash-identical per-trial source/header duplicates are cleaned with a receipt.

Tracking/progress, documentation and whitespace checks pass. Target-independent
CI passes1341 tests with29 skips. This local Oracle result is not a Factory verdict,
remote publication, whole Windows product or runtime/port acceptance.

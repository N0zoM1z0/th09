# Result update uses the implemented ANM providers, 2026-11-06

Recovery starts at clean `0bb32426`; original-target, tracking and status
checks pass. Three fresh local Ghidra batches cover the485-byte result updater,
three ANM providers and175-byte cursor helper. Each attests disk/Program SHA,
x86 little-endian32 language, image base/entry and all685056 mapped file bytes.
Ghidra names/prototypes remain provisional semantic evidence.

Result initialization now calls the implemented native AnmManager LoadSurface,
SetInterruptArray and ExecuteScriptArray through the shared `g_AnmManager`
root at4DC550. The existing native batch-interrupt declaration takes a short;
the old private Title declaration took an int. Native VM casts preserve the
2A4-byte element span, with a size/pending-interrupt-offset guard. The private
Title VM type remains for its other users. Original nominal class names,
member/free-function identity and C++ parameter spelling are not inferred.

| Maintained provider | Original VA | Full bytes / fields |
| --- | --- | --- |
| AnmManager::LoadSurface | 0043CAE0 | 486 /20 |
| AnmManager::SetInterruptArray | 00436DE0 | 72 /3 |
| AnmManager::ExecuteScriptArray | 00439560 | 53 /2 |

Fresh serial builds use four existing source/profile configurations: two for
TitleScreen.cpp, one for AnmManagerLoad.cpp and one for AnmManagerRuntime.cpp.
All41 affected Title units and the three actual providers pass **44 complete
comparisons /19583 physical bytes /978 fields**. Provider objects are isolated
under build/analysis; old Title OBJ/PDB files are preserved before canonical
replacement. Nine compiler-private labels retain actual same-owner definitions
and original PE operands before metadata spelling is refreshed. No profile or
target destination is changed.

The result updater retains every raw candidate byte and every effective field
from its fresh baseline: **484 candidate /485 original bytes**,147 instructions,
34 fields,nine ordered calls,40 identical direct blocks,218 unequal overlaps
and one missing byte. All266 already-equal positions and the actual19-byte
result PNG literal remain protected. Three native global references and three
native method identities replace private aliases, without altering their
original destinations. This is a source-provider correction, not exact closure.

The one-byte deficit begins in character transport. Original reads the selected
byte through CL, sign-extends it into EDI, and preserves that value across the
cursor call. Maintained source computes the table address and sign-extends its
byte directly. The cursor helper's complete target reads its selected index as
a32-bit stack operand; its existing175-byte canonical source remains exact.
No narrower cursor ABI is introduced to change caller code generation.

Six bounded source controls use the same O1 profile:

| Complete source context | Bytes | Unlike overlaps | Missing |
| --- | --- | --- | --- |
| Full pinned Cirno-Labs UpdateState13 adaptation | 483 | 224 | 2 |
| One shared unlock-loop index | 484 | 218 | 1 |
| Captured int character value | 483 | 224 | 2 |
| Shared index and reference-shaped character/loop | 483 | 224 | 2 |
| Native ANM providers, maintained character lifetime | 484 | 218 | 1 |
| Native ANM providers and full reference body | 483 | 224 | 2 |

The four483-byte models have146 instructions rather than147 and fail the
complete protected-byte guard. Shared-loop indexing is neutral. Only native
provider integration is adopted. The pinned GPL reference supplies hypotheses;
all TH09 sizes, fields, calls and comparisons come from the local original.
No barriers, volatile scheduling tactics, assembly, variable-order hints,
padding or profile search are introduced.

The public inspector reports all six native ANM identities; the Title runner
requires their complete order, roots, kinds and zero addends. Six synthetic
contract tests reject wrong aliases with equal numerical destinations, changed
roots/types/addends, missing/duplicate inventories and swapped calls.108 focused
Title tests pass with eight skips. No source-spelling snapshots are added.

Ignored `.analysis/title-result-current-20261106/` retains frozen20/21-file
contexts, complete comparisons, actual COFF fields, Ghidra, commands and logs.
`build-current.py` reproduces the affected builds and44 strict comparisons.
An initial private check incorrectly treated omitted manifest addends as zero;
the existing name-entry unit legitimately has object addend7 and already passed
the full Oracle. That failed bookkeeping attempt is retained separately; the
corrected check preserves the Oracle's original symbol-base/addend semantics.
Source, field, literal and original-byte checks are not weakened.

Totals remain1035 source-present /1022 exact /13 nonexact. Windows product/
runtime, semantic reconstruction, portability and Factory acceptance remain
independent open gates.

Full public CI passes1376 tests with29 skips. Target-required tracking,
documentation, generated progress and whitespace checks pass.

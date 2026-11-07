# Recover the one native full TitleScreen owner across replay drawing and core

Target: Japanese TH09 v1.50a original executable, verified SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a substantial ordinary C++ layout/source-ownership checkpoint,
not proof of original historic class spelling, complete gameplay semantics
or a new byte-exact function.

## Actual single 0x1B388 Title allocation rather than a 0x1B234 overlay

The main `TitleScreen.cpp` previously maintained the full original physical
`TitleScreenView` layout ending at **+0x1B388**. Two independent replay
renderers (`DrawReplayMenu` and the already-exact `DrawReplaySave`) maintained
a separately authored **same-named but truncated 0x1B234-byte struct** in
`TitleScreenReplayView.hpp`. Their fields coincided in the consumed prefix,
but opaque padding concealed real native storage and risked incompatible
cross-TU object definitions.

The complete original-bound Title class now lives in the shared
`TitleScreenReplayView.hpp` and is imported into TitleScreen.cpp itself.
The existing ANM-derived `AnmVmView` definition and native 0xCC-byte
`TitleConfigSnapshotView` live with the shared class, not as duplicate
locally scoped models. Pointer-only `TitleAnmView` and `TitleFloat3View`
are forward-declared in the shared header; their concrete source-specific
implementations and existing exact constructors/callbacks remain in the
original TitleScreen core source.

The two renderers and main Title core now share one actual typed allocation:
50 pointer slots `replayAllocations` at +0x681C; 50 real serialized
0x1EC-byte replay records at +0x68E4; native `currentReplay` at +0xC8FC;
selectedReplay at +0xC908; 57 native 0x2A4-byte ANM VMs at +0x11BA0;
`stateTimer2` at +0x1B230; and the entire config snapshot at +0x1B2BC.
The old opaque `unknown0C8FC` spelling is now replaced by its true replay
pointer in the shared replay slot consistency check. The 50 paths of
0x200 bytes each, two 25-entry columns and ten replay stages are unchanged.

The main core class's actual methods and both independent draw methods are
declared only once. No new allocation, synthetic inherited object or
custom ABI is introduced. The restored layout is explicitly the original
reviewed 0x1B388-byte physical object, not a guessed owner extension.

## Complete source-cold original PE/COFF evidence

A copy-on-write isolated VC7.1 experiment first confirmed that unifying
the full native type preserves all **40 existing exact Title core owners /
18,756 physical bytes / 948 original-bound fields**. The independent
**800-byte `DrawReplaySave` exact leaf** (41 fields) also passes its strict
canonical original-bound comparator with unchanged machine code.
The additional independent 216-byte `score-record-insert` original exact
owner (five fields) is compiled under its separate TitleScreen source profile.
Together, scoped source-cold replay passes **42 complete exact functions,
19,772 physical bytes, 994 relocations**. The independently reviewed
`TitleScreen::DrawReplayMenu` **1,280-byte boss** remains the same
**1,280-byte candidate / 57 original-bound fields / 13 ordered calls /
72 blocks / 29 unlike ordinary bytes**. The full main Title core,
including large state-machine and rendering functions, retains its
original physical bytes and fields; no near-exact boss is promoted.

The header move renumbered **nine compiler-private `$L` labels** in the
original-exact 1,592-byte physical `OnUpdateStartMenu` owner. An independent
complete owner audit verified every actual same-section COFF definition,
owner-relative label offset, original PE field operand/type/addend and
existing target VA before changing only those nine manifest symbol names.
No original destination or strict comparison was changed. Existing Title
source-layout tests were updated to check the recovered full class instead
of requiring the discontinued private 0x1B234 type.

The bounded compile trials, Title core label audit and complete source-cold
reports are retained at `.analysis/gpt-web-title-unified-owner-20261107/`.
No full-project cold replay, Windows product linking, runtime gameplay
acceptance, Factory exactness receipt or remote publication is claimed.
The global ledger remains **1,034 source-present / 1,021 canonical exact /
13 nonexact**.

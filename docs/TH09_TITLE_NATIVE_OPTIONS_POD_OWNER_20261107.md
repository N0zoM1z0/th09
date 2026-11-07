# TH09 native Title Options boss and serialized POD snapshot in one full owner

Target: Japanese TH09 v1.50a original executable, verified SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint restores physical ordinary C++ source ownership; it does
not claim that all nominal class spellings are historically unique or that
a new function has received canonical exactness credit.

## One 0x1B388-byte TitleScreen, including the 2,045-byte options boss

The already complete original-exact **`TitleScreenView::OnUpdateOptions`
(2,045 bytes/135 fields)** lived in a separate TitleScreenOptions.cpp with
yet another *same-named* TitleScreenView class and a duplicate layout
stretching from +0x00 through the 57 embedded ANM VMs and config snapshot.
The independent **34-byte `PlayMenuSound` leaf** was defined in that TU.
The full Title core and ReplayMenu/ReplaySave draw owners now have a real,
shared 0x1B388-byte TitleScreenView; this checkpoint migrates Options to it.

All three core/Options/Replay draw translation units now use the single
`TitleScreenReplayView.hpp` native allocation. `TitleScreenOptions.cpp` no
longer defines an incompatible `TitleScreenView` or an independently named
config member. The shared `TitleAnmView : AnmLoaded` type likewise moved
out of the original Title core source into the native header, so original
ANM sprite calls through the exact `AnmLoaded::SetSprite` target 0x436AC0
are visible to all actual consumers without a shadow owner.

The original options state word at Title +0xC910 and the ready flag at
+0xC918 are now explicit members of the real Title layout, sharing the
existing replay screen frame aliases and the same physical four-byte
storage. The 57 genuine 0x2A4-byte ANM VMs still begin at +0x11BA0, with
no second VM array or newly constructed object.

### Avoid adding GameConfiguration constructors to Title

The 0xCC-byte Title `configSnapshot` at +0x1B2BC is serialized native
GameConfiguration storage, but `GameConfiguration` itself declares a
constructor. Simply embedding a *second* GameConfiguration in the real
Title C++ class would change implicit initialization and potentially
break the original Title construction chronology. The actual snapshot is
now represented as a **trivial 0xCC-byte POD wire type** with the same
original `words[0x33]` raw coverage and explicit byte-accurate
`valueAD` at +0xAD and `windowed` at +0xB1. The separate Supervisor
`config388` remains the real GameConfiguration object. Options continues
to compare these two byte fields in the same branch, while the original
Title core still copies the 0xCC snapshot in the same natural assignment.
No new constructor, inheritance vtable or extra allocation is introduced.

## Original PE/COFF and open replay-menu frontier

An isolated natural C++ source-cold experiment first verified the entire
**2,045-byte Options boss and 34-byte sound leaf** byte-for-byte after
migrating to the native title C++ class, plus the **800-byte exact Replay
Save renderer**. The 40 exact main-profile Title core functions were
independently audited over 18,756 physical bytes/948 fields; adding the
independent 216-byte ScoreRecordInsert profile and the two Options leaves,
scoped strict replay passes **44 complete exact owners / 21,851 physical
bytes / 1,132 original-bound fields**.

The shared native-header change renumbered **nine VC7.1 compiler-private
`$L` labels** in the 1,592-byte physical Title OnUpdateStartMenu carrier.
Every actual COFF field, same-section symbol location and original PE
operand destination was reviewed before refreshing only those local names.
No verified target address, original function/table byte, relocation
addend, compiler profile or strict comparator was changed.

The **1,280-byte ReplayMenu boss** (57 fields/13 ordered calls/72 blocks)
remains nonexact with exactly **29 unlike ordinary bytes**, unchanged.
The 485-byte OnUpdateResult and other open big-boss frontiers are not
promoted by this source/layout improvement. Scoped Title tests verify the
single C++ allocation, genuine ANM fields, and no accidental configuration
constructor. Private bounded recipes are under
`.analysis/gpt-web-title-options-full-native-20261107/`.

The whole-game Windows executable, semantic/runtime acceptance, Factory
exactness receipt, and remote push are outside this checkpoint. No full
cold replay was run. The global ledger remains **1,034 source-present /
1,021 exact / 13 nonexact**.

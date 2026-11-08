# TH09 real GameManager opponent-selection owner across exact leaf and setup boss

Target: SHA-256-pinned Japanese TH09 v1.50a executable
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint recovers an original physical C++ storage/receiver relationship;
unique historic nominal source spelling, full product/runtime acceptance and
newly earned exact-match credit are not claimed.

## The exact 799-byte selector operates on the same native GameManager

The complete `SelectOpponentConfiguration` at original **0x00415910** was
implemented on a duplicate source-only `GameManagerOpponentSelectionView`
containing two partial gameplay sides and a separate `OpponentSelectionState`
view. The original C++ this pointer is the native 0x390-byte
`AsciiGameManagerView`, already used in the real four selection callsites
inside **1,689-byte `GameplaySetupThread`**. The method is now declared
and implemented directly on that native GameManager, and the setup calls
are ordinary `g_GameManager.SelectOpponentConfiguration()` expressions.
No second GameManager, constructed field, alternate ABI or extra allocation
has been introduced.

The genuine four-byte native source offsets now identify a single owner:

- +0xD8: the **same 16 bytes** used as `clearD8[16]` during gameplay setup
  and `usedCharactersD8[16]` while picking opponents. This is an actual
  shared storage relationship, not two independent arrays.
- +0xF0: the live `OpponentSelectionEntry *selectedEntryF0` in the original
  four-byte slot; +0xF4: the selected value; +0xFC: the round counter;
  +0x11C: the original setup mode; +0x138: the native selection index.
- On each original +0x38-sized `AsciiGameManagerSideView`, +0x1C points
  to the real 0xA0-byte `AsciiGameSideRuntimeView`; +0x20 is the character;
  +0x2C is the selected opponent parameter. The original float threshold
  and phase-byte test are read from that real +0x1C runtime allocation.

The 0x10-byte `OpponentSelectionEntry` remains its independent native
read-only table record. The old duplicate GameManager and side prefix
classes have been removed from the former source-facing header, leaving
only that actual table record; no unrelated original class is inferred.
The exact method's score-stat calls, RNG behavior, candidate selection,
used-character updates and branch order are unchanged.

## Whole original leaf and setup big-boss controls

A copy-on-write natural VC7.1 trial proved the new native method compiles
**byte-for-byte identically over all 799 bytes and 32 original COFF
fields** (including every field offset/type/addend). Both the adopted
source and the canonical original target compare as exact. Four original
setup calls continue to target **0x00415910**; the review tools and
function/manifest ledgers now use the real receiver symbol, without
changing any call target or original operand.

The complete **1,689-byte original GameplaySetupThread** remains
**nonexact**, with an unchanged **1,682-byte candidate, 172 bound fields,
35 ordered calls, 78 original/candidate direct blocks, 694 unlike overlap
bytes and seven missing original bytes**. The independently exact
`GameManager::OnUpdate` (1,230 bytes) and setup cleanup (425 bytes)
are protected alongside the exact selector. Targeted source-cold replay
of GameManagerSetup and the native selector passes **12 complete already-
exact owners / 3,790 physical bytes / 275 relocation fields**. Broader
closure including GameManagerMode and the header-dependent FrontMessage
script passed **18 exact owners / 6,854 bytes / 442 fields**, with 30
FrontMessage compiler-private symbols individually checked against their
same-section COFF definitions and immutable original PE operand VAs.

All related target fixtures and diagnostic tools were updated to expect
the new C++ method name while retaining the same original `0x415910`
address, 172 original fields, 35 calls, original physical extent and
strict comparisons. All GameManager, gameplay-setup and FrontMessage
focused regression suites pass. The fixed-profile isolated tests and
original-bound packets are retained at
`.analysis/gpt-web-native-opponent-20261108/`.

No whole-project cold replay, full Windows product link, gameplay semantics
acceptance, Factory acceptance or remote Git push is claimed. The global
ledger remains **1,034 source-present / 1,021 exact / 13 nonexact**.

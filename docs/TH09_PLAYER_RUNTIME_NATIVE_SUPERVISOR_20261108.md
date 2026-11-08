# Native Supervisor controls Player update, drawing and shot time scale

Original executable: independently verified Japanese TH09 v1.50a,
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint recovers source-facing ordinary C++ receiver/data ownership
and does not claim uniquely recovered historic type spelling, newly exact
function credit, a linked Windows product or runtime acceptance.

## One actual 0x7B0-byte Supervisor, no 0x5B8-byte prefix allocation

The original PlayerRuntime maintained an artificial
`PlayerSupervisorRuntimeView` with methods `SelectSide`,
`PreparePlayerDraw`, `ConfigureGameplayViewport`, and a 0x5B8-byte
opaque prefix before its frame-rate multiplier. The real native
`Supervisor` already owns the **+0x5B8** multiplier, identified as
`framerateMultiplier5B8` (same physical float as `gameSpeed`), with all
three separately original-exact viewport/side method destinations.
PlayerRuntime now directly includes `Supervisor.hpp` and uses actual
`g_Supervisor` for four player-side selection calls, the two drawing
viewport calls and both shot-position time-scale reads. No new object,
constructor, vtable, changed reward logic, animation behavior or extra
buffer allocation was introduced.

The already exact functions include **522-byte Player OnUpdate**,
**852-byte physical UpdateBeforeState** (814 source code plus local data),
**166-byte OnDrawHighPrio**, **31-byte OnDrawLowPrio**, and
**251-byte UpdateShots**. The native global remains at original
**0x004B3100**; original `SelectSide` is **0x00401440**,
`ConfigureScreenViewport` is **0x00403D90**, and
`ConfigureGameplayViewport` is **0x00401390**. Field offsets, operand
addends and direct-call targets in every complete exact owner remain
original-bound. The source's actual +0x5B8 multiplier is shared with
the already original-exact `GameManager_CutChain` reset source.

## Whole PE/COFF source-cold proof

A pinned-VC7.1 copy-on-write natural-C++ trial compiled the real
Supervisor receiver and compared all complete old/new machine bytes.
The adopted code's scoped original-bound replay passes **21 existing
complete exact functions / 5,391 physical bytes / 288 relocation fields**
across the affected PlayerRuntime profiles. The larger native header
renumbered exactly 11 compiler-private `$L` definitions in the complete
852-byte UpdateBeforeState and six in the 572-byte physical
InitializeAddedState. Independent PE and COFF auditing verified the
actual same-section label definitions, offsets, operand values and
original absolute targets before updating *only the proven names* in
the strict `config/match-units.toml` manifest. All 17 changes are name-only:
no original PE bytes, function length, target address, field addend or
comparator strictness was adjusted.

Focused Player tests also protect the original exact per-owner global and
callee field offsets and two real shot-time multiplier accesses. Private
source-cold trial/diff/audit receipts are retained at
`.analysis/gpt-web-player-native-supervisor-20261108/`. The independently
nonexact 862-byte Player CheckBulletCollision, 1,900-byte Player movement,
and 1,210-byte ChargeAttack bosses are not promoted by this source
improvement. No whole-project cold replay, game product link, runtime
acceptance, Factory receipt or remote publication is asserted. The global
ledger remains **1,034 source-present / 1,021 exact / 13 nonexact**.

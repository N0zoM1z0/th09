# Native 12-byte Player state setter shared by Enemy, selector and State3 bosses

Original: SHA-256-verified Japanese TH09 v1.50a PE
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint recovers real ordinary C++ receiver ownership and exact
cross-owner call identity, not a proven historic nominal class or new exact
credit; original Windows linking and gameplay acceptance remain open.

## One Player +0x00 update state, not a second four-byte Player

The original **12-byte state setter at 0x00403C20** writes the first
dword of the real Player. The maintained source had attached the method
to a separate `PlayerState4OpsView` containing only the first four bytes,
though all original callers pass a pointer to the real full Player.
`SetUpdateState` now belongs directly to `PlayerLifecycleView` and is
implemented in the same original-backed `PlayerSetUpdateState.cpp` source.
The four-byte role survives *only* for two separately original-bound
transition interfaces which have not yet had their historic member ABI
proven; their declarations and original call targets are preserved.

The source now invokes the native setter in Player Runtime's state machine,
three exact State3 effect setup helpers, and the complete original **3,238-
byte Player selector**. The complete **3,900-byte EnemyManager OnUpdate**
also calls it at original source call offset +0xC06: the original Enemy
reward view is a partial Player-layout role, so its real native Player
receiver remains an explicit `reinterpret_cast<PlayerLifecycleView *>`
from that same side's pointer. A direct reuse of the earlier local `player`
alias changed original literal field positions; that natural source variant
was **rejected** after independent compiler/PE inspection. The retained
expression preserves the retail byte identity, callback chronology,
receiver pointer and call target **0x00403C20**. No inline assembly,
compiler hints, padding or call redirection was introduced.

The existing `FrontMessagePlayerView` separately addresses the *same*
original setter code at 0x00403C20 through a different, independently
maintained source-local interface. Its 0x303C8 timer/score context is not
silently replaced without a separate original-bound source comparison.

## Complete immutable original PE/COFF and negative controls

Prior to adoption, fixed-profile VC7.1 trial builds preserved the **12-byte
setter**, **91-byte State3 effect**, **400-byte PlayerCheckState4** and
**3,238-byte Player selector** whole machine byte sequences; only the
reviewed source-facing setter symbol name changed at existing REL32
positions. After adoption, a targeted cold replay across EclManager,
PlayerGameplayState, PlayerRuntime, PlayerState3EffectSetup, and the actual
setter file passed **110 existing complete exact functions / 39,191
physical bytes / 1,846 original-bound relocation fields**.

An original-PE-bound label audit proved that the changed header and removed
prefix class created **2 ECL, 17 PlayerRuntime, and 18 PlayerGameplayState
private `$L` symbol number shifts**. Each of the 37 source field identities
was checked against a same-section COFF symbol definition, physical owner
relative offset, immutable original PE operand kind/addend and original
absolute target. Only those validated compiler-private names were rebound
in the strict match manifest; source bytes, target values and compiler
profiles remained unchanged. In particular, the independent **346-code /
412-physical-byte selector resolver** still reproduces all 18 original
switch/jump-table address fields, rather than treating an ABI or jump table
as expendable compiler metadata.

Both large nonexact owners were checked separately. **Enemy OnUpdate** keeps
3,900 target/candidate bytes, 97 fields, 69 ordered calls, the same full
direct control flow and precisely **13 ordinary byte differences**.
**RunEcl** keeps 15,564 physical bytes, 598 fields, all jump tables, and
the original 30-byte residual (7 opcode-4 timer-transport bytes and 23
opcode-155 flag-update bytes). The 862-byte Player CheckBulletCollision
is separately protected by its own target-bound diagnostic and remains
nonexact. Source and proof logs are retained under
`.analysis/gpt-web-player-native-update-state-20261107/`.

Only the affected compilation profiles and specified big-boss original-byte
checks were run; no complete project cold replay, Windows game linking,
Factory acceptance, runtime/semantic proof, or remote push is claimed.
The ledger remains **1,034 source-present / 1,021 complete exact / 13
nonexact**.

# Native PlayerOwnerState update reuses real 0x44-byte reward/state allocation

Target: SHA-256-pinned Japanese TH09 v1.50a executable
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is natural C++ storage/call recovery, not new exact credit, original
class-name certainty, complete product linking or runtime acceptance.

## Exact 126-byte owner-state update, without a third partial state class

`PlayerUpdateOwnerState` at **0x0041D760** is an exact 126-byte fastcall
helper, reached from the **522-byte exact Player OnUpdate**. It previously
maintained a third locally scoped `PlayerOwnerStateUpdateView` 0x44-byte
structure, a separate pointer-prefix `PlayerOwnerStatePlayerView`, and a
separate `PlayerOwnerStateSideView::AdjustOwnerMetric` method. The last
method was in fact the already-original-exact **33-byte native
`AsciiGameManagerSideView::AddScore` at 0x0040F860**.

The real `PlayerOwnerStateView` is now complete and already embedded at
Player +0x30410 with 0x44 bytes, native two ZunTimers +0x14/+0x20 and
all reward counters. `PlayerUpdateOwnerState` directly borrows this actual
object from the original `void *` fastcall parameter, preserving the
externally original symbol and stack/ECX ABI. Timer expiry uses
`state->playerOwner00->sideState->AddScore(state->maxValue10)` and clears
native `maxValue10`, `value0C`, `timer14`, status/state counters exactly
as the original did. The same `maxValue10` at +0x10 was previously named
`value10` in this local overlay; it is not a second score storage field.

Three independent pinned VC7.1 natural C++ models (direct real native
call, an explicitly named native side pointer, and native reference) all
reproduced **all 126 original code bytes and nine COFF relocation sites,
field types and addends**. The only source-visible call name change is
from the artificial `AdjustOwnerMetric` to the real `AddScore`; its
original REL32 operand at +0x68 still targets **0x0040F860**.

Source-cold original comparison for the affected PlayerRuntime,
PlayerOwnerStateReward, and GameManagerSideScore carriers passes **23
complete already-exact functions / 6,964 physical bytes / 379 fields**,
including the exact 522-byte Player OnUpdate and complete 1,540-byte
reward state machine. Changing the PlayerRuntime method visibility
renumbered **eleven VC7.1 compiler-private `$L` labels** in the complete
852-byte UpdateBeforeState physical owner. Each actual COFF same-section
symbol definition, original absolute PE operand, and physical byte was
checked before rebinding only those local names in the strict manifest.

The separate nonexact 862-byte `CheckBulletCollision` still has a
849-byte candidate, 26 original fields, 12 calls and 13 missing bytes.
No full-project cold replay, Windows product link, Factory acceptance,
runtime semantic acceptance or remote push is claimed. Trial and strict
proof receipts: `.analysis/gpt-web-native-player-owner-update-20261108/`.
Global ledger is unchanged: **1,034 source-present / 1,021 exact /
13 nonexact**.

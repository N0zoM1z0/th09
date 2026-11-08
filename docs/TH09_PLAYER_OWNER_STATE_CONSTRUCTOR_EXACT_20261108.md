# Newly exact 23-byte TH09 native PlayerOwnerState constructor

Target: SHA-256-verified Japanese TH09 v1.50a i386 executable
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint adds **one complete original-bound exact source owner**.
It does not assert the original C++ explicitly authored a special-member
body rather than relying on indistinguishable implicit compiler code.

## Original native 0x44-byte owner state constructs two ZunTimers

The complete original **23-byte constructor at 0x0041EE30** is reached
by the independently exact 243-byte `PlayerLifecycleView` constructor
at call offset +0xD3, targeting the 0x44-byte embedded owner state
at Player +0x30410. The target uses `push esi; mov esi,ecx`, passes
receiver `this+0x14` and `this+0x20` to the exact
`ZunTimer::ZunTimer @0x00403390`, and returns its original `this`.
Its complete target extent ends with `ret` at 0x0041EE46, with no
post-return jump table or other owned code.

The actual `PlayerOwnerStateView` already has genuine `ZunTimer timer14`
and `timer20` at precisely those offsets, as confirmed independently by
its exact 1,540-byte `ApplyReward`, 41-byte Reset, 126-byte periodic
update, and 16 exact reward callbacks. The ordinary C++ out-of-line
`PlayerOwnerStateView::PlayerOwnerStateView() {}` automatically emits the
target's **23/23 bytes** under original-profile pinned VC7.1 `/O2 /Ob0`,
including precisely two original REL32 timer ctor fields at **+0x07 and
+0x0F**. No artificial assignment or workaround, dummy storage, asm,
volatile, register forcing or compiler-flag roulette is involved.

## Canonical original PE/COFF source registration

`src/PlayerOwnerStateConstructor.cpp` is a new maintained original-bound
source. `player-owner-state-constructor` in `config/match-units.toml`
compiles its complete physical owner using pinned VC7.1 and binds both
original `??0ZunTimer@@QAE@XZ` call operands to 0x00403390. A separate
`compare-coff-function.py` source-cold replay returns **exact 23/23**
with both fields in their immutable original positions. The strict
`functions.csv`, `function-origins.csv`, `implemented.csv`, `matches.csv`
and `reccmp-functions.csv` entries now agree on original address
0x0041EE30, source and owner identity.

The game-specific owner is reconstructed as authored-game for source
tracking, but this is *not* a claim that the original programmer wrote
an explicit empty constructor. The original executable cannot decide
that historic source-authorship question, which remains recorded as
unknown in the maintained function row. This distinction is separate
from unambiguous exact original codegen and function ownership.

Local source-cold original PE/COFF replay of the new source, 1,540-byte
reward boss and full PlayerRuntime owners passes **23 complete exact
functions / 6,954 physical bytes / 381 relocations**, including the
243-byte outer Player constructor. The prior newly exact 28-byte Header
constructor remains independently tracked. The global ledger improves
to **1,036 source-present, 1,023 complete exact and 13 nonexact functions**.
The 13 nonexact functions still account for 29,834 physical bytes; the
large Player, Enemy, Replay and ECL frontiers are unaffected.

Reproducible local evidence is retained under
`.analysis/gpt-web-player-header-three-timers-20261108/`.
No whole-project cold replay, native Windows game product link,
Factory acceptance, runtime semantic acceptance or remote push is
asserted.

# TH09: GameManager_CutChain resets the genuine Supervisor frame multiplier

Original target: Japanese TH09 1.50a, SHA-256 verified
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint restores one native C++ owner without claiming new exact-match
credit, original source spelling, final game link or runtime acceptance.

The original 98-byte `GameManager_CutChain` has ten independently
original-bound relocation fields. At source offset +0x3F it writes
**`g_Supervisor + 0x5B8`** through a DIR32 address field with actual
original base **0x004B3100**, addend **+0x5B8**, immediately before
removing both GameManager chains. That is the same real 0x7B0-byte
`Supervisor::framerateMultiplier5B8` consumed by native Player movement,
projectile integration, ECL runtime, title screen and ZunTimer owners.

The maintained GameManagerSetup.cpp still declared an independent
`GameManagerCutChainSupervisorView` containing 0x5B8 bytes of opaque
padding and one float. This artificial second Supervisor was unnecessary:
its address/offset were already established by the full `Supervisor`.
The fake view was deleted and the code now naturally uses
the actual `g_Supervisor.framerateMultiplier5B8 = 1.0f` member. A compile-time original
layout assertion pins the complete 0x7B0-byte object and +0x5B8 member.
Neither cleanup chain order nor the two side-score publications changed.

Three independently compiled fixed-VC7.1 natural C++ expressions (real
member, reference, pointer) all reproduced the same **98/98 machine bytes
and all ten COFF relocation offsets, types, symbols and addends** as the
existing exact owner. The adopted implementation passes the strict
original PE comparator. A bounded source-cold replay of its original
GameManagerSetup carrier also passes **eleven complete already-exact
functions, 2,991 physical bytes and 243 original relocation fields**.

This source carrier also contains the separate **1,689-byte original
GameplaySetupThread**. Its reviewed **1,682-byte candidate, 172 fields,
35 calls, 694 unequal overlapping bytes and seven missing bytes** remain
unchanged. No nonexact boss has been promoted to exactness. Bounded trial,
original PE and setup-boss receipts are retained in
`.analysis/gpt-web-game-manager-native-cut-chain-20261108/`.

The separately recovered true GameManager opponent-selection owner is
documented in [native GameManager opponent selector](TH09_GAMEMANAGER_NATIVE_OPPONENT_SELECTION_20261108.md)
with an [additional original-PE audit](TH09_GAMEMANAGER_NATIVE_OPPONENT_SETUP_BOSS_20261108.md).

No full cold replay, Windows product linking, runtime acceptance, Factory
receipt or remote publication is claimed. The global ledger remains
**1,034 source-present / 1,021 exact / 13 nonexact**.

# Player reward big owner calls the actual constructed Front transition gate

The original Japanese TH09 v1.50a 1,540-byte `PlayerOwnerStateView::ApplyReward`
at `0x41D150` previously referred to the independently implemented getter
`0x40D4F0` through a declaration-only `PlayerRewardSharedRuntimeView` facade
and `g_PlayerSharedRuntime` at `0x4A7E38`. That source treated the constructed
Front as a second independent object. The real owner is now
`g_GameManager.frontFactoryA8->GetTransitionBlockFlag()`, on the 0x1293C-byte
`FrontFactoryConstructView` constructed by the game's Front factory.

At owner-relative +0x0C the original PE contains the absolute address
`0x4A7E38`, exactly the `g_GameManager` root `0x4A7D90` plus its native
+0xA8 Front pointer. At +0x14 the original call goes to `0x40D4F0`.
Both original operand encodings were directly checked before changing the
source-linked COFF symbols; neither actual call destination nor pointer
value changed. Six compiler-private `$L` relocations also changed their
spellings under VC7.1, but every new same-section definition and its actual
original absolute destination were verified before updating the exact
manifest. No external reference except the two reviewed entry fields changed.

A fresh pinned VC7.1 `/O2 /Ob1` compile and **two independent complete
original-bound comparisons** prove all **1,540/1,540 physical bytes and
91/91 original relocation identities**, including the 1,519-byte code,
one alignment byte and five-entry compiler jump table. The complete
instruction census remains **454**; all **33** original call targets remain
in order. No source register hints, assembly, artificial padding or
alternate compiler switches were introduced.

The independent inspector `scripts/inspect-player-owner-reward.py` now
recognizes only the actual Front getter symbol, not the stale facades.
Eight focused Player reward tests pass and assert the genuine +0xA8
GameManager pointer and +0x14 native method call. This was an exact
function before the source-type improvement: **no additional authored exact
credit** or full cold replay is claimed.

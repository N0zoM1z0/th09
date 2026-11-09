# The original-exact Player damage boss uses native collision callbacks

The complete 996-byte `PlayerLifecycleView::CalcDamageToEnemy` (TH09 1.50a,
0x41FCD0) is already reconstructed on the original full Player and its 128
constructed `PlayerShotCtorView` elements. The leftover `PlayerDamageView.hpp`
still declared duplicate shot, region, timer, ANM, effect, side and entire
Player object layouts, although the only surviving reason to mention them
was a callback-function pointer type.

The legacy callback receiver arguments now use `PlayerLifecycleView *` and
`PlayerShotCtorView *` directly, with the observed three-float
`PlayerDamageFloat3` target position retained to preserve the original
external method's mangled ABI. The full shadow types and redundant storage
have been deleted; the local callback invocation no longer casts its real
Player/Shot objects back into fake allocations. This changes no callback
address, argument register or original memory offset.

Fresh pinned VC7.1 comparison proves `CalcDamageToEnemy` **996/996 bytes,
17/17 relocation fields** and same-source AABB leaf **55/55 bytes, 4/4
fields**. The still-nonexact Enemy boss remains **3,900 bytes, 97 bound
references, 13 nonfield differences**. Local Player, Enemy state and region
tests pass (72 cases in the selected patterns). The canonical exact ledger
remains unchanged and no full cold replay is claimed.

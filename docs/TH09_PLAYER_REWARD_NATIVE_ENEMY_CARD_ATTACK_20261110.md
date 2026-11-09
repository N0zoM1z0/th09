# Original 1540-byte reward state machine uses native EnemyManager and CardAttack

The complete TH09 1.50a `PlayerOwnerStateView::ApplyReward` at 0x41D150
(1519 bytes of code plus 21 bytes of switch/table storage) previously
cast the real opponent side's +0x10/+0x14 objects to independent
`PlayerRewardAttackTargetView` and `PlayerRewardAttackOwnerView` facades.
The exact target owners independently prove +0x10 is the real
`EnemyManagerView` and the method at 0x40F7D0 is
`FindActiveEnemyBySideCategory`, while +0x14 holds the constructed
`CardAttack` allocation with the original `CardAttackStartView::StartAttack`
receiver at 0x403E50.

The C++ reward logic now calls the real `opponentState->enemyManager10`
method and reads the exact native `rewardEnemyCount2AC3B8` member. Attack
calls use the already original-proven `CardAttackStartView` derived
projection of the same 0x1368-byte CardAttack allocation. The two
source-only reward attack owner classes are deleted. Four original PE
call operands at +0xBC/+0xCB/+0xEF/+0x139 were decoded against their
immutable destinations; six VC7.1-private switch labels were rebound only
after their actual COFF section-relative definitions were shown to match
the original PE destinations.

Strict pinned VC7.1 original comparison passes **1540/1540 physical bytes
and 91/91 relocations**; its independent inspector also checks all
454 instructions and 33 ordered calls. The focused reward, CardAttack
and death tests pass. This recovers genuine C++ owner relationships in
already exact code and does not increase the canonical exact count or
assert a complete Windows product/runtime equivalence. No all-owner cold
replay was run.

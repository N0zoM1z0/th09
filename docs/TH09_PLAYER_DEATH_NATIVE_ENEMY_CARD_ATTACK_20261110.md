# Death attack branch uses the actual EnemyManager and CardAttack StartAttack

The 753-byte original TH09 v1.50a death-state method `0x41E420`
previously used independent `PlayerDeathAttackTargetView::CheckMode`
and `PlayerDeathAttackOwnerView::Configure` facades for opponent side
+0x10/+0x14. Independent exact owners at `0x40F7D0` and `0x403E50`
prove these are really `EnemyManagerView::FindActiveEnemyBySideCategory`
and `CardAttackStartView::StartAttack` respectively. The latter is a
bounded derived view with the **same native CardAttack allocation and
unchanged +0x04/+0x08 side pointers**; historic C++ nominal class names
remain provisional.

`EnterDeathState` now calls the true typed `opponentState->enemyManager10`
manager and invokes `StartAttack` on `opponentState->cardAttack14` through
the already-verified `CardAttackStartView` without re-declaring other
prefix objects. The five original REL32 operands at +0x9D/+0xBD/+0xF1/
+0x13D/+0x186 were decoded directly from the immutable retail PE and
verified to retain the same destinations. Fresh VC7.1 strict comparison
passes **753/753 physical bytes, 38/38 relocated fields**. Focused death,
CardAttack and Enemy tests pass. This does not add new exact credit or
run a project-wide cold replay.

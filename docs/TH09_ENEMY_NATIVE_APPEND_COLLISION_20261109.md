# Native Enemy owns the collision-box append leaf and its boss calls

The original TH09 v1.50a `AppendPlayerCollisionBox` at 0x410250 is a
complete 82-byte `thiscall` body. Its receiver's +0x00 GameManager pointer
is the existing `EnemyView::manager00`, not a separately allocated four-byte
`EnemyAppendCollisionView`. The source now implements the method directly
on `EnemyView`; the 3,900-byte `EnemyManagerView::OnUpdate` calls that
native member twice, with no duplicate prefix cast or header. The wrapper
still scales its Float3 hitbox by 1/1.5 and appends to the true Player
collision-query record through `PlayerCollisionQueryStateView`.

The original PE REL32 operands at Enemy OnUpdate offsets +0x7B7 and +0x85C
resolve to unchanged `0x410250`. The complete original-bound VC7.1
comparison verifies **82/82 bytes, 2/2 references**, 30 instructions
and original ordered callees `0x40F5A0` / `0x40F8E0`. The Enemy boss
still emits 3,900 physical bytes with **97 correct fields, 69 calls,
original CFG, and the same 13 nonfield byte differences**. No nonexact
credit was fabricated.

All 77 canonical exact ECL translation-unit functions remain original-
exact (**24,368 bytes / 1,096 fields**). Twenty compiler-private label
spellings were rebound only after their actual same-section COFF definitions
were independently proved to have identical original PE destinations.
Focused Enemy oracle and native-collision mutation tests pass.
No whole-game cold replay or native Windows product/runtime test was run.

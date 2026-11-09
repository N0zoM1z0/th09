# One original GameManager value leaf is shared by death and CardAttack

The retail TH09 v1.50a `0x403E30` 27-byte function is a saturating
subtraction from a signed dword at receiver +0x1C. Its original receiver
in both `CardAttackStartView::StartAttack` (923 bytes, `0x403E50`) and
`PlayerLifecycleView::EnterDeathState` (753 bytes, `0x41E420`) is the
same GameManager base `0x4A7D90` plus +0xB0, with the affected value at
real GameManager +0xCC. Previously CardAttack and PlayerDeath each had a
distinct source-only receiver (`CardAttackStartValueCCView` and
`PlayerDeathCounterView`), and the latter even called the routine `Add`.

One maintained `GameManagerValueCCView` header now declares the bounded
0x20-byte shared receiver; its original nominal C++ identity is still
unknown. It is rooted at `&g_GameManager.valueB0` in both large callers,
which call the real already-authored `SubtractSaturating(500/1500)` method.
The helper source, CardAttack and death compile with the same pinned
VC7.1 profiles. Original PE operands +0x33 (CardAttack), +0x2B4/+0x2B9
(PlayerDeath) were individually decoded: the two address operands remain
0x4A7E40; both call operands retain target 0x403E30. All three complete
functions remain strict original-exact:

- GameManagerValueCC leaf **27/27 bytes, zero fields**
- CardAttack StartAttack **923/923 bytes, 29/29 fields**
- Player EnterDeathState **753/753 bytes, 38/38 fields**

The new original-bound source/manifest tests and CardAttack/Player tests
pass. This consolidates real source-side storage ownership, not historical
class identity or additional exact coverage. No full-project cold replay
or Windows product/runtime closure is claimed.

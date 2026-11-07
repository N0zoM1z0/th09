# Six ExAttack update owners borrow native GameManager and Player

Reconstruction scope is the original Japanese TH09 v1.50a executable,
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Real C++ object ownership is inferred only as far as the original physical
receivers, linked call targets, codegen and independently exact source allow;
we do not claim to know the game's unique historical declaration spelling.

## From local shadows to the native runtime

Six whole ExAttack update methods previously declared separate partial
`ExAttackType...GameManagerView`, side, and Player layouts over the very
same original global at 0x004A7D90. Their source-local `PlayerView` structs
repeated Player collision query storage at +0x36C, the animation resource
pointer at +0xBC, and invented method symbols for collision testing.
Those are not distinct original allocations.

The six update owners now use the shared `AsciiGameManagerView` and native
`PlayerLifecycleView` through the real two-side `GameManager.sides[]` array.
Animation resource reads use Player's native
`header24.anmFile98` path, whose accumulated offset is +0xBC; the real
Player collision query remains +0x36C. The former private view types are
removed, including their separate `g_GameManager` C++ global declarations.
All original gameplay tests, attack state machines, switch tables, resource
calls, and branch chronologies are preserved. The previously unified
`PlayerCollisionQueryStateView::AppendCircleRecord` retains its one real
95-byte method at 0x00440D30.

The affected full original-exact source owners are:

| Function | Original bytes | Actual bound fields |
| --- | ---: | ---: |
| ExAttack Type01 update | 475 | 23 |
| ExAttack Type13 update | 422 | 21 |
| ExAttack Type14/22 update (with table) | 824 | 44 |
| ExAttack Type16/23 update | 496 | 23 |
| ExAttack Type17 update | 420 | 20 |
| ExAttack Type4 update | 739 | 28 |
| **Total** | **3,376** | **159** |

## Complete original PE/COFF and new source-symbol proof

Every changed owner was recompiled with its existing pinned VC7.1 profile.
Independent target-operand checks were performed against the actual compiled
COFF symbol, type, offset and addend, the previously reviewed original
address, and the immutable PE operand. There were **zero unequal physical
bytes** in all six complete owners after binding and **zero unreviewed
field/call changes**. The actual symbol identities changed at **25
GameManager field references** (`AsciiGameManagerView`, target 0x004A7D90)
and **six Player CheckBulletCollision references** (`PlayerLifecycleView`,
original target 0x0041DFF0). The latter target is a separately **nonexact**
862-byte original owner; changing its source identity here does not promote
it. Five compiler-private Type14/22 switch labels were also independently
verified at the same original target addresses before only their names were
updated. The manifest's original field positions, addends, targets, sizes,
profiles, source ownership and all strict comparison rules remain unchanged.

A second, independently retained original-bound audit was run against the
pre-change source revision `6c2569ac` and the six newly compiled objects.
Its 41 reviewed C++ symbol changes comprise the 25 native GameManager
references, six native Player collision calls, five real ANM VM interrupt
calls and five private Type14/22 label names. The separate audit verifies
all six complete original PE owner extents with zero linked-byte differences
and no unresolved fields. Its source-epoch-bound evidence is stored under
`.analysis/gpt-web-exattack-native-player-20261107/full-audit.json`;
this independently corroborates (rather than replaces) the strict canonical
unit-by-unit compiler and PE comparisons below.

Type4 additionally now uses true `AnmVm *` instead of its local VM overlay.
Its five original REL32 calls target the separately exact **18-byte
`AnmVm::SetInterrupt` at 0x00406790**. Its complete 739-byte body and 28
relocation fields remained original-equal, proven independently before
changing its source symbol names. See the dedicated
[Type4 native ANM receiver review](TH09_EXATTACK_TYPE4_NATIVE_PLAYER_ANM_20261107.md).

Fixed-profile scoped original replay passed the **six full ExAttack owners /
3,376 physical bytes / 159 bound fields**. The separately exact two-profile
ANM provider adds seven exact functions, so the selected affected closure
is **13 distinct exact functions / 3,883 physical bytes / 183 fields**.
The stand-alone Type01 disassembler check still resolves all 475 original
bytes, 23 fields, 13 ordered calls and 20 control blocks.
Private reproducible audit sources and reports:
`.analysis/gpt-web-exattack-native-gamemanager-20261107/proof.json`,
`rebind-reviewed.py`, `exact.log`, `type01-special.log`; the actual Type4
VM field audit is in
`.analysis/gpt-web-exattack4-native-player-20261107/native-vm-original-proof.json`.

All affected comparisons were scoped. The global reconstruction ledger
remains 1,034 source-present / 1,021 canonical exact / 13 nonexact. No new
complete authored match, whole-game cold replay, product link, runtime
Oracle, semantic/portability proof, Factory acceptance or remote publication
is claimed.

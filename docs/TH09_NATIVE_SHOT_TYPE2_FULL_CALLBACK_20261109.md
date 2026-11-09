# Type-2 shot update uses the actual constructed Player and Shot objects

The Japanese TH09 v1.50a shot-update callback at `0x441F90` (643 bytes)
was already original-bound exact, but its maintained source repeated a
partial 0x30328-byte Player and 0x484-byte Shot allocation. Its callback
entry retains the original two-argument `__fastcall` nominal ABI while
immediately projecting those parameters onto the **actual constructed**
`PlayerLifecycleView` and `PlayerShotCtorView` types.

The new source uses native `PlayerState3031CView::timer00` and `shot0C`,
Player position +0x1B88, shot position +0x2A4, native 32-element history
at +0x2B0, hitbox +0x430, velocity +0x43C, type/state fields, native
trail count +0x46A and the genuine AnmVm `scale.y` at +0x1C. The final
history element copy borrows a 12-byte constructed vector view without
creating a second allocation. Circle cancellation invokes the already
exact `PlayerLifecycleView::CreateRectType4` at original 0x41D090.

Fresh pinned VC7.1 original PE/COFF comparison passes **643/643 bytes and
16/16 independent relocated fields**. Neighboring 151-byte shot-spawn,
249-byte Type4 update and 61-byte rectangle-producing owners also remain
full-original-exact. Native Player/shot tests and a new native-state test
pass. This is a real C++ ownership improvement to previously exact code;
canonical exact credit and native product/runtime status do not change.
No broad cold replay was run.

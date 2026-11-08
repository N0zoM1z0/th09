# TH09 Type1 shot spawn uses the actual constructed Player state

The exact 151-byte `PlayerShotSpawnCallbackType1` at original 0x00441EF0
previously declared a duplicate 0x1C-byte callback-only state view inside
an opaque Player prefix. The original Player already embeds an actual
`PlayerState3031CView` at +0x3031C, constructed by the independently exact
12-byte member constructor at 0x0041EE20.

The callback now reads/writes the real `PlayerState3031CView::timer00`
(+0x00) and `shot0C` (+0x0C) directly, with its existing external callback
fastcall signature, shot descriptor timing and hitbox/trace behavior
unchanged. `shot0C` is the same native `PlayerShotCtorView *` created by
Player; an explicit type-correct reinterpretation of the callback's
shot-specific structural view is used only at this assignment.

A bounded pinned-VC7.1 source experiment compared two natural forms:
direct member access preserved all 151 bytes and the two original COFF
fields, while introducing an extra `PlayerState3031CView *` local expanded
the candidate to 163 bytes. Only the complete original-exact direct form
was adopted. Original-bound source-cold replay preserves **23 full
existing exact functions, 5,554 physical bytes and 291 fields** across
PlayerRuntime, the new state constructor, and the Type1 shot callback.
The nonexact 862/849-byte Player collision boss retains its 13-byte gap.

This is a native C++ storage recovery, not newly credited exactness.
The retail original TH09 Japanese v1.50a PE has SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Bounded trial receipts are under
`.analysis/gpt-web-native-shot-spawn-state3031c-20261108/`.
No whole-project cold replay, product link or Factory acceptance is claimed.

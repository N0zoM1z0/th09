# Natural native Option and Effect VM access in the Player movement boss

The original TH09 v1.50a `PlayerLifecycleView::UpdateMovementAndOptions`
at 0x41C170 has a 1,900-byte physical extent (1,835 code bytes, alignment
and two switch tables). It already uses the constructed Player and
recorded ReplayInput side slots, but retained redundant `PlayerMovementSideView`
and `PlayerMovementOptionView` typedefs, plus a cast from a genuine
`Effect::vms` pointer back into an `AnmVm` pointer when clearing focus.

The current C++ references the canonical `PlayerSideStateView` and
`PlayerOptionStateCtorView` directly and calls
`player->focusEffect364->vms->SetInterrupt(1)` through its actual typed
`Effect` and `AnmVm` objects. No new allocation, fabricated call target,
ABI/register hints or pre-existing original operand identities are changed.

Twelve additional natural C++ variants were compiled under the pinned
VC7.1 `/O2 /Ob1` profile, exploring the history shift loop, input slot
reference, focused side capture, active effect lifetime, option pointer,
and native vector transfer. The native-focus expression reproduces the
complete previous **1,900-byte/66-field/23-call** candidate with
**122** residual unlike bytes and 435 instruction-alignment matches.
A native history temporary *regresses* to 140 unlike bytes; the direct
replay-side alias fails the strict operand/COFF field census. Neither was
adopted. This owner remains **NONEXACT**, not a new exact function.

The independent exact `player-on-update` (522 bytes, 40 fields) and
`AnmVm::SetInterrupt` (18 bytes, no fields) retain full original equality.
Thirty-six focused movement tests, including negative callee identity tests,
pass. No full cold replay, native-product or runtime-semantic proof is
claimed by this source normalization.

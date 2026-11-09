# TH09 Player death uses native timer, Effect VMs, and reset methods

The complete 753-byte `PlayerLifecycleView::EnterDeathState` at original
0x41E420 has the same receiver/allocation as its own `updateState00`,
`PlayerTimerCtorView timer303C8`, `Effect *focusEffect364`,
`Effect *secondaryEffect368`, `PlayerOwnerStateView ownerState30410`
and `PlayerHeaderStateView header24`. Earlier maintained source projected
all six through unrelated small receiver definitions and pointer casts.

The body now updates `updateState00` directly, assigns `timer303C8 = 0`,
invokes `focusEffect364->vms->SetInterrupt(1)`, deactivates the native
secondary `Effect::active` flag, and calls both real `Reset()` methods.
The null and publication chronology, draw effect handles and return order
are unchanged. The original PE REL32 operands at +0x22A, +0x292, +0x2DE,
and +0x2E6 were directly verified to point to 0x401500, 0x406790,
0x41D7E0 and 0x41BDC0 before rebinding the genuine method names.

Fresh pinned VC7.1 strict replay reproduces **753/753 physical bytes**
with **38/38 relocation fields**, and 61 focused Player death/native
tests pass. This completes the native timer/effect/reset owner cluster
without claiming new exact credit, historic C++ names, native-product
closure, or a full cold replay. The separate reward/attack and counter
facades remain visible and unproven rather than being silently merged.

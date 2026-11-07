# TH09: full Supervisor network-service boss calls actual frame queues

Original target: SHA-256-pinned Japanese TH09 v1.50a PE
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a C++ receiver and full-boss original-bound checkpoint; it is
**not a new byte-exact match** for the 1,633-byte network-service function.

## Real Supervisor queue receiver across complete service branches

`SupervisorServiceUpdate` at **0x00430AA0** is a full 1,633-byte original
fastcall function. It operates on one `Supervisor *` receiver containing
20 native 0x0C-byte frame records at +0x47C, two last-received records at
+0x56C, and the network timers and state at +0x458. After the earlier
recovery of the four exact queue member functions under `Supervisor`, its
source still cast the same pointer to a zero-storage
`SupervisorFrameQueueView : Supervisor` role.

The complete service now calls the **actual `Supervisor` methods** directly:
two `InsertPredictedFrame`, three `InsertReceivedFrame`, four `PopFrame`
and one `AreFrameQueuesSynchronized`. Two received-frame calls use the
original global `g_Supervisor`; the other eight use the saved incoming
Supervisor receiver. Every queue callee, original register/stack calling
convention, reference to seed and predicted-frame outputs, and fixed
original call address is retained. There is no new queue allocation,
changed packet width, defensive bound, queue-order rewrite or fabricated
return status.

## Independent complete compile experiments

Ten ordinary C++ source models were compiled under the pinned original
VC7.1 `/O2 /Ob1 /Oy-` service profile: existing derived alias, real
Supervisor pointer/reference, direct receiver calls, and native global
received-frame calls. **All ten yielded byte-identical 1,627-byte
candidates**, with exactly **106 COFF relocations**, including the same ten
queue field offsets/types/symbols/addends and original destinations.
The independently rebuilt adopted native code also agrees with the old
canonical C++ baseline at all **1,627 bytes and 106 fields**. The full
original PE diagnosis remains **1,633 target / 1,627 candidate**,
**1,381 unlike overlapping byte positions and six missing bytes**.
No nonexact byte was hidden or promoted to complete exactness.

The full exact `Supervisor::OnUpdate` caller (996 physical bytes, 92
fields) is compiled source-cold alongside the four queue leaves. All
**five existing exact functions / 1,568 complete bytes / 93 fields**
strictly match the immutable original. In the caller, changing the
visible native header renumbered **40 compiler-private `$L`/`$title$`
symbols**. An independent real PE/COFF audit verified their same-section
definitions, original target VAs, field kinds and addends before updating
only the manifest spellings. No comparison restriction was relaxed.

Bounded trial and proof outputs are in
`.analysis/gpt-web-network-service-native-queue-20261107/`. Product
linking, gameplay/network runtime acceptance, whole cold replay, Factory
exactness receipt and remote publication remain open. Global status remains
**1,034 source-present / 1,021 exact / 13 nonexact**.

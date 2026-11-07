# TH09 Player State3 effects share real collision-region producer declarations

Target: SHA-256-pinned original Japanese 1.50a PE
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint repairs a **real current-source compile failure**, not a
historical spelling debate or an inferred new accepted exact function.

## Independently check a real regression, then correct the source

Earlier Player reconstruction exposed actual embedded region storage in
`PlayerLifecycleView`, importing the real
`PlayerCollisionRegionCreate.hpp`. The separate
`PlayerState3EffectSetup.cpp` still had its old source-local declarations
of `PlayerRegionPointView` and `PlayerCollisionRegionCreateView`. The pinned
VC7.1 compiler now rejects this TU with **C2011 type redefinition**.
This failure was independently reproduced from the current checkout; the
old successful notes did not account for the newer native Player header.

The fix removes both duplicated local classes and borrows the genuine
`PlayerCollisionRegionCreateView` that lives in the Player allocation.
A native compile-time assertion binds the original Player's embedded region
at +0xB0BC and active region pointer array at +0xB100 to the original
region producer. The three ordinary State3 effect method bodies continue
to call the same `CreateCircleType1` (0x0041CFE0), `CreateCircleType4`
(0x0041D0D0), timer state, and exact `EffectManager::SpawnEffect`
(0x0040CC00) without changing gameplay branches or resource choreography.
The complete 1,210-byte ChargeAttack boss still calls all three State3
methods at their original 0x00403C30/0x00403C90/0x00403CF0 targets.

## Source-cold original PE/COFF evidence

Fixed-profile, independent strict original comparison passes all three
complete State3 methods: **91 + 91 + 100 = 282 physical bytes**, every
original field and callback target. Together with the eight actual region
producer functions, the changed-source closure passes **11 exact functions,
759 bytes and 29 actual relocation fields**. The independently scoped
ChargeAttack big-boss guard still reports **1,210 original / 1,197
candidate bytes, 70 bound fields, 58 ordered calls, 1,160 different
overlap positions and 13 missing bytes**, unchanged. No new exact credit.

The prior duplicate-free Player region type does not depend on separate
runtime allocation or an invented region receiver. A focused unit test
now prevents reintroducing the duplicate declarations, wrong region
coordinates or altered original leaf call targets. Only directly involved
source/profile carriers were rebuilt; **no full cold replay** occurred.

The broader count remains **1,034 authored / 1,021 exact / 13 nonexact**.
No Factory acceptance, runtime-gameplay proof, Windows product link, or
remote push is claimed.

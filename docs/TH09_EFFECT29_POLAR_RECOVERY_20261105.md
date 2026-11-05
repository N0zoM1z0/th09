# Effect29 polar investigation and Type6 source recovery — 2026-11-05

The first adopted checkpoint restores `ExAttackUpdateCallbackType6 @00446060`
to a reproducible current-header build: all 698 bytes and 30 fields match.
This repairs an existing exact unit; totals remain 1021/1035 functions and
259829/291883 reviewed-authored bytes (89.0182%), with14 nonexact functions.
The improved Type29 candidate described below is still private at this checkpoint.

## Target and interrupted source evidence

The session starts from clean `30a2b755`. The original Japanese v1.50a target
passes SHA-256 verification:
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Fresh Ghidra exports of Type29 and Type6 attest the disk/Program identity,
i386 language, base00400000, entry0047D45F and all685056 mapped file-backed
bytes. Ghidra names/prototypes remain provisional semantic evidence.

Following the shared polar callers exposed a real pre-existing failure:
unchanged Type6 source fails current-header compilation. It redeclares the
shared GameManager with an incompatible private type, then accesses the retired
Player `anmFileBC` spelling. A frozen, unmodified source/header control produces
the same errors before any polar-interface migration. The failure is not an
ABI-migration regression and is not silently excluded from the caller cohort.

## Adopted Type6 interfaces

The source removes its conflicting GameManager/side/Player declarations and
uses the already shared GameManager and Player storage. Player's ANM resource
is `header24.anmFile98`, physically Player+BC; a compile assertion checks the
sum. The native `BulletSpawnDescriptorCtorView` constructs the offset-zero
real descriptor, preserving the original28-byte constructor call. Merely
constructing the POD descriptor loses that call and gives687/29, so that
intermediate control is rejected.

The callback now uses five C++ identities that independently predate its own
canonical entry:

| Interface | Original destination |
| --- | --- |
| BulletSpawnDescriptorCtorView constructor | 0040D500 |
| PlayerLifecycleView::CheckBulletCollision | 0041DFF0 |
| AnmVm::SetInterrupt | 00406790 |
| Genuine fastcall AddNormalizeAngle | 0042AED0 |
| Shared GameManager global | 004A7D90 |

All original field offsets, kinds, addends and destinations remain. Full
698-byte/30-field comparison is zero-difference, with19 calls and18 blocks.
The diagnostic rejects the retired private aliases even at the same numeric
addresses. Its shared decoder import is deferred until the private CLI verifies
the target, so the native identity test runs without optional site packages.
No allocation-failure branches or defensive game behavior are added.

The caller, descriptor constructor, seven actual ANM-runtime units across two
profiles, and both angle-runtime units compare exactly:11 units,1359 physical
bytes and60 fields across five actual object carriers. Seven stale private
label spellings in the ANM draw sibling are refreshed only after their actual
same-section COFF definitions and immutable original PE destinations agree.
The entire owned switch-table tail is included. These are existing exact units,
not new exact-function credit.

## Type29 candidate and remaining work

The maintained source initially reproduces2205 of2220 bytes,129 fields and72
ordered calls, with1170 overlap differences plus15 missing. Original target
instructions reload the local center receiver six times between polar conversion
and vector accumulation; vertex receivers are retained. The old member-function
source emits no such local reloads.

Cirno-Labs/th09 `d7653152` provides a source hypothesis in `pl09.cpp` using SDK
local vectors and a two-dimensional static free polar function. This is not
TH09 evidence by itself. A private full-carrier trial reuses the repository's
existing FSINCOS primitive with an explicit vector parameter; its full32-byte
body independently matches the original. No additional primitive instructions,
clobber hints, register annotations or padding are introduced.

With that natural interface, the complete Type29 candidate reaches2220 bytes,
129 correctly positioned fields,675 instructions,72 original ordered calls and
41 matching physical blocks, with92 unlike bytes. All six local reloads and
zero vertex reloads agree. Actual same-object SDK constructor26 and accumulation36
also match their complete original bodies; no added authored/origin credit is
assigned to those aliases. Remaining differences chiefly concern stack homes
and floating-add operand order. Original nominal names/TU ownership stay unknown.

The wider interface migration is investigated privately before adoption.
Its first83 canonical comparisons preserve the complete ECL cohort and the
six-local-vector caller. Main RunEcl raw bytes and all598 relocation identities
remain unchanged. The remaining caller cohort must close before that source
model can replace the maintained Type29 owner. No complete Type29 exactness,
whole-product, runtime, Factory or remote-publication claim follows.

## Evidence and reproduction

Ignored `.analysis/effect29-recovery-20261105/` contains frozen source/header
inputs, the two Ghidra attestations, complete target-bound reports, the original
Type6 compile-failure witness, native repair controls and actual current builds.
`verify-type6-current.py` compares all11 accepted current-source witnesses and
records source/header/manifest/profile/object hashes. Its bound receipt is
recorded separately from the still-private polar migration. Trial objects remain
under `build/analysis/effect29-recovery-20261105/`.

```bash
python3 -B scripts/build-match-unit.py --unit exattack-type6-update
python3 -B scripts/compare-coff-function.py --unit exattack-type6-update --json
python3 -B scripts/inspect-exattack-type6.py build/matching/ExAttackUpdateType6.obj
```

The Type6 source checkpoint passes full target-independent CI:1347 tests,
29 skips, plus target/tracking, documentation, generated progress and whitespace
checks. Local compiler evidence is separate from Factory acceptance.

# Etama real bullet-pool sentinels and large-owner bounds — 2026-11-07

Source of target identity: Japanese TH09 v1.50a
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint implements ordinary C++ source in the **1,932-byte
`EtamaController::SpawnSingleBullet` and 2,216-byte `OnUpdate` big bosses**, as
well as the 173-byte `Initialize`, 95-byte `ResetProjectiles`, and two
pattern spawners. No compiler option, assembly, synthetic register hint,
target patch, or unsupported original class identity is introduced.

## Native storage relationship

`EtamaController` physically owns two adjacent bullet arrays and a laser
array. The exact initialization/spawner owners independently attest the two
last-record sentinels (`state == 6`). The true allocation and iteration
bounds are **not equal**:

| Owned object | Physical records | Usable records / role |
| --- | ---: | --- |
| `primaryBullets` | 176 | 175 usable; primary index 175 is sentinel |
| `secondaryBullets` | 361 | 360 usable; secondary index 360 is sentinel |
| `lasers` | 48 | 48 inspected |
| OnUpdate linear bullet walk | 536 | All 176 primary records (including its skipped sentinel) followed by the first 360 secondary records |

The new `src/EtamaNativePoolLimits.hpp` derives the three native array
sizes, usable counts, and contiguous linear scan count from actual members,
not another shadow memory model. Compile-time layout checks pin the
record cardinalities and the two array adjacency boundaries. The C++ enum
constants preserve the original *signed* integer loop comparisons rather
than accidentally introducing unsigned `size_t` branches.

`EtamaControllerInit.cpp` previously contained interrupted uncommitted
native-limit changes. They were **retained, reviewed, and verified**, not
ignored. Both complete original-exact init/reset functions compile and
compare unchanged. The same capacity type is now consumed by the large
`BulletManager.cpp` handlers for search lengths, return sentinels, the
536-record update, the primary-versus-secondary count, and the 48 laser
records. The original logic's sentinel skip and loop order are untouched.

## Original-bound compiler evidence

Isolated source-cold VC7.1 controls compared the unchanged and modified
`BulletManager.cpp` over **all 18 source functions**, including original
branch-table tails, **6,711 complete physical bytes and 259 relocations**.
All three naturally written variants (baseline, spawner-bound only, and
complete shared capacities) preserve all original bytes/fields under the
unchanged pinned compiler profile. The two additional initialization
functions add 268 exact physical bytes. A fresh live-source replay verifies
**all 20 directly affected exact functions: 6,979 bytes / 259 fields**.

The additional actual 1,320-byte `Bullet::AdvanceTransformProgram`
compile produced seven previously stale VC7-local `$L...` label names. Each
was verified against its real same-section COFF definition and the original
PE target at the same operand offset; the code, 26 original fields,
branch/table bytes, target VAs and external symbols remain unchanged. The
18 BulletManager functions similarly required 16 source-local label-name
rebindings. All 23 name-only changes passed private strict original-bound
comparison *before* being adopted in the canonical manifest. No byte
matching gate or canonical source row was weakened.

Finally, the normal focused `--owner bullet` replay compiled the real
associated owners under their pinned profiles. **30 already exact functions,
21,091 complete original bytes and 764 relocation fields**, all pass.
This is a scoped replay, **not a full all-owner cold replay**. It does not
create a new exact function, whole-product linkage, runtime/semantic proof,
portability evidence, Factory acceptance, or remote publication. The
classification stays 1,034 source-present / 1,021 exact / 13 nonexact.

Source/control/provenance records are retained under
`.analysis/gptweb-etama-native-pools-20261107/`, including baseline and
modified actual compiler objects, 20-body and transform original-bound
private manifest proofs, hashed source/object inputs and scoped production
build/replay results. The original executable remains ignored and unchanged.

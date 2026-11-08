# TH09 3,900-byte Enemy OnUpdate: native GameManager side for Etama spawn

The exact target remains independently verified Japanese TH09 v1.50a PE,
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a semantic C++ object-owner recovery **inside an open nonexact boss**,
not a new byte-exact function, runtime proof or final product-build claim.

## Original hot-branch receiver is the true +0x38 gameplay side

`EnemyManagerView::OnUpdate` at **0x00410730** has a complete original
3,900-byte physical extent, 97 independent original-bound relocation
fields, 69 ordered calls and 170 matching direct CFG blocks. Its special
attack path builds a real 0x214-byte `BulletSpawnDescriptor` and dispatches
to the original exact `EtamaController::SpawnBulletPatternPrimary`.

The maintained C++ previously obtained the side character at +0x20 and
Etama controller at +0x08 through a separate `EnemySideStateView` source
class. These physical slots already have a common native producer and
consumer: the real **0x38-byte `AsciiGameManagerSideView`** used by
GameManager setup, ECL, Etama and the exact Player/score owners.
The special spawn branch now explicitly borrows that true gameplay side
as an `AsciiGameManagerSideView &spawnSide`, through the existing
`EnemyManagerView::sideState320` pointer at manager +0x320.
Its `character20` and `etama08` members supply the same original +0x20
short-count selection and +0x08 call receiver. Compile-time assertions
independently check both side layouts and exact member offsets.
No second side allocation, changed Etama call argument, added virtual
method or reinterpreted descriptor layout is introduced. Other Enemy
side projections remain provisional outside this reviewed branch.

## Non-crediting complete-boss experiments and exact siblings

Ten natural, serial pinned-VC7.1 C++ experiments were run on the whole
3,900-byte callback, not a synthetic leaf: native side reference,
reference declared before descriptor copy, side pointer, direct native
field access, a native position pointer, native array indexing and
threshold lifetime variants. Seven retained the complete original
3,900-byte length, **97/97 original fields**, **69/69 ordered call
identities**, the same 170 CFG blocks and the same **13 byte differences**
at the already-reviewed drawing index/descriptor-copy register sites.
Three variants involving changed character or position materialization
were rejected by strict original-bound literal/field diagnostics.
Only the fully original-bound direct native side reference was adopted;
there is no new byte-exact credit and no regression in the residual.

The shared `EclManager.cpp` source carrier remains original-exact for
**83 complete previously exact sibling functions / 29,856 physical bytes /
1,389 relocation fields**. The changed C++ declaration renumbered two
compiler-private ECL `$L` referents. Their actual same-section COFF
symbol-definition addresses and original PE operands were checked
independently before updating only the corresponding match-manifest
names; no original field address, addend or comparator strictness moved.
The original Enemy/ECL scalar-copy diagnostic was kept operational and
updated to target the real native side, with its unique-source checks
and original byte/layout guards preserved. Enemy and ECL tests pass.

The still-open **3,900/3,900-byte Enemy OnUpdate** has 13 ordinary unlike
bytes, the **862/849-byte Player CheckBulletCollision** has 13 missing,
and **15,564-byte RunEcl** retains 30 unlike ordinary bytes. This
checkpoint does not promote any of those bosses to exactness.

Full original PE/COFF and natural-source receipts:
`.analysis/gpt-web-enemy-native-side-nov8/`.
No full cold replay, linked Windows game product, runtime acceptance,
Factory receipt or remote push is asserted. The global ledger remains
**1,037 source-present / 1,024 exact / 13 nonexact**.

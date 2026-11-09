# Enemy life/timer folded callback arguments are native Enemy pointers

Enemy OnUpdate at 0x410730 invokes two distinct source-facing callbacks
at original-relative +0x6E8 and +0x6FF. Both calls resolve to the same
original TH09 1.50a folded leaf 0x44AB20 (`33 C0 C3`), while the
maintained VC7.1 carrier emits two independent three-byte helper bodies.
Their previously opaque `EnemyCoreView*` parameter has been retyped as
`EnemyView*`, which removes two needless casts in the 3,900-byte caller.
The original nominal callback interface and whether the two entry roles
were distinct in the historical source remain unknown.

The full Enemy OnUpdate stays 3,900 bytes with 97 original-bound fields,
69 ordered calls, unchanged CFG, and 13 residual nonfield bytes. All 77
exact ECL siblings (24,368 bytes, 1,096 fields) remain exact. Two changed
compiler-private local labels were rebound only after their same-section
values matched the original PE destination. Focused Enemy tests pass.

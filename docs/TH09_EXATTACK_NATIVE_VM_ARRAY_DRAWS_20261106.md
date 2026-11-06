# ExAttack draw loops: native ANM VM array elements (2026-11-06)

Both original TH09 ExAttack draw callbacks traverse a field at record +0x1C.
The original-bound 0x4C-byte record has a genuine `AnmVm *vms1C` pointer
and a `vmCount2C` count. The earlier reconstruction computed each VM pointer
by repeatedly casting `(unsigned char *)vms1C + vmOffset` and checked a
manually reinterpreted 16-bit word at `VM + 0x1FC`.

The recovered source now uses the natural `record->vms1C[vmIndex]` element
for `type`, `pos`, `pos2`, rotation and `Draw2D`. The redundant `vmOffset`
byte counter and all corresponding raw pointer casts are gone. The VM type
field is the existing native `AnmVmBase::type` at +0x1FC; compile-time
offsets also check the record's +0x1C pointer and `sizeof(AnmVm)==0x2A4`.
No original call, side traversal order, vertex update or draw behavior changed.

VC7.1 source experiments tried ordinary native indexing, alternate equivalent
index pointer forms, and a separate iteration-local VM pointer. The indexed
forms compiled into the **exact original complete 286-byte OnDraw0 and
193-byte OnDraw1** functions, with 9 and 5 respectively original-bound
relocation fields. The index-pointee-local candidate instead changed the
function extents to 266 and 181 bytes; it was not adopted. The currently
canonical `ExAttackDrawType19_21` callback using the native VM alpha member
also remains exact at 318 bytes / 13 fields under its separate profile.

Reproduction is limited to the affected units:

```bash
python3 -B scripts/build-match-unit.py --unit exattack-on-draw0
python3 -B scripts/compare-coff-function.py --unit exattack-on-draw0
python3 -B scripts/compare-coff-function.py --unit exattack-on-draw1
python3 -B scripts/build-match-unit.py --unit exattack-type19-21-draw
python3 -B scripts/compare-coff-function.py --unit exattack-type19-21-draw
```

These owners were already fully exact against Japanese v1.50a SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The source has become more natural and better typed, but this is no new
match credit, product-build/runtime claim, Factory acceptance or remote push.
The native source trials are retained in ignored local
`.analysis/gpt-web-exattack-native-vm-20261106/`.

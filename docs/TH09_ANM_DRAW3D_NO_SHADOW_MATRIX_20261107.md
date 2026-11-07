# ANM Draw3D: eliminate last VM shadow while retaining exact SDK matrix codegen

The pinned original Japanese TH09 v1.50a 3D renderer already received an
`AnmVm *` from the real script VM allocation. The prior maintained source
still carried a *second* `AnmVmDraw3DView` class purely to expose three
`D3DXMATRIX` slots. That duplicate type replicated offsets, padding and
other state that were independently present on `AnmVmBase`.

The follow-up source now removes the entire shadow VM type and its offset
aliases. All three matrix operands come directly from `vm->matrix1`,
`vm->matrix2` and `vm->matrix3`. At SDK call and local matrix-operation
sites, a `reinterpret_cast<D3DXMATRIX &>` provides the vendor library's
4x4-float ABI view. Compile-time checks pin the native matrices at +0x130,
+0x170, and +0x1B0, and confirm that the source `AnmVmScriptMatrix` and SDK
`D3DXMATRIX` both occupy **0x40 bytes**. Their original nominal C++ types
remain unproven; the representation compatibility is all this source claims.

A focused pinned VC7.1 trial tested **three distinct source shapes** with no
shadow VM: direct pointer dereference, direct SDK reference casts at each
use, and a pointer to the first float of each native matrix. **All three
reproduce the complete original 811-byte function**, including all 26
previously reviewed relocation fields. By contrast, a long-lived named
`D3DXMATRIX &` local caused an 813-byte body and was not adopted. The
current code uses direct SDK reference expressions, preserving the actual
matrix multiply sequence and original calls, and no new allocation or
synthetic helper.

Reproduce only the affected owner:

```bash
python3 -B scripts/build-match-unit.py --unit anm-manager-draw-3d
python3 -B scripts/compare-coff-function.py --unit anm-manager-draw-3d
python3 -B -m unittest discover -s tests -p test_anm_draw3d_native_vm_fields.py
```

The exact function had already been credited in earlier reconstruction work.
No change to original executable bytes, manifest, compiler profile, match
count, whole-game cold replay or Factory acceptance is asserted. Local
trial objects/sources remain at
`.analysis/gpt-web-anm-draw3d-matrix-20261107/`.

Target SHA-256: `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.

# TH09 3D rendering — native VM and ANM shake fields (2026-11-07)

The 811-byte original Japanese TH09 v1.50a `AnmManager::Draw3D` function
shares its real `AnmVm` object with primitive rendering and the animation
interpreter. The maintained source previously read rotation, scale, sprite
size, UV scroll, flags, position and sprite pointers through a second
`AnmVmDraw3DView` structure. It also read the ANM screen shake through a
second manager view despite the real manager owning the two native floats.

The current C++ reads those VM fields directly via its `AnmVm *vm` parameter,
and uses `this->frameShakeX1C`/`frameShakeY20` for the manager-owned shake.
Compile-time offsets check the real VM's rotation `+0x000`, scale `+0x018`,
sprite size `+0x028`, UV scroll `+0x030`, three script matrices `+0x130`/
`+0x170`/`+0x1B0`, flags `+0x1F8`, position `+0x208`, and sprite `+0x224`.
The old `D3DXMATRIX` projection remains **only** for matrix operations:
replacing it with a fresh `D3DXMATRIX&`-to-native-script-matrix alias changed
the 811-byte body to 813 bytes, so those nominal matrix types have not been
conflated without codegen evidence.

Six natural source variants were actually compiled with the pinned VC7.1
profile. Native VM fields alone, native ANM shake alone, and the combined
form all preserve the **entire 811-byte original owner**, including its 26
actual COFF relocation fields. The three versions with additional native
matrix aliases produced 813-byte owners and were rejected. The combined
source was built and checked again after adding the original-field assertions.

This source-reconstruction checkpoint does **not** add a newly exact
function—the owner was already exact. It changes neither original PE bytes
nor compiler profile and does not imply whole-product/runtime, remote push,
or Factory acceptance. The ignored local trials remain under
`.analysis/gpt-web-anm-draw3d-native-20261106/`.

Focused reproduction:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/build-match-unit.py --unit anm-manager-draw-3d
python3 -B scripts/compare-coff-function.py --unit anm-manager-draw-3d
```

Target SHA-256: `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.

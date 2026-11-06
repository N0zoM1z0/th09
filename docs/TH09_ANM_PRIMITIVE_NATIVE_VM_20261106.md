# TH09 ANM primitive drawing: one native VM allocation (2026-11-06)

The original Japanese TH09 v1.50a ANM renderer's primitive paths receive
`AnmVm *` objects from the same 0x2A4-byte animation-VM allocation as the
script interpreter, Draw3D, Title draw, and ExAttack draw code. The maintained
primitive source had an independent `AnmVmPrimitiveView` composed of raw
padding to VM `+0x1F3` (alpha), `+0x1F8` (flags), and `+0x224` (loadedSprite),
then cast the actual `AnmVm*` to this second layout in multiple callers.

The current C++ **removes that redundant shadow structure**. DrawVertices,
QueueSpriteQuad and DrawTexturedTriangleFan use the actual native `AnmVm`
object's `flagsWord`, `color1Script.a`, and `loadedSprite->texture` fields.
Compile-time native layout assertions check alpha +0x1F3, flags +0x1F8,
loaded sprite +0x224, and the complete 0x2A4-byte VM allocation size.
No second VM is allocated or constructed, and no register hint, inline
assembly, extra method or hidden byte patch is introduced.

The original pinned VC7.1 profile was used for **five concrete source
models**: native alpha only; an `AnmVm*` local in place of the shadow view;
direct `vmObject` access; and both versions with the entire shadow layout
removed. All five preserve the same 12 complete source-owner functions,
not merely the touched code regions. The adopted direct-VM/no-shadow version
matches **all 1,763 original physical bytes and 90 original-bound COFF
fields**. This includes the 238-byte DrawVertices, 163-byte QueueSpriteQuad,
200-byte DrawTexturedTriangleFan, 318-byte DrawTriangleStrip/Fan, 472-byte
SetupVertexBuffer, and six smaller state-clearing leaves. The canonical
relocation symbols and match unit manifest are unchanged.

Focused reproduction, not an all-game cold replay:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/build-match-unit.py --unit anm-manager-draw-vertices
python3 -B scripts/compare-coff-function.py --unit anm-manager-draw-vertices
python3 -B scripts/compare-coff-function.py --unit anm-manager-queue-sprite-quad
python3 -B scripts/compare-coff-function.py --unit anm-manager-setup-vertex-buffer
```

All twelve match units live in the same just-built original compiler carrier,
and can be checked separately through `config/match-units.toml`. The target is
Japanese TH09 v1.50a SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Failed and accepted compiled C++ model packets are preserved locally under
`.analysis/gpt-web-anm-primitive-native-20261106/`.

These owners were **already exact**. This is real source/data-type
reconstruction and simplification, not new credited exactness, original
nominal class-name proof, Windows product build, runtime acceptance, Factory
approval or remote publication.

# TH09 ANM background / Title texture-cache native view (2026-11-06)

The real Japanese TH09 1.50a ANM manager is a single allocation shared by
background rendering, low-level primitive drawing and Title presentation.
The complete original-bound `TitleDrawCallback` begins by invalidating the
cached texture pointer at absolute ANM owner offset **+0x12880** before
copying the surface to the backbuffer. The maintained Title source previously
used a raw `unsigned char* + 0x12880` write despite the independent exact
`AnmManager::SetCameraMode`/`SetMixColor` background owner already having a
typed native view of that same offset.

The new shared `AnmManagerBackgroundView.hpp` holds that **one observed
source-facing prefix**, with original manager `mixColor` +0x00,
`useMixColor` +0x04, cached `currentTexture` +0x12880 and
`cameraMode` +0x12888. Both the existing ANM Background code and Title
include the same declaration. Title now writes
`reinterpret_cast<AnmManagerBackgroundView *>(g_AnmManager)->currentTexture
= NULL`, not a pointer-arithmetic cast to an unrelated `void**`.
Compile-time `offsetof` assertions verify +0x12880/+0x12888; this is
**not** a second allocated ANM manager or proof of the original nominal
class/TU name. The original texture-cache invalidation still happens once
before the same original surface-copy call.

## Focused original-bound proof

Only two affected existing VC7.1 compiler carriers were rebuilt. Every
original physical byte and actual COFF field was compared:

| Unit family | Original exact owners | Whole physical bytes | Fields |
| --- | ---: | ---: | ---: |
| ANM background (mode, mix, draw/flush, GetAnm) | 5 | 96 | 2 |
| TitleDrawCallback | 1 | 329 | 20 |
| **Total** | **6** | **425** | **22** |

There is no compiler-private label adjustment, target-byte patch, new
register hint, inexact code accepted as exact, or full-project cold replay.
These six owners were already exact in the canonical source; the change
recovers stronger C++ data ownership only.

Local reproduction:

```bash
python3 -B scripts/build-match-unit.py --unit anm-manager-get-anm
python3 -B scripts/build-match-unit.py --unit title-draw-callback
python3 -B scripts/compare-coff-function.py --unit title-draw-callback
python3 -B scripts/compare-coff-function.py --unit anm-manager-get-anm
```

Other ANM Background exact units share the first match-unit's compiled object
and can be checked separately through `config/match-units.toml`. The target
is Japanese v1.50a SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
No Factory, native-product/runtime or remote acceptance is claimed.

## 2026-11-07 follow-up: one render-cache prefix across draw and title

The earlier `AnmManagerBackgroundView` was subsequently made an ordinary C++
`typedef` of `AnmManagerRenderCacheView`, the native cache prefix shared by
PrimitiveDraw and Draw3D as well as Background and Title. Its actual
observed mix-color, texture, shader, depth-write, camera and shake offsets
remain unchanged. All 19 affected original-bound owner functions pass
complete comparison. See
[ANM render-cache owner consolidation](TH09_ANM_COMMON_RENDER_CACHE_20261107.md).

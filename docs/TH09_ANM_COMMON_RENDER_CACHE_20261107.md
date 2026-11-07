# TH09 common ANM render-cache owner across four source units (2026-11-07)

Previously the maintained TH09 reconstruction had overlapping native ANM
cache views in `AnmManagerPrimitiveDraw.cpp`, `AnmManagerDraw3D.cpp` and
`AnmManagerBackgroundView.hpp`. These views referred to **the same real
ANM manager allocation**. The code now shares one
`AnmManagerRenderCacheView` prefix with the actual target-bound C++ layout:

| Native owner field | Observed offset |
| --- | ---: |
| mix color / use-mix-color | `+0x000` / `+0x004` |
| frame shake XY | `+0x01C` |
| current texture | `+0x12880` |
| blend, color operation, vertex shader, depth write | `+0x12884` through `+0x12887` |
| camera mode | `+0x12888` |
| next derived cache member | `+0x1288C` |

`AnmManagerPrimitiveView` and `AnmManagerDraw3DView` inherit this prefix
and retain their genuine trailing sprite, vertex buffer and draw-count
fields. The Background/Title-facing `AnmManagerBackgroundView` is simply
a typedef for that same prefix; its original source name is retained as
an alias, **not a second allocated object**. The shared state uses the
actual Direct3D texture pointer type and preserves the necessary source
cast in Draw3D, with no change in machine code.

A failed first compile exposed a *real source include dependency*: the
shared prefix needed the actual `Float3.hpp` definition of `Float2`, not
merely the forward declarations in `AnmManager.hpp`. The corrected header
explicitly includes this dependency. The original nominal class name is
not proven by naming this layout.

The Japanese original TH09 v1.50a SHA-256 is
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Four fixed-profile, directly affected current-source compilation carriers
have been rebuilt and checked **in full**:

| Source | Original exact owners | Complete physical bytes | Bound fields |
| --- | ---: | ---: | ---: |
| PrimitiveDraw | 12 | 1,763 | 90 |
| Draw3D | 1 | 811 | 26 |
| Background | 5 | 96 | 2 |
| TitleDrawCallback | 1 | 329 | 20 |
| **Total** | **19** | **2,999** | **138** |

Every complete physical byte, original relocation field and already exact
sibling remains identical. No canonical target, compiler profile, match
unit, external symbol or original function count changed. The local trials
are retained in ignored `.analysis/gpt-web-anm-common-cache-20261107/`.

Reproduce only the affected carriers with
`scripts/build-match-unit.py --unit` for `anm-manager-draw-vertices`,
`anm-manager-draw-3d`, `anm-manager-get-anm`, and `title-draw-callback`, then
`scripts/compare-coff-function.py --unit` for the 19 functions sharing those
four source files. No all-game cold replay, product link, runtime or
Factory acceptance is inferred. All 19 were already exact; this is a
concrete source-ownership improvement rather than new exactness credit.

## Follow-up: render-core native cache and VM alpha

The same original 0x1288C-byte cache prefix now includes the established
+0x18 `flushesThisFrame` word and is inherited by `AnmManagerRenderView`.
Seven Render callbacks now inspect the real `AnmVm::color1Script.a` rather
than a duplicate alpha view. All 16 Render core owners were independently
verified, extending the focused cohort to 35 complete functions/8,813
physical bytes/533 fields. See
[Render core original-bound proof](TH09_ANM_RENDER_CORE_NATIVE_STATE_20261107.md).

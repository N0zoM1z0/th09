# TH09 ANM Render core: native cache and VM alpha (2026-11-07)

The exact `src/AnmManagerRender.cpp` owner group includes two **1,067-byte**
DrawInner methods, Draw2D, camera projection, render-state setters and
small state-transition leaves. Its C++ reconstruction previously maintained
another `AnmManagerRenderView` consisting mostly of raw padding to the
cache at +0x128E4 and to the vertex-buffer cursors at +0x2B28E8/+0x2B28EC.
It also had a separate `AnmVmDrawAlphaView` that copied the native VM's
+0x1F3 color-alpha field.

The current source joins this render owner to the same
`AnmManagerRenderCacheView` used by PrimitiveDraw, Draw3D, Background and
Title. The shared prefix now names the actual +0x18 `flushesThisFrame`
field. `AnmManagerRenderView` inherits the prefix and retains only its
real trailing sprite, vertex-buffer and draw-count fields, preserving
+0x128E4 and the two +0x2B28E8/+0x2B28EC pointers. The seven alpha guards
read `vm->color1Script.a` on the *real* AnmVm; the duplicate alpha view and
corresponding pointer casts are removed. No second manager or VM allocation
is constructed or implied by either C++ view.

Three natural C++ source shapes were **compiled with the pinned VC7.1
profile**: shared manager prefix alone; native VM-alpha alone; and their
combined form. Each reproduced **all 16 full Render-source owners**:
5,814 physical original bytes with 395 independently bound COFF fields.
The accepted combined source also preserves all 19 previously exact owner
functions from PrimitiveDraw, Draw3D, Background and Title. The complete
focused validation covers **35 exact source units / 8,813 physical bytes /
533 fields**. Two 1,067-byte DrawInner methods, the 658-byte Draw2D
rotated/axis-aligned path and 622-byte camera quad projection are among
the fully verified larger functions; the 25-byte MixRenderColor and 49-byte
rotation helper are among the verified leaves.

Only the affected five source-profile carriers were rebuilt. No all-project
cold replay, original executable mutation, compiler profile search, manual
register forcing, or new inline/assembly code was introduced. These
functions had already been exact; the authored **1,021/1,034** exact count
therefore does not increase. The original nominal C++ names, whole product
link and runtime/Factory acceptance remain open.

The original Japanese 1.50a target SHA-256 remains
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The local private source variants, compiled COFF objects and comparison logs
are in ignored `.analysis/gpt-web-anm-render-owner-20261107/`.

Reproduce only the affected owners by building these anchors with
`scripts/build-match-unit.py --unit`: `anm-manager-flush-vertex-buffer`,
`anm-manager-draw-vertices`, `anm-manager-draw-3d`,
`anm-manager-get-anm`, `title-draw-callback`; compare the 35 functions
mapped to their five source files with
`scripts/compare-coff-function.py --unit UNIT`.

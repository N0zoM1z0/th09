# ANM Render inner: true animation VM colors, flags and sprite (2026-11-07)

The real Japanese TH09 v1.50a `AnmManager::DrawInner` and
`DrawInnerFlippedX` both occupy **1,067 original physical bytes**. The
maintained code formerly reinterpreted each actual `AnmVm *` as a separate
`AnmVmDrawInnerLayout`, a padding-heavy type exposing just two 32-bit color
words at +0x1F0/+0x1F4, flags at +0x1F8 and sprite pointer at +0x224.

The reconstruction now accesses the one native VM's `color1`, `color2`,
`flagsWord`, and `loadedSprite` directly. There is no secondary VM allocation,
no reinterpretation to an unrelated VM layout, and no helper call. The
existing temporary `AnmRenderColor` preserves its component overlay
representation for the original render-color mix; the on-VM color reads
remain native C++ 32-bit words. Offset assertions pin all four native
fields. Existing DrawInner normal and flipped passes, SetRenderStateForVm3D,
texture UV setup and other draw calls retain their original chronology.

Two actual source models were compiled using the pinned VC7.1 profile:
regular native `color1/color2` words and native union-channel `.d3dColor`
reads. Both produce **all 16 current-source original-exact Render functions**,
5,814 complete physical bytes and 395 original-bound COFF fields. The
accepted natural direct-word version also preserves both 1,067-byte inner
renderers and the 25-byte MixRenderColor helper. Every defined original
field identity is checked by the canonical function comparator, not just a
single local block.

This checkpoint is about removing duplicated source layout and making the
real renderer's object ownership explicit. The 16 owners were already exact,
so it **does not create new exactness credit**. It does not imply a Windows
product build, full-game cold replay, original nominal C++ name, Factory
acceptance, or remote publication. The original SHA-256 remains
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.

Local private compilation controls are retained in
`.analysis/gpt-web-anm-render-inner-20261107/`. Reproduce only the affected
source unit using `scripts/build-match-unit.py --unit
anm-manager-flush-vertex-buffer` and canonical comparisons of the 16 units
mapped to `src/AnmManagerRender.cpp` in `config/match-units.toml`.

# TH09 shared Player runtime: native block-state gate (2026-11-06)

The original v1.50a shared Player runtime allocation has a signed dword at
`+0xE94C`, the update-block word at `+0x1095C`, and the draw counter at
`+0x10960`. In the prior maintained source, the exact 24-byte
`PlayerSharedRuntimeView::IsBlocked` reconstructed `+0xE94C` through
`reinterpret_cast<unsigned char *>(this) + 0xE94C`; large PlayerRuntime and
ReplayPlaybackFrameAdvance owners separately declared shortened copies of
that same global class.

The new `PlayerSharedRuntimeView.hpp` is the one **global** native class
projection shared by these three original-bound translation units. Its
`blockStateE94C` is an ordinary signed `int`; `IsBlocked` returns true for
`value >= 0 || value == -2`, exactly as the target-bound original and
previous source do. Its originally opaque fields remain opaque, and
compile-time `offsetof`/`sizeof` checks verify the exact +0xE94C/+0x1095C/
+0x10960 words and final `0x10964`-byte extent. The C++ member call of
`IsBlocked`, the big Player callback's update gate, and the Replay frame
control all still use the same observed physical receiver.

## Important rejected source/nominal-type assumption

The exact `PlayerGameplayState.cpp` selector also declares a
`PlayerSharedRuntimeView`, but **inside an anonymous namespace**. A coherent
source experiment replaced that local class with the new global type and
compiled both real selector owners. All 3,238 selector bytes still linked to
the original PE, but two independently reviewed C++ symbol identities
changed: the anonymous-scope `g_PlayerSharedRuntime` type and the anonymous
`IsBlocked` method became different global mangled names. This is **not**
acceptable as a canonical exact-source replacement merely because both
resolve to the same numeric destination. The selector's original class
and declaration were restored; its 3,238-byte selector and 412-byte
physical resolver remain canonical exact. Do not force another such name
merge without target-side original C++ type/ABI evidence.

## Focused original-bound compiler evidence

The pinned VC7.1 compilation and strict full physical byte comparisons of
all **23 affected exact owners** passed: 21 genuine `PlayerRuntime.cpp`
functions over its five existing compiler profiles, plus the exact
24-byte `IsBlocked` leaf and the 59-byte Replay control callback. Their
sum is **5,474 original physical bytes / 291 bound relocation fields**.
That group includes the 522-byte Player OnUpdate and 852-byte physical
UpdateBeforeState. The shared header's new type declaration renumbered
11 compiler-local `$L` labels in the latter's target-bound table; actual
COFF section/value coordinates reproduce all the **unchanged original VAs**,
then the manifest updated only their private symbol spellings. The final
canonical strict comparisons still pass.

The underlying Japanese TH09 v1.50a target SHA-256 is
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Suggested bounded reproduction:

```bash
python3 -B scripts/build-match-unit.py --unit player-shared-is-blocked
python3 -B scripts/compare-coff-function.py --unit player-shared-is-blocked
python3 -B scripts/build-match-unit.py --unit player-on-update
python3 -B scripts/compare-coff-function.py --unit player-on-update
python3 -B scripts/build-match-unit.py --unit replay-control-frame-advance
python3 -B scripts/compare-coff-function.py --unit replay-control-frame-advance
```

All 23 currently affected source owners were tested at their full extents,
without a full-game cold replay. The meaningful class-identity counterexample
and source snapshots are retained in ignored local
`.analysis/gpt-web-shared-runtime-20261106/` and the private PlayerRuntime
original-bound label audit. No new exact-count credit, native product/runtime
verification, Factory acceptance or remote publication is claimed.

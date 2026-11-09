# Native Front transition gate used by Effect callbacks (2026-11-09)

Original TH09 v1.50a has a seven-byte `thiscall` at `0x0040D4F0`
whose complete body returns the dword at receiver `+0x11EA8`.
This is the constructed `FrontFactoryConstructView::transitionState11EA8`,
not a separately allocated `PlayerSharedTransitionView`. Its source now
belongs to the actual Front factory type, preserving all seven original
physical bytes with zero relocations.

Two independent Effect callbacks have stopped calling that leaf through
an opaque shared-runtime cast. Both now read the actual
`g_GameManager.frontFactoryA8` pointer (retail global root `0x4A7D90`,
member `+0xA8`), and call the genuine C++ Front method. The original PE
absolute operand at Effect trail +0xBB and Effect random-axes +0x137
was independently checked against `0x4A7E38`. The REL32 calls at
+0xC0 and +0x13C still resolve to `0x40D4F0`.

| Whole original function | Size | Bound references | Result |
| --- | ---: | ---: | --- |
| Native Front gate getter | 7 | 0 | exact |
| EffectUpdateCallbackType3Type4 (enemy trail) | 464 | 20 | exact |
| EffectUpdateCallbackType1Type2 (random axes) | 775 | 33 | exact |
| Same source Player reward-advance leaf | 12 | 1 | exact |

The natural C++ source profiles were freshly compiled and all four complete
function bodies matched the original PE and relocation identities. The
obsolete `PlayerSharedTransitionView.hpp` was deleted after source search
proved it had no remaining users. Eighteen Effect-specific and four shared
runtime tests pass. The separately typed `PlayerOwnerState::ApplyReward`
(big owner), `PlayerDeath::EnterDeathState`, and other shared-front callers
are **not** silently reclassified or assumed to be migrated here; they
remain independently handled workstreams.

No new canonical authored exact function was added. No full cold replay or
Windows product/runtime equivalence was claimed.

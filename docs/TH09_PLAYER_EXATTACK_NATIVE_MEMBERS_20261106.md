# Player registration, added-state and ExAttack VM members (2026-11-06)

These changes replace literal byte-offset pointer arithmetic with existing
native C++ members in **three independently compiled source units**. This is
actual game-source reconstruction, not a cosmetic diff: the original object
ownership is now described by the Player and AnmVm layouts already shared
with other exact owners. No original binary or compiler flags are modified.

* The 328-byte `PlayerRegisterChain` owner initializes the actual
  `PlayerLifecycleView::timer303C8` using the Timer assignment, not an
  unrelated `Player* + 0x303C8` pointer cast. The offset is statically
  checked against the native Player allocation.
* The 552-code-byte `PlayerLifecycleView::InitializeAddedState` owner invokes
  the existing genuine `PlayerAddedHeaderResetView::Reset` on
  `&this->header24` rather than calculating `Player* + 0x24`. The original
  helper symbol and its receiving storage remain unchanged; this is a typed
  subobject pointer, not a new method or a duplicate header allocation.
* The 318-byte `ExAttackDrawCallbackType19_21` takes the alpha byte directly
  from `record->vms1C->color1Script.a` rather than a raw `+0x1F3` byte
  address. The existing native ARGB view and VM prefix pin the alpha byte.

## Whole-owner, original-bound verification

The Japanese v1.50a executable SHA-256 is
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The pinned compiler/profile was used for every affected source owner, not a
new guessed profile. All **eight distinct function match units** remain exact:

| Canonical unit | Fully compared bytes | Result |
| --- | ---: | --- |
| `player-register-chain` | 328 | Exact |
| `player-release` | 86 | Exact |
| `player-release-owned-resources` | 75 | Exact |
| `player-initialize-added-state` | 572 (552 body plus switch data) | Exact |
| `player-added-header-reset` | 29 | Exact |
| `player-added-collision-reset` | 23 | Exact |
| `player-added-owner-reset` | 41 | Exact |
| `exattack-type19-21-draw` | 318 | Exact |

The exact Player initializer's VC7.1 private label spellings changed at six
fields, while all **25 actual original-bound fields** retained their kind,
coordinate and original destination. Every renamed label was independently
resolved from its actual COFF section/value and owner entry, **before**
refreshing the manifest. The linked full 572-byte owner was compared to the
verified PE; no mask or destination fitting was used. The remaining seven
units also compare completely with original call/literal identities.

Only the listed match units were rebuilt. The eight functions were previously
exact: there is **no new exact byte/function credit** or Factory acceptance.
The source improvements, old-pointer counterexamples and COFF label evidence
are retained in `.analysis/gpt-web-player-exattack-native-20261106/` and
`.analysis/gpt-web-player-added-native-labels.json` (ignored local work).

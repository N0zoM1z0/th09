# Front Reset belongs to the same original constructed object (2026-11-09)

The original Japanese TH09 v1.50a `CreateGameSubsystem` at `0x00418840`
constructs one 0x1293C-byte Front object and calls the 14-byte reset
function `0x00415D40`. A previous source-only `Front` type held nothing but
a byte array and separately named this method. The maintained source now
places the genuine `int FrontFactoryConstructView::Reset()` on the **same**
C++ allocation as its native constructor, release, calculation, draw, added
callback, message and result functionality. Reset still uses ordinary
`memset(this, 0, sizeof(*this))` and returns zero; no new effect was added.

The original factory's REL32 reference at complete-owner offset +0x61
was independently decoded from the immutable original PE and still calls
0x00415D40. The actual source C++ symbol is now
`?Reset@FrontFactoryConstructView@@QAEHXZ`; both its definition and the
factory caller were recompiled using their independently pinned VC7.1
profiles. The obsolete `FrontSupport.hpp` fake 0x1293C-byte allocation was
removed after eliminating its final two unused includes.

| Complete original owner | Bytes matched | Actual relocations | Result |
| --- | ---: | ---: | --- |
| `FrontFactoryConstructView::Reset` | 14/14 | 0/0 | exact |
| `CreateGameSubsystem` | 220/220 | 22/22 | exact |
| `FrontAddedCallback` | 706/706 | 37/37 | exact |
| `FrontCalcCallback` | 1491/1491 | 107/107 | exact |

All five separate original-bound Front constructor-source equivalents also
retain exact linked bytes and referents. The 7 FrontAdded, 31 FrontMessage
and 3 FrontResult focused regression tests pass. This C++ owner recovery
changes no canonical authored exact function count, compiler profiles,
original executable, or native ABI; no complete cold replay or runtime
semantic gate was invoked.

# Native AsciiManager global constructor and real member types (2026-11-09)

The original Japanese TH09 v1.50a CRT initializer at **0x0048D950** is
`mov ecx, 0x004CE458; jmp 0x00435DC0`. The address **0x004CE458** is already
independently bound to the real `g_AsciiManager` in exact gameplay/HUD
callers. This identifies the 227-byte constructor previously modeled only
as an anonymous VM composite as construction of the *real* global
`AsciiManager` allocation. A bounded original-PE check now rejects changes
to either the global address or its called constructor.

The existing `AsciiManager.hpp` already describes the same **0xE0AC**-byte
storage, with 256 strings starting at +0x2264, PauseMenu at +0x82A0,
AsciiMenuState5 at +0x97C8, menu-state storage at +0xA7A8, and 200
score-popups starting at +0xB240. Four original constructor roles now
have exact, natural C++ definitions on those **actual** source types:

| Real member C++ type | Original VA | Bytes | Relocations | Differences |
| --- | --- | ---: | ---: | ---: |
| `AsciiManagerString` | 0x4343D0 | 3 | 0 | 0 |
| `AsciiManagerPopup` | 0x435AF0 | 15 | 1 | 0 |
| `PauseMenu` | 0x435D40 | 39 | 3 | 0 |
| `AsciiMenuState5` | 0x435D70 | 39 | 3 | 0 |

The parent `NativeVmCompositeOwnerView` now constructs real
`AsciiManagerString[256]`, `PauseMenu`, `AsciiMenuState5` and
`AsciiManagerPopup[2][100]` members. Its **complete original 227/227
bytes and 20/20 field identities remain exact**, even after changing the
four actual C++ callee symbols. Compile-time offsets also bind its members
to the independently checked real `AsciiManager` layout. The first nine
individual VMs and the later grouped arrays remain as recovered.

One important boundary is still open: the original +0xA7A8 sub-constructor
constructs *three* animation VMs, then the parent separately constructs the
VM at +0xAF9C. The existing `AsciiMenuState4` gameplay view spans four VMs;
treating the original three-VM constructor as proof of a four-VM nominal
special member would be incorrect. The source uses a small construction
view for those fields without inventing an extra real object.

## Scoped original-bound verification

- Two freshly compiled pinned `/O2 /Ob0` carriers reproduce **10 source
  equivalents / 447 replayed physical bytes / 36 relocation fields**,
  including shared aliases. There are still only **351 distinct physical
  bytes / 29 fields** across the six original constructor addresses.
- All **28 directly affected canonical Ascii owners** from the actual
  compiler profiles match their complete original targets: **6,786
  physical bytes / 394 relocation fields**.
- VC7.1 renamed 25 compiler-private `$L` labels in three existing
  menu-update functions. Every same-section definition was independently
  checked against the original PE target; after rebinding the label
  spelling, the full **1172-, 1776- and 1236-byte** owners are strictly exact.
- The native Ascii header change preserves the complete nonexact big-boss
  frontiers: `Enemy::OnUpdate` 3900/97/13 and `EclManager::RunEcl`
  15564/598/30, with original call identities and dispatch tables intact.

Reproduce only these constructor carriers:

```bash
nice -n 12 bash scripts/compile-probe.sh src/AsciiManagerNativeConstructors.cpp \
  build/matching/AsciiManagerNativeConstructors.obj \
  /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob0 /Oy- /I src
nice -n 12 bash scripts/compile-probe.sh src/NativeVmGroupConstructors.cpp \
  build/matching/NativeVmGroupConstructors.obj \
  /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob0 /Oy- /I src
python3 -B scripts/inspect-native-vm-group-constructors.py \
  build/matching/NativeVmGroupConstructors.obj \
  --ascii-object build/matching/AsciiManagerNativeConstructors.obj
```

Complete constructors remain **origin-unclassified**: byte identity alone
cannot prove explicit versus implicit source authorship or the original
nominal constructor spelling. No authored exact count increases; Windows
product linking and runtime equivalence remain open phases.

# TH09 native ANM constructor device-state fields (2026-11-06)

The original 341-byte ANM manager constructor initializes several device-state
words using fixed addresses in its large `0x2B2934`-byte allocation. The
independently exact ANM renderer, background and vertex-buffer owners already
observe these same physical offsets. The conditional native lifecycle-layout
section of `AnmManager.hpp` now names the corresponding fields, replacing a
single opaque `unknown12308` span **without changing its size or the overall
native manager layout**.

The constructor now initializes the actual shared members:

| Physical manager offset | Maintained member | Initialized value |
| --- | --- | --- |
| `+0x1287C` | `currentTextureFactor1287C` | 1 |
| `+0x12880` | `currentTexture12880` | null |
| `+0x12884` | `currentBlendMode12884` | 0 |
| `+0x12886` | `currentVertexShader12886` | 0 |
| `+0x12887` | `disableZWrite12887` | 0 |
| `+0x12888` | `cameraMode12888` | `0xFF` |
| `+0x12890` | `quadVertexBuffer12890` | null |
| `+0x2B28F0` | preexisting `field2B28F0` | -1 |

The adjacent unknown byte `+0x12885` is explicitly retained as unknown.
The original `+0x08` manager value is also still unknown; no unsupported
semantic name or lifetime hypothesis has been assigned to it. Compile-time
member-offset checks cover all the newly named fields and the established
`+0x2B28F0` tail word. The code makes ordinary C++ assignments, not manual
codegen scheduling, barriers, assembly or target-byte patches.

Under the pinned VC7.1 compiler, the 341-byte constructor remains 341/341
original-exact. All 16 other exact units in `AnmManagerLoad.cpp` are separately
compared under their existing `/Ob1` profile and also remain exact, a total
of 17 complete original-bound units / **3,258 physical bytes**. The independent
20-byte `AnmManager::GetAnm` from its own profile also remains exact. Their
field identities and original target byte sequences are unchanged.

Reproduction is limited to the affected canonical profiles:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/build-match-unit.py --unit anm-manager-constructor
python3 -B scripts/compare-coff-function.py --unit anm-manager-constructor
python3 -B scripts/build-match-unit.py --unit anm-manager-read-anm-entries
python3 -B scripts/compare-coff-function.py --unit anm-manager-read-anm-entries
```

The last unit is a representative original-bound check of the rebuilt shared
loader carrier; the full 16-unit same-object census was independently run in
this source checkpoint. No additional authored byte-exact credit is claimed:
these were already matching owners. Whole-product native linking, original
nominal class identity, runtime semantics and portability remain open.

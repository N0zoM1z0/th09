# Target architecture

## Observed executable identity

The only target is the original Japanese TH09 1.50a executable pinned in
`config/target.toml`. It is a relocation-stripped PE32 GUI program for Intel
i386 with image base `0x00400000`, entry point `0x0047D45F`, five sections, and
image size `0x000E7000`.

| Section | RVA | Virtual size | Raw offset | Raw size |
| --- | ---: | ---: | ---: | ---: |
| `.text` | `0x00001000` | `576,218` | `0x00000400` | `576,512` |
| `.rdata` | `0x0008E000` | `69,668` | `0x0008D000` | `70,144` |
| `.data` | `0x000A0000` | `279,036` | `0x0009E200` | `32,256` |
| `.data1` | `0x000E5000` | `2,272` | `0x000A6000` | `2,560` |
| `.rsrc` | `0x000E6000` | `2,400` | `0x000A6A00` | `2,560` |

Imports observed in the PE include Direct3D 8, DirectInput 8, DirectSound,
WinMM, and the standard Win32 system DLLs. This establishes platform surface,
not source ownership or a complete dependency recipe.

## Toolchain observation

The PE linker version is 7.10. Its Rich header contains the same VC7.1
build-3077 product family found in the independently reconstructed TH095
target, with TH09-specific object counts. This supports Microsoft Visual C++
.NET 2003 build `13.10.3077` as the compiler provider.

Pinned VC7.1 D3DX8, CRT, import, DXErr8, and compiler-helper contributions have
been identified for reviewed target extents. The following whole-product facts
remain unknown until compiler and link Oracles reproduce them:

- per-translation-unit optimization and debug flags;
- calling-convention defaults and local overrides;
- translation-unit partition and source order;
- the complete static-library set and member selection;
- resource compilation inputs;
- linker switches and input order;
- ownership of compiler-generated, library, padding, and data extents.

## Reviewed candidate inventory

`config/functions.csv` began as a navigation inventory exported from the exact
attested IDA database. All 2,191 tracked candidates have now received a
boundary/origin review, including target-derived corrections and additions.
Auto-names and semantic types remain provisional. A deliberately unresolved
set remains where the target cannot distinguish explicit source from implicit
special-member generation or cannot assign a unique owner to a shared/folded
physical body. Its current count and frozen digest belong to
`docs/RE_HANDOFF.md` and the ledgers rather than this architecture summary.
Boundary review still does not imply source presence or exact code generation.

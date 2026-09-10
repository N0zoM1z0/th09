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

The following remain unknown until compiler and link Oracles reproduce them:

- per-translation-unit optimization and debug flags;
- calling-convention defaults and local overrides;
- translation-unit partition and source order;
- static libraries and member selection;
- resource compilation inputs;
- linker switches and input order;
- ownership of compiler-generated, library, padding, and data extents.

## Initial IDA inventory

`config/functions.csv` is a navigation inventory exported from the exact
attested IDA database. Every extent and auto-name is provisional. Shared tails,
switch tables, EH metadata, thunks, alignment, interleaved data, and remote
owned chunks can make an IDA function extent incomplete or over-broad. Review
those surfaces before classifying origin or accepting exact bytes.

# TH09 verified knowledge

This file stores durable TH09-scoped facts and important negative results.
Cross-game promotion is a later Factory review, never an automatic action from
this repository.

| ID | State | Fact | Evidence |
| --- | --- | --- | --- |
| TARGET-001 | observed / corroborated | The supported executable is original Japanese TH09 v1.50a, size 685,056, SHA-256 `10350095...fce6822`. | Direct hash/PE inspection plus pinned thcrap version database |
| PE-001 | observed | The target is relocation-stripped PE32 i386 at image base `0x00400000`, entry `0x0047D45F`, with five sections. | `scripts/verify-target.py`; `config/target.toml` |
| TOOLCHAIN-001 | observed / corroborated | The target belongs to the VC7.1 build-3077 family. | PE linker 7.10 and Rich-header product/build records; TH095 provider-family comparison |
| TOOLCHAIN-002 | unknown | Compiler flags, TU partition, libraries, resources, and link order are not yet established. | Explicit open fields in `config/build.toml` |
| ANALYSIS-001 | observed | The live IDA database matches target hashes, mapped size, image base, entry, and distributed target bytes. | `python3 scripts/check-ida-mcp.py` |
| INVENTORY-001 | inferred | IDA currently supplies a provisional navigation inventory; individual boundaries and origins remain unreviewed. | `config/functions.csv`; `config/function-origins.csv` |
| CRT-001 | observed / corroborated | `__amsg_exit` at `0x0047D416-0x0047D43A` is a 37-byte CRT/library helper. | Attested TH09 control flow and bytes; VC7.1 `libcmt.lib` `wincrt0.obj` structural comparison with relocation fields excluded |
| CRT-002 | observed / corroborated | `_fast_error_exit` starts at `0x0047D43B` and continues through `0x0047D45E`; the imported IDA candidate omitted the final `pop ecx; pop ecx; ret`. | Attested TH09 bytes and adjacent entry at `0x0047D45F`; VC7.1 `libcmt.lib` `wincrt0.obj` structural comparison with relocation fields excluded |
| CRT-003 | observed / corroborated | The PE entry point at `0x0047D45F-0x0047D633` is the 469-byte VC7.1 `libcmt.lib` `wincrt0.obj` `_WinMainCRTStartup` contribution. | No callers; exact 0x1D5 COFF extent; attested TH09 body and COFF body have the same normalized SHA-256 across 321 non-relocation bytes after masking 37 relocation fields |
| CRT-004 | observed / corroborated | `__finite` at `0x0047D634-0x0047D648` is a 21-byte CRT/library helper. | Verified TH09 target body matches VC7.1 `libcmt.lib` `ieeemisc.obj` byte-for-byte; COFF extent is `0x15` with no relocations |
| CRT-005 | observed / corroborated | `__fpclass` at `0x0047D649-0x0047D6E4` is a 156-byte CRT/library helper. | Verified TH09 target body and VC7.1 `libcmt.lib` `ieeemisc.obj` match across all 148 non-relocation bytes after masking two COFF relocation fields |
| BOUNDARY-001 | observed | `0x0047D6E5-0x0047D6E7` is a three-byte `CC CC CC` gap between `__fpclass` and `_longjmp`; physical ownership remains unassigned. | Verified TH09 target bytes plus attested function starts at `0x0047D649` and `0x0047D6E8` |
| CRT-006 | observed / corroborated | `_longjmp` at `0x0047D6E8-0x0047D760` is a 121-byte CRT/library helper; the imported IDA inventory incorrectly split its final `ret` at `0x0047D760` into `nullsub_10`. | Verified TH09 target and VC7.1 `libcmt.lib` `longjmp.obj` match across all 101 non-relocation bytes after masking five COFF relocation fields; COFF extent is `0x79` |
| CRT-007 | observed / corroborated | `__setjmp3` at `0x0047D764-0x0047D7DE` is a 123-byte CRT/library helper. | Verified TH09 target and VC7.1 `libcmt.lib` `setjmp3.obj` match across all 119 non-relocation bytes after masking the COFF relocation at `+0x28`; COFF extent is `0x7B` |
| BOUNDARY-002 | observed | `0x0047D761-0x0047D763` is a three-byte `CC CC CC` gap between the corrected `_longjmp` extent and `__setjmp3`; physical ownership remains unassigned. | Verified TH09 target bytes plus corrected archive-backed function extents |

Do not turn cross-game similarity, a decompiler type, or an attractive reused
name into a TH09 fact without target-local evidence.

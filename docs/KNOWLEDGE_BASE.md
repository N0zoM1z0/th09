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

Do not turn cross-game similarity, a decompiler type, or an attractive reused
name into a TH09 fact without target-local evidence.

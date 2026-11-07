# SaveReplay serialized header spans — 2026-11-07

Original target: Japanese TH09 v1.50a; SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint changes the actual 1,238-byte `ReplayManagerView::SaveReplay`
source. It does not assert original historical C++ identifier spellings or
whole-product runtime equivalence.

## Recovering the wire ranges instead of treating their sizes as unrelated constants

The game physically serializes a 0xC0-byte `ReplayHeaderView` ahead of the
`ReplayPayloadView`. Its native `obfuscationKey` byte is at +0x15 and its
`compressedSize` field at +0x18. Two original fixed-count loops therefore
end at the **same header boundary**:

| Original loop/write | Native calculation | Source-facing name |
| --- | --- | --- |
| 0xAB checksum bytes | `sizeof(ReplayHeaderView) - offsetof(ReplayHeaderView, obfuscationKey)` | `ChecksumHeaderBytes` |
| 0xA8 obfuscated header bytes | `sizeof(ReplayHeaderView) - offsetof(ReplayHeaderView, compressedSize)` | `ObfuscatedHeaderBytes` |
| 0xC0 header file-write bytes | `sizeof(ReplayHeaderView)` | native header size |
| 13 bytes beyond user info string | `sizeof(ReplayUserDataHeaderView) + 1` | `UserInfoWithNulBytes` |

The last item includes the terminating NUL after the 0x0C-byte user-info
record. SaveReplay now uses ordinary C++ enum constants derived from the
actual native structs, with a compile-time guard for offsets and sizes.
The compressed byte stream remains a *separate* count, not another part of
the fixed 0xC0-byte header. The original checksum seed and obfuscation-step
arithmetic are unchanged.

## Codegen exploration and verified unchanged frontier

Six isolated source-cold natural C++ models of these original ranges (direct
`offsetof`, in-function enum, and paired user-header/file-write models) all
retain the complete physical 1,238-byte SaveReplay code body, 53 original-
bound relocations, 23 ordered calls, 49 matching direct-control-flow blocks
and the **same 2 ordinary residual bytes** at `+0xB7`/`+0xBB`.
No new exact credit follows from this partial match.

A separate source-cold chronology study compiled 11 alternate natural C++
forms of the first stage-header copy, including pointer/reference qualifiers,
link lifetime, offset publication and copy scheduling. Moving the link
lifetime or the copy publication order altered up to 1,067 other bytes;
none improved both existing residual bytes. These are bounded negative
results, not proof that all natural source models have been exhausted.

After installing the enum and compile-time layout constraints in the live
source, the focused `--owner replaysave` Oracle freshly compiled the affected
Replay carriers. **All 12 exact siblings remain exact, 4,093 complete bytes
and 232 relocations**. The nonexact SaveReplay still has exactly the original
1,238-byte/53-field/2-byte residual and original graph/call inventory.
The original PE/COFF verifier (not this prose) establishes the bound source
and target extents and rejects changes to formerly matching bytes.

Ignored experiment evidence lives under `.analysis/gptweb-save-wire-spans-20261107/`
and `.analysis/gptweb-save-chronology-20261107/`; nothing in these private
packets or the original executable is added to Git. This commit changes
semantic wire ownership, not the accepted 1,021-function exact count.
Full cold replay, Windows product closure, runtime dual Oracle, Factory
acceptance, portability and remote publication are separate open gates.

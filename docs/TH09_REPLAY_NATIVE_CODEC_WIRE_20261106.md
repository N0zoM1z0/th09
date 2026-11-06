# TH09 Replay codec: native serialized header and payload (2026-11-06)

The original Japanese TH09 v1.50a replay decoder at `ReplayManagerView::LoadReplayData`
reads serialized bytes with two offset-based cursors and decodes the compressed
payload following the 0xC0-byte header. Earlier source represented the header
with raw `(u8 *)replayData + 0x15 / +0x18 / +0xC0` arithmetic despite already
sharing an independently original-bound `ReplayDataView` type across Replay and
Title code. The current C++ now uses the actual `ReplayDataView::obfuscationKey`,
`compressedSize`, and `randomPayloadByte` members, plus the native
`sizeof(ReplayHeaderView)` rather than restating the header size at each copy.

**Signed arithmetic matters.** The original `fileSize` loop bounds have signed
32-bit subtraction. Naively substituting `offsetof(...)` for literal offsets
changes the expression to unsigned size_t arithmetic; the pinned compiler
produced 589/591 and 578/591 matching bytes for those two independent models.
The maintained source explicitly converts `offsetof` back to original `i32`
before subtraction. The two signed versions and their combined variant compile
byte-exact against the original. Compile-time assertions pin the three wire
field offsets and header size:

| Wire owner | Header offset |
| --- | ---: |
| `obfuscationKey` | `0x15` |
| `compressedSize` | `0x18` |
| `randomPayloadByte` (payload start) | `0xC0` |
| `sizeof(ReplayHeaderView)` | `0xC0` |

This is **source/data-model progress**, not a new original C++ nominal name,
file-format redesign or a library ABI change. The executable is untouched.

## Focused exact-source and open boss checks

Only the two real ReplayInput compiler carriers were rebuilt: the canonical
`/EHsc` carrier containing ten exact units and the separate no-EH carrier
containing two exact units and the nonexact SaveReplay. All **12 exact
match units remain byte-exact** across their complete physical extents:
**4,093 physical bytes, 232 original-bound relocation fields**. Notably,
`LoadReplayData` is still **591/591 bytes with 14 fields** and
`BeginRecordingStage` is **1,323/1,323 with 82 fields**.

The full noncrediting `SaveReplay` audit also passes the no-regression guard:
**1,238 physical bytes, 53 fields and two unequal ordinary bytes** at
`+0xB7/+0xBB`. That two-byte boss is **not yet exact**. The immediate
natural C++ destination-pointer variants tried in this session (twelve
fixed-profile full function builds) also retained the same two residuals;
no unverified model was adopted. The attached local diagnostics are under
`.analysis/gpt-web-replay-save-destination-20261106/` and
`.analysis/gpt-web-replay-native-*-20261106/` (ignored evidence, not a
portable Factory receipt).

Suggested focused reproduction:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/build-match-unit.py --unit replay-manager-load-replay-data
python3 -B scripts/compare-coff-function.py --unit replay-manager-load-replay-data
python3 -B scripts/build-match-unit.py --unit replay-manager-begin-recording-stage
python3 -B scripts/compare-coff-function.py --unit replay-manager-begin-recording-stage
python3 -B scripts/inspect-replay-save.py build/matching/ReplayInputNoEh.obj --json --guard
```

The original Japanese target remains identified by
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Original complete inventory, Windows product build, runtime, and semantics
are still separate open phases. No new exactness credit or remote acceptance.

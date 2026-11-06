# TH09 ANM manager: typed native constructor device state (2026-11-06)

The original Japanese TH09 1.50a `AnmManager` allocates one 0x2B2934-byte
runtime object. The maintained constructor formerly wrote eight device state
words/bytes and the vertex-buffer tail via raw arithmetic on its `this`
pointer. The native render, background and vertex-buffer owners independently
read these same storage addresses. `AnmManager.hpp` now names the verified
constructor-facing fields within its existing lifecycle-only layout, and the
constructor initializes those actual members rather than writable offsets:

| Native member | Offset | Original source action preserved |
| --- | --- | --- |
| `currentTextureFactor1287C` | `0x1287C` | initial value `1` |
| `currentTexture12880` | `0x12880` | null texture pointer |
| `currentBlendMode12884` | `0x12884` | initial zero |
| `unknown12885` | `0x12885` | initial zero, role unproven |
| `currentVertexShader12886` | `0x12886` | initial zero |
| `disableZWrite12887` | `0x12887` | initial zero |
| `cameraMode12888` | `0x12888` | initial `0xFF` |
| `quadVertexBuffer12890` | `0x12890` | null vertex buffer pointer |
| `field2B28F0` | `0x2B28F0` | initial `-1`, meaning unproven |

A constructor-local compile-time `offsetof` assertion checks all nine offsets.
The earlier `+0x08` initialization remains raw because its original member
meaning is not yet independently established. No constructor logic, original
binary data or compiler profile is changed. This is typed-source recovery, not
a claim about retail C++ identifiers or an added exact-match function.

**Fixed-profile original-bound proof:** using the unchanged VC7.1 `/O2 /Ob1`
ANM loader profile and distinct `/O2 /Ob0` manager constructor profile,
all **17** exact original owners in `src/AnmManagerLoad.cpp` reproduce their
complete 3,258 bytes and 133 actual relocation fields. Specifically the
constructor is 341/341, `ReadAnmEntries` is 390/390, and the full loader
carrier's other 15 owners are also unchanged. The SHA-256-pinned original is
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Only the affected compiler profiles were rebuilt; no all-project cold replay
or runtime/Factory acceptance is claimed.

Recheck with `scripts/build-match-unit.py --unit anm-manager-constructor`
and `--unit anm-manager-read-anm-entries`, then
`scripts/compare-coff-function.py --unit NAME` for both and every other
same-object exact loader member from `config/match-units.toml`.

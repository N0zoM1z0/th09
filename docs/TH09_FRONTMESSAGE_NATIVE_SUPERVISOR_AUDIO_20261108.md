# TH09: native Supervisor audio owner across the complete FrontMessage boss

Target: verified Japanese TH09 v1.50a executable SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint recovers ordinary C++ object/call ownership; it does
not establish historic nominal class names, whole-game link closure,
new byte-exact source credit or runtime semantic acceptance.

## One real Supervisor across audio opcodes

The original **2,852-byte FrontMessage `Update` interpreter** has a
116-byte physical switch/data suffix, for **2,968 exact physical bytes**.
Its music opcodes previously declared a second, opaque
`FrontMessageSupervisorView` with only `StopAudio`, `PlayMusic` and
`FadeOutMusic` methods. Its `g_Supervisor` global actually denotes the
same **0x7B0-byte native `Supervisor`** used by startup, networking,
rendering, D3D viewport and the independently exact audio implementation.

The script now includes `Supervisor.hpp` and declares the genuine
`Supervisor g_Supervisor` instead of an isolated receiver. Its original
music-control branches, track selection, game-mode test, negative-track
StopAudio behavior and four-second fade are unchanged. No extra audio
controller, timer, resource lifetime, constructor or virtual-call seam
was introduced. The original four global-address referents remain at
**0x004B3100** and all four method call operands have their original
positions and original target VAs:

| Exact method | Original call offset | Original target |
| --- | --- | --- |
| `Supervisor::StopAudio()` | +0x810/+0x815 | 0x0042FD30 |
| `Supervisor::PlayMusic()` | +0x830/+0x835 | 0x00431930 |
| `Supervisor::PlayMusic()` | +0x84A/+0x84F | 0x00431930 |
| `Supervisor::FadeOutMusic()` | +0x89E/+0x8A3 | 0x0042FE20 |

## Complete original-bound source-cold verification

Two independent fixed-profile natural-C++ trial forms (direct native
member calls and an explicit native Supervisor reference) compiled to
the identical **2,968-byte source physical carrier** with **all 167
COFF field positions/kinds/addends unchanged**. The adopted source
was independently bound to the immutable original PE: **four actual
global `Supervisor` symbols, four real audio callee symbols**, and the
**30 compiler-private `$L`/`$run_scripts$` labels** all preserve their
original operand values and target addresses. Each private label was
checked against its real same-section COFF definition, owner-relative
location and original PE destination before refreshing its name only.

The strict canonical comparator confirms **2,852/2,852 source bytes,
2,968/2,968 physical code/table bytes, and 167 original fields**.
Scoped source-cold replay of the message and SupervisorAudio owners
passes **10 existing exact complete functions / 4,061 physical bytes /
249 relocation fields**. The independent 81-byte StopAudio, 254-byte
PlayMusic and 148-byte FadeOutMusic leaf owners remain original exact.
No authored-exact count was increased.

Private bounded original/COFF proofs and natural C++ experiments remain in
`.analysis/gpt-web-front-native-supervisor-20261108/`. No full-project
cold replay, game executable link, runtime acceptance, Factory receipt or
remote push is asserted. Global source ledger is unchanged:
**1,034 source-present / 1,021 byte-exact / 13 nonexact**.

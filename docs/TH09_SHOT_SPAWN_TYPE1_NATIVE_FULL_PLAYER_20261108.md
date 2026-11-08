# TH09 Type1 shot-spawn callback borrows actual full Player receiver

The complete original-exact 151-byte `PlayerShotSpawnCallbackType1` at
0x00441EF0 originally takes the **actual native Player object** in ECX,
not an independently allocated 0x3031C-byte prefix. Its maintained source
previously retained `PlayerShotSpawn1PlayerView` solely to reach the
shared Player+0x3031C shot timer/state.

The method now declares `PlayerLifecycleView *player`, using the original
native `state3031C.timer00` and `shot0C` members. The receiver has the
same original fastcall ABI, all original field offsets and the same
actual Player +0x3031C physical owner. The existing shot-specific view
and descriptor remain unchanged; their native identity is a separate
original-byte question, not assumed here.

A bounded pinned-VC7.1 trial compiled the full callback both with
direct native Player members and with a new `PlayerState3031CView &`
local. The direct form preserves **151/151 original bytes and both
relocation fields** exactly; the new local expands the candidate to
163 bytes and is therefore rejected. The adopted full Player receiver
also changes only the source C++ parameter type and symbol spelling,
not its original 0x00441EF0 entry, original timer callee, the exact
0x0041F350 InitializeShot call, or the SHT callback-table target.

Focused source-cold original PE/COFF replay validates **23 existing exact
functions / 5,554 physical bytes / 291 fields**, including the entire
PlayerRuntime constructor/update owner chain, the exact 12-byte
`PlayerState3031CView` constructor and the exact Type1 callback.
The separate nonexact 862/849-byte Player bullet-collision boss remains
open. Experiment and proof packet:
`.analysis/gpt-web-spawn1-native-player-20261108/`.

Target is Japanese TH09 v1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is native C++ receiver recovery, **not a new byte-exact function** or
Windows product/runtime acceptance. No full cold replay or remote push.

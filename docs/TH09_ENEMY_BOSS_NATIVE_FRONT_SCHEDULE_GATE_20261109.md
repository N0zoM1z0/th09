# Enemy boss schedule gate now uses the constructed Front member

The complete TH09 v1.50a 3,900-byte Enemy manager `OnUpdate` previously read
Front +0x1095C via a source-only `g_PlayerSharedRuntime` pointer at the
physical address 0x4A7E38. That global address is already independently
proven as `g_GameManager` (0x4A7D90) +0xA8, the actual pointer to the
0x1293C-byte `FrontFactoryConstructView`. It now reads
`g_GameManager.frontFactoryA8->transitionVmCount1095C` instead, matching
the native Front factory, Calc, and death-transition C++ member ownership.
The obsolete Enemy-only `extern PlayerSharedRuntimeView*` is retired.

The original Enemy owner's +0x70 DIR32 field is still exactly 0x4A7E38.
`inspect-enemy-manager-update.py` now requires **nine** reviewed absolute
GameManager references (including native +0xA8 at +0x34, +0x70 and
+0xE91), four unchanged playfield calls, actual initialized scalar
literals, a complete original RunEcl callee binding and unchanged
switch/CFG. Its mutation tests reject changed source field offsets,
symbols, kinds, root addends, and PE operand bytes.

The pinned VC7.1 object emits all **3,900 physical bytes, 97/97
original-bound fields and 69 ordered calls** with unchanged 170-block
control flow, but it retains **13 nonfield unlike bytes** at the existing
two original locations (draw-list register selection and special bullet
spawn/descriptor position copy). This change does not earn exact credit.

Two affected ECL exact siblings changed their compiler-private local `$L`
label spellings; each new same-section definition was checked against its
original absolute PE destination before manifest rebinding. All **77
complete EclManager exact siblings / 24,368 bytes / 1,096 relocations**
are still strict-original-exact. Sixteen Enemy inspector tests, 31 Front
message tests and two native shared-runtime tests pass. No full cold replay
or Windows game build/runtime claim was made.

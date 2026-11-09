# FrontCalc now uses the real GameManager and side runtime

The 1,491-byte TH09 v1.50a FrontCalc callback at `0x417630` previously
redeclared the first 0x138 bytes of `g_GameManager` in a separate
`FrontCalcGameManagerView`, with a separate 0x38-byte side and a narrow
side-runtime allocation. The source now uses the already constructed
`AsciiGameManagerView` / `AsciiGameManagerSideView` directly. Its existing
native `Background`, `CardAttack`, `PlayerLifecycleView` and `FrontSide`
member access remains unchanged.

The actual 0xA0-byte side runtime has a floating-point transition value
at member offset **+0x34**; the original FrontCalc reads and increments
that float when shifting transition metrics. `AsciiGameSideRuntimeView`
now exposes that native field, preserving the previous total size and all
other runtime offsets. The existing GameManager flags at +0x134 are read
through its real `flags` member, not an independently declared word.

The pinned VC7.1 compilation produces the complete original **1,491/1,491
bytes and 107/107 relocation fields**, with zero differences. All 28
changed GameManager-related source symbols were separately checked against
the original PE operand at the relevant offset: each absolute address is
`g_GameManager` at `0x4A7D90` plus its actual member addend, and each native
GameManager method call retains its original relative destination.

Related scoped original-bound checks remain exact: 3,238-byte Player selector,
3,094-byte FrontSide draw, 1,230-byte GameManager update, 923-byte CardAttack
start, 553-byte result bonus and the 2,968-byte message interpreter.
All eleven same-TU GameManager exact siblings match. The message
interpreter's 30 compiler-private labels were verified against original
same-section PE destinations before updating their spellings. The
still-nonexact Enemy OnUpdate (13 unlike bytes), GameplaySetupThread
(1,682/1,689 bytes), and RunEcl (30 unlike bytes) keep their prior local
frontiers. Scoped Front/GameManager/Player tests pass.

The change recovers native C++ object ownership in an already-exact owner;
it adds no new canonical exact function or broad cold replay.

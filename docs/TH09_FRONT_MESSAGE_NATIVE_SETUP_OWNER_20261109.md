# TH09 Front message Setup: real C++ owner and complete caller chain

The original Japanese TH09 v1.50a `Setup` entry at **0x00416160** is a
357-byte thiscall on the same embedded **0x1D70-byte** message runtime that
Front constructs, draws, updates, loads and releases. Previously its source
spelled a separate entire `FrontMessageSetupView`, and the 1,491-byte Front
calc and 133-byte `InitializeMessageRuntime` callers referred to two
**different source-only method symbols** at that one target address.

`FrontMessageRuntimeView::Setup(int messageIndex, int mode)` now owns the
actual method source in `FrontMessageSetup.cpp`. This removes the separate
0x1D70 shadow class and its private message-table projection. It uses the
real `FrontMessageTableView` entries (`+4 + index*8`), native text VMs at
+0x1534/+0x17D8, actual color and font flags, and the same target-prescribed
0x1D70 clear and time/mode reset. The shared `FrontMessageRuntimeView.hpp`
declares the real Setup method alongside the already matching Update, Draw,
Load and Release methods. The shared Front owner view embeds that genuine
runtime at +0xE944 and retains the same transition fields and size.

Both direct callers now invoke the real C++ method, without an invented
`FrontMessageSetupView::Setup` or `FrontCalcMessageRuntimeView::Setup`:

| Whole original owner | Bytes | Original bound fields | Result |
| --- | ---: | ---: | --- |
| FrontMessageRuntimeView::Setup, 0x416160 | 357/357 | 10 | exact |
| FrontMessageOwnerView::InitializeMessageRuntime, 0x4181E0 | 133/133 | 9 | exact |
| FrontCalcCallback, 0x417630 | 1,491/1,491 | 107 | exact |

Three actual caller REL32 fields now have the authentic C++ symbol
`?Setup@FrontMessageRuntimeView@@QAEXHH@Z`, at unchanged original target
**0x416160**, original-relative offsets +0x57 (InitializeMessageRuntime)
and +0x4BD/+0x4D6 (FrontCalcCallback). The source-linked canonical
function/matches/reccmp/implemented ledgers were updated consistently.
The new native method itself has the complete original 357 bytes and
all ten original relocation symbols, offsets, types and targets.

## Local regression evidence

The pinned VC7.1 original profiles independently recompile and prove
**11 exact Front owners / 9,713 physical bytes / 599 fields**, plus five
original-bound constructor-source-equivalent bodies. The Front Update
interpreter's 30 VC7.1-private `$L` label spellings changed after adding
Setup to the shared class, but their actual same-section symbol values,
original targets and whole 2,968-byte code/jump-table extent remain equal.
The affected GameManagerSetup source was separately rebuilt in **both** its
normal `/Os` and no-`/Os` profiles: all **11 canonical GameManager owners**
remain exact. The still-nonexact 1,689-byte GameplaySetupThread retains
1,682 candidate bytes, 172 fields and 694 overlap differences, not exact
credit. `test_front_message_*.py` passes **31 tests**.

Only changed/affected translation units, exact owner extents and immediate
constructor/callee proofs were rebuilt. No full cold replay, original-game
binary modification, Windows product-link closure, runtime-semantic proof or
new exact authored-function credit is claimed.

# TH09 Front teardown now uses the constructed native allocation

Original Japanese TH09 v1.50a `ReleaseCore` at `0x418270` takes the same
full **0x1293C-byte** Front object constructed by `CreateGameSubsystem`.
The prior `FrontReleaseView` declared an independent truncated allocation
and a shadow message runtime. The native source now places
`FrontFactoryConstructView::ReleaseCore()` directly on the real owner.
`ReleaseGameSubsystem` calls that member and cuts the real draw/calc chain
heads at +0x04/+0x00 before calling `operator delete` on the allocation.
The actual `FrontMessageRuntimeView` subobject at +0xE944 retains its real
`ReleaseForSideCount` member call.

The exact six-byte `ShouldReleaseStageResources()` policy stays in the
**same translation unit** as ReleaseCore: an earlier split-TU probe produced
38 bytes instead of the original 34-byte method and must not be substituted.
The original thiscall REL32 operand in `ReleaseGameSubsystem` at relative
+0x08 was independently checked against the original PE destination
**0x418270** before updating its source-linked COFF symbol spelling.

| Complete original owner | Bytes | Relocations | Result |
| --- | ---: | ---: | --- |
| Native Front `ReleaseCore` | 34/34 | 3/3 | exact |
| `ReleaseGameSubsystem` | 50/50 | 6/6 | exact |
| `ShouldReleaseStageResources` | 6/6 | 1/1 | exact |
| `CreateGameSubsystem` | 220/220 | 22/22 | exact |
| Front calc callback | 1491/1491 | 107/107 | exact |
| Front draw callback | 1474/1474 | 127/127 | exact |
| Front message runtime draw | 995/995 | 73/73 | exact |

This preserves the literal original method, callback, operator-delete and
chain call boundaries. The original-PE/COFF checks use the same pinned VC7.1
`/O2 /Ob1` profiles as the authored exact ledger. Five scoped
FrontMessage-release tests pass. No new function exact credit is claimed;
these owners were already counted exact. No full-project cold replay or
Windows product/runtime closure has been performed.

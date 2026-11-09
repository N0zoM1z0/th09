# TH09 Front rendering shares the real Supervisor D3D8 device

The original Japanese TH09 v1.50a `FrontMessageRuntimeView::Draw` (0x417130)
and `FrontDrawLineStrip` (0x422330) both access the same D3D8 device
pointer at **g_Supervisor + 0x08**. Their previous C++ source declared
separate truncated `FrontMessageDrawSupervisorView` and
`FrontLineSupervisorView` classes for that 12-byte prefix, despite the
complete constructed `Supervisor` already having the genuine
`IDirect3DDevice8 *d3dDevice08` member at the proven offset.

Both renderers now use ordinary native C++ `g_Supervisor.d3dDevice08`
member calls; no separate global instance, cast-based receiver or
fake object storage remains. Their state changes, draw calls, clear
sequence, timer-controlled rectangle and sprite rendering order are
unchanged. The LineStrip argument and its `D3DPT_LINESTRIP` primitive
still use the same original register/stack ABI.

| Original whole-owner body | Bytes | References | Result |
| --- | ---: | ---: | --- |
| Front message runtime Draw at 0x417130 | 995/995 | 73/73 | exact |
| FrontDrawLineStrip at 0x422330 | 318/318 | 26/26 | exact |
| FrontDrawCallback at 0x417C10 | 1474/1474 | 127/127 | exact |

Every one of the **12 LineStrip device DIR32 original PE operands**
was independently verified as **0x004B3108** before replacing its
source-linked relocation symbol. Each remains a `g_Supervisor` reference
whose root is 0x004B3100 and whose actual COFF addend is +0x08.
The pinned VC7.1 `/O2 /Ob1` original profiles generated identical
complete code and relocation records for both changed functions.

The local regression includes 42 existing Front tests and new
source/manifest tests that check the common Supervisor member identity,
all 12 LineStrip device references and both original function bounds.
These owners were already canonical exact: no new authored exact credit
is claimed. No project-wide cold replay, native Windows link or runtime
semantic proof was performed.

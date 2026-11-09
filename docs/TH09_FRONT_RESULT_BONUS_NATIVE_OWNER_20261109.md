# Front result bonus uses the genuine native Front object (2026-11-09)

The complete original TH09 v1.50a `InitializeResultBonus` at **0x415E50**
(553 bytes, 27 source/PE relocation fields) is now a member of the
constructor-proven `FrontFactoryConstructView`. The previous full-object
`FrontResultBonusView` shadow was deleted; five result animation VM
projections now address genuine members `resultVms10968[3..7]`, starting
at the unchanged original +0x11154. The native seven score fields at
+0x11E88, transition side/phase fields, and real player scores are shared
with the exact 1,474-byte Front draw and 1,491-byte Front calc callbacks.

The original script interpreter opcode 9 at 0x416590 reads the Front pointer
from physical **GameManager +0xA8**. Its maintained source now loads
`g_GameManager.frontFactoryA8` instead of asserting a separate global
`g_FrontMessageOwner`, then calls the genuine
`FrontFactoryConstructView::InitializeResultBonus()`. The original absolute
pointer address **0x4A7E38** and call target **0x415E50** are independently
bound to the unchanged PE operands at interpreter-relative +0x88F and
+0x894. The related `AsciiGameManagerView` declaration exposes a genuine
Front pointer over the existing +0xA8 word without changing the native
0x390-byte GameManager layout.

The pinned VC7.1 original profiles and SHA-256-verified TH09 target prove
whole-function exact equality, not excerpt matching:

| Actual function | Original bytes | Original fields | Result |
| --- | ---: | ---: | --- |
| FrontFactoryConstructView::InitializeResultBonus | 553/553 | 27 | exact |
| FrontMessageRuntimeView::Update (including switch table) | 2968/2968 | 167 | exact |
| FrontCalcCallback | 1491/1491 | 107 | exact |
| FrontAddedCallback | 706/706 | 37 | exact |
| GameManagerSetupLayout::OnUpdate | 1230/1230 | 87 | exact |
| Supervisor::OnUpdate (including private tables) | 996/996 | 92 | exact |

The 39-byte message decoder also remains exact, for **seven directly
recompiled functions / 7,983 physical bytes / 517 fields**. The
canonical `matches`, `reccmp`, `implemented`, function-inventory and
source symbol references consistently name the real member. Independently
checked raw same-section label definitions and original link targets
protect compiler-private label rebinding. The focused Front result tests
pass (3), Front message tests pass (31), and the nonexact ChargeAttack,
RunEcl, Enemy update and GameplaySetupThread original-bound fronts remain
unchanged. The type consolidation changes no authored exact count and is
not evidence of Windows product/runtime closure or unique original C++
identifier spelling. No full cold replay ran.

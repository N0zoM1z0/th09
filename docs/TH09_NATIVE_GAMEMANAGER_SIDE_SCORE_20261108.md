# TH09 native GameManager side score across Enemy, Player and FrontResult bosses

Target: verified Japanese TH09 v1.50a PE SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is ordinary C++ type, receiver and physical storage recovery—not
uniquely known original naming, product/runtime acceptance, or newly
credited complete byte-exact functions.

## A genuine GameManager side, not another 0x1C-byte prefix

The **33-byte AddScore leaf at 0x0040F860** was maintained on a second
`GameManagerSideStateView` carrying opaque bytes 0..0x1C, its own state
pointer at +0x1C, and a score storage prefix at runtime +0x08. But real
`AsciiGameManagerSideView` is an already-proved 0x38-byte native side
record. Its `AsciiGameSideRuntimeView *state` at +0x1C points to the
already-reviewed 0xA0-byte score/phase buffer, where the exact **+0x08**
word is both gameplay setup's unsigned `value08` and the native signed
`score08` accumulated by **`score / 10`**. These are the *same four bytes*,
not two allocations or independently updated counters.

The real `AsciiGameManagerSideView` now declares and defines `AddScore`.
The obsolete `GameManagerSideScoreView.hpp` (a separate storage class)
is deleted; the native side runtime has a signed-score alias and explicit
original-offset assertions. PlayerOwner's exact 1,519-byte reward function,
FrontResult's exact 553-byte bonus renderer, and both large nonexact
`EnemyManagerView::OnUpdate`/`PlayerLifecycleView::CheckBulletCollision`
call the genuine GameManager side method at the unchanged target VA.
The complete Enemy OnUpdate uses its native `sideState320`, and Player
CheckBulletCollision uses its real `sideState` rather than casting to the
old prefix. The four game owners share the same exact callee; no changed
score arithmetic, new resource or synthetic return register is introduced.

The original reward state machine's previously duplicate
`PlayerRewardTimerCurrentView` has also been removed. Its +0x5D6 call
at **0x00435F00** now naturally uses the independently exact
`ZunTimer::operator int()` on the actual `ZunTimer timer20` member.
Three fixed-profile natural models (direct cast, explicit conversion
method, and real timer reference) preserved the entire original machine
code/relocation layout without a shadow timer receiver.

## Whole original PE/COFF and large-boss evidence

A copy-on-write pinned VC7.1 trial compiled the **complete 33-byte
AddScore leaf (zero relocations)** and **1,540-byte physical ApplyReward**
including its six local jump/table referents; the full
**553-byte FrontResult bonus** separately remains byte-exact. An
independent immutable-original PE audit proves all field site/operand
pairs, target VAs and the authentic `+0x08/+0x1C` object offsets. Six
compiler-local `$L` name changes in ApplyReward were checked against
same-section COFF definitions, owner-relative locations and original
PE absolute destinations before refreshing *only the label spellings*
in the strict match manifest. The canonical exact code comparator still
passes every complete original owner.

An expanded source-cold original replay of EclManager (main and Type11),
GameManagerSetup, GameManagerSideScore, PlayerOwnerStateReward,
FrontResultBonus and PlayerCollisionHelpers passes **102 existing exact
functions / 36,193 physical bytes / 1,782 relocation fields**.
The open **3,900-byte Enemy OnUpdate** retains 97 fields, 69 calls and
13 unlike ordinary bytes. The open **862-byte Player CheckBulletCollision**
retains an 849-byte candidate, 26 original fields, 12 ordered calls,
799 unlike overlapping positions and 13 missing original bytes.
Neither large boss has been falsely promoted to byte-exact.

The targeted original PE/COFF verification, trial experiments, compiler
profiles and nonexact-boss evidence are stored locally under
`.analysis/gpt-web-native-gamemanager-side-score-20261108/` and
`.analysis/gpt-web-player-reward-native-timer-20261108/`.
No all-project cold replay, Windows product link, runtime gameplay
acceptance, Factory exactness receipt or remote push is asserted.
The global ledger stays **1,034 source-present / 1,021 exact / 13 nonexact**.

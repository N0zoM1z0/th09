# Front native signed block gate across Player, Enemy and Replay (2026-11-09)

The original Japanese TH09 v1.50a function at `0x404980` is a **24-byte**
thiscall that reads the signed dword at receiver **+0xE94C**, returning 1
when the value is nonnegative or equals -2, otherwise 0. The read is the
real constructed Front message runtime index at **Front +0xE944 +0x08**,
not an independently allocated `PlayerSharedRuntimeView`. The original
source-visible method has therefore moved to
`FrontFactoryConstructView::IsBlocked()`, implemented in the existing
`PlayerSharedRuntimeLeaves.cpp` source carrier. A local VC7.1-compatible
compile-time typedef asserts the shared native member offset, without
altering common header construction or introducing another helper.

The genuinely constructed Front pointer is at **`g_GameManager` +0xA8**:
`0x4A7D90 + 0xA8 == 0x4A7E38`. The actual original PE absolute and REL32
operands were inspected before changing any source-bound reference.
The following complete owners now call the real Front method:

- `PlayerUpdateSelectorState`, the **3,238-byte** Player pattern/AI boss:
  the mode count and signed gate both operate on the actual Front owner
- `PlayerLifecycleView::OnUpdate`, **522 bytes**, with both gate calls
- `GameManagerSetupLayout::OnUpdate`, **1,230 bytes**, directly using its
  existing typed `gameSubsystem` pointer
- `ReplayManagerView::ControlPlaybackFrameAdvance`, **59 bytes**, no
  duplicate Player runtime global
- `EnemyScheduleRuntimeView::Run`, **974 logical / 1,048 physical bytes**,
  using the real Front pointer and native getter

The old `PlayerSharedRuntimeView` remains only a **partial compatibility
projection** for other original Player fields at +0x1095C/+0x10960; its
unimplemented `IsBlocked()` declaration was removed. The redundant
`EnemyTimelineSharedRuntimeView` and its anonymous global pointer were also
removed from the Enemy schedule source. None of the five original call
addresses, caller ABIs, condition meanings or original memory operands
changed.

## Strict scoped original-PE/VC7.1 replay

- **38 exact functions / 13,183 physical bytes / 730 relocation fields**
  across the affected PlayerRuntime, PlayerGameplayState, GameManager,
  Replay, signed leaf and EnemyTimeline carriers.
- **15 exact functions / 11,357 physical bytes / 688 fields** across the
  direct FrontFactory, Effect, PlayerDeath, PlayerReward and message-runtime
  consumers. Five exact-construction-source-equivalent Front constructors
  retain their independent verified original-bound status.
- Together: **53 distinct complete original-exact functions,
  24,540 bytes and 1,418 reviewed field identities**. The 24-byte signed
  leaf is separately exact. All affected compiler profiles were rebuilt
  from this worktree; **none** is a partial-byte credit.
- Private VC7.1 `$L` label spellings changed in 84 positions: 29 across
  Player before-state/resolver, 19 in Enemy timeline, and 36 in
  PlayerReward/Front message Update. Every new label's actual same-section
  COFF definition was checked against its previous immutable PE target
  address before only its spelling was refreshed in the match manifest.
- **93 targeted unit tests pass**; the stale test expectations that required
  fictitious receiver classes were replaced by checks of the real native
  pointer, original call offsets and independent complete-owner identities.

`EnemyScheduleRuntimeView::Run` was **already** an original-byte-exact
canonical function before these edits; the current 1,048/1,048 proof is a
preservation/ownership improvement, **not** a newly fixed nonexact boss.
The remaining canonical ledger is still **1,024 exact / 13 nonexact**.
No full-project cold replay, original game executable patch, native Windows
product closure or gameplay-semantic acceptance is claimed.

Source-bound proof details and negative/label evidence are under the ignored
`.analysis/gpt-web-native-is-blocked-20261109/` directory.

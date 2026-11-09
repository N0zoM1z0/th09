# Death transition uses the constructed Front object

The original TH09 v1.50a `SelectOpponentSide` at `0x415D70` is a method of
the same 0x1293C-byte Front allocation used by gameplay and the message
system. Its former `PlayerDeathSharedView` recreated a shorter copy of the
same member layout. The method now belongs to `FrontFactoryConstructView`
and operates on `resultVms10968[0..2]`, transition state at +0x11EA8,
transition frame/side fields at +0x1095C..+0x10964, and the native ANM
resource at +0x08.

The exact `PlayerLifecycleView::EnterDeathState` caller now obtains the
same Front allocation through `g_GameManager.frontFactoryA8` rather than
a separate `g_PlayerDeathShared` pointer when it invokes the transition
method or publishes side panel timers and colors at +0xE93C/+0xE934.
The original physical pointer value is unchanged: **0x4A7D90 + 0xA8 =
0x4A7E38**. All three pointer operands were checked directly against the
original PE before the relocation source identity was corrected.

Complete pinned VC7.1 source replays remain original-exact:

- `FrontFactoryConstructView::SelectOpponentSide`: **222/222 bytes** and
  all nine relocated fields, target `0x415D70`.
- `PlayerLifecycleView::EnterDeathState`: **753/753 bytes**, all 38 fields,
  including native Front references at +0x1B6, +0x25F and +0x273 and the
  unchanged transition call destination at +0x1BC.
- The directly affected 2,968-byte Front message interpreter, Front
  factory, result-bonus, draw/calc, GameManager and Supervisor functions
  remain complete-original-exact. The interpreter's 30 compiler-private
  local label names were independently rechecked against true same-section
  COFF definitions and immutable original PE jump-table destinations.

The separately implemented seven-byte transition-block getter at
`0x40D4F0` still has its old source-facing method interface: migrating
all its other independent Player, Reward and Effect callers belongs to
another coherent scope. It was not silently reclassified or duplicated.
Four player-death tests, 31 Front-message tests and three Front-result
tests pass. No full cold replay or new authored exact credit is claimed.

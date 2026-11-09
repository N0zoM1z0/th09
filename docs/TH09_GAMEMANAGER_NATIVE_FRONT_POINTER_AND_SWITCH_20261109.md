# Gameplay setup has a typed native Front owner and a localized switch drift

The original TH09 v1.50a `CreateGameSubsystem` registers a 0x1293C-byte
Front allocation, and the GameManager publishes that pointer at +0xA8.
`GameManagerSetupLayout` now models this actual native storage as
`FrontFactoryConstructView *gameSubsystem`, alongside its independently
verified `PlayerSharedRuntimeView` receiver projection in the same slot.
The worker assigns the typed factory result without changing the call, and
`CleanupGameplayState` directly calls the genuine Front member rather than
casting a `void*` on every use.

The still-nonexact `GameplaySetupThread` at 0x41AF2D is **1,689 original
bytes / 1,682 candidate bytes / 172 verified field identities / 35 calls**;
its previous 694 overlapping unlike bytes and seven missing bytes remain
unchanged. The complete target and candidate decode to 413 instructions,
78 direct blocks each, and **412/413 normalized instructions match**.
Independent original-PE and pinned VC7.1 COFF disassembly narrows the
physical discrepancy to the compiler's placement of one ten-byte store:

- Original: `valueC8 = 22` at +0x431 immediately after `valueC4 = 600`
  in the mode-4 switch arm.
- Candidate: the same store at +0x522, after `valueC4 = 720` in the
  mode-2/gameMode-2 arm. The source has both required stores; VC7.1
  moves/merges one between the arms. The aligned fields and calls remain
  the same, but branch edges and the complete extent do not match.

Seven fixed-profile natural-C++ controls were tried privately. An explicit
shared `goto` increased the overlapping disagreement to 695. Inverting
case-2 or case-3 mode tests individually emitted 1,691 bytes; inverting
both emitted 1,688 bytes. Three case-arm reorderings emitted the same
1,682-byte object as the maintained baseline. None produced an exact
1,689-byte owner; all rejected source variants, compiles and full-byte
reports remain in `.analysis/gpt-web-gamemgr-native-front-20261109/`.

The shared type change was verified from current source with the original
VC7.1 `/Os` and no-`/Os` profiles. All **11** exact GameManager siblings,
plus `Supervisor::OnUpdate` in its affected carrier, pass strict complete
original PE/COFF equality: **12 physical owners, 3,987 bytes and 335
independently bound fields**. Forty compiler-private Supervisor labels
were renamed only after proving each actual same-section COFF definition
points at the identical original PE address. The focused gameplay boss
inspector still reports the same 1,682/694/7 nonexact frontier. Both
scoped gameplay and native-Front cleanup test groups pass.

This source checkpoint improves native object ownership without claiming a
new exact big boss, loosening an Oracle or running a full cold replay.

# TH09 boss re-investigation — 2026-10-31

This checkpoint records independent native IDA decompilation and rejected
source-shape experiments. The purpose is to keep the current hypotheses
falsifiable; none of these diagnostics grants byte exactness.

## RunEcl: independently rechecked opcodes 4 and 155

Native IDA decompilation of `0x004086C0` independently agrees with the
Ghidra-bound interpretation:

- Opcode 4 calls `ZunTimer::operator=(int)` (`0x00401500`) with the signed
  dword at instruction `+0x0C`, then advances by the dword at `+0x10`.
- Opcode 155 updates only bit 24 of the enemy primary flags using the masked
  XOR transition:

  `old ^ ((old ^ (wireByte << 24)) & 0x01000000)`

The original native expression is not evidence that a direct C++ spelling
will reproduce VC7.1's global register allocation. The current maintained
`memcpy`/helper spelling preserves the complete owner except for the seven
JUMP bytes and 23 timeout-opcode bytes. The behavior guards remain
non-crediting and cover 90 JUMP combinations and 3,072 timeout-bit cases.

Rejected direct-operand and direct masked-XOR source variants caused large
owner-wide changes (one IDA-shaped combined trial produced 2,060 ordinary
bytes, 242 relocation-field bytes, and 26 table bytes of difference). They
were restored rather than weakening the checker or retaining a regression.

## EnemyManager::OnUpdate: independently rechecked special attack branch

Native IDA decompilation of `0x00410730` confirms the special-attack flow:

1. Test the enemy secondary flag `0x1000`.
2. Call the timer's `HasTickedEvery(2)`; if true, spawn effect 9 at the enemy
   position through the side's EffectManager.
3. Compare the special-attack timer with manager threshold `+0x2AC44C`.
4. If elapsed, construct the bullet descriptor, copy enemy position, derive
   count from side character index `+0x20`, compute speed from the global
   value at `0x004A7E44`, set the remaining descriptor fields, and call the
   Etama primary spawn routine.
5. Deactivate and clean up the enemy.

The decompiler exposes the same observed data: enemy position at `+0x2D74`,
side state at manager `+0x320`, character index at side `+0x20`, and the
special threshold at manager `+0x2AC44C`. This corroborates behavior, not
original C++ names or translation-unit ownership.

A focused source trial replacing the descriptor's aggregate position copy
with three explicit scalar locals moved the native RunEcl call relocation in
the shared object from candidate offset `0x397` to `0x399`; the owner inspector
correctly rejected that layout. Replacing the duplicate manager view with the
public `EnemyManagerView` type, and testing a separate TU, did not reduce the
13-byte residual. Those experiments were restored.

## Restored live baseline

After all rejected trials, the live worktree is clean and only the two
boss-frontier diagnostics were rerun:

- `EclManager::RunEcl`: 15,564 bytes; 30 ordinary differences; zero relocation
  field and table differences. The only residual groups remain opcode 4 (7)
  and opcode 155 (23).
- `EnemyManagerView::OnUpdate`: 3,900 bytes; 13 ordinary differences; all 97
  relocation fields agree; 69 calls agree; reviewed control flow and all
  switch destinations agree.

Do not interpret a source spelling that changes the owner's physical extent,
relocation positions, or switch table as a local improvement. Rebuild only the
shared ECL/Enemy carrier and rerun these two owner inspectors after relevant
source changes; no all-owner cold replay is required.
